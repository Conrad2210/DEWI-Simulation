/*
 * UDPLamp.cc
 *
 *  Created on: 16 Apr 2014
 *      Author: Conrad Dandelski
 */

#include "UDPLamp.h"
#include "BurstMSG_m.h"
#include "UDPControlInfo_m.h"
#include "IPvXAddressResolver.h"
#include "InterfaceTableAccess.h"
#include <string.h>

#ifdef WITH_IPv4
#include "IRoutingTable.h"
#include "RoutingTableAccess.h"
#endif

Define_Module(UDPLamp);

int UDPLamp::nCounter;

simsignal_t UDPLamp::sstSentPkSignal = SIMSIGNAL_NULL;
simsignal_t UDPLamp::sstRcvdPkSignal = SIMSIGNAL_NULL;
simsignal_t UDPLamp::sstOutOfOrderPkSignal = SIMSIGNAL_NULL;
simsignal_t UDPLamp::sstDropPkSignal = SIMSIGNAL_NULL;
simsignal_t UDPLamp::sstReBroadcastPkSignal = SIMSIGNAL_NULL;
simsignal_t UDPLamp::sstDublicatedPkSignal = SIMSIGNAL_NULL;

UDPLamp::UDPLamp()
{
    pMessageLengthPar = NULL;
    pBurstDurationPar = NULL;
    pSleepDurationPar = NULL;
    pSendIntervalPar = NULL;
    mTimerNext = NULL;
    nOutputInterface = -1;
    nOutputInterfaceMulticastBroadcast.clear();
}

UDPLamp::~UDPLamp()
{
    cancelAndDelete(mTimerNext);
}

void UDPLamp::initialize(int stage)
{
    AppBase::initialize(stage);

    if (stage == 0)
    {
        nCounter = 0;
        nNumSent = 0;
        nNumDeleted = 0;
        nNumDoubleReceived = 0;
        nNumDuplicated = 0;
        nNumReceived = 0;
        nNumResend = 0;

        /*
         * Initialize Output Vectors
         */
        std::stringstream a;
        timeForCount = 0.0;
        a << getParentModule()->getIndex();
        //EndToEndDelay = new cOutVector(a.str().c_str());
        E2E = new DataVector(a.str(),"latency");
        Hop = new DataVector(a.str(),"Hops");

        stMethod = par("Method").stdstringValue();
        fPThreshold = par("PThreshold");

        stDelayLimit = par("delayLimit");
        stStartTime = par("startTime");
        stStopTime = par("stopTime");

        if (stStopTime >= SIMTIME_ZERO && stStopTime <= stStartTime)
            error("Invalid startTime/stopTime parameters");

        pMessageLengthPar = &par("messageLength");
        pBurstDurationPar = &par("burstDuration");
        pSleepDurationPar = &par("burstDuration");
        pSendIntervalPar = &par("sendInterval");
        pReBroadcastDelayPar = &par("reBroadcastDelay");
        stNextSleep = stStartTime;
        stNextBurst = stStartTime;
        stNextPkt = stStartTime;

        WATCH(nNumDeleted);
        WATCH(nNumDoubleReceived);
        WATCH(nNumDuplicated);
        WATCH(nNumReceived);
        WATCH(nNumSent);
        WATCH(nNumResend);

        nLocalPort = par("localPort");
        nDestPort = par("destPort");

        mTimerNext = new cMessage("UDPLamp");

        sstSentPkSignal = registerSignal("sentPk");
        sstRcvdPkSignal = registerSignal("rcvdPk");
        sstOutOfOrderPkSignal = registerSignal("outOfOrderPk");
        sstDropPkSignal = registerSignal("dropPk");
        sstReBroadcastPkSignal = registerSignal("ReBroadcastPk");
        sstDublicatedPkSignal = registerSignal("DublicatedPk");
    }
    else if(stage == 3)
    {
        Hop->registerVector();
        E2E->registerVector();
        center = check_and_cast<DataCenter *>(center->getModuleByPath("DataCenter"));
    }

}

cPacket *UDPLamp::createPacket()
{
    char msgName[32];
    sprintf(msgName, "UDPLampData-%d", nCounter++);
    long msgByteLength = pMessageLengthPar->longValue();
    cPacket *payload = new cPacket(msgName);
    payload->setByteLength(msgByteLength);
    payload->addPar("sourceId") = getId();
    payload->addPar("msgId") = nNumSent;

    return payload;
}

cPacket *UDPLamp::createBroadcastPacket()
{
    char msgName[32];
    sprintf(msgName, "UDPLamp-%d", nCounter++);
    long msgByteLength = pMessageLengthPar->longValue();
    cPacket *payload = new cPacket(msgName);
    payload->setByteLength(msgByteLength);
    payload->addPar("sourceId") = getId();
    payload->addPar("msgId") = nNumSent;
    payload->addPar("BroadcastPk") = par("resendBroadcast").boolValue();

    return payload;
}

cPacket *UDPLamp::createRePacket(cPacket *pk)
{
    char msgName[32];
    sprintf(msgName, "UDPLamp-%d", nCounter++);
    cPacket *payload = new cPacket(msgName);
    payload->setByteLength(pk->getByteLength());
    payload->addPar("sourceId") = getId();
    payload->addPar("msgId") = pk->hasPar("msgId");
    payload->addPar("BroadcastPk") = !pk->hasPar("BroadcastPk");

    // payload->addPar("Time") = simTime();

    return payload;
}

void UDPLamp::processStart()
{
    socket.setOutputGate(gate("udpOut"));
    socket.bind(nLocalPort);

    if (par("setBroadcast").boolValue())
        socket.setBroadcast(true);

    if (strcmp(par("outputInterface").stringValue(), "") != 0)
    {
        IInterfaceTable* ift = InterfaceTableAccess().get();
        InterfaceEntry *ie = ift->getInterfaceByName(par("outputInterface").stringValue());
        if (ie == NULL)
            throw cRuntimeError(this, "Invalid output interface name : %s", par("outputInterface").stringValue());
        nOutputInterface = ie->getInterfaceId();
    }

    const char *destAddrs = par("destAddresses");
    cStringTokenizer tokenizer(destAddrs);
    const char *token;

    nOutputInterfaceMulticastBroadcast.clear();
    if (strcmp(par("outputInterfaceMulticastBroadcast").stringValue(), "") != 0)
    {
        IInterfaceTable* ift = InterfaceTableAccess().get();
        const char *ports = par("outputInterfaceMulticastBroadcast");
        cStringTokenizer tokenizer(ports);
        const char *token;
        while ((token = tokenizer.nextToken()) != NULL)
        {
            if (strstr(token, "ALL") != NULL)
            {
                for (int i = 0; i < ift->getNumInterfaces(); i++)
                {
                    InterfaceEntry *ie = ift->getInterface(i);
                    if (ie->isLoopback())
                        continue;
                    if (ie == NULL)
                        throw cRuntimeError(this, "Invalid output interface name : %s", token);
                    nOutputInterfaceMulticastBroadcast.push_back(ie->getInterfaceId());
                }
            }
            else
            {
                InterfaceEntry *ie = ift->getInterfaceByName(token);
                if (ie == NULL)
                    throw cRuntimeError(this, "Invalid output interface name : %s", token);
                nOutputInterfaceMulticastBroadcast.push_back(ie->getInterfaceId());
            }
        }
    }

#ifdef WITH_IPv4
    IRoutingTable *rt = RoutingTableAccess().getIfExists();
#endif

    while ((token = tokenizer.nextToken()) != NULL)
    {
        if (strstr(token, "Broadcast") != NULL)
            ipDestAddresses.push_back(IPv4Address::ALLONES_ADDRESS);
        else
        {
            IPvXAddress addr = IPvXAddressResolver().resolve(token);
#ifdef WITH_IPv4
            if (rt && rt->isLocalAddress(addr.get4()))
                continue;
#endif
            ipDestAddresses.push_back(addr);
        }
    }

    stNextSleep = simTime();
    stNextBurst = simTime();
    stNextPkt = simTime();
    bActiveBurst = false;

    bIsSource = par("isSource");
    bIsSink = par("isSink");

    if (bIsSource)
    {
        bActiveBurst = true;
    }

    mTimerNext->setKind(SEND);
    processSend();

}

IPvXAddress UDPLamp::chooseDestAddr()
{
    int k = intrand(ipDestAddresses.size());
    return ipDestAddresses[k];
}

void UDPLamp::processSend()
{
    if (stStopTime < SIMTIME_ZERO || simTime() < stStopTime)
    {
        // send and reschedule next sending

        if (bIsSource) // if the node is a sink, don't generate messages
        {
            ipDestAddr = chooseDestAddr();
            if (ipDestAddr == IPv4Address::ALLONES_ADDRESS)
                generateBurst();
            else
                sendPacket();
        }

    }
}

void UDPLamp::processStop()
{
    socket.close();
}

void UDPLamp::handleMessageWhenUp(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        if (dynamic_cast<cPacket*>(msg))
        {
            IPvXAddress destAddr(IPv4Address::ALLONES_ADDRESS);
            sendBroadcast(destAddr, PK(msg));
        }
        else
        {
            switch (msg->getKind())
            {
                case START:
                    processStart();
                    break;
                case SEND:
                    processSend();
                    break;
                case STOP:
                    processStop();
                    break;
                default:
                    throw cRuntimeError("Invalid kind %d in self message", (int) msg->getKind());
            }
        }
    }
    else if (msg->getKind() == UDP_I_DATA)
    {
        //process incoming packet
        processPacket(PK(msg));
    }
    else if (msg->getKind() == UDP_I_ERROR)
    {
        EV << "Ignoring UDP error report\n";
        delete msg;
    }
    else
    {
        error("Unrecognized message (%s)%s", msg->getClassName(), msg->getName());
    }
    if (ev.isGUI())
    {
        char buf[40];
        sprintf(buf, "rcvd: %d pks/nsent: %d pks", nNumReceived, nNumSent);
        getDisplayString().setTagArg("t", 0, buf);
    }
}

void UDPLamp::processPacket(cPacket *pk)
{
    if (pk->getKind() == UDP_I_ERROR)
    {
        EV << "UDP error received\n";
        delete pk;
        return;
    }
    if (pk->hasPar("sourceId") && pk->hasPar("msgId"))
    {
        // duplicate control
        int moduleId = (int) pk->par("sourceId");
        int msgId = (int) pk->par("msgId");
        SourceSequence::iterator it = ssSourceSequence.find(moduleId);
        if (it != ssSourceSequence.end())
        {
            if (it->second >= msgId)
            {
                EV << "Packet double received: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
                emit(sstDublicatedPkSignal, pk);
                delete pk;
                nNumDuplicated++;
                return;
            }
            else
                it->second = msgId;
        }
        else
            ssSourceSequence[moduleId] = msgId;
    }

    if (stDelayLimit > 0)
    {
        if (simTime() - pk->getTimestamp() > stDelayLimit)
        {
            EV << "Old packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
            emit(sstDropPkSignal, pk);
            delete pk;
            nNumDeleted++;
            return;
        }
    }

    if (pk->hasPar("BroadcastPk"))
    {

        nNumReceived++;
        if(stMethod == "NONE")
        {
            EV << "Broadcast Packet received and resend packet\n";
            EV << "Received packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
            emit(sstRcvdPkSignal, pk);
            ipDestAddr = IPvXAddress("255.255.255.255");

            UDPDataIndication *ctrl = check_and_cast<UDPDataIndication *>(pk->removeControlInfo());
            if (ctrl->getDestAddr().get4() == IPv4Address::ALLONES_ADDRESS && par("resendBroadcast").boolValue())
            {

                BurstMSG *tmpBurstMsg = check_and_cast<BurstMSG *>(pk);
                Hop->record(tmpBurstMsg->getHopCount());
                tmpBurstMsg->setHopCount(tmpBurstMsg->getHopCount() + 1);
                endtoendDelay = simTime() - pk->getTimestamp();
                E2E->record(endtoendDelay.dbl());
                timeForCount = timeForCount + 1;
                nNumResend++;
                emit(sstReBroadcastPkSignal, tmpBurstMsg);
                scheduleAt(simTime() + par("reBroadcastDelay").doubleValue(), tmpBurstMsg);

            }
            delete ctrl;
        }
        else if( stMethod == "PROBABILISTIC")
        {
            if(((double) rand() / (RAND_MAX)) > fPThreshold)
            {
                EV << "Broadcast Packet received and resend packet\n";
                            EV << "Received packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
                            emit(sstRcvdPkSignal, pk);
                            ipDestAddr = IPvXAddress("255.255.255.255");

                            UDPDataIndication *ctrl = check_and_cast<UDPDataIndication *>(pk->removeControlInfo());
                            if (ctrl->getDestAddr().get4() == IPv4Address::ALLONES_ADDRESS && par("resendBroadcast").boolValue())
                            {

                                BurstMSG *tmpBurstMsg = check_and_cast<BurstMSG *>(pk);
                                Hop->record(tmpBurstMsg->getHopCount());
                                tmpBurstMsg->setHopCount(tmpBurstMsg->getHopCount() + 1);
                                endtoendDelay = simTime() - pk->getTimestamp();
                                E2E->record(endtoendDelay.dbl());
                                timeForCount = timeForCount + 1;
                                nNumResend++;
                                emit(sstReBroadcastPkSignal, tmpBurstMsg);
                                scheduleAt(simTime() + par("reBroadcastDelay").doubleValue(), tmpBurstMsg);

                            }
                            delete ctrl;
            }
            else
            {
                delete pk;
            }
        }

    }
    else
    {
        EV << "Received packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
        emit(sstRcvdPkSignal, pk);
        nNumReceived++;
        delete pk;
    }
}

void UDPLamp::generateBurst()
{
    simtime_t now = simTime();

    if (stNextPkt < now)
        stNextPkt = now;

    double sendInterval = pSendIntervalPar->doubleValue();
    if (sendInterval <= 0.0)
        throw cRuntimeError("The sendInterval parameter must be bigger than 0");
    stNextPkt += sendInterval;

//    if (bActiveBurst && stNextBurst <= now) // new burst
//    {
//        double burstDuration = pBurstDurationPar->doubleValue();
//        if (burstDuration < 0.0)
//            throw cRuntimeError("The burstDuration parameter mustn't be smaller than 0");
//        double sleepDuration = pSleepDurationPar->doubleValue();
//
//        if (burstDuration == 0.0)
//            bActiveBurst = false;
//        else
//        {
//            if (sleepDuration < 0.0)
//                throw cRuntimeError("The sleepDuration parameter mustn't be smaller than 0");
//            stNextSleep = now + burstDuration;
//            stNextBurst = stNextSleep + sleepDuration;
//        }
//
//    }

    ipDestAddr = IPvXAddress("255.255.255.255");

    cPacket *payload = createBroadcastPacket();
    payload->setTimestamp();
    emit(sstSentPkSignal, payload);

    // Check address type
    if (!sendBroadcast(ipDestAddr, payload))
        socket.sendTo(payload, ipDestAddr, nDestPort, nOutputInterface);

    nNumSent++;

    // Next timer
//    if (bActiveBurst && stNextPkt >= stNextSleep)
//        stNextPkt = stNextBurst;

    if (stStopTime >= SIMTIME_ZERO && stNextPkt >= stStopTime)
    {
        mTimerNext->setKind(STOP);
        stNextPkt = stStopTime;
    }
    scheduleAt(stNextPkt, mTimerNext);
}

void UDPLamp::sendPacket()
{
    cPacket *payload = createPacket();

    emit(sstSentPkSignal, payload);
    socket.sendTo(payload, ipDestAddr, nDestPort);
    nNumSent++;
}

void UDPLamp::finish()
{
//    ReceivedMessages->record(nNumReceived);
//    ResendMessages->record(nNumResend);
//    DoubleReceivedMessages->record(nNumDuplicated);
    std::stringstream data,type,index,name;
    data << nNumReceived;
    type << "received";
    index << getParentModule()->getIndex();
    name << ev.getConfigEx()->getActiveConfigName() << "_" << ev.getConfigEx()->getActiveRunNumber();
    center->recordScalar(data.str(),type.str(),index.str(),name.str());

    //Record ReSend messages
    data.str("");
    data << nNumResend;
    type.str("");
    type << "resend";
    center->recordScalar(data.str(),type.str(),index.str(),name.str());

    //Record Double Received Messages
    data.str("");
    data << nNumDuplicated;
    type.str("");
    type << "doublereceived";
    center->recordScalar(data.str(),type.str(),index.str(),name.str());

//    recordScalar("Total sent", nNumSent);
//    recordScalar("Total received", nNumReceived);
//    recordScalar("Total deleted", nNumDeleted);
//    recordScalar("Total double received", nNumDoubleReceived);
//    recordScalar("Total Resend Messages", nNumResend);
    AppBase::finish();
}

bool UDPLamp::startApp(IDoneCallback *doneCallback)
{
    simtime_t start = std::max(stStartTime, simTime());

    if ((stStopTime < SIMTIME_ZERO) || (start < stStopTime) || (start == stStopTime && stStartTime == stStopTime))
    {
        mTimerNext->setKind(START);
        scheduleAt(start, mTimerNext);
    }

    return true;
}

bool UDPLamp::stopApp(IDoneCallback *doneCallback)
{
    if (mTimerNext)
        cancelEvent(mTimerNext);
    bActiveBurst = false;
    //TODO if(socket.isOpened()) socket.close();
    return true;
}

bool UDPLamp::crashApp(IDoneCallback *doneCallback)
{
    if (mTimerNext)
        cancelEvent(mTimerNext);
    bActiveBurst = false;
    return true;
}

bool UDPLamp::sendBroadcast(const IPvXAddress &dest, cPacket *pkt)
{
    if (!nOutputInterfaceMulticastBroadcast.empty()
            && (dest.isMulticast() || (!dest.isIPv6() && dest.get4() == IPv4Address::ALLONES_ADDRESS)))
    {
        for (unsigned int i = 0; i < nOutputInterfaceMulticastBroadcast.size(); i++)
        {
            if (nOutputInterfaceMulticastBroadcast.size() - i > 1)
                socket.sendTo(pkt->dup(), dest, nDestPort, nOutputInterfaceMulticastBroadcast[i]);
            else
                socket.sendTo(pkt, dest, nDestPort, nOutputInterfaceMulticastBroadcast[i]);
        }
        return true;
    }
    return false;
}


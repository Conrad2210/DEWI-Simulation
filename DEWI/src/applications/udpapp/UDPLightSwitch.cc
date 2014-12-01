//
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
// Copyright (C) 2007 Universidad de Málaga
// Copyright (C) 2011 Zoltan Bojthe
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

#include "UDPLightSwitch.h"
#include "BurstMSG_m.h"
#include "UDPControlInfo_m.h"
#include "IPvXAddressResolver.h"
#include "InterfaceTableAccess.h"
#include <string>
#ifdef WITH_IPv4
#include "IRoutingTable.h"
#include "RoutingTableAccess.h"
#endif

#ifdef WITH_IPv6
#include "RoutingTable6.h"
#include "RoutingTable6Access.h"
#endif

EXECUTE_ON_STARTUP(
        cEnum *e = cEnum::find("ChooseDestAddrMode"); if (!e) enums.getInstance()->add(e = new cEnum("ChooseDestAddrMode")); e->insert(UDPLightSwitch::ONCE, "once"); e->insert(UDPLightSwitch::PER_BURST, "perBurst"); e->insert(UDPLightSwitch::PER_SEND, "perSend"););

Define_Module(UDPLightSwitch);

int UDPLightSwitch::counter;
int UDPLightSwitch::burstCounter;

simsignal_t UDPLightSwitch::sentPkSignal = SIMSIGNAL_NULL;
simsignal_t UDPLightSwitch::rcvdPkSignal = SIMSIGNAL_NULL;
simsignal_t UDPLightSwitch::outOfOrderPkSignal = SIMSIGNAL_NULL;
simsignal_t UDPLightSwitch::dropPkSignal = SIMSIGNAL_NULL;

UDPLightSwitch::UDPLightSwitch()
{
    messageLengthPar = NULL;
    burstDurationPar = NULL;
    sleepDurationPar = NULL;
    sendIntervalPar = NULL;
    timerNext = NULL;
    outputInterface = -1;
    outputInterfaceMulticastBroadcast.clear();
}

UDPLightSwitch::~UDPLightSwitch()
{
    cancelAndDelete(timerNext);
}

void UDPLightSwitch::initialize(int stage)
{
    AppBase::initialize(stage);

    if (stage == 0)
    {
        counter = 0;
        numSent = 0;
        numReceived = 0;
        numDeleted = 0;
        numDuplicated = 0;

        delayLimit = par("delayLimit");
        startTime = par("startTime");
        stopTime = par("stopTime");
        if (stopTime >= SIMTIME_ZERO && stopTime <= startTime)
            error("Invalid startTime/stopTime parameters");

        messageLengthPar = &par("messageLength");
        burstDurationPar = &par("burstDuration");
        sleepDurationPar = &par("sleepDuration");
        sendIntervalPar = &par("sendInterval");
        nextSleep = startTime;
        nextBurst = startTime;
        nextPkt = startTime;

        destAddrRNG = par("destAddrRNG");
        const char *addrModeStr = par("chooseDestAddrMode").stringValue();
        int addrMode = cEnum::get("ChooseDestAddrMode")->lookup(addrModeStr);
        if (addrMode == -1)
            throw cRuntimeError("Invalid chooseDestAddrMode: '%s'", addrModeStr);
        chooseDestAddrMode = (ChooseDestAddrMode) addrMode;

//        std::stringstream a;
//        a << ev.getConfigEx()->getActiveConfigName() << "_" << ev.getConfigEx()->getActiveRunNumber() << "_BurstsSent_"
//                << getParentModule()->getIndex();
//        burstSent = new cOutVector(a.str().c_str());
//        a.str("");
//        a << ev.getConfigEx()->getActiveConfigName() << "_" << ev.getConfigEx()->getActiveRunNumber() << "_MSGSent_"
//                << getParentModule()->getIndex();
//        msgSent = new cOutVector(a.str().c_str());
//        a.str("");

        WATCH(numSent);
        WATCH(numReceived);
        WATCH(numDeleted);
        WATCH(numDuplicated);

        localPort = par("localPort");
        destPort = par("destPort");

        timerNext = new cMessage("UDPLightSwitchTimer");

        sentPkSignal = registerSignal("sentPk");
        rcvdPkSignal = registerSignal("rcvdPk");
        outOfOrderPkSignal = registerSignal("outOfOrderPk");
        dropPkSignal = registerSignal("dropPk");
    }
    else if(stage == 3)
    {
        center = check_and_cast<DataCenter *>(center->getModuleByPath("DataCenter"));
    }
}

IPvXAddress UDPLightSwitch::chooseDestAddr()
{
    if (destAddresses.size() == 1)
        return destAddresses[0];

    int k = genk_intrand(destAddrRNG, destAddresses.size());
    return destAddresses[k];
}

cPacket *UDPLightSwitch::createPacket()
{
    char msgName[32];
    sprintf(msgName, "UDPLightSwitch-%d-%d", burstCounter, counter++);
    long msgByteLength = messageLengthPar->longValue();
    BurstMSG *payload = new BurstMSG(msgName);
    payload->setByteLength(msgByteLength);
    payload->addPar("sourceId") = getId();
    payload->addPar("msgId") = numSent;
    payload->addPar("BroadcastPk") = true;
    payload->setHopCount(0);

    return payload;
}

void UDPLightSwitch::processStart()
{
    socket.setOutputGate(gate("udpOut"));
    socket.bind(localPort);

    const char *destAddrs = par("destAddresses");
    cStringTokenizer tokenizer(destAddrs);
    const char *token;
    bool excludeLocalDestAddresses = par("excludeLocalDestAddresses").boolValue();
    if (par("setBroadcast").boolValue())
        socket.setBroadcast(true);

    if (strcmp(par("outputInterface").stringValue(), "") != 0)
    {
        IInterfaceTable* ift = InterfaceTableAccess().get();
        InterfaceEntry *ie = ift->getInterfaceByName(par("outputInterface").stringValue());
        if (ie == NULL)
            throw cRuntimeError(this, "Invalid output interface name : %s", par("outputInterface").stringValue());
        outputInterface = ie->getInterfaceId();
    }

    outputInterfaceMulticastBroadcast.clear();
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
                    outputInterfaceMulticastBroadcast.push_back(ie->getInterfaceId());
                }
            }
            else
            {
                InterfaceEntry *ie = ift->getInterfaceByName(token);
                if (ie == NULL)
                    throw cRuntimeError(this, "Invalid output interface name : %s", token);
                outputInterfaceMulticastBroadcast.push_back(ie->getInterfaceId());
            }
        }
    }

#ifdef WITH_IPv4
    IRoutingTable *rt = RoutingTableAccess().getIfExists();
#endif
#ifdef WITH_IPv6
    RoutingTable6 *rt6 = RoutingTable6Access().getIfExists();
#endif

    while ((token = tokenizer.nextToken()) != NULL)
    {
        if (strstr(token, "Broadcast") != NULL)
            destAddresses.push_back(IPv4Address::ALLONES_ADDRESS);
        else
        {
            IPvXAddress addr = IPvXAddressResolver().resolve(token);
#ifdef WITH_IPv4
            if (excludeLocalDestAddresses && rt && rt->isLocalAddress(addr.get4()))
                continue;
#endif
#ifdef WITH_IPv6
            if (excludeLocalDestAddresses && rt6 && rt6->isLocalAddress(addr.get6()))
                continue;
#endif
            destAddresses.push_back(addr);
        }
    }

    nextSleep = simTime();
    nextBurst = simTime();
    nextPkt = simTime();
    activeBurst = false;

    isSource = !destAddresses.empty();

    if (isSource)
    {
        if (chooseDestAddrMode == ONCE)
            destAddr = chooseDestAddr();

        activeBurst = true;
    }
    timerNext->setKind(SEND);
    processSend();
}

void UDPLightSwitch::processSend()
{
    if (stopTime < SIMTIME_ZERO || simTime() < stopTime)
    {
        // send and reschedule next sending
        if (isSource) // if the node is a sink, don't generate messages
            generateBurst();
    }
}

void UDPLightSwitch::processStop()
{
    socket.close();
}

void UDPLightSwitch::handleMessageWhenUp(cMessage *msg)
{
    if (msg->isSelfMessage())
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
    else if (msg->getKind() == UDP_I_DATA)
    {
        // process incoming packet
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
        sprintf(buf, "rcvd: %d pks\nsent: %d pks", numReceived, numSent);
        getDisplayString().setTagArg("t", 0, buf);
    }
}

void UDPLightSwitch::processPacket(cPacket *pk)
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

        SourceSequence::iterator it = sourceSequence.find(moduleId);
        if (it != sourceSequence.end())
        {
            if (it->second >= msgId)
            {
                EV << "Out of order packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
                emit(outOfOrderPkSignal, pk);
                delete pk;
                numDuplicated++;
                return;
            }
            else
                it->second = msgId;
        }
        else
            sourceSequence[moduleId] = msgId;
    }

    if (delayLimit > 0)
    {
        if (simTime() - pk->getTimestamp() > delayLimit)
        {
            EV << "Old packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
            emit(dropPkSignal, pk);
            delete pk;
            numDeleted++;
            return;
        }
    }

    EV << "Received packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
    emit(rcvdPkSignal, pk);
    numReceived++;
    delete pk;
}

void UDPLightSwitch::generateBurst()
{
    simtime_t now = simTime();

    if (nextPkt < now)
        nextPkt = now;

    double sendInterval = sendIntervalPar->doubleValue();
    if (sendInterval <= 0.0)
        throw cRuntimeError("The sendInterval parameter must be bigger than 0");
    nextPkt += sendInterval;

    if (activeBurst && nextBurst <= now) // new burst
    {
        burstCounter++;
        double burstDuration = burstDurationPar->doubleValue();
        if (burstDuration < 0.0)
            throw cRuntimeError("The burstDuration parameter mustn't be smaller than 0");
        double sleepDuration = sleepDurationPar->doubleValue();

        if (burstDuration == 0.0)
            activeBurst = false;
        else
        {
            if (sleepDuration < 0.0)
                throw cRuntimeError("The sleepDuration parameter mustn't be smaller than 0");
            nextSleep = now + burstDuration;
            nextBurst = nextSleep + sleepDuration;
        }

        if (chooseDestAddrMode == PER_BURST)
            destAddr = chooseDestAddr();
    }

    if (chooseDestAddrMode == PER_SEND)
        destAddr = chooseDestAddr();

    BurstMSG *payload = check_and_cast<BurstMSG *>(createPacket());
    payload->setTimestamp();
    emit(sentPkSignal, payload);

    // Check address type
    if (!sendBroadcast(destAddr, payload))
        socket.sendTo(payload, destAddr, destPort, outputInterface);

    numSent++;

    // Next timer
    if (activeBurst && nextPkt >= nextSleep)
    {
        nextPkt = nextBurst;
        counter = 0;
    }

    if (stopTime >= SIMTIME_ZERO && nextPkt >= stopTime)
    {
        timerNext->setKind(STOP);
        nextPkt = stopTime;
    }
    scheduleAt(nextPkt, timerNext);
}

void UDPLightSwitch::finish()
{
    if(!par("visualization").boolValue())
    {
        std::stringstream data,type,index,name;
        data << burstCounter;
        type << "burst";
        index << getParentModule()->getIndex();
        name << ev.getConfigEx()->getActiveConfigName() << "_" << ev.getConfigEx()->getActiveRunNumber();
        center->recordScalar(data.str(),type.str(),index.str(),name.str());


        //Record ReSend messages
        data.str("");
        data << numSent;
        type.str("");
        type << "msgsent";
        center->recordScalar(data.str(),type.str(),index.str(),name.str());
    }


}

bool UDPLightSwitch::startApp(IDoneCallback *doneCallback)
{
    simtime_t start = std::max(startTime, simTime());

    if ((stopTime < SIMTIME_ZERO) || (start < stopTime) || (start == stopTime && startTime == stopTime))
    {
        timerNext->setKind(START);
        scheduleAt(start, timerNext);
    }

    return true;
}

bool UDPLightSwitch::stopApp(IDoneCallback *doneCallback)
{
    if (timerNext)
        cancelEvent(timerNext);
    activeBurst = false;
    //TODO if(socket.isOpened()) socket.close();
    return true;
}

bool UDPLightSwitch::crashApp(IDoneCallback *doneCallback)
{
    if (timerNext)
        cancelEvent(timerNext);
    activeBurst = false;
    return true;
}

bool UDPLightSwitch::sendBroadcast(const IPvXAddress &dest, cPacket *pkt)
{
    if (!outputInterfaceMulticastBroadcast.empty()
            && (dest.isMulticast() || (!dest.isIPv6() && dest.get4() == IPv4Address::ALLONES_ADDRESS)))
    {
        for (unsigned int i = 0; i < outputInterfaceMulticastBroadcast.size(); i++)
        {
            if (outputInterfaceMulticastBroadcast.size() - i > 1)
                socket.sendTo(pkt->dup(), dest, destPort, outputInterfaceMulticastBroadcast[i]);
            else
                socket.sendTo(pkt, dest, destPort, outputInterfaceMulticastBroadcast[i]);
        }
        return true;
    }
    return false;
}

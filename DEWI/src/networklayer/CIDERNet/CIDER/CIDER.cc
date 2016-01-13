//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <CIDER/CIDER.h>
#include "CIDERControlInfo_m.h"
#include "CIDERFrame_m.h"

#include "IInterfaceTable.h"
#include "InterfaceTableAccess.h"
#include "macNeighborTableEntry.h"

Define_Module(CIDER);
CIDER::CIDER()
{
    // TODO Auto-generated constructor stub

}

CIDER::~CIDER()
{
    // TODO Auto-generated destructor stub
}

void CIDER::initialize(int stage)
{
    if (stage == 1)
    {
        networkLayerIn = findGate("networkLayerIn");
        networkLayerOut = findGate("networkLayerOut");
    }
    else if (stage == 2)
    {
        counterPing = 0;
        dOwnWeight = -1;
        nNodeDegree = 0;
        nClusterDegree = 0;
        dMeanRSSI = 0;
        w1 = par("w1").doubleValue();
        w2 = par("w2").doubleValue();
        w3 = par("w3").doubleValue();
        IInterfaceTable *ift = InterfaceTableAccess().getIfExists();
        myInterface = ift->getInterface(0);
        neighbourTable = MacNeighborTableAccess().getIfExists();
        uint64_t temp = myInterface->getMacAddress().getInt() - 0xaaa00feff000000;
        double startTime = (double) (temp % 0xFFFF) * 0.01;
        timerInitialPing = new cMessage("Ping", CIDERPingTimer);
        timerNeighUpdate = new cMessage("NeighbourUpdate", CIDERNeighUpdateTimer);
        timerWeight = new cMessage("WeightTimer", CIDERWeightTimer);
        timerFistCH = new cMessage("KeepAliveTimer", CIDERFirstCHTimer);
        scheduleAt(startTime, timerInitialPing);
        WATCH(dOwnWeight);
        WATCH(nNodeDegree);
        WATCH(nClusterDegree);
        WATCH(dMeanRSSI);
    }

}

void CIDER::finish()
{
    cancelAndDelete(timerInitialPing);
}

void CIDER::handleMessage(cMessage* msg)
{
    if (msg->isSelfMessage())
        handleSelfMessage(msg);
    else if (dynamic_cast<CIDERFrame *>(msg))
    {
        handleCIDERMessage(msg);
    }
    else
    {
        EV << "No Cider Message, delete msg";
        delete msg;
    }
}

void CIDER::handleSelfMessage(cMessage* msg)
{
    if (msg->getKind() == CIDERPingTimer)
    {
        if (counterPing < 2)
        {
            CIDERFrame *newFrame = new CIDERFrame("CIDERPing", CIDERPing);
            CIDERControlInfo *cntrl = new CIDERControlInfo("CiderContrl", CIDERCntrlInfo);
            newFrame->setControlInfo(cntrl);
            newFrame->setAddress(myInterface->getMacAddress());
            newFrame->setNodeDegree(neighbourTable->getNumNeighbors());
            for (int i = 0; i < neighbourTable->getNumNeighbors(); i++)
            {
                if (neighbourTable->getNeighborByPos(i)->isPosCluster())
                {
                    newFrame->setClusterDegree(newFrame->getClusterDegree() + 1);
                }
            }
            send(newFrame, networkLayerOut);
            counterPing++;
        }
    }
    else if (msg->getKind() == CIDERNeighUpdateTimer)
    {
        if (counterPing < 2)
        { //here also calc weight for each node;
            CIDERFrame *newFrame = new CIDERFrame("CIDERNeighUpdate", CIDERNeighUpdate);
            CIDERControlInfo *cntrl = new CIDERControlInfo("CiderContrl", CIDERCntrlInfo);
            newFrame->setControlInfo(cntrl);
            newFrame->setAddress(myInterface->getMacAddress());
            macVector temp;
            newFrame->setNodeDegree(neighbourTable->getNumNeighbors());
            for (int i = 0; i < neighbourTable->getNumNeighbors(); i++)
            {
                if (neighbourTable->getNeighborByPos(i)->isPosCluster())
                {
                    temp.push_back(neighbourTable->getNeighborByPos(i)->getExtendedAddress());
                    newFrame->setClusterDegree(newFrame->getClusterDegree() + 1);
                }
            }

            newFrame->setMacAddressesList(temp);
            send(newFrame, networkLayerOut);
            counterPing++;
        }
    }
    else if (msg->getKind() == CIDERWeightTimer)
    {
        //Calc weight metrics
        calcWeight();
        CIDERFrame *newFrame = new CIDERFrame("CIDERWeightMessage", CIDERWeightMessage);
        CIDERControlInfo *cntrl = new CIDERControlInfo("CiderContrl", CIDERCntrlInfo);
        newFrame->setControlInfo(cntrl);
        newFrame->setAddress(myInterface->getMacAddress());
        newFrame->setNodeDegree(neighbourTable->getNumNeighbors());
        for (int i = 0; i < neighbourTable->getNumNeighbors(); i++)
        {
            if (neighbourTable->getNeighborByPos(i)->isPosCluster())
            {
                newFrame->setClusterDegree(newFrame->getClusterDegree() + 1);
            }
        }
        newFrame->setWeight(dOwnWeight);
        send(newFrame, networkLayerOut);
        counterPing++;

    }
    else if (msg->getKind() == CIDERFirstCHTimer)
    {

        for (int i = 0; i < neighbourTable->getNumNeighbors(); i++)
        {
            int temp = getParentModule()->getIndex();
                    if (temp == 28)
                    {
                        int p = 0;
                        p++;
                    }
                    double NeighWeight = neighbourTable->getNeighborByPos(i)->getWeight();
            if (NeighWeight > dOwnWeight)
                return;
        }
        cDisplayString* parentDisp = &getParentModule()->getDisplayString();
        cDisplayString* tempStr = new cDisplayString();

        tempStr->parse("b=1.5,1.5,oval,orange;i=device/accesspoint");

        parentDisp->updateWith(*tempStr);
//        if (timerFistCH->isScheduled())
//        {
//            cancelEvent(timerFistCH);
//        }
//        CIDERFrame *newFrame = new CIDERFrame("CIDERKeepAliveMessage", CIDERFirstCH);
//        CIDERControlInfo *cntrl = new CIDERControlInfo("CiderContrl", CIDERCntrlInfo);
//        newFrame->setControlInfo(cntrl);
//        send(newFrame, networkLayerOut);
//
//        scheduleAt(simTime() + 30, timerFistCH);
    }
}

void CIDER::handleCIDERMessage(cMessage* msg)
{
    if (msg->getKind() == CIDERPing)
    {
        if (timerNeighUpdate->isScheduled())
        {
            cancelEvent(timerNeighUpdate);
        }
        CIDERFrame *recFrame = check_and_cast<CIDERFrame *>(msg);

        macNeighborTableEntry *entry = new macNeighborTableEntry();
        entry->setExtendedAddress(recFrame->getAddress());
        entry->setCurTxPw(recFrame->getTxPower());
        entry->setRssi(recFrame->getRxPower());
        entry->setNodeDegree(recFrame->getNodeDegree());
        entry->setClusterDegree(recFrame->getClusterDegree());
        nNodeDegree++;
        if (entry->getRssidBm() > -80.0)
        {
            entry->setPosCluster(true);
            nClusterDegree++;
        }
        entry->setLastPktReceived(simTime());
        neighbourTable->addNeighbor(entry);
        myMACList.push_back(recFrame->getAddress());
        uint64_t temp = myInterface->getMacAddress().getInt() - 0xaaa00feff000000;
        double delay = (double) (temp % 0xFFFF) * 0.01 + 10;
        scheduleAt(delay, timerNeighUpdate);
        delete recFrame;
    }
    else if (msg->getKind() == CIDERNeighUpdate)
    {
        if (timerWeight->isScheduled())
        {
            cancelEvent(timerWeight);
        }
        CIDERFrame *recFrame = check_and_cast<CIDERFrame *>(msg);
        macNeighborTableEntry *entry = neighbourTable->getNeighborByEAddr(recFrame->getAddress());

        if (entry != NULL)
        {
            macVector temp = recFrame->getMacAddressesList();
            entry->setNewCoverage(compareMACList(temp, myMACList, recFrame->getAddress()));
            entry->setCurTxPw(recFrame->getTxPower());
            entry->setRssi(recFrame->getRxPower());
            entry->setNodeDegree(recFrame->getNodeDegree());
            entry->setClusterDegree(recFrame->getClusterDegree());
            entry->setLastPktReceived(simTime());
            neighbourTable->editNeighbor(entry);
        }
        uint64_t temp = myInterface->getMacAddress().getInt() - 0xaaa00feff000000;
        double delay = (double) (temp % 0xFFFF) * 0.01 + 20;
        scheduleAt(delay, timerWeight);
        delete recFrame;
    }
    else if (msg->getKind() == CIDERWeightMessage)
    {
        if (timerFistCH->isScheduled())
        {
            cancelEvent(timerFistCH);
        }
        CIDERFrame *recFrame = check_and_cast<CIDERFrame *>(msg);
        macNeighborTableEntry *entry = neighbourTable->getNeighborByEAddr(recFrame->getAddress());

        if (entry != NULL)
        {
            entry->setCurTxPw(recFrame->getTxPower());
            entry->setRssi(recFrame->getRxPower());
            entry->setNodeDegree(recFrame->getNodeDegree());
            entry->setClusterDegree(recFrame->getClusterDegree());
            entry->setWeight(recFrame->getWeight());
            entry->setLastPktReceived(simTime());
            neighbourTable->editNeighbor(entry);
        }
        uint64_t temp = myInterface->getMacAddress().getInt() - 0xaaa00feff000000;
        double delay = (double) (temp % 0xFFFF) * 0.01 + 30;
        scheduleAt(delay, timerFistCH);
        delete recFrame;
    }
    else if (msg->getKind() == CIDERFirstCH)
    {
        CIDERFrame *recFrame = check_and_cast<CIDERFrame *>(msg);
//        macNeighborTableEntry *entry = neighbourTable->getNeighborByEAddr(recFrame->getAddress());
//
//        if (entry != NULL)
//        {
//            entry->setLastPktReceived(simTime());
//        }
        delete recFrame;
    }

    msg = NULL;
}

void CIDER::calcWeight()
{

    double allRSSI = 0;

    for (int i = 0; i < neighbourTable->getNumNeighbors(); i++)
    {
        allRSSI = allRSSI + (neighbourTable->getNeighborByPos(i)->getRssidBm());

    }
    dMeanRSSI = allRSSI / nNodeDegree;

    dOwnWeight = w1 * nNodeDegree + w2 * nClusterDegree + w3 * dMeanRSSI;
}

//returns number of elements (of vec1) not included in vec2
int CIDER::compareMACList(macVector vec1, macVector vec2, MACAddress sender)
{

    int nCount = 0;
    for (int i = 0; i < (int) vec1.size(); i++)
    {
        bool isIncluded = false;
        for (int k = 0; k < (int) vec2.size(); k++)
        {

            if (vec1.at(i) == vec2.at(k) && vec1.at(i) != sender
                    && neighbourTable->getNeighborByEAddr(vec2.at(k))->isPosCluster())
            {
                isIncluded = true;
                break;
            }
        }

        if (!isIncluded)
            nCount++;
    }

    return nCount;

}

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
#include <limits>
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
        w4 = par("w4").doubleValue();
        w5 = par("w5").doubleValue();
        parent = MACAddress::UNSPECIFIED_ADDRESS;
        IInterfaceTable *ift = InterfaceTableAccess().getIfExists();
        myInterface = ift->getInterface(0);
        neighbourTable = MacNeighborTableAccess().getIfExists();
        uint64_t temp = myInterface->getMacAddress().getInt() - 0xaaa00feff000000;
        double startTime = (double) (temp % 0xFFFF) * 0.01;
        timerInitialPing = new cMessage("Ping", CIDERPingTimer);
        timerNeighUpdate = new cMessage("NeighbourUpdate", CIDERNeighUpdateTimer);
        timerWeight = new cMessage("WeightTimer", CIDERWeightTimer);
        timerCompWeight = new cMessage("CompareWeightTimer", CIDERCompareWeightTimer);
        timerCompetition = new cMessage("CompetitionTimer", CIDERFirstCHCompetitionTimer);
        timerAdvert = new cMessage("AdvertTimer", CIDERCHAdvertTimer);
        timerCalcWeight = new cMessage("Calc Weight Timer", CIDERSecondCHCalcTimer);
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
            newFrame->setSrcAddress(myInterface->getMacAddress());
            newFrame->setDstAddress(MACAddress::BROADCAST_ADDRESS);
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
            newFrame->setSrcAddress(myInterface->getMacAddress());
            newFrame->setDstAddress(MACAddress::BROADCAST_ADDRESS);
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
        newFrame->setSrcAddress(myInterface->getMacAddress());
        newFrame->setDstAddress(MACAddress::BROADCAST_ADDRESS);
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
    else if (msg->getKind() == CIDERCompareWeightTimer)
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

        double waitTime = uniform(0.5, 1.5);

        scheduleAt(simTime() + waitTime, timerCompetition);
    }
    else if (msg->getKind() == CIDERFirstCHCompetitionTimer)
    {
        CIDERFrame *newFrame = new CIDERFrame("CIDERFirstCHCompetition", CIDERFirstCHCompetition);
        CIDERControlInfo *cntrl = new CIDERControlInfo("CiderContrl", CIDERCntrlInfo);
        newFrame->setControlInfo(cntrl);
        newFrame->setSrcAddress(myInterface->getMacAddress());
        newFrame->setDstAddress(MACAddress::BROADCAST_ADDRESS);
        newFrame->setWeight(dOwnWeight);
        send(newFrame, networkLayerOut);

        double waitTime = uniform(0.5, 1.5);

        scheduleAt(simTime() + waitTime, timerAdvert);
    }
    else if (msg->getKind() == CIDERCHAdvertTimer)
    {
        CIDERFrame *newFrame = new CIDERFrame("CIDERFirstCHAdvert", CIDERCHAdvert);
        CIDERControlInfo *cntrl = new CIDERControlInfo("CiderContrl", CIDERCntrlInfo);
        newFrame->setControlInfo(cntrl);
        newFrame->setSrcAddress(myInterface->getMacAddress());
        newFrame->setDstAddress(MACAddress::BROADCAST_ADDRESS);
        newFrame->setWeight(dOwnWeight);
        send(newFrame, networkLayerOut);

        for (int i = 0; i < neighbourTable->getNumNeighbors(); i++)
        {
            if (neighbourTable->getNeighborByPos(i)->isPosCluster())
            {
                neighbourTable->getNeighborByPos(i)->isMyCS(true);
                neighbourTable->getNeighborByPos(i)->setAssigndTo(myInterface->getMacAddress());
                myCS.push_back(neighbourTable->getNeighborByPos(i)->getExtendedAddress());
            }
        }
        if(parent == MACAddress::UNSPECIFIED_ADDRESS)
            scheduleAt(simTime() + 0.2, timerCalcWeight);
    }
    else if (msg->getKind() == CIDERSecondCHCalcTimer)
    {
        double tempWeight = -DBL_MAX;
        int index = -1;
        for (int i = 0; i < neighbourTable->getNumNeighbors(); i++)
        {
            macNeighborTableEntry *entry = neighbourTable->getNeighborByPos(i);
            if (entry->isPosCluster() == false)
            {
                entry->setWeightSecond(w4 * entry->getNewCoverage() + w5 * entry->getRssidBm());
                if (entry->getWeightSecond() > tempWeight)
                {
                    tempWeight = entry->getWeightSecond();
                    index = entry->getNeighborId();
                }
            }
        }

        CIDERFrame *newFrame = new CIDERFrame("CIDERSecondCHElect", CIDERSecondCHElect);
        CIDERControlInfo *cntrl = new CIDERControlInfo("CiderContrl", CIDERCntrlInfo);
        newFrame->setControlInfo(cntrl);
        newFrame->setSrcAddress(myInterface->getMacAddress());
        newFrame->setDstAddress(neighbourTable->getNeighborById(index)->getExtendedAddress());
        newFrame->setMacAddressesList(myCS);
        send(newFrame, networkLayerOut);

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
        entry->setExtendedAddress(recFrame->getSrcAddress());
        entry->setCurTxPw(recFrame->getTxPower());
        entry->setRssi(recFrame->getRxPower());
        entry->setNodeDegree(recFrame->getNodeDegree());
        entry->setClusterDegree(recFrame->getClusterDegree());
        nNodeDegree++;
        if (entry->getRssidBm() > -80.0)
        {
            entry->setPosCluster(true);
            myMACList.push_back(entry->getExtendedAddress());
            nClusterDegree++;
        }
        entry->setLastPktReceived(simTime());
        neighbourTable->addNeighbor(entry);
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
        macNeighborTableEntry *entry = neighbourTable->getNeighborByEAddr(recFrame->getSrcAddress());

        if (entry != NULL)
        {
            macVector temp = recFrame->getMacAddressesList();
            entry->setNewCoverage(compareMACList(temp, myMACList, recFrame->getSrcAddress()));
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
        if (timerCompWeight->isScheduled())
        {
            cancelEvent(timerCompWeight);
        }
        CIDERFrame *recFrame = check_and_cast<CIDERFrame *>(msg);
        macNeighborTableEntry *entry = neighbourTable->getNeighborByEAddr(recFrame->getSrcAddress());

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
        scheduleAt(delay, timerCompWeight);
        delete recFrame;
    }
    else if (msg->getKind() == CIDERFirstCHCompetition)
    {
        CIDERFrame *recFrame = check_and_cast<CIDERFrame *>(msg);
        if (recFrame->getWeight() == dOwnWeight)
        {
            if (timerCompetition->isScheduled())
            {
                cancelEvent(timerCompetition);
            }
            if (timerAdvert->isScheduled())
            {
                cancelEvent(timerAdvert);
            }
            cDisplayString* parentDisp = &getParentModule()->getDisplayString();
            cDisplayString* tempStr = new cDisplayString();

            tempStr->parse("b=1.5,1.5,oval,red;i=device/bulb");

            parentDisp->updateWith(*tempStr);
        }
        delete recFrame;
    }
    else if (msg->getKind() == CIDERCHAdvert)
    {
        CIDERFrame *recFrame = check_and_cast<CIDERFrame *>(msg);

        if (mWTodBm(recFrame->getRxPower()) > -80)
        {

            macNeighborTableEntry *entry = neighbourTable->getNeighborByEAddr(recFrame->getSrcAddress());
            if (entry->getAssigndTo() == MACAddress::UNSPECIFIED_ADDRESS)
            {
                if (entry != NULL)
                {
                    entry->isMyCH(true);
                    neighbourTable->editNeighbor(entry);
                    parent = entry->getExtendedAddress();
                }
                cDisplayString* parentDisp = &getParentModule()->getDisplayString();
                cDisplayString* tempStr = new cDisplayString();

                tempStr->parse("b=1.5,1.5,oval,orange;i=device/bulb");

                parentDisp->updateWith(*tempStr);
            }
        }
        delete recFrame;
    }
    else if (msg->getKind() == CIDERSecondCHElect)
    {
        CIDERFrame *recFrame = check_and_cast<CIDERFrame *>(msg);

        macNeighborTableEntry *entry = neighbourTable->getNeighborByEAddr(recFrame->getSrcAddress());

        if (entry != NULL)
        {
            entry->isMyCH(true);
            neighbourTable->editNeighbor(entry);
            parent = entry->getExtendedAddress();
        }
        macVector temp = recFrame->getMacAddressesList();
        for (int i = 0; i < neighbourTable->getNumNeighbors(); i++)
        {
            entry = neighbourTable->getNeighborByPos(i);
            for (int k = 0; k < (int) temp.size(); k++)
            {
                if (entry->getExtendedAddress().compareTo(temp.at(k)) == 0)
                {
                    entry->setAssigndTo(recFrame->getSrcAddress());
                }
            }
        }
        cDisplayString* parentDisp = &getParentModule()->getDisplayString();
        cDisplayString* tempStr = new cDisplayString();

        tempStr->parse("b=1.5,1.5,oval,blue;i=device/accesspoint");

        parentDisp->updateWith(*tempStr);

        scheduleAt(simTime() + 0.2, timerAdvert);

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

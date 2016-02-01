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
#include "common.h"
Define_Module(CIDER);

const static int numberOfBytes = 3;
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
        LPDevice = par("LPDevice").boolValue();
        parent = NULL;
        updatedisplay();
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
        timerElectChildCH = new cMessage("ElectChildCHTimer", CIDERElectChildCHTimer);
        timerUpdateParent = new cMessage("UpdateParentTimer", CIDERParentUpdateTimer);
        timerCoverageUpdate = new cMessage("CoverageUpdateTimer", CIDERCoverageUpdateTimer);
        timerDelectCH = new cMessage("DelectCHTimer", CIDERDelectCHTimer);
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
    cancelAndDelete(timerNeighUpdate);
    cancelAndDelete(timerWeight);
    cancelAndDelete(timerCompWeight);
    cancelAndDelete(timerCompetition);
    cancelAndDelete(timerAdvert);
    cancelAndDelete(timerElectChildCH);
    cancelAndDelete(timerUpdateParent);
    cancelAndDelete(timerCoverageUpdate);
    cancelAndDelete(timerDelectCH);
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
                    temp.push_back(
                            neighbourTable->getNeighborByPos(i)->getExtendedAddress().getLastKBytes(numberOfBytes));
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
        double waitTime = 0.0;
        if (nNodeDegree / 100.0 < 0.5)
            waitTime = uniform(0.5, 1.5);
        else
            waitTime = uniform(nNodeDegree / 100.0, 2 * (nNodeDegree / 100.0));

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
                neighbourTable->getNeighborByPos(i)->setAssignedTo(
                        myInterface->getMacAddress().getLastKBytes(numberOfBytes));
                if (neighbourTable->getNeighborByPos(i)->getAssignedTo() != -1)
                    assignedCS.push_back(
                            neighbourTable->getNeighborByPos(i)->getExtendedAddress().getLastKBytes(numberOfBytes));

            }
        }
        if (parent == NULL)
            scheduleAt(simTime() + 0.2, timerElectChildCH);
        else
            scheduleAt(simTime() + 0.05, timerUpdateParent);
    }
    else if (msg->getKind() == CIDERElectChildCHTimer)
    {
        double tempWeight = -DBL_MAX;
        int index = -1;
        for (int i = 0; i < neighbourTable->getNumNeighbors(); i++)
        {
            macNeighborTableEntry *entry = neighbourTable->getNeighborByPos(i);
            if (entry->isPosCluster() == false && entry->getAssignedTo() == -1)
            {
                entry->setWeightSecond(w4 * entry->getNewCoverage() + w5 * entry->getRssidBm());
                if (entry->getWeightSecond() > tempWeight)
                {
                    tempWeight = entry->getWeightSecond();
                    index = entry->getNeighborId();
                }
            }
        }
        if (index != -1)
        {
            CIDERFrame *newFrame = new CIDERFrame("CIDERSecondCHElect", CIDERElectChildCH);
            CIDERControlInfo *cntrl = new CIDERControlInfo("CiderContrl", CIDERCntrlInfo);
            newFrame->setControlInfo(cntrl);
            newFrame->setSrcAddress(myInterface->getMacAddress());
            newFrame->setDstAddress(neighbourTable->getNeighborById(index)->getExtendedAddress());
            newFrame->setMacAddressesList(assignedCS);
            send(newFrame, networkLayerOut);
            neighbourTable->getNeighborById(index)->setAssignedTo(
                    myInterface->getMacAddress().getLastKBytes(numberOfBytes));
        }
        else
        {
            scheduleAt(simTime() + 0.2, timerCoverageUpdate);
        }

    }
    else if (msg->getKind() == CIDERParentUpdateTimer)
    {
        CIDERFrame *newFrame = new CIDERFrame("CIDERSecondCHElect", CIDERParentUpdate);
        CIDERControlInfo *cntrl = new CIDERControlInfo("CiderContrl", CIDERCntrlInfo);
        newFrame->setControlInfo(cntrl);
        newFrame->setSrcAddress(myInterface->getMacAddress());
        newFrame->setDstAddress(parent->getExtendedAddress());
        newFrame->setMacAddressesList(assignedCS);
        send(newFrame, networkLayerOut);
    }
    else if (msg->getKind() == CIDERCoverageUpdateTimer)
    {
        CIDERFrame *newFrame = new CIDERFrame("CoverageUpdate", CIDERCoverageUpdate);
        CIDERControlInfo *cntrl = new CIDERControlInfo("CiderContrl", CIDERCntrlInfo);
        newFrame->setControlInfo(cntrl);
        newFrame->setSrcAddress(myInterface->getMacAddress());
        newFrame->setDstAddress(MACAddress::BROADCAST_ADDRESS);
        newFrame->setMacAddressesList(assignedCS);
        send(newFrame, networkLayerOut);
    }
    else if (msg->getKind() == CIDERDelectCHTimer)
    {
        CIDERFrame *newFrame = new CIDERFrame("DelectCH", CIDERDelectCH);
        CIDERControlInfo *cntrl = new CIDERControlInfo("CiderContrl", CIDERCntrlInfo);
        newFrame->setControlInfo(cntrl);
        newFrame->setSrcAddress(myInterface->getMacAddress());
        newFrame->setDstAddress(delectAddr);
        send(newFrame, networkLayerOut);
    }
    else if(msg->getKind() == CIDERDelectCSTimer)
    {
        CIDERFrame *newFrame = new CIDERFrame("DelectCH", CIDERDelectCH);
        CIDERControlInfo *cntrl = new CIDERControlInfo("CiderContrl", CIDERCntrlInfo);
        newFrame->setControlInfo(cntrl);
        newFrame->setSrcAddress(myInterface->getMacAddress());
        newFrame->setDstAddress(MACAddress::BROADCAST_ADDRESS);

        for(int i = 0; i < (int)assignedCS.size();i++)
        {
            for(int k = 0; k = neighbourTable->getNumNeighbors(); k++)
            {
                macNeighborTableEntry *entry = neighbourTable->getNeighborByPos(k);
                if(entry->getAssignedTo() == assignedCS.at(i))
                {
                    entry->setAssignedTo(-1);
                    break;
                }
            }
        }

        scheduleAt(simTime() + 0.2, timerDelectCHRep);

    }
    else if (msg->getKind() == CIDERDelectCHRepTimer)
        {
            CIDERFrame *newFrame = new CIDERFrame("DelectCHRep", CIDERDelectCHRep);
            CIDERControlInfo *cntrl = new CIDERControlInfo("CiderContrl", CIDERCntrlInfo);
            newFrame->setControlInfo(cntrl);
            newFrame->setSrcAddress(myInterface->getMacAddress());
            newFrame->setDstAddress(parent->getExtendedAddress());
            newFrame->setMacAddressesList(assignedCS);
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
            myMACList.push_back(entry->getExtendedAddress().getLastKBytes(numberOfBytes));
            nClusterDegree++;
        }
        entry->setLastPktReceived(simTime());
        neighbourTable->addNeighbor(entry);
        uint64_t temp = myInterface->getMacAddress().getInt() - 0xaaa00feff000000;
        double delay = uniform(simTime().dbl() + 1,
                simTime().dbl() + 1 + (double) neighbourTable->getNumNeighbors() / 10.0);
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

        if (mWTodBm(recFrame->getRxPower()) > -80 && parent == NULL)
        {

            macNeighborTableEntry *entry = neighbourTable->getNeighborByEAddr(recFrame->getSrcAddress());
            if (entry->getAssignedTo() == -1)
            {
                if (entry != NULL)
                {
                    entry->isMyCH(true);
                    neighbourTable->editNeighbor(entry);
                    parent = entry;
                    updatedisplay();
                }
                cDisplayString* parentDisp = &getParentModule()->getDisplayString();
                cDisplayString* tempStr = new cDisplayString();

                tempStr->parse("b=1.5,1.5,oval,orange;i=device/bulb");

                parentDisp->updateWith(*tempStr);
            }
        }
        delete recFrame;
    }
    else if (msg->getKind() == CIDERElectChildCH)
    {
        CIDERFrame *recFrame = check_and_cast<CIDERFrame *>(msg);

        macNeighborTableEntry *entry = neighbourTable->getNeighborByEAddr(recFrame->getSrcAddress());

        if (entry != NULL)
        {
            entry->isMyCH(true);
            neighbourTable->editNeighbor(entry);
            parent = entry;
            updatedisplay();
        }
        macVector temp = recFrame->getMacAddressesList();
        for (int i = 0; i < neighbourTable->getNumNeighbors(); i++)
        {
            entry = neighbourTable->getNeighborByPos(i);
            for (int k = 0; k < (int) temp.size(); k++)
            {
                if (entry->getExtendedAddress().compareTo(temp.at(k), numberOfBytes) == 0)
                {
                    entry->setAssignedTo(recFrame->getSrcAddress().getLastKBytes(numberOfBytes));
                    entry->setPosCluster(false);
                    break;
                }
            }
        }
        cDisplayString* parentDisp = &getParentModule()->getDisplayString();
        cDisplayString* tempStr = new cDisplayString();

        tempStr->parse("b=1.5,1.5,oval,orange;i=device/accesspoint");

        parentDisp->updateWith(*tempStr);

        scheduleAt(simTime() + 0.2, timerAdvert);

        delete recFrame;
    }
    else if (msg->getKind() == CIDERParentUpdate)
    {
        CIDERFrame *recFrame = check_and_cast<CIDERFrame *>(msg);
        macNeighborTableEntry *entry;
        macVector temp = recFrame->getMacAddressesList();
        int tempSize = temp.size();
        int assignedCSBefore = assignedCS.size();
        while (assignedCS.size() != 0)
            assignedCS.erase(assignedCS.begin());

        if (tempSize < 5)
        {
            for (int i = 0; i < neighbourTable->getNumNeighbors(); i++)
            {
                entry = neighbourTable->getNeighborByPos(i);
                for (int k = 0; k < (int) temp.size(); k++)
                {
                    if (entry->getExtendedAddress().compareTo(temp.at(k), numberOfBytes) == 0)
                    {
                        entry->setAssignedTo(recFrame->getSrcAddress().getLastKBytes(numberOfBytes));
                    }

                }
                if (entry->getAssignedTo() != -1)
                {
                    assignedCS.push_back(entry->getExtendedAddress().getLastKBytes(numberOfBytes));
                }
            }
            int assignedCSafter = assignedCS.size();
            scheduleAt(simTime() + 0.2, timerElectChildCH);
        }
        else
        {
            delectAddr = recFrame->getSrcAddress();
            //tell the note it become a cs;
            scheduleAt(simTime() + 0.2, timerDelectCH);

        }
        delete recFrame;
    }
    else if (msg->getKind() == CIDERCoverageUpdate)
    {
        CIDERFrame *recFrame = check_and_cast<CIDERFrame *>(msg);
        macNeighborTableEntry *entry;
        macVector temp = recFrame->getMacAddressesList();
        int tempSize = temp.size();
        for (int i = 0; i < neighbourTable->getNumNeighbors(); i++)
        {
            entry = neighbourTable->getNeighborByPos(i);
            for (int k = 0; k < (int) temp.size(); k++)
            {
                if (entry->getExtendedAddress().compareTo(temp.at(k), numberOfBytes) == 0
                        && entry->getAssignedTo() != myInterface->getMacAddress().getLastKBytes(numberOfBytes))
                {
                    entry->setAssignedTo(recFrame->getSrcAddress().getLastKBytes(numberOfBytes));
                }

            }
        }
    }
    else if (msg->getKind() == CIDERDelectCH)
    {
        CIDERFrame *recFrame = check_and_cast<CIDERFrame *>(msg);
        parent = neighbourTable->getNeighborByLastBytes(recFrame->getSrcAddress().getLastKBytes(numberOfBytes),numberOfBytes);
        cDisplayString* parentDisp = &getParentModule()->getDisplayString();
        cDisplayString* tempStr = new cDisplayString();

        tempStr->parse("b=1.5,1.5,oval,orange;i=device/bulb");

        parentDisp->updateWith(*tempStr);
        updatedisplay();
        scheduleAt(simTime() + 0.2, timerDelectCS);

    }
    else if (msg->getKind() == CIDERDelectCHRep)
    {
        delectAddr = MACAddress::UNSPECIFIED_ADDRESS;

        CIDERFrame *recFrame = check_and_cast<CIDERFrame *>(msg);
        macNeighborTableEntry *entry;
        entry = neighbourTable->getNeighborByEAddr(recFrame->getSrcAddress());
        entry->setAssignedTo(myInterface->getMacAddress().getLastKBytes(numberOfBytes));
        scheduleAt(simTime() + 0.2, timerElectChildCH);

    }
    else if (msg->getKind() == CIDERDelectCS)
    {
        CIDERFrame *recFrame = check_and_cast<CIDERFrame *>(msg);
        macNeighborTableEntry *entry;

        if (recFrame->getSrcAddress().getLastKBytes(numberOfBytes) == parent->getExtendedAddress().getLastKBytes(numberOfBytes));
        {
            parent = NULL;
            cDisplayString* parentDisp = &getParentModule()->getDisplayString();
            cDisplayString* tempStr = new cDisplayString();

            tempStr->parse("b=1.5,1.5,oval,red;i=device/bulb");

            parentDisp->updateWith(*tempStr);
        }

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

            if (vec1.at(i) == vec2.at(k) && vec1.at(i) != sender.getLastKBytes(numberOfBytes)
                    && neighbourTable->getNeighborByLastBytes(vec2.at(k), numberOfBytes)->isPosCluster())
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
void CIDER::updatedisplay()
{
    cDisplayString* parentDisp = &getParentModule()->getDisplayString();

    char buf[40];
    if (parent != NULL)
        sprintf(buf, intToString(parent->getExtendedAddress().getLastKBytes(numberOfBytes)).c_str());
    else
        sprintf(buf, "-1");
    parentDisp->setTagArg("t", 0, buf);
}

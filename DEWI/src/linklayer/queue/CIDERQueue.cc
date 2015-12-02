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

#include <CIDERQueue.h>
#include "Ieee802Ctrl_m.h"
#include "Ieee802154eFrame_m.h"
Define_Module(CIDERQueue);

CIDERQueue::CIDERQueue() {
    // TODO Auto-generated constructor stub

}

CIDERQueue::~CIDERQueue() {
    // TODO Auto-generated destructor stub
}

void CIDERQueue::initialize() {
    PassiveQueueBase::initialize();
    rec_delMsg = par("rec_delMsg").boolValue();
    queue.setName(par("queueName"));

    //statistics
//    queueLengthSignal = registerSignal("queueLength");
//    emit(queueLengthSignal, queue.length());

    outCSMA = gate("outCSMA");
    outTSCH = gate("outTSCH");
    inCSMA = gate("inCSMA");
    inTSCH = gate("inTSCH");

    // configuration
    frameCapacity = par("frameCapacity");

    std::stringstream a;
    a << getParentModule()->getParentModule()->getName() << " "
            << getParentModule()->getParentModule()->getIndex();
    //EndToEndDelay = new cOutVector(a.str().c_str());
    delMsg = new DataVector(a.str(), "DelMsg");
    if (rec_delMsg)
        delMsg->registerVector();
    activeMAC = 1;
    dataCenter = check_and_cast<DataCenter *>(
            dataCenter->getModuleByPath("DataCenter"));
}

cMessage* CIDERQueue::enqueue(cMessage* msg) {
    if (frameCapacity && queue.length() >= frameCapacity) {
        EV << "Ieee802154eQueue: Queue full, dropping packet.\n";
        return msg;
    } else {
        queue.insert(msg);
//        emit(queueLengthSignal, queue.length());
        return NULL;
    }
}

cMessage* CIDERQueue::dequeue() {
    if (queue.empty())
        return NULL;

    cMessage *msg = (cMessage *) queue.pop();

//    // statistics
//    emit(queueLengthSignal, queue.length());

    return msg;
}

void CIDERQueue::sendOut(cMessage* msg) {
    switch (activeMAC) {
    case 1: //CSMA
    {
        send(msg, outCSMA);
    }
        break;
    case 2: //TSCH
    {
        send(msg, outTSCH);
    }
        break;
    }

}

bool CIDERQueue::isEmpty() {
    return queue.empty();
}

cMessage* CIDERQueue::requestSpcPacket(MACAddress addr) {
    for (int i = 0; i < queue.length(); ++i) {
        cPacket *msg = PK(queue.get(i));
        if (dynamic_cast<Ieee802154eFrame *>(msg)) {
            Ieee802154eFrame* tmpMsg = check_and_cast<Ieee802154eFrame *>(msg);

            if (tmpMsg->getDstAddr() == addr) {
                emit(dequeuePkSignal, msg);
                emit(queueingTimeSignal, simTime() - msg->getArrivalTime());
                return msg;
            }
        }
    }

    EV << "Ieee802154eQueue: msg don't exist in the queue" << endl;
    return NULL;
}

cMessage* CIDERQueue::requestSpcPacket(MACAddress dstAddr,
        macLinkTableEntry* entry) {
    if (entry->getLinkOption() == LNK_OP_RECEIVE)
        return NULL;

    int direction = -2;

    if (entry->issameStage())
        direction = 0;
    if (entry->isnextStage())
        direction = 1;
    if (entry->isprevStage())
        direction = -1;

    switch (direction) {
    case 0: {

        for (int i = 0; i < queue.length(); ++i) {
            Ieee802154eFrame *msg = dynamic_cast<Ieee802154eFrame *>(PK(
                    queue.get(i)));
            if (msg) {

                if (msg->getDstAddr() == dstAddr) {
                    Ieee802154eNetworkCtrlInfo * tmp =
                            dynamic_cast<Ieee802154eNetworkCtrlInfo *>(msg->getControlInfo());
                    if (tmp) {
                        if (tmp->getTxCS()) {
                            delete msg->removeControlInfo();

                            return msg;
                        }
                    }
                }
            }
        }
        break;
    }
    case 1: {
        for (int i = 0; i < queue.length(); ++i) {
            Ieee802154eFrame *msg = dynamic_cast<Ieee802154eFrame *>(PK(
                    queue.get(i)));
            if (msg) {

                if (msg->getDstAddr() == dstAddr) {
                    Ieee802154eNetworkCtrlInfo *tmpMsg =
                            dynamic_cast<Ieee802154eNetworkCtrlInfo *>(msg->getControlInfo());
                    if (tmpMsg) {
                        if (tmpMsg->getTxHigherCH()) {
                            delete msg->removeControlInfo();

                            emit(dequeuePkSignal, msg);
                            emit(queueingTimeSignal,
                                    simTime() - msg->getArrivalTime());
                            return msg;
                        }
                    }
                }
            }
        }
        break;
    }
    case -1: {
        for (int i = 0; i < queue.length(); ++i) {
            Ieee802154eFrame *msg = dynamic_cast<Ieee802154eFrame *>(PK(
                    queue.get(i)));
            if (msg) {

                if (msg->getDstAddr() == dstAddr) {
                    Ieee802154eNetworkCtrlInfo *tmpMsg =
                            dynamic_cast<Ieee802154eNetworkCtrlInfo *>(msg->getControlInfo());
                    if (tmpMsg) {
                        if (tmpMsg->getTxLowerCH()) {
                            delete msg->removeControlInfo();

                            emit(dequeuePkSignal, msg);
                            emit(queueingTimeSignal,
                                    simTime() - msg->getArrivalTime());
                            return msg;
                        }
                    }
                }
            }
        }
        break;
    }
    default:
        return NULL;
    }
    return NULL;
}

cMessage* CIDERQueue::requestAdvPacket() {
    for (int i = 0; i < queue.length(); ++i) {
        cPacket *msg = PK(queue.get(i));
        if (dynamic_cast<Ieee802154eFrame *>(msg)) {
            Ieee802154eFrame* tmpMsg = check_and_cast<Ieee802154eFrame *>(msg);

            if ((strcmp(tmpMsg->getName(), "AssociationRequest") == 0
                    || strcmp(tmpMsg->getName(), "AssociationResponse") == 0)) {
                emit(dequeuePkSignal, msg);
                emit(queueingTimeSignal, simTime() - msg->getArrivalTime());
                return msg;
            }
        }
    }
    return NULL;
}

cMessage* CIDERQueue::requestDisAssPacket(bool response) {
    for (int i = 0; i < queue.length(); ++i) {
        cPacket *msg = PK(queue.get(i));
        if (response) {
            if (msg->getKind() == TP_MLME_DISASSOCIATE_RESPONSE
                    || msg->getKind() == Ieee802154e_DISASSOCIATION_RESPONSE) {
                emit(dequeuePkSignal, msg);
                emit(queueingTimeSignal, simTime() - msg->getArrivalTime());
                return msg;
            }
        } else {
            if (msg->getKind() == TP_MLME_DISASSOCIATE_REQUEST
                    || msg->getKind() == Ieee802154e_DISASSOCIATION_REQUEST) {
                emit(dequeuePkSignal, msg);
                emit(queueingTimeSignal, simTime() - msg->getArrivalTime());
                return msg;
            }
        }
    }
    return NULL;
}

cMessage* CIDERQueue::requestSchdulePacket(MACTSCHLinkType tp) {
    for (int i = 0; i < queue.length(); ++i) {
        cPacket *msg = PK(queue.get(i));
        if (tp == LNK_TP_JOIN) {
            if (msg->getKind() == TP_SCHEDULE_REQUEST) {
                emit(dequeuePkSignal, msg);
                emit(queueingTimeSignal, simTime() - msg->getArrivalTime());
                return msg;
            }
        } else if (tp == LNK_TP_ADVERTISING) {
            if (msg->getKind() == TP_SCHEDULE_RESPONSE) {
                emit(dequeuePkSignal, msg);
                emit(queueingTimeSignal, simTime() - msg->getArrivalTime());
                return msg;
            }
        }
    }
    return NULL;
}

cMessage* CIDERQueue::requestBeaconPacket() {
    for (int i = 0; i < queue.length(); ++i) {
        cPacket *msg = PK(queue.get(i));
        if (dynamic_cast<Ieee802154EnhancedBeaconFrame *>(msg)) {

            emit(dequeuePkSignal, msg);
            emit(queueingTimeSignal, simTime() - msg->getArrivalTime());
            return msg;

        }
    }
    return NULL;
}

bool CIDERQueue::existAssReq(MACAddress addr) {
    for (int i = 0; i < queue.length(); ++i) {
        Ieee802154eFrame *msg = dynamic_cast<Ieee802154eFrame *>(PK(
                queue.get(i)));
        if ((msg)) {

            if (msg->getDstAddr() == addr
                    && strcmp(msg->getName(), "AssociationRequest") == 0) {
                return true;
            }
        }
    }
    return false;
}

bool CIDERQueue::existAssRes(MACAddress addr) {
    for (int i = 0; i < queue.length(); ++i) {
        Ieee802154eFrame *msg = dynamic_cast<Ieee802154eFrame *>(PK(
                queue.get(i)));
        if ((msg)) {

            if (msg->getDstAddr() == addr
                    && strcmp(msg->getName(), "AssociationResponse") == 0) {
                return true;
            }
        }
    }
    return false;
}

bool CIDERQueue::existDisAssReq(MACAddress addr) {
    for (int i = 0; i < queue.length(); ++i) {
        Ieee802154eFrame *msg = dynamic_cast<Ieee802154eFrame *>(PK(
                queue.get(i)));
        if ((msg)) {

            if (msg->getDstAddr() == addr
                    && strcmp(msg->getName(), "DisassociationRequest") == 0) {
                return true;
            }
        }
    }
    return false;
}

bool CIDERQueue::existDisAssRes(MACAddress addr) {
    for (int i = 0; i < queue.length(); ++i) {
        Ieee802154eFrame *msg = dynamic_cast<Ieee802154eFrame *>(PK(
                queue.get(i)));
        if ((msg)) {

            if (msg->getDstAddr() == addr
                    && strcmp(msg->getName(), "DisassociationResponse") == 0) {
                return true;
            }
        }
    }
    return false;
}

bool CIDERQueue::existSchedReq(MACAddress addr) {
    for (int i = 0; i < queue.length(); ++i) {
        Ieee802154eFrame *msg = dynamic_cast<Ieee802154eFrame *>(PK(
                queue.get(i)));
        if ((msg)) {

            if (msg->getDstAddr() == addr
                    && strcmp(msg->getName(), "ScheduleRequest") == 0) {
                return true;
            }
        }
    }
    return false;
}

bool CIDERQueue::existSchedRes(MACAddress addr) {
    for (int i = 0; i < queue.length(); ++i) {
        Ieee802154eFrame *msg = dynamic_cast<Ieee802154eFrame *>(PK(
                queue.get(i)));
        if ((msg)) {

            if (msg->getDstAddr() == addr
                    && strcmp(msg->getName(), "ScheduleResponse") == 0) {
                return true;
            }
        }
    }
    return false;
}

int CIDERQueue::checkForNewerControlMessage(cMessage* msg, cMessage* txPaket) {
    int i = 0;
    int deleted = 0;
    while (!queue.empty() && i < queue.length()) {
        Ieee802154eFrame *queueMsg = dynamic_cast<Ieee802154eFrame *>(PK(
                queue.get(i)));
        if (queueMsg) {
            if (queueMsg->getCreationTime() < msg->getCreationTime()
                    && queueMsg->getKind() == msg->getKind()) {
                if (queueMsg != txPaket) {
                    if (queueMsg->getEncapsulatedMsg() && rec_delMsg)
                        delMsg->record(
                                queueMsg->getEncapsulatedMsg()->getName());
                    delete queue.remove(queueMsg);
                    i = 0;
                    deleted++;
                } else {
                    i++;
                }
            } else
                i++;
        }
    }
    return deleted;
}

bool CIDERQueue::deleteMsgFromQueu(cMessage* msg) {
    int i = 0;
    bool foundFirst = false;

    while (!queue.empty() && i < queue.length()) {
        Ieee802154eFrame *msg1 = dynamic_cast<Ieee802154eFrame *>(PK(
                queue.get(i)));

        if (msg1) {

            if (msg1 == msg) {

                queue.remove(msg1);

                i = 0; // start from the beginning, to prevent an NULL pointer
            } else
                i++;
        } else
            i++;
    }
    if (!foundFirst)
        EV << "Ieee802154eQueue: msg couldn't delete from the queue" << endl;

//    emit(queueLengthSignal, queue.length());

    return foundFirst ? true : false;
}

bool CIDERQueue::deleteMsgQueue(MACAddress dstAddr, bool all) {
    bool foundFirst = false;
    int countPkt = 0;
    int i = 0;

    while (!queue.empty() && i < queue.length()) {
        cPacket *msg = PK(queue.get(i));
        Ieee802154eFrame* tmpMsg = dynamic_cast<Ieee802154eFrame *>(msg);
        if (tmpMsg) {

            if (tmpMsg->getDstAddr() == dstAddr) {
                countPkt++;

                if (all || !foundFirst) {
                    queue.remove(queue.get(i));
                    foundFirst = true;
                    i = 0; // start from the beginning, to prevent an NULL pointer
                } else
                    ++i;
            }
            ++i;
        } else
            ++i;
    }

    if (!foundFirst)
        EV << "Ieee802154eQueue: msg couldn't delete from the queue" << endl;

//    emit(queueLengthSignal, queue.length());

    return foundFirst ? true : false;
}

bool CIDERQueue::deleteBCNQueue() {
    bool foundFirst = false;
    int countPkt = 0;
    int i = 0;
    bool all = true;

    while (!queue.empty() && i < queue.length()) {
        cPacket *msg = PK(queue.get(i));
        if (dynamic_cast<Ieee802154EnhancedBeaconFrame *>(msg)) {
            Ieee802154EnhancedBeaconFrame* tmpMsg = check_and_cast<
                    Ieee802154EnhancedBeaconFrame *>(msg);

            if (all || !foundFirst) {
                queue.remove(queue.get(i));
                foundFirst = true;
                i = 0; // start from the beginning, to prevent an NULL pointer
            } else
                ++i;

        } else
            ++i;
    }

    if (!foundFirst)
        EV << "Ieee802154eQueue: msg couldn't delete from the queue" << endl;

    return foundFirst ? true : false;
}

void CIDERQueue::insertInQueue(cMessage* msg) {
    numQueueReceived++;

    emit(rcvdPkSignal, msg);

    if (packetRequested > 0) {
        packetRequested--;
        emit(enqueuePkSignal, msg);
        emit(dequeuePkSignal, msg);
        emit(queueingTimeSignal, SIMTIME_ZERO);
        sendOut(msg);
    } else {
        msg->setArrivalTime(simTime());
        cMessage *droppedMsg = enqueue(msg);
        if (msg != droppedMsg)
            emit(enqueuePkSignal, msg);

        if (droppedMsg) {
            numQueueDropped++;
            emit(dropPkByQueueSignal, droppedMsg);
            delete droppedMsg;
        } else
            notifyListeners();
    }

    if (ev.isGUI()) {
        char buf[40];
        sprintf(buf, "q rcvd: %d\nq dropped: %d", numQueueReceived,
                numQueueDropped);
        getDisplayString().setTagArg("t", 0, buf);
    }
}

cMessage* CIDERQueue::requestSpcPacket() {

    cMessage *temp = dynamic_cast<cMessage *>(queue.get(0));
    if (dynamic_cast<Ieee802154eFrame *>(temp)) {
        return temp;

    }

    EV << "Ieee802154eQueue: msg don't exist in the queue" << endl;
    return NULL;

}

void CIDERQueue::clearQueue() {
    while (!queue.empty()) {

        delete queue.remove(queue.get(0));
    }
}

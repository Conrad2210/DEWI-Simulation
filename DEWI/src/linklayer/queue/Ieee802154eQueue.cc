//
// Copyright (C) 2005 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include "INETDefs.h"

#include "Ieee802154eQueue.h"
#include "Ieee802Ctrl_m.h"
#include "Ieee802154eFrame_m.h"

//@author: Stefan Reis      2014    (modified)
Define_Module(Ieee802154eQueue);

simsignal_t Ieee802154eQueue::queueLengthSignal = SIMSIGNAL_NULL;

void Ieee802154eQueue::initialize()
{
	PassiveQueueBase::initialize();

	queue.setName(par("queueName"));

	//statistics
	queueLengthSignal = registerSignal("queueLength");
	emit(queueLengthSignal, queue.length());

	outGate = gate("out");
	inGate = gate("in");

	// configuration
	frameCapacity = par("frameCapacity");
}

cMessage *Ieee802154eQueue::enqueue(cMessage *msg)
{
	if (frameCapacity && queue.length() >= frameCapacity)
	{
		EV << "Ieee802154eQueue: Queue full, dropping packet.\n";
		return msg;
	}
	else
	{
		queue.insert(msg);
		emit(queueLengthSignal, queue.length());
		return NULL;
	}
}

cMessage *Ieee802154eQueue::dequeue()
{
	if (queue.empty())
		return NULL;

	cMessage *msg = (cMessage *) queue.pop();

	// statistics
	emit(queueLengthSignal, queue.length());

	return msg;
}

void Ieee802154eQueue::sendOut(cMessage *msg)
{
	send(msg, outGate);
}

bool Ieee802154eQueue::isEmpty()
{
	return queue.empty();
}

//@author: Stefan Reis      2014
cMessage *Ieee802154eQueue::requestSpcPacket(MACAddress dstAddr)
{
	for (int i = 0; i < queue.length(); ++i)
	{
		cPacket *msg = PK(queue.get(i));
		if (dynamic_cast<Ieee802154eFrame *>(msg))
		{
			Ieee802154eFrame* tmpMsg = check_and_cast<Ieee802154eFrame *>(msg);

			if (tmpMsg->getDstAddr() == dstAddr)
			{
				emit(dequeuePkSignal, msg);
				emit(queueingTimeSignal, simTime() - msg->getArrivalTime());
				return msg;
			}
		}
	}

	EV << "Ieee802154eQueue: msg don't exist in the queue" << endl;
	return NULL;
}

cMessage *Ieee802154eQueue::requestSpcPacket(MACAddress dstAddr, macLinkTableEntry *entry)
{
	if (entry->getLinkOption() == LNK_OP_RECEIVE)
		return NULL;

	int direction = -2;

	if (entry->issameStage())
		direction = 0;
	if (entry->isnextStage())
		direction = 1;
	if (entry->isprevStage())
		direction = -1;

	switch (direction)
	{
	case 0:
	{

		for (int i = 0; i < queue.length(); ++i)
		{
			cPacket *msg = PK(queue.get(i));
			if (dynamic_cast<Ieee802154eFrame *>(msg))
			{
				Ieee802154eFrame* tmpMsg = check_and_cast<Ieee802154eFrame *>(msg);

				if (tmpMsg->getDstAddr() == dstAddr)
				{
					if (dynamic_cast<Ieee802154eNetworkCtrlInfo *>(tmpMsg->getControlInfo()))
					{
						if (dynamic_cast<Ieee802154eNetworkCtrlInfo *>(tmpMsg->getControlInfo())->getTxCS())
						{
							tmpMsg->removeControlInfo();

							emit(dequeuePkSignal, msg);
							emit(queueingTimeSignal, simTime() - msg->getArrivalTime());
							return msg;
						}
					}
				}
			}
		}
		break;
	}
	case 1:
	{
		for (int i = 0; i < queue.length(); ++i)
		{
			cPacket *msg = PK(queue.get(i));
			if (dynamic_cast<Ieee802154eFrame *>(msg))
			{
				Ieee802154eFrame* tmpMsg = check_and_cast<Ieee802154eFrame *>(msg);

				if (tmpMsg->getDstAddr() == dstAddr)
				{
					if (dynamic_cast<Ieee802154eNetworkCtrlInfo *>(tmpMsg->getControlInfo()))
					{
						if (dynamic_cast<Ieee802154eNetworkCtrlInfo *>(tmpMsg->getControlInfo())->getTxHigherCH())
						{
							tmpMsg->removeControlInfo();

							emit(dequeuePkSignal, msg);
							emit(queueingTimeSignal, simTime() - msg->getArrivalTime());
							return msg;
						}
					}
				}
			}
		}
		break;
	}
	case -1:
	{
		for (int i = 0; i < queue.length(); ++i)
		{
			cPacket *msg = PK(queue.get(i));
			if (dynamic_cast<Ieee802154eFrame *>(msg))
			{
				Ieee802154eFrame* tmpMsg = check_and_cast<Ieee802154eFrame *>(msg);

				if (tmpMsg->getDstAddr() == dstAddr)
				{
					if (dynamic_cast<Ieee802154eNetworkCtrlInfo *>(tmpMsg->getControlInfo()))
					{
						if (dynamic_cast<Ieee802154eNetworkCtrlInfo *>(tmpMsg->getControlInfo())->getTxLowerCH())
						{
							tmpMsg->removeControlInfo();
							emit(dequeuePkSignal, msg);
							emit(queueingTimeSignal, simTime() - msg->getArrivalTime());
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

cMessage *Ieee802154eQueue::requestAdvPacket()
{
	for (int i = 0; i < queue.length(); ++i)
	{
		cPacket *msg = PK(queue.get(i));
		if ((strcmp(msg->getName(), "AssociationRequest") == 0 || strcmp(msg->getName(), "AssociationResponse") == 0))
		{
			emit(dequeuePkSignal, msg);
			emit(queueingTimeSignal, simTime() - msg->getArrivalTime());
			return msg;
		}
	}
	return NULL;
}

cMessage *Ieee802154eQueue::requestDisAssPacket(bool response)
{
	for (int i = 0; i < queue.length(); ++i)
	{
		cPacket *msg = PK(queue.get(i));
		if (response)
		{
			if (msg->getKind() == TP_MLME_DISASSOCIATE_RESPONSE || msg->getKind() == Ieee802154e_DISASSOCIATION_RESPONSE)
			{
				emit(dequeuePkSignal, msg);
				emit(queueingTimeSignal, simTime() - msg->getArrivalTime());
				return msg;
			}
		}
		else
		{
			if (msg->getKind() == TP_MLME_DISASSOCIATE_REQUEST || msg->getKind() == Ieee802154e_DISASSOCIATION_REQUEST)
			{
				emit(dequeuePkSignal, msg);
				emit(queueingTimeSignal, simTime() - msg->getArrivalTime());
				return msg;
			}
		}
	}
	return NULL;

}

cMessage *Ieee802154eQueue::requestBeaconPacket()
{
	for (int i = 0; i < queue.length(); ++i)
	{
		cPacket *msg = PK(queue.get(i));
		if (strcmp(msg->getName(), "Ieee802154BEACON") == 0)
		{
			emit(dequeuePkSignal, msg);
			emit(queueingTimeSignal, simTime() - msg->getArrivalTime());
			return msg;
		}
	}
	return NULL;
}

bool Ieee802154eQueue::existSchedReq(MACAddress addr)
{
	for (int i = 0; i < queue.length(); ++i)
	{
		cPacket *msg = PK(queue.get(i));
		if (dynamic_cast<Ieee802154eFrame *>(msg))
		{
			Ieee802154eFrame* tmpMsg = check_and_cast<Ieee802154eFrame *>(msg);

			if (tmpMsg->getDstAddr() == addr && strcmp(msg->getName(), "ScheduleRequest") == 0)
			{
				return true;
			}
		}
	}
	return false;
}

bool Ieee802154eQueue::existSchedRes(MACAddress addr)
{
	for (int i = 0; i < queue.length(); ++i)
	{
		cPacket *msg = PK(queue.get(i));
		if (dynamic_cast<Ieee802154eFrame *>(msg))
		{
			Ieee802154eFrame* tmpMsg = check_and_cast<Ieee802154eFrame *>(msg);

			if (tmpMsg->getDstAddr() == addr && strcmp(msg->getName(), "ScheduleResponse") == 0)
			{
				return true;
			}
		}
	}
	return false;
}

bool Ieee802154eQueue::existAssRes(MACAddress addr)
{
	for (int i = 0; i < queue.length(); ++i)
	{
		cPacket *msg = PK(queue.get(i));
		if (dynamic_cast<Ieee802154eFrame *>(msg))
		{
			Ieee802154eFrame* tmpMsg = check_and_cast<Ieee802154eFrame *>(msg);

			if (tmpMsg->getDstAddr() == addr && strcmp(msg->getName(), "AssociationResponse") == 0)
			{
				return true;
			}
		}
	}
	return false;
}

bool Ieee802154eQueue::existAssReq(MACAddress addr)
{
	for (int i = 0; i < queue.length(); ++i)
	{
		cPacket *msg = PK(queue.get(i));
		if (dynamic_cast<Ieee802154eFrame *>(msg))
		{
			Ieee802154eFrame* tmpMsg = check_and_cast<Ieee802154eFrame *>(msg);

			if (tmpMsg->getDstAddr() == addr && strcmp(msg->getName(), "AssociationRequest") == 0)
			{
				return true;
			}
		}
	}
	return false;
}
bool Ieee802154eQueue::existDisAssReq(MACAddress addr)
{
	for (int i = 0; i < queue.length(); ++i)
	{
		cPacket *msg = PK(queue.get(i));
		if (dynamic_cast<Ieee802154eFrame *>(msg))
		{
			Ieee802154eFrame* tmpMsg = check_and_cast<Ieee802154eFrame *>(msg);

			if (tmpMsg->getDstAddr() == addr && strcmp(msg->getName(), "DisassociationRequest") == 0)
			{
				return true;
			}
		}
	}
	return false;
}

bool Ieee802154eQueue::existDisAssRes(MACAddress addr)
{
	for (int i = 0; i < queue.length(); ++i)
	{
		cPacket *msg = PK(queue.get(i));
		if (dynamic_cast<Ieee802154eFrame *>(msg))
		{
			Ieee802154eFrame* tmpMsg = check_and_cast<Ieee802154eFrame *>(msg);

			if (tmpMsg->getDstAddr() == addr && strcmp(msg->getName(), "DisassociationResponse") == 0)
			{
				return true;
			}
		}
	}
	return false;
}

int Ieee802154eQueue::checkForNewerControlMessage(cMessage *msg)
{
	int i = 0;
	int deleted = 0;
	while(!queue.empty() && i < queue.length())
	{
		cPacket *queueMsg = PK(queue.get(i));
		if(dynamic_cast<Ieee802154eFrame *>(queueMsg))
		{

			if(queueMsg->getCreationTime() < msg->getCreationTime() && queueMsg->getKind() == msg->getKind())
			{
				queue.remove(queue.get(i));
				i = 0;
				deleted++;
			}
			else
				i++;
		}
	}
	return deleted;
}

cMessage *Ieee802154eQueue::requestSchdulePacket()
{
	for (int i = 0; i < queue.length(); ++i)
	{
		cPacket *msg = PK(queue.get(i));
		if (msg->getKind() == TP_SCHEDULE_REQUEST || msg->getKind() == TP_SCHEDULE_RESPONSE)
		{
			emit(dequeuePkSignal, msg);
			emit(queueingTimeSignal, simTime() - msg->getArrivalTime());
			return msg;
		}
	}
	return NULL;
}

//@author: Stefan Reis      2014
bool Ieee802154eQueue::deleteMsgQueue(MACAddress dstAddr, bool all)
{
	bool foundFirst = false;
	int countPkt = 0;
	int i = 0;

	while (!queue.empty() && i < queue.length())
	{
		cPacket *msg = PK(queue.get(i));
		if (dynamic_cast<Ieee802154eFrame *>(msg))
		{
			Ieee802154eFrame* tmpMsg = check_and_cast<Ieee802154eFrame *>(msg);
			if (tmpMsg->getDstAddr() == dstAddr)
			{
				countPkt++;

				if (all || !foundFirst)
				{
					queue.remove(queue.get(i));
					foundFirst = true;
					i = 0; // start from the beginning, to prevent an NULL pointer
				}
				else
					++i;
			}
			++i;
		}
		else
			++i;
	}

	if (!foundFirst)
		EV << "Ieee802154eQueue: msg couldn't delete from the queue" << endl;

	emit(queueLengthSignal, queue.length());

	return foundFirst ? true : false;
}

bool Ieee802154eQueue::deleteBCNQueue()
{
	bool foundFirst = false;
	int countPkt = 0;
	int i = 0;
	bool all = true;

	while (!queue.empty() && i < queue.length())
	{
		cPacket *msg = PK(queue.get(i));
		if (dynamic_cast<Ieee802154EnhancedBeaconFrame *>(msg))
		{
			Ieee802154EnhancedBeaconFrame* tmpMsg = check_and_cast<Ieee802154EnhancedBeaconFrame *>(msg);

			if (all || !foundFirst)
			{
				queue.remove(queue.get(i));
				foundFirst = true;
				i = 0; // start from the beginning, to prevent an NULL pointer
			}
			else
				++i;

		}
		else
			++i;
	}

	if (!foundFirst)
		EV << "Ieee802154eQueue: msg couldn't delete from the queue" << endl;

	return foundFirst ? true : false;
}

//@author: Stefan Reis      2014
void Ieee802154eQueue::insertInQueue(cMessage *msg)
{
	numQueueReceived++;

	emit(rcvdPkSignal, msg);

	if (packetRequested > 0)
	{
		packetRequested--;
		emit(enqueuePkSignal, msg);
		emit(dequeuePkSignal, msg);
		emit(queueingTimeSignal, SIMTIME_ZERO);
		sendOut(msg);
	}
	else
	{
		msg->setArrivalTime(simTime());
		cMessage *droppedMsg = enqueue(msg);
		if (msg != droppedMsg)
			emit(enqueuePkSignal, msg);

		if (droppedMsg)
		{
			numQueueDropped++;
			emit(dropPkByQueueSignal, droppedMsg);
			delete droppedMsg;
		}
		else
			notifyListeners();
	}

	if (ev.isGUI())
	{
		char buf[40];
		sprintf(buf, "q rcvd: %d\nq dropped: %d", numQueueReceived, numQueueDropped);
		getDisplayString().setTagArg("t", 0, buf);
	}
}

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
    if(frameCapacity && queue.length() >= frameCapacity)
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
    if(queue.empty())
	return NULL;

    cMessage *msg = (cMessage *)queue.pop();

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
    for(int i = 0; i < queue.length(); ++i)
    {
	cPacket *msg = PK(queue.get(i));
	if(dynamic_cast<Ieee802154eFrame *>(msg))
	{
	    Ieee802154eFrame* tmpMsg = check_and_cast<Ieee802154eFrame *>(msg);

	    if(tmpMsg->getDstAddr() == dstAddr)
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

cMessage *Ieee802154eQueue::requestAdvPacket()
{
    for(int i = 0; i < queue.length(); ++i)
    {
	cPacket *msg = PK(queue.get(i));
	if(dynamic_cast<Ieee802154eAssociationFrame *>(msg))
	{
	    emit(dequeuePkSignal, msg);
	    emit(queueingTimeSignal, simTime() - msg->getArrivalTime());
	    return msg;
	}
    }
}
cMessage *Ieee802154eQueue::requestSchdulePacket()
{
    for(int i = 0; i < queue.length(); ++i)
    {
	cPacket *msg = PK(queue.get(i));
	if(dynamic_cast<Ieee802154eScheduleFrame *>(msg))
	{
	    emit(dequeuePkSignal, msg);
	    emit(queueingTimeSignal, simTime() - msg->getArrivalTime());
	    return msg;
	}
    }
}

//@author: Stefan Reis      2014
bool Ieee802154eQueue::deleteMsgQueue(MACAddress dstAddr, bool all)
{
    bool foundFirst = false;
    int countPkt = 0;
    int i = 0;

    while(!queue.empty() && i < queue.length())
    {
	cPacket *msg = PK(queue.get(i));
	if(dynamic_cast<Ieee802154eFrame *>(msg))
	{
	    Ieee802154eFrame* tmpMsg = check_and_cast<Ieee802154eFrame *>(msg);
	    if(tmpMsg->getDstAddr() == dstAddr)
	    {
		countPkt++;

		if(all || !foundFirst)
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

    if(!foundFirst)
	EV << "Ieee802154eQueue: msg couldn't delete from the queue" << endl;

    emit(queueLengthSignal, queue.length());

    return foundFirst ? true:false;
}

//@author: Stefan Reis      2014
void Ieee802154eQueue::insertInQueue(cMessage *msg)
{
    numQueueReceived++;

    emit(rcvdPkSignal, msg);

    if(packetRequested > 0)
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
	if(msg != droppedMsg)
	    emit(enqueuePkSignal, msg);

	if(droppedMsg)
	{
	    numQueueDropped++;
	    emit(dropPkByQueueSignal, droppedMsg);
	    delete droppedMsg;
	}
	else
	    notifyListeners();
    }

    if(ev.isGUI())
    {
	char buf[40];
	sprintf(buf, "q rcvd: %d\nq dropped: %d", numQueueReceived, numQueueDropped);
	getDisplayString().setTagArg("t", 0, buf);
    }
}

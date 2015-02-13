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


#ifndef __INET_IEEE802154EQUEUE_H
#define __INET_IEEE802154EQUEUE_H

#include "INETDefs.h"
#include "MACAddress.h"

#include "PassiveQueueBase.h"

/**
 * Ieee802154e queue. See NED for more info.
 * @author: Stefan Reis     2014    (modified)
 */
class INET_API Ieee802154eQueue : public PassiveQueueBase
{
  protected:
    // configuration
    int frameCapacity;

    // state
    cQueue queue;
    cGate *outGate;
    cGate *inGate;

    // statistics
    static simsignal_t queueLengthSignal;

  protected:
    virtual void initialize();

    /**
     * Redefined from PassiveQueueBase.
     */
    virtual cMessage *enqueue(cMessage *msg);

    /**
     * Redefined from PassiveQueueBase.
     */
    virtual cMessage *dequeue();

    /**
     * Redefined from PassiveQueueBase.
     */
    virtual void sendOut(cMessage *msg);

  public:
    /**
     * Redefined from IPassiveQueue.
     */
    virtual bool isEmpty();

    /**
     * request a specific packet from the queue for a MACAdress without/with specific msduHandle
     */
    virtual cMessage *requestSpcPacket(MACAddress addr);

    virtual cMessage *requestAdvPacket();

    virtual cMessage *requestSchdulePacket();

    /**
     * delete a specific one/all packet(s) from the queue
     */
    virtual bool deleteMsgQueue(MACAddress dstAddr, bool all);

    virtual void insertInQueue(cMessage *msg);
};

#endif

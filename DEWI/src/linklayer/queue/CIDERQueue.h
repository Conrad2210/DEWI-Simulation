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

#ifndef CIDERQUEUE_H_
#define CIDERQUEUE_H_

#include "INETDefs.h"
#include "MACAddress.h"

#include "PassiveQueueBase.h"
#include "macLinkTableEntry.h"

#include "DataCenter.h"
#include "DataVector.h"

class CIDERQueue: public PassiveQueueBase {
public:
    CIDERQueue();
    virtual ~CIDERQueue();

protected:
    // configuration
    int frameCapacity;

    // state
    cQueue queue;
    cGate *outCSMA, *outTSCH;
    cGate *inCSMA, *inTSCH;

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


    DataCenter *dataCenter;
    DataVector *delMsg;
    bool rec_delMsg;

  public:
    /**
     * Redefined from IPassiveQueue.
     */
    int getCapacity(){return frameCapacity;}
    int activeMAC;
    void setActiveMAC(int i){activeMAC = i;}
    virtual bool isEmpty();

    virtual bool contains(cObject *o){return queue.contains(o);}

    /**
     * request a specific packet from the queue for a MACAdress without/with specific msduHandle
     */
    virtual int getQueueSize(){return queue.length();}
    virtual cMessage *requestSpcPacket();
    virtual cMessage *requestSpcPacket(MACAddress addr);
    virtual cMessage *requestSpcPacket(MACAddress dstAddr, macLinkTableEntry *entry);

    virtual cMessage *requestAdvPacket();

    virtual cMessage *requestDisAssPacket(bool response);

    virtual cMessage *requestSchdulePacket(MACTSCHLinkType tp);

    virtual cMessage *requestBeaconPacket();

    virtual bool existAssReq(MACAddress addr);
    virtual bool existAssRes(MACAddress addr);

    virtual bool existDisAssReq(MACAddress addr);
    virtual bool existDisAssRes(MACAddress addr);

    virtual bool existSchedReq(MACAddress addr);
    virtual bool existSchedRes(MACAddress addr);

    virtual int checkForNewerControlMessage(cMessage *msg, cMessage *txPaket);
    virtual bool deleteMsgFromQueu(cMessage *msg);
    /**
     * delete a specific one/all packet(s) from the queue
     */
    virtual bool deleteMsgQueue(MACAddress dstAddr, bool all);

    virtual bool deleteBCNQueue();

    virtual void insertInQueue(cMessage *msg);

    virtual void clearQueue();
};

#endif /* CIDERQUEUE_H_ */

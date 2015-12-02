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

#include <CIDERNet/Net/SimpleNet.h>
#include "RLLFrame_m.h"
#include "CIDERFrame_m.h"
Define_Module(SimpleNet);
SimpleNet::SimpleNet() {
    // TODO Auto-generated constructor stub

}

SimpleNet::~SimpleNet() {
    // TODO Auto-generated destructor stub
}

void SimpleNet::initialize(int stage) {
    switch (stage) {
    case 0:
        mRLLIn = findGate("RLLIn");
        mRLLOut = findGate("RLLOut");
        mCIDERIn = findGate("CIDERIn");
        mCIDEROut = findGate("CIDEROut");

        mLowerLayerIn = findGate("lowerLayerIn");
        mLowerLayerOut = findGate("lowerLayerOut");

        mUpperLayerIn = findGate("upperLayerIn");
        mUpperLayerOut = findGate("upperLayerOut");
        break;
    }
}

void SimpleNet::finish() {
}

void SimpleNet::handleMessage(cMessage* msg) {

    int ArrivalGate = msg->getArrivalGateId();

    if (ArrivalGate == mLowerLayerIn) {
        //do stuff for lower layer in
        if (dynamic_cast<RLLFrame*>(msg)) {
            send(msg, mRLLOut);
        } else if (dynamic_cast<CIDERFrame*>(msg)) {
            send(msg, mCIDEROut);
        } else {

            EV << "Nothing to do with this message";
            delete msg;
        }
    } else if (ArrivalGate == mUpperLayerIn) {
        //do stuff for upper layer in
        handleDataMessage(msg);
    } else if (ArrivalGate == mRLLIn) {
        //do stuff for RLL in
        handleRLLMessage(msg);
    } else if (ArrivalGate == mCIDERIn) {
        //do stuff for CIDER in
        handleCIDERMessge(msg);
    } else {

        EV << "Nothing to do with this message";
        delete msg;
    }
}

void SimpleNet::handleDataMessage(cMessage* msg) {
}

void SimpleNet::handleRLLMessage(cMessage* msg) {
}

void SimpleNet::handleCIDERMessge(cMessage* msg) {
}

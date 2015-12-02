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
Define_Module(SimpleNet);
SimpleNet::SimpleNet() {
    // TODO Auto-generated constructor stub

}

SimpleNet::~SimpleNet() {
    // TODO Auto-generated destructor stub
}

void SimpleNet::initialize(int stage) {
    switch(stage){
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
}

void SimpleNet::handleDataMessage(cPacket* msg) {
}

void SimpleNet::handleRLLMessage(cPacket* msg) {
}

void SimpleNet::handleCIDERMessge(cPacket* msg) {
}

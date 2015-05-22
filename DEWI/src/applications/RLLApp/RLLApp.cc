//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY{} without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <RLLApp.h>
#include "RLLAppMsg_m.h"
Define_Module(RLLApp);
RLLApp::RLLApp()
{
    // TODO Auto-generated constructor stub

}

RLLApp::~RLLApp()
{
    // TODO Auto-generated destructor stub
}

void RLLApp::initialize(int stage)
{
    TrafGenPar::initialize(stage);
    EV << getParentModule()->getFullName() << ": initializing RLL Test App, stage=" << stage << endl;
    if (0 == stage)
    {
        m_debug             = par("debug");
        mLowerLayerIn        = findGate("lowerLayerIn");
        mLowerLayerOut       = findGate("lowerLayerOut");
        m_moduleName        = getParentModule()->getFullName();

    }
}

void RLLApp::finish()
{

}

// OPERATIONS
void RLLApp::handleSelfMsg(cMessage* msg)
{
    TrafGenPar::handleSelfMsg(msg);
}
void RLLApp::handleLowerMsg(cMessage*)
{
}

void RLLApp::SendTraf(cPacket *msg, const char* dest)
{
    delete msg;

    RLLAppMsg* appPkt = new RLLAppMsg("RLLAppMsg");
    appPkt->setBitLength(PacketSize()*8);
    appPkt->setSourceName(m_moduleName);
    appPkt->setDestName("Broadcast");
    appPkt->setCreationTime(simTime());

    send(appPkt->dup(),mLowerLayerOut);
    delete appPkt;
}

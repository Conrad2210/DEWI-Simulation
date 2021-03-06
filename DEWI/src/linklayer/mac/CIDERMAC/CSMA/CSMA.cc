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

#include <CSMA.h>
#include "Ieee802154eNetworkCtrlInfo_m.h"
Define_Module(CSMA)
CSMA::CSMA()
{
	// TODO Auto-generated constructor stub

}

CSMA::~CSMA()
{
	// TODO Auto-generated destructor stub
}

void CSMA::initialize(int stage)
{
	if(stage == 1)
	{

	     mUpperLayerIn = findGate("upperDeciderIn");
	     mUpperLayerOut = findGate("upperDeciderOut");
	     mLowerLayerIn = findGate("lowereciderIn");
	     mLowerLayerOut = findGate("lowerDeciderOut");
	}
}

void CSMA::handleMessage(cPacket* msg)
{

}

void CSMA::handleSelfMsg(cMessage* msg)
{
}

void CSMA::handleUpperMsg(cPacket* msg)
{
}

void CSMA::handleCommand(cMessage* msg)
{
}

void CSMA::handleLowerMsg(cPacket* msg)
{
}

InterfaceEntry* CSMA::createInterfaceEntry()
{
}

void CSMA::flushQueue()
{
}

void CSMA::clearQueue()
{
}

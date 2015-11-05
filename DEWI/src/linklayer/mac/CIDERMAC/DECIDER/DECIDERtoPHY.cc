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

#include <DECIDERtoPHY.h>
Define_Module(DECIDERtoPHY)
DECIDERtoPHY::DECIDERtoPHY()
{
	// TODO Auto-generated constructor stub

}

DECIDERtoPHY::~DECIDERtoPHY()
{
	// TODO Auto-generated destructor stub
}

void DECIDERtoPHY::initialize(int stage)
{
	if(stage!=1)
		return;

	mLowerLayerIn = findGate("lowerLayerIn"); 			// from the upper layer
	mLowerLayerOut = findGate("lowerLayerOut"); 		// to the upper layer
	mCSMAIn = findGate("CSMAIn"); 			// from the physical layer
	mCSMAOut = findGate("CSMAOut"); 			// to the physical layer
	mTSCHIn = findGate("TSCHIn"); 			// from the physical layer
	mTSCHOut = findGate("TSCHOut"); 			// to the physical layer

	mTSCHActive = false;
}

void DECIDERtoPHY::finish()
{
}

void DECIDERtoPHY::handleMessage(cMessage* msg)
{
	if(msg->getArrivalGateId() == (mCSMAIn || mTSCHIn))
		handleUpperMsg(msg);
	else if(msg->getArrivalGateId() == mLowerLayerIn)
	{
		handleLowerMsg(msg);
	}

}

void DECIDERtoPHY::handleLowerMsg(cMessage* msg)
{
	if(mTSCHActive)
	{
		send(msg,mTSCHOut);
	}
	else
	{
		send(msg,mCSMAOut);
	}
}

void DECIDERtoPHY::handleUpperMsg(cMessage* msg)
{
	send(msg,mLowerLayerOut);
}

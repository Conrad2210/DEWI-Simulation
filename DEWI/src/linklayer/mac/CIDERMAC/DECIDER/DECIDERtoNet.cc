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

#include <cmessage.h>
#include <cobjectfactory.h>
#include <cregistrationlist.h>
#include <DECIDERtoNet.h>
#include <regmacros.h>
#include <simutil.h>

Define_Module(DECIDERtoNet)
DECIDERtoNet::DECIDERtoNet()
{
	// TODO Auto-generated constructor stub

}

DECIDERtoNet::~DECIDERtoNet()
{
	// TODO Auto-generated destructor stub
}

void DECIDERtoNet::initialize(int stage)
{
	if (stage != 1)
		return;

	mUpperLayerIn = findGate("upperLayerIn");
	mUpperLayerOut = findGate("upperLayerOut");

	mCSMAIn = findGate("CSMAIn");
	mCSMAOut = findGate("CSMAOut");
	mCSMAQueueIn = findGate("CSMAQueueIn");
	mCSMAQueueOut = findGate("CSMAQueueOut");

	mTSCHIn = findGate("TSCHIn");
	mTSCHOut = findGate("TSCHOut");
	mTSCHQueueIn = findGate("TSCHQueueIn");
	mTSCHQueueOut = findGate("TSCHQueueOut");

	mQueueIn = findGate("queueIn");
	mQueueOut = findGate("queueOut");
	mTSCHActive = false;

}

void DECIDERtoNet::finish()
{
}

void DECIDERtoNet::handleMessage(cMessage* msg)
{
	if(msg->getArrivalGateId() == mUpperLayerIn)
	{
		handleUpperMsg(msg);
	}
	else if(msg->getArrivalGateId() == mCSMAIn || msg->getArrivalGateId() ==  mTSCHIn)
	{
		handleLowerMsg(msg);
	}
}

void DECIDERtoNet::handleLowerMsg(cMessage* msg)
{
	send(msg,mUpperLayerOut);
}

void DECIDERtoNet::handleUpperMsg(cMessage* msg)
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

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

#include <CIDER/CIDER.h>
#include "CIDERControlInfo_m.h"
#include "CIDERFrame_m.h"

#include "IInterfaceTable.h"
#include "InterfaceTableAccess.h"
Define_Module(CIDER);
CIDER::CIDER() {
    // TODO Auto-generated constructor stub

}

CIDER::~CIDER() {
    // TODO Auto-generated destructor stub
}

void CIDER::initialize(int stage)
{
	if(stage == 1)
	{
		networkLayerIn = findGate("networkLayerIn");
		networkLayerOut = findGate("networkLayerOut");
	}
	else if(stage == 2)
	{
		IInterfaceTable *ift = InterfaceTableAccess().getIfExists();
		myInterface = ift->getInterface(0);
		uint64_t temp = myInterface->getMacAddress().getInt() - 0xaaa00feff000000;
		uint64_t startTime = (temp % 0xFFFF) * 10;
		timerInitialPing = new cMessage("Ping",500);
		scheduleAt(startTime,timerInitialPing);
	}

}

void CIDER::finish()
{
	cancelAndDelete(timerInitialPing);
}

void CIDER::handleMessage(cMessage* msg)
{
	if(msg->isSelfMessage())
		handleSelfMessage(msg);
}

void CIDER::handleSelfMessage(cMessage* msg)
{
	if(strcmp(msg->getName(),"Ping") == 0)
	{
		CIDERFrame *newFrame= new CIDERFrame("CIDERFRAME",501);
		CIDERControlInfo *cntrl = new CIDERControlInfo("CiderContrl",502);
		newFrame->setControlInfo(cntrl);
		send(newFrame,networkLayerOut);
	}
}

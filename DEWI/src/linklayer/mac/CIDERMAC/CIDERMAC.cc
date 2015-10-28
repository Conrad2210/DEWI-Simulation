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

#include <cenvir.h>
#include <cobjectfactory.h>
#include <cownedobject.h>
#include <cpar.h>
#include <cregistrationlist.h>
#include <CIDERMAC.h>
#include <Ieee802154eQueue.h>
#include <IMacHoppingSequenceList.h>
#include <IMacLinkTable.h>
#include <IMacNeighborTable.h>
#include <IMacSlotframeTable.h>
#include <IMacTimeslotTable.h>
#include <ModuleAccess.h>
#include <NotificationBoard.h>
#include <NotifierConsts.h>
#include <regmacros.h>
#include <simutil.h>

Define_Module(CIDERMAC);
CIDERMAC::CIDERMAC()
{
	// TODO Auto-generated constructor stub

}

CIDERMAC::~CIDERMAC()
{
	// TODO Auto-generated destructor stub
}

void CIDERMAC::intialize(int stage)
{
	WirelessMacBase::initialize(stage);

	switch (stage)
	{
		case 0:
		{

			commonInitialize();
		}
			break;
		case 1:
		{

		}
			break;
		case 2:
		{

		}
			break;
		case 3:
		{

		}
			break;
		case 4:
		{

		}
			break;
		case 5:
		{

		}
			break;
		default:
		{
			return;
		}
	}

}

void CIDERMAC::handleMessage(cMessage* msg)
{
}

void CIDERMAC::handleLowerMsg(cPacket* pkt)
{
}

void CIDERMAC::handleSelfMsg(cMessage* msg)
{
}

void CIDERMAC::handleUpperMsg(cPacket* pkt)
{
}

void CIDERMAC::handleCommand(cMessage* msg)
{
}

InterfaceEntry*
CIDERMAC::createInterfaceEntry()
{
	return NULL;
}

void CIDERMAC::flushQueue()
{
}

void CIDERMAC::commonInitialize()
{
	macaddress = configurationMacAddress();
	if (!macaddress.getFlagEui64())
		opp_error("802154 address error, address is not EUI64");

	aExtendedAddress = macaddress;

	iface = NULL;
	registerInterface();

	// get gate I
	mUpperLayerIn = findGate("upperLayerIn");
	mUpperLayerOut = findGate("upperLayerOut");
	mLowerLayerIn = findGate("lowerLayerIn");
	mLowerLayerOut = findGate("lowerLayerOut");
	mQueueIn = findGate("queueIn");
	mQueueOut = findGate("queueOut");

	// get a pointer to the SlotframeTable Module
	slotframeTable = check_and_cast<IMacSlotframeTable *>(getModuleByPath(par("macSlotframeTableModule")));
	// get a pointer to the LinkTable Module
	linkTable = check_and_cast<IMacLinkTable *>(getModuleByPath(par("macLinkTableModule")));
	// get a pointer to the TimeslotTable Module
	timeslotTable = check_and_cast<IMacTimeslotTable *>(getModuleByPath(par("macTimeslotTableModule")));
	// get a pointer to the HoppingSequenceList Module
	hoppingSequenceList = check_and_cast<IMacHoppingSequenceList *>(getModuleByPath(par("macHoppingSequenceListModule")));

	// get a pointer to the NeighborTable Module
	neighborTable = check_and_cast<IMacNeighborTable *>(getModuleByPath(par("macNeighborTableModule")));

	// get a pointer to the NotificationBoard module
	mpNb = NotificationBoardAccess().get();
	// subscribe for the information of the carrier sens
	mpNb->subscribe(this, NF_RADIOSTATE_CHANGED);
	//mpNb->subscribe(this, NF_BITRATE_CHANGED)
	mpNb->subscribe(this, NF_RADIO_CHANNEL_CHANGED);
	mpNb->subscribe(this, NF_PHY_PKT_DISCARDED); // for the TSCH handle, if a pkt is dropped at the phy layer
	radioState = RadioState::IDLE;

	// obtain pointer to external queue
	initializeQueueModule();
}

void CIDERMAC::clearQueue()
{
}

MACAddress CIDERMAC::configurationMacAddress()
{
	MACAddress address;

	// assign automatic address
	address = MACAddress::generateAutoAddress();
	address.convert64();
	// change module parameter from "auto" to concrete address
	par("address").setStringValue(address.str().c_str());

	return address;
}

void CIDERMAC::initializeQueueModule()
{
	// use of external queue module is optional -- find it if there's one specified
	if (par("queueModule").stringValue()[0])
	{
		cModule *module = getParentModule()->getSubmodule(par("queueModule").stringValue());
		//queueModule = check_and_cast<IPassiveQueue *>(module); //[SR] old version
		queueModule = check_and_cast<Ieee802154eQueue *>(module);

		EV << "[IFQUEUE]: requesting first one frame from queue module\n";
		//queueModule->requestPacket(); //[SR] old version
	}
}

MACAddress CIDERMAC::getLongAddress(UINT_16 v)
{
}

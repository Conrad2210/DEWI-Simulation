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

#include <SlottedAloha/SlottedAlohaScheduler.h>
#include "Ieee802154eConst.h"
#include "Ieee802154eNetworkCtrlInfo_m.h"
Define_Module(SlottedAlohaScheduler);
static const int PhyConfig = 134215680;
static const int ExtendedBitmap = 0;
static const int minChannelNum = 11;
static const int maxChannelNum = 27;
static const int channelList[16] =
{
11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26
};
static const int numChannel = 16;

SlottedAlohaScheduler::SlottedAlohaScheduler()
{
    // TODO Auto-generated constructor stub
    lastSCANChannel = 0;
}

SlottedAlohaScheduler::~SlottedAlohaScheduler()
{
    // TODO Auto-generated destructor stub

}

void SlottedAlohaScheduler::initialize(int stage)
{
    if(stage == 0)
    {

    }
    else if(stage == 1)
    {
	slotframeTable = check_and_cast<IMacSlotframeTable *>(getModuleByPath(par("macSlotframeTableModule")));
	// get a pointer to the LinkTable Module
	linkTable = check_and_cast<IMacLinkTable *>(getModuleByPath(par("macLinkTableModule")));
	// get a pointer to the TimeslotTable Module
	timeslotTable = check_and_cast<IMacTimeslotTable *>(getModuleByPath(par("macTimeslotTableModule")));
	// get a pointer to the HoppingSequenceList Module
	hoppingSequenceList = check_and_cast<IMacHoppingSequenceList *>(getModuleByPath(par("macHoppingSequenceListModule")));

	// get a pointer to the NeighborTable Module
	neighborTable = check_and_cast<IMacNeighborTable *>(getModuleByPath(par("macNeighborTableModule")));

	inGate = findGate("from_mac");
	outGate = findGate("to_mac");

	isPANCoor = par("isPANCoor").boolValue();
    }
    else if(stage == 2)
    {

    }
    else if(stage == 3)
    {
	createInitialEntries();
	StartTimer = new cMessage("StartTimer", START_TIMER);
	double start = par("StartTime").doubleValue();
	scheduleAt(simTime() + start, StartTimer);

    }
}

void SlottedAlohaScheduler::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage())
    {
	handleSelfMessage(msg);
    }
    else if(msg->getArrivalGateId() == inGate)
    {
	handleMACMessage(msg);
    }
    else
    {
	if(ev.isGUI())
	{
	    EV << "Undefined Message type; Dropp Message" << endl;

	}
	delete msg;
    }
}

void SlottedAlohaScheduler::handleMACMessage(cMessage *msg)
{
    switch(msg->getKind())
    {
	case TP_MLME_START_CONFIRM:
	{
	    handle_MLME_START_confirm(msg);
	    break;
	}
	case TP_MLME_SCAN_CONFIRM:
	{
	    handle_MLME_SCAN_confirm(msg);
	    break;
	}
	case TP_MLME_BEACON_CONFIRM:
	{
	    handle_MLME_BEACON_confirm(msg);
	    break;
	}
	case TP_MLME_BEACON_NOTIFY_INDICATION:
	{
	    handle_MLME_BEACON_indication(msg);
	    break;
	}
	default:
	{
	    if(ev.isGUI())
	    {
		EV << "invalid CMD, delete msg" << endl;
	    }
	    delete msg;
	    break;
	}
    }

}
void SlottedAlohaScheduler::handleSelfMessage(cMessage *msg)
{
    switch(msg->getKind())
    {
	case START_TIMER:
	{
	    MLME_START_request(msg);
	    break;
	}
	case BEACON_REQUEST:
	{
	    MLME_BEACON_request(msg);
	    break;
	}
	default:
	{
	    if(ev.isGUI())
	    {
		EV << "Invalid MSG kind, delete msg" << endl;

	    }
	    delete msg;
	    break;
	}
    }
}

//Association Process
void SlottedAlohaScheduler::MLME_ASSOCIATE_request(cMessage *msg)
{
}

void SlottedAlohaScheduler::handle_MLME_ASSOCIATE_indication(cMessage *msg)
{
}

void SlottedAlohaScheduler::MLME_ASSOCIATE_responce(cMessage *msg)
{
}

void SlottedAlohaScheduler::handle_MLME_ASSOCIATE_confirm(cMessage *msg)
{
}

//Dissassociation Process
void SlottedAlohaScheduler::MLME_DISASSOCIATE_request(cMessage *msg)
{
}

void SlottedAlohaScheduler::handle_MLME_DIASSOCIATE_indication(cMessage *msg)
{
}

void SlottedAlohaScheduler::handle_MLME_DIASSOCIATE_confirm(cMessage *msg)
{
}

//Advertisment

void SlottedAlohaScheduler::MLME_BEACON_request(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *tempMsg = new Ieee802154eNetworkCtrlInfo("BeaconRequest", TP_MLME_BEACON_REQUEST);
    tempMsg->setBeaconType(0x01);
    tempMsg->setChannel(11);

    send(tempMsg->dup(), outGate);
    delete tempMsg;
    delete msg;

}

void SlottedAlohaScheduler::handle_MLME_BEACON_confirm(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *beaconCon = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    double time = pow(2, beaconCon->getBeaconOrder()) * beaconCon->getSuperframeOrder() * timeslotTable->getTemplate(0)->getTimeslotLengthDbl();
    BeaconTimer = new cMessage("BeaconTimer", BEACON_REQUEST);
    scheduleAt(simTime() + time, BeaconTimer);
    delete beaconCon;
}

void SlottedAlohaScheduler::handle_MLME_BEACON_indication(cMessage *msg)
{
    MLME_SET_BEACON_request(msg);
}

void SlottedAlohaScheduler::MLME_SETSLOTFRAME_request(cMessage *msg)
{

}

void SlottedAlohaScheduler::handle_MLME_SETSLOTFRAME_confirm(cMessage *msg)
{
}

void SlottedAlohaScheduler::MLME_SETLINK_request(cMessage *msg)
{
}

void SlottedAlohaScheduler::handle_MLME_SETLINK_confirm(cMessage *msg)
{
}

void SlottedAlohaScheduler::MLME_TSCHMODE_request(cMessage *msg)
{
}

void SlottedAlohaScheduler::handle_MLME_TSCHMODE_confirm(cMessage *msg)
{
}

//////START
void SlottedAlohaScheduler::MLME_START_request(cMessage *msg)
{

    Ieee802154eNetworkCtrlInfo *startMsg = new Ieee802154eNetworkCtrlInfo("StartMsg", TP_MLME_START_REQUEST);
    startMsg->setPanCoordinator(par("isPANCoor").boolValue());
    startMsg->setStartTime((uint32_t)par("StartTime").doubleValue());
    send(startMsg->dup(), outGate);
    delete startMsg;
    delete msg;
}

void SlottedAlohaScheduler::handle_MLME_START_confirm(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *startCo = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);

    if(startCo->getPanCoordinator())
    {
	MLME_BEACON_request(msg);
    }
    else
    {
	MLME_SCAN_request(msg);
    }
}

//Scan
void SlottedAlohaScheduler::MLME_SCAN_request(cMessage *msg)
{

    Ieee802154eNetworkCtrlInfo *scanReq = new Ieee802154eNetworkCtrlInfo("ScanRequest", TP_MLME_SCAN_REQUEST);

    if(lastSCANChannel == 0)
    {
	scanReq->setChannel(channelList[0]);
	lastSCANChannel = channelList[0];
    }
    else
    {
	for(int i = 0; i < numChannel; i++)
	{
	    if(lastSCANChannel == channelList[i] && lastSCANChannel != channelList[numChannel])
	    {
		lastSCANChannel = channelList[i + 1];
		scanReq->setChannel(channelList[i + 1]);
		break;
	    }
	    else if(lastSCANChannel == channelList[numChannel])
	    {
		scanReq->setChannel(channelList[0]);
		lastSCANChannel = channelList[0];
		break;
	    }
	}
    }
    scanReq->setScanType(0x02);
    scanReq->setScanDuration(5);
    send(scanReq->dup(), outGate);

    delete scanReq;
    delete msg;
}

void SlottedAlohaScheduler::handle_MLME_SCAN_confirm(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *tmp = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    if(tmp->getStatus() == mac_NO_BEACON)
	MLME_SCAN_request(tmp->dup());
    delete tmp;
}

void SlottedAlohaScheduler::MLME_SET_BEACON_request(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *tmp = new Ieee802154eNetworkCtrlInfo("SetSlotRequest",TP_MLME_SET_BEACON_REQUEST);
    send(tmp->dup(),outGate);
    delete tmp;
    delete msg;
}

void SlottedAlohaScheduler::handle_MLME_SET_BEACON_confirm(cMessage *msg)
{

}

void SlottedAlohaScheduler::createInitialEntries()
{
    if(isPANCoor)
    {
	macTimeslotTableEntry *templateEntry = new macTimeslotTableEntry();
	templateEntry->setCCA(def_macTsCCA);
	templateEntry->setCCAOffset(def_macTsCCAOffset);
	templateEntry->setMaxAck(def_macTsMaxAck);
	templateEntry->setMaxTx(def_macTsMaxTx);
	templateEntry->setRxAckDelay(def_macTsRxAckDelay);
	templateEntry->setRxOffset(def_macTsRxOffset);
	templateEntry->setRxTx(def_macTsRxTx);
	templateEntry->setRxWait(def_macTsRxWait);
	templateEntry->setTimeslotLength(def_macTsTimeslotLength);
	templateEntry->setTxAckDelay(def_macTsTxAckDelay);
	templateEntry->setTxOffset(def_macTsTxOffset);
	timeslotTable->addTemplate(templateEntry);

	macSlotframeTableEntry *slotframeEntry = new macSlotframeTableEntry();
	slotframeEntry->setSlotframeId(0);
	slotframeEntry->setSlotframeSize(2);
	slotframeEntry->setSlotframeTable(slotframeTable);
	slotframeTable->addSlotframe(slotframeEntry);

	macLinkTableEntry *linkEntry = new macLinkTableEntry();
	linkEntry->setChannelOffset(10); //Advertisment always with channelOffset 0;
	linkEntry->setLinkOption(LNK_OP_SHARED_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
	linkEntry->setLinkType(LNK_TP_ADVERTISING);
	linkEntry->setMacLinkTable(linkTable);
	linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
	linkEntry->setSlotframeId(0);
	linkEntry->setTimeslot(0);
	linkEntry->setLinkId(0);
	linkTable->addLink(linkEntry);

	//Create Hopping Sequence List for all devices
	macHoppingSequenceListEntry *hoppingEntry = new macHoppingSequenceListEntry();
	hoppingEntry->setChannelPage(0);
	hoppingEntry->setCurrentHop(0);
	hoppingEntry->setExtendedBitmap(ExtendedBitmap);
	hoppingEntry->setHopDwellTime(0);
	hoppingEntry->setHoppingSequenceId(def_macHoppingSequenceID);
	std::vector<UINT_16> tempList;
	for(int i = minChannelNum; i < maxChannelNum; i++)
	    tempList.push_back(i);
	hoppingEntry->setHoppingSequenceLength(tempList.size());
	hoppingEntry->setHoppingSequenceList(tempList);
	hoppingEntry->setNumberOfChannels(tempList.size());
	hoppingEntry->setPhyConfiguration(PhyConfig);
	hoppingSequenceList->addEntry(hoppingEntry);
    }

}

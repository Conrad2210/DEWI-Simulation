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

#include <SlottedAloha/SlottedAloha.h>
#include "Ieee802154eConst.h"
#include "Ieee802154eNetworkCtrlInfo_m.h"
#include "Ieee802154eFrame_m.h"

Define_Module(SlottedAloha);

static const int PhyConfig = 134215680;
static const int ExtendedBitmap = 0;
static const int minChannelNum = 11;
static const int maxChannelNum = 27;
static const int channelList[16] =
{
11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26
};
static const int numChannel = 16;

SlottedAloha::SlottedAloha()
{
    // TODO Auto-generated constructor stub
    lastSCANChannel = 0;
    notAssociated = true;
}

SlottedAloha::~SlottedAloha()
{
    // TODO Auto-generated destructor stub
    cancelAndDelete(AssociateTimer);

    cancelAndDelete( BeaconTimer);
    cancelAndDelete( StartTimer);
    cancelAndDelete( ScheduleTimer);
    cancelAndDelete( ScanTimer);

}

void SlottedAloha::initialize(int stage)
{

    if(stage != 4)
	return;
    slotframeTable = check_and_cast<IMacSlotframeTable *>(getModuleByPath(par("macSlotframeTableModule")));

    // get a pointer to the LinkTable Module
    linkTable = check_and_cast<IMacLinkTable *>(getModuleByPath(par("macLinkTableModule")));
    // get a pointer to the TimeslotTable Module
    timeslotTable = check_and_cast<IMacTimeslotTable *>(getModuleByPath(par("macTimeslotTableModule")));
    // get a pointer to the HoppingSequenceList Module
    hoppingSequenceList = check_and_cast<IMacHoppingSequenceList *>(getModuleByPath(par("macHoppingSequenceListModule")));

    // get a pointer to the NeighborTable Module
    neighborTable = check_and_cast<IMacNeighborTable *>(getModuleByPath(par("macNeighborTableModule")));

    lastNeighbor = NULL;
    tempLinkEntryRx = new macLinkTableEntry();
    tempLinkEntryTx = NULL;
    firstLink = true;
    inGate = findGate("from_mac");
    outGate = findGate("to_mac");

    isPANCoor = par("isPANCoor").boolValue();
    AssociateTimer = new cMessage("AssociationTimer", ASSOCIATION_TIMER);
    ScheduleTimer = new cMessage("ScheduleTimer", SCHEDULE_TIMER);
    BeaconTimer = new cMessage("BeaconTimer", BEACON_REQUEST);
    StartTimer = new cMessage("StartTimer", START_TIMER);
    AssociateWaitTimer = new cMessage("AssociationTimer", ASSOCIATION_WAIT_TIMER);
    ScanTimer = new cMessage("ScanTimer", MAC_SCAN_TIMER);

    createInitialEntries();

    double start = 0;

    const char* temp = getParentModule()->getParentModule()->getName();
    if(!isPANCoor)
    {
	if(!strcmp(temp, "lightSwitch"))
	{
	    start = par("StartTime").doubleValue();
	}
	else
	{
	    int index = getParentModule()->getParentModule()->getIndex();
	    start = index * 60.0;
	}
    }
    else
    {
	start = 0;
    }
    scheduleAt(simTime() + start, StartTimer);
    WATCH(notAssociated);

}

void SlottedAloha::handleMessage(cMessage *msg)
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

void SlottedAloha::handleMACMessage(cMessage *msg)
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
	case TP_MLME_SET_BEACON_CONFIRM:
	{
	    handle_MLME_SET_BEACON_confirm(msg);
	    break;
	}
	case TP_MLME_ASSOCIATE_INDICATION:
	{
	    handle_MLME_ASSOCIATE_indication(msg);
	    break;
	}
	case TP_MLME_ASSOCIATE_CONFIRM:
	{
	    handle_MLME_ASSOCIATE_confirm(msg);
	    break;
	}
	case TP_SCHEDULE_INDICATION:
	{
	    handle_SCHEDULE_indication(msg);
	    break;
	}
	case TP_SCHEDULE_CONFIRM:
	{
	    handle_SCHEDULE_confirm(msg);
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
void SlottedAloha::handleSelfMessage(cMessage *msg)
{
    switch(msg->getKind())
    {
	case START_TIMER:
	{
	    MLME_START_request(msg->dup());
	    break;
	}
	case MAC_SCAN_TIMER:
	{
	    MLME_SCAN_request(msg->dup());
	    break;
	}
	case BEACON_REQUEST:
	{
	    MLME_BEACON_request(msg->dup());
	    break;
	}
	case ASSOCIATION_TIMER:
	{
	    MLME_ASSOCIATE_request(msg->dup());
	    break;
	}
	case ASSOCIATION_WAIT_TIMER:
	{
	    MLME_ASSOCIATE_request(msg->dup());
	    break;
	}
	case SCHEDULE_TIMER:
	{
	    SCHEDULE_request(msg->dup());
	    break;
	}
	case SCHEDULE_WAIT_TIMER:
	{
//	    ScheduleTimer = new cMessage("ScheduleTimer", SCHEDULE_TIMER);
//	    double tempTime = 0 + ((double)rand() / RAND_MAX) * (0.1 - 0);
//	    scheduleAt(simTime() + tempTime, ScheduleTimer);
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
void SlottedAloha::MLME_ASSOCIATE_request(cMessage *msg)
{
    if(notAssociated)
    {
	if(!AssociateWaitTimer->isScheduled())
	{
	    Ieee802154eNetworkCtrlInfo *tmp = new Ieee802154eNetworkCtrlInfo("AssociationRequest", TP_MLME_ASSOCIATE_REQUEST);
	    send(tmp->dup(), outGate);
	    scheduleAt(simTime() + par("AssociateWaitTime"), AssociateWaitTimer);
	    delete tmp;
	}

    }
}

void SlottedAloha::handle_MLME_ASSOCIATE_indication(cMessage *msg)
{
    MLME_ASSOCIATE_response(msg->dup());
    delete msg;
}

void SlottedAloha::MLME_ASSOCIATE_response(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *tmp = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    Ieee802154eNetworkCtrlInfo *AssRes = new Ieee802154eNetworkCtrlInfo("AssociationResponse", TP_MLME_ASSOCIATE_RESPONSE);
    AssRes->setDeviceAddress(tmp->getDeviceAddress());
    AssRes->setAssocShortAddress(tmp->getSrcAddr());
    AssRes->setStatus(mac_FastA_successful);
    AssRes->setChannelOffset(tmp->getChannelOffset());
    send(AssRes->dup(), outGate);
    delete AssRes;
    delete tmp;
}

void SlottedAloha::handle_MLME_ASSOCIATE_confirm(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *tmp = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    if(AssociateWaitTimer->isScheduled())
	cancelEvent(AssociateWaitTimer);
    if(tmp->getStatus() == mac_FastA_successful)
    {

	notAssociated = false;
	cDisplayString* parentDisp = &getParentModule()->getParentModule()->getDisplayString();
	    cDisplayString* tempStr = new cDisplayString();
	const char* temp = getParentModule()->getParentModule()->getName();
	if(!strcmp(temp, "lightSwitch"))
	{
	    tempStr->parse("b=0.1,0.1,rect;i=lighting/lightswitch");
	}
	else
	{
	    tempStr->parse("b=1.5,1.5,oval,green;i=status/bulb");
	}

	if(ScheduleTimer->isScheduled())
	    cancelEvent(ScheduleTimer);
	double tempTime = 0 + ((double)rand() / RAND_MAX) * (0.1 - 0);
	scheduleAt(simTime() + tempTime, ScheduleTimer);
	    parentDisp->updateWith(*tempStr);
    }
    else
    {

	notAssociated = true;
	cDisplayString* parentDisp = &getParentModule()->getParentModule()->getDisplayString();
	    cDisplayString* tempStr = new cDisplayString();
	const char* temp = getParentModule()->getParentModule()->getName();
	if(!strcmp(temp, "lightSwitch"))
	{
	    tempStr->parse("b=0.1,0.1,rect;i=lighting/lightswitch");
	}
	else
	{
	    tempStr->parse("b=1.5,1.5,oval,yellow;i=status/bulb");
	}
	scheduleAt(simTime(), AssociateTimer);
	parentDisp->updateWith(*tempStr);
    }


    delete tmp;
}

//Dissassociation Process
void SlottedAloha::MLME_DISASSOCIATE_request(cMessage *msg)
{
}

void SlottedAloha::handle_MLME_DIASSOCIATE_indication(cMessage *msg)
{
}

void SlottedAloha::handle_MLME_DIASSOCIATE_confirm(cMessage *msg)
{
}

//Advertisment

void SlottedAloha::MLME_BEACON_request(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *tempMsg = new Ieee802154eNetworkCtrlInfo("BeaconRequest", TP_MLME_BEACON_REQUEST);
    tempMsg->setBeaconType(0x01);
    tempMsg->setChannel(11);

    send(tempMsg->dup(), outGate);
    delete tempMsg;
    delete msg;

}

void SlottedAloha::handle_MLME_BEACON_confirm(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *beaconCon = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    double time = pow(2, beaconCon->getBeaconOrder()) * beaconCon->getSuperframeOrder() * timeslotTable->getTemplate(0)->getTimeslotLengthDbl();

    scheduleAt(simTime() + time, BeaconTimer);
    delete beaconCon;
}

void SlottedAloha::handle_MLME_BEACON_indication(cMessage *msg)
{
    MLME_SET_BEACON_request(msg);
}

void SlottedAloha::MLME_SETSLOTFRAME_request(cMessage *msg)
{

}

void SlottedAloha::handle_MLME_SETSLOTFRAME_confirm(cMessage *msg)
{
}

void SlottedAloha::MLME_SETLINK_request(cMessage *msg)
{
}

void SlottedAloha::handle_MLME_SETLINK_confirm(cMessage *msg)
{
}

void SlottedAloha::MLME_TSCHMODE_request(cMessage *msg)
{
}

void SlottedAloha::handle_MLME_TSCHMODE_confirm(cMessage *msg)
{
}

//////START
void SlottedAloha::MLME_START_request(cMessage *msg)
{

    Ieee802154eNetworkCtrlInfo *startMsg = new Ieee802154eNetworkCtrlInfo("StartMsg", TP_MLME_START_REQUEST);
    startMsg->setPanCoordinator(par("isPANCoor").boolValue());
    startMsg->setStartTime((uint32_t)par("StartTime").doubleValue());
    send(startMsg->dup(), outGate);
    delete startMsg;
    delete msg;
}

void SlottedAloha::handle_MLME_START_confirm(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *startCo = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    cDisplayString* parentDisp = &getParentModule()->getParentModule()->getDisplayString();
    cDisplayString* tempStr = new cDisplayString();
    const char* temp = getParentModule()->getParentModule()->getName();
    if(!strcmp(temp, "lightSwitch"))
    {
	tempStr->parse("b=0.1,0.1,rect;i=lighting/lightswitch");
    }
    else
    {
	if(isPANCoor)
	    tempStr->parse("b=1.5,1.5,oval,blue;i=status/bulb");
	else
	    tempStr->parse("b=1.5,1.5,oval,yellow;i=status/bulb");
    }
    if(startCo->getPanCoordinator())
    {
	MLME_BEACON_request(msg);
    }
    else
    {
	MLME_SCAN_request(msg);
    }
    if(isPANCoor)
    {
//	if(ScheduleTimer->isScheduled())
//	    cancelEvent(ScheduleTimer);
//	scheduleAt(simTime() + 60, ScheduleTimer);
    }
    parentDisp->updateWith(*tempStr);
}

//Scan
void SlottedAloha::MLME_SCAN_request(cMessage *msg)
{

    Ieee802154eNetworkCtrlInfo *scanReq = new Ieee802154eNetworkCtrlInfo("ScanRequest", TP_MLME_SCAN_REQUEST);
    if(!dynamic_cast<Ieee802154EnhancedBeaconFrame *>(msg))
    {
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
	send(scanReq->dup(), outGate);

	double tempTime = aBaseSuperframeDuration * pow(2, 5) / 62.5e3;
	if(ScanTimer->isScheduled())
	    cancelEvent(ScanTimer);

	scheduleAt(simTime() + tempTime, ScanTimer);
    }
    else
    {
	scanReq->setScanType(0x00);
	send(scanReq->dup(), outGate);
	if(ScanTimer->isScheduled())
		    cancelEvent(ScanTimer);
    }
    delete scanReq;
    delete msg;
}

void SlottedAloha::handle_MLME_SCAN_confirm(cMessage *msg)
{
    Ieee802154EnhancedBeaconFrame *tmp = check_and_cast<Ieee802154EnhancedBeaconFrame *>(msg);
    if(ScanTimer->isScheduled())
	cancelEvent(ScanTimer);

    MLME_SCAN_request(tmp->dup());
    MLME_SET_BEACON_request(tmp->dup());



    delete tmp;

}

void SlottedAloha::MLME_SET_BEACON_request(cMessage *msg)
{

    if(dynamic_cast<Ieee802154EnhancedBeaconFrame *>(msg))
    {
	Ieee802154EnhancedBeaconFrame * tmp = check_and_cast<Ieee802154EnhancedBeaconFrame *>(msg);
	tmp->setKind(TP_MLME_SET_BEACON_REQUEST);
	tmp->setName("SetSlotRequest");
	send(tmp->dup(), outGate);
	delete tmp;
    }
    else
    {

	Ieee802154eNetworkCtrlInfo *tmp = new Ieee802154eNetworkCtrlInfo("SetSlotRequest", TP_MLME_SET_BEACON_REQUEST);

	send(tmp->dup(), outGate);
	delete tmp;
	delete msg;
    }

}

void SlottedAloha::handle_MLME_SET_BEACON_confirm(cMessage *msg)
{
    EV << "[SCHEDULER]: Recieved information from coordinator, start Association process" << endl;

    scheduleAt(simTime(), AssociateTimer);
    delete msg;
}

void SlottedAloha::createInitialEntries()
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
	slotframeEntry->setSlotframeSize(3);
	slotframeEntry->setSlotframeTable(slotframeTable);
	slotframeTable->addSlotframe(slotframeEntry);

	macLinkTableEntry *linkEntry = new macLinkTableEntry();
	linkEntry->setChannelOffset(0); //Advertisment always with channelOffset 0;
	linkEntry->setLinkOption(LNK_OP_SHARED_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
	linkEntry->setLinkType(LNK_TP_ADVERTISING);
	linkEntry->setMacLinkTable(linkTable);
	linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
	linkEntry->setSlotframeId(0);
	linkEntry->setTimeslot(0);
	linkEntry->setLinkId(linkTable->getNumLinks());
	linkTable->addLink(linkEntry);

	linkEntry = new macLinkTableEntry();
	linkEntry->setChannelOffset(0); //Advertisment always with channelOffset 0;
	linkEntry->setLinkOption(LNK_OP_SHARED_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
	linkEntry->setLinkType(LNK_TP_JOIN);
	linkEntry->setMacLinkTable(linkTable);
	linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
	linkEntry->setSlotframeId(0);
	linkEntry->setTimeslot(1);
	linkEntry->setLinkId(linkTable->getNumLinks());
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
void SlottedAloha::SCHEDULE_request(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *tmp = new Ieee802154eNetworkCtrlInfo("ScheduleRequest", TP_SCHEDULE_REQUEST);
    bool linkExists = false;
//    if(tempLinkEntryTx == NULL || linkTable->existLink(tempLinkEntryTx))
//    {
//	firstLink = false;

    if(lastNeighbor == NULL)
    {
	lastNeighbor = neighborTable->getNeighborByPos(0, false);
    }

    for(int k = 0; k < linkTable->getNumLinks(); k++)
    {
	if(linkTable->getLink(k)->getNodeAddress() == lastNeighbor->getShortAddress() && linkTable->getLink(k)->getLinkType() == LNK_TP_NORMAL /*
	 && linkTable->getLink(k)->getNodeAddress() != tempLinkEntryTx->getNodeAddress()
	 && linkTable->getLink(k)->getChannelOffset() != tempLinkEntryTx->getChannelOffset()
	 && linkTable->getLink(k)->getTimeslot() != tempLinkEntryTx->getTimeslot()*/)
	    linkExists = true;
    }
    if(!linkExists)
    {
	tempLinkEntryTx = new macLinkTableEntry();
	tempLinkEntryTx->setChannelOffset(1);
	tempLinkEntryTx->setLinkId(linkTable->getNumLinks());
	tempLinkEntryTx->setLinkOption(LNK_OP_SHARED_RECEIVE);
	tempLinkEntryTx->setLinkType(LNK_TP_NORMAL);
	tempLinkEntryTx->setNodeAddress(lastNeighbor->getShortAddress());
	tempLinkEntryTx->setSlotframeId(0);
	tempLinkEntryTx->setTimeslot(2);

	tmp->setDeviceAddrMode(defFrmCtrl_AddrMode64);
	tmp->setDeviceAddress(lastNeighbor->getExtendedAddress().getInt());
	tmp->setTimeslot(tempLinkEntryTx->getTimeslot());
	tmp->setChannelOffset(tempLinkEntryTx->getChannelOffset());
	send(tmp->dup(), outGate);

	if(ScheduleTimer->isScheduled())
	    cancelEvent(ScheduleTimer);
	double tempTime = 1 + ((double)rand() / RAND_MAX) * (2 - 1);
	scheduleAt(simTime() + tempTime, ScheduleTimer);
    }
    else if(neighborTable->getNeighborById(lastNeighbor->getNeighborId() + 1) != NULL)
    {
	lastNeighbor = neighborTable->getNeighborById(lastNeighbor->getNeighborId() + 1);
	if(ScheduleTimer->isScheduled())
	    cancelEvent(ScheduleTimer);

	scheduleAt(simTime(), ScheduleTimer);
    }
    else if(lastNeighbor == neighborTable->getNeighborByPos(neighborTable->getNumNeighbors() - 1, false))
    {
	for(int i = 0; i < neighborTable->getNumNeighbors(); i++)
	{
	    linkExists = false;
	    lastNeighbor = neighborTable->getNeighborByPos(i, false);
	    for(int k = 0; k < linkTable->getNumLinks(); k++)
	    {
		if(linkTable->getLink(k)->getNodeAddress() == lastNeighbor->getShortAddress() && linkTable->getLink(k)->getLinkType() == LNK_TP_NORMAL)
		    linkExists = true;
	    }
	    if(!linkExists)
	    {
		if(ScheduleTimer->isScheduled())
		    cancelEvent(ScheduleTimer);
		scheduleAt(simTime(), ScheduleTimer);
		break;
	    }

	}

	if(ScheduleTimer->isScheduled())
	    cancelEvent(ScheduleTimer);
	double tempTime = 1 + ((double)rand() / RAND_MAX) * (0.5 - 0.4);
	scheduleAt(simTime() + tempTime, ScheduleTimer);

    }
    else
    {
	if(ScheduleTimer->isScheduled())
	    cancelEvent(ScheduleTimer);
	double tempTime = 0.5 + ((double)rand() / RAND_MAX) * (1 - 0.5);
	scheduleAt(simTime() + tempTime, ScheduleTimer);
    }
//    }
//    else
//    {
//	if(ScheduleTimer->isScheduled())
//	    cancelEvent(ScheduleTimer);
//	double tempTime = 0 + ((double)rand() / RAND_MAX) * (1 - 0);
//	scheduleAt(simTime() + tempTime, ScheduleTimer);
//    }

    delete tmp;
    //delete msg;

}

void SlottedAloha::handle_SCHEDULE_indication(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *Cntrl = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    tempLinkEntryRx = new macLinkTableEntry();

    tempLinkEntryRx->setChannelOffset(Cntrl->getChannelOffset());
    tempLinkEntryRx->setLinkId(linkTable->getNumLinks());
    tempLinkEntryRx->setLinkOption(LNK_OP_SHARED_RECEIVE);
    tempLinkEntryRx->setLinkType(LNK_TP_NORMAL);
    tempLinkEntryRx->setNodeAddress(Cntrl->getDeviceAddress());
    tempLinkEntryRx->setSlotframeId(0);
    tempLinkEntryRx->setTimeslot(Cntrl->getTimeslot());

    if(!linkTable->existLink(tempLinkEntryRx))
    {
	Cntrl->setStatus(mac_SUCCESS);
	Cntrl->setDeviceAddress(neighborTable->getNeighborBySAddr(Cntrl->getDeviceAddress())->getExtendedAddress().getInt());
    }
    else
    {
	Cntrl->setStatus(mac_LINK_EXISTS);
	Cntrl->setDeviceAddress(neighborTable->getNeighborBySAddr(Cntrl->getDeviceAddress())->getExtendedAddress().getInt());

    }

    SCHEDULE_response(Cntrl->dup());

    delete Cntrl;

}

void SlottedAloha::SCHEDULE_response(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *Cntrl = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);

    Cntrl->setName("SchedulerResponse");
    Cntrl->setKind(TP_SCHEDULE_RESPONSE);

    send(Cntrl->dup(), outGate);

    delete Cntrl;

}

void SlottedAloha::handle_SCHEDULE_confirm(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *Ctrl = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    cDisplayString* parentDisp = &getParentModule()->getParentModule()->getDisplayString();
    cDisplayString* tempStr = new cDisplayString();
    const char* temp = getParentModule()->getParentModule()->getName();
    if(!Ctrl->getReceivedByACK())
    {
	if(Ctrl->getStatus() == mac_SUCCESS)
	{
	    linkTable->addLink(tempLinkEntryTx);
	    if(!strcmp(temp, "lightSwitch"))
	    {
		tempStr->parse("b=0.1,0.1,rect;i=lighting/lightswitch");
	    }
	    else
	    {

		tempStr->parse("b=1.5,1.5,oval,blue;i=status/bulb");
	    }
	}
	else if(Ctrl->getStatus() == mac_LINK_EXISTS)
	{
	    linkTable->addLink(tempLinkEntryTx);
	    if(!strcmp(temp, "lightSwitch"))
	    {
		tempStr->parse("b=0.1,0.1,rect;i=lighting/lightswitch");
	    }
	    else
	    {

		tempStr->parse("b=1.5,1.5,oval,blue;i=status/bulb");
	    }
	}

	if(ScheduleTimer->isScheduled())
	    cancelEvent(ScheduleTimer);
	//double tempTime = 0.4 + ((double)rand() / RAND_MAX) * (0.5 - 0.4);
	scheduleAt(simTime(), ScheduleTimer);
    }
    else
    {
	if(Ctrl->getStatus() == mac_SUCCESS)
	{
	    linkTable->addLink(tempLinkEntryRx);
	    if(!strcmp(temp, "lightSwitch"))
	    {
		tempStr->parse("b=0.1,0.1,rect;i=lighting/lightswitch");
	    }
	    else
	    {

		tempStr->parse("b=1.5,1.5,oval,blue;i=status/bulb");
	    }
	}
	else if(Ctrl->getStatus() == mac_LINK_EXISTS)
	{
	    tempLinkEntryRx = new macLinkTableEntry();

	}
    }
    parentDisp->updateWith(*tempStr);
}

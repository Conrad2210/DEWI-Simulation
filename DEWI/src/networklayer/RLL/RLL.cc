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

#include <RLL.h>
#include <Ieee802154eNetworkCtrlInfo_m.h>
#include "Ieee802154eFrame_m.h"
#include <Radio80211aControlInfo_m.h>

Define_Module(RLL);

static const int PhyConfig = 134215680;
static const int ExtendedBitmap = 0;
static const int minChannelNum = 11;
static const int maxChannelNum = 27;
static const int channelList[16] =
{
11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26
};
static const int numChannel = 15;

RLL::RLL()
{
    nLastSCANChannel = 0;
    bNotAssociated = false;
    nCluStage = -1;
    AssociateTimer = NULL;
    AssociateWaitTimer = NULL;
    ScheduleTimer = NULL;
    BeaconScanTimer = NULL;
    DisassociateWaitTimer = NULL;
    BeaconTimer = NULL;
    ScanTimer = NULL;
}

RLL::~RLL()
{

	cancelAndDelete(AssociateTimer);
	cancelAndDelete(ScheduleTimer);
	cancelAndDelete(BeaconTimer);
	cancelAndDelete(StartTimer);
	cancelAndDelete(AssociateWaitTimer);
	cancelAndDelete(ScanTimer);
	cancelAndDelete(BeaconScanTimer);
	cancelAndDelete(DisassociateWaitTimer);
}

void RLL::initialize(int stage)
{
    if(stage == 0)
    {

	bCapablePanCoor = par("capablePanCoor").boolValue();
	if(bCapablePanCoor)
	    bIsPANCoor = par("isPANCoor").boolValue();
	else
	    bIsPANCoor = false;
    }
    else if(stage == 2)
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

	clusterTable = check_and_cast<IRLLClusterTable *>(getModuleByPath(par("RLLClusterTableModule")));

	beaconTable = check_and_cast<cBeaconTable *>(getModuleByPath(par("cBeaconTableModule")));
	lowerLayerIn = findGate("lowerLayerIn");
	lowerLayerOut = findGate("lowerLayerOut");
	upperLayerIn = findGate("upperLayerIn");
	upperLayerOut = findGate("upperLayerOut");

    }
    else if(stage == 3)
    {
	//timer init
	AssociateTimer = new cMessage("AssociationTimer", ASSOCIATION_TIMER);
	AssociateWaitTimer = new cMessage("AssociationWaitTimer", ASSOCIATION_WAIT_TIMER);
	ScheduleTimer = new cMessage("ScheduleTimer", SCHEDULE_TIMER);
	BeaconScanTimer = new cMessage("BeaconScanTimer", SCHEDULE_BEACON_SCAN_TIMER);
	DisassociateWaitTimer = new cMessage("DisassociateWaitTimer", DISASSOCIATION_WAIT_TIMER);
	BeaconTimer = new cMessage("BeaconTimer", BEACON_REQUEST);
	ScanTimer = new cMessage("ScanTimer", MAC_SCAN_TIMER);
	StartTimer = new cMessage("StartTimer", START_TIMER);
    }
    else if(stage == 4)
    {
	if(bIsPANCoor)
	{
	    nCluStage = 0;
	    createInitialEntries();
	    nScanDuration = -1;
	}
	else
	{
	    nCluStage = -1;
	    nScanDuration = 0;
	}
	nRestartCounter = 0;
	bNotAssociated = true;
	bAssociateDirectly = false;
	nScanCounter = 0;
	nLastSCANChannel = 0;

	WATCH(nLastSCANChannel);
	WATCH(nScanDuration);

	double start = 0.0;
	if(!bIsPANCoor)
	{

	    start = par("StartTime").doubleValue();

	}
	else
	{
	    start = 0;
	}
	scheduleAt(simTime() + start, StartTimer);
    }
}
void RLL::finish()
{

}

void RLL::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage())
    {
	handleSelfMessage(msg);
    }
    else if(msg->getArrivalGateId() == lowerLayerIn)
    {
	handleMACMessage(msg);
    }
    else if(msg->getArrivalGateId() == upperLayerIn)
    {
	//TODO: handle Upper Message
    }
    else
    {
	if(ev.isGUI())
	{
	    EV << "Undefined Message type; Drop Message" << endl;

	}
	delete msg;
    }
    updatedisplay();
}

void RLL::handleMACMessage(cMessage *msg)
{
    switch(msg->getKind())
    {
	case TP_MLME_START_CONFIRM:
	{
	    handle_MLME_START_confirm(msg);
	    break;
	}
	case TP_MLME_BEACON_CONFIRM:
	{
	    handle_MLME_BEACON_confirm(msg);
	    break;
	}
	case TP_MLME_SCAN_CONFIRM:
	{
	    handle_MLME_SCAN_confirm(msg);
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
	case TP_MLME_DISASSOCIATE_INDICATION:
	{
	    handle_MLME_DIASSOCIATE_indication(msg);
	    break;
	}
	case TP_MLME_DISASSOCIATE_CONFIRM:
	{
	    handle_MLME_DIASSOCIATE_confirm(msg);
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
	case TP_RESTART_CONFIRM:
	{
	    handle_RESTART_confirm(msg->dup());
	    delete msg;
	    break;
	}
	    // TODO: Add case for handling data messages

	default:
	{
	    delete msg;
	    break;
	}
    }
}

void RLL::handleSelfMessage(cMessage *msg)
{
    switch(msg->getKind())
    {
	case START_TIMER:
	{
	    MLME_START_request(NULL);
	    break;
	}
	case MAC_SCAN_TIMER:
	{
	    MLME_SCAN_request(NULL);
	    break;
	}
	case BEACON_REQUEST:
	{
	    MLME_BEACON_request(NULL);
	    break;
	}
	case ASSOCIATION_TIMER:
	{
	    MLME_ASSOCIATE_request(NULL);
	    break;
	}
	case SCHEDULE_TIMER:
	{
	    SCHEDULE_request(NULL);
	    break;
	}
	case SCHEDULE_BEACON_SCAN_TIMER:
	{
	    handle_BEACON_WAIT_timer(NULL);
	    break;
	}
	case DISASSOCIATION_WAIT_TIMER:
	{
	    MLME_DISASSOCIATE_request(NULL);
	    break;
	}
	case ASSOCIATION_WAIT_TIMER:
	{
	    MLME_ASSOCIATE_request(NULL);
	    break;
	}
	default:
	{
	    break;
	}

    }
}

//Association Process
void RLL::MLME_ASSOCIATE_request(cMessage *msg)
{
    if(bNotAssociated)
    {
	if(!AssociateTimer->isScheduled())
	{
	    Ieee802154eNetworkCtrlInfo *tmp = new Ieee802154eNetworkCtrlInfo("AssociationRequest", TP_MLME_ASSOCIATE_REQUEST);
	    tmp->setPanCoordinator(bIsPANCoor);
	    send(tmp->dup(), lowerLayerOut);
	    if(AssociateWaitTimer->isScheduled())
		cancelEvent(AssociateWaitTimer);
	    scheduleAt(simTime() + 5, AssociateWaitTimer);
	    delete tmp;
	    tmp = NULL;
	}
    }

}

void RLL::handle_MLME_ASSOCIATE_indication(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *tmp = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    if(tmp->getPanCoordinator() && bIsPANCoor)
    {
	clusterTable->addEntry(nCluStage + 1, tmp->getAssocShortAddress(), (char*)"", tmp->getPanCoordinator(), tmp->getPanId());
	//tmp->getPanCoordinator(), nCluStage + 1, tmp->getAssocShortAddress(), tmp->getPanId());
	macNeighborTableEntry* tmpEntry = neighborTable->getNeighborBySAddr(tmp->getAssocShortAddress());
	tmpEntry->setStage(nCluStage + 1);
	tmpEntry->isNextStageCH(true);
	tmpEntry->isSameStageCH(false);
	tmpEntry->isPrevStageCH(false);
	tmpEntry->isMyCS(false);
	tmpEntry->isMyCH(false);
    }
    else if(tmp->getPanCoordinator() && !bIsPANCoor)
    {
	clusterTable->addEntry(nCluStage, tmp->getAssocShortAddress(), (char*)"", tmp->getPanCoordinator(), tmp->getPanId());
	macNeighborTableEntry* tmpEntry = neighborTable->getNeighborBySAddr(tmp->getAssocShortAddress());
	tmpEntry->setStage(nCluStage);
	tmpEntry->isMyCH(tmp->getPanCoordinator());
	tmpEntry->isNextStageCH(false);
	tmpEntry->isSameStageCH(true);
	tmpEntry->isPrevStageCH(false);
	tmpEntry->isMyCS(false);
	tmpEntry->isMyCH(true);
    }
    else
    {
	clusterTable->addEntry(nCluStage, tmp->getAssocShortAddress(), (char*)"", false, tmp->getPanId());
	macNeighborTableEntry* tmpEntry = neighborTable->getNeighborBySAddr(tmp->getAssocShortAddress());
	tmpEntry->setStage(nCluStage);
	tmpEntry->isNextStageCH(false);
	tmpEntry->isSameStageCH(false);
	tmpEntry->isPrevStageCH(false);
	tmpEntry->isMyCS(true);
	tmpEntry->isMyCH(false);
    }
    MLME_ASSOCIATE_response(tmp->dup());
    delete tmp;
    tmp = NULL;
    msg = NULL;
}

void RLL::MLME_ASSOCIATE_response(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *tmp = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    Ieee802154eNetworkCtrlInfo *AssRes = new Ieee802154eNetworkCtrlInfo("AssociationResponse", TP_MLME_ASSOCIATE_RESPONSE);
    AssRes->setDeviceAddress(tmp->getDeviceAddress());
    AssRes->setAssocShortAddress(tmp->getSrcAddr());
    AssRes->setStatus(mac_FastA_successful);
    AssRes->setChannelOffset(tmp->getChannelOffset());
    AssRes->setStage(nCluStage);
    AssRes->setPanCoordinator(bIsPANCoor);
    send(AssRes->dup(), lowerLayerOut);
    delete AssRes;
    AssRes = NULL;
    delete tmp;
    tmp = NULL;
}

void RLL::handle_MLME_ASSOCIATE_confirm(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *tmp = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    if(AssociateWaitTimer->isScheduled())
	cancelEvent(AssociateWaitTimer);
    if(AssociateTimer->isScheduled())
	cancelEvent(AssociateTimer);
    if(tmp->getStatus() == mac_FastA_successful)
    {
	bNotAssociated = false;
	cDisplayString* parentDisp = &getParentModule()->getDisplayString();
	cDisplayString* tempStr = new cDisplayString();

	const char* temp = getParentModule()->getName();
	if(!strcmp(temp, "lightSwitch"))
	{
	    tempStr->parse("b=0.1,0.1,rect;i=lighting/lightswitch");
	}
	else
	{
	    tempStr->parse("b=1.5,1.5,oval,green;i=status/bulb");

	    bubble("test");
	    if(bIsPANCoor)
		tempStr->parse("b=1.5,1.5,oval,green;i=device/accesspoint");
	}

	if(tmp->getPanCoordinator() && bIsPANCoor)
	{
	    nCluStage = tmp->getStage() + 1;
	    clusterTable->addEntry(nCluStage - 1, tmp->getAssocShortAddress(), (char*)"", tmp->getPanCoordinator(), tmp->getPanId());
	    macNeighborTableEntry* tmpEntry = neighborTable->getNeighborBySAddr(tmp->getAssocShortAddress());
	    tmpEntry->setStage(nCluStage - 1);
	    tmpEntry->isNextStageCH(false);
	    tmpEntry->isSameStageCH(false);
	    tmpEntry->isPrevStageCH(true);
	    tmpEntry->isMyCS(false);
	    tmpEntry->isMyCH(false);
	}
	else if(tmp->getPanCoordinator() && !bIsPANCoor)
	{
	    nCluStage = tmp->getStage();
	    clusterTable->addEntry(nCluStage, tmp->getAssocShortAddress(), (char*)"", tmp->getPanCoordinator(), tmp->getPanId());
	    macNeighborTableEntry* tmpEntry = neighborTable->getNeighborBySAddr(tmp->getAssocShortAddress());
	    tmpEntry->setStage(nCluStage);
	    tmpEntry->isMyCH(tmp->getPanCoordinator());
	    tmpEntry->isNextStageCH(false);
	    tmpEntry->isSameStageCH(true);
	    tmpEntry->isPrevStageCH(false);
	    tmpEntry->isMyCS(false);
	    tmpEntry->isMyCH(true);
	}
	else
	{
	    clusterTable->addEntry(nCluStage, tmp->getAssocShortAddress(), (char*)"", false, tmp->getPanId());
	    macNeighborTableEntry* tmpEntry = neighborTable->getNeighborBySAddr(tmp->getAssocShortAddress());
	    tmpEntry->setStage(nCluStage);
	    tmpEntry->isNextStageCH(false);
	    tmpEntry->isSameStageCH(false);
	    tmpEntry->isPrevStageCH(false);
	    tmpEntry->isMyCS(true);
	    tmpEntry->isMyCH(false);
	}
	parentDisp->updateWith(*tempStr);

	if(!bIsPANCoor)
	{
	    if(ScheduleTimer->isScheduled())
		cancelEvent(ScheduleTimer);

	    scheduleAt(simTime() + ((double)rand() / (RAND_MAX)), ScheduleTimer);
	}
	else
	{
	    if(BeaconScanTimer->isScheduled())
		cancelEvent(BeaconScanTimer);

	    scheduleAt(simTime() + fBI * 2, BeaconScanTimer);
	}
	setSchedule();
	    delete tempStr;
	    tempStr = NULL;
    }
    else
    {
	bNotAssociated = true;
	cDisplayString* parentDisp = &getParentModule()->getDisplayString();
	cDisplayString* tempStr = new cDisplayString();
	const char* temp = getParentModule()->getName();
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
	    delete tempStr;
	    tempStr = NULL;
    }

    delete tmp;
    tmp = NULL;
}

//Dissassociation Process
void RLL::MLME_DISASSOCIATE_request(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *cnt = new Ieee802154eNetworkCtrlInfo("DisassociationRequest", TP_MLME_DISASSOCIATE_REQUEST);
    //scheduleAt(simTime()+2,DisassociateWaitTimer);
    send(cnt->dup(), lowerLayerOut);
    delete cnt;
    cnt = NULL;
}

void RLL::handle_MLME_DIASSOCIATE_indication(cMessage *msg)
{
    Ieee802154eDisassociationFrame *tmpMsg = check_and_cast<Ieee802154eDisassociationFrame *>(msg);

    clusterTable->deleteEntry(clusterTable->getEntryByShrtAddr(tmpMsg->getSrcAddr().getInt()));
    MLME_DISASSOCIATE_response(tmpMsg->dup());

    delete tmpMsg;
    tmpMsg = NULL;
    msg = NULL;
}
void RLL::MLME_DISASSOCIATE_response(cMessage *msg)
{
    Ieee802154eDisassociationFrame *tmpMsg = check_and_cast<Ieee802154eDisassociationFrame *>(msg);
    tmpMsg->setKind(TP_MLME_DISASSOCIATE_RESPONSE);
    send(tmpMsg->dup(), lowerLayerOut);
    delete tmpMsg;
    tmpMsg = NULL;
    msg = NULL;

}

void RLL::handle_MLME_DIASSOCIATE_confirm(cMessage *msg)
{
    Ieee802154eDisassociationFrame *tmpMsg = check_and_cast<Ieee802154eDisassociationFrame *>(msg);
    clusterTable->deleteEntry(clusterTable->getEntryByShrtAddr(tmpMsg->getSrcAddr().getInt()));
    if(DisassociateWaitTimer->isScheduled())
	cancelEvent(DisassociateWaitTimer);
    RESTART_request(NULL);

    delete tmpMsg;
    tmpMsg = NULL;
    msg = NULL;
}

//Advertisment

void RLL::MLME_BEACON_request(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *tempMsg = new Ieee802154eNetworkCtrlInfo("BeaconRequest", TP_MLME_BEACON_REQUEST);
    tempMsg->setBeaconType(0x01);
    tempMsg->setChannel(11);

    send(tempMsg->dup(), lowerLayerOut);
    delete tempMsg;
    tempMsg = NULL;
}

void RLL::handle_MLME_BEACON_confirm(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *beaconCon = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    double time = pow(2, beaconCon->getBeaconOrder()) * beaconCon->getSuperframeOrder() * timeslotTable->getTemplate(0)->getTimeslotLengthDbl();
    if(BeaconTimer->isScheduled())
	cancelEvent(BeaconTimer);
    scheduleAt(simTime() + time, BeaconTimer);
    delete beaconCon;
    beaconCon = NULL;
    msg = NULL;
}

void RLL::handle_MLME_BEACON_indication(cMessage *msg)
{
    if(dynamic_cast<Ieee802154EnhancedBeaconFrame *>(msg) && bIsPANCoor)
    {
	handle_BEACON_CH_SAME_STAGE(msg);
    }
    else
    {
	MLME_SET_BEACON_request(msg->dup());
	delete msg;
	    msg = NULL;
    }


}

//START
void RLL::MLME_START_request(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *startMsg = new Ieee802154eNetworkCtrlInfo("StartMsg", TP_MLME_START_REQUEST);
    startMsg->setPanCoordinator(bIsPANCoor);
    startMsg->setStartTime((uint32_t)par("StartTime").doubleValue());
    send(startMsg->dup(), lowerLayerOut);
    delete startMsg;
    startMsg = NULL;
}

void RLL::handle_MLME_START_confirm(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *startCo = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    cDisplayString* parentDisp = &getParentModule()->getDisplayString();
    cDisplayString* tempStr = new cDisplayString();
    const char* temp = getParentModule()->getName();
    if(!strcmp(temp, "lightSwitch"))
    {
	tempStr->parse("b=0.1,0.1,rect;i=lighting/lightswitch");
    }
    else if(!strcmp(temp, "gateWay"))
    {
	tempStr->parse("b=0.1,0.1,rect;i=device/accesspoint");
    }
    else
    {
	if(bIsPANCoor)
	    tempStr->parse("b=1.5,1.5,oval,blue;i=status/bulb");
	else
	    tempStr->parse("b=1.5,1.5,oval,yellow;i=status/bulb");
    }
    if(startCo->getPanCoordinator())
    {
	MLME_BEACON_request(NULL);
    }
    else
    {
	MLME_SCAN_request(NULL);
    }
    parentDisp->updateWith(*tempStr);
    delete startCo;
    startCo = NULL;
    msg = NULL;


    delete tempStr;
    tempStr = NULL;
}

//SCAN
void RLL::MLME_SCAN_request(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *scanReq = new Ieee802154eNetworkCtrlInfo("ScanRequest", TP_MLME_SCAN_REQUEST);
    if(!dynamic_cast<Ieee802154EnhancedBeaconFrame *>(msg))
    {
	if(nLastSCANChannel == 0)
	{
	    scanReq->setChannel(channelList[0]);
	    nLastSCANChannel = channelList[0];
	}
	else if(nLastSCANChannel == channelList[numChannel])
	{
	    scanReq->setScanType(0x00);
	    send(scanReq->dup(), lowerLayerOut);
	    if(ScanTimer->isScheduled())
		cancelEvent(ScanTimer);
	    nScanDuration++;
	    if(nScanDuration > 6)
		nScanDuration = 0;

	    if(beaconTable->getNumberBeacons() == 0)
	    {
		scanReq->setChannel(channelList[0]);
		nLastSCANChannel = channelList[0];
	    }
	    else
	    {
		handle_MLME_SCAN_confirm(NULL);
		delete scanReq;
		scanReq = NULL;
		return;
	    }
	}
	else

	{
	    for(int i = 0; i < (numChannel + 1); i++)
	    {
		if(nLastSCANChannel == channelList[i] && nLastSCANChannel != channelList[numChannel])
		{
		    nLastSCANChannel = channelList[i + 1];
		    scanReq->setChannel(channelList[i + 1]);
		    break;
		}
	    }
	}
	scanReq->setScanType(0x02);
	send(scanReq->dup(), lowerLayerOut);

	double tempTime = aBaseSuperframeDuration * pow(2, nScanDuration) / 62.5e3;
	if(ScanTimer->isScheduled())
	    cancelEvent(ScanTimer);
	scheduleAt(simTime() + tempTime, ScanTimer);

    }
    else
    {
	delete msg;
	msg = NULL;

    }
    delete scanReq;
    scanReq = NULL;
}

void RLL::handle_MLME_SCAN_confirm(cMessage *msg)
{
    if(dynamic_cast<Ieee802154EnhancedBeaconFrame *>(msg) && bNotAssociated)
    {

	Ieee802154EnhancedBeaconFrame *tmpBcn = check_and_cast<Ieee802154EnhancedBeaconFrame *>(msg);
	Radio80211aControlInfo control = getRadioControl(check_and_cast<Radio80211aControlInfo *>(tmpBcn->getControlInfo()));
	if(!beaconTable->existBeaconEntry(tmpBcn->getSrcAddr(), tmpBcn->getSrcAddr().getInt(), tmpBcn->getSrcPanId()))
	    beaconTable->addEntry(tmpBcn, tmpBcn->getSrcAddr(), tmpBcn->getSrcAddr().getInt(), control.getRecPow(), control.getSnr(), tmpBcn->getSrcPanId(), simTime(), control.getLossRate(), calcDistance(control.getLossRate(), control.getRecPow()));
	else
	    beaconTable->updateBeaconEntry(tmpBcn, tmpBcn->getSrcAddr(), tmpBcn->getSrcAddr().getInt(), control.getRecPow(), control.getSnr(), tmpBcn->getSrcPanId(), simTime());

	//delete control;
	//control = NULL;


    }
    else
    {
	if(msg == NULL)
	{

	    double rxpower, rssi, txPower, distance;
	    Ieee802154EnhancedBeaconFrame *tmpBcn = beaconTable->returnBestBeaconMsg(&rssi, &rxpower, &txPower, &distance);

	    if(rssi < 20 && bCapablePanCoor)
	    {

		bIsPANCoor = true;
	    }
	    else if(nRestartCounter < 3)
	    {
		nRestartCounter++;
		RESTART_request(NULL);
		return;
	    }

	    if(tmpBcn != NULL)
		MLME_SET_BEACON_request(tmpBcn);

	    beaconTable->flushBeaconTable();
	}
	else
	{
	    delete msg;
	    msg = NULL;
	}
    }
}

//set Beacon
void RLL::MLME_SET_BEACON_request(cMessage *msg)
{
    if(dynamic_cast<Ieee802154EnhancedBeaconFrame *>(msg))
    {

	Ieee802154EnhancedBeaconFrame * tmp = check_and_cast<Ieee802154EnhancedBeaconFrame *>(msg);
	if(bIsPANCoor)
	{
	    nBO = tmp->getBO();
	    nSO = tmp->getSO();
	}
	tmp->setKind(TP_MLME_SET_BEACON_REQUEST);
	tmp->setName("SetSlotRequest");
	send(tmp, lowerLayerOut);
    }
    else
    {

	Ieee802154eNetworkCtrlInfo *tmp = new Ieee802154eNetworkCtrlInfo("SetSlotRequest", TP_MLME_SET_BEACON_REQUEST);

	send(tmp->dup(), lowerLayerOut);
	delete tmp;
	tmp = NULL;
	delete msg;
	msg = NULL;
    }
}

void RLL::handle_MLME_SET_BEACON_confirm(cMessage *msg)
{
    EV << "[SCHEDULER]: Recieved information from coordinator, start Association process" << endl;

    if(bIsPANCoor)
	fBI = pow(2, nBO) * nSO * timeslotTable->getTemplate(0)->getTimeslotLengthDbl();

    if(bIsPANCoor)
    {
	if(bNotAssociated)
	{
	    double waitTime = 0 + ((double)rand() / RAND_MAX) * (200 - 0);
	    if(!AssociateTimer->isScheduled())
		scheduleAt(simTime() + waitTime, AssociateTimer);
	}
    }
    else
    {
	if(bNotAssociated)
	{
	    double waitTime = 0 + ((double)rand() / RAND_MAX) * (10 - 0);
	    if(!bAssociateDirectly)
	    {
		waitTime = 300 + ((double)rand() / RAND_MAX) * (700 - 300); //FIXME: Make it variable or changable by init parameters;
		bAssociateDirectly = true;
	    }
	    if(!AssociateTimer->isScheduled())
		scheduleAt(simTime() + waitTime, AssociateTimer);
	}
    }

    delete msg;
    msg = NULL;
}

//retrieve schedule
void RLL::SCHEDULE_request(cMessage *msg)
{
    Ieee802154eMulHoCluFrame *scheduleFrame = new Ieee802154eMulHoCluFrame("ScheduleRequest", TP_SCHEDULE_REQUEST);
    if(neighborTable->getAddressFromCH() != MACAddress::UNSPECIFIED_ADDRESS)
    {
	scheduleFrame->setDstAddr(neighborTable->getAddressFromCH());
	scheduleFrame->setDstPanId(clusterTable->getEntryByShrtAddr(neighborTable->getNeighborByEAddr(neighborTable->getAddressFromCH())->getShortAddress())->getPanId());
	scheduleFrame->setTimeslot(linkTable->getTimeSlotByOffset(-1));
    }
    else
    {
	delete scheduleFrame;
	scheduleFrame = NULL;
	return;
    }

    send(scheduleFrame->dup(), lowerLayerOut);
    delete scheduleFrame;
    scheduleFrame = NULL;
}
void RLL::handle_SCHEDULE_indication(cMessage *msg)
{
    Ieee802154eMulHoCluFrame *scheduleFrame = check_and_cast<Ieee802154eMulHoCluFrame *>(msg);
    scheduleFrame->setChannelOffset(linkTable->getOffsetByTimeslot(scheduleFrame->getTimeslot()));

    SCHEDULE_response(scheduleFrame->dup());

    delete scheduleFrame;
    scheduleFrame = NULL;
    msg = NULL;
}
void RLL::SCHEDULE_response(cMessage *msg)
{
    Ieee802154eMulHoCluFrame *scheduleFrame = new Ieee802154eMulHoCluFrame("ScheduleResponse", TP_SCHEDULE_RESPONSE);
    Ieee802154eMulHoCluFrame *tempFrame = check_and_cast<Ieee802154eMulHoCluFrame *>(msg);

    scheduleFrame->setDstAddr(tempFrame->getSrcAddr());
    scheduleFrame->setDstPanId(tempFrame->getSrcPanId());
    scheduleFrame->setTimeslot(tempFrame->getTimeslot());
    scheduleFrame->setChannelOffset(tempFrame->getChannelOffset());

    send(scheduleFrame->dup(), lowerLayerOut);
    delete scheduleFrame;
    scheduleFrame = NULL;
    delete tempFrame;
    tempFrame = NULL;
    msg = NULL;
}
void RLL::handle_SCHEDULE_confirm(cMessage *msg)
{
    if(dynamic_cast<Ieee802154eMulHoCluFrame *>(msg))
    {
	Ieee802154eMulHoCluFrame *frame = check_and_cast<Ieee802154eMulHoCluFrame *>(msg);

	macLinkTableEntry *entry = linkTable->getLinkByTimeslot(frame->getTimeslot());
	entry->setChannelOffset(frame->getChannelOffset());

	if(linkTable->getTimeSlotByOffset(-1) != -1)
	{
	    if(ScheduleTimer->isScheduled())
		cancelEvent(ScheduleTimer);
	    scheduleAt(simTime(), ScheduleTimer);
	}
	delete frame;
	frame = NULL;
	msg = NULL;
    }
    else
    {
	delete msg;
	msg = NULL;
    }

}

//PAN COORD check for beacon from CH same stage
void RLL::handle_BEACON_WAIT_timer(cMessage *msg)
{
    if(nScanCounter < 4)
    {
	nScanCounter++;
	int waitConstant = intuniform(2, 5);
	if(!BeaconScanTimer->isScheduled())
	    scheduleAt(simTime() + (fBI * waitConstant), BeaconScanTimer);
    }
    else
    {
	if(beaconTable->CHinDistance(15))
	{
	    MLME_DISASSOCIATE_request(NULL);
	}
	else
	    MLME_BEACON_request(NULL);
    }
}
void RLL::handle_BEACON_CH_SAME_STAGE(cMessage *msg)
{
    Ieee802154EnhancedBeaconFrame *tmpBcn = check_and_cast<Ieee802154EnhancedBeaconFrame *>(msg);
    Radio80211aControlInfo control = getRadioControl(check_and_cast<Radio80211aControlInfo *>(tmpBcn->getControlInfo()));
    if(!beaconTable->existBeaconEntry(tmpBcn->getSrcAddr(), tmpBcn->getSrcAddr().getInt(), tmpBcn->getSrcPanId()))
	beaconTable->addEntry(tmpBcn, tmpBcn->getSrcAddr(), tmpBcn->getSrcAddr().getInt(), control.getRecPow(), control.getSnr(), tmpBcn->getSrcPanId(), simTime(), control.getLossRate(), calcDistance(control.getLossRate(), control.getRecPow()));
    else
	beaconTable->updateBeaconEntry(tmpBcn, tmpBcn->getSrcAddr(), tmpBcn->getSrcAddr().getInt(), control.getRecPow(), control.getSnr(), tmpBcn->getSrcPanId(), simTime());


    delete tmpBcn;
    tmpBcn = NULL;
    msg = NULL;
}

void RLL::RESTART_request(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *cnt = new Ieee802154eNetworkCtrlInfo("Restart", TP_RESTART_REQUEST);

    send(cnt->dup(), lowerLayerOut);
    delete cnt;
    cnt = NULL;
    delete msg;
    msg = NULL;
}

void RLL::handle_RESTART_confirm(cMessage *msg)
{
    linkTable->clearTable();
    neighborTable->clearTable();
    hoppingSequenceList->clearTable();
    timeslotTable->clearTable();
    slotframeTable->clearTable();
    clusterTable->clearTable();
    beaconTable->flushBeaconTable();
    nLastSCANChannel = 0;
    nScanCounter = 3;

    bCapablePanCoor = false;
    if(bCapablePanCoor)
	bIsPANCoor = par("isPANCoor").boolValue();
    else
	bIsPANCoor = false;

    bAssociateDirectly = false;
    if(AssociateTimer->isScheduled())
	cancelAndDelete(AssociateTimer);
    else
	delete AssociateTimer;

    if(ScheduleTimer->isScheduled())
	cancelAndDelete(ScheduleTimer);
    else
	delete ScheduleTimer;

    if(BeaconTimer->isScheduled())
	cancelAndDelete(BeaconTimer);
    else
	delete BeaconTimer;

    if(StartTimer->isScheduled())
	cancelAndDelete(StartTimer);
    else
	delete StartTimer;

    if(AssociateWaitTimer->isScheduled())
	cancelAndDelete(AssociateWaitTimer);
    else
	delete AssociateWaitTimer;

    if(ScanTimer->isScheduled())
	cancelAndDelete(ScanTimer);
    else
	delete ScanTimer;

    if(BeaconScanTimer->isScheduled())
	cancelAndDelete(BeaconScanTimer);
    else
	delete BeaconScanTimer;

    if(DisassociateWaitTimer->isScheduled())
	cancelAndDelete(DisassociateWaitTimer);
    else
	delete DisassociateWaitTimer;
    AssociateTimer = new cMessage("AssociationTimer", ASSOCIATION_TIMER);
    ScheduleTimer = new cMessage("ScheduleTimer", SCHEDULE_TIMER);
    BeaconTimer = new cMessage("BeaconTimer", BEACON_REQUEST);
    StartTimer = new cMessage("StartTimer", START_TIMER);
    AssociateWaitTimer = new cMessage("AssociationWaitTimer", ASSOCIATION_WAIT_TIMER);
    ScanTimer = new cMessage("ScanTimer", MAC_SCAN_TIMER);
    BeaconScanTimer = new cMessage("BeaconScanTimer", SCHEDULE_BEACON_SCAN_TIMER);
    DisassociateWaitTimer = new cMessage("DisassociateWaitTimer", DISASSOCIATION_WAIT_TIMER);

//    fSensitivity = par("Sensitivity").doubleValue();
//    fPCH = par("pCh").doubleValue();
//    fTransmitterPower = par("transmitterPower").doubleValue();
    bNotAssociated = true;
    if(bIsPANCoor)
    {
	nCluStage = 0;
	createInitialEntries();

	nScanDuration = -1;
    }
    else
    {
	nCluStage = -1;
	nScanDuration = 0;

    }

    //double start = 0.0;

    scheduleAt(simTime(), StartTimer);
    delete msg;
    msg = NULL;
}

///////////////////////////////////////////////////////////////
///
///		Helper Functions
//
////////////////////////////////////////////////////////////////

void RLL::createInitialEntries()
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
    slotframeEntry->setSlotframeSize(12);
    slotframeEntry->setSlotframeTable(slotframeTable);
    slotframeTable->addSlotframe(slotframeEntry);


    //Add links ino link table
    setSchedule();

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

void RLL::setSchedule()
{
    if(bIsPANCoor)
    {
	if(nCluStage % 2 == 1)
	{
	    setScheduleChStUn();
	}
	else if(nCluStage % 2 == 0 && nCluStage != 0)
	{
	    setScheduleChStZe();
	}
	else
	{
	    setScheduleChInit();
	}
    }
    else
    {
	setScheduleCs();
    }
}

void RLL::setScheduleChStUn()
{
    //first step: Update initial entry received by beacon from previous stage
    macLinkTableEntry *linkEntry = linkTable->getLink(0);
    linkEntry->isprevStage(true);
    linkEntry->setLinkType(LNK_TP_JOIN);
    linkTable->editLink(linkEntry);

    //new links
    linkEntry = new macLinkTableEntry();
    linkEntry->setChannelOffset(nCluStage % (numChannel + 1)); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(1);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    linkEntry->setChannelOffset(nCluStage % (numChannel + 1)); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(2);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    linkEntry->setChannelOffset((nCluStage - 1) % (numChannel + 1)); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(3);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    linkEntry->setChannelOffset((nCluStage - 1) % (numChannel + 1)); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(4);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    linkEntry->setChannelOffset(nCluStage % (numChannel + 1)); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(5);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    linkEntry->setChannelOffset(nCluStage % (numChannel + 1)); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(6);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    linkEntry->setChannelOffset((nCluStage - 1) % (numChannel + 1)); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(7);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    linkEntry->setChannelOffset((nCluStage - 1) % (numChannel + 1)); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(8);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    linkEntry->setChannelOffset((nCluStage) % (numChannel + 1)); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_SHARED_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_ADVERTISING);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(9);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkEntry->issameStage(true);
    linkEntry->isnextStage(true);
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    int offset = nCluStage % (numChannel + 1) + intuniform(0, 16);
    if(offset > 16)
	offset = offset - 16;

    linkEntry->setChannelOffset(offset); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(10);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    offset = nCluStage % (numChannel + 1) + intuniform(0, 16);
    if(offset > 16)
	offset = offset - 16;

    linkEntry->setChannelOffset(offset); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(11);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

}

void RLL::setScheduleChStZe()
{
    macLinkTableEntry *linkEntry = linkTable->getLink(0);
    linkEntry->isprevStage(true);
    linkEntry->setLinkType(LNK_TP_JOIN);
    linkTable->editLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    linkEntry->setChannelOffset(nCluStage % (numChannel + 1)); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_SHARED_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_ADVERTISING);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(0);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkEntry->isnextStage(true);
    linkEntry->issameStage(true);
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    linkEntry->isprevStage(true);
    linkEntry->setChannelOffset((nCluStage - 1) % (numChannel + 1)); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(1);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    linkEntry->isprevStage(true);
    linkEntry->setChannelOffset((nCluStage - 1) % (numChannel + 1)); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(2);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    linkEntry->isprevStage(false);
    linkEntry->setChannelOffset((nCluStage) % (numChannel + 1)); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(3);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    linkEntry->isprevStage(false);
    linkEntry->setChannelOffset((nCluStage) % (numChannel + 1)); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(4);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    linkEntry->isprevStage(true);
    linkEntry->setChannelOffset((nCluStage - 1) % (numChannel + 1)); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(5);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    linkEntry->isprevStage(true);
    linkEntry->setChannelOffset((nCluStage - 1) % (numChannel + 1)); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(6);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    linkEntry->isprevStage(false);
    linkEntry->setChannelOffset((nCluStage) % (numChannel + 1)); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(7);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    linkEntry->isprevStage(false);
    linkEntry->setChannelOffset((nCluStage) % (numChannel + 1)); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(8);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    int offset = nCluStage % (numChannel + 1) + intuniform(0, 16);
    if(offset > 16)
	offset = offset - 16;

    linkEntry->setChannelOffset(offset); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(10);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    offset = nCluStage % (numChannel + 1) + intuniform(0, 16);
    if(offset > 16)
	offset = offset - 16;

    linkEntry->setChannelOffset(offset); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(11);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);
}

void RLL::setScheduleChInit()
{
    macLinkTableEntry *linkEntry = new macLinkTableEntry();
    linkEntry->setChannelOffset(nCluStage % numChannel); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_SHARED_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_ADVERTISING);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(0);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkEntry->isnextStage(true);
    linkEntry->issameStage(true);
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    linkEntry->setChannelOffset(nCluStage % (numChannel + 1)); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(3);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    linkEntry->setChannelOffset(nCluStage % (numChannel + 1)); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(4);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    linkEntry->setChannelOffset(nCluStage % (numChannel + 1)); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(7);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    linkEntry->setChannelOffset(nCluStage % (numChannel + 1)); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(8);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    int tempOffset = nCluStage % (numChannel + 1) + intuniform(0, 16);
    if(tempOffset > (numChannel + 1))
	tempOffset = tempOffset - (numChannel + 1);

    linkEntry = new macLinkTableEntry();
    linkEntry->setChannelOffset(tempOffset); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(10);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    tempOffset = nCluStage % (numChannel + 1) + intuniform(0, 16);
    if(tempOffset > (numChannel + 1))
	tempOffset = tempOffset - (numChannel + 1);

    linkEntry = new macLinkTableEntry();
    linkEntry->setChannelOffset(tempOffset); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(11);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);
}

void RLL::setScheduleCs()
{
    macLinkTableEntry *linkEntry = new macLinkTableEntry();
    linkEntry->setChannelOffset(-1); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(10);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);

    linkEntry = new macLinkTableEntry();
    linkEntry->setChannelOffset(-1); //Advertisment always with channelOffset 0;
    linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
    linkEntry->setLinkType(LNK_TP_NORMAL);
    linkEntry->setMacLinkTable(linkTable);
    linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
    linkEntry->setSlotframeId(0);
    linkEntry->setTimeslot(11);
    linkEntry->setLinkId(linkTable->getNumLinks());
    linkTable->addLink(linkEntry);
}

double RLL::calcDistance(double transPowmW, double minRecvPowermW)
{
    double lambda = SPEED_OF_LIGHT / 2.4e9;
    double temp1 = transPowmW * pow(lambda, 2);
    double temp2 = (16 * pow(3.14, 2) * minRecvPowermW);
    double temp3 = temp1 / temp2;
    double temp4 = 1.0 / 3.0;
    double temp5 = pow(temp3, temp4);
    return temp5;
}

void RLL::updatedisplay()
{
    cDisplayString* parentDisp = &getParentModule()->getDisplayString();

    char buf[40];

    sprintf(buf, "Stage: %d", nCluStage);
    parentDisp->setTagArg("t", 0, buf);
}

Radio80211aControlInfo RLL::getRadioControl(Radio80211aControlInfo *cntrl)
{
    Radio80211aControlInfo temp;

    temp.setAirtimeMetric(cntrl->getAirtimeMetric());
    temp.setLossRate(cntrl->getLossRate());
    temp.setModulationType(cntrl->getModulationType());
    temp.setRecPow(cntrl->getRecPow());
    temp.setSnr(cntrl->getSnr());
    temp.setTestFrameDuration(cntrl->getTestFrameDuration());
    temp.setTestFrameError(cntrl->getTestFrameError());
    temp.setTestFrameSize(cntrl->getTestFrameSize());

    return temp;
}

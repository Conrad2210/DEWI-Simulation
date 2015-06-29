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
#include "Ieee802Ctrl_m.h"
#include "Ieee802154eFrame_m.h"
#include <Radio80211aControlInfo_m.h>
#include "RLLAppMsg_m.h"

Define_Module(RLL);

static const int PhyConfig = 134215680;
static const int ExtendedBitmap = 0;
static const int minChannelNum = 11;
static const int maxChannelNum = 27;
static const int channelList[16] =
{ 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 };
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
	nDistance = 0;
	ScheduleWaitTimer = NULL;
	generalCheckTimer = NULL;
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
	cancelAndDelete(ScheduleWaitTimer);
	cancelAndDelete(generalCheckTimer);
}

void RLL::initialize(int stage)
{
	if (stage == 0)
	{

		bCapablePanCoor = par("capablePanCoor").boolValue();
		if (bCapablePanCoor)
			bIsPANCoor = par("isPANCoor").boolValue();
		else
			bIsPANCoor = false;
	}
	else if (stage == 2)
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
		mLowerLayerIn = findGate("lowerLayerIn");
		mLowerLayerOut = findGate("lowerLayerOut");
		mUpperLayerIn = findGate("upperLayerIn");
		mUpperLayerOut = findGate("upperLayerOut");

	}
	else if (stage == 3)
	{
		//timer init
		AssociateTimer = new cMessage("AssociationTimer", ASSOCIATION_TIMER);
		AssociateWaitTimer = new cMessage("AssociationWaitTimer", ASSOCIATION_WAIT_TIMER);
		ScheduleTimer = new cMessage("ScheduleTimer", SCHEDULE_TIMER);
		ScheduleWaitTimer = new cMessage("ScheduleWaitTimer", SCHEDULE_WAIT_TIMER);
		BeaconScanTimer = new cMessage("BeaconScanTimer", SCHEDULE_BEACON_SCAN_TIMER);
		DisassociateWaitTimer = new cMessage("DisassociateWaitTimer", DISASSOCIATION_WAIT_TIMER);
		BeaconTimer = new cMessage("BeaconTimer", BEACON_REQUEST);
		ScanTimer = new cMessage("ScanTimer", MAC_SCAN_TIMER);
		StartTimer = new cMessage("StartTimer", START_TIMER);
		generalCheckTimer = new cMessage("CheckTimer", CHECK_TIMER);
	}
	else if (stage == 4)
	{
		if (bIsPANCoor)
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
		nAssociateCounter = 0;

		nDistance = par("Distance");

		WATCH(nLastSCANChannel);
		WATCH(nScanDuration);

		double start = 0.0;
		if (!bIsPANCoor)
		{

			start = par("StartTime").doubleValue();

		}
		else
		{
			start = 0;
		}
		scheduleAt(simTime() + start, StartTimer);
		scheduleAt(simTime() + 100, generalCheckTimer);

		dataCenter = check_and_cast<DataCenter *>(dataCenter->getModuleByPath("DataCenter"));
	}
}
void RLL::finish()
{

}

void RLL::handleMessage(cMessage *msg)
{
	if (msg->isSelfMessage())
	{
		handleSelfMessage(msg);
	}
	else if (msg->getArrivalGateId() == mLowerLayerIn)
	{
		if (!handleLowerMessage(msg))
		{
			if (ev.isGUI())
				EV << "[RLL]: Received packet is DATA packet, process in handleData..." << endl;

			handleDataMessage(PK(msg));
		}
	}
	else if (msg->getArrivalGateId() == mUpperLayerIn)
	{
		handleUpperMessage(PK(msg));
	}
	else
	{
		if (ev.isGUI())
		{
			EV << "Undefined Message type; Drop Message" << endl;

		}
		delete msg;
	}
	updatedisplay();
}

void RLL::handleDataMessage(cPacket *msg)
{
	if (dynamic_cast<RLLAppMsg *>(msg) != NULL)
	{
		RLLAppMsg *temp = check_and_cast<RLLAppMsg *>(msg);
		//First send it to application layer
		send(temp->dup(), mUpperLayerOut);

		//First check if stage 0
//	if(nCluStage == 0 && bIsPANCoor)
//	{
//	    if(temp->getControlInfo() != NULL)
//		temp->removeControlInfo();
//
//	    Ieee802154eNetworkCtrlInfo *ctrl = new Ieee802154eNetworkCtrlInfo();
//
//	    ctrl->setDstAddr(MACAddress::BROADCAST_ADDRESS.getInt());
//
//	    temp->setControlInfo(ctrl);
//	    send(temp, mLowerLayerOut);
//	}
//	else
		if (bIsPANCoor)
		{
			Ieee802154eNetworkCtrlInfo *ctrl;
			if (temp->getControlInfo() != NULL)
				ctrl = check_and_cast<Ieee802154eNetworkCtrlInfo*>(temp->removeControlInfo());
			else
				ctrl = new Ieee802154eNetworkCtrlInfo();

			if (clusterTable->getEntryByShrtAddr(ctrl->getSrcAddr()) != NULL)
			{
				if (clusterTable->getEntryByShrtAddr(ctrl->getSrcAddr())->getStage() > nCluStage)
				{
					//handle msg if arrived from higher cluster stage

					if (clusterTable->existLowerCH(nCluStage))
					{
						ctrl->setTxLowerCH(true);
						ctrl->setTxHigherCH(false);
						ctrl->setTxCS(false);
						ctrl->setDstAddr(MACAddress::BROADCAST_ADDRESS.getInt());

						temp->setControlInfo(ctrl);
						send(temp, mLowerLayerOut);

					}

					RLLAppMsg *temp1 = temp->dup();
					ctrl = new Ieee802154eNetworkCtrlInfo();
					if (nCluStage == 0)
					{
						if (clusterTable->existHigherCH(nCluStage))
						{
							ctrl->setTxLowerCH(false);
							ctrl->setTxHigherCH(true);
							ctrl->setTxCS(false);
							ctrl->setDstAddr(MACAddress::BROADCAST_ADDRESS.getInt());

							temp->setControlInfo(ctrl);
							send(temp, mLowerLayerOut);

						}
					}

					temp1 = temp->dup();
					ctrl = new Ieee802154eNetworkCtrlInfo();
					if (clusterTable->existCS(nCluStage))
					{
						ctrl->setTxLowerCH(false);
						ctrl->setTxHigherCH(false);
						ctrl->setTxCS(true);
						ctrl->setDstAddr(MACAddress::BROADCAST_ADDRESS.getInt());

						temp1->setControlInfo(ctrl);
						send(temp1, mLowerLayerOut);

					}

				}
				else if (clusterTable->getEntryByShrtAddr(ctrl->getSrcAddr())->getStage() == nCluStage)
				{
					//handle Message received from same stage (probably CS)

					if (clusterTable->existLowerCH(nCluStage))
					{
						ctrl->setTxLowerCH(true);
						ctrl->setTxHigherCH(false);
						ctrl->setTxCS(false);
						ctrl->setDstAddr(MACAddress::BROADCAST_ADDRESS.getInt());

						temp->setControlInfo(ctrl);
						send(temp, mLowerLayerOut);

					}

					RLLAppMsg *temp1 = temp->dup();

					ctrl = new Ieee802154eNetworkCtrlInfo();
					if (clusterTable->existHigherCH(nCluStage))
					{
						ctrl->setTxLowerCH(false);
						ctrl->setTxHigherCH(true);
						ctrl->setTxCS(false);
						ctrl->setDstAddr(MACAddress::BROADCAST_ADDRESS.getInt());

						temp1->setControlInfo(ctrl);
						send(temp1, mLowerLayerOut);

					}

					temp1 = temp->dup();

					ctrl = new Ieee802154eNetworkCtrlInfo();
					if (clusterTable->existCS(nCluStage))
					{
						ctrl->setTxLowerCH(false);
						ctrl->setTxHigherCH(false);
						ctrl->setTxCS(true);
						ctrl->setDstAddr(MACAddress::BROADCAST_ADDRESS.getInt());

						temp1->setControlInfo(ctrl);
						send(temp1, mLowerLayerOut);

					}

				}
				else if (clusterTable->getEntryByShrtAddr(ctrl->getSrcAddr())->getStage() < nCluStage)
				{
					//handle Message received from lower stage

					if (clusterTable->existHigherCH(nCluStage))
					{
						ctrl->setTxLowerCH(false);
						ctrl->setTxHigherCH(true);
						ctrl->setTxCS(false);
						ctrl->setDstAddr(MACAddress::BROADCAST_ADDRESS.getInt());

						temp->setControlInfo(ctrl);
						send(temp, mLowerLayerOut);

					}

					RLLAppMsg *temp1 = temp->dup();

					ctrl = new Ieee802154eNetworkCtrlInfo();
					if (clusterTable->existCS(nCluStage))
					{
						ctrl->setTxLowerCH(false);
						ctrl->setTxHigherCH(false);
						ctrl->setTxCS(true);
						ctrl->setDstAddr(MACAddress::BROADCAST_ADDRESS.getInt());

						temp1->setControlInfo(ctrl);
						send(temp1, mLowerLayerOut);

					}
				}

			}
			else
			{
				delete msg;
				msg = NULL;
				if (ev.isGUI())
					EV << "[RLL]: Received packet is not for me, delete packet and resume..." << endl;

			}
		}
		else
		{
			delete msg;
			msg = NULL;
			if (ev.isGUI())
				EV << "[RLL]: I'm a CS nothing to do here..." << endl;

		}

	}
	else
	{
		delete msg;
		msg = NULL;

		if (ev.isGUI())
			EV << "[RLL]: Received packet is not a data packet, delete packet and resume..." << endl;

	}
}

void RLL::handleUpperMessage(cPacket *msg)
{
	RLLAppMsg *appPkt = check_and_cast<RLLAppMsg *>(msg);
	if (bIsPANCoor)
	{

	}
	else
	{
		Ieee802154eNetworkCtrlInfo *cntr = new Ieee802154eNetworkCtrlInfo();
		if (!strcmp(appPkt->getDestName(), "Broadcast"))
		{
			cntr->setDstAddr(MACAddress::BROADCAST_ADDRESS.getInt());
			cntr->setTxCS(true);
		}

		appPkt->setControlInfo(cntr);

		send(appPkt, mLowerLayerOut);
	}
}

bool RLL::handleLowerMessage(cMessage *msg)
{
	switch (msg->getKind())
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
		return false;
		break;
	}
	}
	return true;
}

void RLL::handleSelfMessage(cMessage *msg)
{
	switch (msg->getKind())
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
	case SCHEDULE_WAIT_TIMER:
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
	case CHECK_TIMER:
	{
		//GENERAL_CHECK(NULL);
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
	if (bNotAssociated)
	{
		if (nAssociateCounter < 15)
		{
			if (!AssociateTimer->isScheduled())
			{
				nAssociateCounter++;
				Ieee802154eNetworkCtrlInfo *tmp = new Ieee802154eNetworkCtrlInfo("AssociationRequest", TP_MLME_ASSOCIATE_REQUEST);
				tmp->setPanCoordinator(bIsPANCoor);
				send(tmp->dup(), mLowerLayerOut);
				if (AssociateWaitTimer->isScheduled())
					cancelEvent(AssociateWaitTimer);

				double temp = 5 + (rand() / RAND_MAX) * (15 - 5);
				scheduleAt(simTime() + temp, AssociateWaitTimer);
				delete tmp;
				tmp = NULL;
			}
		}
		else
		{
			bAssociateDirectly = true;
			RESTART_request(NULL);
		}
	}

}

void RLL::handle_MLME_ASSOCIATE_indication(cMessage *msg)
{
	Ieee802154eNetworkCtrlInfo *tmp = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
	if (tmp->getPanCoordinator() && bIsPANCoor)
	{
		clusterTable->addEntry(nCluStage + 1, tmp->getAssocShortAddress(), (char*) "", tmp->getPanCoordinator(), tmp->getPanId());
		//tmp->getPanCoordinator(), nCluStage + 1, tmp->getAssocShortAddress(), tmp->getPanId());
		macNeighborTableEntry* tmpEntry = neighborTable->getNeighborBySAddr(tmp->getAssocShortAddress());
		tmpEntry->setStage(nCluStage + 1);
		tmpEntry->isNextStageCH(true);
		tmpEntry->isSameStageCH(false);
		tmpEntry->isPrevStageCH(false);
		tmpEntry->isMyCS(false);
		tmpEntry->isMyCH(false);
	}
	else if (tmp->getPanCoordinator() && !bIsPANCoor)
	{
		clusterTable->addEntry(nCluStage, tmp->getAssocShortAddress(), (char*) "", tmp->getPanCoordinator(), tmp->getPanId());
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
		clusterTable->addEntry(nCluStage, tmp->getAssocShortAddress(), (char*) "", false, tmp->getPanId());
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
	AssRes->setAssocShortAddress(tmp->getAssocShortAddress());
	AssRes->setStatus(mac_FastA_successful);
	AssRes->setChannelOffset(tmp->getChannelOffset());
	AssRes->setStage(nCluStage);
	AssRes->setPanCoordinator(bIsPANCoor);
	AssRes->setNumberCH(clusterTable->getNumberCH());
	AssRes->setChannelOffset10(linkTable->getLinkByTimeslot(10)->getChannelOffset());
	AssRes->setChannelOffset11(linkTable->getLinkByTimeslot(11)->getChannelOffset());
	send(AssRes->dup(), mLowerLayerOut);
	delete AssRes;
	AssRes = NULL;
	delete tmp;
	tmp = NULL;
}

void RLL::handle_MLME_ASSOCIATE_confirm(cMessage *msg)
{
	Ieee802154eNetworkCtrlInfo *tmp = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
	if (AssociateWaitTimer->isScheduled())
		cancelEvent(AssociateWaitTimer);
	if (AssociateTimer->isScheduled())
		cancelEvent(AssociateTimer);
	if (tmp->getStatus() == mac_FastA_successful)
	{
		bNotAssociated = false;
		cDisplayString* parentDisp = &getParentModule()->getDisplayString();
		cDisplayString* tempStr = new cDisplayString();

		const char* temp = getParentModule()->getName();
		if (!strcmp(temp, "lightSwitch"))
		{
			tempStr->parse("b=0.1,0.1,rect;i=lighting/lightswitch");
		}
		else
		{
			tempStr->parse("b=1.5,1.5,oval,orange;i=status/bulb");

			if (bIsPANCoor)
				tempStr->parse("b=1.5,1.5,oval,orange;i=device/accesspoint");
		}

		if (tmp->getPanCoordinator() && bIsPANCoor)
		{
			nCluStage = tmp->getStage() + 1;
			clusterTable->addEntry(nCluStage - 1, tmp->getAssocShortAddress(), (char*) "", tmp->getPanCoordinator(), tmp->getPanId());
			macNeighborTableEntry* tmpEntry = neighborTable->getNeighborBySAddr(tmp->getAssocShortAddress());
			tmpEntry->setStage(nCluStage - 1);
			tmpEntry->isNextStageCH(false);
			tmpEntry->isSameStageCH(false);
			tmpEntry->isPrevStageCH(true);
			tmpEntry->isMyCS(false);
			tmpEntry->isMyCH(false);

			setSchedule();
		}
		else if (tmp->getPanCoordinator() && !bIsPANCoor)
		{

			nCluStage = tmp->getStage();

			clusterTable->addEntry(nCluStage, tmp->getAssocShortAddress(), (char*) "", tmp->getPanCoordinator(), tmp->getPanId());
			macNeighborTableEntry* tmpEntry = neighborTable->getNeighborBySAddr(tmp->getAssocShortAddress());
			tmpEntry->setStage(nCluStage);
			tmpEntry->isMyCH(tmp->getPanCoordinator());
			tmpEntry->isNextStageCH(false);
			tmpEntry->isSameStageCH(true);
			tmpEntry->isPrevStageCH(false);
			tmpEntry->isMyCS(false);
			tmpEntry->isMyCH(true);

			setSchedule();
            if(linkTable->getLinkByTimeslot(10) != NULL)
			    linkTable->getLinkByTimeslot(10)->setChannelOffset(tmp->getChannelOffset10());
            if(linkTable->getLinkByTimeslot(11) != NULL)
                linkTable->getLinkByTimeslot(11)->setChannelOffset(tmp->getChannelOffset11());
		}
		else
		{
			clusterTable->addEntry(nCluStage, tmp->getAssocShortAddress(), (char*) "", false, tmp->getPanId());
			macNeighborTableEntry* tmpEntry = neighborTable->getNeighborBySAddr(tmp->getAssocShortAddress());
			tmpEntry->setStage(nCluStage);
			tmpEntry->isNextStageCH(false);
			tmpEntry->isSameStageCH(false);
			tmpEntry->isPrevStageCH(false);
			tmpEntry->isMyCS(true);
			tmpEntry->isMyCH(false);

			setSchedule();
		}
		parentDisp->updateWith(*tempStr);

		if (!bIsPANCoor)
		{
			tempStr->parse("b=1.5,1.5,oval,green");
			parentDisp->updateWith(*tempStr);
			dataCenter->updateAssociatedVector(getParentModule()->getIndex(), getParentModule()->getName(), true, nCluStage, clusterTable->getEntry(0)->getAddress(), "");

		}
		else
		{

			if (BeaconScanTimer->isScheduled())
				cancelEvent(BeaconScanTimer);
			waitConstant = tmp->getNumberCH();
			scheduleAt(simTime() + tmp->getNumberCH() * 5, BeaconScanTimer);
			tempStr->parse("b=1.5,1.5,oval,orange");
			parentDisp->updateWith(*tempStr);
		}
		delete tempStr;
		tempStr = NULL;

	}
	else
	{
		bNotAssociated = true;
		cDisplayString* parentDisp = &getParentModule()->getDisplayString();
		cDisplayString* tempStr = new cDisplayString();
		const char* temp = getParentModule()->getName();
		if (!strcmp(temp, "lightSwitch"))
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
	if (DisassociateWaitTimer->isScheduled())
		cancelEvent(DisassociateWaitTimer);

	cDisplayString* parentDisp = &getParentModule()->getDisplayString();
	cDisplayString* tempStr = new cDisplayString();
	tempStr->parse("b=1.5,1.5,oval,blue");
	parentDisp->updateWith(*tempStr);

	Ieee802154eNetworkCtrlInfo *cnt = new Ieee802154eNetworkCtrlInfo("DisassociationRequest", TP_MLME_DISASSOCIATE_REQUEST);
	double temp = 5 + (rand() / RAND_MAX) * (15 - 5);
	scheduleAt(simTime() + temp, DisassociateWaitTimer);
	dataCenter->updateAssociatedVector(getParentModule()->getIndex(), getParentModule()->getName(), false, -1, -1, "");
	send(cnt->dup(), mLowerLayerOut);
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
	send(tmpMsg->dup(), mLowerLayerOut);
	delete tmpMsg;
	tmpMsg = NULL;
	msg = NULL;

}

void RLL::handle_MLME_DIASSOCIATE_confirm(cMessage *msg)
{
	Ieee802154eDisassociationFrame *tmpMsg = check_and_cast<Ieee802154eDisassociationFrame *>(msg);
	clusterTable->deleteEntry(clusterTable->getEntryByShrtAddr(tmpMsg->getSrcAddr().getInt()));
	if (DisassociateWaitTimer->isScheduled())
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

	send(tempMsg->dup(), mLowerLayerOut);
	delete tempMsg;
	tempMsg = NULL;
}

void RLL::handle_MLME_BEACON_confirm(cMessage *msg)
{
	Ieee802154eNetworkCtrlInfo *beaconCon = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
	double time = pow(2, beaconCon->getBeaconOrder()) * beaconCon->getSuperframeOrder() * timeslotTable->getTemplate(0)->getTimeslotLengthDbl();
	if (BeaconTimer->isScheduled())
		cancelEvent(BeaconTimer);
	scheduleAt(simTime() + time, BeaconTimer);
	delete beaconCon;
	beaconCon = NULL;
	msg = NULL;
}

void RLL::handle_MLME_BEACON_indication(cMessage *msg)
{
	if (dynamic_cast<Ieee802154EnhancedBeaconFrame *>(msg) && bIsPANCoor)
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
	startMsg->setStartTime((uint32_t) par("StartTime").doubleValue());
	send(startMsg->dup(), mLowerLayerOut);
	delete startMsg;
	startMsg = NULL;
}

void RLL::handle_MLME_START_confirm(cMessage *msg)
{
	Ieee802154eNetworkCtrlInfo *startCo = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
	cDisplayString* parentDisp = &getParentModule()->getDisplayString();
	cDisplayString* tempStr = new cDisplayString();
	const char* temp = getParentModule()->getName();
	if (!strcmp(temp, "lightSwitch"))
	{
		tempStr->parse("b=0.1,0.1,rect;i=lighting/lightswitch");
	}
	else if (!strcmp(temp, "gateWay"))
	{
		tempStr->parse("b=0.1,0.1,rect;i=device/accesspoint");
	}
	else
	{
		if (bIsPANCoor)
			tempStr->parse("b=1.5,1.5,oval,blue;i=status/bulb");
		else
			tempStr->parse("b=1.5,1.5,oval,yellow;i=status/bulb");
	}
	if (startCo->getPanCoordinator())
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
	if (!dynamic_cast<Ieee802154EnhancedBeaconFrame *>(msg))
	{
		if (nLastSCANChannel == 0)
		{
			scanReq->setChannel(channelList[0]);
			nLastSCANChannel = channelList[0];
		}
		else if (nLastSCANChannel == channelList[numChannel])
		{
			scanReq->setScanType(0x00);
			send(scanReq->dup(), mLowerLayerOut);
			if (ScanTimer->isScheduled())
				cancelEvent(ScanTimer);
			nScanDuration++;
			if (nScanDuration > 6)
				nScanDuration = 0;

			if (beaconTable->getNumberBeacons() == 0)
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
			for (int i = 0; i < (numChannel + 1); i++)
			{
				if (nLastSCANChannel == channelList[i] && nLastSCANChannel != channelList[numChannel])
				{
					nLastSCANChannel = channelList[i + 1];
					scanReq->setChannel(channelList[i + 1]);
					break;
				}
			}
		}
		scanReq->setScanType(0x02);
		send(scanReq->dup(), mLowerLayerOut);

		double tempTime = aBaseSuperframeDuration * pow(2, nScanDuration) / 62.5e3;
		if (ScanTimer->isScheduled())
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
	if (dynamic_cast<Ieee802154EnhancedBeaconFrame *>(msg) && bNotAssociated)
	{

		Ieee802154EnhancedBeaconFrame *tmpBcn = check_and_cast<Ieee802154EnhancedBeaconFrame *>(msg);
		msg = NULL;
		Radio80211aControlInfo control = getRadioControl(check_and_cast<Radio80211aControlInfo *>(tmpBcn->getControlInfo()));
		if (!beaconTable->existBeaconEntry(tmpBcn->getSrcAddr(), tmpBcn->getSrcAddr().getInt(), tmpBcn->getSrcPanId()))
			beaconTable->addEntry(tmpBcn, tmpBcn->getSrcAddr(), tmpBcn->getSrcAddr().getInt(), control.getRecPow(), control.getSnr(), tmpBcn->getSrcPanId(), simTime(), control.getLossRate(), calcDistance(control.getLossRate(), control.getRecPow()));
		else
			beaconTable->updateBeaconEntry(tmpBcn, tmpBcn->getSrcAddr(), tmpBcn->getSrcAddr().getInt(), control.getRecPow(), control.getSnr(), tmpBcn->getSrcPanId(), simTime());

		//delete control;
		//control = NULL;

	}
	else
	{
		if (msg == NULL)
		{

			double rxpower, rssi, txPower, distance;
			Ieee802154EnhancedBeaconFrame *tmpBcn = beaconTable->returnBestBeaconMsg(&rssi, &rxpower, &txPower, &distance);

			if (rssi < nDistance && bCapablePanCoor)
			{

				bIsPANCoor = true;
				beaconTable->flushBeaconTable();
			}
			else if (nRestartCounter < 3)
			{
				nRestartCounter++;
				delete tmpBcn;
				tmpBcn = NULL;
				bCapablePanCoor = false;
				RESTART_request(NULL);
				beaconTable->flushBeaconTable();
				return;
			}

			if (tmpBcn != NULL)
				MLME_SET_BEACON_request(tmpBcn);

		}
		else
		{
			delete msg;
			msg = NULL;
			beaconTable->flushBeaconTable();
		}
	}
}

//set Beacon
void RLL::MLME_SET_BEACON_request(cMessage *msg)
{
	if (dynamic_cast<Ieee802154EnhancedBeaconFrame *>(msg))
	{

		Ieee802154EnhancedBeaconFrame * tmp = check_and_cast<Ieee802154EnhancedBeaconFrame *>(msg);
		if (bIsPANCoor)
		{
			nBO = tmp->getBO();
			nSO = tmp->getSO();
		}
		tmp->setKind(TP_MLME_SET_BEACON_REQUEST);
		tmp->setName("SetSlotRequest");
		send(tmp, mLowerLayerOut);
	}
	else
	{

		Ieee802154eNetworkCtrlInfo *tmp = new Ieee802154eNetworkCtrlInfo("SetSlotRequest", TP_MLME_SET_BEACON_REQUEST);

		send(tmp->dup(), mLowerLayerOut);
		delete tmp;
		tmp = NULL;
		delete msg;
		msg = NULL;
	}
}

void RLL::handle_MLME_SET_BEACON_confirm(cMessage *msg)
{
	EV << "[SCHEDULER]: Recieved information from coordinator, start Association process" << endl;

	if (bIsPANCoor)
		fBI = pow(2, nBO) * nSO * timeslotTable->getTemplate(0)->getTimeslotLengthDbl();

	if (bIsPANCoor)
	{
		if (bNotAssociated)
		{
			double waitTime = 0 + ((double) rand() / RAND_MAX) * (10 - 0);
			if (!AssociateTimer->isScheduled())
				scheduleAt(simTime() + waitTime, AssociateTimer);
		}
	}
	else
	{
		if (bNotAssociated)
		{
			double waitTime = 0 + ((double) rand() / RAND_MAX) * (10 - 0);
			if (!bAssociateDirectly)
			{
				waitTime = 50 + ((double) rand() / RAND_MAX) * ((double) dataCenter->getNumRegisteredAssVectors() * 2.0 - (double) dataCenter->getNumRegisteredAssVectors() / 2.0); //FIXME: Make it variable or changable by init parameters;
				bAssociateDirectly = true;
			}
			if (!AssociateTimer->isScheduled())
				scheduleAt(simTime() + waitTime, AssociateTimer);
		}
	}

	delete msg;
	msg = NULL;
}

//retrieve schedule
void RLL::SCHEDULE_request(cMessage *msg)
{

	if (!strcmp("lightSwitch", getParentModule()->getName()))
	{
		int i = 0;
		i = i + 1;
	}

	Ieee802154eMulHoCluFrame *scheduleFrame = new Ieee802154eMulHoCluFrame("ScheduleRequest", TP_SCHEDULE_REQUEST);
	if (neighborTable->getAddressFromCH() != MACAddress::UNSPECIFIED_ADDRESS)
	{
		scheduleFrame->setDstAddr(neighborTable->getAddressFromCH());
		scheduleFrame->setDstPanId(clusterTable->getEntryByShrtAddr(neighborTable->getNeighborByEAddr(neighborTable->getAddressFromCH())->getShortAddress())->getPanId());
		scheduleFrame->setTimeslot(linkTable->getTimeSlotByOffset(-1));
		scheduleFrame->setSrcName(getParentModule()->getName());
		scheduleFrame->setSrcIndex(getParentModule()->getIndex());
		scheduleFrame->setDestIndex(-1);
		scheduleFrame->setDestName("");
	}
	else
	{
		delete scheduleFrame;
		scheduleFrame = NULL;
		return;
	}

	send(scheduleFrame->dup(), mLowerLayerOut);
	delete scheduleFrame;

	if (ScheduleWaitTimer->isScheduled())
		cancelEvent(ScheduleWaitTimer);

	scheduleAt(simTime() + 5, ScheduleWaitTimer);
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
	msg = NULL;

	scheduleFrame->setDstAddr(tempFrame->getSrcAddr());
	scheduleFrame->setDstPanId(tempFrame->getSrcPanId());
	scheduleFrame->setTimeslot(tempFrame->getTimeslot());
	scheduleFrame->setChannelOffset(tempFrame->getChannelOffset());
	scheduleFrame->setDestName(tempFrame->getSrcName());
	scheduleFrame->setDestIndex(tempFrame->getSrcIndex());
	scheduleFrame->setSrcName(getParentModule()->getName());
	scheduleFrame->setSrcIndex(getParentModule()->getIndex());

	send(scheduleFrame->dup(), mLowerLayerOut);
	delete scheduleFrame;
	delete tempFrame;
	scheduleFrame = NULL;
	tempFrame = NULL;
}
void RLL::handle_SCHEDULE_confirm(cMessage *msg)
{
	if (dynamic_cast<Ieee802154eMulHoCluFrame *>(msg))
	{
		if (ScheduleWaitTimer->isScheduled())
			cancelEvent(ScheduleWaitTimer);
		Ieee802154eMulHoCluFrame *frame = check_and_cast<Ieee802154eMulHoCluFrame *>(msg);

		if (!strcmp("lightSwitch", getParentModule()->getName()))
		{
			int i = 0;
			i = i + 1;
		}

		if (frame->getChannelOffset() == 0 || frame->getChannelOffset() == -1)
		{
			delete frame;
			return;
		}
		macLinkTableEntry *entry = linkTable->getLinkByTimeslot(frame->getTimeslot());
		entry->setChannelOffset(frame->getChannelOffset());

		if (linkTable->getTimeSlotByOffset(-1) != -1)
		{
			if (ScheduleTimer->isScheduled())
				cancelEvent(ScheduleTimer);
			double temp = 5 + (rand() / RAND_MAX) * (15 - 5);
			scheduleAt(simTime() + temp, ScheduleTimer);
		}
		else
		{
			cDisplayString* parentDisp = &getParentModule()->getDisplayString();
			cDisplayString* tempStr = new cDisplayString();
			tempStr->parse("b=1.5,1.5,oval,green;");
			parentDisp->updateWith(*tempStr);
			delete tempStr;

			if (!bIsPANCoor)
				dataCenter->updateAssociatedVector(getParentModule()->getIndex(), getParentModule()->getName(), true, nCluStage, clusterTable->getEntry(0)->getAddress(), "");

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
	cDisplayString* parentDisp = &getParentModule()->getDisplayString();
	cDisplayString* tempStr = new cDisplayString();
	tempStr->parse("b=1.5,1.5,oval,orange");
	parentDisp->updateWith(*tempStr);
	if (nScanCounter < waitConstant)
	{

		nScanCounter++;
		if (!BeaconScanTimer->isScheduled())
			scheduleAt(simTime() + 5, BeaconScanTimer);
	}
	else
	{
		if (beaconTable->CHinDistance(nDistance))
		{
			bCapablePanCoor = false;
			MLME_DISASSOCIATE_request(NULL);
		}
		else
		{
			tempStr->parse("b=1.5,1.5,oval,green");
			parentDisp->updateWith(*tempStr);
			MLME_BEACON_request(NULL);
			if (bIsPANCoor)
				dataCenter->updateAssociatedVector(getParentModule()->getIndex(), getParentModule()->getName(), true, nCluStage, clusterTable->getEntry(0)->getAddress(), "");

		}
	}
}
void RLL::handle_BEACON_CH_SAME_STAGE(cMessage *msg)
{
	Ieee802154EnhancedBeaconFrame *tmpBcn = check_and_cast<Ieee802154EnhancedBeaconFrame *>(msg);
	Radio80211aControlInfo control = getRadioControl(check_and_cast<Radio80211aControlInfo *>(tmpBcn->getControlInfo()));
	if (!beaconTable->existBeaconEntry(tmpBcn->getSrcAddr(), tmpBcn->getSrcAddr().getInt(), tmpBcn->getSrcPanId()))
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

	send(cnt->dup(), mLowerLayerOut);
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
	//bCapablePanCoor = false;
	if (bCapablePanCoor)
		bIsPANCoor = par("isPANCoor").boolValue();
	else
		bIsPANCoor = false;

	//bAssociateDirectly = false;

	cancelAndDelete(AssociateTimer);
	cancelAndDelete(ScheduleTimer);
	cancelAndDelete(BeaconTimer);
	cancelAndDelete(StartTimer);
	cancelAndDelete(AssociateWaitTimer);
	cancelAndDelete(ScanTimer);
	cancelAndDelete(BeaconScanTimer);
	cancelAndDelete(DisassociateWaitTimer);
	cancelAndDelete(ScheduleWaitTimer);

	AssociateTimer = new cMessage("AssociationTimer", ASSOCIATION_TIMER);
	ScheduleTimer = new cMessage("ScheduleTimer", SCHEDULE_TIMER);
	ScheduleWaitTimer = new cMessage("ScheduleWaitTimer", SCHEDULE_WAIT_TIMER);
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
	if (bIsPANCoor)
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
	nAssociateCounter = 0;

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
	for (int i = minChannelNum; i < maxChannelNum; i++)
		tempList.push_back(i);
	hoppingEntry->setHoppingSequenceLength(tempList.size());
	hoppingEntry->setHoppingSequenceList(tempList);
	hoppingEntry->setNumberOfChannels(tempList.size());
	hoppingEntry->setPhyConfiguration(PhyConfig);
	hoppingSequenceList->addEntry(hoppingEntry);
}

void RLL::setSchedule()
{
	if (bIsPANCoor)
	{
		if (nCluStage % 2 == 1)
		{
			setScheduleChStUn();
		}
		else if (nCluStage % 2 == 0 && nCluStage != 0)
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
	linkEntry->issameStage(false);
	linkEntry->isnextStage(false);
//    if(!linkTable->editLink(linkEntry))
//    {
//	EV << "Coudlnt add link table entry" << endl;
//    }

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
	linkEntry->issameStage(false);
	linkEntry->isnextStage(true);
	linkEntry->isprevStage(false);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

	linkEntry = new macLinkTableEntry();
	linkEntry->setChannelOffset(nCluStage % (numChannel + 1)); //Advertisment always with channelOffset 0;
	linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
	linkEntry->setLinkType(LNK_TP_NORMAL);
	linkEntry->setMacLinkTable(linkTable);
	linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
	linkEntry->setSlotframeId(0);
	linkEntry->setTimeslot(2);
	linkEntry->setLinkId(linkTable->getNumLinks());

	linkEntry->issameStage(false);
	linkEntry->isnextStage(true);
	linkEntry->isprevStage(false);

	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

	linkEntry = new macLinkTableEntry();
	linkEntry->setChannelOffset((nCluStage - 1) % (numChannel + 1)); //Advertisment always with channelOffset 0;
	linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
	linkEntry->setLinkType(LNK_TP_NORMAL);
	linkEntry->setMacLinkTable(linkTable);
	linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
	linkEntry->setSlotframeId(0);
	linkEntry->setTimeslot(3);
	linkEntry->setLinkId(linkTable->getNumLinks());
	linkEntry->issameStage(false);
	linkEntry->isnextStage(false);
	linkEntry->isprevStage(true);

	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

	linkEntry = new macLinkTableEntry();
	linkEntry->setChannelOffset((nCluStage - 1) % (numChannel + 1)); //Advertisment always with channelOffset 0;
	linkEntry->setLinkOption(LNK_OP_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
	linkEntry->setLinkType(LNK_TP_NORMAL);
	linkEntry->setMacLinkTable(linkTable);
	linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
	linkEntry->setSlotframeId(0);
	linkEntry->setTimeslot(4);
	linkEntry->setLinkId(linkTable->getNumLinks());
	linkEntry->issameStage(false);
	linkEntry->isnextStage(false);
	linkEntry->isprevStage(true);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

	linkEntry = new macLinkTableEntry();
	linkEntry->setChannelOffset(nCluStage % (numChannel + 1)); //Advertisment always with channelOffset 0;
	linkEntry->setLinkOption(LNK_OP_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
	linkEntry->setLinkType(LNK_TP_NORMAL);
	linkEntry->setMacLinkTable(linkTable);
	linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
	linkEntry->setSlotframeId(0);
	linkEntry->setTimeslot(5);
	linkEntry->setLinkId(linkTable->getNumLinks());
	linkEntry->issameStage(false);
	linkEntry->isnextStage(true);
	linkEntry->isprevStage(false);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

	linkEntry = new macLinkTableEntry();
	linkEntry->setChannelOffset(nCluStage % (numChannel + 1)); //Advertisment always with channelOffset 0;
	linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
	linkEntry->setLinkType(LNK_TP_NORMAL);
	linkEntry->setMacLinkTable(linkTable);
	linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
	linkEntry->setSlotframeId(0);
	linkEntry->setTimeslot(6);
	linkEntry->setLinkId(linkTable->getNumLinks());
	linkEntry->issameStage(false);
	linkEntry->isnextStage(true);
	linkEntry->isprevStage(false);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

	linkEntry = new macLinkTableEntry();
	linkEntry->setChannelOffset((nCluStage - 1) % (numChannel + 1)); //Advertisment always with channelOffset 0;
	linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
	linkEntry->setLinkType(LNK_TP_NORMAL);
	linkEntry->setMacLinkTable(linkTable);
	linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
	linkEntry->setSlotframeId(0);
	linkEntry->setTimeslot(7);
	linkEntry->setLinkId(linkTable->getNumLinks());
	linkEntry->issameStage(false);
	linkEntry->isnextStage(false);
	linkEntry->isprevStage(true);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

	linkEntry = new macLinkTableEntry();
	linkEntry->setChannelOffset((nCluStage - 1) % (numChannel + 1)); //Advertisment always with channelOffset 0;
	linkEntry->setLinkOption(LNK_OP_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
	linkEntry->setLinkType(LNK_TP_NORMAL);
	linkEntry->setMacLinkTable(linkTable);
	linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
	linkEntry->setSlotframeId(0);
	linkEntry->setTimeslot(8);
	linkEntry->setLinkId(linkTable->getNumLinks());
	linkEntry->issameStage(false);
	linkEntry->isnextStage(false);
	linkEntry->isprevStage(true);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

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
	linkEntry->isprevStage(false);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

	linkEntry = new macLinkTableEntry();
	int tempOffset = 0;

	while (tempOffset == 0)
	{
		tempOffset = nCluStage % (numChannel + 1) + intuniform(0, 16);

		if (tempOffset > (numChannel + 1))
			tempOffset = tempOffset - (numChannel + 1);
	}

	linkEntry->setChannelOffset(tempOffset); //Advertisment always with channelOffset 0;
	linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
	linkEntry->setLinkType(LNK_TP_NORMAL);
	linkEntry->setMacLinkTable(linkTable);
	linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
	linkEntry->setSlotframeId(0);
	linkEntry->setTimeslot(10);
	linkEntry->setLinkId(linkTable->getNumLinks());
	linkEntry->issameStage(true);
	linkEntry->isnextStage(false);
	linkEntry->isprevStage(false);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

	linkEntry = new macLinkTableEntry();
	tempOffset = 0;

	while (tempOffset == 0)
	{
		tempOffset = nCluStage % (numChannel + 1) + intuniform(0, 16);

		if (tempOffset > (numChannel + 1))
			tempOffset = tempOffset - (numChannel + 1);
	}
	linkEntry->setChannelOffset(tempOffset); //Advertisment always with channelOffset 0;
	linkEntry->setLinkOption(LNK_OP_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
	linkEntry->setLinkType(LNK_TP_NORMAL);
	linkEntry->setMacLinkTable(linkTable);
	linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
	linkEntry->setSlotframeId(0);
	linkEntry->setTimeslot(11);
	linkEntry->setLinkId(linkTable->getNumLinks());
	linkEntry->issameStage(true);
	linkEntry->isnextStage(false);
	linkEntry->isprevStage(false);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

}

void RLL::setScheduleChStZe()
{
	macLinkTableEntry *linkEntry = linkTable->getLink(0);
	linkEntry->isprevStage(true);
	linkEntry->isnextStage(false);
	linkEntry->issameStage(false);
	linkEntry->setLinkType(LNK_TP_JOIN);
//    if(!linkTable->addLink(linkEntry))
//    {
//	EV << "Coudlnt add link table entry" << endl;
//    }

	linkEntry = new macLinkTableEntry();
	linkEntry->setChannelOffset(nCluStage % (numChannel + 1)); //Advertisment always with channelOffset 0;
	linkEntry->setLinkOption(LNK_OP_SHARED_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
	linkEntry->setLinkType(LNK_TP_ADVERTISING);
	linkEntry->setMacLinkTable(linkTable);
	linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
	linkEntry->setSlotframeId(0);
	linkEntry->setTimeslot(0);
	linkEntry->setLinkId(linkTable->getNumLinks());
	linkEntry->issameStage(true);
	linkEntry->isnextStage(true);
	linkEntry->isprevStage(false);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

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
	linkEntry->issameStage(false);
	linkEntry->isnextStage(false);
	linkEntry->isprevStage(true);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

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
	linkEntry->issameStage(false);
	linkEntry->isnextStage(false);
	linkEntry->isprevStage(true);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

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
	linkEntry->issameStage(false);
	linkEntry->isnextStage(true);
	linkEntry->isprevStage(false);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

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
	linkEntry->issameStage(false);
	linkEntry->isnextStage(true);
	linkEntry->isprevStage(false);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

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
	linkEntry->issameStage(false);
	linkEntry->isnextStage(false);
	linkEntry->isprevStage(true);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

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
	linkEntry->issameStage(false);
	linkEntry->isnextStage(false);
	linkEntry->isprevStage(true);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

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
	linkEntry->issameStage(false);
	linkEntry->isnextStage(true);
	linkEntry->isprevStage(false);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

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
	linkEntry->issameStage(false);
	linkEntry->isnextStage(true);
	linkEntry->isprevStage(false);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

	linkEntry = new macLinkTableEntry();
	int tempOffset = 0;

	while (tempOffset == 0)
	{
		tempOffset = nCluStage % (numChannel + 1) + intuniform(0, 16);

		if (tempOffset > (numChannel + 1))
			tempOffset = tempOffset - (numChannel + 1);
	}

	linkEntry->setChannelOffset(tempOffset); //Advertisment always with channelOffset 0;
	linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
	linkEntry->setLinkType(LNK_TP_NORMAL);
	linkEntry->setMacLinkTable(linkTable);
	linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
	linkEntry->setSlotframeId(0);
	linkEntry->setTimeslot(10);
	linkEntry->setLinkId(linkTable->getNumLinks());
	linkEntry->issameStage(true);
	linkEntry->isnextStage(false);
	linkEntry->isprevStage(false);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

	linkEntry = new macLinkTableEntry();
	tempOffset = 0;

	while (tempOffset == 0)
	{
		tempOffset = nCluStage % (numChannel + 1) + intuniform(0, 16);

		if (tempOffset > (numChannel + 1))
			tempOffset = tempOffset - (numChannel + 1);
	}

	linkEntry->setChannelOffset(tempOffset); //Advertisment always with channelOffset 0;
	linkEntry->setLinkOption(LNK_OP_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
	linkEntry->setLinkType(LNK_TP_NORMAL);
	linkEntry->setMacLinkTable(linkTable);
	linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
	linkEntry->setSlotframeId(0);
	linkEntry->setTimeslot(11);
	linkEntry->setLinkId(linkTable->getNumLinks());
	linkEntry->issameStage(true);
	linkEntry->isnextStage(false);
	linkEntry->isprevStage(false);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}
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
	linkEntry->issameStage(true);
	linkEntry->isnextStage(true);
	linkEntry->isprevStage(false);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

	linkEntry = new macLinkTableEntry();
	linkEntry->setChannelOffset(nCluStage % (numChannel + 1)); //Advertisment always with channelOffset 0;
	linkEntry->setLinkOption(LNK_OP_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
	linkEntry->setLinkType(LNK_TP_NORMAL);
	linkEntry->setMacLinkTable(linkTable);
	linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
	linkEntry->setSlotframeId(0);
	linkEntry->setTimeslot(3);
	linkEntry->setLinkId(linkTable->getNumLinks());
	linkEntry->issameStage(false);
	linkEntry->isnextStage(true);
	linkEntry->isprevStage(false);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

	linkEntry = new macLinkTableEntry();
	linkEntry->setChannelOffset(nCluStage % (numChannel + 1)); //Advertisment always with channelOffset 0;
	linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
	linkEntry->setLinkType(LNK_TP_NORMAL);
	linkEntry->setMacLinkTable(linkTable);
	linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
	linkEntry->setSlotframeId(0);
	linkEntry->setTimeslot(4);
	linkEntry->setLinkId(linkTable->getNumLinks());
	linkEntry->issameStage(false);
	linkEntry->isnextStage(true);
	linkEntry->isprevStage(false);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

	linkEntry = new macLinkTableEntry();
	linkEntry->setChannelOffset(nCluStage % (numChannel + 1)); //Advertisment always with channelOffset 0;
	linkEntry->setLinkOption(LNK_OP_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
	linkEntry->setLinkType(LNK_TP_NORMAL);
	linkEntry->setMacLinkTable(linkTable);
	linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
	linkEntry->setSlotframeId(0);
	linkEntry->setTimeslot(7);
	linkEntry->setLinkId(linkTable->getNumLinks());
	linkEntry->issameStage(false);
	linkEntry->isnextStage(true);
	linkEntry->isprevStage(false);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

	linkEntry = new macLinkTableEntry();
	linkEntry->setChannelOffset(nCluStage % (numChannel + 1)); //Advertisment always with channelOffset 0;
	linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
	linkEntry->setLinkType(LNK_TP_NORMAL);
	linkEntry->setMacLinkTable(linkTable);
	linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
	linkEntry->setSlotframeId(0);
	linkEntry->setTimeslot(8);
	linkEntry->setLinkId(linkTable->getNumLinks());
	linkEntry->issameStage(false);
	linkEntry->isnextStage(true);
	linkEntry->isprevStage(false);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

	int tempOffset = 0;

	while (tempOffset == 0)
	{
		tempOffset = nCluStage % (numChannel + 1) + intuniform(0, 16);

		if (tempOffset > (numChannel + 1))
			tempOffset = tempOffset - (numChannel + 1);
	}

	linkEntry = new macLinkTableEntry();
	linkEntry->setChannelOffset(tempOffset); //Advertisment always with channelOffset 0;
	linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
	linkEntry->setLinkType(LNK_TP_NORMAL);
	linkEntry->setMacLinkTable(linkTable);
	linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
	linkEntry->setSlotframeId(0);
	linkEntry->setTimeslot(10);
	linkEntry->setLinkId(linkTable->getNumLinks());
	linkEntry->issameStage(true);
	linkEntry->isnextStage(false);
	linkEntry->isprevStage(false);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

	tempOffset = 0;

	while (tempOffset == 0)
	{
		tempOffset = nCluStage % (numChannel + 1) + intuniform(0, 16);

		if (tempOffset > (numChannel + 1))
			tempOffset = tempOffset - (numChannel + 1);
	}

	linkEntry = new macLinkTableEntry();
	linkEntry->setChannelOffset(tempOffset); //Advertisment always with channelOffset 0;
	linkEntry->setLinkOption(LNK_OP_RECEIVE); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
	linkEntry->setLinkType(LNK_TP_NORMAL);
	linkEntry->setMacLinkTable(linkTable);
	linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
	linkEntry->setSlotframeId(0);
	linkEntry->setTimeslot(11);
	linkEntry->setLinkId(linkTable->getNumLinks());
	linkEntry->issameStage(true);
	linkEntry->isnextStage(false);
	linkEntry->isprevStage(false);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}
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
	linkEntry->issameStage(true);
	linkEntry->isnextStage(false);
	linkEntry->isprevStage(false);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}

	linkEntry = new macLinkTableEntry();
	linkEntry->setChannelOffset(-1); //Advertisment always with channelOffset 0;
	linkEntry->setLinkOption(LNK_OP_TRANSMIT); // always shared receive (Coordinator is able to receive Acc requests and transmit beacons
	linkEntry->setLinkType(LNK_TP_NORMAL);
	linkEntry->setMacLinkTable(linkTable);
	linkEntry->setNodeAddress(0xffff); //Transmit Beacons always to Broadcast Address
	linkEntry->setSlotframeId(0);
	linkEntry->setTimeslot(11);
	linkEntry->setLinkId(linkTable->getNumLinks());
	linkEntry->issameStage(true);
	linkEntry->isnextStage(false);
	linkEntry->isprevStage(false);
	if (!linkTable->addLink(linkEntry))
	{
		EV << "Coudlnt add link table entry" << endl;
	}
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

void RLL::GENERAL_CHECK(cMessage *msg)
{

	if (bNotAssociated)
	{
		if (StartTimer->isScheduled() || ScanTimer->isScheduled() || AssociateTimer->isScheduled() || AssociateWaitTimer->isScheduled())
		{
			setCheckTimer();
			return;
		}
		else
		{
			RESTART_request(NULL);
			return;
		}

	}
	else
	{
		if (bIsPANCoor)
		{
			setCheckTimer();
			return;
		}
		else
		{
			if (ScheduleTimer->isScheduled() || ScheduleWaitTimer->isScheduled())
			{
				setCheckTimer();
				return;
			}

			if (DisassociateWaitTimer->isScheduled())
			{
				setCheckTimer();
				return;
			}
		}

		if (linkTable->getTimeSlotByOffset(-1) == -1)
		{
			setCheckTimer();
			return;
		}
		else
		{
			SCHEDULE_request(NULL);
			setCheckTimer();
			return;
		}

	}

	setCheckTimer();

}

void RLL::setCheckTimer()
{
	if (generalCheckTimer->isScheduled())
		cancelEvent(generalCheckTimer);

	scheduleAt(simTime() + 10, generalCheckTimer);
}

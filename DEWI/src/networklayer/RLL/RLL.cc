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

Define_Module(RLL);
RLL::RLL()
{

    bNotAssociated = false;
    nCluStage = -1;
    AssociateTimer = NULL;
    AssociateWaitTimer = NULL;
}

RLL::~RLL()
{

    delete AssociateTimer;
    delete AssociateWaitTimer;
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
	if(bIsPANCoor)
	{
	    nCluStage = 0;
	}
	else
	{
	    nCluStage = -1;
	}
    }
    else if(stage == 3)
    {
	//timer init
	AssociateTimer = new cMessage("AssociationTimer", ASSOCIATION_TIMER);
	AssociateWaitTimer = new cMessage("AssociationWaitTimer", ASSOCIATION_WAIT_TIMER);
    }
    else if(stage == 4)
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

	lowerLayerIn = findGate("lowerLayerIn");
	lowerLayerOut = findGate("lowerLayerOut");
	upperLayerIn = findGate("upperLayerIn");
	upperLayerOut = findGate("upperLayerOut");
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
	    scheduleAt(simTime() + 5, AssociateWaitTimer);
	    delete tmp;
	}
    }

}

void RLL::handle_MLME_ASSOCIATE_indication(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *tmp = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    if(tmp->getPanCoordinator() && bIsPANCoor)
    {
	clusterTable->addEntry(nCluStage + 1,tmp->getAssocShortAddress(),"",tmp->getPanCoordinator());
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
	clusterTable->addEntry(nCluStage,tmp->getAssocShortAddress(),"",tmp->getPanCoordinator());
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
	clusterTable->addEntry(nCluStage,tmp->getAssocShortAddress(),"",false);
	macNeighborTableEntry* tmpEntry = neighborTable->getNeighborBySAddr(tmp->getAssocShortAddress());
	tmpEntry->setStage(nCluStage);
	tmpEntry->isNextStageCH(false);
	tmpEntry->isSameStageCH(false);
	tmpEntry->isPrevStageCH(false);
	tmpEntry->isMyCS(true);
	tmpEntry->isMyCH(false);
    }
    MLME_ASSOCIATE_response(msg->dup());
    delete msg;
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
    delete tmp;
}

void RLL::handle_MLME_ASSOCIATE_confirm(cMessage *msg)
{
}

//Dissassociation Process
void RLL::MLME_DISASSOCIATE_request(cMessage *msg)
{
}

void RLL::handle_MLME_DIASSOCIATE_indication(cMessage *msg)
{
}
void RLL::MLME_DISASSOCIATE_response(cMessage *msg)
{
}

void RLL::handle_MLME_DIASSOCIATE_confirm(cMessage *msg)
{
}

//Advertisment

void RLL::MLME_BEACON_request(cMessage *msg)
{
}

void RLL::handle_MLME_BEACON_confirm(cMessage *msg)
{
}

void RLL::handle_MLME_BEACON_indication(cMessage *msg)
{
}

void RLL::MLME_SETSLOTFRAME_request(cMessage *msg)
{
}

void RLL::handle_MLME_SETSLOTFRAME_confirm(cMessage *msg)
{
}

void RLL::MLME_SETLINK_request(cMessage *msg)
{
}

void RLL::handle_MLME_SETLINK_confirm(cMessage *msg)
{
}

void RLL::MLME_TSCHMODE_request(cMessage *msg)
{
}

void RLL::handle_MLME_TSCHMODE_confirm(cMessage *msg)
{
}

//START
void RLL::MLME_START_request(cMessage *msg)
{
}

void RLL::handle_MLME_START_confirm(cMessage *msg)
{
}

//SCAN
void RLL::MLME_SCAN_request(cMessage *msg)
{
}

void RLL::handle_MLME_SCAN_confirm(cMessage *msg)
{
}

//set Beacon
void RLL::MLME_SET_BEACON_request(cMessage *msg)
{
}

void RLL::handle_MLME_SET_BEACON_confirm(cMessage *msg)
{
}

//retrieve schedule
void RLL::SCHEDULE_request(cMessage *msg)
{
}
void RLL::handle_SCHEDULE_indication(cMessage *msg)
{
}
void RLL::SCHEDULE_response(cMessage *msg)
{
}
void RLL::handle_SCHEDULE_confirm(cMessage *msg)
{
}

//PAN COORD check for beacon from CH same stage
void RLL::handle_BEACON_WAIT_timer(cMessage *msg)
{
}
void RLL::handle_BEACON_CH_SAME_STAGE(cMessage *msg)
{
}

void RLL::RESTART_request(cMessage *msg)
{
}

void RLL::handle_RESTART_confirm(cMessage *msg)
{
}


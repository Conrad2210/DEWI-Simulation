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

#include <cdisplaystring.h>
#include <cenvir.h>
#include <cmessage.h>
#include <cmodule.h>
#include <cnamedobject.h>
#include <cobjectfactory.h>
#include <cownedobject.h>
#include <cpar.h>
#include <cregistrationlist.h>
#include <csimulation.h>
#include <cwatch.h>
#include <Ieee802154Def.h>
#include <Ieee802154eConst.h>
#include <Ieee802154eEnum.h>
#include <Ieee802154eNetworkCtrlInfo_m.h>
#include <macHoppingSequenceListEntry.h>
#include <macLinkTableEntry.h>
#include <macSlotframeTableEntry.h>
#include <macTimeslotTableEntry.h>
#include <MulHopClu.h>
#include <regmacros.h>
#include <simtime.h>
#include <simutil.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

Define_Module(MulHopClu);

static const int PhyConfig = 134215680;
static const int ExtendedBitmap = 0;
static const int minChannelNum = 11;
static const int maxChannelNum = 27;
static const int channelList[16] =
{
11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26
};
static const int numChannel = 16;

MulHopClu::MulHopClu()
{
    // TODO Auto-generated constructor stub
    lastSCANChannel = 0;
    notAssociated = true;
}

MulHopClu::~MulHopClu()
{
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

}

void MulHopClu::initialize(int stage)
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

    if(isPANCoor)
    {
	createInitialEntries();
	stage = 1;
	scanDuration = -1;
    }
    else
    {
	stage = -1;
	scanDuration = 0;
    }

    const char* temp = getParentModule()->getParentModule()->getName();
    double start = 0.0;
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
    WATCH(lastSCANChannel);

}

void MulHopClu::handleMessage(cMessage *msg)
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

void MulHopClu::handleMACMessage(cMessage *msg)
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
    }
}
void MulHopClu::handleSelfMessage(cMessage *msg)
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

    }
}

//Association Process
void MulHopClu::MLME_ASSOCIATE_request(cMessage *msg)
{

}

void MulHopClu::handle_MLME_ASSOCIATE_indication(cMessage *msg)
{

}

void MulHopClu::MLME_ASSOCIATE_responce(cMessage *msg)
{

}

void MulHopClu::handle_MLME_ASSOCIATE_confirm(cMessage *msg)
{

}

//Dissassociation Process
void MulHopClu::MLME_DISASSOCIATE_request(cMessage *msg)
{
}

void MulHopClu::handle_MLME_DIASSOCIATE_indication(cMessage *msg)
{
}

void MulHopClu::handle_MLME_DIASSOCIATE_confirm(cMessage *msg)
{
}

//Advertisment

void MulHopClu::MLME_BEACON_request(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *tempMsg = new Ieee802154eNetworkCtrlInfo("BeaconRequest", TP_MLME_BEACON_REQUEST);
    tempMsg->setBeaconType(0x01);
    tempMsg->setChannel(11);

    send(tempMsg->dup(), outGate);
    delete tempMsg;
    delete msg;
}

void MulHopClu::handle_MLME_BEACON_confirm(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *beaconCon = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    double time = pow(2, beaconCon->getBeaconOrder()) * beaconCon->getSuperframeOrder() * timeslotTable->getTemplate(0)->getTimeslotLengthDbl();

    scheduleAt(simTime() + time, BeaconTimer);
    delete beaconCon;
}

void MulHopClu::handle_MLME_BEACON_indication(cMessage *msg)
{

}

void MulHopClu::MLME_SETSLOTFRAME_request(cMessage *msg)
{

}

void MulHopClu::handle_MLME_SETSLOTFRAME_confirm(cMessage *msg)
{
}

void MulHopClu::MLME_SETLINK_request(cMessage *msg)
{
}

void MulHopClu::handle_MLME_SETLINK_confirm(cMessage *msg)
{
}

void MulHopClu::MLME_TSCHMODE_request(cMessage *msg)
{
}

void MulHopClu::handle_MLME_TSCHMODE_confirm(cMessage *msg)
{
}

//////START
void MulHopClu::MLME_START_request(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *startMsg = new Ieee802154eNetworkCtrlInfo("StartMsg", TP_MLME_START_REQUEST);
    startMsg->setPanCoordinator(isPANCoor);
    startMsg->setStartTime((uint32_t)par("StartTime").doubleValue());
    send(startMsg->dup(), outGate);
    delete startMsg;
}

void MulHopClu::handle_MLME_START_confirm(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *startCo = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    cDisplayString* parentDisp = &getParentModule()->getParentModule()->getDisplayString();
    cDisplayString* tempStr = new cDisplayString();
    const char* temp = getParentModule()->getParentModule()->getName();
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
    parentDisp->updateWith(*tempStr);
}

//Scan
void MulHopClu::MLME_SCAN_request(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *scanReq = new Ieee802154eNetworkCtrlInfo("ScanRequest", TP_MLME_SCAN_REQUEST);
    if(!dynamic_cast<Ieee802154EnhancedBeaconFrame *>(msg))
    {
	if(lastSCANChannel == 0)
	{
	    scanReq->setChannel(channelList[0]);
	    lastSCANChannel = channelList[0];
	}
	else if(lastSCANChannel == channelList[numChannel - 1])
	{
	    scanReq->setScanType(0x00);
	    send(scanReq->dup(), outGate);
	    if(ScanTimer->isScheduled())
		cancelEvent(ScanTimer);
	    scanDuration++;

	    if(BeaconTable.getNumberBeacons() == 0)
	    {
		scanReq->setChannel(channelList[0]);
		lastSCANChannel = channelList[0];
	    }
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
	    }
	}
	scanReq->setScanType(0x02);
	send(scanReq->dup(), outGate);

	double tempTime = aBaseSuperframeDuration * pow(2, scanDuration) / 62.5e3;
	if(ScanTimer->isScheduled())
	    cancelEvent(ScanTimer);
	scheduleAt(simTime() + tempTime, ScanTimer);

    }
    delete scanReq;
    delete msg;
}

void MulHopClu::handle_MLME_SCAN_confirm(cMessage *msg)
{

}

void MulHopClu::MLME_SET_BEACON_request(cMessage *msg)
{

}

void MulHopClu::handle_MLME_SET_BEACON_confirm(cMessage *msg)
{

}

void MulHopClu::createInitialEntries()
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
    slotframeEntry->setSlotframeSize(10);
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
void MulHopClu::SCHEDULE_request(cMessage *msg)
{

}

void MulHopClu::handle_SCHEDULE_indication(cMessage *msg)
{

}

void MulHopClu::SCHEDULE_response(cMessage *msg)
{

}

void MulHopClu::handle_SCHEDULE_confirm(cMessage *msg)
{

}

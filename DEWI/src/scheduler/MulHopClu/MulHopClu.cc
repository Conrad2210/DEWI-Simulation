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

#include <MulHopClu/MulHopClu.h>

#include "Ieee802154eConst.h"
#include "Ieee802154eNetworkCtrlInfo_m.h"
#include "Ieee802154eFrame_m.h"

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
    // TODO Auto-generated destructor stub
    delete AssociateTimer;

    delete BeaconTimer;
    delete StartTimer;
    delete ScheduleTimer;

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

    createInitialEntries();


    WATCH(notAssociated);

}

void MulHopClu::handleMessage(cMessage *msg)
{

}

void MulHopClu::handleMACMessage(cMessage *msg)
{


}
void MulHopClu::handleSelfMessage(cMessage *msg)
{

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


}

void MulHopClu::handle_MLME_BEACON_confirm(cMessage *msg)
{

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

}

void MulHopClu::handle_MLME_START_confirm(cMessage *msg)
{

}

//Scan
void MulHopClu::MLME_SCAN_request(cMessage *msg)
{


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

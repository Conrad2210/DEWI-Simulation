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

#ifndef SLOTTEDALOHASCHEDULER_H_
#define SLOTTEDALOHASCHEDULER_H_

#include <IbaseScheduler.h>
#include <csimplemodule.h>
#include "IMacLinkTable.h"
#include "IMacSlotframeTable.h"
#include "IMacTimeslotTable.h"
#include "IMacHoppingSequenceList.h"
#include "IMacNeighborTable.h"

class INET_API SlottedAlohaScheduler: public cSimpleModule,public IbaseScheduler {
public:
    SlottedAlohaScheduler();
    virtual ~SlottedAlohaScheduler();

protected:
    int numInitStages() const {return 5;}

    virtual void initialize(int);

    virtual void handleMessage(cMessage *msg);

    virtual void handleSelfMessage(cMessage *msg);

    virtual void handleMACMessage(cMessage *msg);

    //Association Process
    virtual void MLME_ASSOCIATE_request(cMessage *msg );

    virtual void handle_MLME_ASSOCIATE_indication(cMessage *msg);

    virtual void MLME_ASSOCIATE_responce(cMessage *msg);

    virtual void handle_MLME_ASSOCIATE_confirm(cMessage *msg);


    //Dissassociation Process
    virtual void MLME_DISASSOCIATE_request(cMessage *msg);

    virtual void handle_MLME_DIASSOCIATE_indication(cMessage *msg);

    virtual void handle_MLME_DIASSOCIATE_confirm(cMessage *msg);

    //Advertisment

    virtual void MLME_BEACON_request(cMessage *msg);

    virtual void handle_MLME_BEACON_confirm(cMessage *msg);

    virtual void handle_MLME_BEACON_indication(cMessage *msg);

    virtual void MLME_SETSLOTFRAME_request(cMessage *msg);

    virtual void handle_MLME_SETSLOTFRAME_confirm(cMessage *msg);

    virtual void MLME_SETLINK_request(cMessage *msg);

    virtual void handle_MLME_SETLINK_confirm(cMessage *msg);

    virtual void MLME_TSCHMODE_request(cMessage *msg);

    virtual void handle_MLME_TSCHMODE_confirm(cMessage *msg);

    //START
    virtual void MLME_START_request(cMessage *msg);

    virtual void handle_MLME_START_confirm(cMessage *msg);

    //SCAN
    virtual void MLME_SCAN_request(cMessage *msg);

    virtual void handle_MLME_SCAN_confirm(cMessage *msg);

    //set Beacon
    virtual void MLME_SET_BEACON_request(cMessage *msg);

    virtual void handle_MLME_SET_BEACON_confirm(cMessage *msg);



    //build schedule
    virtual void SCHEDULE_request(cMessage *msg);
    virtual void handle_SCHEDULE_indication(cMessage *msg);
    virtual void SCHEDULE_response(cMessage *msg);
    virtual void handle_SCHEDULE_confirm(cMessage *msg);
    ////////////////////////////////////////////////////////////////////////
    ///////////////////Helper Functions//////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    void createInitialEntries();

    //Variables
protected:

    int outGate;
    int inGate;

    /** @brief Link table */
    IMacLinkTable* linkTable;
    /** @brief slotframe table */
    IMacSlotframeTable* slotframeTable;

    /** @brief timeslot table */
    IMacTimeslotTable* timeslotTable;

    /** @brief buffer for one timeslot entry */
    macTimeslotTableEntry* timeslotTemplate;

    /** @brief Hopping Sequence List */
    IMacHoppingSequenceList* hoppingSequenceList;

    /** @brief neighbor information table */
    IMacNeighborTable* neighborTable;

    bool isPANCoor;

    int lastSCANChannel;

    bool notAssociated;

protected:
    //////////TMER
    cMessage *BeaconTimer;
    cMessage *StartTimer;
    cMessage *AssociateTimer;
    cMessage *ScheduleTimer;
    //cMessage *AssociateWaitTimer;
};

#endif /* SLOTTEDALOHASCHEDULER_H_ */

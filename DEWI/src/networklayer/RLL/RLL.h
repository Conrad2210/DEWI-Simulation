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

#ifndef RLL_H_
#define RLL_H_
#include "INETDefs.h"
#include <omnetpp.h>
#include "IRLLClusterTable.h"
#include "IMacLinkTable.h"
#include "IMacSlotframeTable.h"
#include "IMacTimeslotTable.h"
#include "IMacHoppingSequenceList.h"
#include "IMacNeighborTable.h"

class INET_API RLL : public cSimpleModule
{
    public:
	RLL();
	virtual ~RLL();

	virtual int numInitStages() const
	{
	    return 5;
	}
	virtual void initialize(int);
	virtual void finish();

    protected:
	virtual void handleMessage(cMessage *msg);

	virtual void handleSelfMessage(cMessage *msg);

	virtual void handleMACMessage(cMessage *msg);

    protected:
	//Association Process
	virtual void MLME_ASSOCIATE_request(cMessage *msg);

	virtual void handle_MLME_ASSOCIATE_indication(cMessage *msg);

	virtual void MLME_ASSOCIATE_response(cMessage *msg);

	virtual void handle_MLME_ASSOCIATE_confirm(cMessage *msg);

	//Dissassociation Process
	virtual void MLME_DISASSOCIATE_request(cMessage *msg);

	virtual void handle_MLME_DIASSOCIATE_indication(cMessage *msg);
	virtual void MLME_DISASSOCIATE_response(cMessage *msg);

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

	//retrieve schedule
	virtual void SCHEDULE_request(cMessage *msg);
	virtual void handle_SCHEDULE_indication(cMessage *msg);
	virtual void SCHEDULE_response(cMessage *msg);
	virtual void handle_SCHEDULE_confirm(cMessage *msg);

	//PAN COORD check for beacon from CH same stage
	virtual void handle_BEACON_WAIT_timer(cMessage *msg);
	virtual void handle_BEACON_CH_SAME_STAGE(cMessage *msg);

	virtual void RESTART_request(cMessage *msg);

	virtual void handle_RESTART_confirm(cMessage *msg);

    protected:
	int lowerLayerIn;
	int upperLayerIn;
	int lowerLayerOut;
	int upperLayerOut;

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

	// @brief cluster information table
	IRLLClusterTable *clusterTable;

	//TIMER
    protected:
	cMessage *AssociateTimer;
	cMessage *AssociateWaitTimer;

    protected:
	//is associated or not
	bool bNotAssociated;

	//Is pan coor or not
	bool bIsPANCoor;

	bool bCapablePanCoor;

	//saves the cluster stage
	int nCluStage;
};

#endif /* RLL_H_ */

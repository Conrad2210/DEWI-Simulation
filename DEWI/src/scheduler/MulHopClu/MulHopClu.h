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

#ifndef MULHOPCLU_H_
#define MULHOPCLU_H_

#include <csimplemodule.h>
#include <IbaseScheduler.h>

#include "IMacLinkTable.h"
#include "IMacSlotframeTable.h"
#include "IMacTimeslotTable.h"
#include "IMacHoppingSequenceList.h"
#include "IMacNeighborTable.h"
#include "beaconTable.h"
#include "clusterTable.h"

class MulHopClu : public cSimpleModule, public IbaseScheduler
{
    public:
	MulHopClu();
	virtual ~MulHopClu();

	int numInitStages() const
	{
	    return 5;
	}

	virtual void initialize(int);

	virtual void handleMessage(cMessage *msg);

	virtual void handleSelfMessage(cMessage *msg);

	virtual void handleMACMessage(cMessage *msg);

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


	////////////////////////////////////////////////////////////////////////
	///////////////////Helper Functions//////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	void createInitialEntries();

	void setSchedule();
	void setScheduleChStUn();
	void setScheduleChStZe();
	void setScheduleChInit();
	void setScheduleCs();

	double calcDistance(double transPowmW, double minRecvPowermW);

	double mWTodBm(double mW){return 10*log10(mW);}
	double dBmTomW(double dBm){return pow(10,dBm/10);}




	void updateDisplay();
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

	//last Neighbor
	macNeighborTableEntry *lastNeighbor;

	//.temp link entry for scheduler messages
	//entry for received requests
	macLinkTableEntry *tempLinkEntryRx;

	//entry for transmitted requests
	macLinkTableEntry *tempLinkEntryTx;



    protected:
	//////////TMER
	cMessage *BeaconTimer;
	cMessage *StartTimer;
	cMessage *AssociateTimer;
	cMessage *ScheduleTimer;
	cMessage *ScanTimer;
	cMessage *BeaconScanTimer;

	cMessage *AssociateWaitTimer;
	cMessage *DisassociateWaitTimer;

	beaconTable BeaconTable;
	clusterTable ClusterTable;

	//saves the cluster stage
	int nCluStage;

	//Beacon Order
	int nBO;

	//Superframe order
	int nSO;

	//BeaconIntervaltime
	double fBI;

	//For PanCoord,
	//after association store's the number of scans for beacon
	int nScanCounter;

	// Variable to set the length of the initial channel scan period
	int nScanDuration;

	// is scanning for beacon from Ch same stage
	bool bScanBeaconCH;

	//Is pan coor or not
	bool bIsPANCoor;

	//is capable to become a pan coord or not
	bool bCapablePanCoor;

	//tried to associate more than once
	bool bAssociateDirectly;

	//variable to store the channel which was scanned before
	int nLastSCANChannel;

	//is associated or not
	bool bNotAssociated;

	//stores the sensitivity
	double fSensitivity;

	//variable to stores the probability to become a CH
	double fPCH;

	//stores the transmitpower
	double fTransmitterPower;

	//counts the restarts
	int nRestartCounter;
};

#endif /* MULHOPCLU_H_ */

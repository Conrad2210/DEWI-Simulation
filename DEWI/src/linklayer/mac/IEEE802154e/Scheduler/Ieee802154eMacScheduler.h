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

#ifndef IEEE802154EMACSCHEDULER_H_
#define IEEE802154EMACSCHEDULER_H_

#include "Ieee802154eMac.h"

class Ieee802154eMacScheduler : public Ieee802154eMac
{
    public:
	Ieee802154eMacScheduler();
	virtual ~Ieee802154eMacScheduler();

    public:
	virtual int numInitStages() const
	{
	    return 5;
	}
	virtual void initialize(int stage);

    protected:

	virtual void handleMessage(cMessage *msg);
	virtual void handleSelfMsg(cMessage*);
	virtual void handleSchedulerMsg(cMessage *msg);
	virtual void handleLowerMsg(cPacket*);

	/**
	 * @name MAC management service - Std 802.15.4-2011 (table 8) page 77
	 */
	//@{
	virtual void handle_MLME_ASSOCIATE_request(cMessage *msg);
	virtual void MLME_ASSOCIATE_indication(cMessage *msg);
	virtual void handle_MLME_ASSOCIATE_response(cMessage *msg);
	virtual void MLME_ASSOCIATE_confirm(cMessage *msg);
	virtual void handle_MLME_DISASSOCIATE_request(cMessage *msg);
	virtual void MLME_DISASSOCIATE_indication(cMessage *msg);
	virtual void handle_MLME_DISASSOCIATE_response(cMessage *msg);
	virtual void MLME_DISASSOCIATE_confirm(cMessage *msg);

	virtual void handle_MLME_SCAN_request(cMessage *msg);
	virtual void MLME_SCAN_confirm(cMessage *msg);

	virtual void handle_MLME_START_request(cMessage *msg);
	virtual void MLME_START_confirm(MACenum status);

	virtual void handle_MLME_BEACON_request(cMessage *msg);
	virtual void MLME_BEACON_confirm(MACenum status);
	virtual void MLME_BEACON_NOTIFY_indication(cMessage *msg);

	virtual void handle_MLME_SET_SLOTFRAME_request(cMessage *msg);

	//SCHEDULE

	virtual void handle_SCHEDULE_request(cMessage *msg);
	virtual void SCHEDULE_indication(cMessage *msg);
	virtual void handle_SCHEDULE_response(cMessage *msg);
	virtual void SCHEDULE_confirm(cMessage *msg, bool ack);

	//Multihop CLustering
	virtual void handle_RESTART_request(cMessage *msg);
	virtual void RESTART_confirm(cMessage *msg);

	virtual void handleAwaitingBeaconTimer();

	virtual void handleEB(cMessage *msg);
	virtual void handleAck(Ieee802154eFrame *frame);


	virtual void handleAsnTimer();
	virtual void handleTsAckWait();

    protected:

	int mSchedulerIn;
	int mSchedulerOut;

	bool awaitingNextBeacon;

	cMessage* awaitingBeacon;
	cMessage *scanTimer;
};

#endif /* IEEE802154EMACSCHEDULER_H_ */

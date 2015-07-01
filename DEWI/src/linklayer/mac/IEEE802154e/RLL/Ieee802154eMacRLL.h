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

#ifndef IEEE802154EMACRLL_H_
#define IEEE802154EMACRLL_H_

#include "Ieee802154eMac.h"
#include "DataCenter.h"
#include "DataVector.h"

class Ieee802154eMacRLL : public Ieee802154eMac
{
    public:
	Ieee802154eMacRLL();
	virtual ~Ieee802154eMacRLL();
	virtual void finish();
	virtual int numInitStages() const
	{
	    return 5;
	}
	virtual void initialize(int stage);
    protected:
	virtual void handleMessage(cMessage *msg);
	virtual bool handleSchedulerMsg(cMessage *msg);
	virtual void handleLowerMsg(cPacket*);
	virtual void handleSelfMsg(cMessage*);
	virtual void handleUpperMsg(cPacket *);


	virtual void MCPS_DATA_indication(Ieee802154eAddrMode srcAddrMode, UINT_16 srcPANId, IE3ADDR srcAddr, Ieee802154eAddrMode dstAddrMode, UINT_16 dstPANId, IE3ADDR dstAddr, UINT_8 msduLength, Ieee802154eFrame* msdu, UINT_8 mpduLinkQuality, UINT_8 dsn, UINT_32 Timestamp, UINT_8 SecurityLevel, UINT_8 KeyIdMode, UINT_64 keySource, UINT_8 keyIndex, UINT_8 uwbPRF, Ieee802154eUWBFType uwbPreambleSymbolRepetitions, UINT_8 dataRate, RangingControl rangingReceived, UINT_32 rangingCounterStart, UINT_32 rangingCounterStop, UINT_32 rangingTrackingInterval, UINT_32 rangingOffset, UINT_8 rangingFOM);
	virtual void MCPS_DATA_request(Ieee802154eAddrMode srcAddrMode, Ieee802154eAddrMode dstAddrMode, UINT_16 dstPANId, IE3ADDR dstAddr, UINT_8 msduLength, cPacket* msdu, UINT_8 msduHandle, bool ackTX, bool gtsTX, bool indirectTX, UINT_8 securityLevel, UINT_8 keyIdMode, UINT_64 keySource, UINT_8 keyIndex, Ieee802154eUWBFType uwbPRF, RangingControl ranging, UINT_16 uwbPreambleSymbolRepetitions,
			UINT_8 dataRate, FrameCrlOptions frameControlOptions, std::vector<Ieee802154eIEHeaderType> headerIElist, std::vector<Ieee802154eIEPayloadType> payloadIElist, bool sendMultipurpose);

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
	bool awaitingNextBeacon;

	cMessage* awaitingBeacon;
	cMessage *scanTimer;


};

#endif /* IEEE802154EMACRLL_H_ */

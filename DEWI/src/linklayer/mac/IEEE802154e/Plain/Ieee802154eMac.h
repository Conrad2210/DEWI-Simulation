/**
 * IEEE 802.15.4 Media Access Control (MAC) Layer
 * Refer to IEEE Std 802.15.4-2006, IEEE Std 802.15.4-2011 & IEEE Std 802.15.4e-2012
 *
 * @author Feng Chen
 * @ingroup macLayer
 *
 * @author: 2014    Stefan Reis      (modified)
 */

#ifndef IEEE_802154_E_MAC_H
#define IEEE_802154_E_MAC_H

#include <list>

#include "RadioState.h"
#include "Ieee802154eConst.h"
#include "Ieee802154eDef.h"
#include "Ieee802154eEnum.h"
#include "Ieee802154eField.h"
#include "Ieee802154Link.h"
#include "Ieee802154eFrame_m.h"
#include "Ieee802154eMacPhyPrimitives_m.h"
#include "Ieee802154eNetworkCtrlInfo_m.h"
#include "NotificationBoard.h"
#include "NotifierConsts.h"
#include "MACAddress.h"
#include "InterfaceEntry.h"
#include "WirelessMacBase.h"
#include "FWMath.h"

//#include "IPassiveQueue.h"
#include "Ieee802154eQueue.h"

#include "IMacLinkTable.h"
#include "IMacSlotframeTable.h"
#include "IMacTimeslotTable.h"
#include "IMacHoppingSequenceList.h"
#include "IMacNeighborTable.h"

class IMacSlotframeTable;
class ImacLinkTable;
class IMacTimeslotTable;
class IMacHoppingSequenceList;
class IMacNeighborTable;

/**
 * For the CSMA-CA and TSCH CSMA-CA algorithm
 */
struct taskPending
{
	taskPending()
	{
	    init();
	}
	inline void init()
	{
	    mcps_data_request = false;
	    mcps_data_request_STEP = 0;
	    CCA_csmaca = false;
	    CCA_csmaca_STEP = 0;
	    RX_ON_csmaca = false;
	    RX_ON_csmaca_STEP = 0;
	    TS_TX_CCA_tschcca = false;
	    TS_TX_CCA_tschcca_STEP = 0;
	    TS_RX_CCA_tschcca = false;
	    TS_RX_CCA_tschcca_STEP = 0;
	}

	bool &taskStatus(Ieee802154eMacTaskType task)
	{
	    switch(task)
	    {
		case TP_MCPS_DATA_REQUEST:
		    return mcps_data_request;
		case TP_CCA_CSMACA:
		    return CCA_csmaca;
		case TP_RX_ON_CSMACA:
		    return RX_ON_csmaca;
		    // for TSCH CCA - Transmitter
		case TP_TS_TX_CCA_TSCHCCA:
		    return TS_TX_CCA_tschcca;
		    // for TSCH CCA - Receiver
		case TP_TS_RX_CCA_TSCHCCA:
		    return TS_RX_CCA_tschcca;

		default:
		    ASSERT(0);
		    // shutup the compiler.
		    return RX_ON_csmaca;
	    }
	}

	UINT_8 &taskStep(Ieee802154eMacTaskType task)
	{
	    switch(task)
	    {
		case TP_MCPS_DATA_REQUEST:
		    return mcps_data_request_STEP;
		case TP_CCA_CSMACA:
		    return CCA_csmaca_STEP;
		case TP_RX_ON_CSMACA:
		    return RX_ON_csmaca_STEP;
		    // for TSCH CCA - Transmitter
		case TP_TS_TX_CCA_TSCHCCA:
		    return TS_TX_CCA_tschcca_STEP;
		    // for TSCH CCA - Receiver
		case TP_TS_RX_CCA_TSCHCCA:
		    return TS_RX_CCA_tschcca_STEP;

		default:
		    ASSERT(0);
		    // shutup compiler.
		    return RX_ON_csmaca_STEP;
	    }
	}

	char *taskFrFunc(Ieee802154eMacTaskType task)
	{
	    switch(task)
	    {
		case TP_MCPS_DATA_REQUEST:
		    return mcps_data_request_frFunc;
		default:
		    ASSERT(0);
		    // shutup compiler.
		    //return mlme_poll_request_frFunc;
	    }
	    return 0;
	}

	//----------------
	bool mcps_data_request;
	UINT_8 mcps_data_request_STEP;
	char mcps_data_request_frFunc[81];
	Ieee802154eTxOption mcps_data_request_TxOption;
	Ieee802154eFrame* mcps_data_request_pendPkt;
	//----------------
	bool CCA_csmaca;
	UINT_8 CCA_csmaca_STEP;
	//----------------
	bool RX_ON_csmaca;
	UINT_8 RX_ON_csmaca_STEP;
	//--- TSCH CCA - Transmitter ---
	bool TS_TX_CCA_tschcca;
	UINT_8 TS_TX_CCA_tschcca_STEP;
	//--- TSCH CCA - Receiver ---
	bool TS_RX_CCA_tschcca;
	UINT_8 TS_RX_CCA_tschcca_STEP;
	//----------------
};

class INET_API Ieee802154eMac : public WirelessMacBase
{
    public:
	/**
	 * @name Constructor and destructor
	 */
	//@{
	Ieee802154eMac();
	~Ieee802154eMac();
	//@}

	/** @brief called by other MAC modules in order to translate host name into MAC address */
	IE3ADDR getMacAddr()
	{
	    return aExtendedAddress;
	}
	/** @brief called by other MAC modules in order to translate host name into MAC address */

    protected:
	/** @brief the bit rate at which we transmit */
	double bitrate;

	/**
	 * @name Initialization functions
	 */
	//@{
	virtual void initialize(int);
	virtual void initializeQueueModule();
	virtual int numInitStages() const
	{
	    return 4;
	}

	void commonInitialize();    // common initialization Ieee802154MAC csma802154

	virtual InterfaceEntry *createInterfaceEntry();

	/** should clear queue and (emit signal "dropPkFromHLIfaceDown" with entire packets) */
	virtual void flushQueue();

	/** should clear queue silently */
	virtual void clearQueue();
	//@}

	/**
	 * @name Link Table
	 */
	//@{
	/**[SR] get the link to the ASN back */
	virtual macLinkTableEntry* getLinkEntry(UINT_64 asnCounter);

	/**[SR] get the next active timeslot (include the future ASN counter for the next active timeslot) */
	virtual double getNextActiveTimeslot();
	//@}

	virtual void finish();
	virtual void receiveChangeNotification(int, const cPolymorphic*);

	// Functions for starting a WPAN with star topology
	virtual void startPANCoor();
	virtual void startDevice();

	/**
	 * @name Message handling functions
	 */
	//@{
	virtual void handleMessage(cMessage *msg);
	virtual void handleSelfMsg(cMessage*);
	virtual void handleUpperMsg(cPacket*);   // for MCPS-SAP (Data)
	virtual void handlePrimitive(int msgkind, cMessage *msg);  // for MLME-SAP (Commands)
	virtual void handleLowerMsg(cPacket*);
	virtual void handleMacPhyPrimitive(int, cMessage*);
	virtual void handleBeacon(Ieee802154eFrame*);
	virtual void handleEB(cMessage *msg);
	virtual void handleCommand(cMessage *msg)
	{
	}
	;

	//virtual void    handleCommand80215(Ieee802154eFrame*);
	virtual void handleData(Ieee802154eFrame*);
	virtual void handleAck(Ieee802154eFrame*);
	virtual void handleLLDN802154e(Ieee802154eFrame*);   // LLDN (Low Latency Deterministic Network)
	virtual void handleMulti802154e(Ieee802154eFrame*);  // Multipurpose

	/** @brief MAC frame filter, return true if frame is filtered out */
	virtual bool frameFilter(Ieee802154eFrame*);
	virtual void sendDown(Ieee802154eFrame*);
	virtual void constructACK(Ieee802154eFrame*);
	virtual void constructBCN();

	/** @brief request a msg from IFQueue, if it exists */
	virtual void reqtMsgFromIFq();
	//@}

	/**
	 * @name Command handling functions
	 *  for convenience, some MAC commands are implemented in a tricky way in the model
	 *  these functions are only called directly by other MAC modules, instead of via messages exchange like in real world
	 */
	//@{
	/** This function is a part of simplified association process in the model.
	 *  After receiving a beacon from the coordinator for the first time and associating with it,
	 *  each device should call this function of its coordinator to register its MAC address and
	 *  capability info at the coordinator and get the short MAC address from the return value */
	virtual UINT_16 associate_request_cmd(IE3ADDR extAddr, const DevCapability& capaInfo);

	/** This function is a part of simplified GTS request process in the model.
	 *  After association, each device should call this function of its PAN Coordinator
	 *  to apply for GTS and get its GTS starting number from the return value */
	virtual uint16_t gts_request_cmd(UINT_16 macShortAddr, uint16_t length, bool isReceive);

	/**
	 * @name PHY service specifications
	 * Std 802.15.4-2006 (table 5) page 32...
	 *
	 * MAC-PHY primitives related functions
	 */
	//@{
	virtual void PD_DATA_request(UINT_8 psduLength, Ieee802154eFrame* psdu);

	// PHY management service - Std 802.15.4-2006 (table 9) page 35
	virtual void PLME_CCA_request();
	virtual void PLME_ED_request();
	virtual void PLME_GET_request(PHYPIBenum pibAttribute);
	virtual void PLME_SET_TRX_STATE_request(PHYenum state);
	virtual void PLME_SET_request(PHYPIBenum pibAttribute, PHY_PIB pibAttributeValue);

	virtual void handle_PD_DATA_confirm(PHYenum status);
	virtual void handle_PD_DATA_indication(UINT_8 psduLength, cPacket* psdu, UINT_8 ppduLinkQuality);
	virtual void handle_PLME_CCA_confirm(PHYenum status);
	virtual void handle_PLME_ED_confirm(PHYenum status, UINT_8 energyLevel);
	virtual void handle_PLME_GET_confirm(PHYenum status, PHYPIBenum pibAttribute, PHY_PIB pibAttributeValue);
	virtual void handle_PLME_SET_TRX_STATE_confirm(PHYenum status);
	virtual void handle_PLME_SET_confirm(PHYenum status, PHYPIBenum pibAttribute);
	//@}

	/**
	 * @name MAC data service - MCPS-SAP -
	 * Std 802.15.4-2011 (table 46) page 117 - Std 802.15.4e-2012 (table 46) page 168
	 *
	 * SSCS-MAC primitives related functions
	 */
	//@{
	virtual void MCPS_DATA_request(Ieee802154eAddrMode srcAddrMode, Ieee802154eAddrMode dstAddrMode, UINT_16 dstPANId, IE3ADDR dstAddr, UINT_8 msduLength, cPacket* msdu, UINT_8 msduHandle, bool ackTX, bool gtsTX, bool indirectTX, UINT_8 securityLevel, UINT_8 keyIdMode, UINT_64 keySource, UINT_8 keyIndex, Ieee802154eUWBFType uwbPRF, RangingControl ranging, UINT_16 uwbPreambleSymbolRepetitions, UINT_8 dataRate, FrameCrlOptions frameControlOptions, std::vector<Ieee802154eIEHeaderType> headerIElist, std::vector<Ieee802154eIEPayloadType> payloadIElist, bool sendMultipurpose);
	virtual void MCPS_DATA_confirm(UINT_8 msduHandle, UINT_64 timestamp, bool rangingReceived, UINT_32 rangingCounterStart, UINT_32 rangingCounterStop, UINT_32 rangingTrackingInterval, UINT_32 rangingOffset, UINT_8 rangingFOM, MACenum status, UINT_8 numBackoffs, UINT_8 dsn, cPacket* ackPayload);
	virtual void MCPS_DATA_indication(Ieee802154eAddrMode srcAddrMode, UINT_16 srcPANId, IE3ADDR srcAddr, Ieee802154eAddrMode dstAddrMode, UINT_16 dstPANId, IE3ADDR dstAddr, UINT_8 msduLength, Ieee802154eFrame* msdu, UINT_8 mpduLinkQuality, UINT_8 dsn, UINT_32 Timestamp, UINT_8 SecurityLevel, UINT_8 KeyIdMode, UINT_64 keySource, UINT_8 keyIndex, UINT_8 uwbPRF, Ieee802154eUWBFType uwbPreambleSymbolRepetitions, UINT_8 dataRate, RangingControl rangingReceived, UINT_32 rangingCounterStart, UINT_32 rangingCounterStop, UINT_32 rangingTrackingInterval, UINT_32 rangingOffset, UINT_8 rangingFOM);
	virtual void MCPS_PURGE_request(UINT_8 msduHandle);
	virtual void MCPS_PURGE_confirm(UINT_8 msduHandle, MACenum status);
	//@}

	/**
	 * @name CSMA/CA related functions
	 */
	//@{
	virtual void csmacaEntry(char pktType);  // 'c': txBcnCmd; 'u': txBcnCmdUpper; 'd': txData
	virtual void csmacaResume();
	virtual void csmacaStart(bool firsttime, Ieee802154eFrame* frame = 0, bool ackReq = 0);
	virtual void csmacaCancel();
	virtual void csmacaCallBack(PHYenum status); // CSMA-CA success or failure
	virtual void csmacaReset(bool bcnEnabled);
	virtual bool csmacaCanProceed(simtime_t wtime, bool afterCCA = false);
	virtual void csmaca_handle_RX_ON_confirm(PHYenum status);    // To be called by handle_PLME_SET_TRX_STATE_confirm
	virtual void csmacaTrxBeacon(char trx);  // To be called each time that a beacon received or transmitted
	virtual simtime_t csmacaAdjustTime(simtime_t wtime);
	virtual simtime_t csmacaLocateBoundary(bool toParent, simtime_t wtime);
	virtual simtime_t getFinalCAP(char trxType);
	//@}

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

	virtual void MLME_BEACON_NOTIFY_indication(cMessage *msg);
	virtual void MLME_COMM_STATUS_indication(UINT_16 panId, Ieee802154eAddrMode srcAddrMode, IE3ADDR srcAddr, Ieee802154eAddrMode dstAddrMode, IE3ADDR dstAddr, MACenum status, UINT_8 securityLevel, UINT_8 keyIdMode, UINT_64 keySource, UINT_8 keyIndex);
	virtual void handle_MLME_GET_request(MACPIBenum pibAttribute);
	virtual void MLME_GET_confirm(MACenum status, MACPIBenum pibAttribute, MAC_PIB *pibAttributeValue);
	virtual void handle_MLME_GTS_request(UINT_8 gtsCharacteristics, UINT_8 securityLevel, UINT_8 keyIdMode, UINT_64 keySource, UINT_8 keyIndex);
//    virtual void    MLME_GTS_indication(UINT_16 deviceAddress, GTS_Char_Field gtsCharacteristics, UINT_8 securityLevel, UINT_8 keyIdMode,
//                                            UINT_64 keySource, UINT_8 keyIndex);
//    virtual void    MLME_GTS_confirm(GTS_Char_Field gtsCharacteristics, MACenum status);
	virtual void MLME_ORPHAN_indication(IE3ADDR orphanAddress, UINT_8 securityLevel, UINT_8 keyIdMode, UINT_64 keySource, UINT_8 keyIndex);
	virtual void MLME_ORPHAN_responce(IE3ADDR orphanAddress, UINT_16 shortAddress, bool associatedMember, UINT_8 securityLevel, UINT_8 keyIdMode, UINT_64 keySource, UINT_8 keyIndex);
	virtual void handle_MLME_RESET_request(bool setDefaultPIB);
	virtual void MLME_RESET_confirm(MACenum status);
	virtual void handle_MLME_RX_ENABLE_request(bool deferPermit, UINT_32 rxOnTime, UINT_32 rxOnDuration, RangingControl rangingRxControl);
	virtual void MLME_RX_ENABLE_confirm(MACenum status);
	virtual void handle_MLME_SCAN_request(cMessage *msg);
	virtual void MLME_SCAN_confirm(cMessage *msg);

	virtual void handle_MLME_SET_SLOTFRAME_request(cMessage *msg);
	virtual void handle_MLME_SET_request(MACPIBenum pibAttribute, MAC_PIB pibAttributeValue);
	virtual void MLME_SET_confirm(MACenum status, MACPIBenum pibAttribute);
	virtual void handle_MLME_START_request(cMessage *msg);
	virtual void MLME_START_confirm(MACenum status);
	virtual void handle_MLME_SYNC_request(UINT_8 channelNumber, UINT_8 channelPage, bool trackBeacon);
	virtual void MLME_SYNC_LOSS_indication(MACenum lossReason, UINT_16 panId, UINT_8 channelNumber, UINT_8 channelPage, UINT_8 securityLevel, UINT_8 keyIdMode, UINT_64 keySource, UINT_8 keyIndex);
	virtual void handle_MLME_POLL_request(Ieee802154eAddrMode coordAddrMode, UINT_16 coordPANId, IE3ADDR coordAddress, UINT_8 securityLevel, UINT_8 keyIdMode, UINT_64 keySource, UINT_8 keyIndex);
	virtual void MLME_POLL_confirm(MACenum status);
	virtual void handle_MLME_DPS_request(UINT_8 txDPSIndex, UINT_8 rxDPSIndex, UINT_32 dpsIndexDuration);
	virtual void MLME_DPS_indication();
	virtual void MLME_DPS_confirm(MACenum status);
	virtual void handle_MLME_SOUNDING_request();
	virtual void MLME_SOUNDING_confirm(std::vector<int> soundingList, MACenum status);
	// for UWB PHYs
	//virtual void    handle_MLME_CALIBRATE_request();
	//virtual void    MLME_CALIBRATE_confirm(MACenum status, UINT_64 CalTxRMARKEROffset, UINT_64 CalRxRMARKEROffset);

	// Std 802.15.4e-2012 page 140...
	virtual void handle_MLME_BEACON_request(cMessage *msg);
	virtual void MLME_BEACON_confirm(MACenum status);
	virtual void MLME_BEACON_REQUEST_indication(Ieee802154eBeaconType beaconType, UINT_8 srcAddrMode, IE3ADDR srcAddr, UINT_16 dstPANID, std::vector<IE_LIST> ieList);
	//@}

	/**
	 * @name TSCH MAC management service - Std 802.15.4e-2012 (table 8a) page 123
	 */
	//@{
	virtual void MLME_SET_SLOTFRAME_confirm(UINT_8 slotframeHandle, MACenum status);
	virtual void handle_MLME_SET_LINK_request(Ieee802154eOperation operation, UINT_16 linkHandle, UINT_8 slotframeHandle, UINT_16 timeslot, UINT_16 channelOffset, MACTSCHLinkOptions linkOptions, MACTSCHLinkType linkType, UINT_16 nodeAddr);
	virtual void MLME_SET_LINK_confirm(MACenum status, UINT_16 linkHandle, UINT_8 slotframeHandle);
	virtual void handle_MLME_TSCH_MODE_request(Ieee802154eTSCHMode tschMode);
	virtual void MLME_TSCH_MODE_confirm(Ieee802154eTSCHMode tschMode, MACenum status);
	virtual void handle_MLME_KEEP_ALIVE_request(UINT_16 dstAddr, UINT_16 keepAlivePeriod);
	virtual void MLME_KEEP_ALIVE_confirm(MACenum status);
	//@}

	virtual void handle_RESTART_request(cMessage *msg);
	virtual void RESTART_confirm(cMessage *msg);
	virtual void handle_SCHEDULE_request(cMessage *msg);
	virtual void SCHEDULE_indication(cMessage *msg);
	virtual void handle_SCHEDULE_response(cMessage *msg);
	virtual void SCHEDULE_confirm(cMessage *msg, bool ack);
	/**
	 * @name State control and task management functions
	 */
	//@{
	virtual void dispatch(PHYenum pStatus, const char *frFunc, PHYenum req_state = phy_SUCCESS, MACenum mStatus = mac_SUCCESS);
	virtual void taskSuccess(char type, bool csmacaRes = true);
	virtual void taskFailed(char type, MACenum status, bool csmacaRes = true);
	virtual void checkTaskOverflow(Ieee802154eMacTaskType task);
	virtual void FSM_MCPS_DATA_request(PHYenum pStatus = phy_SUCCESS, MACenum mStatus = mac_SUCCESS);
	virtual void resetTRX();
	//@}

	/**
	 * @name Timer handling functions
	 */
	//@{
	virtual void handleAwaitingBeaconTimer();
	virtual void handleBackoffTimer();
	virtual void handleDeferCCATimer();
	virtual void handleBcnRxTimer();
	virtual void handleBcnTxTimer();
	virtual void handleAckTimeoutTimer();
	virtual void handleTxAckBoundTimer(); // ACK is sent here
	virtual void handleTxCmdDataBoundTimer();    // Cmd or data is sent here
	virtual void handleIfsTimer();
	virtual void handleSDTimer();    // shared by txSDTimer and rxSDTimer
	virtual void handleFinalCapTimer();
	virtual void handleGtsTimer();
	virtual void handleAsnTimer();
	//@}

	/**
	 * @name GTS related functions
	 */
	//@{
	virtual bool gtsCanProceed();
	virtual void gtsScheduler();
	//@}

	/**
	 * @name Timer starting functions
	 */
	//@{
	virtual void startBackoffTimer(simtime_t);
	virtual void startDeferCCATimer(simtime_t);
	virtual void startBcnRxTimer();
	virtual void startBcnTxTimer(bool txFirstBcn = false, simtime_t startTime = 0.0);
	virtual void startAckTimeoutTimer();
	virtual void startTxAckBoundTimer(simtime_t);
	virtual void startTxCmdDataBoundTimer(simtime_t);
	virtual void startIfsTimer(bool);
	virtual void startTxSDTimer();
	virtual void startRxSDTimer();
	virtual void startGtsTimer(simtime_t);
	virtual void startFinalCapTimer(simtime_t);
	// for TSCH Std 802.15.4e-2012
	virtual void startAsnTimer(simtime_t);
	//@}

	/**
	 * @name TSCH related functions - Std 802.15.4e-2012 (Figure 22b�Timeslot diagram of acknowledged transmission) page 28
	 */
	//@{
	// transmission: macTsTxOffset
	/** @brief Transmission CCA Offset */
	virtual void handleTsCCAOffset();
	/** @brief Transmission CCA Offset */
	virtual void handleTsTxOffset();
	/** @brief Transmission delay before wait for ack */
	virtual void handleTsRxAckDelay();
	/** @brief Transmission delay before wait for ack */
	virtual void handleTsAckWait();

	// Receiving:
	/** @brief Receiving CCA Offset */
	virtual void handleTsRxOffset();
	/** @brief Receiving wait for transmission */
	virtual void handleTsRxWait();
	/** @brief Receiving wait for transmission of the ack */
	virtual void handleTsTxAckDelay();

	// Information Elements
	/** @brief handle the Header and Payload IE's - give true back if there was a NACK IE in the frame */
	virtual bool handleIEfield(Ieee802154eFrame* frame);
	//@}

	/**
	 * @name Utility functions
	 */
	//@{
	/** @brief check if the packet is sent to its parent or not */
	virtual bool toParent(Ieee802154eFrame*);

	/** @brief calculate byte length of frame of certain type */
	virtual int calFrmByteLength(Ieee802154eFrame*);

	/** @brief calculate byte length of frame header  */
	virtual int calMHRByteLength(uint8_t, uint8_t);

	/** @brief calculate duration of the frame transmitted over wireless channel  */
	virtual simtime_t calDuration(Ieee802154eFrame*);

	/** @brief return current bit or symbol rate  at PHY*/
	virtual double getRate(char);

	/** @brief[SR] calculate the time when the first bit of the frame was received */
	virtual simtime_t getArrivalTime(Ieee802154eFrame*);

	/** @brief[SR] calculate the time correction (for TSCH mode) */
	virtual void calcTimeCorr(Ieee802154eFrame*);
	//@}

	//Check beacon ASN
	virtual void checkBeaconASN(Ieee802154eFrame*);

	// Use to distinguish the radio module that send the event
	int radioModule;
	InterfaceEntry *iface;
	RadioState::State radioState;

	int getRadioModuleId()
	{
	    return radioModule;
	}

	MACAddress configurationMacAddress();

	void MPIBcopyStandard();

	void updateNeighbor(Ieee802154eFrame *frame, FrameCtrl frmCtrl);
// member variables
    public:

    protected:
	UINT_16 getShortAddress(MACAddress v)
	{
	    return (UINT_16)v.getInt();
	}
	MACAddress getLongAddress(UINT_16 v);

	/** @brief to get the PAN ID compression - Std 802.15.4e-2012 */
	bool getPANIDComp(FrameCtrl frmCtrl, UINT_16 srcPanId, UINT_16 dstPanId);

	/**
	 * @name LinkTable, SlotframeTable and TimeslotTable
	 */
	//@{
	/** @brief number of all Hosts */
	int numHosts;

	/** @brief current ASN */
	int curASN;

	/** @brief slotframe table */
	IMacSlotframeTable* slotframeTable;

	/** @brief timeslot table */
	IMacTimeslotTable* timeslotTable;

	/** @brief buffer for one timeslot entry */
	macTimeslotTableEntry* timeslotTemplate;

	/** @brief the active used timeslot ID */
	UINT_8 useTimeslotID;

	/** @brief Link table */
	IMacLinkTable* linkTable;

	/** @brief active LinkEntry */
	macLinkTableEntry* activeLinkEntry;

	/** @brief Hopping Sequence List */
	IMacHoppingSequenceList* hoppingSequenceList;

	/** @brief the active used hopping sequence ID */
	UINT_8 useHoppingSequenceID;

	/** @brief neighbor information table */
	IMacNeighborTable* neighborTable;

	/** @brief active neighbor information entry */
	macNeighborTableEntry* activeNeighborEntry;
	//@}

	/**
	 * @name User-adjustable MAC parameters
	 */
	//@{
	/** @brief debug switch */
	bool m_debug;

	/** @brief enable or disable ACK for data transmission in CAP*/
	bool ack4Data;

	/** @brief enable or disable ACK for data transmission in CFP */
	bool ack4Gts;

	/** @brief enable or disable ACK for broadcast transmission (from CSMA802154)*/
	bool ack4Broadcast;

	/** @brief enable or disable security for data frames (TODO) */
	bool secuData;

	/** @brief enable or disable security for beacon frames (TODO) */
	bool secuBeacon;

	/** @brief I'm the PAN coordinator or not
	 * If this value is TRUE, the device will become the PAN coordinator of a new PAN.
	 * If this value is FALSE, the device will begin using a new superframe configuration
	 * on the PAN with which it is associated.
	 * see MLME-Start.request - Std 802.15.4-2006 (table 72) page 123 */
	bool isPANCoor;

	/** @brief TRUE if a coordinator realignment command is to be transmitted prior to
	 * changing the superframe configuration or FALSE otherwise.
	 * see Start PAN coordinator - MLME-Start.request - Std 802.15.4-2006 (table 72) page 124 */
	bool coordRealignment;

	/** @brief when PAN starts
	 * The time at which to begin transmitting beacons. If this parameter is equal to 0x000000,
	 * beacon transmissions will begin immediately. Otherwise, the specified time is relative
	 * to the received beacon of the coordinator with which the device synchronizes.
	 * This parameter is ignored if either the BeaconOrder parameter has a value of 15 or the
	 * PANCoordinator parameter is TRUE. The time is specified in symbols and is rounded to a
	 * backoff slot boundary.
	 * see MLME-Start.request - Std 802.15.4-2006 (table 72) page 123 */
	simtime_t panStartTime;

	/** @brief host name of the PAN coordinator in string */
	const char* panCoorName;

	/** @brief data transfer mode:  1: direct; 2: indirect; 3: GTS */
	int dataTransMode;

	/** @brief transmit or receive GTS, only used by devices */
	bool isRecvGTS;

	/** @brief payload of data frames transmitted in GTS, copied from traffic module */
	int gtsPayload;

	/** @brief to check if the control info is from the Ieee802Ctrl or Ieee802154eNetworkCtrlInfo */
	bool useIeee802Ctrl;

	/** @brief to use the IEEE Std 802.15.4e-2012 TSCH mode */
	bool useTSCH;

	/** @brief to set this device as time source (for IEEE Std 802.15.4e-2012) */
	bool timeSource;

	/** @brief to use the CCA for the transmission (for TSCH CCA - IEEE Std 802.15.4e-2012 (5.1.1.4.2 TSCH CCA algorithm) page 14) */
	bool useCCA;

	/** @brief for the backoff calculation at shared links (for TSCH CCA - IEEE Std 802.15.4e-2012 (5.1.1.4.3 TSCH CSMA-CA algorithm) page 15) */
	bool tschSharedLink;
	//@}

	/** @brief variable to indicate if in SCAN period or not */
	bool isSCAN;

	/** @brief variable to indicate to construct and transmit a beacon */
	bool txBeaconNow;

	/**
	 * @name Static variables
	 */
	//@{
	/** @brief global counter for generating unique MAC extended address */
	static UINT_16 addrCount;

	/** @brief global counter for the timeslot where the retransmission is successful */
	static simsignal_t sucTimeslot;

	/** @brief default MAC PIB attributes */
	static MAC_PIB MPIB;
	//@}

	/**
	 * @name Module gate ID
	 */
	//@{
	int mUpperLayerIn;
	int mUpperLayerOut;
	int mLowerLayerIn;
	int mLowerLayerOut;
	int mQueueIn;
	int mQueueOut;
	//@}

	/** @brief  pointer to the NotificationBoard module */
	NotificationBoard* mpNb;

	/** @brief pointer to the passive queue module */
	// IPassiveQueue* queueModule;
	Ieee802154eQueue* queueModule;

	// for task processing
	taskPending taskP;

	/** @brief current requested radio state sent to PHY via PLME_SET_TRX_STATE_request */
	PHYenum trx_state_req;
	string trx_state_req_txt;
	bool trx_state_req_activ;

	/**
	 * @name Variables for PHY parameters
	 */
	//@{
	/** @brief PHY PIB attributes copied from PHY via notification board */
	PHY_PIB ppib, tmp_ppib;

	/** @brief current requested radio state sent to PHY via PLME_SET_TRX_STATE_request */
	PHYPIBenum ppib_pibAttribure_req;

	/** @brief current bit rate at PHY */
	double phy_bitrate;

	/** @brief current symbol rate at PHY */
	double phy_symbolrate;
	//@}

	/**
	 * @name Variables for MAC parameters
	 */
	//@{
	/** @brief MAC extended address, in simulation use only 16 bit instead of 64 bit */
	IE3ADDR aExtendedAddress;
	MACAddress macaddress;

	/** @brief MAC PIB attributes */
	MAC_PIB mpib;

	/** @brief MAC frame type strings (for debug)*/
	static string frameTypeTxt[];

	/** @brief MAC status strings (for debug)*/
	static string stateTxt[];

	/** @brief MAC parameter strings (for debug)*/
	static string paraTxt[];

	/** @brief device capability (TODO TBD) */
	DevCapability capability;
	//@}

	/**
	 * @name Beacon related variables
	 */
	//@{
	/** @brief beacon order of incoming superframe */
	uint16_t rxBO;

	/** @brief superframe order of incoming superframe */
	uint16_t rxSO;

	/** @brief duration (in symbol) of a outgoing superframe slot (aBaseSlotDuration * 2^mpib.macSuperframeOrder) */
	UINT_32 txSfSlotDuration;

	/** @brief duration (in symbol) of a incoming superframe slot (aBaseSlotDuration * 2^rxSO) */
	UINT_32 rxSfSlotDuration;

	/** @brief duration (in backoff periods) of latest outgoing beacon */
	uint16_t txBcnDuration;

	/** @brief duration (in backoff periods) of latest incoming beacon */
	uint16_t rxBcnDuration;

	/** @brief length (in s) of a unit of backoff period, aUnitBackoffPeriod/phy_symbolrate */
	simtime_t bPeriod;

	/** @brief the time that the last beacon was received
	 updated right after the beacon is received at MAC */
	simtime_t bcnRxTime;

	/** @brief the scheduled time that the latest beacon should have arrived
	 sometimes bcnRxTime lags due to beacon loss (assumed impossible in the model) or in the middle of receiving a beacon,
	 some calculations use this value to avoid incorrectness */
	simtime_t schedBcnRxTime;

	/** @brief outgoing superframe specification used by coordinators */
	SuperframeSpec txSfSpec;

	/** @brief incoming superframe specification used by devices */
	SuperframeSpec rxSfSpec;

	/** @brief outgoing PAN descriptor transmitted used by coordinators (TBD) */
	PAN_ELE txPanDescriptor;

	/** @brief incoming PAN descriptor transmitted used by devices (TBD) */
	PAN_ELE rxPanDescriptor;

	/** @brief flag for using beacon or not */
	bool beaconEnabled;

	/** @brief indicating a beacon frame waiting for transmission, suppress all other transmissions */
	bool beaconWaitingTx;

	/** @brief indicating whether associated with a coordinator or not */
	bool notAssociated;

	/** @brief num of incoming beacons lost in a row */
	uint16_t bcnLossCounter;
	//@}

	/**
	 * @name CSMA/CA related variables
	 */
	//@{
	/** @brief Defines the backoff method to be used. (from cmsa802154e)*/
	backoff_methods backoffMethod;

	/** @brief initial contention window size
	 * Only used for linear and constant backoff method. (from cmsa802154e)*/
	double initialCW;

	/** @brief NB -> number of backoffs
	 *  NB is the number of times the CSMA-CA algorithm was required to backoff
	 *  while attempting the current transmission; this value shall be initialized to
	 *  zero before each new transmission attempt.
	 *
	 *  It not in use in TSCH mode, the NB will counted directly with each packet in the mac queue [SR]*/
	uint16_t NB;

	/** @brief CW -> contention window length
	 *  CW is the contention window length, defining the number of backoff
	 *  periods that need to be clear of channel activity before the transmission can
	 *  commence; this value shall be initialized to two before each transmission
	 *  attempt and reset to two each time the channel is assessed to be busy.
	 *  The CW variable is only used for slotted CSMA-CA. (from cmsa802154e)*/
	uint16_t CW;

	/** @brief BE -> backoff exponent
	 *  BE is the backoff exponent, which is related to how many backoff
	 *  periods a device shall wait before attempting to assess a channel. In unslotted
	 *  systems, or slotted systems with the received BLE subfield (see Figure 47) set to
	 *  zero, BE shall be initialized to the value of macMinBE. In slotted systems with
	 *  the received BLE subfield set to one, this value shall be initialized to the
	 *  lesser of two and the value of macMinBE. Note that if macMinBE is set to zero,
	 *  collision avoidance will be disabled during the first iteration of this algorithm.
	 *
	 *  It also used in the TSCH CSMA-CA algorithm - Std 802.15.4e-2012 (5.1.1.4.3 TSCH CSMA-CA algorithm) page 15 */
	uint16_t BE;

	/** @brief backoff status
	 *  0: no backoff; 1: backoff successful; 2: backoff failed; 99: is backing off */
	uint16_t backoffStatus;

	/** @brief backoff periods left
	 *  number of backoff periods left */
	int bPeriodsLeft;

	/** @brief CSMA ACK requirement */
	bool csmacaAckReq;

	/** @brief CSMA wait of next beacon */
	bool csmacaWaitNextBeacon;

	//@}

	/**
	 * @name GTS related variables only for PAN coordinator
	 */
	//@{
	/** @brief number of GTS descriptors being maintained */
	uint16_t gtsCount;

	/** @brief list of GTS descriptors for all existing GTS being maintained */
	GTSDescriptor gtsList[7];

	/** @brief there is data pending, send a note in the next beacon for the device */
	bool isTxPending;    // there is a data pending for txing

	/** @brief store new value of final superframe slot in the CAP after updating GTS settings
	 and put into effect when next beacon is transmitted  */
	uint16_t tmp_finalCap;

	/** @brief index of current running GTS, 99 means not in GTS */
	uint16_t indexCurrGts;
	//@}

	/**
	 * @name GTS related variables only for devices
	 */
	//@{
	/** @brief number of superframe slots for the GTS, calculated in handleBeacon()  */
	uint16_t gtsLength;

	/** @brief GTS starting slot, 0 means no GTS for me, allocated by the PAN coordinator when applying for GTS */
	uint16_t gtsStartSlot;

	/** @brief duration of one complete data transaction in GTS, calculated in handleBeacon() */
	simtime_t gtsTransDuration;
	//@}

	/**
	 * @name Indirect data transfer related variables
	 */
	//@{
	//PendingAddrFields txPaFields; // pending address fields transmitted as a coordinator
	//PendingAddrFields rxPaFields; // pending address fields received as a device
	//@}

	/**
	 * @name Frame buffers
	 */
	//@{
	/** @brief buffer for frames currently being transmitted */
	Ieee802154eFrame* txPkt;

	/** @brief buffer for beacon frames to be transmitted without CSMA/CA */
	Ieee802154eFrame* txBeacon;

	/** @brief buffer for beacon or cmd frames coming from the upper layer,
	 to be transmitted with CSMA/CA */
	Ieee802154eFrame* txBcnCmdUpper;

	/** @brief buffer for beacon or cmd frames responding to receiving a packet,
	 to be transmitted with CSMA/CA */
	Ieee802154eFrame* txBcnCmd;

	/** @brief buffer for data frames to be transmitted */
	Ieee802154eFrame* txData;

	/** @brief buffer for data frames to be transmitted in GTS */
	Ieee802154eFrame* txGTS;

	/** @brief buffer for ack frames to be transmitted (no wait) */
	Ieee802154eFrame* txAck;

	/** @brief buffer for frames currently being CSMA/CA performed,
	 one of txBcnCmdUpper, txBcnCmd or txData */
	Ieee802154eFrame* txCsmaca;

	/** @brief temp buffer for CSMA-CA, set when csmacaStart is called for first time,
	 cleared when csmacaCallBack or csmacaCancel is called */
	Ieee802154eFrame* tmpCsmaca;

	/** @brief buffer for received beacon frames,
	 only used by mlme_scan_request and mlme_rx_enable_request (TBD) */
	Ieee802154EnhancedBeaconFrame* rxBeacon;

	/** @brief buffer for received data frames */
	Ieee802154eFrame* rxData;

	/** @brief buffer for received cmd frames */
	Ieee802154eFrame* rxCmd;

	/** @brief buffer for frames to be transmitted after the beacon (PAN coordinator only) */
	Ieee802154eFrame* txBroadCmd;
	//@}

	/**
	 * @name Timer messages
	 */
	//@{
	/** @brief Timer for awaiting a Beacon
	 *
	 */

	/** @brief backoff timer for CSMA-CA */
	cMessage* backoffTimer;

	/** @brief timer for locating backoff boundary before sending a CCA request */
	cMessage* deferCCATimer;

	/** @brief timer for tracking beacons */
	cMessage* bcnRxTimer;

	/** @brief timer for transmitting beacon periodically */
	cMessage* bcnTxTimer;

	/** @brief timer for timer for ACK timeout */
	cMessage* ackTimeoutTimer;

	/** @brief timer for locating backoff boundary before txing ACK if beacon-enabled */
	cMessage* txAckBoundTimer;

	/** @brief timer for locating backoff boundary before txing Cmd or data if beacon-enabled */
	cMessage* txCmdDataBoundTimer;

	/** @brief timer for delay of IFS after receiving a data or cmd pkt */
	cMessage* ifsTimer;

	/** @brief timer for indicating being in the active period of outgoing (txing) superframe */
	cMessage* txSDTimer;

	/** @brief timer for indicating being in the active period of incoming (rxed) superframe */
	cMessage* rxSDTimer;

	/** @brief timer for indicating the end of CAP and the starting of CFP
	 used only by devices to put radio into sleep at the end of CAP if my GTS is not the first one in the CFP */
	cMessage* finalCAPTimer;

	/** @brief timer for scheduling of GTS, shared by both PAN coordinator and devices */
	cMessage* gtsTimer;

	/** @brief timer for scheduling of the ASN, for the TSCH Std 802.15.4e-2012 */
	cMessage* asnTimer;

	/** @brief timer for scheduling of the transmission, for the TSCH Std 802.15.4e-2012 */
	cMessage* tsCCAOffsetTimer;

	/** @brief timer for scheduling of the transmission, for the TSCH Std 802.15.4e-2012 */
	cMessage* tsTxOffsetTimer;

	/** @brief timer for scheduling of the transmission, for the TSCH Std 802.15.4e-2012 */
	cMessage* tsRxAckDelayTimer;

	/** @brief timer for scheduling of the transmission, for the TSCH Std 802.15.4e-2012 */
	cMessage* tsAckWaitTimer;

	/** @brief timer for scheduling of the receiving, for the TSCH Std 802.15.4e-2012 */
	cMessage* tsRxOffsetTimer;

	/** @brief timer for scheduling of the receiving, for the TSCH Std 802.15.4e-2012 */
	cMessage* tsRxWaitTimer;

	/** @brief timer for scheduling of the receiving, for the TSCH Std 802.15.4e-2012 */
	cMessage* tsTxAckDelayTimer;

	/** @brief timer for scheduling of the receiving, for the TSCH Std 802.15.4e-2012 */
	cMessage* tsMaxAckTimer;
	//@}

	/**
	 * @name Variables for timers
	 */
	//@{
	simtime_t lastTime_bcnRxTimer;
	bool txNow_bcnTxTimer;

	/** @brief true while in active period of the outgoing superframe  */
	bool inTxSD_txSDTimer;

	/** @brief true while in active period of the incoming superframe  */
	bool inRxSD_rxSDTimer;

	/** for PAN coordinator: index of GTS descriptor in the GTS list
	 * that GTS timer is currently scheduling for its starting;
	 * for devices: 99 indicating currently being in my GTS
	 */
	uint16_t index_gtsTimer;

	/** @brief the scheduled time for the wait timer */
	simtime_t tsWaitTimer;

	/** @brief [SR] for the TSCH synchronization (for time correction)*/
	double timeCorrection;

	/** @brief [SR] for the TSCH synchronization (for time correction) the time error of the last pkt*/
	UINT_16 timeError;

	/** @brief [SR] for the TSCH synchronization (timeslot start time)*/
	simtime_t startTimeslot;
	//@}

	/**
	 * @name Data transmission related variables
	 */
	//@{
	/** @brief true while a packet being transmitted at PHY */
	bool inTransmission;

	/** @brief true while a sent beacon or cmd frame in txBcnCmd is waiting for ACK */
	bool waitBcnCmdAck;

	/** @brief true while a sent beacon or cmd frame in txBcnCmdUpper is waiting for ACK */
	bool waitBcnCmdUpperAck;

	/** @brief true while a sent frame in txData is waiting for ACK */
	bool waitDataAck;

	/** @brief true while a sent frame in txGTS is waiting for ACK */
	bool waitGTSAck;

	/** @brief number of retries for txing a beacon or cmd frame in txBcnCmd */
	uint16_t numBcnCmdRetry;

	/** @brief number of retries for txing a beacon or cmd frame in txBcnCmdUpper */
	uint16_t numBcnCmdUpperRetry;

	/** @brief number of retries for txing a data frame in txData */
	uint16_t numDataRetry;

	/** @brief number of retries for txing a data frame in txGTS */
	uint16_t numGTSRetry;
	//@}

	/**
	 * @name Link objects for beacon and data pkts duplication detection
	 */
	//@{
	HLISTLINK* hlistBLink1;
	HLISTLINK* hlistBLink2;
	HLISTLINK* hlistDLink1;
	HLISTLINK* hlistDLink2;
	//@}

	/**
	 * @name Container used by coordinator to store info of associated devices
	 */
	typedef std::map<UINT_16,DevCapability> DeviceList;
	DeviceList deviceList;

	/**
	 * @name Statistical variables
	 */
	//@{
	/** @brief number of data pkts received from upper layer, counted in <handleUpperMsg()> */
	double numUpperPkt;

	/** @brief number of data pkts from upper layer dropped by MAC due to busy MAC or invalid size (e.g. oversize), counted in <handleUpperMsg()> */
	double numUpperPktLost;

	/** @brief number of collisions detected at PHY */
	double numCollision;

	/** @brief number of incoming beacons lost, counted in <handleBcnRxTimer()> */
	double numLostBcn;

	/** @brief number of transmitted beacons, counted in <taskSuccess('b')> */
	double numTxBcnPkt;

	/** @brief number of successfully transmitted data frames, counted in <taskSuccess('d')> */
	double numTxDataSucc;

	/** @brief number of data frames that MAC failed to transmit, counted in <taskFailed()> */
	double numTxDataFail;

	/** @brief number of successfully transmitted data frames in GTS, counted in <taskSuccess('g')> */
	double numTxGTSSucc;

	/** @brief number of data frames that MAC failed to transmit in GTS, counted in <taskFailed()> */
	double numTxGTSFail;

	/** @brief number of transmitted ack frames, counted in <taskSuccess()> */
	double numTxAckPkt;

	/** @brief number of received beacons, counted in <handleBeacon()> */
	double numRxBcnPkt;

	/** @brief number of received data frames, counted in <MCPS_DATA_indication()> */
	double numRxDataPkt;

	/** @brief number of received data frames in GTS, counted in <MCPS_DATA_indication()> */
	double numRxGTSPkt;

	/** @brief count only ACKs received before timeout, for both Cmd and Data  in <handleAck()> */
	double numRxAckPkt;

	/** @brief count all frames with two retries */
	double numRetryTwo;

	/** @brief count all frames with three retries */
	double numRetryThree;

	/** @brief count all frames with four retries */
	double numRetryFour;
	//@}

	// tmp variables for debug
	double numTxAckInactive;
};
#endif

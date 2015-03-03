/**
 * IEEE 802.15.4 Media Access Control (MAC) Layer
 * Refer to IEEE Std 802.15.4-2006, IEEE Std 802.15.4-2011 & IEEE Std 802.15.4e-2012
 *
 * @author Feng Chen
 * @ingroup macLayer
 *
 * @author: 2014    Stefan Reis      (modified)
 */

#include "Ieee802154eMac.h"
#include "InterfaceTableAccess.h"
#include "MACAddress.h"
#include "Ieee802Ctrl_m.h"
#include "Ieee802154ePhy.h"
#include "NotificationBoard.h"
#include "SlotframeLinkIE.h"

/**@author: 2014    Stefan Reis     (modified)
 * Default MAC PIB Attributes */
MAC_PIB Ieee802154eMac::MPIB =
{
MACAddress(0xffff)/*def_macExtendedAddress*/,
def_macAckWaitDuration,
def_macAssociatedPANCoord,
def_macAssociationPermit,
def_macAutoRequest,
def_macBattLifeExt,
def_macBattLifeExtPeriods,
def_macBeaconPayload,
def_macBeaconPayloadLength,
def_macBeaconOrder,
def_macBeaconTxTime, 0/*def_macBSN*/, MACAddress(def_macCoordExtendedAddress),
def_macCoordShortAddress, 0/*def_macDSN*/,
def_macGTSPermit,
def_macMaxBE,
def_macMaxCSMABackoffs,
def_macMaxFrameRetries,
def_macMinBE,
def_macLIFSPeriod,
def_macSIFSPeriod,
def_macPANId,
def_macPromiscuousMode,
def_macResponseWaitTime,
def_macRxOnWhenIdle,
def_macSecurityEnabled,
def_macEnhAckWaitDuration,
def_macImplicitBroadcast,
def_macSimpleAddress,
def_macShortAddress,
def_macSuperframeOrder, 0 /*def_macSyncSymbolOffset*/, 0 /*def_macTimestampSupported*/,
def_macTransactionPersistenceTime, 0 /*def_macTxControlActiveDuration*/, 0 /*def_macTxControlPauseDuration*/,
def_macTxTotalDuration,

def_macFrameCounter,
def_macAutoRequestSecurityLevel,
def_macAutoRequestKeyIdMode,
def_macAutoRequestKeyIndex, MACAddress::BROADCAST_ADDRESS64/*macPANCoordExtendedAddress*/,
def_macPANCoordShortAddress,
def_macFrameCounterMode,

def_macTSCHcapable,
def_macLLcapable,
def_macDSMEcapable,
def_macLEcapable,
def_macRFIDcapable,
def_macHoppingCapable,
def_macAMCACapable,
def_macMetricsCapable,
def_macTSCHenabled,
def_macLLenabled,
def_macDSMEenabled,
def_macLEenabled,
def_macRFIDenabled,
def_macHoppingEnabled,
def_macAMCAenabled,
def_macMetricsEnabled,

def_macDisconnectTime,
def_macJoinPriority,
def_macASN,
def_macNoHLBuffers,

0/*def_macCounterOctets*/,
def_macRetryCount,
def_macMultipleRetryCount,
def_macTXFailCount,
def_macTXSuccessCount,
def_macFCSErrorCount,
def_macSecurityFailure,
def_macDuplicateFrameCount,
def_macRXSuccessCount,
def_macNACKcount,

def_macUseEnhancedBeacon,
def_macEBFilteringEnabled, 0/*def_macEBSN*/,
def_macEBAutoSA
};

/**@author: 2014    Stefan Reis
 * for the debug output */
string Ieee802154eMac::frameTypeTxt[] =
{
"unknown type", "Ieee802154e_BEACON", "Ieee802154e_DATA", "Ieee802154e_ACK", "Ieee802154e_CMD", "Ieee802154e_LLDN", "Ieee802154e_MULTI", "Ieee802154e_Asscociation"
};

/**@author: 2014    Stefan Reis
 * for the debug output */
string Ieee802154eMac::stateTxt[] =
{
"phy_BUSY", "phy_BUSY_RX", "phy_BUSY_TX", "phy_FORCE_TRX_OFF", "phy_IDLE", "phy_INVALID_PARAMETER", "phy_RX_ON", "phy_SUCCESS", "phy_TRX_OFF", "phy_TX_ON", "phy_UNSUPPORT_ATTRIBUTE", "phy_READ_ONLY"
};

/**@author: 2014    Stefan Reis
 * for the debug output */
string Ieee802154eMac::paraTxt[] =
{
	"PHY_INVALID_PARAMETER",
	"PHY_CURRENT_CHANNEL",
	"PHY_CHANNELS_SUPPORTED",
	"PHY_TRANSMIT_POWER",
	"PHY_TX_POWER_TOLERANCE",
	"PHY_TX_POWER",
	"PHY_CCA_MODE",
	"PHY_CURRENT_PAGE",
	"PHY_MAX_FRAME_DURATION",
	"PHY_SHR_DURATION",
	"PHY_SYMBOLS_PER_OCTET",
	"PHY_PREAMBLE_SYMBOL_LENGTH",
	"PHY_UWB_DATA_RATES_SUPPORTED",
	"PHY_CSS_LOW_DATA_RATE_SUPPORTED",
	"PHY_UWB_COU_SUPPORTED",
	"PHY_UWB_CS_SUPPORTED",
	"PHY_UWB_LCP_SUPPORTED",
	"PHY_UWB_CURRENT_PULSE_SHAPE",
	"PHY_UWB_COU_PULSE",
	"PHY_UWB_CS_PULSE",
	"PHY_UWB_LCP_WEIGHT_1",
	"PHY_UWB_LCP_WEIGHT_2",
	"PHY_UWB_LCP_WEIGHT_3",
	"PHY_UWB_LCP_WEIGHT_4",
	"PHY_UWB_LCP_DELAY_2",
	"PHY_UWB_LCP_DELAY_3",
	"PHY_UWB_LCP_DELAY_4",
	"PHY_RANGING",
	"PHY_RANGING_CRYSTAL_OFFSET",
	"PHY_RANGING_DPS",
	"PHY_CURRENT_CODE",
	"PHY_NATIVE_PRF",
	"PHY_UWB_SCAN_BINS_PER_CHANNEL",
	"PHY_UWB_INSERTED_PREAMBLE_INTERVAL",
	"PHY_TX_RMARKER_OFFSET",
	"PHY_RX_RMARKER_OFFSET",
	"PHY_RFRAME_PROCESSING_TIME",
	"PHY_CCA_DURATION"
};

UINT_16 Ieee802154eMac::addrCount = 0;

simsignal_t Ieee802154eMac::sucTimeslot = SIMSIGNAL_NULL;

Define_Module(Ieee802154eMac);

/**@author: 2014    Stefan Reis     (modified)
 * Constructor */
Ieee802154eMac::Ieee802154eMac()
{
    // buffer
    txPkt = NULL;
    txBeacon = NULL;
    txBcnCmdUpper = NULL;
    txBcnCmd = NULL;
    txData = NULL;
    txGTS = NULL;
    txAck = NULL;
    txCsmaca = NULL;
    tmpCsmaca = NULL;
    rxBeacon = NULL;
    rxData = NULL;
    rxCmd = NULL;

    // timer
    backoffTimer = NULL;
    deferCCATimer = NULL;
    bcnRxTimer = NULL;
    bcnTxTimer = NULL;
    ackTimeoutTimer = NULL;
    txAckBoundTimer = NULL;
    txCmdDataBoundTimer = NULL;
    ifsTimer = NULL;
    txSDTimer = NULL;
    rxSDTimer = NULL;
    finalCAPTimer = NULL;
    gtsTimer = NULL;

    asnTimer = NULL;
    tsCCAOffsetTimer = NULL;
    tsTxOffsetTimer = NULL;
    tsRxAckDelayTimer = NULL;
    tsAckWaitTimer = NULL;
    tsRxOffsetTimer = NULL;
    tsRxWaitTimer = NULL;
    tsTxAckDelayTimer = NULL;

    // link
    hlistBLink1 = NULL;
    hlistBLink2 = NULL;
    hlistDLink1 = NULL;
    hlistDLink2 = NULL;
}

/**@author: 2014    Stefan Reis     (modified)
 * Destructor */
Ieee802154eMac::~Ieee802154eMac()
{
    cancelAndDelete(backoffTimer);
    cancelAndDelete(deferCCATimer);
    cancelAndDelete(bcnRxTimer);
    cancelAndDelete(bcnTxTimer);
    cancelAndDelete(ackTimeoutTimer);
    cancelAndDelete(txAckBoundTimer);
    cancelAndDelete(txCmdDataBoundTimer);
    cancelAndDelete(ifsTimer);
    cancelAndDelete(txSDTimer);
    cancelAndDelete(rxSDTimer);
    cancelAndDelete(finalCAPTimer);
    cancelAndDelete(gtsTimer);

    // TSCH timer
    cancelAndDelete(asnTimer);
    cancelAndDelete(tsCCAOffsetTimer);
    cancelAndDelete(tsTxOffsetTimer);
    cancelAndDelete(tsRxAckDelayTimer);
    cancelAndDelete(tsAckWaitTimer);
    cancelAndDelete(tsRxOffsetTimer);
    cancelAndDelete(tsRxWaitTimer);
    cancelAndDelete(tsTxAckDelayTimer);
    cancelAndDelete(scanTimer);

    emptyHListLink(&hlistBLink1, &hlistBLink2);
    emptyHListLink(&hlistDLink1, &hlistDLink2);
}

InterfaceEntry * Ieee802154eMac::createInterfaceEntry()
{
    InterfaceEntry *e = new InterfaceEntry(this);

    // interface name: NetworkInterface module's name without special characters ([])
    char *interfaceName = new char[strlen(getParentModule()->getFullName()) + 1];
    char *d = interfaceName;

    for(const char *s = getParentModule()->getFullName(); *s; s++)
	if(isalnum(*s))
	    *d++ = *s;
    *d = '\0';

    e->setName(interfaceName);
    delete[] interfaceName;

    // address
    e->setMACAddress(macaddress);
    e->setInterfaceToken(macaddress.formInterfaceIdentifier());
    e->setMtu(Ieee802154ePhy::aMaxMACFrameSize);

    // capabilities
    e->setBroadcast(true);
    e->setMulticast(true);
    e->setPointToPoint(false);
    iface = e;

    // add
    return e;
}

MACAddress Ieee802154eMac::configurationMacAddress()
{
    MACAddress address;
    const char *addressString = par("address");
    if(!strcmp(addressString, "auto"))
    {
	// assign automatic address
	address = MACAddress::generateAutoAddress();
	address.convert64();
	// change module parameter from "auto" to concrete address
	par("address").setStringValue(address.str().c_str());
    }
    else
	address.setAddress(addressString);

    return address;
}

/**@author: 2014    Stefan Reis     (modified)
 * set and configure the pointer to the other modules */
void Ieee802154eMac::commonInitialize()
{
    macaddress = configurationMacAddress();

    if(!macaddress.getFlagEui64())
	opp_error("802154 address error, address is not EUI64");

    aExtendedAddress = macaddress;

    // InterfaceTable
    iface = NULL;
    registerInterface();

    // get gate I
    mUpperLayerIn = findGate("upperLayerIn");
    mUpperLayerOut = findGate("upperLayerOut");
    mLowerLayerIn = findGate("lowerLayerIn");
    mLowerLayerOut = findGate("lowerLayerOut");
    mQueueIn = findGate("queueIn");
    mQueueOut = findGate("queueOut");
    mSchedulerIn = findGate("schedulerIn");
    mSchedulerOut = findGate("schedulerOut");

    // get a pointer to the SlotframeTable Module
    slotframeTable = check_and_cast<IMacSlotframeTable *>(getModuleByPath(par("macSlotframeTableModule")));
    // get a pointer to the LinkTable Module
    linkTable = check_and_cast<IMacLinkTable *>(getModuleByPath(par("macLinkTableModule")));
    // get a pointer to the TimeslotTable Module
    timeslotTable = check_and_cast<IMacTimeslotTable *>(getModuleByPath(par("macTimeslotTableModule")));
    // get a pointer to the HoppingSequenceList Module
    hoppingSequenceList = check_and_cast<IMacHoppingSequenceList *>(getModuleByPath(par("macHoppingSequenceListModule")));

    // get a pointer to the NeighborTable Module
    neighborTable = check_and_cast<IMacNeighborTable *>(getModuleByPath(par("macNeighborTableModule")));

    // get a pointer to the NotificationBoard module
    mpNb = NotificationBoardAccess().get();
    // subscribe for the information of the carrier sens
    mpNb->subscribe(this, NF_RADIOSTATE_CHANGED);
    //mpNb->subscribe(this, NF_BITRATE_CHANGED)
    mpNb->subscribe(this, NF_RADIO_CHANNEL_CHANGED);
    mpNb->subscribe(this, NF_PHY_PKT_DISCARDED); // for the TSCH handle, if a pkt is dropped at the phy layer
    radioState = RadioState::IDLE;

    // obtain pointer to external queue
    initializeQueueModule();
}

/**@author: 2014    Stefan Reis     (modified)
 * Initialization of the Ieee802154Mac Module*/
void Ieee802154eMac::initialize(int stage)
{
    EV << getParentModule()->getParentModule()->getFullName() << ": initializing Ieee802154eMac, stage=" << stage << endl;
    WirelessMacBase::initialize(stage);

    if(0 == stage)
    {
	commonInitialize();

	// initialize MAC PIB attributes
	MPIBcopyStandard();
	mpib.macExtendedAddress = aExtendedAddress;
	mpib.macBSN = intrand(256);     // Beacon Sequence Number
	mpib.macDSN = intrand(256);     // Data Sequence Number
	mpib.macEBSN = intrand(256);     // Enhanced Beacon Sequence Number

	// User-specified MAC parameters
	m_debug = par("debug");
	isPANCoor = par("isPANCoor");
	coordRealignment = false;
	mpib.macBeaconOrder = par("BO");
	mpib.macSuperframeOrder = par("SO");
	dataTransMode = par("dataTransMode");
	panCoorName = par("panCoorName");
//        isRecvGTS               = par("isRecvGTS");
//        gtsPayload              = par("gtsPayload");
//        ack4Gts                 = par("ack4Gts");
	ack4Data = par("ack4Data");
	mpib.macRxOnWhenIdle = par("setRxOnWhenIdle");

	trx_state_req_activ = false;
	trx_state_req = phy_INVALID_PARAMETER;

	// to use the IPv4 or IPv6 network layer and not a simple network layer
	useIeee802Ctrl = par("useIeee802Ctrl");

	// for test [SR]
	mpib.macMaxFrameRetries = par("maxRetries");

	// set TSCHCapable
	mpib.macTSCHcapable = par("TSCHcapable").boolValue();
	// for the IEEE Std 802.15.4e-2012 TSCH mode
	useTSCH = par("useTSCH");
	timeSource = false;    // It will be only set in the TSCH mode
	useCCA = par("useCCA");     // It will be only set in the TSCH mode
	txBeaconNow = false;
	if(useTSCH)
	{
	    mpib.macTSCHcapable = true;
	    /* Note: It will be set true for a PAN coordinator or if the first enhanced beacon with ASN arrived.
	     * useTSCHdirect - activate the TSCh mode without a Beacon from the PAN coordinator (note: the device will be not associated with the coordinator.) */
	    mpib.macTSCHenabled = isPANCoor ? true:par("useTSCHdirect");
	    mpib.macTimestampSupported = true; // see Std 802.15.4e-2012 (5.1.4.2a.2 Node synchronization) page 29
	    mpib.macUseEnhancedBeacon = true;  // Use Enhanced Beacon
	    timeSource = par("timeSource");
	    useCCA = par("useCCA");
	    useTimeslotID = par("useTimeslotID"); //default timeslot template=0 - Std 802.15.4e-2012 (Table 52e�TSCH-MAC PIB attributes for macTimeslotTemplate) page 176
	    useHoppingSequenceID = par("useHoppingSequenceID"); // default = 0; all channels are supported
	}
	timeError = 0x0000;
	timeCorrection = 0.0;
	startTimeslot = 0.0;

	// activate the counter of the MAC Metrics
	// - Std 802.15.4e-2012 (6.4.3.9 MAC-performance-metrics-specific MAC PIB attributes) page 182
	if(par("useMetrics"))
	{
	    mpib.macMetricsCapable = true;
	    mpib.macMetricsEnabled = true;
	}

	if(mpib.macBeaconOrder > 15 || mpib.macSuperframeOrder > mpib.macBeaconOrder)
	    error("[MAC]: wrong values for MAC parameter BO or SO");
	else if(mpib.macBeaconOrder == 15)
	    error("[MAC]: non-beacon mode (BO = 15) has not been tested!");
	else if(mpib.macSuperframeOrder == mpib.macBeaconOrder)
	    error("[MAC]: warning!! the case BO == SO has not been tested!");

	if(dataTransMode != 1 && dataTransMode != 2 && dataTransMode != 3) // Std IEEE 802.15.4-2011 - 4.5.2 Data transfer model; page 13
	    error("[MAC]: data transfer mode = %d is not supported in the model!", dataTransMode);

	// Initialization parameters for backoff method
	std::string backoffMethodStr = par("backoffMethod").stdstringValue();

	// for TSCH - The device shall use an exponential backoff mechanism - Std 802.15.4e-2012 (5.1.1.4.3 TSCH CSMA-CA algorithm) page 14
	if(mpib.macTSCHenabled)
	{
	    backoffMethod = EXPONENTIAL;
	    // TSCH-specific MAC PIB attributes - Std 802.15.4e-2012 (table 52b) page 174
	    mpib.macMinBE = par("macMinBE");    // default: for CSMA-CA = 3; for TSCH-CA = 1
	    mpib.macMaxBE = par("macMaxBE");    // default: for CSMA-CA = 5; for TSCH-CA = 7
	}
	else if(backoffMethodStr == "exponential")
	{
	    backoffMethod = EXPONENTIAL;
	    mpib.macMinBE = par("macMinBE");
	    mpib.macMaxBE = par("macMaxBE");
	}
	else
	{
	    if(backoffMethodStr == "linear")
		backoffMethod = LINEAR;
	    else if(backoffMethodStr == "constant")
		backoffMethod = CONSTANT;
	    else
	    {
		error("Unknown backoff method \"%s\".\
                       Use \"constant\", \"linear\" or \"\
                       \"exponential\".", backoffMethodStr.c_str());
	    }
	    initialCW = par("contentionWindow"); // need to be check [SR]
	}
	NB = 0;
	BE = 0;

	panStartTime = 5.0;

	secuData = false;    // security not implemented
	secuBeacon = false;    // security not implemented

	taskP.init();
	taskP.TS_RX_CCA_tschcca = false;
	taskP.TS_TX_CCA_tschcca = false;
	taskP.TS_RX_CCA_tschcca_STEP = 0;
	taskP.TS_TX_CCA_tschcca_STEP = 0;

	// for beacon
	rxBO = 15;       // set through the beacon
	rxSO = 15;       // set through the beacon
	beaconWaitingTx = false;
	notAssociated = true;
	bcnLossCounter = 0;

	// for transmission
	inTransmission = false;
	waitBcnCmdAck = false;
	waitBcnCmdUpperAck = false;
	waitDataAck = false;
	waitGTSAck = false;
	numBcnCmdRetry = 0;
	numBcnCmdUpperRetry = 0;
	numDataRetry = 0;
	numGTSRetry = 0;

	// for CSMA-CA
	csmacaWaitNextBeacon = false;
	backoffStatus = 0;

	// for timer
	lastTime_bcnRxTimer = 0;
	inTxSD_txSDTimer = false;
	inRxSD_rxSDTimer = false;
	index_gtsTimer = 0;

	// device capability
	capability.alterPANCoor = false;
	capability.FFD = true;
	capability.powerSource = false;
	capability.recvOnWhenIdle = mpib.macRxOnWhenIdle; // default: true
	capability.secuCapable = false;
	capability.alloShortAddr = true;
	capability.hostName = getParentModule()->getParentModule()->getFullName();

	// GTS variables for PAN coordinator
	gtsCount = 0;
	for(int i = 0; i < 7; i++)
	{
	    gtsList[i].devShortAddr = def_macShortAddress;  // 0xffff
	    gtsList[i].startSlot = 0;
	    gtsList[i].length = 0;
	    gtsList[i].isRecvGTS = false;
	    gtsList[i].isTxPending = false;
	}
	tmp_finalCap = aNumSuperframeSlots - 1; // 15 if no GTS
	indexCurrGts = 99;
	isTxPending = false;

	// GTS variables for devices
	gtsLength = 0;
	gtsStartSlot = 0;
	gtsTransDuration = 0;

	/* for indirect transmission
	 txPaFields.numShortAddr = 0;
	 txPaFields.numExtendedAddr  = 0;
	 rxPaFields.numShortAddr = 0;
	 rxPaFields.numExtendedAddr = 0;*/

	// packet counter //note: the metrics counter are initialized with the default values (= 0)
	numUpperPkt = 0;
	numUpperPktLost = 0;
	numCollision = 0;
	numLostBcn = 0;
	numTxBcnPkt = 0;
	numTxDataSucc = 0;
	numTxDataFail = 0;
	numTxGTSSucc = 0;
	numTxGTSFail = 0;
	numTxAckPkt = 0;
	numRxBcnPkt = 0;
	numRxDataPkt = 0;
	numRxGTSPkt = 0;
	numRxAckPkt = 0;

	numTxAckInactive = 0;

	numRetryTwo = 0;
	numRetryThree = 0;
	numRetryFour = 0;

	WATCH(taskP.TS_RX_CCA_tschcca);
	WATCH(taskP.TS_TX_CCA_tschcca);
	WATCH(taskP.TS_RX_CCA_tschcca_STEP);
	WATCH(taskP.TS_TX_CCA_tschcca_STEP);

	// [to watch the value in the Tkenv GUI]
	WATCH(inTxSD_txSDTimer);
	WATCH(inRxSD_rxSDTimer);
	WATCH(numUpperPkt);
	WATCH(numUpperPktLost);
	WATCH(numCollision);
	WATCH(numLostBcn);
	WATCH(numTxBcnPkt);
	WATCH(numTxDataSucc);
	WATCH(numTxDataFail);
	WATCH(numTxGTSSucc);
	WATCH(numTxGTSFail);
	WATCH(numTxAckPkt);
	WATCH(numRxBcnPkt);
	WATCH(numRxDataPkt);
	WATCH(numRxGTSPkt);
	WATCH(numRxAckPkt);

	WATCH(numTxAckInactive);

	WATCH(numRetryTwo);
	WATCH(numRetryThree);
	WATCH(numRetryFour);
	WATCH(curASN);
	WATCH(mpib.macASN);
	// Std 802.15.4e-2012 - Metrics counter
	if(mpib.macMetricsEnabled)
	{
	    WATCH(mpib.macRetryCount);
	    WATCH(mpib.macMultipleRetryCount);
	    WATCH(mpib.macTXFailCount);
	    WATCH(mpib.macTXSuccessCount);
	    WATCH(mpib.macFCSErrorCount);

	    if(mpib.macSecurityEnabled)
		WATCH(mpib.macSecurityFailure);

	    WATCH(mpib.macDuplicateFrameCount);
	    WATCH(mpib.macRXSuccessCount);
	    WATCH(mpib.macNACKcount);
	}

	// to record the timeslot where the retransmission is successful
	sucTimeslot = registerSignal("timeSlot");

	radioModule = gate("lowerLayerOut")->getNextGate()->getOwnerModule()->getId();
    }
    else if(1 == stage)
    {
	// initialize timers
	backoffTimer = new cMessage("backoffTimer", MAC_BACKOFF_TIMER);
	deferCCATimer = new cMessage("deferCCATimer", MAC_DEFER_CCA_TIMER);
	bcnRxTimer = new cMessage("bcnRxTimer", MAC_BCN_RX_TIMER);
	bcnTxTimer = new cMessage("bcnTxTimer", MAC_BCN_TX_TIMER);
	ackTimeoutTimer = new cMessage("ackTimeoutTimer", MAC_ACK_TIMEOUT_TIMER);
	txAckBoundTimer = new cMessage("txAckBoundTimer", MAC_TX_ACK_BOUND_TIMER);
	txCmdDataBoundTimer = new cMessage("txCmdDataBoundTimer", MAC_TX_CMD_DATA_BOUND_TIMER);
	ifsTimer = new cMessage("ifsTimer", MAC_IFS_TIMER);
	txSDTimer = new cMessage("txSDTimer", MAC_TX_SD_TIMER);
	rxSDTimer = new cMessage("rxSDTimer", MAC_RX_SD_TIMER);
	finalCAPTimer = new cMessage("finalCAPTimer", MAC_FINAL_CAP_TIMER);
	gtsTimer = new cMessage("gtsTimer", MAC_GTS_TIMER);
	// for TSCH Std 802.15.4e-2012
	asnTimer = new cMessage("asnTimer", MAC_ASN_TIMER);
	// TSCH transmission
	tsCCAOffsetTimer = new cMessage("tsCCAOffsetTimer", MAC_TS_CCA_OFFSET_TIMER);
	tsTxOffsetTimer = new cMessage("tsTxOffsetTimer", MAC_TS_TX_OFFSET_TIMER);
	tsRxAckDelayTimer = new cMessage("tsRxAckDelayTimer", MAC_TS_RX_ACK_DELAY_TIMER);
	tsAckWaitTimer = new cMessage("tsAckWaitTimer", MAC_TS_ACK_WAIT_TIMER);
	// TSCH receiving
	tsRxOffsetTimer = new cMessage("tsRxOffsetTimer", MAC_TS_RX_OFFSET_TIMER);
	tsRxWaitTimer = new cMessage("tsRxWaitTimer", MAC_TS_RX_WAIT_TIMER);
	tsTxAckDelayTimer = new cMessage("tsTxAckDelayTimer", MAC_TS_TX_ACK_DELAY_TIMER);
	// get initial radio state, channel number, transmit power etc. from Phy layer in this stage
	scanTimer = new cMessage("scanTimer", MAC_SCAN_TIMER);
    }
    else if(3 == stage)
    {
	if(iface && (iface->getMTU() != Ieee802154ePhy::aMaxMACFrameSize))
	    iface->setMtu(Ieee802154ePhy::aMaxMACFrameSize);

	EV << "MAC extended address is: " << mpib.macExtendedAddress << endl;
	EV << "mpib.macBSN initialized with: " << (int)mpib.macBSN << endl;
	EV << "mpib.macDSN initialized with: " << (int)mpib.macDSN << endl;
	EV << "mpib.macEBSN initialized with: " << (int)mpib.macEBSN << endl;
	EV << "mpib.macMaxFrameRetries initialized with: " << (int)mpib.macMaxFrameRetries << endl;

	if(mpib.macTSCHenabled)
	    EV << "The node is running in the IEEE Std 802.15.4e-2012 TSCH mode" << endl;
	else
	    EV << "The node is running in the IEEE Std 802.15.4-2006/2011 mode" << endl;

	// start a PAN Coordinator or a Device
	if(isPANCoor)
	{   // Start PAN Coordinator - Std 802.15.4-2006 (7.5.2.3.1) page 177
	    // check name
	    panCoorName = getParentModule()->getParentModule()->getFullName();
//	    if(strcmp(panCoorName, getParentModule()->getParentModule()->getFullName()) != 0)
//		error("[MAC]: name of PAN coordinator does not match!");

	    // change icon displayed in Tkenv
	    cDisplayString* display_string = &getParentModule()->getParentModule()->getDisplayString();
	    display_string->setTagArg("i", 0, "misc/house");

	    EV << "************************ PAN Coordinator Parameters ************************" << endl;
	    EV << getParentModule()->getParentModule()->getFullName() << " is the PAN coordinator!" << endl;
	    EV << "Channel Number: " << ppib.phyCurrentChannel << "; bit rate: " << phy_bitrate / 1000 << " kb/s; symbol rate: " << phy_symbolrate / 1000 << " ksymbol/s" << endl;

	    if(mpib.macTSCHenabled)
	    {
		EV << "macMinBE = " << (int)mpib.macMinBE << ", macMaxBE " << (int)mpib.macMaxBE << endl;
	    }
	    else
	    {
		EV << "BO = " << (int)mpib.macBeaconOrder << ", BI = " << (int)aBaseSuperframeDuration * (1 << mpib.macBeaconOrder) / phy_symbolrate << " s; SO = " << (int)mpib.macSuperframeOrder << ", SD = " << (int)aBaseSuperframeDuration * (1 << mpib.macSuperframeOrder) / phy_symbolrate << " s; duty cycle = " << pow(2.0, (mpib.macSuperframeOrder - mpib.macBeaconOrder)) * 100 << "%" << endl;
		EV << "There are " << (int)aBaseSuperframeDuration * (1 << mpib.macSuperframeOrder) << " symbols (" << (int)aBaseSuperframeDuration * (1 << mpib.macSuperframeOrder) / aUnitBackoffPeriod << " backoff periods) in CAP" << endl;
		EV << "Length of a unit of backoff period: " << bPeriod << " s" << endl;
	    }
	    EV << "************************************************************************" << endl;

	    // init address
	    mpib.macShortAddress = getShortAddress(mpib.macExtendedAddress);
	    mpib.macSimpleAddress = (UINT_8)mpib.macShortAddress;
	    mpib.macPANId = mpib.macShortAddress; // Simply use mac extended address
	    mpib.macAssociationPermit = true;

	    //Check if TimeslotTemplate is empty

	    if(mpib.macTSCHenabled)
	    { // Std 802.15.4e-2012
//                PLME_SET_TRX_STATE_request(phy_TRX_OFF);
//                // transmit one enhanced beacon, that all the devices have the time and the ASN
//                constructBCN(); // construct beacon, save it in the frame queue
//                // start ASN
//                BE = mpib.macMinBE; // for shared links (TSCH CSMA-CA)
//                scheduleAt(simTime()+ panStartTime, asnTimer);
	    }
	    else
	    { // Std 802.15.4-2011
		PLME_SET_TRX_STATE_request(phy_TX_ON);

		cMessage* startPANCoorTimer = new cMessage("startPANCoorTimer", START_PAN_COOR_TIMER);
		// start the transmission of beacon
		// timer will be handled after initialization is done, even when panStartTime = 0.
		scheduleAt(simTime() + panStartTime, startPANCoorTimer);
	    }
	}
	else    // start a device
	{
	    //startDevice();

//            if(mpib.macTSCHenabled)
//            { // start the TSCH without a beacon from the PAN coordinator (note: the device will be not associated with the coordinator.)
//                BE = mpib.macMinBE; // for shared links (TSCH CSMA-CA)
//                scheduleAt(panStartTime, asnTimer);
//            }

	}
    }
}

void Ieee802154eMac::initializeQueueModule()
{
    // use of external queue module is optional -- find it if there's one specified
    if(par("queueModule").stringValue()[0])
    {
	cModule *module = getParentModule()->getSubmodule(par("queueModule").stringValue());
	//queueModule = check_and_cast<IPassiveQueue *>(module); //[SR] old version
	queueModule = check_and_cast<Ieee802154eQueue *>(module);

	EV << "[IFQUEUE]: requesting first one frame from queue module\n";
	//queueModule->requestPacket(); //[SR] old version
    }
}

void Ieee802154eMac::flushQueue()
{
    if(queueModule)
    {
	while(!queueModule->isEmpty())
	{
	    cMessage *msg = queueModule->pop();

	    //emit(dropPkIfaceDownSignal, msg); -- 'pkDropped' signals are missing in this module!

	    delete msg;
	}
	queueModule->clear(); // clear request count
    }
}

void Ieee802154eMac::clearQueue()
{
    if(queueModule)
    {
	queueModule->clear(); // clear request count
    }
}

/**@author: 2014    Stefan Reis     (modified)
 * Store the captured values in the scalar file */
void Ieee802154eMac::finish()
{
    double t = SIMTIME_DBL(simTime());
    if(t == 0)
	return;

    recordScalar("Total simulation time", t);
    recordScalar("total num of upper pkts received", numUpperPkt);
    recordScalar("num of upper pkts dropped", numUpperPktLost);
    recordScalar("num of BEACON pkts sent", numTxBcnPkt);
    recordScalar("num of DATA pkts sent successfully", numTxDataSucc);
    recordScalar("num of DATA pkts failed", numTxDataFail);
    recordScalar("num of DATA pkts sent successfully in GTS", numTxGTSSucc);
    recordScalar("num of DATA pkts failed in GTS", numTxGTSFail);
    recordScalar("num of ACK pkts sent", numTxAckPkt);
    recordScalar("num of BEACON pkts received", numRxBcnPkt);
    recordScalar("num of BEACON pkts lost", numLostBcn);
    recordScalar("num of DATA pkts received", numRxDataPkt);
    recordScalar("num of DATA pkts received in GTS", numRxGTSPkt);
    recordScalar("num of ACK pkts received", numRxAckPkt);
    recordScalar("num of collisions", numCollision);

    // MAC-performance-metrics-specific / Std 802.15.4e-2012 (6.4.3.9 MAC-performance-metrics-specific MAC PIB attributes) page 182
    if(mpib.macMetricsEnabled)
    {
	recordScalar("num of frames sent successfully one retry", mpib.macRetryCount);
	recordScalar("num of frames sent successfully multi retry", mpib.macMultipleRetryCount);
	recordScalar("num of frames sent failed", mpib.macTXFailCount);
	recordScalar("num of frames sent successfully within macAckWaitDuration", mpib.macTXSuccessCount);
	recordScalar("num of frames with FCS error", mpib.macFCSErrorCount);

	if(mpib.macSecurityEnabled)
	    recordScalar("num of frames with security error", mpib.macSecurityFailure);

	recordScalar("num of duplicated frames received", mpib.macDuplicateFrameCount);
	recordScalar("num of frames received successfully", mpib.macRXSuccessCount);
	recordScalar("num of ACK/NACK timing correction IE with NACK", mpib.macNACKcount);
    }

    recordScalar("num of frames sent successfully two retry", numRetryTwo);
    recordScalar("num of frames sent successfully three retry", numRetryThree);
    recordScalar("num of frames sent successfully four retry", numRetryFour);
}

/**@author: 2014    Stefan Reis     (modified) */
void Ieee802154eMac::receiveChangeNotification(int category, const cPolymorphic *details)
{
    Enter_Method_Silent
    ();
    printNotificationBanner(category, details);

    switch(category)
    {
	if(check_and_cast<RadioState *>(details)->getRadioId() != getRadioModuleId())
	    return;

    case NF_RADIO_CHANNEL_CHANGED:
	ppib.phyCurrentChannel = check_and_cast<RadioState *>(details)->getChannelNumber();
	bitrate = getRate('b');
	phy_bitrate = bitrate;
	phy_symbolrate = getRate('s');
	bPeriod = aUnitBackoffPeriod / phy_symbolrate;
	break;
    case NF_RADIOSTATE_CHANGED:
	radioState = check_and_cast<RadioState *>(details)->getState();

	// for TSCH CCA -  cancel the TsWaitTimer
	if(radioState == RadioState::RECV && mpib.macTSCHenabled)
	{
	    if(taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 9)
	    {
		taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 10;
		EV << "[TSCH CCA]-Transmitter:[10] start to receive a frame before TsAckWaitTimer timeout, cancel timeout timer" << endl;

		if(tsAckWaitTimer->isScheduled())
		    cancelEvent(tsAckWaitTimer);
		/* two possible next steps:
		 * 1. receiveChangeNotification() (failed of the receiving of the ACK pkt)
		 * 2. handleLowerMsg() (successful receive a pkt) */
	    }
	    else if(taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) == 4)
	    {
		taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) = 5;
		EV << "[TSCH CCA]-Receiver:[5] start to receive a frame before TsRxWaitTimer timeout, cancel timeout timer" << endl;

		if(tsRxWaitTimer->isScheduled())
		    cancelEvent(tsRxWaitTimer);
		/* two possible next steps:
		 * 1. receiveChangeNotification() (failed of the receiving of the ACK pkt)
		 * 2. handleLowerMsg() (successful receive a pkt) */
	    }
	}
	break;
    case NF_PHY_PKT_DISCARDED:     // a frame is discarded at the PHY layer
	if(mpib.macTSCHenabled)
	{ // we need this, to detect that the PHY layer finished the receiving of a frame
	    if(taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 10)
	    {
		taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 11;
		EV << "[TSCH CCA]-Transmitter:[11] frame was dropped at the PHY layer" << endl;
		handleTsAckWait();
		// next step in handleTsAckWait()
	    }
	    else if(taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) == 5)
	    {
		taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) = 6;
		EV << "[TSCH CCA]-Receiver:[6] frame was dropped at the PHY layer" << endl;
		handleTsRxWait();
		// next step in handleTsRxWait()
	    }
	}
	break;

    default:
	break;
    }
}

//------------------------------------------------------------------------------/
/*************************** <Start star topology> *****************************/
//------------------------------------------------------------------------------/
/**@author: 2014    Stefan Reis     (modified) */
void Ieee802154eMac::startPANCoor()
{
    // for Std 802.15.4e-2012 TSCH mode
    if(!mpib.macTSCHenabled)    // for Std 802.15.4-2011
    {
	txSfSlotDuration = aBaseSlotDuration * (1 << mpib.macSuperframeOrder);
	startBcnTxTimer(true);  // start to transmit my first beacon immediately
    }
}

/**@author: 2014    Stefan Reis     (modified) */
void Ieee802154eMac::startDevice()
{
    /** brief: not associated device - only PAN coordinator transmits beacon
     *  note: for the value of the address fields - see Std 802.15.4-2011 (table 87) page 181 */
//    mpib.macPANId = 0xffff; // default value (broadcast PAN ID) - it is set after receiving of first beacon
//    mpib.macShortAddress = 0xfffe; // not associated, but initialized - it is set after receiving of first beacon
//    mpib.macSimpleAddress = 0xfe; // not associated, but initialized - it is set after receiving of first beacon
//    mpib.macBeaconOrder = 15;
//    mpib.macSuperframeOrder = 15;
//    mpib.macAssociationPermit = false;
    // open radio receiver, waiting for first beacon's arrival
    if(!mpib.macTSCHenabled)
	PLME_SET_TRX_STATE_request(phy_RX_ON);
    else if(mpib.macTSCHenabled && !isPANCoor)
    {
	PLME_SET_TRX_STATE_request(phy_RX_ON);
    }
    else
    {
	PLME_SET_TRX_STATE_request(phy_TRX_OFF);
	//XXX: [SR] workaround because the device are not associated from the beginning
//	mpib.macShortAddress = (UINT_16)aExtendedAddress.getInt();
//	mpib.macSimpleAddress = (UINT_8)aExtendedAddress.getInt();
    }
}

//-------------------------------------------------------------------------------/
/*************************** <General Msg Handler> ******************************/
//-------------------------------------------------------------------------------/
/**@author: 2014    Stefan Reis     (modified) */
void Ieee802154eMac::handleMessage(cMessage* msg)
{
    if(!isOperational)
    {
	handleMessageWhenDown(msg);
	return;
    }

    if(msg->getArrivalGateId() == mLowerLayerIn && dynamic_cast<cPacket*>(msg) == NULL)
    {
	if(msg->getKind() == 0)
	    error("[MAC]: message '%s' with length == 0 is supposed to be a primitive, but msg kind is also zero", msg->getName());

	handleMacPhyPrimitive(msg->getKind(), msg); // from PHY layer - confirm messages
	return;
    }

    if(msg->getArrivalGateId() == mUpperLayerIn && dynamic_cast<cPacket*>(msg) == NULL)
    {
	if(msg->getKind() == 0)
	    error("[MAC]: message '%s' with length == 0 is supposed to be a primitive, but msg kind is also zero", msg->getName());

	handlePrimitive(msg->getKind(), msg); // from NET layer - command messages
	//XXX: note: not in use, not for the StarNet or the CSMA802154 example, from the NET layer comes only data packets
	return;
    }else if(msg->getArrivalGateId() == mSchedulerIn)
    {
	handleSchedulerMsg(msg);
    }

    else if(msg->getArrivalGateId() == mLowerLayerIn)
    {
	handleLowerMsg(PK(msg));    // from PHY layer - data messages
    }
    else if(msg->isSelfMessage())
    {
	handleSelfMsg(msg);         // for timer
    }
    else if(msg->getArrivalGateId() == mLowerLayerIn)
    {
	EV << "QUEUE: do something if a packet comes from the Queue" << endl;
    }
    else
    {
	handleUpperMsg(PK(msg)); // from network layer - data and command messages
    }
}

/**@author: 2014    Stefan Reis     (modified)
 * (the structure is copied from the PHY layer) */
void Ieee802154eMac::handlePrimitive(int msgkind, cMessage *msg) // for MLME-SAP (Command messages) - XXX: not in use
{
    Ieee802154eNetworkCtrlInfo *primitive = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    switch(msgkind)
    {
	case TP_MCPS_DATA_REQUEST:
	    EV << "[MAC]: this is a MCPS_DATA_REQUEST" << endl;
//        handle_MCPS_DATA_request(primitive->getPsduLength(), encapsulatePacket(msg));
	    break;

	case TP_MCPS_PURGE_REQUEST:
	    EV << "[MAC]: this is a MCPS_PURGE_REQUEST" << endl;
//        handle_MCPS_PURGE_request(UINT_8 channelNumber, UINT_8 channelPage, Ieee802154eAddrMode coordAddrMode,
//                UINT_16 coordPANId, IE3ADDR coordAddress, UINT_8 capabilityInformation,
//                UINT_8 securityLevel, UINT_8 keyIdMode, UINT_64 keySource, UINT_8 keyIndex,
//                UINT_64 lowLatencyNetworkInfo, UINT_16 channelOffset, UINT_8 hoppingSequenceID);
	    delete primitive;
	    break;

	case TP_MLME_GET_REQUEST:
	    EV << "[MAC]: this is a MLME_GET_REQUEST" << endl;
	    handle_MLME_GET_request((MACPIBenum)primitive->getPibAttribute());
	    delete primitive;
	    break;

	case TP_MLME_GTS_REQUEST:
	    EV << "[MAC]: this is a MLME_GTS_REQUEST" << endl;
	    handle_MLME_GTS_request(primitive->getGtsCharacteristic(), primitive->getSecurityLevel(), primitive->getKeyIdMode(), primitive->getKeySource(), primitive->getKeyIndex());
	    delete primitive;
	    break;

	case TP_MLME_RESET_REQUEST:
	    EV << "[MAC]: this is a MLME_RESET_REQUEST" << endl;
	    handle_MLME_RESET_request(primitive->getSetDefaultPIB());
	    delete primitive;
	    break;

	case TP_MLME_RX_ENABLE_REQUEST:
	    EV << "[MAC]: this is a MLME_RX_ENABLE_REQUEST" << endl;
	    handle_MLME_RX_ENABLE_request(primitive->getDeferPermit(), primitive->getRxOnTime(), primitive->getRxOnDuration(), (RangingControl)primitive->getRangingRxControl());
	    delete primitive;
	    break;

	case TP_MLME_SYNC_REQUEST:
	    EV << "[MAC]: this is a MLME_SYNC_REQUEST" << endl;
	    handle_MLME_SYNC_request(primitive->getChannelNumber(), primitive->getChannelPage(), primitive->getTrackBeacon());
	    delete primitive;
	    break;

	case TP_MLME_POLL_REQUEST:
	    EV << "[MAC]: this is a MLME_POLL_REQUEST" << endl;
	    handle_MLME_POLL_request((Ieee802154eAddrMode)primitive->getCoordAddrMode(), primitive->getCoordPANId(), (IE3ADDR)primitive->getCoordAddress(), primitive->getSecurityLevel(), primitive->getKeyIdMode(), primitive->getKeySource(), primitive->getKeyIndex());
	    delete primitive;
	    break;

	case TP_MLME_DPS_REQUEST:
	    EV << "[MAC]: this is a MLME_DPS_REQUEST" << endl;
	    handle_MLME_DPS_request(primitive->getTxDPSIndex(), primitive->getRxDPSIndex(), primitive->getDpsIndexDuration());
	    delete primitive;
	    break;

	case TP_MLME_SOUNDING_REQUEST:
	    EV << "[MAC]: this is a MLME_SOUNDING_REQUEST" << endl;
	    handle_MLME_SOUNDING_request();
	    delete primitive;
	    break;

	case TP_MLME_CALIBRATE_REQUEST:
	    EV << "[MAC]: this is a MLME_CALIBRATE_REQUEST - Note: UWB is not supported" << endl;
	    // handle_MLME_CALIBRATE_requeste(...); // UWB actually not supported
	    delete primitive;
	    break;

	default:
	    error("[MAC]: unknown primitive received (msgkind=%d)", msgkind);
	    break;
    }
}

/**@author: 2014    Stefan Reis     (modified) */
void Ieee802154eMac::handleUpperMsg(cPacket* msg) // MCPS-SAP
{
    uint16_t index;
    IE3ADDR destAddr;
    UINT_16 destPanId;
    UINT_8 srcAddrMode;
    bool gtsFound = false;
    numUpperPkt++;

    /** brief:  TRUE if a GTS is to be used for transmission, FALSE indicates that the CAP will be used.
     *          see Std 802.15.4-2011 (6.3.1 MCPS-DATA.request) page 118 */
    bool gtsTX = false;

    /** brief:  TRUE if indirect transmission is to be used, FALSE otherwise.
     *
     *  note:   If the TxOptions parameter specifies that an indirect transmission is required and
     *          if the device receiving this primitive is not a coordinator, the destination
     *          address is not present, or the TxOptions parameter also specifies a GTS transmission,
     *          the indirect transmission option will be ignored. - see Std 802.15.4-2006 (7.1.1.1.3 Effect on receipt) page 70
     *
     *  note:   Specifying a GTS transmission in the TxOptions parameter overrides an
     *          indirect transmission request. - see Std 802.15.4-2011 (6.3.1 MCPS-DATA.request) page 118
     *
     *  note:   If the TxOptions parameter specifies that an indirect transmission is not required, the MAC sublayer will
     *          transmit the MSDU using CSMA-CA either in the CAP for a beacon-enabled PAN or immediately for a
     *          nonbeacon-enabled PAN, or at the next timeslot to the destination address if in TSCH mode.
     *          - see Std 802.15.4e-2012 (6.3.1 MCPS-DATA.request) page 168 */
    bool indirectTX = false;

    // MAC layer can process only one data request at a time
    // If no ifq exists, upper layer is not aware of the current state of MAC layer
    // Check if MAC is busy processing one data request, if true, drop it
    // if ifq exists, request another msg from the ifq whenever MAC is idle and ready for next data transmission
    if(taskP.taskStatus(TP_MCPS_DATA_REQUEST))
    {
	EV << "[MAC]: an " << msg->getName() << " (#" << numUpperPkt << ") received from the upper layer, but drop it due to busy MAC" << endl;
	MCPS_DATA_confirm(0, msg->getArrivalTime().getScale(), false, 0, 0, 0, 0, 0, mac_TRANSACTION_EXPIRED, 0, 0, msg);
	delete msg;
	numUpperPktLost++;
	reqtMsgFromIFq();
	return;
    }

    //check if parameters valid or not, only check msdu size here
    if(PK(msg)->getByteLength() > Ieee802154ePhy::aMaxMACFrameSize)
    {
	EV << "[MAC]: an " << msg->getName() <<" (#" << numUpperPkt << ") received from the upper layer, but drop it due to oversize" << endl;
	MCPS_DATA_confirm(0, msg->getArrivalTime().getScale(), false, 0, 0, 0, 0, 0, mac_FRAME_TOO_LONG, 0, 0, msg);
	delete msg;
	numUpperPktLost++;
	reqtMsgFromIFq();
	return;
    }

    cObject *controlInfo = msg->removeControlInfo();

    if(dynamic_cast<Ieee802Ctrl *>(controlInfo)) // for IPv4 Network layer and MANET routing
    {
	useIeee802Ctrl = true;
	Ieee802Ctrl* cInfo = check_and_cast<Ieee802Ctrl *>(controlInfo);
	destAddr = cInfo->getDest();

	if(destAddr.isBroadcast()) //  packet destined as broadcast
	{
	    destAddr = (IE3ADDR)0xffff; // broadcast address - Std 802.15.4-2006 (7.3.1.1) page 150
	    destPanId = 0xffff; // broadcast PAN ID - Std 802.15.4-2006 (7.3.1.1) page 150
	    gtsTX = false;              // send in the CAP
	    indirectTX = false;         // send direct
	}
	else if(destAddr.equals(mpib.macCoordExtendedAddress) || destAddr.equals((IE3ADDR)mpib.macCoordShortAddress)) // packet destined for my coordinator
	{
	    if(notAssociated)
	    {
		EV << "[MAC]: an " << msg->getName() << " destined for the coordinator received from the upper layer, but drop it due to being not associated with any coordinator yet" << endl;
		MCPS_DATA_confirm(0, msg->getArrivalTime().getScale(), false, 0, 0, 0, 0, 0, mac_INVALID_ADDRESS, 0, 0, msg);
		delete msg;
		delete cInfo;
		numUpperPktLost++;
		reqtMsgFromIFq();
		return;
	    }
	    else if(dataTransMode == 3)  // send in GTS (set in the omnetpp.ini)
	    {
		ASSERT(!isPANCoor);
		// check if I have a transmit GTS
		if(gtsStartSlot == 0 || isRecvGTS) // no transmit GTS
		{
		    EV << "[MAC]: an " << msg->getName() << " requesting GTS transmission destined for the PAN Coordinator received from the upper layer, but drop it due to no transmit GTS allocated by the PAN coordinator yet" << endl;
		    MCPS_DATA_confirm(0, msg->getArrivalTime().getScale(), false, 0, 0, 0, 0, 0, mac_INVALID_GTS, 0, 0, msg);
		    delete msg;
		    delete cInfo;
		    numUpperPktLost++;
		    reqtMsgFromIFq();
		    return;
		}
		else    // found one transmit GTS
		{
		    destAddr = MACAddress(mpib.macCoordShortAddress);
		    destPanId = mpib.macPANId;
		    gtsTX = true;       // send in the GTS
		    indirectTX = false; // send direct
		}
	    }
	    else // other transfer mode (dataTransMode == 1) - send direct (indirect transmission are not allowed for devices)
	    {
		destAddr = MACAddress(mpib.macCoordShortAddress);
		destPanId = mpib.macPANId;
		gtsTX = false;      // send in the CAP
		indirectTX = false; // send direct
	    }
	}
	else        //  packet destined for device
	{
	    destPanId = (mpib.macPANId == 0xffff) ? 0xffff:mpib.macPANId; // if the device have no PAN ID use the broadcast PAN ID - Std 802.15.4-2006 (7.3.1.1) page 150

	    if(mpib.macShortAddress != 0xfffe) // device is associated to a PAN coordinator
	    {
		// check if dest node is in my device list (associated or not)
		if(mpib.macShortAddress != 0xfffe && deviceList.find(getShortAddress(destAddr)) == deviceList.end() && !neighborTable->isNeighborBySAddr(getShortAddress(destAddr)))
		{
		    EV << "[MAC]: an " << msg->getName() << " destined for the device with MAC address " << destAddr << " received from the upper layer, but drop it due to no device with this address found in my device list" << endl;
		    MCPS_DATA_confirm(0, msg->getArrivalTime().getScale(), false, 0, 0, 0, 0, 0, mac_INVALID_ADDRESS, 0, 0, msg);
		    delete msg;
		    delete cInfo;
		    numUpperPktLost++;
		    reqtMsgFromIFq();
		    return;
		}
		// if GTS, check in my GTS list if the dest device has a receive GTS
		if(dataTransMode == 3)   // send in GTS (set in the omnetpp.ini)
		{
		    ASSERT(isPANCoor);  // i must be the PAN coordinator

		    // check if there is a receive GTS allocated for the dest node in my GTS list
		    for(index = 0; index < gtsCount; index++)
		    {
			if(gtsList[index].devShortAddr == getShortAddress(destAddr) && gtsList[index].isRecvGTS)
			{
			    gtsFound = true;
			    break;
			}
		    }
		    if(gtsFound)
		    {
			// set isTxPending true in corresponding GTS descriptor
			gtsList[index].isTxPending = true;
			gtsTX = true;      // send in the GTS
			indirectTX = true; // send indirect (will be ignored)
		    }
		    else
		    {
			EV << "[MAC]: an " << msg->getName() << " requesting GTS transmission destined for the device received from the upper layer, but drop it due to no valid GTS for this device found in my GTS list" << endl;
			MCPS_DATA_confirm(0, msg->getArrivalTime().getScale(), false, 0, 0, 0, 0, 0, mac_INVALID_GTS, 0, 0, msg);
			delete msg;
			delete cInfo;
			numUpperPktLost++;
			reqtMsgFromIFq();
			return;
		    }
		}
		else
		{ // other transfer mode (dataTransMode -> direct or indirect) - send it in CAP
		    gtsTX = false;     // send in the CAP
		    indirectTX = isPANCoor ? true:false; // send indirect for PAN, send direct for devices
		    indirectTX = mpib.macTSCHenabled ? false:indirectTX; // send direct in TSCH mode
		    destAddr.setFlagEui64(false); // use the short address if associated
		}
	    }
	    else // device is not associated to a PAN coordinator
	    {
		if(!destAddr.getFlagEui64()) // not associated device should use there extended address
		    destAddr.convert64();

		gtsTX = false;      // send in the CAP
		indirectTX = false; // send direct
	    }
	}
    }
    else // for the StarRouting Network layer - for the StarNet and Nodes Random Direct example
    {
	useIeee802Ctrl = false;
	Ieee802154eNetworkCtrlInfo* cInfo = check_and_cast<Ieee802154eNetworkCtrlInfo *>(controlInfo);

	indirectTX = false; // send direct

	// translate to MAC address
	if(cInfo->getToParent())    // packet destined for my coordinator
	{
	    if(notAssociated)
	    {
		EV << "[MAC]: an " << msg->getName() << " destined for the coordinator received from the upper layer, but drop it due to being not associated with any coordinator yet" << endl;
		MCPS_DATA_confirm(0, msg->getArrivalTime().getScale(), false, 0, 0, 0, 0, 0, mac_INVALID_ADDRESS, 0, 0, msg);
		delete msg;
		delete cInfo;
		numUpperPktLost++;
		reqtMsgFromIFq();
		return;
	    }
	    else if(dataTransMode == 3) // GTS mode - Std 802.15.4-2011 (4.5.2) page 13 - Note: The third transaction is the data transfer between two peer devices.
	    {
		gtsTX = true;      // send in the GTS

		ASSERT(!isPANCoor);
		// check if I have a transmit GTS - see text in Std 802.15.4-2011 (6.3.1 MCPS-DATA.request) page 118
		if(gtsStartSlot == 0 || isRecvGTS)
		{
		    EV << "[MAC]: an " << msg->getName() << " requesting GTS transmission destined for the PAN Coordinator received from the upper layer, but drop it due to no transmit GTS allocated by the PAN coordinator yet" << endl;
		    MCPS_DATA_confirm(0, msg->getArrivalTime().getScale(), false, 0, 0, 0, 0, 0, mac_INVALID_GTS, 0, 0, msg);
		    delete msg;
		    delete cInfo;
		    numUpperPktLost++;
		    reqtMsgFromIFq();
		    return;
		}
		else
		{
		    destAddr = MACAddress(mpib.macCoordShortAddress);
		    destPanId = mpib.macPANId;
		}
	    }
	    else        // other transfer mode (dataTransMode == 1)
	    {
		destAddr = MACAddress(mpib.macCoordShortAddress);
		destPanId = mpib.macPANId;
		gtsTX = true;      // send in the CAP
	    }
	}
	else        //  packet destined for device
	{
	    if(!simulation.getModuleByPath(cInfo->getDestName()))
		error("[MAC]: address conversion fails, destination host does not exist!");

#if OMNETPP_VERSION >= 0x403
	    cModule* module =
	    simulation.getModuleByPath(cInfo->getDestName())->getModuleByPath(".wlan.mac");
#else
	    cModule* module = simulation.getModuleByPath(cInfo->getDestName())->getModuleByRelativePath("wlan.mac");
#endif

	    Ieee802154eMac* macModule = check_and_cast<Ieee802154eMac *>(module);
	    destAddr = macModule->getMacAddr();
	    destPanId = mpib.macPANId;

	    // check if dest node is in my device list (associated or not)
	    if(deviceList.find(getShortAddress(destAddr)) == deviceList.end())
	    {
		EV << "[MAC]: an " << msg->getName() << " destined for the device with MAC address " << destAddr << " received from the upper layer, but drop it due to no device with this address found in my device list" << endl;

		MCPS_DATA_confirm(0, msg->getArrivalTime().getScale(), false, 0, 0, 0, 0, 0, mac_INVALID_ADDRESS, 0, 0, msg);

		delete msg;
		delete cInfo;
		numUpperPktLost++;
		reqtMsgFromIFq();
		return;
	    }

	    // if GTS, check in my GTS list if the dest device has a receive GTS
	    if(dataTransMode == 3) // Std 802.15.4-2011 (4.5.2) page 13 - Note: The third transaction is the data transfer between two peer devices.
	    {
		gtsTX = true;      // send in the GTS
		indirectTX = false; // send direct

		ASSERT(isPANCoor);  // i must be the PAN coordinator

		// check if there is a receive GTS allocated for the dest node in my GTS list
		for(index = 0; index < gtsCount; index++)
		{
		    if(gtsList[index].devShortAddr == getShortAddress(destAddr) && gtsList[index].isRecvGTS)
		    {
			gtsFound = true;
			break;
		    }
		}
		if(gtsFound)
		{
		    // set isTxPending true in corresponding GTS descriptor
		    gtsList[index].isTxPending = true;
		    indirectTX = true; // send indirect
		}
		else
		{
		    EV << "[MAC]: an " << msg->getName() << " requesting GTS transmission destined for the device received from the upper layer, but drop it due to no valid GTS for this device found in my GTS list" << endl;
		    MCPS_DATA_confirm(0, msg->getArrivalTime().getScale(), false, 0, 0, 0, 0, 0, mac_INVALID_GTS, 0, 0, msg);
		    delete msg;
		    delete cInfo;
		    numUpperPktLost++;
		    reqtMsgFromIFq();
		    return;
		}
	    }
	}
    }
    delete controlInfo;

    EV << "[MAC]: an " << msg->getName() << " (#" << numUpperPkt << ", " << PK(msg)->getByteLength() << " Bytes, destined for MAC address " << destAddr << ", transfer mode " << dataTransMode << ") received from the upper layer" << endl;

    /** brief: Corresponding to the options in the multipurpose frame control
     *         or general frame control for frame version 0b10. - see Std 802.15.4e-2012 (table 46) page 168 */
    FrameCrlOptions frameControlOptions;
    frameControlOptions.panIDsuppressed = false;            //default: false
    frameControlOptions.iesIncluded = (useTSCH) ? true:false; //TODO:[SR] handel of the IE's list
    frameControlOptions.seqSuppressed = false;

    std::vector<Ieee802154eIEHeaderType> headerIElist(Ieee802154eIEHeaderType(0x1a));
    std::vector<Ieee802154eIEPayloadType> payloadIElist(Ieee802154eIEHeaderType(0x00));

    if(mpib.macShortAddress == 0xfffe) // if the device is not associated with a coordinator - Std 802.15.4-2011
	srcAddrMode = defFrmCtrl_AddrMode64;
    else
	srcAddrMode = defFrmCtrl_AddrMode16;

    if(mpib.macTSCHenabled) // in the timeslots it may used a 2-octet address - Std 802.15.4e-2012 (5.1.2.6 TSCH PAN formation) page 24
	srcAddrMode = defFrmCtrl_AddrMode16;

    // for the acknowledge of the transmission
    bool ackTX;
    if(destAddr == (IE3ADDR)0xffff) //no ACK at broadcast - see Std 802.15.4-2006 (7.5.6.4) page 189
	ackTX = false;
    else if(dataTransMode == 3 && ack4Gts)
	ackTX = true;
    else
	ackTX = ack4Data;

    MCPS_DATA_request((Ieee802154eAddrMode)srcAddrMode, (Ieee802154eAddrMode)(destAddr.getFlagEui64() ?
    defFrmCtrl_AddrMode64:
													defFrmCtrl_AddrMode16), destPanId, destAddr, (UINT_8)msg->getByteLength(), msg, mpib.macDSN++/*msduHandle (FIXME [SR] we have no msduHandle from the NETWORK layer)*/, ackTX, gtsTX, indirectTX, secuData/*securityLevel*/, 0/*keyIdMode*/, 0/*keySource*/, 0/*keyIndex*/, UWBPRF_PRF_OFF, RANGING_OFF, 0/*uwbPreambleSymbolRepetitions,*/, 0/*dataRate*/, frameControlOptions, headerIElist, payloadIElist, false /*sendMultipurpose*/);
}

/**@author: 2014    Stefan Reis     (modified) */
void Ieee802154eMac::handleLowerMsg(cPacket* msg) // PD_SAP
{
    //[SR] old version
    bool noAck;
    int i = 0;
    Ieee802154eFrame* frame = dynamic_cast<Ieee802154eFrame *>(msg);

    if(!frame)
    {
	EV << "[MAC]: message from physical layer (" << msg->getClassName() << ")" << msg->getName() << " is not a subclass of Ieee802154eFrame, drop it" << endl;
	delete frame;
	return;
    }

    if(msg->getControlInfo())
	delete msg->removeControlInfo();

    FrameCtrl frmCtrl = frame->getFrmCtrl();
    Ieee802154eFrameType frmType = frmCtrl.frameType;

    EV << "[MAC]: an " << frameTypeTxt[frmType] << " frame received from the PHY layer, performing filtering now ..." << endl;

    // perform MAC frame filtering
    if(frameFilter(frame))
    {
	EV << "The received frame is filtered, drop frame" << endl;
	delete frame;
	// reset the TSCH task if the pkt was dropped
	if(mpib.macTSCHenabled)
	{
	    if(taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 10)
	    {
		taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 11;
		EV << "[TSCH CCA]-Transmitter:[11] frame was dropped at the MAC layer" << endl;
		handleTsAckWait();
		// next step in handleTsAckWait()
	    }
	    else if(taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) == 5)
	    {
		taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) = 6;
		EV << "[TSCH CCA]-Receiver:[6] frame was dropped at the MAC layer" << endl;
		handleTsRxWait();
	    }
	}
	return;
    }

    // check if neighbor in the neighborTable

    // check timing for GTS (debug)
//    if (frmType == Ieee802154e_DATA && frame->getIsGTS())
//    {
//        if (isPANCoor)
//        {
//            // check if I'm supposed to receive the data from this device in this GTS
//            if (indexCurrGts == 99 || gtsList[indexCurrGts].isRecvGTS || gtsList[indexCurrGts].devShortAddr != getShortAddress(frame->getSrcAddr()))
//                error("[GTS]: timing error, PAN coordinator is not supposed to receive this DATA pkt at this time!");
//        }
//        else
//        {
//            if (index_gtsTimer != 99 || !isRecvGTS || frame->getSrcAddr().getInt() != mpib.macCoordShortAddress)
//                error("[GTS]: timing error, the device is not supposed to receive this DATA pkt at this time!");
//        }
//    }

    EV << "[MAC]: checking if the received frame requires an ACK" << endl;
    //send an acknowledgment if needed (no matter this is a duplicated packet or not)
    if((frmType == Ieee802154e_DATA) || (frmType == Ieee802154e_CMD) || (frmType == Ieee802154e_MULTI) || (frmType == Ieee802154e_ASSOCIATION_REQUEST) || (frmType == Ieee802154e_ASSOCIATION_RESPONCE) || (frmType == Ieee802154e_SCHEDULER_REQUEST) || (frmType == Ieee802154e_SCHEDULER_RESPONCE))
    {
	if(frmCtrl.ackReq) //Acknowledgment required
	{
	    /*
	     //association request command will be ignored under following cases
	     if (frmType == Ieee802154_CMD && check_and_cast<Ieee802154CmdFrame *>(frame)->getCmdType() == Ieee802154_ASSOCIATION_REQUEST)
	     if ((!capability.FFD) //not an FFD
	     || (mpib.macShortAddress == 0xffff) //not yet joined any PAN
	     || (!macAssociationPermit)) //association not permitted
	     {
	     delete frame;
	     return;
	     }*/

	    noAck = false;
	    // MAC layer can process only one command (rx or tx) at a time
	    if(frmType == Ieee802154e_CMD)
		if((rxCmd) || (txBcnCmd))
		    noAck = true;

	    if(!noAck)
	    {
		EV << "[MAC]: yes, constructing the ACK frame" << endl;
		if(!txAck)
		    constructACK(frame);
		else
		{
		    resetTRX();
		    return;
		}

		// for TSCH Std 802.15.4e-2012 - TSCH CCA Receiver
		if(mpib.macTSCHenabled && taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) == 5)
		{
		    taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) = 7;
		    EV << "[TSCH CCA]-Receiver:[7] the data frame arrived, set the TRX_STATE on TRX_OFF and wait the TsTxAckDelay" << endl;
		    PLME_SET_TRX_STATE_request(phy_TRX_OFF); // state change confirm will be ignored

		    if(tsTxAckDelayTimer->isScheduled())
			cancelEvent(tsTxAckDelayTimer);

		    scheduleAt(simTime() + timeslotTable->getTemplate(useTimeslotID)->getTxAckDelayDbl(), tsTxAckDelayTimer);
		    // next step in handleTsTxAckDelay()
		}
		else
		{    // old version [SR]
		     //stop CSMA-CA if it is pending (it will be restored after the transmission of ACK)
		    if(backoffStatus == 99)
		    {
			EV << "[MAC]: CSMA-CA is pending, stop it, it will resume after sending ACK" << endl;
			backoffStatus = 0;
			csmacaCancel();
		    }
		    EV << "[MAC]: prepare to send the ACK, ask PHY layer to turn on the transmitter first" << endl;
		    PLME_SET_TRX_STATE_request(phy_TX_ON);
		}
	    }
	}
	else // no ACK required
	{
//            if (frame->getIsGTS()) // received in GTS
//            {
//                if (isPANCoor)
//                {
//                    // the device may transmit more pkts in this GTS, turn on radio
//                    PLME_SET_TRX_STATE_request(phy_RX_ON);
//                }
//                else
//                {
//                    // PAN coordinator can transmit only one pkt to me in my GTS, turn off radio now
//                    PLME_SET_TRX_STATE_request(phy_TRX_OFF);
//                }
//            }
//            else
//            {   // if this is a broadcast or a beacon, go back to sleep
	    if(taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) == 5)
	    {
		taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) = false;
		taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) = 0;
	    }
	    resetTRX();
//            }
	}
    }

    // drop new received cmd pkt if mac is current processing a cmd
    if(frmType == Ieee802154e_CMD)
    {
	if((rxCmd) || (txBcnCmd))
	{
	    EV << "[MAC]: the received CMD frame is dropped, because MAC is currently processing a MAC CMD" << endl;
	    delete frame;
	    return;
	}
    }

    // drop new received data pkt if mac is current processing last received data pkt
    if(frmType == Ieee802154e_DATA)
    {
	if(rxData)
	{
	    EV << "[MAC]: the received DATA frame is dropped, because MAC is currently processing the last received DATA frame" << endl;
	    delete frame;
	    return;
	}
    }

    //check duplication -- must be performed AFTER all drop's
    if(frmType == Ieee802154e_BEACON)
	i = chkAddUpdHListLink(&hlistBLink1, &hlistBLink2, frame->getSrcAddr(), frame->getSeqNmbr());
    else if(frmType != Ieee802154e_ACK) // data or cmd
	i = chkAddUpdHListLink(&hlistDLink1, &hlistDLink2, frame->getSrcAddr(), frame->getSeqNmbr());
    else // ACK
    {
	// check ACK in <handleAck()>
    }

    if(i == 2) // duplication found in the HListLink
    {
	EV << "[MAC]: duplication detected, drop frame" << endl;
	delete frame;

	if(frmType == Ieee802154e_DATA && mpib.macMetricsEnabled)
	    mpib.macDuplicateFrameCount++;

	return;
    }

    switch(frmType)
    {
	case Ieee802154e_BEACON:
	    EV << "[MAC]: continue to process received BEACON pkt" << endl;
	    handleBeacon(frame);
	    break;

	case Ieee802154e_DATA:
	    EV << "[MAC]: continue to process received DATA pkt" << endl;
	    handleData(frame);
	    break;

	case Ieee802154e_ACK:
	    EV << "[MAC]: continue to process received ACK pkt" << endl;
	    handleAck(frame);
	    break;

//	case Ieee802154e_CMD:
//	    EV << "[MAC]: continue to process received CMD pkt" << endl;
//	    handleCommand80215(frame);
//	    break;

	case Ieee802154e_LLDN:  // LLDN (Low Latency Deterministic Network)
	    EV << "[MAC]: continue to process received LLDN pkt" << endl;
	    handleLLDN802154e(frame);
	    break;

	case Ieee802154e_MULTI:  // Multipurpose
	    EV << "[MAC]: continue to process received Multipurpose pkt" << endl;
	    handleMulti802154e(frame);
	    break;

	case Ieee802154e_ASSOCIATION_REQUEST:
	    EV << "[MAC} continue to proxess received association request pkt" << endl;
	    MLME_ASSOCIATE_indication(frame);
	    break;
	case Ieee802154e_ASSOCIATION_RESPONCE:
	    EV << "[MAC} continue to process received association response pkt" << endl;
	    MLME_ASSOCIATE_confirm(frame->dup());
	    delete frame;
	    break;
	case Ieee802154e_SCHEDULER_REQUEST:
	    EV << "[MAC] continue to process received scheduler request pkt" << endl;
	    SCHEDULE_indication(frame->dup());
	    delete frame;
	    break;
	case Ieee802154e_SCHEDULER_RESPONCE:
	    EV << "[MAC] continue to process received scheduler response pkt" << endl;
	    SCHEDULE_confirm(frame->dup(), false);
	    delete frame;
	    break;
	default:
	    error("[MAC]: undefined MAC frame type: %d", frmType);
	    break;
    }
}

void Ieee802154eMac::handleSelfMsg(cMessage* msg)
{
    switch(msg->getKind())
    {
	case START_PAN_COOR_TIMER:
	    startPANCoor();
	    delete msg;     // it's a dynamic timer
	    break;

	case MAC_BACKOFF_TIMER:
	    handleBackoffTimer();
	    break;

	case MAC_DEFER_CCA_TIMER:
	    handleDeferCCATimer();
	    break;

	case MAC_BCN_RX_TIMER:
	    handleBcnRxTimer();
	    break;

	case MAC_BCN_TX_TIMER:
	    handleBcnTxTimer();
	    break;

	case MAC_ACK_TIMEOUT_TIMER:
	    handleAckTimeoutTimer();
	    break;

	case MAC_TX_ACK_BOUND_TIMER:
	    handleTxAckBoundTimer();
	    break;

	case MAC_TX_CMD_DATA_BOUND_TIMER:
	    handleTxCmdDataBoundTimer();
	    break;

	case MAC_IFS_TIMER:
	    handleIfsTimer();
	    break;

	case MAC_TX_SD_TIMER:
	case MAC_RX_SD_TIMER:
	    handleSDTimer();
	    break;

	case MAC_FINAL_CAP_TIMER:
	    handleFinalCapTimer();
	    break;

	case MAC_GTS_TIMER:
	    handleGtsTimer();
	    break;

	case MAC_ASN_TIMER:
	    handleAsnTimer();
	    break;

	case MAC_TS_CCA_OFFSET_TIMER:
	    handleTsCCAOffset();
	    break;

	case MAC_TS_TX_OFFSET_TIMER:
	    handleTsTxOffset();
	    break;

	case MAC_TS_RX_ACK_DELAY_TIMER:
	    handleTsRxAckDelay();
	    break;

	case MAC_TS_ACK_WAIT_TIMER:
	    handleTsAckWait();
	    break;

	case MAC_TS_RX_OFFSET_TIMER:
	    handleTsRxOffset();
	    break;

	case MAC_TS_RX_WAIT_TIMER:
	    handleTsRxWait();
	    break;

	case MAC_TS_TX_ACK_DELAY_TIMER:
	    handleTsTxAckDelay();
	    break;

	case MAC_SCAN_TIMER:
	    MLME_SCAN_confirm(msg);
	    break;

	default:
	    error("[MAC]: unknown MAC timer type!");
	    break;
    }
}

void Ieee802154eMac::sendDown(Ieee802154eFrame* frame)
{
    /*if (updateNFailLink(fl_oper_est,index_) == 0)
     {
     if (txBeacon)
     {
     beaconWaiting = false;
     Packet::free(txBeacon);
     txBeacon = 0;
     }
     return;
     }
     else if (updateLFailLink(fl_oper_est,index_,p802_15_4macDA(p)) == 0)
     {
     dispatch(p_UNDEFINED,"PD_DATA_confirm");
     return;
     }*/

    // TODO: energy model
    inTransmission = true;           // cleared by PD_DATA_confirm
    EV << "[MAC]: sending frame " << (frame->getFrmCtrl().frameType != Ieee802154e_DATA ? (frame->getName()):(frame->getEncapsulatedPacket()->getName())) << " (" << frame->getByteLength() << " Bytes) to PHY layer" << endl;
    EV << "[MAC]: the estimated transmission time is " << calDuration(frame) << " s" << endl;
    send(frame, mLowerLayerOut);     // send a duplication
}

void Ieee802154eMac::reqtMsgFromIFq()
{
    if(queueModule)
    {
	// tell queue module that we've become idle
	EV << "[MAC]: requesting another frame from queue module" << endl;
	queueModule->requestPacket();
    }
}

//-------------------------------------------------------------------------------/
/************************ <MAC Frame Reception Handler> *************************/
//-------------------------------------------------------------------------------/
/**@author: 2014    Stefan Reis     (modified) */
void Ieee802154eMac::handleBeacon(Ieee802154eFrame* frame)
{
    if(frame->getFrmCtrl().frameVersion == Ieee802154_compatible && mpib.macTSCHcapable) // enhanced beacon frame, only if the device is TSCH capable
    {
	if(isSCAN)
	{




	    MLME_SCAN_confirm(frame->dup());
	    delete frame;

	}
	else
	{
	    rxBeacon = check_and_cast<Ieee802154EnhancedBeaconFrame *>(frame);
	    Ieee802154eNetworkCtrlInfo *tmp = new Ieee802154eNetworkCtrlInfo("BeaconConfirm", TP_MLME_SCAN_CONFIRM);

	    tmp->setName("BeaconNotify");
	    tmp->setKind(TP_MLME_BEACON_NOTIFY_INDICATION);
	    MLME_BEACON_NOTIFY_indication(tmp->dup());
	    delete tmp;
	}
//	EV << "[MAC]: starting processing received enhanced Beacon frame with frame version=" << (int)frame->getFrmCtrl().frameVersion << endl;
//	Ieee802154EnhancedBeaconFrame *bcnFrame = check_and_cast<Ieee802154EnhancedBeaconFrame *>(frame);
//

//
//	// handle the IE's
//	handleIEfield(bcnFrame);
//
//	// start ASN timer
//	startAsnTimer(bcnRxTime - (timeslotTable->getTemplate(useTimeslotID)->getRxOffsetDbl() + (timeslotTable->getTemplate(useTimeslotID)->getRxWaitDbl() / 2)));
//
//	// reset lost beacon counter
//	bcnLossCounter = 0;
//
//	// temporary solution for association process, to be modified in later version
//	if(notAssociated) // this is my first rxed beacon, associate with this one,
//	{
//	    // activate the TSCH mode
//	    mpib.macTSCHenabled = mpib.macTSCHcapable ? true:false; //activate TSCH mode only if the device is TSCH capable
//	    backoffMethod = EXPONENTIAL;
//	    // TSCH-specific MAC PIB attributes - Std 802.15.4e-2012 (table 52b) page 174
//	    mpib.macMinBE = 1;    // default: for CSMA-CA = 3; for TSCH-CA = 1
//	    mpib.macMaxBE = 7;    // default: for CSMA-CA = 5; for TSCH-CA = 7
//
//	    ASSERT(mpib.macCoordShortAddress == def_macCoordShortAddress);
//	    ASSERT(mpib.macPANId == def_macPANId);
//	    notAssociated = false;
//
//	    mpib.macShortAddress = (UINT_16)mpib.macExtendedAddress.getInt();
//	    mpib.macSimpleAddress = (UINT_8)mpib.macExtendedAddress.getInt();
//
//	    mpib.macPANId = bcnFrame->getSrcPanId();           // store PAN id
//	    mpib.macCoordShortAddress = getShortAddress(bcnFrame->getSrcAddr()); // store coordinator address, always use short address
//	    mpib.macCoordExtendedAddress = bcnFrame->getSrcAddr(); // PAN coordinator uses the same address for both its own 16 and 64 bit address
//
//	    EV << "This is my first beacon, associate with it" << endl;
//#if OMNETPP_VERSION >= 0x403
//	    cModule* module =
//	    simulation.getModuleByPath(panCoorName)->getModuleByPath(".wlan.mac");
//#else
//	    cModule* module = simulation.getModuleByPath(panCoorName)->getModuleByRelativePath("wlan.mac");
//#endif
//	    Ieee802154eMac* macModule = check_and_cast<Ieee802154eMac *>(module);
//	    mpib.macShortAddress = macModule->associate_request_cmd(aExtendedAddress, capability);
//	}
//	else
//	{
//	    // time correction
//	    calcTimeCorr(frame);
//	}
//
//	taskP.TS_RX_CCA_tschcca = false;
//	taskP.TS_TX_CCA_tschcca = false;
//	taskP.TS_RX_CCA_tschcca_STEP = 0;
//	taskP.TS_TX_CCA_tschcca_STEP = 0;
//
//	dispatch(phy_SUCCESS, __FUNCTION__);
//
//	resetTRX();
//	delete bcnFrame;
//	numRxBcnPkt++;
    }
    else    // beacon frame - Std 802.15.4-2006/2011
    {
	// old version [SR]
	EV << "[MAC]: starting processing received Beacon frame with frame version=" << frame->getFrmCtrl().frameVersion << endl;
	Ieee802154eBeaconFrame *bcnFrame = check_and_cast<Ieee802154eBeaconFrame *>(frame);

	simtime_t now, tmpf, w_time, duration;
	uint16_t ifs;
	int dataFrmLength;
	now = simTime();

	//update beacon parameters
	rxSfSpec = bcnFrame->getSfSpec();
	rxBO = rxSfSpec.BO;
	rxSO = rxSfSpec.SO;
	rxSfSlotDuration = aBaseSlotDuration * (1 << rxSO);

	//calculate the time when the first bit of the beacon was received
	duration = calDuration(frame);
	bcnRxTime = now - duration;

	/**
	 * important: this value is calculated in <csmacaStart()>, if later on a CSMA-CA is pending
	 *        for this bcn and backoff will resume without calling <csmacaStart()> (see <csmacaTrxBeacon()>) ,
	 *        therefore this value will not be updated, but <csmacaCanProceed()> and other functions will
	 *        use it and needs to be updated here
	 */
	schedBcnRxTime = bcnRxTime;
	EV << "The first bit of this beacon was received by PHY layer at " << bcnRxTime << endl;

	//calculate <rxBcnDuration>
	if(bcnFrame->getByteLength() <= aMaxSIFSFrameSize)
	    ifs = mpib.macSIFSPeriod;
	else
	    ifs = mpib.macLIFSPeriod;

	tmpf = duration * phy_symbolrate;
	tmpf += ifs;
	rxBcnDuration = (uint16_t)(SIMTIME_DBL(tmpf) / aUnitBackoffPeriod);

	if(fmod(tmpf, aUnitBackoffPeriod) > 0.0)
	    rxBcnDuration++;

	// TODO: store GTS fields
	//gtsFields = bcnFrame->getGtsFields();

	//update PAN descriptor
	rxPanDescriptor.coordAddrMode = (Ieee802154eAddrMode)frame->getFrmCtrl().srcAddrMode;
	rxPanDescriptor.coordPANId = bcnFrame->getSrcPanId();
	rxPanDescriptor.coordAddress_16_or_64 = bcnFrame->getSrcAddr();
	rxPanDescriptor.logicalChannel = ppib.phyCurrentChannel;
	// rxPanDescriptor.SuperframeSpec       // ignored, store in rxSfSpec above
	//rxPanDescriptor.GTSPermit     = gtsFields.permit;
	// rxPanDescriptor.LinkQuality          // TODO link quality at PHY layer
	// rxPanDescriptor.TimeStamp            // ignored, store in bcnRxTime above
	// rxPanDescriptor.SecurityUse          // security - not implemented
	// rxPanDescriptor.ACLEntry             // security - not implemented
	// rxPanDescriptor.SecurityFailure      // security - not implemented

	// start rxSDTimer
	startRxSDTimer();

	// reset lost beacon counter
	bcnLossCounter = 0;

	// temporary solution for association process, to be modified in later version
	if(notAssociated) // this is my first rxed beacon, associate with this one,
	{
	    ASSERT(mpib.macCoordShortAddress == def_macCoordShortAddress);
	    ASSERT(mpib.macPANId == def_macPANId);
	    notAssociated = false;

	    mpib.macShortAddress = (UINT_16)mpib.macExtendedAddress.getInt();
	    mpib.macSimpleAddress = (UINT_8)mpib.macExtendedAddress.getInt();

	    mpib.macPANId = bcnFrame->getSrcPanId();           // store PAN id
	    mpib.macCoordShortAddress = getShortAddress(bcnFrame->getSrcAddr()); // store coordinator address, always use short address
	    mpib.macCoordExtendedAddress = bcnFrame->getSrcAddr(); // PAN coordinator uses the same address for both its own 16 and 64 bit address

	    EV << "This is my first beacon, associate with it" << endl;
#if OMNETPP_VERSION >= 0x403
	    cModule* module =
	    simulation.getModuleByPath(panCoorName)->getModuleByPath(".wlan.mac");
#else
	    cModule* module = simulation.getModuleByPath(panCoorName)->getModuleByRelativePath("wlan.mac");
#endif
	    Ieee802154eMac* macModule = check_and_cast<Ieee802154eMac *>(module);
	    mpib.macShortAddress = macModule->associate_request_cmd(aExtendedAddress, capability);

	    startBcnRxTimer();              // start tracking beacon, always on
	    // start sending beacon from here, if I want to be a coordinator
	    /*
	     if ((mpib.macBeaconOrder != 15) && capability.FFD)
	     {
	     // actually started by mlme-start.request
	     startBcnTxTimer(true, simtime_t startTime);     // TBD
	     }*/

	    // if GTS, calculate the required GTS length for transmitting pkts with constant length
	    if(dataTransMode == 3) //  Std 802.15.4-2011 (4.5.2) page 13 - Note: The third transaction is the data transfer between two peer devices.
	    {
		// calculate the length of data frame transmitted in GTS
		// MHR(11) + gtsPayload + MFR(2)
		dataFrmLength = 11 + gtsPayload + 2;

		if(dataFrmLength <= aMaxSIFSFrameSize)
		    ifs = mpib.macSIFSPeriod;
		else
		    ifs = mpib.macLIFSPeriod;

		// calculate duration of the entire data transaction
		duration = (def_phyHeaderLength + dataFrmLength) * 8 / phy_bitrate;

		if(ack4Gts)
		    duration += (mpib.macAckWaitDuration + ifs) / phy_symbolrate;
		else
		    // no ACK required
		    duration += (Ieee802154ePhy::aTurnaroundTime + ifs) / phy_symbolrate;
		// store duration value for later evaluation in gtsCanProceed()
		gtsTransDuration = duration;

		// duration of one superframe slot (one GTS slot)
		tmpf = rxSfSlotDuration / phy_symbolrate;

		if(duration < tmpf)
		    gtsLength = 1;
		else
		{
		    gtsLength = (uint16_t)(duration / tmpf);
		    if(fmod(duration, tmpf) > 0.0)
			gtsLength++;
		}

		EV << "[GTS]: gtsTransDuration = " << gtsTransDuration << " s, duration of one GTS slot = " << tmpf << " s" << endl;

		// call gts_request_cmd() at the PAN coordinator to apply for GTS
		EV << "[GTS]: request " << (int)gtsLength << " GTS slots from the PAN coordinator" << endl;
		gtsStartSlot = macModule->gts_request_cmd(mpib.macShortAddress, gtsLength, isRecvGTS);

		if(gtsStartSlot != 0)      // successfully
		    EV << "[GTS]: my GTS start slot is " << (int)gtsStartSlot << endl;
		else        // failed
		{
		    // TBD: what to do if failed
		    // EV << "[GTS]: request for GTS failed" << endl;
		    error("[GTS]: request for GTS failed!");
		}
	    }
	}
	// can start my GTS timer only after receiving the second beacon
	else if(gtsStartSlot != 0)
	{
	    tmpf = bcnRxTime + gtsStartSlot * rxSfSlotDuration / phy_symbolrate;
	    w_time = tmpf - now;
	    // should turn on radio receiver aTurnaroundTime symbols before GTS starts, if I have a receive GTS
	    if(isRecvGTS)
		w_time = w_time - Ieee802154ePhy::aTurnaroundTime / phy_symbolrate;
	    EV << "[GTS]: schedule for my GTS with start slot #" << (int)gtsStartSlot << endl;
	    startGtsTimer(w_time);

	    // if my GTS is not the first one in the CFP, should turn radio off at the end of CAP using finalCAPTimer
	    if(gtsStartSlot != rxSfSpec.finalCap + 1)
	    {
		ASSERT(gtsStartSlot > rxSfSpec.finalCap);
		tmpf = bcnRxTime + (rxSfSpec.finalCap + 1) * rxSfSlotDuration / phy_symbolrate;
		w_time = tmpf - now;
		EV << "[GTS]: my GTS is not the first one in the CFP, schedule a timer to turn off radio at the end of CAP" << endl;
		startFinalCapTimer(w_time);
	    }
	}

	dispatch(phy_SUCCESS, __FUNCTION__);

	//CSMA-CA may be waiting for the new beacon
	if(backoffStatus == 99)
	    csmacaTrxBeacon('r');

	// TODO process pending address
	/*
	 #ifdef test_802154_INDIRECT_TRANS
	 rxPaFields = bcnFrame->getPaFields();       //store pending address fields
	 if (mpib.macAutoRequest)
	 {
	 //handle the pending packet
	 pending = false;
	 for (i=0;i<rxPaFields.numShortAddr;i++)
	 {
	 if (rxPaFields.addrList[i] == mpib.macShortAddress)
	 {
	 pending = true;
	 break;
	 }
	 }
	 if (!pending)
	 for (i=0;i<rxPaFields.numExtendedAddr;i++)
	 {
	 if (rxPaFields.addrList[rxPaFields.numShortAddr + i] == aExtendedAddress)
	 {
	 pending = true;
	 break;
	 }
	 }

	 if (pending)    // mlme_poll_request TBD
	 mlme_poll_request(frmCtrl.srcAddrMode,wph->MHR_SrcAddrInfo.panID,wph->MHR_SrcAddrInfo.addr_64,capability.secuCapable,true,true);
	 }
	 #endif
	 */
	resetTRX();
	delete bcnFrame;
	numRxBcnPkt++;
    }
}

void Ieee802154eMac::handleEB(cMessage *msg)
{
    if(dynamic_cast<Ieee802154EnhancedBeaconFrame*>(msg))
	rxBeacon = check_and_cast<Ieee802154EnhancedBeaconFrame *>(msg);
    else
	delete msg;

    simtime_t now, tmpf, duration;
    uint16_t ifs;
    now = simTime();

    //calculate the time when the first bit of the beacon was received
    duration = calDuration(rxBeacon);
    bcnRxTime = now - duration;

    /**
     * important: this value is calculated in <csmacaStart()>, if later on a CSMA-CA is pending
     *        for this bcn and backoff will resume without calling <csmacaStart()> (see <csmacaTrxBeacon()>) ,
     *        therefore this value will not be updated, but <csmacaCanProceed()> and other functions will
     *        use it and needs to be updated here
     */
    schedBcnRxTime = bcnRxTime;
    EV << "The first bit of this beacon was received by PHY layer at " << bcnRxTime << endl;

    //calculate <rxBcnDuration>
    if(rxBeacon->getByteLength() <= aMaxSIFSFrameSize)
	ifs = mpib.macSIFSPeriod;
    else
	ifs = mpib.macLIFSPeriod;

    tmpf = duration * phy_symbolrate;
    tmpf += ifs;
    rxBcnDuration = (uint16_t)(SIMTIME_DBL(tmpf) / aUnitBackoffPeriod);

    if(fmod(tmpf, aUnitBackoffPeriod) > 0.0)
	rxBcnDuration++;

    //update PAN descriptor
    rxPanDescriptor.coordAddrMode = (Ieee802154eAddrMode)rxBeacon->getFrmCtrl().srcAddrMode;
    rxPanDescriptor.coordPANId = rxBeacon->getSrcPanId();
    rxPanDescriptor.coordAddress_16_or_64 = rxBeacon->getSrcAddr();
    rxPanDescriptor.logicalChannel = ppib.phyCurrentChannel;

    // handle the IE's
    handleIEfield(rxBeacon);

    // reset lost beacon counter
    bcnLossCounter = 0;
    mpib.macTSCHenabled = mpib.macTSCHcapable ? true:false; //activate TSCH mode only if the device is TSCH capable
    backoffMethod = EXPONENTIAL;
    // TSCH-specific MAC PIB attributes - Std 802.15.4e-2012 (table 52b) page 174
    mpib.macMinBE = par("macMinBE");    // default: for CSMA-CA = 3; for TSCH-CA = 1
    mpib.macMaxBE = par("macMaxBE");    // default: for CSMA-CA = 5; for TSCH-CA = 7

    // start ASN timer
    startAsnTimer(bcnRxTime - (timeslotTable->getTemplate(useTimeslotID)->getRxOffsetDbl() + (timeslotTable->getTemplate(useTimeslotID)->getRxWaitDbl() / 2)));

//    ASSERT(mpib.macCoordShortAddress == def_macCoordShortAddress);
//    ASSERT(mpib.macPANId == def_macPANId);
    notAssociated = notAssociated;

    mpib.macShortAddress = (UINT_16)mpib.macExtendedAddress.getInt();
    mpib.macSimpleAddress = (UINT_8)mpib.macExtendedAddress.getInt();

    mpib.macPANId = rxBeacon->getSrcPanId();           // store PAN id
    mpib.macCoordShortAddress = getShortAddress(rxBeacon->getSrcAddr()); // store coordinator address, always use short address
    mpib.macCoordExtendedAddress = rxBeacon->getSrcAddr(); // PAN coordinator uses the same address for both its own 16 and 64 bit address

    // time correction
    calcTimeCorr(rxBeacon);

    taskP.TS_RX_CCA_tschcca = false;
    taskP.TS_TX_CCA_tschcca = false;
    taskP.TS_RX_CCA_tschcca_STEP = 0;
    taskP.TS_TX_CCA_tschcca_STEP = 0;

    dispatch(phy_SUCCESS, __FUNCTION__);

    resetTRX();
    Ieee802154eNetworkCtrlInfo *tmp = new Ieee802154eNetworkCtrlInfo("BeaconConfirm", TP_MLME_SET_BEACON_CONFIRM);
    send(tmp->dup(), mSchedulerOut);
    delete tmp;
    delete rxBeacon;
    rxBeacon = NULL;
    //delete bcnFrame;
    numRxBcnPkt++;

}

/**@author: 2014    Stefan Reis     (modified) */
void Ieee802154eMac::handleData(Ieee802154eFrame* frame)
{
    FrameCtrl frmCtrl = frame->getFrmCtrl();
    bool isSIFS = false;

    if(mpib.macTSCHenabled && frmCtrl.frameVersion == Ieee802154_compatible) // 802.15.4e Data frame
    {
	// Time correction
	calcTimeCorr(frame);

	ASSERT(rxData == NULL);
	rxData = frame;

	//FIXME:[SR] to we need this delay? (refer to Std 802.15.4-2006 (figure 68-IFS) page 170 for details of SIFS/LIFS)

	MCPS_DATA_indication((Ieee802154eAddrMode)rxData->getFrmCtrl().srcAddrMode, rxData->getSrcPanId(), rxData->getSrcAddr(), (Ieee802154eAddrMode)rxData->getFrmCtrl().dstAddrMode, rxData->getDstPanId(), rxData->getDstAddr(), rxData->getByteLength(), rxData, 0 /*mpduLinkQuality*/, rxData->getSeqNmbr(), (UINT_32)rxData->getTimestamp().raw(), rxData->getAuxSecHd().secLvl, rxData->getAuxSecHd().keyIdentMode, rxData->getAuxSecHd().keySource, rxData->getAuxSecHd().keyIndex, 0 /*uwbPRF*/, (Ieee802154eUWBFType)0 /*uwbPreambleSymbolRepetitions*/, 0 /*dataRate*/, (RangingControl)0 /*rangingReceived*/, 0 /*rangingCounterStart*/, 0 /*rangingCounterStop*/, 0 /*rangingTrackingInterval*/, 0 /*rangingOffset*/, 0 /*rangingFOM*/);

	rxData = NULL;
    }
    else    // 802.15.4 Data frame
    {
	if(mpib.macTSCHenabled)
	{
	    //FIXME:[SR] to we need this delay? (refer to Std 802.15.4-2006 (figure 68-IFS) page 170 for details of SIFS/LIFS)
	    ASSERT(rxData == NULL);
	    rxData = frame;

	    MCPS_DATA_indication((Ieee802154eAddrMode)rxData->getFrmCtrl().srcAddrMode, rxData->getSrcPanId(), rxData->getSrcAddr(), (Ieee802154eAddrMode)rxData->getFrmCtrl().dstAddrMode, rxData->getDstPanId(), rxData->getDstAddr(), rxData->getByteLength(), rxData, 0 /*mpduLinkQuality*/, rxData->getSeqNmbr(), (UINT_32)rxData->getTimestamp().raw(), rxData->getAuxSecHd().secLvl, rxData->getAuxSecHd().keyIdentMode, rxData->getAuxSecHd().keySource, rxData->getAuxSecHd().keyIndex, 0 /*uwbPRF*/, (Ieee802154eUWBFType)0 /*uwbPreambleSymbolRepetitions*/, 0 /*dataRate*/, (RangingControl)0 /*rangingReceived*/, 0 /*rangingCounterStart*/, 0 /*rangingCounterStop*/, 0 /*rangingTrackingInterval*/, 0 /*rangingOffset*/, 0 /*rangingFOM*/);

	    rxData = NULL;
	}
	else
	{
	    // [SR] old version
	    /** pass the data packet to upper layer
	     *  (we need some time to process the packet -- so delay SIFS/LIFS symbols from now or after finishing sending the ack.)
	     *  (refer to Std 802.15.4-2006 (figure 68-IFS) page 170 for details of SIFS/LIFS) */
	    ASSERT(rxData == NULL);
	    rxData = frame;
	    //rxDataTime = simTime();
	    if(!frame->getFrmCtrl().ackReq)
	    {
		if(frame->getByteLength() <= aMaxSIFSFrameSize)
		    isSIFS = true;

		startIfsTimer(isSIFS);
	    }
	    //else  //schedule and dispatch after finishing ack. transmission
	}
    }
}

/**@author: 2014    Stefan Reis     (modified) */
void Ieee802154eMac::handleAck(Ieee802154eFrame* frame) // TODO: include the 802.15.4e Ack Frame [SR]
{
    if(frame->getFrmCtrl().frameType == Ieee802154e_ACK)
    {
	if(frame->getFrmCtrl().frameVersion == Ieee802154_compatible) // 802.15.4e Enhanced ACK frame
	{
	    // TODO:[SR] handle timekeeping and keepalive

	    // TSCH CCA - Transmitter
	    if(mpib.macTSCHenabled && taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 10)
	    {
		//check the sequence number in the ACK to see if it matches that in the <txPkt>
		ASSERT(txData);

		// stop the TsAckWaitTimer
		if(tsAckWaitTimer->isScheduled())
		    cancelEvent(tsAckWaitTimer);

		if(frame->getSeqNmbr() != txData->getSeqNmbr())
		{
		    delete frame;
		    EV << "[TSCH CCA]-Transmitter:[11] frame was dropped at the MAC layer, the SeqNumber in the ACK does not match" << endl;

		    taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 11;
		    handleTsAckWait();
		    // next step in handleTsAckWait()
		    return;
		}
		else // successful transmission, with ACK
		{
		    // handle the Header and Payload IE's and check if there was a NACK inside
		    if(handleIEfield(frame))
			mpib.macNACKcount++; // count NACK

		    // count successful transmission
		    int retries = txData->getRetries();
		    if(mpib.macMetricsEnabled)
		    {
			if(retries == 0)
			    mpib.macTXSuccessCount++;
			else if(retries == 1)
			    mpib.macRetryCount++;
			else
			{
			    mpib.macMultipleRetryCount++;
			    if(retries == 2)
				numRetryTwo++;
			    else if(retries == 3)
				numRetryThree++;
			    else if(retries == 4)
				numRetryFour++;
			}
		    }

//		    if(retries == 4)
//			int test = 0;

		    // record for the statistics (with slotframe size 100 and sim end time=10000, we can use the ASN number direct)
		    int timeslot = FWMath::modulo(curASN, slotframeTable->getSlotframe(activeLinkEntry->getSlotframeId())->getSlotframeSize());
		    timeslot = retries * 100 + timeslot;
		    emit(sucTimeslot, timeslot);

		    // count the transmission in the neighborTable
		    UINT_16 destAddr = getShortAddress(txData->getDstAddr());

		    taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) = false;
		    taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 0;

		    string tmpTxt = (txData->getFrmCtrl().frameType == Ieee802154e_DATA) ? txData->getEncapsulatedPacket()->getName():frameTypeTxt[txData->getFrmCtrl().frameType];
		    EV << "[TSCH CCA]-Transmitter:[10] the correct ACK arrives, transmission of the " << txPkt->getName() << "(" << tmpTxt << ") frame was successful, delete frame from queue" << endl;

		    MCPS_DATA_confirm(0, txData->getArrivalTime().getScale(), false, 0, 0, 0, 0, 0, mac_SUCCESS, txData->getRetries(), 0, txData);

		    // delete the msg from the queue
		    queueModule->deleteMsgQueue(txData->getDstAddr(), false);

		    /**note:
		     * A successful transmission in a shared link resets the backoff window to the minimum value.
		     * The backoff window is reset to the minimum value if the transmission in a dedicated link
		     * is successful and the transmit queue is then empty.
		     * Std 802.15.4e-2012 (5.1.1.4.3 TSCH CSMA-CA algorithm) page 14 */
		    if(destAddr != 0xffff) // if is not the broadcast address
		    {
			activeNeighborEntry = neighborTable->getNeighborBySAddr(destAddr);

			if(tschSharedLink || queueModule->isEmpty())
			{
			    BE = mpib.macMinBE;
			    if(activeNeighborEntry != NULL)
				activeNeighborEntry->setTransDelay(-1); // reset the transmission delay
			}

			if(activeNeighborEntry != NULL) // count the successful transmission
			    activeNeighborEntry->incrNumTxDataAck();
		    }

		    delete frame;   // delete the ACK
		    if(txPkt == txData)
		    {
			if(txData->getFrmCtrl().frameType == Ieee802154e_SCHEDULER_RESPONCE)
			{
			    SCHEDULE_confirm(txData->dup(), true);
			}
			delete txData;
			txData = NULL;
		    }
		    numRxAckPkt++;

		    resetTRX();
		    // finish
		}
	    }
	}
	else    // 802.15.4 ACK frame
	{   //[SR] old version
	    if((txBcnCmd == NULL) && (txBcnCmdUpper == NULL) && (txData == NULL) && (txGTS == NULL))
	    {
		EV << "[MAC]: no pending transmission task is waiting for this ACK, drop it!" << endl;
		delete frame;
		return;
	    }

	    if((txPkt != txBcnCmd) && (txPkt != txBcnCmdUpper) && (txPkt != txData) && (txPkt != txGTS))
	    // ack received after corresponding task has failed duo to reaching max retries
	    {
		EV << "[MAC]: this is a late ACK received after corresponding task has failed, drop it!" << endl;
		delete frame;
		return;
	    }

	    //check the sequence number in the ACK to see if it matches that in the <txPkt>
	    if(frame->getSeqNmbr() != check_and_cast<Ieee802154eFrame *>(txPkt)->getSeqNmbr())
	    {
		EV << "[MAC]: the SeqNumber in the ACK does not match, drop it!" << endl;
		delete frame;
		return;
	    }

	    if(ackTimeoutTimer->isScheduled()) // ACK arrives before ACK timeout expires
	    {
		numRxAckPkt++;
		EV << "[MAC]: the ACK arrives before timeout, cancel timeout timer" << endl;
		cancelEvent(ackTimeoutTimer);
		// reset retry counter
		if(txPkt == txBcnCmd)
		    numBcnCmdRetry = 0;
		else if(txPkt == txBcnCmdUpper)
		    numBcnCmdUpperRetry = 0;
		else if(txPkt == txData)
		    numDataRetry = 0;
		else if(txPkt == txGTS)
		    numGTSRetry = 0;
	    }
	    else
	    {
		//only handle late ACK for data packet not in GTS
		if(txPkt != txData)
		{
		    EV << "[MAC]: this is a late ACK, but not for a DATA pkt, drop it!" << endl;
		    delete frame;
		    return;
		}

		if(backoffStatus == 99)
		{
		    EV << "[MAC]: this is a late ACK for " << txData->getName() << ":#" << (int)txData->getSeqNmbr() << ", stop retrying" << endl;
		    backoffStatus = 0;
		    csmacaCancel();
		}
	    }

	    // TODO
	    /*
	     // If ack is for data request cmd (through data poll primitive), set pending flag for data polling
	     if (txPkt == txBcnCmdUpper)
	     if ((taskP.taskStatus(TP_mlme_poll_request))
	     && (strcmp(taskP.taskFrFunc(TP_mlme_poll_request),__FUNCTION__) == 0))
	     {
	     frmCtrl = frame->getFrmCtrl();
	     taskP.mlme_poll_request_pending = frmCtrl.frmPending;   // indicating whether a data is pending at the coordinator for me
	     }*/

	    dispatch(phy_SUCCESS, __FUNCTION__);

	    delete frame;
	}
    }
}

/**@author: 2014    Stefan Reis     (modified) */
//void Ieee802154eMac::handleCommand80215(Ieee802154eFrame* frame) // TODO: include the 802.15.4e Command Frame [SR]
//{
//    if(frame->getFrmCtrl().frameVersion == Ieee802154_compatible) // 802.15.4e Command frame
//    {
//	// TODO: include the 802.15.4e Command Frame [SR]
//    }
//    else    // 802.15.4 Command frame
//    {
//	bool ackReq = false; // flag indicating if cmd needs to be released at the end of this function
//
//	Ieee802154eCmdFrame* tmpCmd = check_and_cast<Ieee802154eCmdFrame *>(frame);
//
//	// all cmd pkts requiring ACK are put in rxCmd and
//	// will be handled by <handleIfsTimer()> after the transmission of ACK and delay of ifs
//	// other cmd pkts are processed here
//	switch(tmpCmd->getCmdType())
//	{
//	    case ASSOCIATION_REQUEST:    //Association request
//		ASSERT(rxCmd == NULL);
//		rxCmd = check_and_cast<Ieee802154eCmdFrame *>(frame);
//		ackReq = true;
//		break;
//
//	    case ASSOCIATION_RESPONSE:   //Association response
//		// TODO
//		/*ASSERT(rxCmd == NULL);
//		 rxCmd = frame;
//		 ackReq = true;
//		 wph = HDR_LRWPAN(p);
//		 rt_myNodeID = *((UINT_16 *)wph->MSDU_Payload);
//		 #ifdef ZigBeeIF
//		 sscs->setGetClusTreePara('g',p);
//		 #endif*/
//		break;
//
//	    case DISASSOCIATION_NOTIFICATION:    //Disassociation notification
//		// TODO
//		break;
//
//	    case DATA_REQUEST:   //Data request
//		ASSERT(rxCmd == NULL);
//		rxCmd = check_and_cast<Ieee802154eCmdFrame *>(frame);
//		ackReq = true;
//		break;
//
//	    case PAN_ID_CONFLICT_NOTIFICATION:    //PAN ID conflict notification
//		// TODO
//		break;
//
//	    case ORPHAN_NOTIFICATION:    //Orphan notification
//		// TODO
//		/*wph = HDR_LRWPAN(p);
//		 sscs->MLME_ORPHAN_indication(wph->MHR_SrcAddrInfo.addr_64,false,0);*/
//		break;
//
//	    case BEACON_REQUEST: //Beacon request
//		/*if (capability.FFD                        //I am an FFD
//		 && (mpib.macAssociationPermit)                 //association permitted
//		 && (mpib.macShortAddress != 0xffff)                //allow to send beacons
//		 && (mpib.macBeaconOrder == 15))                //non-beacon enabled mode
//		 {
//		 //send a beacon using unslotted CSMA-CA
//		 ASSERT(rxCmd == NULL);
//		 txBcnCmd = Packet::alloc();
//		 if (!txBcnCmd) break;
//		 wph = HDR_LRWPAN(txBcnCmd);
//		 frmCtrl.FrmCtrl = 0;
//		 frmCtrl.setFrmType(defFrmCtrl_Type_Beacon);
//		 frmCtrl.setSecu(secuBeacon);
//		 frmCtrl.setFrmPending(false);
//		 frmCtrl.setAckReq(false);
//		 frmCtrl.setDstAddrMode(defFrmCtrl_AddrModeNone);
//		 if (mpib.macShortAddress == 0xfffe)
//		 {
//		 frmCtrl.setSrcAddrMode(defFrmCtrl_AddrMode64);
//		 wph->MHR_SrcAddrInfo.panID = mpib.macPANId;
//		 wph->MHR_SrcAddrInfo.addr_64 = aExtendedAddress;
//		 }
//		 else
//		 {
//		 frmCtrl.setSrcAddrMode(defFrmCtrl_AddrMode16);
//		 wph->MHR_SrcAddrInfo.panID = mpib.macPANId;
//		 wph->MHR_SrcAddrInfo.addr_16 = mpib.macShortAddress;
//		 }
//		 sfSpec.SuperSpec = 0;
//		 sfSpec.setBO(15);
//		 sfSpec.setBLE(mpib.macBattLifeExt);
//		 sfSpec.setPANCoor(isPANCoor);
//		 sfSpec.setAssoPmt(mpib.macAssociationPermit);
//		 wph->MSDU_GTSFields.spec = 0;
//		 wph->MSDU_PendAddrFields.spec = 0;
//		 wph->MSDU_PayloadLen = 0;
//		 #ifdef ZigBeeIF
//		 sscs->setGetClusTreePara('s',txBcnCmd);
//		 #endif
//		 constructMPDU(4,txBcnCmd,frmCtrl.FrmCtrl,mpib.macBSN++,wph->MHR_DstAddrInfo,wph->MHR_SrcAddrInfo,sfSpec.SuperSpec,0,0);
//		 hdr_dst((char *)HDR_MAC(txBcnCmd),p802_15_4macSA(p));
//		 hdr_src((char *)HDR_MAC(txBcnCmd),index_);
//		 HDR_CMN(txBcnCmd)->ptype() = PT_MAC;
//		 //for trace
//		 HDR_CMN(txBcnCmd)->next_hop_ = p802_15_4macDA(txBcnCmd);        //nam needs the nex_hop information
//		 p802_15_4hdrBeacon(txBcnCmd);
//		 csmacaBegin('c');
//		 }*/
//		break;
//
//	    case COORDINATOR_REALIGNMENT:    //Coordinator realignment
//		// TODO
//		/*wph = HDR_LRWPAN(p);
//		 frmCtrl.FrmCtrl = wph->MHR_FrmCtrl;
//		 frmCtrl.parse();
//		 if (frmCtrl.dstAddrMode == defFrmCtrl_AddrMode64)       //directed to an orphan device
//		 {
//		 //recv() is in charge of sending ack.
//		 //further handling continues after the transmission of ack.
//		 assert(rxCmd == 0);
//		 rxCmd = p;
//		 ackReq = true;
//		 }
//		 else                                //broadcasted realignment command
//		 if ((wph->MHR_SrcAddrInfo.addr_64 == macCoordExtendedAddress)
//		 && (wph->MHR_SrcAddrInfo.panID == mpib.macPANId))
//		 {
//		 //no specification in the draft as how to handle this packet, so use our discretion
//		 mpib.macPANId = *((UINT_16 *)wph->MSDU_Payload);
//		 mpib.macCoordShortAddress = *((UINT_16 *)(wph->MSDU_Payload + 2));
//		 tmp_ppib.phyCurrentChannel = wph->MSDU_Payload[4];
//		 phy->PLME_SET_request(phyCurrentChannel,&tmp_ppib);
//		 }*/
//		break;
//
//	    case GTS_REQUEST:                //GTS request
//		// TODO
//		break;
//
//	    default:
//		error("Undefined MAC command type (frmType=%d)");
//		break;
//	}
//
//	if(!ackReq)        // all cmds requiring no ACK sink here
//	    delete frame;
//	// other cmds will be released by <handleIfsTimer()>
//    }
//}

/**@author: 2014    Stefan Reis */
void Ieee802154eMac::handleLLDN802154e(Ieee802154eFrame*) // TODO: [SR]
{

}

/**@author: 2014    Stefan Reis */
void Ieee802154eMac::handleMulti802154e(Ieee802154eFrame*) // TODO: [SR]
{

}

/**@author: 2014    Stefan Reis     (modified)
 * perform AMC frame filtering, return true if frame is filtered */
bool Ieee802154eMac::frameFilter(Ieee802154eFrame* frame)
{
    FrameCtrl frmCtrl = frame->getFrmCtrl();
    Ieee802154eFrameType frmType = frmCtrl.frameType;

    /*
     Ieee802154eMacCmdType cmdType;
     if (frmType == Ieee802154e_CMD)
     cmdType = check_and_cast<Ieee802154CmdFrame *>(frame)->getCmdType();
     */

    // First check flag set by PHY layer, COLLISION or
    if(frame->getKind() == COLLISION)
    {
	EV << "[MAC]: frame corrupted due to collision, dropped" << endl;
	numCollision++;
	return true;
    }
//    if(frame->getKind() == BITERROR)
//    {
//	EV << "[MAC]: frame corrupted due to bit error, dropped" << endl;
//	//numCollision++;
//	return true;
//    }
    else if(frame->getKind() == RX_DURING_CCA)
    {
	EV << "[MAC]: frame corrupted due to being received during CCA, dropped" << endl;
	return true;
    }

    /*
     // TODO: check if received during channel scanning
     if (taskP.taskStatus(TP_mlme_scan_request))
     {
     if (taskP.mlme_scan_request_ScanType == ED_SCAN)                            //ED scan, drop all received pkts
     {
     return true;
     }
     else if (((taskP.mlme_scan_request_ScanType == ACTIVE_SCAN)     //Active scan or Passive scan, drop all except for beacon
     ||(taskP.mlme_scan_request_ScanType == PASSIVE_SCAN))
     && (frmType != Ieee802154e_BEACON))
     {
     return true;
     }
     else if ((taskP.mlme_scan_request_ScanType == ORPHAN_SCAN)      //Orphan scan, drop all except for coordinator realignment cmd
     && ((frmType != Ieee802154e_CMD)||(cmdType != Ieee802154e_COORDINATOR_REALIGNMENT)))
     {
     return true;
     }
     }*/

    //perform further filtering only if the PAN is currently not in promiscuous mode
    if(!mpib.macPromiscuousMode)
    {
	//check packet type
	if((frmType != Ieee802154e_BEACON) && (frmType != Ieee802154e_DATA) && (frmType != Ieee802154e_ACK) && (frmType != Ieee802154e_CMD) && (frmType != Ieee802154e_LLDN) && (frmType != Ieee802154e_MULTI) && (frmType != Ieee802154e_ASSOCIATION_REQUEST) && (frmType != Ieee802154e_ASSOCIATION_RESPONCE) && (frmType != Ieee802154e_SCHEDULER_REQUEST) && (frmType != Ieee802154e_SCHEDULER_RESPONCE))
	{
	    return true;
	}

	// for Std 802.15.4e-2012
	if(frmCtrl.frameVersion == Ieee802154_compatible)
	{
	    // Check if message from node in same pan, if yes add the node
	    if(frame->getSrcPanId() == mpib.macPANId || frmType == Ieee802154e_BEACON)
	    {
		updateNeighbor(frame, frmCtrl);
	    }
	    //[SR] old version
	    if((frmType == Ieee802154e_BEACON) // enhanced beacon
	    && (mpib.macPANId != 0xffff)       // associated
		    && (frame->getSrcPanId() != mpib.macPANId)) // PAN id not match
	    {
		return true;
	    }

	    //check dest. PAN ID (beacon has no dest address fields)
	    if((frmCtrl.dstAddrMode == defFrmCtrl_AddrMode16) || (frmCtrl.dstAddrMode == defFrmCtrl_AddrMode64))
	    {
		if((frame->getDstPanId() != 0xffff) // PAN id do not match for other pkts
		&& (frame->getDstPanId() != mpib.macPANId))
		{
		    return true;
		}
	    }

	    //check dest. address
	    if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode8)    // simple address
	    {   // is not my or the broadcast address
		UINT_8 addr = (UINT_8)frame->getDstAddr().getInt();
		if(addr != mpib.macSimpleAddress)
		{
		    return true;
		}
	    }
	    else if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode16) // short address
	    {
		UINT_16 addr = (UINT_16)frame->getDstAddr().getInt();
		if((addr != 0xffff) && (addr != mpib.macShortAddress))
		{
		    return true;
		}
	    }
	    else if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode64) // extended address
	    {   // is not my or the broadcast address
		if(frame->getDstAddr() != aExtendedAddress && !frame->getDstAddr().isBroadcast())
		{
		    return true;
		}
	    }

	    // !!! the ACK filtering will be done in the handleAck() !!!

	    //check for Data/Cmd frame only with source address:: destined for PAN coordinator
	    // temporary solution, consider only star topology
	    if((frmType == Ieee802154e_DATA) || (frmType == Ieee802154e_CMD))
		if(frmCtrl.dstAddrMode == defFrmCtrl_AddrModeNone) // dest address fields not included
		{
		    if(!isPANCoor)
		    {     // not a PAN coordinator
			return true;
		    }
		    /*
		     if (((!capability.FFD)||(numberDeviceLink(&deviceLink1) == 0))  //I am not a coordinator (nor a PAN coordinator)
		     ||(wph->MHR_SrcAddrInfo.panID != mpib.macPANId))
		     {
		     return true;
		     }*/
		}
	}
	else
	{   // for Std 802.15.4-2006 & Std 802.15.4-2011
	    //check source PAN ID for beacon frame (802.15.4 beacon frame)
	    if((frmType == Ieee802154e_BEACON) && (mpib.macPANId != 0xffff) // associated
		    && (frame->getSrcPanId() != mpib.macPANId)) // PAN id not match
	    {
		return true;
	    }

	    //check dest. PAN ID (beacon has no dest address fields)
	    if((frmCtrl.dstAddrMode == defFrmCtrl_AddrMode16) || (frmCtrl.dstAddrMode == defFrmCtrl_AddrMode64))
	    {
		if((frame->getDstPanId() != 0xffff) // PAN id do not match for other pkts
		&& (frame->getDstPanId() != mpib.macPANId))
		{
		    return true;
		}
	    }

	    //check dest. address
	    if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode16)   // short address
	    {
		UINT_16 addr = (UINT_16)frame->getDstAddr().getInt();
		if((addr != 0xffff) && (addr != mpib.macShortAddress))
		{
		    return true;
		}
	    }
	    else if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode64) // extended address
	    {   // is not my or the broadcast address
		if(frame->getDstAddr() != mpib.macExtendedAddress && !frame->getDstAddr().isBroadcast())
		{
		    return true;
		}
	    }

	    // !!! the ACK filtering will be done in the handleAck() !!!

	    //check for Data/Cmd frame only with source address:: destined for PAN coordinator
	    // temporary solution, consider only star topology
	    if((frmType == Ieee802154e_DATA) || (frmType == Ieee802154e_CMD))
		if(frmCtrl.dstAddrMode == defFrmCtrl_AddrModeNone) // dest address fields not included
		{
		    if(!isPANCoor)
		    {     // not a PAN coordinator
			return true;
		    }
		    /*
		     if (((!capability.FFD)||(numberDeviceLink(&deviceLink1) == 0))  //I am not a coordinator (nor a PAN coordinator)
		     ||(wph->MHR_SrcAddrInfo.panID != mpib.macPANId))
		     {
		     return true;
		     }*/
		}
	}
    }
    return false;
}

/**@author: 2014    Stefan Reis     (modified)
 * called by <handleLowerMsg()> if the received pkt requires an ACK
 * this function constructs an ACK and puts it in txAck */
void Ieee802154eMac::constructACK(Ieee802154eFrame* rxFrame)
{
    Ieee802154eFrame* tmpframe = check_and_cast<Ieee802154eFrame *>(rxFrame);

    int i;
    FrameCtrl origFrmCtrl, ackFrmCtrl;
    origFrmCtrl = tmpframe->getFrmCtrl();

    /*#ifdef test_802154_INDIRECT_TRANS
     //if it is a data request command, then need to check if there is any packet pending.
     //In implementation, we may not have enough time to check if packets pending. If this is the case,
     //then the pending flag in the ack. should be set to 1, and then send a zero-length data packet
     //if later it turns out there is no packet actually pending.
     //In simulation, we assume having enough time to determine the pending status -- so zero-length packet will never be sent.
     //(refer to page 155, line 46-50)
     if ((origFrmCtrl.frmType == Ieee802154e_CMD)     //command packet
     && (check_and_cast<Ieee802154eMacCmdType *>(rxFrame)->getCmdType() == Ieee802154e_DATA_REQUEST))      //data request command
     {
     i = updateTransacLink(tr_oper_est, &transacLink1, &transacLink2, origFrmCtrl.srcAddrMode, rxFrame->getSrcAddr());
     // if pending pkt found in transaction list, return 0
     }
     else
     #endif*/
    i = 1;

    if(mpib.macTSCHenabled)
    { // enhanced Acknowledgment - see IEEE Std 802.15.4e-2012 (5.2.2.3 Acknowledgment frame format) page 66
	ackFrmCtrl.frameType = Ieee802154e_ACK;
	ackFrmCtrl.securityEnabled = secuData;
	ackFrmCtrl.frmPending = (i == 0) ? true:false;
	ackFrmCtrl.ackReq = false; // default - see Std 802.15.4-2006 (7.5.6.4) page 189
	ackFrmCtrl.compPanID = origFrmCtrl.compPanID; // use the same settings as the data frame
	ackFrmCtrl.seqNbSup = origFrmCtrl.seqNbSup; // use the same settings as the data frame
	ackFrmCtrl.dstAddrMode = origFrmCtrl.srcAddrMode; // use the same settings as the data frame
	ackFrmCtrl.frameVersion = Ieee802154_compatible; // for IEEE Std 802.15.4e-2012
	ackFrmCtrl.srcAddrMode = origFrmCtrl.dstAddrMode; // use the same settings as the data frame

	Ieee802154eEnhancedAckFrame* tmpEAck = new Ieee802154eEnhancedAckFrame();
	tmpEAck->setName("Ieee802154ACK");
	tmpEAck->setSeqNmbr(ackFrmCtrl.seqNbSup ? 0:tmpframe->getSeqNmbr()); // copy from original frame

	IE3ADDR tmpDstAddr;
	IE3ADDR tmpSrcAddr;
	UINT_16 tmpDstPanId;
	UINT_16 tmpSrcPanId;

	if(ackFrmCtrl.dstAddrMode == defFrmCtrl_AddrModeNone)
	{ // no address
	    tmpDstPanId = 0;
	    tmpDstAddr = (IE3ADDR)0xffff;
	}
	else if(ackFrmCtrl.dstAddrMode == defFrmCtrl_AddrMode8)
	{ // 8 bit address
	    tmpDstPanId = rxFrame->getSrcPanId();
	    tmpDstAddr = IE3ADDR((UINT_8)rxFrame->getSrcAddr().getInt());
	}
	else if(ackFrmCtrl.dstAddrMode == defFrmCtrl_AddrMode16)
	{ // 16 bit address
	    tmpDstPanId = rxFrame->getSrcPanId();
	    tmpDstAddr = IE3ADDR((UINT_16)rxFrame->getSrcAddr().getInt());
	}
	else
	{ // 64 bit address
	    tmpDstPanId = rxFrame->getSrcPanId();
	    tmpDstAddr = rxFrame->getSrcAddr();
	}

	if(ackFrmCtrl.srcAddrMode == defFrmCtrl_AddrModeNone)
	{ // no address
	    tmpSrcPanId = 0;
	    tmpSrcAddr = (IE3ADDR)0xffff;
	}
	else if(ackFrmCtrl.srcAddrMode == defFrmCtrl_AddrMode8)
	{ // 8 bit address
	    tmpSrcPanId = mpib.macPANId;
	    tmpSrcAddr = (IE3ADDR)mpib.macSimpleAddress;
	}
	else if(ackFrmCtrl.srcAddrMode == defFrmCtrl_AddrMode16)
	{ // 16 bit address
	    tmpSrcPanId = mpib.macPANId;
	    tmpSrcAddr = (IE3ADDR)mpib.macShortAddress;
	}
	else
	{ // 64 bit address
	    tmpSrcPanId = mpib.macPANId;
	    tmpSrcAddr = (IE3ADDR)mpib.macExtendedAddress;
	}

	tmpEAck->setDstPanId(tmpDstPanId);
	tmpEAck->setDstAddr(tmpDstAddr);
	tmpEAck->setSrcPanId(tmpSrcPanId);
	tmpEAck->setSrcAddr(tmpSrcAddr);

	// security is not implemented
	AuxiliarySecurityHeader auxSecHd;
	auxSecHd.secLvl = ASH_SLVL_NONE;
	auxSecHd.keyIdentMode = 0;
	auxSecHd.keySource = 0;
	auxSecHd.keyIndex = 0;

	// IE list
	// ACK/NACK time correction IE - Std 802.15.4e-2012 (5.2.4.11 ACK/NACK time correction IE) page 88
	IE_ACK_NACK_TIME_CORREC* ackNack = new IE_ACK_NACK_TIME_CORREC;

	// the time error calculation is done at the receiving of the data pkt
	if(mpib.macNoHLBuffers) //send a NACK back - this will be set by the higher layer, if there a lack of buffers
	    timeError += 0x8000;

	ackNack->timeSyncInfo = timeError;

	// construct the IE Header list
	std::vector<IE_Header*> tmpIEHlist;

	IE_Header tmpIEHeader1;
	tmpIEHeader1.ieLenght = 2;
	tmpIEHeader1.elementID = IE_HD_ACK_NACK_TIME_CORRECTION;
	tmpIEHeader1.typeDescriptor = false;
	tmpIEHeader1.ieACKNACKTimeCorrc = ackNack;

	tmpIEHlist.push_back(&tmpIEHeader1);

	// List Termination 1
	IE_Header tmpIEHeaderEnd;
	tmpIEHeaderEnd.ieLenght = 0;
	tmpIEHeaderEnd.elementID = IE_HD_LIST_TERMINATION_1;
	tmpIEHeaderEnd.typeDescriptor = false;

	tmpIEHlist.push_back(&tmpIEHeaderEnd);

	tmpEAck->setIeHeaderArraySize(tmpIEHlist.size());

	for(unsigned int i = 0; i < tmpIEHlist.size(); i++)
	{
	    tmpEAck->setIeHeader(i, *tmpIEHlist[i]);
	}

	// construct the IE Payload list
	std::vector<IE_Payload*> tmpIEPlist;

	//no IE payload in a ACK

	// List Termination 1
	IE_Payload tmpIEPEnd;
	tmpIEPEnd.ieLenght = 0;
	tmpIEPEnd.groupID = IE_PL_LIST_TERMINATION;
	tmpIEPEnd.typeDescriptor = true;

	tmpIEPlist.push_back(&tmpIEPEnd);

	tmpEAck->setIePayloadArraySize(tmpIEPlist.size());
	for(unsigned int i = 0; i < tmpIEPlist.size(); i++)
	{
	    tmpEAck->setIePayload(i, *tmpIEPlist[i]);
	}

	ackFrmCtrl.presIElist = true;

	tmpEAck->setFrmCtrl(ackFrmCtrl);
	tmpEAck->setAuxSecHd(auxSecHd);
	tmpEAck->setByteLength(calFrmByteLength(tmpEAck));

	if(!txAck)
	    ASSERT(!txAck); //it's impossible to receive the second packet before the Ack has been sent out.
	else
	    ASSERT(!txAck);
	txAck = tmpEAck;
    }
    else
    {
	// construct frame control field - Std 802.15.4-2011 (figure 47) page 65
	ackFrmCtrl.frameType = Ieee802154e_ACK;
	ackFrmCtrl.securityEnabled = secuData;
	ackFrmCtrl.frmPending = (i == 0) ? true:false;
	ackFrmCtrl.ackReq = false; // default - see Std 802.15.4-2006 (7.5.6.4) page 189
	ackFrmCtrl.compPanID = false;    // default: false
	ackFrmCtrl.seqNbSup = false;    // default: false
	ackFrmCtrl.frmPending = false;    // has no IE lists
	ackFrmCtrl.dstAddrMode = defFrmCtrl_AddrModeNone; // dst address fields empty
	ackFrmCtrl.frameVersion = Ieee802154_2006_introduced;
	ackFrmCtrl.srcAddrMode = defFrmCtrl_AddrModeNone; // src address fields empty

	Ieee802154eAckFrame* tmpAck = new Ieee802154eAckFrame();
	tmpAck->setName("Ieee802154ACK");
	tmpAck->setFrmCtrl(ackFrmCtrl);
	tmpAck->setSeqNmbr(tmpframe->getSeqNmbr());  // copy from original frame
	tmpAck->setByteLength(calFrmByteLength(tmpAck)); // constant size for ACK

//        if (tmpframe->getIsGTS())
//            tmpAck->setIsGTS(true);

	ASSERT(!txAck); //it's impossible to receive the second packet before the Ack has been sent out.
	txAck = tmpAck;
    }
}

/**@author: 2014    Stefan Reis     (modified)
 * brief: construct a Beacon and transmit it to the upper layer
 * called by <handleBcnTxTimer()> if a Beacon is to transmit
 * note: see Std 802.15.4-2011 (figure 38) page 61 */
void Ieee802154eMac::constructBCN()
{
    FrameCtrl frmCtrl;

    if(mpib.macTSCHenabled && mpib.macUseEnhancedBeacon)
    {
	//--- construct a enhanced beacon --- Std 802.15.4e-2012 (figure 40a) page 64
	Ieee802154EnhancedBeaconFrame* tmpEBcn = new Ieee802154EnhancedBeaconFrame();
	tmpEBcn->setName("Ieee802154BEACON");
	// construct frame control field
	frmCtrl.frameType = Ieee802154e_BEACON;
	frmCtrl.securityEnabled = secuBeacon;
	//If a broadcast data or command frame is pending
	frmCtrl.frmPending = (txBroadCmd == NULL) ? false:true;
	frmCtrl.ackReq = false;

	frmCtrl.seqNbSup = false;   // send Sequence number

	/**
	 * 5.2.4.5 MLME Information Elements - 2012 page 82
	 *      Figure 48r�Format of the nested MLME IE of type short - 2012 page 83
	 *          5.2.4.13 TSCH synchronization IE - 2012 page 90 *** The TSCH Synchronization IE is used to construct enhanced beacons that allow new devices to synchronize to a TSCH PAN.
	 *          5.2.4.14 TSCH slotframe and link IE - 2012 page 90 *** The TSCH Slotframe and Link IE is used to construct enhanced beacons that allow new devices to synchronize to a TSCH PAN.
	 *          5.2.4.15 TSCH timeslot IE - 2012 page 91 *** The TSCH Timeslot IE is used in constructing enhanced beacons to allow new devices to synchronize to a TSCH PAN.
	 *
	 *       Figure 48s�Format of the nested MLME IE of type long
	 *          5.2.4.16 Channel hopping IE - 2012 page 92
	 */

	/** When the beacon is generated in response to an EBR, the Frame Version field shall
	 * be set to 0b10 (enhanced beacon), and the Destination Address field shall contain
	 * the source address contained in the received EBR. */
	frmCtrl.dstAddrMode = defFrmCtrl_AddrModeNone;
	frmCtrl.frameVersion = Ieee802154_compatible;

	if(mpib.macEBAutoSA == 0)  // NONE Address
	    frmCtrl.srcAddrMode = defFrmCtrl_AddrModeNone;
	else if(mpib.macEBAutoSA == 1) // SHORT Address
	    frmCtrl.srcAddrMode = defFrmCtrl_AddrMode16;
	else if(mpib.macEBAutoSA == 2) // FULL Address
	    frmCtrl.srcAddrMode = defFrmCtrl_AddrMode64;

	// enhanced beacon sequence number
	tmpEBcn->setSeqNmbr(mpib.macEBSN++);

	IE3ADDR tmpDstAddr;
	IE3ADDR tmpSrcAddr;
	UINT_16 tmpDstPanId;
	UINT_16 tmpSrcPanId;

	//TODO:[SR] here must copy the address of EBR
	if(frmCtrl.dstAddrMode == defFrmCtrl_AddrModeNone)
	{ // no address
	    tmpDstPanId = 0;
	    tmpDstAddr = (IE3ADDR)0xffff;
	}
	else if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode8)
	{ // 8 bit address
	    tmpDstPanId = 0;
	    tmpDstAddr = (IE3ADDR)0xffff;
	}
	else if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode16)
	{ // 16 bit address
	    tmpDstPanId = 0;
	    tmpDstAddr = (IE3ADDR)0xffff;
	}
	else
	{ // 64 bit address
	    tmpDstPanId = 0;
	    tmpDstAddr = (IE3ADDR)0xffff;
	}

	if(frmCtrl.srcAddrMode == defFrmCtrl_AddrModeNone)
	{ // no address
	    tmpSrcPanId = 0;
	    tmpSrcAddr = (IE3ADDR)0xffff;
	}
	else if(frmCtrl.srcAddrMode == defFrmCtrl_AddrMode8)
	{ // 8 bit address
	    tmpSrcPanId = mpib.macPANId;
	    tmpSrcAddr = (IE3ADDR)mpib.macSimpleAddress;
	}
	else if(frmCtrl.srcAddrMode == defFrmCtrl_AddrMode16)
	{ // 16 bit address
	    tmpSrcPanId = mpib.macPANId;
	    tmpSrcAddr = (IE3ADDR)mpib.macShortAddress;
	}
	else
	{ // 64 bit address
	    tmpSrcPanId = mpib.macPANId;
	    tmpSrcAddr = (IE3ADDR)mpib.macExtendedAddress;
	}

	tmpEBcn->setDstPanId(tmpDstPanId);
	tmpEBcn->setDstAddr(tmpDstAddr);
	tmpEBcn->setSrcPanId(tmpSrcPanId);
	tmpEBcn->setSrcAddr(tmpSrcAddr);

	frmCtrl.compPanID = getPANIDComp(frmCtrl, tmpEBcn->getSrcPanId(), tmpEBcn->getDstPanId());

	// security is not implemented
	AuxiliarySecurityHeader auxSecHd;
	auxSecHd.secLvl = ASH_SLVL_NONE;
	auxSecHd.keyIdentMode = 0;
	auxSecHd.keySource = 0;
	auxSecHd.keyIndex = 0;

	//TODO [SR] Beacon Payload field

	//----- IE field------
	// control the length, if the IE fit in the aMaxPHYPacketSize
	frmCtrl.presIElist = false;
	tmpEBcn->setFrmCtrl(frmCtrl);
	tmpEBcn->setAuxSecHd(auxSecHd);
	int lengthNoIE = calFrmByteLength(tmpEBcn);

	//----- construct the IE's -----
	// IE Header field
	std::vector<IE_Header *> tmpIEHlist;

	// List Termination 1
	IE_Header tmpIEHend;
	tmpIEHend.ieLenght = 0;
	tmpIEHend.elementID = IE_HD_LIST_TERMINATION_1;
	tmpIEHend.typeDescriptor = false;
	tmpIEHlist.push_back(&tmpIEHend);

	// IE Payload field
	std::vector<IE_Payload*> tmpIEPlist;

	// construct IE Payload - MLME for TSCH mode
	IE_Payload payldIE;
	payldIE.groupID = IE_PL_MLME;
	payldIE.typeDescriptor = true;

	int payloadLength = 0;
	std::list<IE_MLME_Sub*> tmpSubMLME;

	// IE MLME - Outer IE descriptor - Std 802.15.4e-2012 (figure 48q) page 82
	IE_MLME* tmpIeMLME = new IE_MLME;

	// TSCH synchronization IE
	IE_MLME_Sub* tmpSynchroIE = new IE_MLME_Sub;
	payloadLength += 6 + 2;
	tmpSynchroIE->ieNestedMLMELength = 6;
	tmpSynchroIE->ieMLMESubID = IE_MLME_SHORT_TSCH_SYNCHRONISATION;
	tmpSynchroIE->ieMLMEtype = false;   // short
	IE_TSCH_SYNC* tmpTSCHSync = new IE_TSCH_SYNC; // Std 802.15.4e-2012 (5.2.4.13 TSCH synchronization IE) page 90
	tmpTSCHSync->tschASN = curASN;
	tmpTSCHSync->joinPriotity = 0;    // for PAN coordinator
	tmpSynchroIE->ieTSCHSync = tmpTSCHSync;

	tmpSubMLME.push_back(tmpSynchroIE);

	// TSCH slotframe and link IE - Std 802.15.4e-2012 (5.2.4.14 TSCH slotframe and link IE) page 90
	IE_MLME_Sub* tmpSlotIE = new IE_MLME_Sub;
	tmpSlotIE->ieMLMESubID = IE_MLME_SHORT_TSCH_SLOTFRAME_LINK;
	tmpSlotIE->ieMLMEtype = false;   // short
	SlotframeLinkIE* tmpTSCHSlot = new SlotframeLinkIE(slotframeTable, linkTable); // Std 802.15.4e-2012 (5.2.4.13 TSCH synchronization IE) page 90
	tmpSlotIE->ieNestedMLMELength = tmpTSCHSlot->fillSlotframeLinkIE();
	tmpSlotIE->ieTSCHSLotfrmLink = tmpTSCHSlot;

	if((lengthNoIE + payloadLength + tmpSlotIE->ieNestedMLMELength + 3) < aMaxPHYPacketSize)
	{
	    payloadLength += tmpSlotIE->ieNestedMLMELength;
	    tmpSubMLME.push_back(tmpSlotIE);
	}

	// TSCH timeslot IE
	// check the length for the next IE field
	if((lengthNoIE + payloadLength + 3) < aMaxPHYPacketSize)
	{
	    IE_MLME_Sub* tmpTimesltIE = new IE_MLME_Sub;
	    bool tmpAll = ((lengthNoIE + payloadLength + 25 + 2) < aMaxPHYPacketSize) ? true:false;
	    payloadLength += tmpAll ? (25 + 2):3;
	    tmpTimesltIE->ieNestedMLMELength = tmpAll ? 25:3; // max length of this IE
	    tmpTimesltIE->ieMLMESubID = IE_MLME_SHORT_TSCH_TIMESLOT;
	    tmpTimesltIE->ieMLMEtype = false;   // short

	    macTimeslotTableEntry* tmpTSCHTimeSlt = new macTimeslotTableEntry; // Std 802.15.4e-2012 (5.2.4.15 TSCH timeslot IE) page 91
	    // get the active timeslot from the timeslotTable
	    timeslotTemplate = timeslotTable->getTemplate(useTimeslotID);
	    tmpTSCHTimeSlt->setTemplateId(useTimeslotID);

	    if(tmpAll)
	    {
		tmpTSCHTimeSlt->setCCAOffset(timeslotTemplate->getCCAOffset());
		tmpTSCHTimeSlt->setCCA(timeslotTemplate->getCCA());
		tmpTSCHTimeSlt->setTxOffset(timeslotTemplate->getTxOffset());
		tmpTSCHTimeSlt->setRxOffset(timeslotTemplate->getRxOffset());
		tmpTSCHTimeSlt->setRxAckDelay(timeslotTemplate->getRxAckDelay());
		tmpTSCHTimeSlt->setTxAckDelay(timeslotTemplate->getTxAckDelay());
		tmpTSCHTimeSlt->setRxWait(timeslotTemplate->getRxWait());
		tmpTSCHTimeSlt->setAckWait(timeslotTemplate->getAckWait());
		tmpTSCHTimeSlt->setRxTx(timeslotTemplate->getRxTx());
		tmpTSCHTimeSlt->setMaxAck(timeslotTemplate->getMaxAck());
		tmpTSCHTimeSlt->setMaxTx(timeslotTemplate->getMaxTx());
		tmpTSCHTimeSlt->setTimeslotLength(timeslotTemplate->getTimeslotLength());
	    }

	    tmpTimesltIE->ieTSCHTimeslt = tmpTSCHTimeSlt;
	    tmpSubMLME.push_back(tmpTimesltIE);

	    IE_MLME_Sub* tmphopping = new IE_MLME_Sub;
	    tmpAll = ((lengthNoIE + payloadLength + 25 + 2) < aMaxPHYPacketSize) ? true:false;
	    payloadLength += tmpAll ? (25 + 2):3;

	    tmphopping->ieNestedMLMELength = tmpAll ? 25:3; // max length of this IE
	    tmphopping->ieMLMESubID = IE_MLME_LONG_CHANNEL_HOPPING_SEQUENCE;
	    tmphopping->ieMLMEtype = true;   // short

	    if(tmpAll)
	    {
		IE_CHANNEL_HOPPING *tmpChannel = new IE_CHANNEL_HOPPING;
		tmpChannel->channelPage = hoppingSequenceList->getEntry(0)->getChannelPage();
		tmpChannel->currentHop = hoppingSequenceList->getEntry(0)->getCurrentHop();
		tmpChannel->extBitmap = hoppingSequenceList->getEntry(0)->getExtendedBitmap();
		tmpChannel->hoppingSeqID = hoppingSequenceList->getEntry(0)->getHoppingSequenceId();
		tmpChannel->hoppingSeqLength = hoppingSequenceList->getEntry(0)->getHoppingSequenceLength();
		tmpChannel->hoppingSeqLst = hoppingSequenceList->getEntry(0)->getHoppingSequenceList();
		tmpChannel->numberOfChannels = hoppingSequenceList->getEntry(0)->getNumberOfChannels();
		tmpChannel->phyConfig = hoppingSequenceList->getEntry(0)->getPhyConfiguration();

		tmphopping->ieChannelHop = tmpChannel;
		tmpSubMLME.push_back(tmphopping);
	    }

	}
	tmpIeMLME->ieMLMESub = tmpSubMLME;
	payldIE.ieLenght = payloadLength++;
	payldIE.ieMLME = tmpIeMLME;

	tmpIEPlist.push_back(&payldIE);

	frmCtrl.presIElist = ((lengthNoIE + payloadLength) <= aMaxPHYPacketSize) ? true:false;

	if(frmCtrl.presIElist)
	{
	    // IE Header
	    tmpEBcn->setIeHeaderArraySize(tmpIEHlist.size());
	    for(unsigned int i = 0; i < tmpIEHlist.size(); i++)
	    {
		tmpEBcn->setIeHeader(i, *tmpIEHlist[i]);
	    }
	    // IE Payload
	    tmpEBcn->setIePayloadArraySize(tmpIEPlist.size());
	    for(unsigned int i = 0; i < tmpIEPlist.size(); i++)
	    {
		tmpEBcn->setIePayload(i, *tmpIEPlist[i]);
	    }
	}

	tmpEBcn->setFrmCtrl(frmCtrl);
	tmpEBcn->setByteLength(calFrmByteLength(tmpEBcn));

	txBeacon = tmpEBcn; // released in taskSuccess or in PD_DATA_confirm (if tx failure)

	if(mpib.macTSCHenabled)
	{ // save in the mac queue
	  //send(tmpEBcn, mQueueOut);
	    queueModule->insertInQueue(tmpEBcn);
	}
	else
	{
	    txPkt = tmpEBcn;
	    mpib.macBeaconTxTime = SIMTIME_DBL(simTime());
	    sendDown(check_and_cast<Ieee802154eFrame *>(txBeacon->dup())); // no delay
	}
    }
    else
    {
	//--- construct a beacon --- Std 802.15.4-2011 (figure 38) page 61
	Ieee802154eBeaconFrame* tmpBcn = new Ieee802154eBeaconFrame();
	tmpBcn->setName("Ieee802154BEACON");

	// construct frame control field
	frmCtrl.frameType = Ieee802154e_BEACON;
	frmCtrl.securityEnabled = secuBeacon;

	// check if some data is pending for a device or is it the first beacon
	// Pending Address Specification field - Std 802.15.4-2011 (figure 45) page 64
	PendingAddrFields tmpPAF;

	UINT_8 numShortAddr = 0;
	UINT_8 numExtendedAddr = 0; // [SR] only short addresses are stored in the GTS list
	for(int i = 0; i < 7; i++)
	{
	    if(gtsList[i].isTxPending == true)
	    {
		frmCtrl.frmPending = true;

		if(gtsList[i].devShortAddr != 0xffff) // is no broadcast address - for the Pending Address Specification field
		    tmpPAF.addrList[numShortAddr++] = (IE3ADDR)gtsList[i].devShortAddr;
	    }
	}

	tmpPAF.numShortAddr = numShortAddr;
	tmpPAF.numExtendedAddr = numExtendedAddr;

	//[SR] TODO frame pending
	//If a broadcast data or command frame is pending
	frmCtrl.frmPending = false; // see Std 802.15.4-2011 (5.2.1.1.3) page 58

	frmCtrl.ackReq = false;     // ignored upon reception
	frmCtrl.frameVersion = Ieee802154_2003_compatible;

	frmCtrl.seqNbSup = false;   // send Sequence number
	frmCtrl.presIElist = false;

	frmCtrl.dstAddrMode = defFrmCtrl_AddrModeNone; // 0x00, ignored upon reception

	if(mpib.macShortAddress == 0xfffe)
	{
	    frmCtrl.srcAddrMode = defFrmCtrl_AddrMode64;
	    tmpBcn->setSrcAddr(mpib.macExtendedAddress);
	}
	else
	{
	    frmCtrl.srcAddrMode = defFrmCtrl_AddrMode16;
	    tmpBcn->setSrcAddr(MACAddress(mpib.macShortAddress));
	}

	tmpBcn->setSeqNmbr(mpib.macBSN++);
	tmpBcn->setDstPanId(0);     // ignored upon reception
	tmpBcn->setDstAddr(MACAddress::UNSPECIFIED_ADDRESS); // ignored upon reception
	tmpBcn->setSrcPanId(mpib.macPANId);

	frmCtrl.compPanID = getPANIDComp(frmCtrl, tmpBcn->getSrcPanId(), tmpBcn->getDstPanId());

	// construct superframe specification
	// Beacon order
	txSfSpec.BO = mpib.macBeaconOrder;
	txSfSpec.BI = aBaseSuperframeDuration * (1 << mpib.macBeaconOrder);
	// superframe order
	txSfSpec.SO = mpib.macSuperframeOrder;
	txSfSpec.SD = aBaseSuperframeDuration * (1 << mpib.macSuperframeOrder);
	// this parameter may vary each time when new GTS slots were allocated in last superframe
	txSfSpec.finalCap = tmp_finalCap;
	txSfSpec.battLifeExt = mpib.macBattLifeExt;
	txSfSpec.panCoor = isPANCoor;
	txSfSpec.assoPmt = mpib.macAssociationPermit;

	tmpBcn->setFrmCtrl(frmCtrl);
	tmpBcn->setSfSpec(txSfSpec);

	// GTS Specification field - for GTS allocation initiated by a device - Std 802.15.4-2011 (figure 30) page 50
	GTSFields tmpGts;
	tmpGts.gtsDescrCnt = gtsCount;
	tmpGts.gtsPermit = mpib.macGTSPermit;
	tmpGts.gtsDirMask = 0; // this is set by the omnetpp.ini (isRecvGTS)
	//tmpGts.gtsDescr
	tmpBcn->setGtsField(tmpGts);

	tmpBcn->setPendingAdrrFld(tmpPAF);

	//TODO [SR] Beacon Payload field - Std 802.15.4-2011 (5.2.2.1.8) page 65

	tmpBcn->setByteLength(calFrmByteLength(tmpBcn));

	txBeacon = tmpBcn; // released in taskSuccess or in PD_DATA_confirm (if tx failure)
	txPkt = tmpBcn;
	mpib.macBeaconTxTime = SIMTIME_DBL(simTime());      // no delay
	sendDown(check_and_cast<Ieee802154eFrame *>(txBeacon->dup()));
    }
}

//-------------------------------------------------------------------------------/
/***************************** <Radio State Control> ****************************/
//-------------------------------------------------------------------------------/
void Ieee802154eMac::resetTRX()
{
    PHYenum t_state;
    EV << "[MAC]: reset radio state after a task has completed" << endl;
    if(mpib.macTSCHenabled)
    {
	EV << "[MAC]: it's now in inactive period, should turn off radio and go to sleep" << endl;
	t_state = mpib.macRxOnWhenIdle ? phy_RX_ON:phy_TRX_OFF;
    }
    else
    { //[SR] old version
	if((mpib.macBeaconOrder != 15) || (rxBO != 15))  // beacon enabled
	{
	    if((!inTxSD_txSDTimer) && (!inRxSD_rxSDTimer)) // in inactive portion, go to sleep
	    {
		EV << "[MAC]: it's now in inactive period, should turn off radio and go to sleep" << endl;
		t_state = phy_TRX_OFF;
	    }
	    else if(inTxSD_txSDTimer)      // should not go to sleep
	    {
		EV << "[MAC]: it's now in outgoing active period (as a coordinator), should stay awake and turn on receiver" << endl;
		t_state = phy_RX_ON;
	    }
	    else                       // in rx SD, according to macRxOnWhenIdle
	    {
		EV << "[MAC]: it's now in incoming active period (as a device), whether go to sleep depending on parameter macRxOnWhenIdle" << endl;
		t_state = mpib.macRxOnWhenIdle ? phy_RX_ON:phy_TRX_OFF;
	    }
	}
	else // non-beacon
	{
	    EV << "[MAC]: non-beacon, whether go to sleep depending on parameter macRxOnWhenIdle" << endl;
	    t_state = mpib.macRxOnWhenIdle ? phy_RX_ON:phy_TRX_OFF;
	}
    }

    PLME_SET_TRX_STATE_request(t_state);
}

//-------------------------------------------------------------------------------/
/**************************** <MAC2PHY primitive> *******************************/
//-------------------------------------------------------------------------------/
/**@author: 2014    Stefan Reis     (modified)
 * brief: PD-DATA.request requests the transmission of an MPDU
 *
 * The PD-DATA.request primitive is generated by a local MAC sublayer entity and issued to its PHY entity to
 * request the transmission of an MPDU.
 *
 * note: see Std 802.15.4-2006 (6.2.1.1.1) page 32
 *
 * param[in]      uint        psduLength:     The number of octets contained in the PSDU to be transmitted by the PHY entity
 *                cPacket*    psdu:           The set of octets forming the PSDU to be transmitted by the PHY entity*/
void Ieee802154eMac::PD_DATA_request(UINT_8 psduLength, Ieee802154eFrame * psdu)
{
    EV << "[MAC]: sending PD_DATA_request < PSDU Length " << PK(psdu)->getByteLength() <<"> to PHY layer" << endl;

    if ((psduLength > Ieee802154ePhy::aMaxPHYPacketSize) && (PK(psdu)->getByteLength() > Ieee802154ePhy::aMaxPHYPacketSize))
    error("invalid Packet Length");

//    const char *temp =    getParentModule()->getParentModule()->getFullName();
//    std::string test = temp;
//    if(test == "fixhost[0]")
//    {
//        simtime_t temp1 = simTime();
//        psdu->getEncapsulatedPacket()->getEncapsulatedPacket()->getEncapsulatedPacket()->setTimestamp(temp1);
//    }

    sendDown(psdu);
}

/**@author: 2014    Stefan Reis     (modified)
 * brief: PD-DATA.indication indicates the transfer of an MPDU (i.e., PSDU)
 *
 * The PD-DATA.indication primitive indicates the transfer of an MPDU (i.e., PSDU) from the PHY to the
 * local MAC sublayer entity.
 *
 * note: see Std 802.15.4-2006 (6.2.1.3.1) page 34
 *
 * param[in]      uint        psduLength        The number of octets contained in the PSDU received by the PHY entity
 *                cPacket*    psdu              The set of octets forming the PSDU received by the PHY entity
 *                int         ppduLinkQuality   Link quality (LQI) value measured during reception of the PPDU (see 6.9.8)*/
void Ieee802154eMac::handle_PD_DATA_indication(UINT_8 psduLength, cPacket* psdu, UINT_8 ppduLinkQuality)
{
    // simple handle function
    handleData((Ieee802154eFrame*)psdu);

    //TODO:[SR] handle the LinkQuality
}

//-------------------------------------------------------------------------------/
/************************* <PHY management service> *****************************/
//-------------------------------------------------------------------------------/
/**@author: 2014    Stefan Reis     (modified)
 * brief: PLME-CCA.request requests that the PLME perform a CCA
 *
 * The PLME-CCA.request primitive requests that the PLME perform a CCA as defined in 6.9.9.
 *
 * note: see Std 802.15.4-2006 (6.2.2.1) page 35 */
void Ieee802154eMac::PLME_CCA_request()
{
    // construct PLME-CCA.request primitive
    Ieee802154eMacPhyPrimitives *primitive = new Ieee802154eMacPhyPrimitives();
    primitive->setKind(PLME_CCA_REQUEST);

    send(primitive, mLowerLayerOut);
    EV << "[MAC]: send PLME-CCA.request to PHY layer" << endl;
}

/**@author: 2014    Stefan Reis     (modified)
 * brief: PLME-ED.request requests that the PLME perform an ED measurement
 * The PLME-ED.request primitive requests that the PLME perform an ED measurement (see 6.9.7).
 *
 * note: see Std 802.15.4-2006 (6.2.2.3) page 36 */
void Ieee802154eMac::PLME_ED_request()
{
    // construct PLME-ED.request primitive
    Ieee802154eMacPhyPrimitives *primitive = new Ieee802154eMacPhyPrimitives();
    primitive->setKind(PLME_ED_REQUEST);

    send(primitive, mLowerLayerOut);
    EV << "[MAC]: send PLME-ED.request to PHY layer" << endl;
}

/**@author: 2014    Stefan Reis
 * brief: PLME-GET.request requests information about a given PHY PIB attribute
 *
 * The PLME-GET.request primitive requests information about a given PHY PIB attribute.
 *
 * note: see Std 802.15.4-2006 (6.2.2.5) page 37
 *
 * param[in]    pibAttribute    Enumeration     Description: The identifier of the PHY PIB attribute to get. */
void Ieee802154eMac::PLME_GET_request(PHYPIBenum pibAttribute)
{
    // construct PLME-GET.request primitive
    Ieee802154eMacPhyPrimitives *primitive = new Ieee802154eMacPhyPrimitives();
    primitive->setKind(PLME_GET_REQUEST);
    primitive->setPibAttribute(pibAttribute);

    send(primitive, mLowerLayerOut);
    EV << "[MAC]: send PLME-GET.request to PHY layer" << endl;
}

/**@author: 2014    Stefan Reis     (modified)
 * brief: PLME-SET-TRX-STATE.request requests that the PHY entity change the internal operating state
 *        of the transceiver.
 *
 * The PLME-SET-TRX-STATE.request primitive requests that the PHY entity change the internal operating
 * state of the transceiver.
 * note: see Std 802.15.4-2006 (6.2.2.7) page 39
 *
 * param[in]     PHYenum    state   Description: The new state in which to configure the transceiver. */
void Ieee802154eMac::PLME_SET_TRX_STATE_request(PHYenum state)
{
    // to handle that the MAC layer only send one SET_TRX_STATE msg and wait for the results of this msg,
    // and to prevent the msg if the Phy layer is all ready in this state
    //FIXME:[SR] how to handle FORCE_TRX_OFF or alert msg
    if(!trx_state_req_activ && trx_state_req != state)
    {
	EV << "[MAC]: sending PLME-SET-TRX-STATE.request <" << stateTxt[state] << "> to PHY layer" << endl;

	// construct PLME_SET_TRX_STATE_request primitive
	Ieee802154eMacPhyPrimitives *primitive = new Ieee802154eMacPhyPrimitives();
	primitive->setKind(PLME_SET_TRX_STATE_REQUEST);
	primitive->setStatus(state);

	send(primitive, mLowerLayerOut);

	trx_state_req = state;      // store requested radio state
	trx_state_req_activ = true;
    }
}

/**@author: 2014    Stefan Reis     (modified)
 * brief: PLME-SET.request primitive attempts to set the indicated PHY PIB attribute to the given value
 *
 * The PLME-SET.request primitive attempts to set the indicated PHY PIB attribute to the given value.
 *
 * note: see Std 802.15.4-2006 (6.2.2.9) page 40
 *
 * param[in]     "param1 Input parameter (i.e. read by this function). */
void Ieee802154eMac::PLME_SET_request(PHYPIBenum pibAttribute, PHY_PIB pibAttributeValue)
{
    //[SR] for the PLME-SET.confirm, if the correct PIBAttribute is changed
    ppib_pibAttribure_req = pibAttribute;

    // construct PLME_SET_request primitive
    Ieee802154eMacPhyPrimitives *primitive = new Ieee802154eMacPhyPrimitives();
    primitive->setKind(PLME_SET_REQUEST);
    primitive->setPibAttributeValue(pibAttributeValue);

    switch(pibAttribute)
    {
	case PHY_CURRENT_CHANNEL:
	{
	    primitive->setPibAttribute(PHY_CURRENT_CHANNEL);
	    break;
	}
	case PHY_CHANNELS_SUPPORTED:
	{
	    primitive->setPibAttribute(PHY_CHANNELS_SUPPORTED);
	    break;
	}
	case PHY_TX_POWER_TOLERANCE:
	{
	    primitive->setPibAttribute(PHY_TX_POWER_TOLERANCE);
	    break;
	}
	case PHY_TRANSMIT_POWER:
	{
	    primitive->setPibAttribute(PHY_TRANSMIT_POWER);
	    break;
	}
	case PHY_TX_POWER:
	{
	    primitive->setPibAttribute(PHY_TX_POWER);
	    break;
	}
	case PHY_CCA_MODE:
	{
	    primitive->setPibAttribute(PHY_CCA_MODE);
	    break;
	}
	case PHY_CURRENT_PAGE:
	{
	    primitive->setPibAttribute(PHY_CURRENT_PAGE);
	    break;
	}
	case PHY_MAX_FRAME_DURATION:
	{
	    primitive->setPibAttribute(PHY_MAX_FRAME_DURATION);
	    break;
	}
	case PHY_SHR_DURATION:
	{
	    primitive->setPibAttribute(PHY_SHR_DURATION);
	    break;
	}
	case PHY_SYMBOLS_PER_OCTET:
	{
	    primitive->setPibAttribute(PHY_SYMBOLS_PER_OCTET);
	    break;
	}
	case PHY_PREAMBLE_SYMBOL_LENGTH:
	{
	    primitive->setPibAttribute(PHY_PREAMBLE_SYMBOL_LENGTH);
	    break;
	}
	case PHY_UWB_DATA_RATES_SUPPORTED:
	{
	    primitive->setPibAttribute(PHY_UWB_DATA_RATES_SUPPORTED);
	    break;
	}
	case PHY_CSS_LOW_DATA_RATE_SUPPORTED:
	{
	    primitive->setPibAttribute(PHY_CSS_LOW_DATA_RATE_SUPPORTED);
	    break;
	}
	case PHY_UWB_COU_SUPPORTED:
	{
	    primitive->setPibAttribute(PHY_UWB_COU_SUPPORTED);
	    break;
	}
	case PHY_UWB_CS_SUPPORTED:
	{
	    primitive->setPibAttribute(PHY_UWB_CS_SUPPORTED);
	    break;
	}
	case PHY_UWB_LCP_SUPPORTED:
	{
	    primitive->setPibAttribute(PHY_UWB_LCP_SUPPORTED);
	    break;
	}
	case PHY_UWB_CURRENT_PULSE_SHAPE:
	{
	    primitive->setPibAttribute(PHY_UWB_CURRENT_PULSE_SHAPE);
	    break;
	}
	case PHY_UWB_COU_PULSE:
	{
	    primitive->setPibAttribute(PHY_UWB_COU_PULSE);
	    break;
	}
	case PHY_UWB_CS_PULSE:
	{
	    primitive->setPibAttribute(PHY_UWB_CS_PULSE);
	    break;
	}
	case PHY_UWB_LCP_WEIGHT_1:
	{
	    primitive->setPibAttribute(PHY_UWB_LCP_WEIGHT_1);
	    break;
	}
	case PHY_UWB_LCP_WEIGHT_2:
	{
	    primitive->setPibAttribute(PHY_UWB_LCP_WEIGHT_2);
	    break;
	}
	case PHY_UWB_LCP_WEIGHT_3:
	{
	    primitive->setPibAttribute(PHY_UWB_LCP_WEIGHT_3);
	    break;
	}
	case PHY_UWB_LCP_WEIGHT_4:
	{
	    primitive->setPibAttribute(PHY_UWB_LCP_WEIGHT_4);
	    break;
	}
	case PHY_UWB_LCP_DELAY_2:
	{
	    primitive->setPibAttribute(PHY_UWB_LCP_DELAY_2);
	    break;
	}
	case PHY_UWB_LCP_DELAY_3:
	{
	    primitive->setPibAttribute(PHY_UWB_LCP_DELAY_3);
	    break;
	}
	case PHY_UWB_LCP_DELAY_4:
	{
	    primitive->setPibAttribute(PHY_UWB_LCP_DELAY_4);
	    break;
	}
	case PHY_RANGING:
	{
	    primitive->setPibAttribute(PHY_RANGING);
	    break;
	}
	case PHY_RANGING_CRYSTAL_OFFSET:
	{
	    primitive->setPibAttribute(PHY_RANGING_CRYSTAL_OFFSET);
	    break;
	}
	case PHY_RANGING_DPS:
	{
	    primitive->setPibAttribute(PHY_RANGING_DPS);
	    break;
	}
	case PHY_CURRENT_CODE:
	{
	    primitive->setPibAttribute(PHY_CURRENT_CODE);
	    break;
	}
	case PHY_NATIVE_PRF:
	{
	    primitive->setPibAttribute(PHY_NATIVE_PRF);
	    break;
	}
	case PHY_UWB_SCAN_BINS_PER_CHANNEL:
	{
	    primitive->setPibAttribute(PHY_UWB_SCAN_BINS_PER_CHANNEL);
	    break;
	}
	case PHY_UWB_INSERTED_PREAMBLE_INTERVAL:
	{
	    primitive->setPibAttribute(PHY_UWB_INSERTED_PREAMBLE_INTERVAL);
	    break;
	}
	case PHY_TX_RMARKER_OFFSET:
	{
	    primitive->setPibAttribute(PHY_TX_RMARKER_OFFSET);
	    break;
	}
	case PHY_RX_RMARKER_OFFSET:
	{
	    primitive->setPibAttribute(PHY_RX_RMARKER_OFFSET);
	    break;
	}
	case PHY_RFRAME_PROCESSING_TIME:
	{
	    primitive->setPibAttribute(PHY_RFRAME_PROCESSING_TIME);
	    break;
	}
	case PHY_CCA_DURATION:
	{
	    primitive->setPibAttribute(PHY_CCA_DURATION);
	    break;
	}
	default:
	{
	    error("invalid PHY PIB attribute");
	    break;
	}
    }

    send(primitive, mLowerLayerOut);
}

//[SR] old version
/*
 void Ieee802154eMac::PLME_bitrate_request()
 {
 // construct PLME_CCA_request primitive
 Ieee802154eMacPhyPrimitives *primitive = new Ieee802154eMacPhyPrimitives();
 primitive->setKind(PLME_GET_BITRATE);
 send(primitive, mLowerLayerOut);
 EV << "[MAC]: send PLME_GET_BITRATE to PHY layer" << endl;
 }
 */

//-------------------------------------------------------------------------------/
/************************* <PHY2MAC primitive Handler> **************************/
//-------------------------------------------------------------------------------/
/**@author: 2014    Stefan Reis
 * handle the msg from the PHY layer */
void Ieee802154eMac::handleMacPhyPrimitive(int msgkind, cMessage* msg)
{
    Ieee802154eMacPhyPrimitives* primitive = check_and_cast<Ieee802154eMacPhyPrimitives *>(msg);
    switch(msgkind)
    {
	if(primitive->getBitRate() > 0)
	{
	    bitrate = primitive->getBitRate();
	    phy_bitrate = bitrate;
	}

    case PD_DATA_CONFIRM:
	handle_PD_DATA_confirm(PHYenum(primitive->getStatus()));
	delete primitive;
	break;

    case PD_DATA_INDICATION:
	handle_PD_DATA_indication(primitive->getPsduLength(), (cPacket*)msg, 0); //not implemented LQI (ppduLinkQuality)
	delete primitive;
	break;

	// PHY management service
    case PLME_CCA_CONFIRM:
	handle_PLME_CCA_confirm(PHYenum(primitive->getStatus()));
	delete primitive;
	break;

    case PLME_ED_CONFIRM:
	handle_PLME_ED_confirm(PHYenum(primitive->getStatus()), primitive->getEnergyLevel());
	delete primitive;
	break;

    case PLME_GET_CONFIRM:
	handle_PLME_GET_confirm(PHYenum(primitive->getStatus()), PHYPIBenum(primitive->getPibAttribute()), PHY_PIB(primitive->getPibAttributeValue()));
	delete primitive;
	break;

    case PLME_SET_TRX_STATE_CONFIRM:
	handle_PLME_SET_TRX_STATE_confirm(PHYenum(primitive->getStatus()));
	delete primitive;
	break;

    case PLME_SET_CONFIRM:
	handle_PLME_SET_confirm(PHYenum(primitive->getStatus()), PHYPIBenum(primitive->getPibAttribute()));
	delete primitive;
	break;

    default:
	error("unknown primitive (msgkind=%d)", msgkind);
	break;
    }
}

/**@author: 2014    Stefan Reis     (modified)
 * brief: PD-DATA.confirm reports the results of the PD-DATA.request command
 *
 * The PD-DATA.confirm primitive is generated by the PHY entity and issued to its MAC sublayer entity in
 * response to a PD-DATA.request primitive. The PD-DATA.confirm primitive will return a status of either
 * SUCCESS, indicating that the request to transmit was successful, or an error code of RX_ON, TRX_OFF, or
 * BUSY_TX. The reasons for these status values are fully described in 6.2.1.1.3.
 *
 * note: see Std 802.15.4-2006 (6.2.1.2.1) page 33
 *
 * param[in]      Enumeration     status      Description: The result of the request to transmit a packet */
void Ieee802154eMac::handle_PD_DATA_confirm(PHYenum status)
{
    inTransmission = false;

    if(mpib.macTSCHenabled)
    {
	if(taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 6)
	{
	    if(status == phy_SUCCESS) // successful transmission
	    {
		Ieee802154eFrame* tmpPkt = check_and_cast<Ieee802154eFrame *>(txPkt);
		if(tmpPkt->getFrmCtrl().ackReq)
		{   // Acknowledge required
		    taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 7;
		    EV << "[TSCH CCA]-Transmitter:[7] wait the TsRxAckDelay" << endl;

		    if(tsRxAckDelayTimer->isScheduled())
			cancelEvent(tsRxAckDelayTimer);

		    scheduleAt(simTime() + timeslotTable->getTemplate(useTimeslotID)->getRxAckDelayDbl(), tsRxAckDelayTimer);

		    PLME_SET_TRX_STATE_request(phy_TRX_OFF);
		    // next step at handleTsRxAckDelayTimer()
		}
		else
		{   // no Acknowledge required
		    taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) = false;
		    taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 0;
		    EV << "[TSCH CCA]-Transmitter:[6] frame without Acknowledge required successful transmitted go back to sleep" << endl;

		    if(tsRxAckDelayTimer->isScheduled())
			cancelEvent(tsRxAckDelayTimer);

		    MCPS_DATA_confirm(0, tmpPkt->getArrivalTime().getScale(), false, 0, 0, 0, 0, 0, mac_SUCCESS, tmpPkt->getRetries(), 0, tmpPkt);

		    // set the counter in neighborTable
		    UINT_16 destAddr = getShortAddress(tmpPkt->getDstAddr());
		    if(destAddr != 0xffff) // if is not the broadcast address
		    {
			activeNeighborEntry = neighborTable->getNeighborBySAddr(destAddr);
			if(activeNeighborEntry != NULL)
			    activeNeighborEntry->incrNumTxData();
		    }

		    queueModule->deleteMsgQueue(tmpPkt->getDstAddr(), false);

		    if(txPkt == txBeacon)
		    {
			// delete beacon frame
			beaconWaitingTx = false;
			delete txBeacon;
			txBeacon = NULL;
			numTxBcnPkt++;
		    }
		    else if(txPkt == txData)
		    {
			delete txData;
			txData = NULL;
			numTxDataSucc++;
		    }

		    resetTRX();
		    // finish
		}
	    }
	    else // transmission failed
	    {
		handleTsAckWait();
		// next step at handleTsAckWait()
	    }
	}
	else if(taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) == 9)
	{
	    // cancel TsMaxAckTimer and go back to sleep
	    taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) = false;
	    taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) = 0;

	    if(status == phy_SUCCESS)  // transmission successful
		EV << "[TSCH CCA]-Receiver:[10] successful transmitting of the ACK frame in the TsMacAckTime, go back to sleep" << endl;
	    else
		// transmission failed
		EV << "[TSCH CCA]-Receiver:[10] failed transmission of the ACK frame in the tsMaxAckTime" << endl;

	    // delete the pkt
	    if(txPkt == txBeacon)
	    {
		beaconWaitingTx = false;
		delete txBeacon;
		txBeacon = NULL;
	    }
	    else if(txPkt == txAck)
	    {
		delete txAck;
		txAck = NULL;
		numTxAckPkt++;
	    }
	    else if(txPkt == txData)
	    {
		delete txData;
		txData = NULL;
	    }

	    resetTRX();
	    // finish
	}
	return;
    }

    //[SR] old version
    if(backoffStatus == 1)
	backoffStatus = 0;

    if(status == phy_SUCCESS)
    {
	dispatch(status, __FUNCTION__);
    }
    // sending pkt at phy layer failed, it may happen only when txing is terminated by FORCE_TRX_OFF
    else if(txPkt == txBeacon)
    {
	beaconWaitingTx = false;
	delete txBeacon;
	txBeacon = NULL;
    }
    else if(txPkt == txAck)
    {
	delete txAck;
	txAck = NULL;
    }
    else if(txPkt == txGTS)
    {
	error("[GTS]: GTS transmission failed");
    }
    else //TODO TBD: RX_ON/TRX_OFF -- possible if the transmission is terminated by a FORCE_TRX_OFF or change of channel, or due to energy depletion
    {
    }
}

/**@author: 2014    Stefan Reis     (modified)
 * brief: PLME-CCA.confirm primitive reports the results of a CCA
 *
 * The PLME-CCA.confirm primitive reports the results of a CCA as defined in 6.9.9.
 *
 * note: see Std 802.15.4-2006 (6.2.2.2) page 35
 *
 * param[in]     Enumeration status: The result of the request to transmit a packet */
void Ieee802154eMac::handle_PLME_CCA_confirm(PHYenum status)
{
    if(mpib.macTSCHenabled && taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) == true)
    {
	if(status == phy_IDLE && taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 4)
	{
	    taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 5;
	    EV << "[TSCH CCA]-Transmitter:[5] switch from Rx to Tx state" << endl;
	    PLME_SET_TRX_STATE_request(phy_TX_ON);
	    // the RxTx delay will be done in the PHY layer
	    // next step in handle_PLME_SET_TRX_STATE_confirm()
	}
	else // channel busy or TRX off
	{
	    // cancel the transmission, retry in the next Tx-timeslot to the Dest-address and go back to sleep
	    taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 99;
	    handleTsAckWait();
	}

    }

//[SR] old version
    if(taskP.taskStatus(TP_CCA_CSMACA))
    {
	taskP.taskStatus(TP_CCA_CSMACA) = false;

	// the following from CsmaCA802_15_4::CCA_confirm
	if(status == phy_IDLE)                     // idle channel
	{
	    if(!beaconEnabled)                 // non-beacon, unslotted
	    {
		tmpCsmaca = NULL;
		csmacaCallBack(phy_IDLE);    // unslotted successfully, callback
		return;
	    }
	    else                            // beacon-enabled, slotted
	    {
		CW--;                       // Case C1, beacon-enabled, slotted
		if(CW == 0)                    // Case D1
		{
		    //timing condition may not still hold -- check again
		    if(csmacaCanProceed(0.0, true))    // Case E1
		    {
			tmpCsmaca = 0;
			csmacaCallBack(phy_IDLE); // slotted CSMA-CA successfully
			return;
		    }
		    else // Case E2: postpone until next beacon sent or received
		    {
			CW = 2;
			bPeriodsLeft = 0;
			csmacaWaitNextBeacon = true;    // Debugged
		    }
		}
		else
		    handleBackoffTimer(); // Case D2: perform CCA again, this function sends a CCA_request
	    }
	}
	else    // busy channel
	{
	    if(beaconEnabled)
		CW = 2;              // Case C2
	    NB++;
	    if(NB > mpib.macMaxCSMABackoffs)   // Case F1
	    {
		tmpCsmaca = 0;
		csmacaCallBack(phy_BUSY);
		return;
	    }
	    else    // Case F2: backoff again
	    {
		BE++;
		if(BE > mpib.macMaxBE)
		    BE = mpib.macMaxBE;
		csmacaStart(false);             // not the first time
	    }
	}
    }
}

/**@author: 2014    Stefan Reis
 * brief: PLME-ED.confirm reports the results of the ED measurement
 *
 * The PLME-ED.confirm primitive reports the results of the ED measurement as defined in 6.9.7.
 *
 * note: see Std 802.15.4-2006 (6.2.2.4) page 37
 *
 * param[in]    Enumeration status:         The result of the request to transmit a packet
 *              int         EnergyLevel:    ED level for the current channel. If status is set to
 *                                          SUCCESS, this is the ED level for the current channel.
 *                                          Otherwise, the value of this parameter will be ignored. */
void Ieee802154eMac::handle_PLME_ED_confirm(PHYenum status, UINT_8 energyLevel)
{

}

/**@author: 2014    Stefan Reis     (modified)
 * brief: PLME-GET.confirm reports the results of an information request from the PHY PIB
 *
 * The PLME-GET.confirm primitive reports the results of an information request from the PHY PIB.
 *
 * note: see Std 802.15.4-2006 (6.2.2.6) page 38
 *
 * param[in]    Enumeration     status:                 The result of the request to transmit a packet
 *              Enumeration     PIB Attribute:          The identifier of the PHY PIB attribute that was requested
 *              Various         PIB Attribute Value:    The value of the indicated PHY PIB attribute that was requested.
 *                                                      This parameter has zero length when the status parameter is set to
 *                                                      UNSUPPORTED_ATTRIBUTE. */
void Ieee802154eMac::handle_PLME_GET_confirm(PHYenum status, PHYPIBenum pibAttribute, PHY_PIB pibAttributeValue)
{
    // used the code from the NS2 implementation [CD & SR]
    if(status == phy_SUCCESS)
    {
	switch(pibAttribute)
	{
	    case PHY_CURRENT_CHANNEL:
	    {
		ppib.phyCurrentChannel = pibAttributeValue.phyCurrentChannel;
		break;
	    }
	    case PHY_CHANNELS_SUPPORTED:
	    {
		ppib.phyChannelsSupported = pibAttributeValue.phyChannelsSupported;
		break;
	    }
	    case PHY_TRANSMIT_POWER:
	    {
		ppib.phyTransmitPower = pibAttributeValue.phyTransmitPower;
		break;
	    }
	    case PHY_TX_POWER_TOLERANCE:
	    {
		ppib.phyTXPowerTolerance = pibAttributeValue.phyTXPowerTolerance;
		break;
	    }
	    case PHY_TX_POWER:
	    {
		ppib.phyTXPower = pibAttributeValue.phyTXPower;
		break;
	    }
	    case PHY_CCA_MODE:
	    {
		ppib.phyCCAMode = pibAttributeValue.phyCCAMode;
		break;
	    }
	    case PHY_MAX_FRAME_DURATION:
	    {
		ppib.phyMaxFrameDuration = pibAttributeValue.phyMaxFrameDuration;
		break;
	    }
	    case PHY_SHR_DURATION:
	    {
		ppib.phySHRDuration = pibAttributeValue.phySHRDuration;
		break;
	    }
	    case PHY_SYMBOLS_PER_OCTET:
	    {
		ppib.phySymbolsPerOctet = pibAttributeValue.phySymbolsPerOctet;
		break;
	    }
	    case PHY_PREAMBLE_SYMBOL_LENGTH:
	    {
		ppib.phyPreambleSymbolLength = pibAttributeValue.phyPreambleSymbolLength;
		break;
	    }
	    case PHY_UWB_DATA_RATES_SUPPORTED:
	    {
		ppib.phyUWBDataRatesSupported = pibAttributeValue.phyUWBDataRatesSupported;
		break;
	    }
	    case PHY_CSS_LOW_DATA_RATE_SUPPORTED:
	    {
		ppib.phyCSSLowDataRateSupported = pibAttributeValue.phyCSSLowDataRateSupported;
		break;
	    }
	    case PHY_UWB_COU_SUPPORTED:
	    {
		ppib.phyUWBCoUSupported = pibAttributeValue.phyUWBCoUSupported;
		break;
	    }
	    case PHY_UWB_CS_SUPPORTED:
	    {
		ppib.phyUWBCSSupported = pibAttributeValue.phyUWBCSSupported;
		break;
	    }
	    case PHY_UWB_LCP_SUPPORTED:
	    {
		ppib.phyUWBLCPSupported = pibAttributeValue.phyUWBLCPSupported;
		break;
	    }
	    case PHY_UWB_CURRENT_PULSE_SHAPE:
	    {
		ppib.phyUWBCurrentPulseShape = pibAttributeValue.phyUWBCurrentPulseShape;
		break;
	    }
	    case PHY_UWB_COU_PULSE:
	    {
		ppib.phyUWBCoUpulse = pibAttributeValue.phyUWBCoUpulse;
		break;
	    }
	    case PHY_UWB_CS_PULSE:
	    {
		ppib.phyUWBCSpulse = pibAttributeValue.phyUWBCSpulse;
		break;
	    }
	    case PHY_UWB_LCP_WEIGHT_1:
	    {
		ppib.phyUWBLCPWeight1 = pibAttributeValue.phyUWBLCPWeight1;
		break;
	    }
	    case PHY_UWB_LCP_WEIGHT_2:
	    {
		ppib.phyUWBLCPWeight2 = pibAttributeValue.phyUWBLCPWeight2;
		break;
	    }
	    case PHY_UWB_LCP_WEIGHT_3:
	    {
		ppib.phyUWBLCPWeight3 = pibAttributeValue.phyUWBLCPWeight3;
		break;
	    }
	    case PHY_UWB_LCP_WEIGHT_4:
	    {
		ppib.phyUWBLCPWeight4 = pibAttributeValue.phyUWBLCPWeight4;
		break;
	    }
	    case PHY_UWB_LCP_DELAY_2:
	    {
		ppib.phyUWBLCPDelay2 = pibAttributeValue.phyUWBLCPDelay2;
		break;
	    }
	    case PHY_UWB_LCP_DELAY_3:
	    {
		ppib.phyUWBLCPDelay3 = pibAttributeValue.phyUWBLCPDelay3;
		break;
	    }
	    case PHY_UWB_LCP_DELAY_4:
	    {
		ppib.phyUWBLCPDelay4 = pibAttributeValue.phyUWBLCPDelay4;
		break;
	    }
	    case PHY_RANGING:
	    {
		ppib.phyRanging = pibAttributeValue.phyRanging;
		break;
	    }
	    case PHY_RANGING_CRYSTAL_OFFSET:
	    {
		ppib.phyRangingCrystalOffset = pibAttributeValue.phyRangingCrystalOffset;
		break;
	    }
	    case PHY_RANGING_DPS:
	    {
		ppib.phyRangingDPS = pibAttributeValue.phyRangingDPS;
		break;
	    }
	    case PHY_CURRENT_CODE:
	    {
		ppib.phyCurrentCode = pibAttributeValue.phyCurrentCode;
		break;
	    }
	    case PHY_NATIVE_PRF:
	    {
		ppib.phyNativePRF = pibAttributeValue.phyNativePRF;
		break;
	    }
	    case PHY_UWB_SCAN_BINS_PER_CHANNEL:
	    {
		ppib.phyUWBScanBinsPerChannel = pibAttributeValue.phyUWBScanBinsPerChannel;
		break;
	    }
	    case PHY_UWB_INSERTED_PREAMBLE_INTERVAL:
	    {
		ppib.phyUWBInsertedPreambleInterval = pibAttributeValue.phyUWBInsertedPreambleInterval;
		break;
	    }
	    case PHY_TX_RMARKER_OFFSET:
	    {
		ppib.phyTXRMARKEROffset = pibAttributeValue.phyTXRMARKEROffset;
		break;
	    }
	    case PHY_RX_RMARKER_OFFSET:
	    {
		ppib.phyRXRMARKEROffset = pibAttributeValue.phyRXRMARKEROffset;
		break;
	    }
	    case PHY_RFRAME_PROCESSING_TIME:
	    {
		ppib.phyRFRAMEProcessingTime = pibAttributeValue.phyRFRAMEProcessingTime;
		break;
	    }
	    case PHY_CCA_DURATION:
	    {
		ppib.phyCCADuration = pibAttributeValue.phyCCADuration;
		break;
	    }
	    default:
		break;
	}
    }
}

/**@author: 2014    Stefan Reis     (modified)
 * brief: PLME-SET-TRX-STATE.confirm reports the result of a PLME-SET-TRX-STATE.request command
 *
 * The PLME-SET-TRX-STATE.confirm primitive reports the result of a request to change the internal
 * operating state of the transceiver.
 *
 * note: see Std 802.15.4-2006 (6.2.2.8) page 40
 *
 * param[in]      Enumeration status: The result of the request to transmit a packet */
void Ieee802154eMac::handle_PLME_SET_TRX_STATE_confirm(PHYenum status)
{
    EV << "[MAC]: a PLME_SET_TRX_STATE_confirm with " << stateTxt[status] << " received from PHY, the requested state is " << stateTxt[trx_state_req] << endl;

    // to handle only one SET_TRX_STATE.request
    if(status == trx_state_req)
    {
	trx_state_req_activ = false;
    }
    else if(trx_state_req == phy_FORCE_TRX_OFF && status == phy_TRX_OFF)
    {
	trx_state_req = status;
	trx_state_req_activ = false;
    }
    else if(status == phy_SUCCESS)
    {
	status = trx_state_req;
	trx_state_req_activ = false;
    }
    else
    {   // to cancelled the msg, if we switch from Rx to Tx state or reverse
	trx_state_req_activ = false;
	return;
    }

    // for TSCH Std 802.15.4e-2012
    if(mpib.macTSCHenabled)
    {
	if(trx_state_req == phy_RX_ON)
	{
	    // for TSCH CCA Transmitter
	    if(taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA))
	    {
		if(taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 3)
		{
		    taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 4;
		    EV << "[TSCH CCA]-Transmitter:[4] send CCA request" << endl;
		    PLME_CCA_request();
		    return;
		    // next step in handle_PLME_CCA_confirm()
		}
		else if(taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 8)
		{
		    taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 9;
		    EV << "[TSCH CCA]-Transmitter:[9] start the TsAckWaitTimer" << endl;

		    if(tsAckWaitTimer->isScheduled())
			cancelEvent(tsAckWaitTimer);

		    scheduleAt(simTime() + timeslotTable->getTemplate(useTimeslotID)->getAckWaitDbl(), tsAckWaitTimer);
		    return;
		    /* two possible next steps:
		     * 1. start to receive frame before the TsAckWaitTimer (receiveChangeNotification())
		     * 2. no frame received during the TsAckWaitTime (handleTsAckWait()) */
		}
	    }
	    else if(taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) == 3) // TSCH CCA Receiver
	    {
		taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) = 4;
		EV << "[TSCH CCA]-Receiver:[4] start the TsRxWaitTimer" << endl;

		if(tsRxWaitTimer->isScheduled())
		    cancelEvent(tsRxWaitTimer);

		scheduleAt(simTime() + timeslotTable->getTemplate(useTimeslotID)->getRxWaitDbl(), tsRxWaitTimer);
		return;
		/* two possible next steps:
		 * 1. start to receive a frame before the TsRxWaitTimer (receiveChangeNotification())
		 * 2. no frame received during the TsRxWaitTime (handleTsRxWait()) */
	    }
	}
	else if(trx_state_req == phy_TX_ON)
	{
	    // for TSCH CCA Transmitter
	    if(taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 5)
	    {
		taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 6;
		EV << "[TSCH CCA]-Transmitter:[6] transmit the msg" << endl;
		// the check if the msg fit in the timeslots (transmission duration <+ macTsMaxTx) is done in the MCPS_DATA_request
		ASSERT(txData);
		txPkt = txData;
		PD_DATA_request(txData->getByteLength(), txData->dup());
		return;
		// next step will be the PD_DATA_confirm()
	    }
	    else if(taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) == 8) // TSCH CCA Receiver
	    {
		taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) = 9;
		EV << "[TSCH CCA]-Receiver:[9] send the Ack" << endl;
		// check if the ack can be transmit during the macTsMaxAck

		ASSERT(txAck);
		if(calDuration(txAck) <= timeslotTable->getTemplate(useTimeslotID)->getMaxAckDbl())
		{
		    txPkt = txAck;
		    PD_DATA_request(txAck->getByteLength(), check_and_cast<Ieee802154eFrame *>(txAck->dup()));
		    return;
		    // next step will be the PD_DATA_confirm()
		}
		else
		{
		    EV << "[TSCH CCA]-Receiver:[9-ERROR]: the transmission duration (" << calDuration(txAck) << " s ) is bigger than the macTsMaxAck (" << timeslotTable->getTemplate(useTimeslotID)->getMaxAckDbl() * 1000 << " ms)" << endl;

		    delete txAck;
		    txAck = NULL;
		    resetTRX();
		    return;
		}
	    }
	}
	else
	{
	    if(taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 7)
		return; // wait for the TsRxAckDelayTimer

	    if(taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) == 7)
		return; // wait for the TsRxAckDelayTimer

	}
    }

    //[SR] old version
    simtime_t delay;

    if(backoffStatus == 99)
    {
	if(trx_state_req == phy_RX_ON)
	{
	    if(taskP.taskStatus(TP_RX_ON_CSMACA))
	    {
		taskP.taskStatus(TP_RX_ON_CSMACA) = false;
		csmaca_handle_RX_ON_confirm(status);
	    }
	}
    }
    /*else
     dispatch(status,__FUNCTION__,trx_state_req);*/

    if(status != phy_TX_ON)
	return;
    // wait the phy_succes

    //transmit the packet
    if(beaconWaitingTx)        // periodically tx beacon
    {
	// to synchronize better, we don't transmit the beacon here
    }
    else if(txAck)
    {
	//although no CSMA-CA required for the transmission of ack.,
	//but we still need to locate the backoff period boundary if beacon enabled
	//(refer to page 157, line 25-31)
	if((mpib.macBeaconOrder == 15) && (rxBO == 15))  //non-beacon enabled
	    delay = 0.0;
//         else if (txAck->getIsGTS())
//             delay = 0.0;
	else                                //beacon enabled
	{
	    // here we use the hidden dst addr that we already set in ACK on purpose

	    delay = csmacaLocateBoundary((getShortAddress(txAck->getDstAddr()) == mpib.macCoordShortAddress), 0.0);
	    ASSERT(delay < bPeriod);
	}

	if(delay == 0.0)
	    handleTxAckBoundTimer();
	else
	    startTxAckBoundTimer(delay);
    }
    else if(txGTS)
    {
	// send data frame in GTS here, no delay
	txPkt = txGTS;
	PD_DATA_request(txGTS->getByteLength(), check_and_cast<Ieee802154eFrame *>(txGTS->dup()));
    }
    else        // tx cmd or data
    {
	if((mpib.macBeaconOrder == 15) && (rxBO == 15))  //non-beacon enabled
	    delay = 0.0;
	else
	    delay = csmacaLocateBoundary(toParent(txCsmaca), 0.0);

	if(delay == 0.0)
	    handleTxCmdDataBoundTimer();            //transmit immediately
	else
	    startTxCmdDataBoundTimer(delay);
    }
}

/**@author: 2014    Stefan Reis
 * brief: PLME-SET.confirm reports the result of a PLME-SET.request command
 *
 * The PLME-SET.confirm primitive reports the results of the attempt to set a PIB attribute.
 *
 * note: see Std 802.15.4-2006 (6.2.2.10) page 41
 *
 * param[in]    Enumeration     status:         The result of the request to transmit a packet
 *              Enumeration     PIB Attribute:  The identifier of the PIB attribute being confirmed. */
void Ieee802154eMac::handle_PLME_SET_confirm(PHYenum status, PHYPIBenum pibAttribute)
{
    EV << "[MAC]: a PLME_SET_confirm with " << stateTxt[status] << " received from PHY, the requested PIBAttribute is " << paraTxt[pibAttribute] << endl;

    if(status == phy_SUCCESS && pibAttribute == ppib_pibAttribure_req)
    {
	ppib = tmp_ppib;
	if(taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA))
	{
	    if(taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 2)
	    {
		taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 3;
		EV << "[TSCH CCA]-Transmitter:[3] before sending CCA primitive, tell the PHY layer to turn on the receiver" << endl;
		PLME_SET_TRX_STATE_request(phy_RX_ON);
		// next step in handle_PLME_SET_TRX_STATE_confirm()
	    }
	    else if(taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 101)
	    {
		taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 5;
		EV << "[TSCH CCA]-Transmitter:[5] (without CCA) switch to Tx state" << endl;
		PLME_SET_TRX_STATE_request(phy_TX_ON);
		// next step in handle_PLME_SET_TRX_STATE_confirm()
	    }
	}
	else if(taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) == 2)
	{
	    taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) = 3;
	    EV << "[TSCH CCA]-Receiver:[3] set the TRX_STATE to RX_ON and wait for the pkt" << endl;
	    PLME_SET_TRX_STATE_request(phy_RX_ON);
	    // next step in PLME_SET_TRX_STATE_confirm()
	}
    }
    else
    {
	//TODO:[SR] error handle (UNSUPPORTED_ATTRIBUTE, INVALID_PARAMETER, or READ_ONLY)
    }
}

//-------------------------------------------------------------------------------/
/***************************** <MCPS-SAP primitives> ****************************/
//-------------------------------------------------------------------------------/
/**@author: 2014    Stefan Reis     (modified)
 * brief: MCPS-DATA.request requests the transfer of data to another device.
 *
 * The MCPS-DATA.request primitive requests the transfer of data to another device.
 *
 * note: see Std 802.15.4-2011 (6.3.1) page 116
 *
 * param[in]     "param1 Input parameter (i.e. read by this function). */
void Ieee802154eMac::MCPS_DATA_request(Ieee802154eAddrMode srcAddrMode, Ieee802154eAddrMode dstAddrMode, UINT_16 dstPANId, IE3ADDR dstAddr, UINT_8 msduLength, cPacket* msdu, UINT_8 msduHandle, bool ackTX, bool gtsTX, bool indirectTX, UINT_8 securityLevel, UINT_8 keyIdMode, UINT_64 keySource, UINT_8 keyIndex, Ieee802154eUWBFType uwbPRF, RangingControl ranging, UINT_16 uwbPreambleSymbolRepetitions, UINT_8 dataRate, FrameCrlOptions frameControlOptions, std::vector<Ieee802154eIEHeaderType> headerIElist, std::vector<Ieee802154eIEPayloadType> payloadIElist, bool sendMultipurpose)
{
    Ieee802154eMacTaskType task = TP_MCPS_DATA_REQUEST;
    if(!mpib.macTSCHenabled)
    {
	checkTaskOverflow(task); // reset step to 0 if no task of this type is pending
	taskP.taskStatus(task) = true;
    }

    //--- construct MPDU - Std 802.15.4-2011 (figure 46) page 65 ---

    Ieee802154eDataFrame* tmpData = new Ieee802154eDataFrame();
    Ieee802154eMPFrame* tmpMPData = new Ieee802154eMPFrame();

    // construct frame control field
    FrameCtrl frmCtrl;
    MPFrameCtrl mpfrmCtrl; // for Multipurpose frame

    IE3ADDR tmpDstAddr;
    IE3ADDR tmpSrcAddr;
    UINT_16 tmpDstPanId;
    UINT_16 tmpSrcPanId;

    if(sendMultipurpose) // see Std 802.15.4e-2012 page 168 // see packet structure Std 802.15.4e-2012 (figure 48k) page 75
    {
	delete tmpData; // release the unused pointer
	frmCtrl.frameType = Ieee802154e_MULTI;  //multipurpose type
	tmpMPData->setName("Ieee802154eMULTI");
	mpfrmCtrl.lngFrameCtrl = true; //no default value in the Std 802.15.4e-2012
    }
    else
    {
	delete tmpMPData; // release the unused pointer
	frmCtrl.frameType = Ieee802154e_DATA;   //data type
	tmpData->setName("Ieee802154DATA");
    }

    //    frmCtrl.intraPan = (SrcPANId == DstPANId)? true:false;
    frmCtrl.dstAddrMode = dstAddrMode;
    frmCtrl.srcAddrMode = srcAddrMode;

    if(sendMultipurpose && mpfrmCtrl.lngFrameCtrl)
    {
	mpfrmCtrl.presPANID = frameControlOptions.panIDsuppressed; //by default: false
	frmCtrl.securityEnabled = (securityLevel == ASH_SLVL_NONE) ? false:true; //by default: false
	frmCtrl.seqNbSup = frameControlOptions.seqSuppressed;
	frmCtrl.frmPending = false;
	frmCtrl.frameVersion = Ieee802154_compatible;
	frmCtrl.ackReq = ackTX;
	frmCtrl.presIElist = frameControlOptions.iesIncluded;

	if(!frmCtrl.seqNbSup)
	    tmpMPData->setSeqNmbr(msduHandle);
    }
    else
    {
	frmCtrl.securityEnabled = (securityLevel == ASH_SLVL_NONE) ? false:true; //secuData;
	frmCtrl.frmPending = false;
	frmCtrl.ackReq = ackTX;

	// see Std 802.15.4-2011 page 118 or see Std 802.15.4e-2012 (5.2.3 Frame compatibility) page 79
	if(msduLength > aMaxMACSafePayloadSize || msdu->getByteLength() > aMaxMACSafePayloadSize)
	    frmCtrl.frameVersion = Ieee802154_2006_introduced;
	else
	    frmCtrl.frameVersion = Ieee802154_2003_compatible; // Frames are compatible between IEEE Std 802.15.4-2003 and IEEE Std 802.15.4e-2012

	// for Std 802.15.4e-2012 TSCH
	if(mpib.macTSCHenabled)
	    frmCtrl.frameVersion = Ieee802154_compatible;

	if(frmCtrl.frameVersion == Ieee802154_2003_compatible || frmCtrl.frameVersion == Ieee802154_2006_introduced)
	{
	    frmCtrl.seqNbSup = false;
	    tmpData->setSeqNmbr(msduHandle);        // == macDSN
	}
	else
	    frmCtrl.seqNbSup = frameControlOptions.seqSuppressed; // for the Std 802.15.4e-2012

	tmpData->setSeqNmbr(!frmCtrl.seqNbSup ? msduHandle:0);

    }

    if(frmCtrl.dstAddrMode == defFrmCtrl_AddrModeNone)
    { // no address
	tmpDstPanId = 0;
	tmpDstAddr = (IE3ADDR)0xffff;
    }
    else if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode8)
    { // 8 bit address
	tmpDstPanId = dstPANId;
	tmpDstAddr = IE3ADDR((UINT_8)dstAddr.getInt());
    }
    else if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode16)
    { // 16 bit address
	tmpDstPanId = dstPANId;
	tmpDstAddr = IE3ADDR((UINT_16)dstAddr.getInt());
    }
    else
    { // 64 bit address
	tmpDstPanId = dstPANId;
	tmpDstAddr = dstAddr;
    }

    if(frmCtrl.srcAddrMode == defFrmCtrl_AddrModeNone)
    { // no address
	tmpSrcPanId = 0;
	tmpSrcAddr = (IE3ADDR)0xffff;
    }
    else if(frmCtrl.srcAddrMode == defFrmCtrl_AddrMode8)
    { // 8 bit address
	tmpSrcPanId = mpib.macPANId;
	tmpSrcAddr = (IE3ADDR)mpib.macSimpleAddress;
    }
    else if(frmCtrl.srcAddrMode == defFrmCtrl_AddrMode16)
    { // 16 bit address
	tmpSrcPanId = mpib.macPANId;
	tmpSrcAddr = (IE3ADDR)mpib.macShortAddress;
    }
    else
    { // 64 bit address
	tmpSrcPanId = mpib.macPANId;
	tmpSrcAddr = (IE3ADDR)mpib.macExtendedAddress;
    }

    if(sendMultipurpose)
    {
	if(mpfrmCtrl.presPANID)
	{ // see Std 802.15.4e-2012 (5.2.2.6.13) page77
	    tmpMPData->setDstPanId(dstPANId);
	    tmpMPData->setSrcPanId(dstPANId);
	}
	else
	{
	    tmpMPData->setDstPanId(0);
	    tmpMPData->setSrcPanId(0);
	}

	tmpMPData->setDstAddr(tmpDstAddr);
	tmpMPData->setSrcAddr(tmpSrcAddr);
    }
    else
    {
	tmpData->setDstPanId(tmpDstPanId);
	tmpData->setDstAddr(tmpDstAddr);
	tmpData->setSrcPanId(tmpSrcPanId);
	tmpData->setSrcAddr(tmpSrcAddr);
    }

    AuxiliarySecurityHeader auxSecHd;
    auxSecHd.secLvl = (Ieee802154eASHSeclvlType)securityLevel;
    auxSecHd.keyIdentMode = keyIdMode;
    auxSecHd.keySource = keySource;
    auxSecHd.keyIndex = keyIndex;

    if(sendMultipurpose)
    {
	frmCtrl.compPanID = getPANIDComp(frmCtrl, tmpMPData->getSrcPanId(), tmpMPData->getDstPanId()); // PAN ID Compression field
//        tmpMPData->setIsGTS(gtsTX);

	tmpMPData->setFrmCtrl(frmCtrl);
	tmpMPData->setMpFrmCtrl(mpfrmCtrl);
	tmpMPData->setAuxSecHd(auxSecHd);

	// set length and encapsulate msdu
	tmpMPData->setByteLength(calFrmByteLength(tmpMPData));
	tmpMPData->encapsulate(msdu);     // the length of msdu is added to mpdu
	EV << "[MAC]: MPDU constructed: " << tmpMPData->getName() << " (" << tmpMPData->getEncapsulatedPacket()->getName() << "), #" << (int)tmpMPData->getSeqNmbr() << ", " << tmpMPData->getByteLength() << " Bytes" << endl;
    }
    else
    {
	frmCtrl.compPanID = getPANIDComp(frmCtrl, tmpData->getSrcPanId(), tmpData->getDstPanId()); // PAN ID Compression
	//tmpData->setIsGTS(gtsTX);

	tmpData->setFrmCtrl(frmCtrl);
	tmpData->setAuxSecHd(auxSecHd);

	// set length and encapsulate msdu
	tmpData->setByteLength(calFrmByteLength(tmpData));
	tmpData->encapsulate(msdu);     // the length of msdu is added to mpdu
	EV << "[MAC]: MPDU constructed: " << tmpData->getName() << " (" << tmpData->getEncapsulatedPacket()->getName() << "), #" << (int)tmpData->getSeqNmbr() << ", " << tmpData->getByteLength() << " Bytes" << endl;
    }

    //***********************************************
    //********* decide the transmission mode ********
    //***********************************************
    Ieee802154eTxOption TxOption;

    if(gtsTX)
	TxOption = GTS_TRANS;
    else if(indirectTX)
	TxOption = INDIRECT_TRANS;
    else
	TxOption = DIRECT_TRANS;

    taskP.mcps_data_request_TxOption = TxOption;

    switch(TxOption)
    {
	case DIRECT_TRANS: // send in the CAP or at the Link for TSCH transmission
	{
	    // TSCH CSME-CA algorithm - Std 802.15.4e-2012 (figure 11a) page 14
	    if(mpib.macTSCHenabled)
	    {
		// check the could send in a timeslot (TSCH CCA - macTsMaxTX time)
		simtime_t duration;
		if(sendMultipurpose)
		    duration = calDuration(tmpMPData);
		else
		    duration = calDuration(tmpData);

		if(duration > timeslotTable->getTemplate(useTimeslotID)->getMaxTxDbl()) // delete msg and report MLME_DATA_confirm
		{
		    EV << "[TSCH CCA]: the estimated transmission time (" << duration << " s) is bigger then " << timeslotTable->getTemplate(useTimeslotID)->getMaxTxDbl() * 1000 << " ms, msg will be discarded" << endl;

		    if(sendMultipurpose)
			MCPS_DATA_confirm(tmpMPData->getSeqNmbr(), 0, false, 0, 0, 0, 0, 0, mac_FRAME_TOO_LONG, 0, 0, tmpMPData);
		    else
			MCPS_DATA_confirm(tmpData->getSeqNmbr(), 0, false, 0, 0, 0, 0, 0, mac_FRAME_TOO_LONG, 0, 0, tmpData);

		    delete tmpMPData;
		    delete tmpData;
		    numUpperPktLost++;
		    reqtMsgFromIFq();
		    return;
		}
		else
		{
		    EV << "[TSCH CCA]: the estimated transmission time (" << duration << " s) is smaller or equal as the " << timeslotTable->getTemplate(useTimeslotID)->getMaxTxDbl() * 1000 << " ms, msg will be saved and transmitted later in the Tx timeslot to the dest Addr" << endl;

		    if(sendMultipurpose)
		    {   // if a timeslot is linked to the device
			if(linkTable->existLink((UINT_16)tmpMPData->getDstAddr().getInt()))
			{
			    //send(tmpMPData, mQueueOut);
			    queueModule->insertInQueue(tmpMPData);
			}
			else
			{
			    EV << "[MAC][TSCH CSMA-CA]: ERROR, no TX link available to the device!" << endl;
			    delete tmpMPData;
			    numUpperPktLost++;
			}
		    }
		    else
		    {   // if a timeslot is linked to the device
			if(linkTable->existLink((UINT_16)tmpData->getDstAddr().getInt()))
			{
			    //send(tmpData, mQueueOut);
			    queueModule->insertInQueue(tmpData);
			}
			else
			{
			    EV << "[MAC][TSCH CSMA-CA]: ERROR, no TX link available to the device!" << endl;
			    delete tmpData;
			    numUpperPktLost++;
			}
		    }
		}
	    }
	    else
	    {
		// CSMA-CA algorithm - Std 802.15.4e-2012 (figure 11) page 13
		taskP.taskStep(task)++;strcpy
		(taskP.taskFrFunc(task), "csmacaCallBack");
		ASSERT(txData == NULL);

		if(sendMultipurpose)
		    txData = tmpMPData;
		else
		    txData = tmpData;

		csmacaEntry('d');
	    }
	    break;
	}

	case INDIRECT_TRANS:    // for the PAN to Device transmission
	    //TODO TBD
	    break;

	case GTS_TRANS: // don't use CSMA-CA, send directly in the GTS
	{
	    taskP.taskStep(task)++;
	    // waiting for GTS arriving, callback from handleGtsTimer()
	    strcpy
	    (taskP.taskFrFunc(task), "handleGtsTimer");

	    ASSERT(txGTS == NULL);

	    if(sendMultipurpose)
		txGTS = tmpMPData;
	    else
		txGTS = tmpData;

	    numGTSRetry = 0;

	    // if i'm the PAN coordinator, should defer the transmission until the start of the receive GTS
	    // if i'm the device, should try to transmit if now is in my GTS
	    // refer to spec. 7.5.7.3
	    if(!isPANCoor && index_gtsTimer == 99)
	    {
		ASSERT(gtsTimer->isScheduled());
		// first check if the requested transaction can be completed before the end of current GTS
		if(gtsCanProceed())
		{
		    // directly hand over to FSM, which will go to next step, state parameters are ignored
		    FSM_MCPS_DATA_request();
		}
	    }
	    break;
	}
	default:
	{
	    error("[MAC]: undefined txOption: %d!", TxOption);
	    break;
	}
    }
}

/**@author: 2014    Stefan Reis     (modified)
 * brief: MCPS-DATA.confirm reports the results of a request to transfer data to another device
 *
 * The MCPS-DATA.confirm primitive reports the results of a request to transfer data to another device.
 *
 * note: see Std 802.15.4-2011 (6.3.2) page 118
 *
 * param[in]     "param1 Input parameter (i.e. read by this function). */
void Ieee802154eMac::MCPS_DATA_confirm(UINT_8 msduHandle, UINT_64 timestamp, bool rangingReceived, UINT_32 rangingCounterStart, UINT_32 rangingCounterStop, UINT_32 rangingTrackingInterval, UINT_32 rangingOffset, UINT_8 rangingFOM, MACenum status, UINT_8 numBackoffs, UINT_8 dsn, cPacket* ackPayload)
{
    cMessage *appframe;
    if(ackPayload == NULL)
	appframe = new cMessage();
    else
    {
	cMessage* tmpFrame = ackPayload->decapsulate();
	if(tmpFrame == NULL)
	    appframe = new cMessage();
	else
	    appframe = tmpFrame;
    }

    appframe->setName("MCPS_DATA_confirm");

    string statusTxt = "";
    switch(status)
    {
	case mac_SUCCESS:
	    statusTxt = "mac_SUCCESS";
	    break;
	case mac_TRANSACTION_OVERFLOW:
	    statusTxt = "mac_TRANSACTION_OVERFLOW";
	    break;
	case mac_TRANSACTION_EXPIRED:
	    statusTxt = "mac_TRANSACTION_EXPIRED";
	    break;
	case mac_CHANNEL_ACCESS_FAILURE:
	    statusTxt = "mac_CHANNEL_ACCESS_FAILURE";
	    break;
	case mac_INVALID_ADDRESS:
	    statusTxt = "mac_INVALID_ADDRESS";
	    break;
	case mac_INVALID_GTS:
	    statusTxt = "mac_INVALID_GTS";
	    break;
	case mac_NO_ACK:
	    statusTxt = "mac_NO_ACK";
	    break;
	case mac_COUNTER_ERROR:
	    statusTxt = "mac_COUNTER_ERROR";
	    break;
	case mac_FRAME_TOO_LONG:
	    statusTxt = "mac_FRAME_TOO_LONG";
	    break;
	case mac_UNAVAILABLE_KEY:
	    statusTxt = "mac_UNAVAILABLE_KEY";
	    break;
	case mac_UNSUPPORTED_SECURITY:
	    statusTxt = "mac_UNSUPPORTED_SECURITY";
	    break;
	case mac_INVALID_PARAMETER:
	    statusTxt = "mac_INVALID_PARAMETER";
	    break;
	case mac_ACK_RCVD_NODSN_NOSA:
	    statusTxt = "mac_ACK_RCVD_NODSN_NOSA";
	    break;
	default:
	    statusTxt = "status not defined";
	    break;
    }

    if(useIeee802Ctrl)
    { //FIXME: need to test [SR]
	Ieee802Ctrl* cinfo = new Ieee802Ctrl();
	appframe->setControlInfo(cinfo);

	EV << "[MAC]: sending MCPS-DATA.confirm frame to upper layer (disabled), frame retries=" << (int)numBackoffs << " with status=" << statusTxt << endl;
	//send(appframe->dup(), mUpperLayerOut); // [SR] not supported from the higher layer
	delete appframe;
    }
    else
    {
	Ieee802154eNetworkCtrlInfo *cinfo = new Ieee802154eNetworkCtrlInfo();
	cinfo->setName("MCPS_DATA_confirm");
	cinfo->setMsduHandle(msduHandle);
	cinfo->setTimeStamp(timestamp);
	cinfo->setRangingReceived(rangingReceived);
	cinfo->setRangingCounterStart(rangingCounterStart);
	cinfo->setRangingCounterStop(rangingCounterStop);
	cinfo->setRangingTrackingInterval(rangingTrackingInterval);
	cinfo->setRangingOffset(rangingOffset);
	cinfo->setRangingFOM(rangingFOM);
	cinfo->setStatus(status);
	// for 802.15.4e-2012
	cinfo->setNumBackoffs(numBackoffs);
	//control_info->setAckPayload     //XXX: [SR] not implemented

	appframe->setControlInfo(cinfo);
	EV << "[MAC]: sending MCPS-DATA.confirm frame to upper layer, the frame retries=" << (int)numBackoffs << " with status=" << statusTxt << endl;
	send(appframe->dup(), mUpperLayerOut);
	delete appframe;
    }
}

/**@author: 2014    Stefan Reis     (modified)
 * brief: The MCPS-DATA.indication indicates the reception of data from another device.
 *
 * The MCPS-DATA.indication primitive indicates the reception of data from another device.
 *
 * note: see Std 802.15.4-2011 (6.3.3) page 119
 *
 * param[in]     "param1 Input parameter (i.e. read by this function). */
void Ieee802154eMac::MCPS_DATA_indication(Ieee802154eAddrMode srcAddrMode, UINT_16 srcPANId, IE3ADDR srcAddr, Ieee802154eAddrMode dstAddrMode, UINT_16 dstPANId, IE3ADDR dstAddr, UINT_8 msduLength, Ieee802154eFrame* msdu, UINT_8 mpduLinkQuality, UINT_8 dsn, UINT_32 Timestamp, UINT_8 SecurityLevel, UINT_8 KeyIdMode, UINT_64 keySource, UINT_8 keyIndex, UINT_8 uwbPRF, Ieee802154eUWBFType uwbPreambleSymbolRepetitions, UINT_8 dataRate, RangingControl rangingReceived, UINT_32 rangingCounterStart, UINT_32 rangingCounterStop, UINT_32 rangingTrackingInterval, UINT_32 rangingOffset, UINT_8 rangingFOM)
{
    // [SR] old version
//     if (msdu->getIsGTS())
//     {
//         numRxGTSPkt++;
//     }
//     else
//     {
    numRxDataPkt++;
    //}

    if(mpib.macMetricsEnabled)
    {
	mpib.macRXSuccessCount++;
    }

    cPacket * msg = msdu->decapsulate();
    if(useIeee802Ctrl)
    {
	Ieee802Ctrl* cinfo = new Ieee802Ctrl();
	MACAddress destination = srcAddr;
	if(srcAddr.isBroadcast())
	    destination = MACAddress::BROADCAST_ADDRESS;
	else
	    destination.convert48();

	cinfo->setSrc(destination);
	msg->setControlInfo(cinfo);
    }
    else
    {
	Ieee802154eNetworkCtrlInfo * cinfo = new Ieee802154eNetworkCtrlInfo();
	cinfo->setNetwAddr(msdu->getSrcAddr().getInt());
	msg->setControlInfo(cinfo);
    }

    EV << "[MAC]: sending received " << msg->getName() << " frame to upper layer" << endl;
    send(msg->dup(), mUpperLayerOut);
    delete msdu;
    delete msg;
}

/**@author: 2014    Stefan Reis
 * brief: MCPS-PURGE.request allows the next higher layer to purge an MSDU from the transaction queue
 *
 * The MCPS-PURGE.request primitive allows the next higher layer to purge an MSDU from the transaction
 * queue.
 *
 * note: see Std 802.15.4-2011 (6.3.4) page 123
 *
 * param[in]     "param1 Input parameter (i.e. read by this function). */
void Ieee802154eMac::MCPS_PURGE_request(UINT_8 msduHandle)
{

}

/**@author: 2014    Stefan Reis
 * brief: MCPS-PURGE.confirm reports the result of a MCPS-PURGE.request command
 *
 * The MCPS-PURGE.confirm primitive allows the MAC sublayer to notify the next higher layer of the
 * success of its request to purge an MSDU from the transaction queue.
 *
 * note: see Std 802.15.4-2011 (6.3.2) page 118
 *
 * param[in]     "param1 Input parameter (i.e. read by this function). */
void Ieee802154eMac::MCPS_PURGE_confirm(UINT_8 msduHandle, MACenum status)
{

}

//-------------------------------------------------------------------------------/
/****************************** CSMA/CA Functions *******************************/
//-------------------------------------------------------------------------------/
void Ieee802154eMac::csmacaEntry(char pktType)
{
    if(pktType == 'c')     // txBcnCmd
    {
	waitBcnCmdAck = false;          // beacon packet not yet transmitted
	numBcnCmdRetry = 0;
	if(backoffStatus == 99)        // backoffing for data packet
	{
	    backoffStatus = 0;
	    csmacaCancel();
	}
	csmacaResume();
    }
    else if(pktType == 'u')    // txBcnCmdUpper
    {
	waitBcnCmdUpperAck = false;         //command packet not yet transmitted
	numBcnCmdUpperRetry = 0;
	if((backoffStatus == 99) && (txCsmaca != txBcnCmd)) //backoffing for data packet
	{
	    backoffStatus = 0;
	    csmacaCancel();
	}
	csmacaResume();

    }
    else //if (pktType == 'd')  //txData and txData_multi
    {
	waitDataAck = false;            //data packet not yet transmitted
	numDataRetry = 0;
	csmacaResume();
    }
}

void Ieee802154eMac::csmacaResume()
{
    bool ackReq;

    if((backoffStatus != 99)           //not during backoff
    && (!inTransmission))              //not during transmission
    {
	if((txBcnCmd) && (!waitBcnCmdAck))
	{
	    backoffStatus = 99;
	    ackReq = txBcnCmd->getFrmCtrl().ackReq;
	    txCsmaca = txBcnCmd;
	    csmacaStart(true, txBcnCmd, ackReq);
	}
	else if((txBcnCmdUpper) && (!waitBcnCmdUpperAck))
	{
	    backoffStatus = 99;
	    ackReq = txBcnCmdUpper->getFrmCtrl().ackReq;
	    txCsmaca = txBcnCmdUpper;
	    csmacaStart(true, txBcnCmdUpper, ackReq);
	}
	else if((txData) && (!waitDataAck))
	{
	    strcpy(taskP.taskFrFunc(TP_MCPS_DATA_REQUEST), "csmacaCallBack"); //the transmission may be interrupted and need to backoff again
	    taskP.taskStep(TP_MCPS_DATA_REQUEST) = 1;        //also set the step

	    backoffStatus = 99;
	    ackReq = txData->getFrmCtrl().ackReq;
	    txCsmaca = txData;
	    csmacaStart(true, txData, ackReq);
	}
    }
}

/**@author: 2014    Stefan Reis     (modified) */
void Ieee802154eMac::csmacaStart(bool firsttime, Ieee802154eFrame* frame, bool ackReq)
{
    bool backoff;
    simtime_t backoffTime, rxBI;

    if(txAck)
    {
	backoffStatus = 0;
	tmpCsmaca = 0;
	return;
    }

    ASSERT(!backoffTimer->isScheduled());

    if(firsttime)
    {
	EV << "[CSMA-CA]: starting CSMA-CA for " << frame->getName() << ":#" << (int)frame->getSeqNmbr() << endl;
	beaconEnabled = ((mpib.macBeaconOrder != 15) || (rxBO != 15));
	csmacaReset(beaconEnabled);
	ASSERT(tmpCsmaca == 0);
	tmpCsmaca = frame;
	csmacaAckReq = ackReq;
    }

    if(rxBO != 15)     // is receiving beacon
    {
	// set schedBcnRxTime: the latest time that the beacon should have arrived
	// schedBcnRxTime may not be bcnRxTime, when missing some beacons
	// or being in the middle of rxing a beacon
	schedBcnRxTime = bcnRxTime;
	rxBI = rxSfSpec.BI / phy_symbolrate;
	while(schedBcnRxTime + rxBI < simTime())
	    schedBcnRxTime += rxBI;
    }

    //XXX: from the csma802154e.cc [SR]
    switch(backoffMethod)
    {
	case EXPONENTIAL:
	{
	    int BE = std::min(mpib.macMinBE + NB, (int)mpib.macMaxBE);
	    double d = pow((double)2, (int)BE);
	    int v = (int)d - 1;
	    int r = intuniform(1, v, 0);
	    backoffTime = r * bPeriod.dbl();

	    EV << "(startTimer) backoffTimer value=" << backoffTime << " (BE=" << BE << ", 2^BE-1= " << v << ", r=" << r << ")" << endl;
	    break;
	}
	case LINEAR:
	{
	    int slots = intuniform(1, initialCW + NB, 0);
	    backoffTime = slots * bPeriod.dbl();
	    EV << "(startTimer) backoffTimer value=" << backoffTime << endl;
	    break;
	}
	case CONSTANT:
	{
	    int slots = intuniform(1, initialCW, 0);
	    backoffTime = slots * bPeriod.dbl();
	    EV << "(startTimer) backoffTimer value=" << backoffTime << endl;
	    break;
	}
	default:
	{
	    error("Unknown backoff method!");
	}
    }

    EV << "[CSMA-CA]: choosing random number of backoff periods: " << backoffTime / bPeriod << endl;
    EV << "[CSMA-CA]: backoff time before adjusting: " << backoffTime * 1000 << " ms" << endl;
    backoffTime = csmacaAdjustTime(backoffTime); // if scheduled backoff ends before CAP, backoffTime should be adjusted
    EV << "[CSMA-CA]: backoff time after adjusting: " << backoffTime * 1000 << " ms" << endl;
    backoff = true;

    if(beaconEnabled)
    {
	if(firsttime)
	{
	    backoffTime = csmacaLocateBoundary(toParent(tmpCsmaca), backoffTime);
	    EV << "[CSMA-CA]: backoff time after locating boundary: " << backoffTime * 1000 << " ms" << endl;
	}
	if(!csmacaCanProceed(backoffTime)) // check if backoff can be applied now. If not, canProceed() will decide how to proceed.
	    backoff = false;
    }
    if(backoff)
    {
	startBackoffTimer(backoffTime);
    }
}

void Ieee802154eMac::csmacaCancel()
{
    if(backoffTimer->isScheduled())
	cancelEvent(backoffTimer);
    else if(deferCCATimer->isScheduled())
	cancelEvent(deferCCATimer);
    else
	taskP.taskStatus(TP_CCA_CSMACA) = false;
    tmpCsmaca = 0;
}

void Ieee802154eMac::csmacaCallBack(PHYenum status)
{
    if(((!txBcnCmd) || (waitBcnCmdAck)) && ((!txBcnCmdUpper) || (waitBcnCmdUpperAck)) && ((!txData) || (waitDataAck)))
	return;

    backoffStatus = (status == phy_IDLE) ? 1:2;

    dispatch(status, __FUNCTION__);
}

void Ieee802154eMac::csmacaReset(bool bcnEnabled)
{ // CSMA-CA: Slotted? - Std 802.15.4e-2012 (Figure 11 - CSMA-CA Algorithm) page 13
    if(bcnEnabled) //Slotted: YES
    {
	NB = 0;
	CW = 2;
	BE = mpib.macMinBE;
	if((mpib.macBattLifeExt) && (BE > 2))
	    BE = 2;
    }
    else            // Slotted: NO
    {
	NB = 0;
	BE = mpib.macMinBE;
    }
}

simtime_t Ieee802154eMac::csmacaAdjustTime(simtime_t wtime)
{
    //find the beginning point of CAP and adjust the scheduled time
    //if it comes before CAP
    simtime_t neg;
    simtime_t tmpf;

    ASSERT(tmpCsmaca);
    if(!toParent(tmpCsmaca))       // as a coordinator
    {
	if(mpib.macBeaconOrder != 15)
	{
	    /* Linux floating number compatibility
	     neg = (CURRENT_TIME + wtime - bcnTxTime) - mac->beaconPeriods * bPeriod;
	     */
	    {
		tmpf = txBcnDuration * bPeriod;
		tmpf = simTime() - tmpf;
		tmpf += wtime;
		neg = tmpf - mpib.macBeaconTxTime;
	    }

	    if(neg < 0.0)
		wtime -= neg;
	    return wtime;
	}
	else
	    return wtime;
    }
    else    // as a device
    {
	if(rxBO != 15)
	{
	    /* Linux floating number compatibility
	     neg = (CURRENT_TIME + wtime - bcnRxTime) - mac->beaconPeriods2 * bPeriod;
	     */
	    {
		tmpf = rxBcnDuration * bPeriod;
		tmpf = simTime() - tmpf;
		tmpf += wtime;
		neg = tmpf - schedBcnRxTime;
	    }

	    if(neg < 0.0)
		wtime -= neg;
	    return wtime;
	}
	else
	    return wtime;
    }
}

simtime_t Ieee802154eMac::csmacaLocateBoundary(bool toParent, simtime_t wtime)
{
    int align;
    simtime_t bcnTxRxTime, tmpf, newtime, rxBI;

    if((mpib.macBeaconOrder == 15) && (rxBO == 15))   // non-enabled beacon mode
	return wtime;

    if(toParent)
	align = (rxBO == 15) ? 1:2;
    else
	align = (mpib.macBeaconOrder == 15) ? 2:1;

    // we need to do this here, because this function may be called outside the CSMA-CA algorithm, e.g. in handle_PLME_SET_TRX_STATE_confirm while preparing to send an ACK
    if(rxBO != 15)     // is receiving beacon
    {
	// set schedBcnRxTime: the latest time that the beacon should have arrived
	// schedBcnRxTime may not be bcnRxTime, when missing some beacons
	// or being in the middle of rxing a beacon
	schedBcnRxTime = bcnRxTime;
	rxBI = rxSfSpec.BI / phy_symbolrate;
	while(schedBcnRxTime + rxBI < simTime())
	    schedBcnRxTime += rxBI;
    }

    bcnTxRxTime = (align == 1) ? mpib.macBeaconTxTime:schedBcnRxTime;

    /* Linux floating number compatibility
     newtime = fmod(CURRENT_TIME + wtime - bcnTxRxTime, bPeriod);
     */
    tmpf = simTime() + wtime;
    tmpf -= bcnTxRxTime;
    newtime = fmod(tmpf, bPeriod);
    //EV << "simTime() = " << simTime() << "; wtime = " << wtime << "; newtime = " << newtime << endl;

    if(newtime > 0.0)
    {
	/* Linux floating number compatibility
	 newtime = wtime + (bPeriod - newtime);
	 */

	tmpf = bPeriod - newtime;
	EV << "[CSMA-CA]: deviation to backoff boundary: " << tmpf << " s" << endl;
	newtime = wtime + tmpf;
    }
    else
	newtime = wtime;

    return newtime;
}

bool Ieee802154eMac::csmacaCanProceed(simtime_t wtime, bool afterCCA)
{
    bool ok;
    UINT_16 t_bPeriods, t_CAP;
    simtime_t tmpf, rxBI, t_fCAP, t_CCATime, t_IFS, t_transacTime;
    simtime_t now = simTime();
    csmacaWaitNextBeacon = false;
    t_transacTime = 0;
    wtime = csmacaLocateBoundary(toParent(tmpCsmaca), wtime);

    // check if CSMA-CA can proceed within the current superframe
    // in the case the node acts as both a coordinator and a device, both the superframes from and to this node should be taken into account)
    // Check 1: if now is in CAP
    // Check 2: if the number of backoff periods greater than the remaining number of backoff periods in the CAP
    // check 3: if the entire transmission can be finished before the end of current CAP

    EV << "[CSMA-CA]: starting to evaluate whether CSMA-CA can proceed ..." << endl;
    if(!toParent(tmpCsmaca))       // as a coordinator
    {
	if(mpib.macBeaconOrder != 15)      // beacon enabled as a coordinator
	{
	    t_fCAP = mpib.macBeaconTxTime + (txSfSpec.finalCap + 1) * txSfSlotDuration / phy_symbolrate;
	    if(now >= t_fCAP)      // Check 1
	    {
		csmacaWaitNextBeacon = true;
		bPeriodsLeft = 0;   // rechoose random backoff
		EV << "[CSMA-CA]: cannot proceed because it's now NOT in CAP (outgoing), resume at the beginning of next CAP" << endl;
		return false;
	    }
	    else                    // Check 2
	    {
		// calculate num of Backoff periods in CAP except for txBcnDuration
		t_CAP = (txSfSpec.finalCap + 1) * txSfSlotDuration / aUnitBackoffPeriod - txBcnDuration;
		EV << "[CSMA-CA]: total number of backoff periods in CAP (except for txBcnDuration): " << t_CAP << endl;

		// calculate num of Backoff periods from the first backoff in CAP to now+wtime
		tmpf = now + wtime;
		tmpf -= mpib.macBeaconTxTime;
		EV << "[CSMA-CA]: wtime = " << wtime << "; mpib.macBeaconTxTime = " << mpib.macBeaconTxTime << endl;
		tmpf = tmpf / bPeriod;
		EV << "[CSMA-CA]: tmpf = " << tmpf << "; bPeriod  = " << bPeriod << endl;
		t_bPeriods = (UINT_16)(SIMTIME_DBL(tmpf) - txBcnDuration);
		EV << "[CSMA-CA]: t_bPeriods = " << t_bPeriods << "; txBcnDuration = " << (int)txBcnDuration << endl;

		tmpf = now + wtime;
		tmpf -= mpib.macBeaconTxTime;
		if(fmod(tmpf, bPeriod) > 0.0)
		    t_bPeriods++;

		// calculate the difference
		bPeriodsLeft = t_bPeriods - t_CAP; // backoff periods left for next superframe
	    }
	}
	else
	    // non-beacon as a coordinator, but beacon enabled as a device
	    bPeriodsLeft = -1;
    }
    else        // as a device
    {
	if(rxBO != 15)     // beacon enabled as a device
	{
	    // we assume that no beacon will be lost
	    /* If max beacon loss reached, use unslotted version
	     rxBI = rxSfSpec.BI / phy_symbolrate;
	     tmpf = (rxSfSpec.finalCap + 1) * (aBaseSlotDuration * (1 << rxSfSpec.SO))/phy_symbolrate;
	     tmpf += bcnRxTime;
	     tmpf += aMaxLostBeacons * rxBI;

	     if (tmpf < now)
	     bPeriodsLeft = -1;
	     else
	     {*/

	    t_fCAP = schedBcnRxTime + (rxSfSpec.finalCap + 1) * rxSfSlotDuration / phy_symbolrate;
	    if(now >= t_fCAP)      // Check 1
	    {
		csmacaWaitNextBeacon = true;
		bPeriodsLeft = 0;   // rechoose random backoff
		EV << "[CSMA-CA]: cannot proceed because it's now NOT in CAP (incoming), resume at the beginning of next CAP" << endl;
		return false;
	    }
	    else
	    {
		// TODO TBD: battLifeExt
		/*if (rxSfSpec.battLifeExt)
		 ;//t_CAP = getBattLifeExtSlotNum();
		 else*/
		t_CAP = (rxSfSpec.finalCap + 1) * rxSfSlotDuration / aUnitBackoffPeriod - rxBcnDuration;
		EV << "[CSMA-CA]: total number of backoff periods in CAP (except for rxBcnDuration): " << t_CAP << endl;
		// calculate num of Backoff periods from the first backoff in CAP to now+wtime
		tmpf = now + wtime;
		tmpf -= schedBcnRxTime;
		double temp = SIMTIME_DBL(tmpf);
		tmpf = (temp / bPeriod);
		EV << "[CSMA-CA]: tmpf = " << tmpf << "; bPeriod  = " << bPeriod << endl;
		t_bPeriods = (UINT_16)(SIMTIME_DBL(tmpf) - rxBcnDuration);
		EV << "[CSMA-CA]: t_bPeriods = " << t_bPeriods << "; rxBcnDuration = " << (int)rxBcnDuration << endl;

		tmpf = now + wtime;
		tmpf -= schedBcnRxTime;
		if(fmod(tmpf, bPeriod) > 0.0)
		    t_bPeriods++;
		bPeriodsLeft = t_bPeriods - t_CAP; // backoff periods left for next superframe
	    }
	}
	else
	    // non-beacon as a device, but beacon enabled as a coordinator
	    bPeriodsLeft = -1;
    }

    EV << "[CSMA-CA]: bPeriodsLeft = " << bPeriodsLeft << endl;
    ok = true;
    if(bPeriodsLeft > 0)
	ok = false;
    else if(bPeriodsLeft == 0)
    {
	if((!toParent(tmpCsmaca)) && (!txSfSpec.battLifeExt))
	    ok = false;
	else if((toParent(tmpCsmaca)) && (!rxSfSpec.battLifeExt))
	    ok = false;
    }
    if(!ok)
    {
	EV << "[CSMA-CA]: cannot proceed because the chosen num of backoffs cannot finish before the end of current CAP, resume at the beginning of next CAP" << endl;
	// if as a device, need to track next beacon if it is not being tracked
	if(rxBO != 15)
	    if(!bcnRxTimer->isScheduled())
		startBcnRxTimer();
	csmacaWaitNextBeacon = true;
	return false;
    }

    // check 3
    // If backoff can finish before the end of CAP or sent in non-beacon, calculate the time needed to finish the transaction and evaluate it
    t_CCATime = 8.0 / phy_symbolrate;
    if(tmpCsmaca->getByteLength() <= aMaxSIFSFrameSize)
	t_IFS = mpib.macSIFSPeriod;
    else
	t_IFS = mpib.macLIFSPeriod;
    t_IFS = t_IFS / phy_symbolrate;

    if(!afterCCA)
    {
	t_transacTime = t_CCATime;              //first CCA time
	t_transacTime += csmacaLocateBoundary(toParent(tmpCsmaca), t_transacTime) - (t_transacTime); // locate boundary for second CCA
	t_transacTime += t_CCATime;             //second CCA time
    }
    t_transacTime += csmacaLocateBoundary(toParent(tmpCsmaca), t_transacTime) - (t_transacTime);        // locate boundary for transmission
    t_transacTime += calDuration(tmpCsmaca); // calculate packet transmission time

    if(csmacaAckReq)       // if ACK required
    {
	t_transacTime += mpib.macAckWaitDuration / phy_symbolrate; //ack. waiting time (this value does not include round trip propagation delay)
	//t_transacTime += 2*max_pDelay;    //round trip propagation delay (802.15.4 ignores this, but it should be there even though it is very small)
	t_transacTime += t_IFS; //IFS time -- not only ensure that the sender can finish the transaction, but also the receiver
    }
    else                    // no ACK required
    {
	t_transacTime += Ieee802154ePhy::aTurnaroundTime / phy_symbolrate; //transceiver turn-around time (receiver may need to do this to transmit next beacon)
	//t_transacTime += max_pDelay;      //one-way trip propagation delay (802.15.4 ignores this, but it should be there even though it is very small)
	t_transacTime += t_IFS; //IFS time -- not only ensure that the sender can finish the transaction, but also the receiver
    }

    tmpf = now + wtime;
    tmpf += t_transacTime;
    EV << "[CSMA-CA]: the entire transmission is estimated to finish at " << tmpf << " s" << endl;
    // calculate the time for the end of cap
    if(!toParent(tmpCsmaca))               // sent as a coordinator
    {
	if(mpib.macBeaconOrder != 15) // sent in beacon-enabled, check the end of tx cap
	    t_fCAP = getFinalCAP('t');
	else
	    // rxBO != 15, this case should be reconsidered, check the end of rx cap
	    t_fCAP = getFinalCAP('r');
    }
    else                            // sent as a device
    {
	if(rxBO != 15)        // sent in beacon-enabled, check the end of rx cap
	{
	    t_fCAP = getFinalCAP('r');
	}
	else
	    // mpib.macBeaconOrder != 15, this case should be reconsidered, check the end of rx cap
	    t_fCAP = getFinalCAP('t');
    }

    EV << "[CSMA-CA]: the current CAP will end at " << t_fCAP << " s" << endl;
    // evaluate if the entire transmission process can be finished before end of current CAP
    if(tmpf > t_fCAP)
    {
	ok = false;
	EV << "[CSMA-CA]: cannot proceed because the entire transmission cannot finish before the end of current cap" << endl;
    }
    else
    {
	ok = true;
	EV << "[CSMA-CA]: can proceed" << endl;
    }

    //check if have enough CAP to finish the transaction
    if(!ok)
    {
	bPeriodsLeft = 0; // in the next superframe, apply a further backoff delay before evaluating once again
	// if as a device, need to track next beacon if it is not being tracked
	if(rxBO != 15)
	    if(!bcnRxTimer->isScheduled())
		startBcnRxTimer();
	csmacaWaitNextBeacon = true;
	return false;
    }
    else
    {
	bPeriodsLeft = -1;
	return true;
    }
}

simtime_t Ieee802154eMac::getFinalCAP(char trxType)
{
    simtime_t txSlotDuration, rxSlotDuration, rxBI, txCAP, rxCAP;
    simtime_t now, oneDay, tmpf;

    now = simTime();
    oneDay = now + 24.0 * 3600;

    if((mpib.macBeaconOrder == 15) && (rxBO == 15))      //non-beacon enabled
	return oneDay;                        //transmission can always go ahead

    txSlotDuration = txSfSlotDuration / phy_symbolrate;
    rxSlotDuration = rxSfSlotDuration / phy_symbolrate;
    rxBI = rxSfSpec.BI / phy_symbolrate;

    if(trxType == 't')                 // check tx CAP
    {
	if(mpib.macBeaconOrder != 15)  // beacon enabled
	{
	    if(txSfSpec.battLifeExt)
	    {
		error("[MAC]: this function TBD");
		/* TODO
		 * tmpf = (beaconPeriods + getBattLifeExtSlotNum()) * aUnitBackoffPeriod;
		 t_CAP = bcnTxTime + tmpf;*/
	    }
	    else
	    {
		tmpf = (txSfSpec.finalCap + 1) * txSlotDuration;
		txCAP = mpib.macBeaconTxTime + tmpf;
	    }
	    return (txCAP >= now) ? txCAP:oneDay;
	}
	else
	    return oneDay;
    }
    else                                        // check rx CAP
    {
	if(rxBO != 15)                         // beacon enabled
	{
	    if(rxSfSpec.battLifeExt)
	    {
		error("[MAC]: this function TBD");
		/* TODO
		 tmpf = (beaconPeriods2 + getBattLifeExtSlotNum()) * aUnitBackoffPeriod;
		 t_CAP2 = bcnRxTime + tmpf;
		 */
	    }
	    else
	    {
		tmpf = (rxSfSpec.finalCap + 1) * rxSlotDuration;
		EV << "[getfinalcap]: now = " << now << endl;
		EV << "[getfinalcap]: tmpf = " << tmpf << endl;
		rxCAP = bcnRxTime + tmpf;
		EV << "[getfinalcap]: rxCAP = " << rxCAP << endl;
	    }

	    tmpf = aMaxLostBeacons * rxBI;
	    // adjust if beacon loss occurs or during receiving beacon
	    if((rxCAP < now) && (rxCAP + tmpf >= now)) //no more than <aMaxLostBeacons> beacons missed
	    {
		EV << "[MAC]: during receiving a beacon, now is: " << now << ", last beacon is received at " << bcnRxTime << endl;
		while(rxCAP < now)
		    rxCAP += rxBI;
	    }
	    return (rxCAP >= now) ? rxCAP:oneDay;
	}
	else
	    return oneDay;
    }
}

void Ieee802154eMac::csmaca_handle_RX_ON_confirm(PHYenum status)
{
    simtime_t now, wtime;

    if(status != phy_RX_ON)
    {
	if(status == phy_BUSY_TX)
	{
	    EV << "[CSMA-CA]: radio is busy Txing now, RX_ON will be set later" << endl;
	    taskP.taskStatus(TP_RX_ON_CSMACA) = true;
	    EV << "[MAC-TASK]: TP_RX_ON_CSMACA = true" << endl;
	}
	else
	{
	    EV << "[CSMA-CA]: RX_ON request did not succeed, try again" << endl;
	    handleBackoffTimer();
	}
	return;
    }

    // phy_RX_ON
    // locate backoff boundary if needed
    EV << "[CSMA-CA]: ok, radio is set to RX_ON " << endl;
    now = simTime();
    if(beaconEnabled)
    {
	EV << "[CSMA-CA]: locate backoff boundary before sending CCA request" << endl;
	wtime = csmacaLocateBoundary(toParent(tmpCsmaca), 0.0);
    }
    else
	wtime = 0.0;

    if(wtime == 0.0)
    {
	taskP.taskStatus(TP_CCA_CSMACA) = true;
	EV << "[MAC-TASK]: TP_CCA_CSMACA = true" << endl;
	PLME_CCA_request();
    }
    else
	startDeferCCATimer(wtime);
}

void Ieee802154eMac::csmacaTrxBeacon(char trx)
// To be called each time that a beacon received or transmitted if backoffStatus == 99
{
    simtime_t wtime;
    if(!txAck)
	PLME_SET_TRX_STATE_request(phy_RX_ON);

    //update values
    beaconEnabled = ((mpib.macBeaconOrder != 15) || (rxBO != 15));
    csmacaReset(beaconEnabled);

    if(csmacaWaitNextBeacon)
	if(tmpCsmaca && (!backoffTimer->isScheduled()))
	{
	    EV << "[CSMA-CA]:  triggered again by the starting of the new CAP" << endl;
	    ASSERT(bPeriodsLeft >= 0);
	    if(bPeriodsLeft == 0)      // backoff again
	    {
		EV << "[CSMA-CA]: bPeriodsLeft == 0, need to rechoose random number of backoffs" << endl;
		csmacaStart(false);
	    }
	    else // resume backoff: this is the second entry to CSMA-CA without calling <csmacaStart()>
	    {
		/*wtime = csmacaAdjustTime(0.0);
		 EV << "wtime = " << wtime << endl;*/
		EV << "[CSMA-CA]: bPeriodsLeft > 0, continue with the number of backoffs left from last time" << endl;
		wtime += bPeriodsLeft * bPeriod;
		if(csmacaCanProceed(wtime))
		    startBackoffTimer(wtime);
	    }

	    return;             // debug here
	}
    csmacaWaitNextBeacon = false;   // really necessary??
}

//-------------------------------------------------------------------------------/
/******* <MAC management service - Std 802.15.4-2011 (table 8) page 77> *********/
//-------------------------------------------------------------------------------/
/**@author: 2014    Stefan Reis
 * brief: MLME-ASSOCIATE.request is used by a device to request an association with a coordinator
 *
 * The MLME-ASSOCIATE.request primitive is used by a device to request an association with a coordinator.
 *
 * note: see Std 802.15.4-2011 (6.2.2.1) page 79
 * from MAC to MAC
 *
 * param[in]. */
void Ieee802154eMac::handle_MLME_ASSOCIATE_request(cMessage *msg)
{
    /*UINT_8 channelNumber, UINT_8 channelPage, Ieee802154eAddrMode coordAddrMode,
     UINT_16 coordPANId, IE3ADDR coordAddress, UINT_8 capabilityInformation,
     UINT_8 securityLevel, UINT_8 keyIdMode, UINT_64 keySource, UINT_8 keyIndex,
     UINT_64 lowLatencyNetworkInfo, UINT_16 channelOffset, UINT_8 hoppingSequenceID
     */
    Ieee802154eAssociationFrame *dataFrame = new Ieee802154eAssociationFrame(msg->getName(), msg->getKind());
    Ieee802154eNetworkCtrlInfo *AssReq = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    FrameCtrl frmCtrl;
    //Create Request
    AssReq->setCoordAddrMode(MAC_COORD_SHORT_ADDRESS);
    AssReq->setCoordPANId(mpib.macPANId);
    AssReq->setCoordAddress(mpib.macCoordShortAddress);
    AssReq->setChannelOffset(linkTable->getLink(0)->getChannelOffset());

    dataFrame->setCntrlInfo(*AssReq->dup());
    dataFrame->setSeqNmbr(mpib.macDSN++);
    //Create Frame Control
    frmCtrl.frameType = Ieee802154e_ASSOCIATION_REQUEST;
    frmCtrl.securityEnabled = secuData;
    frmCtrl.frmPending = (txBroadCmd == NULL) ? false:true;
    frmCtrl.ackReq = true;
    frmCtrl.seqNbSup = false;   // send Sequence number
    frmCtrl.dstAddrMode = defFrmCtrl_AddrMode16;
    frmCtrl.frameVersion = Ieee802154_compatible;
    if(mpib.macEBAutoSA == 0)  // NONE Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrModeNone;
    else if(mpib.macEBAutoSA == 1) // SHORT Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrMode16;
    else if(mpib.macEBAutoSA == 2) // FULL Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrMode64;

    IE3ADDR tmpDstAddr;
    IE3ADDR tmpSrcAddr;
    UINT_16 tmpDstPanId;
    UINT_16 tmpSrcPanId;

    if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode16)
    { // 16 bit address
	tmpDstPanId = mpib.macPANId;
	tmpDstAddr = (IE3ADDR)mpib.macCoordShortAddress;
    }
    else
    { // 64 bit address
	tmpDstPanId = mpib.macPANId;
	tmpDstAddr = (IE3ADDR)mpib.macCoordExtendedAddress;
    }

    if(frmCtrl.srcAddrMode == defFrmCtrl_AddrMode16)
    { // 16 bit address
	tmpSrcPanId = mpib.macPANId;
	tmpSrcAddr = (IE3ADDR)mpib.macShortAddress;
    }
    else
    { // 64 bit address
	tmpSrcPanId = mpib.macPANId;
	tmpSrcAddr = (IE3ADDR)mpib.macExtendedAddress;
    }

    dataFrame->setDstPanId(tmpDstPanId);
    dataFrame->setDstAddr(tmpDstAddr);
    dataFrame->setSrcPanId(tmpSrcPanId);
    dataFrame->setSrcAddr(tmpSrcAddr);
    frmCtrl.compPanID = getPANIDComp(frmCtrl, dataFrame->getSrcPanId(), dataFrame->getDstPanId());

    // security is not implemented
    AuxiliarySecurityHeader auxSecHd;
    auxSecHd.secLvl = ASH_SLVL_NONE;
    auxSecHd.keyIdentMode = 0;
    auxSecHd.keySource = 0;
    auxSecHd.keyIndex = 0;

    dataFrame->setFrmCtrl(frmCtrl);
    dataFrame->setAuxSecHd(auxSecHd);

    if(!queueModule->existAssReq(tmpDstAddr))
	queueModule->insertInQueue(dataFrame->dup());

    delete dataFrame;
    delete AssReq;

}

/**@author: 2014    Stefan Reis
 * brief: MLME-ASSOCIATE.indication is used to indicate the reception of an MLME-ASSOCIATE.request command
 *
 * The MLME-ASSOCIATE.indication primitive is used to indicate the reception of an association request
 * command.
 *
 * note: see Std 802.15.4-2011 (6.2.2.2) page 80
 * When the next higher layer of a coordinator receives the MLME-ASSOCIATE.indication primitive, the
 * coordinator determines whether to accept or reject the unassociated device using an algorithm outside the
 * scope of this standard.
 * from MAC to NETWORK
 *
 * param[in] */
void Ieee802154eMac::MLME_ASSOCIATE_indication(cMessage *msg)
{
    Ieee802154eAssociationFrame *tmp = check_and_cast<Ieee802154eAssociationFrame*>(msg);
    Ieee802154eNetworkCtrlInfo *primitive = new Ieee802154eNetworkCtrlInfo("AssociationIndication", TP_MLME_ASSOCIATE_INDICATION);

    primitive->setDeviceAddress(tmp->getSrcAddr().getInt());

    primitive->setChannelOffset(tmp->getCntrlInfo().getChannelOffset());

    EV << "[MAC]: sending a MLME-ASSOCIATE.indication to NETWORK" << endl;
    send(primitive->dup(), mSchedulerOut);
    delete primitive;
    delete tmp;
}

void Ieee802154eMac::handle_MLME_ASSOCIATE_responce(cMessage *msg)
{
    Ieee802154eAssociationFrame *dataFrame = new Ieee802154eAssociationFrame();
    Ieee802154eNetworkCtrlInfo *AssReq = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    FrameCtrl frmCtrl;
    //Create Request
    dataFrame->setName("AssociationResponse");
    dataFrame->setCntrlInfo(*AssReq->dup());
    //Create Frame Control
    frmCtrl.frameType = Ieee802154e_ASSOCIATION_RESPONCE;
    frmCtrl.securityEnabled = secuData;
    frmCtrl.frmPending = (txBroadCmd == NULL) ? false:true;
    frmCtrl.ackReq = true;
    frmCtrl.seqNbSup = false;   // send Sequence number
    frmCtrl.dstAddrMode = defFrmCtrl_AddrMode16;
    frmCtrl.frameVersion = Ieee802154_compatible;

    if(mpib.macEBAutoSA == 0)  // NONE Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrModeNone;
    else if(mpib.macEBAutoSA == 1) // SHORT Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrMode16;
    else if(mpib.macEBAutoSA == 2) // FULL Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrMode64;

    IE3ADDR tmpDstAddr;
    IE3ADDR tmpSrcAddr;
    UINT_16 tmpDstPanId;
    UINT_16 tmpSrcPanId;

    if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode16)
    { // 16 bit address
	tmpDstPanId = mpib.macPANId;
	tmpDstAddr = (IE3ADDR)AssReq->getDeviceAddress();
    }
    else
    { // 64 bit address
	tmpDstPanId = mpib.macPANId;
	tmpDstAddr = (IE3ADDR)AssReq->getDeviceAddress();
    }

    if(frmCtrl.srcAddrMode == defFrmCtrl_AddrMode16)
    { // 16 bit address
	tmpSrcPanId = mpib.macPANId;
	tmpSrcAddr = (IE3ADDR)mpib.macShortAddress;
    }
    else
    { // 64 bit address
	tmpSrcPanId = mpib.macPANId;
	tmpSrcAddr = (IE3ADDR)mpib.macExtendedAddress;
    }

    dataFrame->setDstPanId(tmpDstPanId);
    dataFrame->setDstAddr(tmpDstAddr);
    dataFrame->setSrcPanId(tmpSrcPanId);
    dataFrame->setSrcAddr(tmpSrcAddr);
    frmCtrl.compPanID = getPANIDComp(frmCtrl, dataFrame->getSrcPanId(), dataFrame->getDstPanId());

    // security is not implemented
    AuxiliarySecurityHeader auxSecHd;
    auxSecHd.secLvl = ASH_SLVL_NONE;
    auxSecHd.keyIdentMode = 0;
    auxSecHd.keySource = 0;
    auxSecHd.keyIndex = 0;

    dataFrame->setFrmCtrl(frmCtrl);
    dataFrame->setAuxSecHd(auxSecHd);

    if(!queueModule->existAssRes(tmpDstAddr))
    	queueModule->insertInQueue(dataFrame->dup());

    delete AssReq;
    delete dataFrame;
}

/**@author: 2014    Stefan Reis
 * brief: MLME-ASSOCIATE.confirm reports the result of a MLME-ASSOCIATE.request command
 *
 * The MLME-ASSOCIATE.confirm primitive is used to inform the next higher layer of the initiating device
 * whether its request to associate was successful or unsuccessful.
 *
 * note: see Std 802.15.4-2011 (6.2.2.4) page 81
 * from MAC to NETWORK
 *
 * param[in] */
void Ieee802154eMac::MLME_ASSOCIATE_confirm(cMessage *msg)
{
    Ieee802154eAssociationFrame *tmp = check_and_cast<Ieee802154eAssociationFrame *>(msg);
    Ieee802154eNetworkCtrlInfo *primitive = new Ieee802154eNetworkCtrlInfo("AssociationConfirm", TP_MLME_ASSOCIATE_CONFIRM);

    if(tmp->getCntrlInfo().getStatus() == mac_FastA_successful)
    {
	notAssociated = false;
	panCoorName = tmp->getSenderModule()->getFullName();
	mpib.macShortAddress = tmp->getCntrlInfo().getAssocShortAddress();
    }
    else
    {
	notAssociated = true;
    }
    primitive->setStatus(tmp->getCntrlInfo().getStatus());

    EV << "[MAC]: sending a MLME-ASSOCIATE.confirm to NETWORK" << endl;
    send(primitive->dup(), mSchedulerOut);

    delete primitive;
    delete tmp;

}

/**@author: 2014    Stefan Reis
 * brief: MLME-DISASSOCIATE.request is used by an associated device to notify the coordinator to leave the PAN
 *
 * The MLME-DISASSOCIATE.request primitive is used by an associated device to notify the coordinator of
 * its intent to leave the PAN. It is also used by the coordinator to instruct an associated device
 * to leave the PAN.
 *
 * note: see Std 802.15.4-2011 (6.2.3.1) page 83
 *
 * param[in] */
/*
 Ieee802154eAddrMode deviceAddrMode,
 UINT_16 devicePANId,
 IE3ADDR deviceAddress,
 UINT_8 disassociateReason,
 bool txIndirect,
 UINT_8 securityLevel,
 UINT_8 keyIdMode,
 UINT_64 keySource,
 UINT_8 keyIndex
 */
void Ieee802154eMac::handle_MLME_DISASSOCIATE_request(cMessage *msg)
{

}

/**@author: 2014    Stefan Reis
 * brief: MLME-DISASSOCIATE.indication is used to indicate the reception of an MLME-DISASSOCIATE.request command
 *
 * The MLME-DISASSOCIATE.indication primitive is used to indicate the reception of a disassociation
 * notification command.
 *
 * note: see Std 802.15.4-2011 (6.2.3.2) page 84
 * from MAC to NETWORK
 *
 * param[in]
 * IE3ADDR deviceAddress,
 * UINT_8 disassociateReason,
 * UINT_8 securityLevel,
 * UINT_8 keyIdMode,
 * UINT_64 keySource,
 * UINT_8 keyIndex */
void Ieee802154eMac::MLME_DISASSOCIATE_indication(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *primitive = check_and_cast<Ieee802154eNetworkCtrlInfo*>(msg);
    primitive->setKind(TP_MLME_DISASSOCIATE_INDICATION);
//    primitive->setDeviceAddress(deviceAddress.getInt());
//    primitive->setDisassociateReason(disassociateReason);
//    primitive->setSecurityLevel(securityLevel);
//    primitive->setKeyIdMode(keyIdMode);
//    primitive->setKeySource(keySource);
//    primitive->setKeyIndex(keyIndex);

    EV << "[MAC]: sending a MLME-DISASSOCIATE.indication to NETWORK" << endl;
    send(primitive, mSchedulerOut);
}

/**@author: 2014    Stefan Reis
 * brief: MLME-DISASSOCIATE.confirm reports the result of a MLME-DISASSOCIATE.request command
 *
 * The MLME-DISASSOCIATE.confirm primitive reports the results of an MLME-DISASSOCIATE.request primitive.
 *
 * note: see Std 802.15.4-2011 (6.2.3.3) page 85
 * from MAC to NETWORK
 *
 * param[in]
 * MACenum status,
 * Ieee802154eAddrMode deviceAddrMode,
 * UINT_16 devicePANId,
 * IE3ADDR deviceAddress*/
void Ieee802154eMac::MLME_DISASSOCIATE_confirm(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *primitive = check_and_cast<Ieee802154eNetworkCtrlInfo*>(msg);
//    primitive->setKind(TP_MLME_DISASSOCIATE_CONFIRM);
//    primitive->setStatus(status);
//    primitive->setDeviceAddrMode(deviceAddrMode);
//    primitive->setDevicePANId(devicePANId);
//    primitive->setDeviceAddress(deviceAddress.getInt());

    EV << "[MAC]: sending a MLME-DISASSOCIATE.confirm to NETWORK" << endl;
    send(primitive, mSchedulerOut);
}

/**@author: 2014    Stefan Reis
 * brief: MLME-BEACON-NOTIFY.indication is used to send parameters contained within a beacon frame
 *
 * The MLME-BEACON-NOTIFY.indication primitive is used to send parameters contained within a beacon
 * frame received by the MAC sublayer to the next higher layer when either macAutoRequest is set to FALSE
 * or when the beacon frame contains one or more octets of payload. The primitive also sends a measure of the
 * LQI and the time the beacon frame was received.
 *
 * note: see Std 802.15.4-2011 (6.2.4.1) page 86
 * from MAC to NETWORK
 *
 * param[in] */
void Ieee802154eMac::MLME_BEACON_NOTIFY_indication(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *primitive = new Ieee802154eNetworkCtrlInfo();
    primitive->setKind(TP_MLME_BEACON_NOTIFY_INDICATION);
//    primitive->setBsn(bsn);
//    primitive->setCoordAddrMode(panDescriptor.coordAddrMode);
//    primitive->setCoordPANId(panDescriptor.coordPANId);
//    primitive->setCoordAddress(panDescriptor.coordAddress_16_or_64.getInt());
//    primitive->setChannelNumber(panDescriptor.channelNumber);
//    primitive->setChannelPage(panDescriptor.channelPage);
//    primitive->setBeaconOrder(panDescriptor.superframeSpec.BO);
//    primitive->setSuperframeOrder(panDescriptor.superframeSpec.SO);
//    primitive->setFinalCAPSlot(panDescriptor.superframeSpec.finalCap);
//    primitive->setBatteryLifeExtension(panDescriptor.superframeSpec.battLifeExt);
//    primitive->setPanCoordinator(panDescriptor.superframeSpec.panCoor);
//    primitive->setAssociationPermit(panDescriptor.superframeSpec.assoPmt);
//    primitive->setGtsPermit(panDescriptor.gtsPermit);
//    primitive->setLinkQuality(panDescriptor.linkQuality);
//    primitive->setTimeStamp(panDescriptor.timeStamp.getScale());
//    primitive->setSecurityStatus(panDescriptor.securityStatus);
//    primitive->setSecurityLevel(panDescriptor.securityLevel);
//    primitive->setKeyIdMode(panDescriptor.keyIdMode);
//    primitive->setKeySource(panDescriptor.keySource);
//    primitive->setKeyIndex(panDescriptor.keyIndex);
//    // [SR] need to be done: codelist Problem: not fixed array
//    primitive->setNmbrShortAddrPending(pendAddrSpec.numShortAddr);
//    primitive->setNmbrExtAddrPending(pendAddrSpec.numExtendedAddr);
//    // [SR] NTB: pendAddrSpec addrlist[7]
//    // [SR] need to be done: addrList (bitmap)
//    primitive->setSduLength(sduLength);
//    // [SR] NTB: sdu (set of octets)
//    primitive->setEbsn(ebsn);
//    primitive->setBeaconType(beaconType);
//
    EV << "[MAC]: sending a MLME-BEACON-NOTIFY.indication to NETWORK" << endl;
    send(primitive->dup(), mSchedulerOut);
    delete primitive;
    delete msg;
}

/**@author: 2014    Stefan Reis
 * brief: MLME-COMM-STATUS.indication allows the MLME to indicate a communications status.
 *
 * The MLME-COMM-STATUS.indication primitive allows the MLME to indicate a communications status.
 *
 * note: see Std 802.15.4-2011 (6.2.4.2) page 88
 *
 * param[in] */
void Ieee802154eMac::MLME_COMM_STATUS_indication(UINT_16 panId, Ieee802154eAddrMode srcAddrMode, IE3ADDR srcAddr, Ieee802154eAddrMode dstAddrMode, IE3ADDR dstAddr, MACenum status, UINT_8 securityLevel, UINT_8 keyIdMode, UINT_64 keySource, UINT_8 keyIndex)
{
    Ieee802154eNetworkCtrlInfo *primitive = new Ieee802154eNetworkCtrlInfo();
    primitive->setKind(TP_MLME_COMM_STATUS_INDICATION);
    primitive->setPanId(panId);
    primitive->setSrcAddrMode(srcAddrMode);
    primitive->setSrcAddr(srcAddr.getInt());
    primitive->setDstAddrMode(dstAddrMode);
    primitive->setDstAddr(dstAddr.getInt());
    primitive->setStatus(status);
    primitive->setSecurityLevel(securityLevel);
    primitive->setKeyIdMode(keyIdMode);
    primitive->setKeySource(keySource);
    primitive->setKeyIndex(keyIndex);

    EV << "[MAC]: sending a MLME_COMM_STATUS.indication to NETWORK" << endl;
    send(primitive, upperLayerOut);
}

/**@author: 2014    Stefan Reis     (modified)
 * brief: MLME-GET.request requests information about a given PIB attribute
 *
 * The MLME-GET.request primitive requests information about a given PIB attribute.
 *
 * note: see Std 802.15.4-2011 (6.2.5.1) page 90
 *
 * param[in] */
void Ieee802154eMac::handle_MLME_GET_request(MACPIBenum pibAttribute)
{
    // used the code from the NS2 implementation [SR]
    MACenum t_status;

    switch(pibAttribute)
    {
	case MAC_EXTENDED_ADDRESS:
	case MAC_ACK_WAIT_DURATION:
	case MAC_ASSOCIATED_PAN_COORD:
	case MAC_ASSOCIATION_PERMIT:
	case MAC_AUTO_REQUEST:
	case MAC_BATT_LIFE_EXT:
	case MAC_BATT_LIFE_EXT_PERIODS:
	case MAC_BEACON_PAYLOAD:
	case MAC_BEACON_PAYLOAD_LENGTH:
	case MAC_BEACON_ORDER:
	case MAC_BEACON_TX_TIME:
	case MAC_BSN:
	case MAC_COORD_EXTENDED_ADDRESS:
	case MAC_COORD_SHORT_ADDRESS:
	case MAC_DSN:
	case MAC_GTS_PERMIT:
	case MAC_MAX_BE:
	case MAC_MAX_CSMA_BACKOFFS:
	case MAC_MAX_FRAME_RETRIES:
	case MAC_MIN_BE:
	case MAC_LIFS_PERIOD:
	case MAC_SIFS_PERIOD:
	case MAC_PAN_ID:
	case MAC_PROMISCUOUS_MODE:
	case MAC_RESPONSE_WAIT_TIME:
	case MAC_RX_ON_WHEN_IDLE:
	case MAC_SECURITY_ENABLED:
	case MAC_ENH_ACK_WAIT_DURATION:
	case MAC_IMPLICIT_BROADCAST:
	case MAC_SIMPLE_ADDRESS:
	case MAC_SHORT_ADDRESS:
	case MAC_SUPERFRAME_ORDER:
	case MAC_SYNC_SYMBOL_OFFSET:
	case MAC_TIMESTAMP_SUPPORTED:
	case MAC_TRANSACTION_PERSISTENCE_TIME:
	case MAC_TX_CONTROL_ACTIVE_DURATION:
	case MAC_TX_CONTROL_PAUSE_DURATION:
	case MAC_TX_TOTAL_DURATION:
	    // Security-related MAC PIB attributes - Std 802.15.4-2011 (table 60) page 142
	    //case MAC_KEY_TABLE:
	    //case MAC_DEVICE_TABLE:
	    //case MAC_SECURITY_LEVEL_TABLE:
	case MAC_FRAME_COUNTER:
	case MAC_AUTO_REQUEST_SECURITY_LEVEL:
	case MAC_AUTO_REQUEST_KEY_ID_MODE:
	    //case MAC_AUTO_REQUEST_KEY_SOURCE:
	case MAC_AUTO_REQUEST_KEY_INDEX:
	    //case MAC_DEFAULT_KEY_SOURCE:
	case MAC_PAN_COORD_EXTENDED_ADDRESS:
	case MAC_PAN_COORD_SHORT_ADDRESS:

	    // Security-related MAC PIB attributes - Std 802.15.4e-2012 (table 60) page 188
	case MAC_FRAME_COUNTER_MODE:
	    // General MAC PIB attributes for functional organization - Std 802.15.4e-2012 (table 52a) page 173
	case MAC_TSCH_CAPABLE:
	case MAC_LL_CAPABLE:
	case MAC_DSME_CAPABLE:
	case MAC_LE_CAPABLE:
	case MAC_RFID_CAPABLE:
	case MAC_HOPPING_CAPABLE:
	case MAC_AMCA_CAPABLE:
	case MAC_METRICS_CAPABLE:
	case MAC_TSCH_ENABLED:
	case MAC_LL_ENABLED:
	case MAC_DSME_ENABLED:
	case MAC_LE_ENABLED:
	case MAC_RFID_ENABLED:
	case MAC_HOPPING_ENABLED:
	case MAC_AMCA_ENABLED:
	case MAC_METRCIS_ENABLED:
	    // TSCH-specific MAC PIB attributes - Std 802.15.4e-2012 (table 52b) page 174
	case MAC_DISCONNECT_TIME:
	case MAC_JOIN_PRIORITY:
	case MAC_ASN:
	case MAC_NO_HL_BUFFERS:
	    t_status = mac_SUCCESS;
	    break;
	default:
	    t_status = mac_UNSUPPORTED_ATTRIBUTE;
	    break;
    }
    MLME_GET_confirm(t_status, pibAttribute, &mpib);
}

/**@author: 2014    Stefan Reis
 * brief: MLME-GET.confirm reports the result of a MLME-GET.request command
 *
 * The MLME-GET.confirm primitive reports the results of an information request from the PIB.
 *
 * note: see Std 802.15.4-2011 (6.2.5.2) page 91
 *
 * param[in] */
void Ieee802154eMac::MLME_GET_confirm(MACenum status, MACPIBenum pibAttribute, MAC_PIB *pibAttributeValue)
{
    Ieee802154eNetworkCtrlInfo *primitive = new Ieee802154eNetworkCtrlInfo();
    primitive->setKind(TP_MLME_GET_CONFIRM);
    primitive->setStatus(status);
    primitive->setPibAttribute(pibAttribute);
    primitive->setPibAttributeValue(*pibAttributeValue);

    EV << "[MAC]: sending a MLME_GET.confirm to NETWORK" << endl;
    send(primitive, upperLayerOut);
}

/**@author: 2014    Stefan Reis
 * brief: MLME-GTS.request allows a device to send a request to the PAN coordinator to allocate a new GTS
 *        or to deallocate an existing GTS.
 *
 * The MLME-GTS.request primitive allows a device to send a request to the PAN coordinator to allocate a
 * new GTS or to deallocate an existing GTS. This primitive is also used by the PAN coordinator to initiate a
 * GTS deallocation.
 *
 * note: see Std 802.15.4-2011 (6.2.6.1) page 91
 * from MAC to PHY
 *
 * param[in] */
void Ieee802154eMac::handle_MLME_GTS_request(UINT_8 gtsCharacteristics, UINT_8 securityLevel, UINT_8 keyIdMode, UINT_64 keySource, UINT_8 keyIndex)
{

}

/**@author: 2014    Stefan Reis
 * brief: MLME-GTS.confirm reports the result of a MLME-GTS.request command
 *
 * The MLME-GTS.confirm primitive reports the results of a request to allocate a new GTS or to deallocate an
 * existing GTS.
 *
 * note: see Std 802.15.4-2011 (6.2.6.2) page 92
 * from MAC to NETWORK
 *
 * param[in] */
//void Ieee802154eMac::MLME_GTS_confirm(GTS_Char_Field gtsCharacteristics, MACenum status)
//{
//    Ieee802154eNetworkCtrlInfo *primitive = new Ieee802154eNetworkCtrlInfo();
//    primitive->setKind(TP_MLME_GTS_CONFIRM);
//    primitive->setGtsLength(gtsCharacteristics.gtsLength);
//    primitive->setGtsDirection(gtsCharacteristics.gtsDirection);
//    primitive->setGtsCharType(gtsCharacteristics.gtsCharType);
//    primitive->setStatus(status);
//
//    EV << "[MAC]: sending a MLME_GTS.confirm to NETWORK" << endl;
//    send(primitive, upperLayerOut);
//}
/**@author: 2014    Stefan Reis
 * brief: MLME-GTS.indication indicates that a GTS has been allocated or that a previously allocated
 *        GTS has been deallocated.
 *
 * The MLME-GTS.indication primitive indicates that a GTS has been allocated or that a previously allocated
 * GTS has been deallocated.
 *
 * note: see Std 802.15.4-2011 (6.2.6.3) page 93
 *
 * param[in] */
//void Ieee802154eMac::MLME_GTS_indication(UINT_16 deviceAddress, GTS_Char_Field gtsCharacteristics, UINT_8 securityLevel, UINT_8 keyIdMode,
//        UINT_64 keySource, UINT_8 keyIndex)
//{
//    Ieee802154eNetworkCtrlInfo *primitive = new Ieee802154eNetworkCtrlInfo();
//    primitive->setKind(TP_MLME_GTS_INDICATION);
//    primitive->setDeviceAddress(deviceAddress);
//    primitive->setGtsLength(gtsCharacteristics.gtsLength);
//    primitive->setGtsDirection(gtsCharacteristics.gtsDirection);
//    primitive->setGtsCharType(gtsCharacteristics.gtsCharType);
//    primitive->setSecurityLevel(securityLevel);
//    primitive->setKeyIdMode(keyIdMode);
//    primitive->setKeySource(keySource);
//    primitive->setKeyIndex(keyIndex);
//
//    EV << "[MAC]: sending a MLME_GTS.indication to NETWORK" << endl;
//    send(primitive, upperLayerOut);
//}
/**@author: 2014    Stefan Reis
 * brief: The MLME-ORPHAN.indication is generated by the MLME of a coordinator
 *
 * The MLME-ORPHAN.indication primitive is generated by the MLME of a coordinator and issued to its
 * next higher layer on receipt of an orphan notification command, as defined in 5.3.6.
 *
 * note: see Std 802.15.4-2011 (6.2.7.1) page 94
 *
 * param[in] */
void Ieee802154eMac::MLME_ORPHAN_indication(IE3ADDR orphanAddress, UINT_8 securityLevel, UINT_8 keyIdMode, UINT_64 keySource, UINT_8 keyIndex)
{

}

/**@author: 2014    Stefan Reis
 * brief: MLME-ORPHAN.response allows the next higher layer of a coord to respond to the MLME-ORPHAN.indication
 *
 * The MLME-ORPHAN.response primitive allows the next higher layer of a coordinator to respond to the
 * MLME-ORPHAN.indication primitive.
 *
 * note: see Std 802.15.4-2011 (6.2.7.2) page 95
 *
 * param[in] */
void Ieee802154eMac::MLME_ORPHAN_responce(IE3ADDR orphanAddress, UINT_16 shortAddress, bool associatedMember, UINT_8 securityLevel, UINT_8 keyIdMode, UINT_64 keySource, UINT_8 keyIndex)
{

}

/**@author: 2014    Stefan Reis
 * brief: MLME-RESET.request is used by the next higher layer to request that the MLME performs a reset operation
 *
 * The MLME-RESET.request primitive is used by the next higher layer to request that the MLME performs a
 * reset operation
 *
 * note: see Std 802.15.4-2011 (6.2.8.1) page 96
 *
 * param[in] */
void Ieee802154eMac::handle_MLME_RESET_request(bool setDefaultPIB)
{

}

/**@author: 2014    Stefan Reis
 * brief: MLME-RESET.confirm reports the results of the reset operation.
 *
 * The MLME-RESET.confirm primitive reports the results of the reset operation.
 *
 * note: see Std 802.15.4-2011 (6.2.8.2) page 97
 *
 * param[in] */
void Ieee802154eMac::MLME_RESET_confirm(MACenum status)
{
    Ieee802154eNetworkCtrlInfo *primitive = new Ieee802154eNetworkCtrlInfo();
    primitive->setKind(TP_MLME_RESET_CONFIRM);
    primitive->setStatus(status);

    EV << "[MAC]: sending a MLME_RESET.confirm to NETWORK" << endl;
    send(primitive, upperLayerOut);
}

/**@author: 2014    Stefan Reis
 * brief: MLME-RX-ENABLE.request allows the next higher layer to request that the receiver is either
 *        enabled for a finite period of time or disabled.
 *
 * The MLME-RX-ENABLE.request primitive allows the next higher layer to request that the receiver is either
 * enabled for a finite period of time or disabled.
 *
 * note: see Std 802.15.4-2011 (6.2.9.1) page 97
 * from NETWORK to MAC (this option is optional)
 *
 * param[in] */
void Ieee802154eMac::handle_MLME_RX_ENABLE_request(bool deferPermit, UINT_32 rxOnTime, UINT_32 rxOnDuration, RangingControl rangingRxControl)
{

}

/**@author: 2014    Stefan Reis
 * brief: MLME-RX-ENABLE.confirm reports the results of the MLME-RX-ENABLE.request command
 *
 * The MLME-RX-ENABLE.confirm primitive reports the results of the attempt to enable or disable the receiver.
 *
 * note: see Std 802.15.4-2011 (6.2.9.2) page 99
 *
 * param[in] */
void Ieee802154eMac::MLME_RX_ENABLE_confirm(MACenum status)
{
    Ieee802154eNetworkCtrlInfo *primitive = new Ieee802154eNetworkCtrlInfo();
    primitive->setKind(TP_MLME_RX_ENABLE_CONFIRM);
    primitive->setStatus(status);

    EV << "[MAC]: sending a MLME_RX_ENABLE.confirm to NETWORK" << endl;
    send(primitive, upperLayerOut);
}

/**@author: 2014    Stefan Reis
 * brief: MLME-SCAN.request is used to initiate a channel scan over a given list of channels.
 *
 * The MLME-SCAN.request primitive is used to initiate a channel scan over a given list of channels.
 *
 * note: see Std 802.15.4-2011 (6.2.10.1) page 99
 *
 * param[in]
 * Ieee802154eChannelScanType scanType,UINT_64 scanChannels, UINT_8 scanDuration, UINT_8 channelPage,
 UINT_8 securityLevel, UINT_8 keyIdMode, UINT_64 keySource, UINT_8 keyIndex, bool linkQualityScan,
 bool* frameControlOptions[], unsigned int frameControlOptions_length, UINT_8* headerIElist[],
 unsigned int headerIElist_length, UINT_8* payloadIElist[],unsigned int payloadIElist_length*/
void Ieee802154eMac::handle_MLME_SCAN_request(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *scanReq = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    if(scanReq->getScanType() == 0x02)
    {
	PHY_PIB tmpPIB;
	tmpPIB.phyCurrentChannel = scanReq->getChannel();
	PLME_SET_request(PHY_CURRENT_CHANNEL, tmpPIB);
	PLME_SET_TRX_STATE_request(phy_RX_ON);
	isSCAN = true;
    }
    else if(scanReq->getScanType() == 0x00)
    {
	PHY_PIB tmpPIB;
	tmpPIB.phyCurrentChannel = scanReq->getChannel();
	PLME_SET_request(PHY_CURRENT_CHANNEL, tmpPIB);
	PLME_SET_TRX_STATE_request(phy_TRX_OFF);
	resetTRX();
	isSCAN = false;
    }
    delete scanReq;

}

/**@author: 2014    Stefan Reis
 * brief: MLME-SCAN.confirm reports the results of the MLME-SCAN.request command
 *
 * The MLME-SCAN.confirm primitive reports the result of the channel scan request.
 *
 * note: see Std 802.15.4-2011 (6.2.9.2) page 99
 *
 * param[in]
 * MACenum status, Ieee802154eChannelScanType scanType, UINT_8 channelPage, std::vector<int> unscannedChannels,
 UINT_8 resultListSize, std::vector<int> energyDetectList, std::vector<PAN_ELE> panDescriptorList,
 UINT_8 detectedCategory, std::vector<int> uwbEnergyDetectList
 * */
void Ieee802154eMac::MLME_SCAN_confirm(cMessage *msg)
{
    Ieee802154EnhancedBeaconFrame * frame = check_and_cast<Ieee802154EnhancedBeaconFrame *>(msg);
    frame->setName("ScanConfirm");
    frame->setKind(TP_MLME_SCAN_CONFIRM);
    send(frame->dup(), mSchedulerOut);
    delete frame;
}

/**@author: 2014    Stefan Reis
 * brief: MLME-SET.request primitive set the given value to the PIB attribute.
 *
 * The MLME-SET.request primitive attempts to write the given value to the indicated PIB attribute.
 *
 * note: see Std 802.15.4-2011 (6.2.11.1) page 102
 *
 * param[in]    PIBAttribute         Octet string    The name of the PIB attribute to write.
 *              PIBAttributeValue    Various         The value to write to the indicated PIB attribute. */
void Ieee802154eMac::handle_MLME_SET_request(MACPIBenum pibAttribute, MAC_PIB pibAttributeValue)
{
    // used the code from the NS2 implementation [SR]
    PHYenum p_state;
    MACenum t_status;
    t_status = mac_SUCCESS;

    switch(pibAttribute)
    {
	case MAC_EXTENDED_ADDRESS:
	{
	    mpib.macExtendedAddress = pibAttributeValue.macExtendedAddress;
	}
	case MAC_ACK_WAIT_DURATION:
	{
	    PLME_GET_request(PHY_CURRENT_CHANNEL); //value will be returned in ppib
	    if(((ppib.phyCurrentChannel <= 10) && (pibAttributeValue.macAckWaitDuration != 120)) || ((ppib.phyCurrentChannel > 10) && (pibAttributeValue.macAckWaitDuration != 54)))
	    {
		t_status = mac_INVALID_PARAMETER;
	    }
	    else
	    {
		mpib.macAckWaitDuration = pibAttributeValue.macAckWaitDuration;
	    }
	    break;
	}
	case MAC_ASSOCIATED_PAN_COORD:
	{
	    mpib.macAssociatedPANCoord = pibAttributeValue.macAssociatedPANCoord;
	    break;
	}
	case MAC_ASSOCIATION_PERMIT:
	{
	    mpib.macAssociationPermit = pibAttributeValue.macAssociationPermit;
	    break;
	}
	case MAC_AUTO_REQUEST:
	{
	    mpib.macAutoRequest = pibAttributeValue.macAutoRequest;
	    break;
	}
	case MAC_BATT_LIFE_EXT:
	{
	    mpib.macBattLifeExt = pibAttributeValue.macBattLifeExt;
	    break;
	}
	case MAC_BATT_LIFE_EXT_PERIODS:
	{
	    PLME_GET_request(PHY_CURRENT_CHANNEL); //value will be returned in ppib
	    if(((ppib.phyCurrentChannel <= 10) && (pibAttributeValue.macBattLifeExtPeriods != 8)) || ((ppib.phyCurrentChannel > 10) && (pibAttributeValue.macBattLifeExtPeriods != 6)))
	    {
		t_status = mac_INVALID_PARAMETER;
	    }
	    else
	    {
		mpib.macBattLifeExtPeriods = pibAttributeValue.macBattLifeExtPeriods;
	    }
	    break;
	}
	case MAC_BEACON_PAYLOAD:
	{
	    //<macBeaconPayloadLength> should be set first
	    memcpy(mpib.macBeaconPayload, pibAttributeValue.macBeaconPayload, mpib.macBeaconPayloadLength);
	    break;
	}
	case MAC_BEACON_PAYLOAD_LENGTH:
	{
	    if(pibAttributeValue.macBeaconPayloadLength > aMaxBeaconPayloadLength)
	    {
		t_status = mac_INVALID_PARAMETER;
	    }
	    else
	    {
		mpib.macBeaconPayloadLength = pibAttributeValue.macBeaconPayloadLength;
	    }
	    break;
	}
	case MAC_BEACON_ORDER:
	{
	    if(pibAttributeValue.macBeaconOrder > 15)
	    {
		t_status = mac_INVALID_PARAMETER;
	    }
	    else
	    {
		mpib.macBeaconOrder = pibAttributeValue.macBeaconOrder;
	    }
	    break;
	}
	case MAC_BEACON_TX_TIME:
	{
	    if(pibAttributeValue.macBeaconTxTime > 0xffffff)
	    {
		t_status = mac_INVALID_PARAMETER;
	    }
	    else
	    {
		mpib.macBeaconTxTime = pibAttributeValue.macBeaconTxTime;
	    }
	    break;
	}
	case MAC_BSN:
	{
	    mpib.macBSN = pibAttributeValue.macBSN;
	    break;
	}
	case MAC_COORD_EXTENDED_ADDRESS:
	{
	    mpib.macCoordExtendedAddress = pibAttributeValue.macCoordExtendedAddress;
	    break;
	}
	case MAC_COORD_SHORT_ADDRESS:
	{
	    mpib.macCoordShortAddress = pibAttributeValue.macCoordShortAddress;
	    break;
	}
	case MAC_DSN:
	{
	    mpib.macDSN = pibAttributeValue.macDSN;
	    break;
	}
	case MAC_GTS_PERMIT:
	{
	    mpib.macGTSPermit = pibAttributeValue.macGTSPermit;
	    break;
	}
	case MAC_MAX_BE:
	{
	    if(pibAttributeValue.macMaxBE < 3 || pibAttributeValue.macMaxBE > 8)
	    {
		t_status = mac_INVALID_PARAMETER;
	    }
	    else
	    {
		mpib.macMaxBE = pibAttributeValue.macMaxBE;
	    }
	    break;
	}
	case MAC_MAX_CSMA_BACKOFFS:
	{
	    if(pibAttributeValue.macMaxCSMABackoffs > 5)
	    {
		t_status = mac_INVALID_PARAMETER;
	    }
	    else
	    {
		mpib.macMaxCSMABackoffs = pibAttributeValue.macMaxCSMABackoffs;
	    }
	    break;
	}
	case MAC_MAX_FRAME_RETRIES:
	{
	    if(pibAttributeValue.macMaxFrameRetries > 7)
	    {
		t_status = mac_INVALID_PARAMETER;
	    }
	    else
	    {
		mpib.macMaxFrameRetries = pibAttributeValue.macMaxFrameRetries;
	    }
	    break;
	}
	case MAC_MIN_BE:
	{
	    if(pibAttributeValue.macMinBE > mpib.macMaxBE)
	    {
		t_status = mac_INVALID_PARAMETER;
	    }
	    else
	    {
		mpib.macMinBE = pibAttributeValue.macMinBE;
	    }
	    break;
	}
	case MAC_LIFS_PERIOD:
	{
	    mpib.macLIFSPeriod = pibAttributeValue.macLIFSPeriod;
	    break;
	}
	case MAC_SIFS_PERIOD:
	{
	    mpib.macSIFSPeriod = pibAttributeValue.macSIFSPeriod;
	    break;
	}
	case MAC_PAN_ID:
	{
	    mpib.macPANId = pibAttributeValue.macPANId;
	    break;
	}
	case MAC_PROMISCUOUS_MODE:
	{
	    mpib.macPromiscuousMode = pibAttributeValue.macPromiscuousMode;

	    //TODO [SR] need to be check
	    //some other operations (refer to sec. 7.5.6.6)
	    mpib.macRxOnWhenIdle = pibAttributeValue.macPromiscuousMode;

	    p_state = mpib.macRxOnWhenIdle ? phy_RX_ON:phy_TRX_OFF;
	    PLME_SET_TRX_STATE_request(p_state);
	    break;
	}
	case MAC_RESPONSE_WAIT_TIME:
	{
	    if(pibAttributeValue.macResponseWaitTime < 2 || pibAttributeValue.macResponseWaitTime > 64)
	    {
		t_status = mac_INVALID_PARAMETER;
	    }
	    else
	    {
		mpib.macResponseWaitTime = pibAttributeValue.macResponseWaitTime;
	    }
	    break;
	}
	case MAC_RX_ON_WHEN_IDLE:
	{
	    mpib.macRxOnWhenIdle = pibAttributeValue.macRxOnWhenIdle;
	    break;
	}
	case MAC_SECURITY_ENABLED:
	{
	    mpib.macSecurityEnabled = pibAttributeValue.macSecurityEnabled;
	    break;
	}
	case MAC_ENH_ACK_WAIT_DURATION:
	{
	    mpib.macEnhAckWaitDuration = pibAttributeValue.macEnhAckWaitDuration;
	    break;
	}
	case MAC_IMPLICIT_BROADCAST:
	{   // from Std 802.15.4e-2012 page 171
	    mpib.macImplicitBroadcast = pibAttributeValue.macImplicitBroadcast;
	    break;
	}
	case MAC_SIMPLE_ADDRESS:
	{   // from Std 802.15.4e-2012 page 171
	    mpib.macSimpleAddress = pibAttributeValue.macSimpleAddress;
	    break;
	}
	case MAC_SHORT_ADDRESS:
	{   // from Std 802.15.4e-2012 page 172
	    mpib.macShortAddress = pibAttributeValue.macShortAddress;
	    break;
	}
	case MAC_SUPERFRAME_ORDER:
	{
	    if(pibAttributeValue.macSuperframeOrder > 15)
	    {
		t_status = mac_INVALID_PARAMETER;
	    }
	    else
	    {
		mpib.macSuperframeOrder = pibAttributeValue.macSuperframeOrder;
	    }
	    break;
	}
	case MAC_SYNC_SYMBOL_OFFSET:
	{
	    PLME_GET_request(PHY_CURRENT_CHANNEL); //value will be returned in ppib
	    if(((ppib.phyCurrentChannel <= 10) && (pibAttributeValue.macSyncSymbolOffset > 0x400)) || ((ppib.phyCurrentChannel > 10) && (pibAttributeValue.macSyncSymbolOffset > 0x100)))
	    {
		t_status = mac_INVALID_PARAMETER;
	    }
	    else
	    {
		mpib.macSyncSymbolOffset = pibAttributeValue.macSyncSymbolOffset;
	    }
	    break;
	}
	case MAC_TIMESTAMP_SUPPORTED:
	{
	    mpib.macTimestampSupported = pibAttributeValue.macTimestampSupported;
	    break;
	}
	case MAC_TRANSACTION_PERSISTENCE_TIME:
	{
	    mpib.macTransactionPersistenceTime = pibAttributeValue.macTransactionPersistenceTime;
	    break;
	}
	case MAC_TX_CONTROL_ACTIVE_DURATION:
	{
	    if(pibAttributeValue.macTxControlActiveDuration > 100000)
	    {
		t_status = mac_INVALID_PARAMETER;
	    }
	    else
	    {
		mpib.macTxControlActiveDuration = pibAttributeValue.macTxControlActiveDuration;
	    }
	    break;
	}
	case MAC_TX_CONTROL_PAUSE_DURATION:
	{
	    if(pibAttributeValue.macTxControlPauseDuration != 2000 || pibAttributeValue.macTxControlPauseDuration != 10000)
	    {
		t_status = mac_INVALID_PARAMETER;
	    }
	    else
	    {
		mpib.macTxControlPauseDuration = pibAttributeValue.macTxControlPauseDuration;
	    }
	    break;
	}
	case MAC_TX_TOTAL_DURATION:
	{
	    mpib.macTxTotalDuration = pibAttributeValue.macTxTotalDuration;
	    break;
	}
	    // Security-related MAC PIB attributes - Std 802.15.4-2011 (table 60) page 142
	case MAC_FRAME_COUNTER:
	{
	    if(((mpib.macFrameCounterMode == 0x04) && (pibAttributeValue.macFrameCounter > 0xffffffff)) || ((mpib.macFrameCounterMode == 0x05) && (pibAttributeValue.macFrameCounter > 0xffffffffff)))
	    {
		t_status = mac_INVALID_PARAMETER;
	    }
	    else
	    {
		mpib.macFrameCounter = pibAttributeValue.macFrameCounter;
	    }
	    break;
	}
	case MAC_AUTO_REQUEST_SECURITY_LEVEL:
	{
	    mpib.macAutoRequestSecurityLevel = pibAttributeValue.macAutoRequestSecurityLevel;
	    break;
	}
	case MAC_AUTO_REQUEST_KEY_ID_MODE:
	{
	    if(pibAttributeValue.macAutoRequestKeyIdMode > 0x03)
	    {
		t_status = mac_INVALID_PARAMETER;
	    }
	    else
	    {
		mpib.macAutoRequestKeyIdMode = pibAttributeValue.macAutoRequestKeyIdMode;
	    }
	    break;
	}
	case MAC_AUTO_REQUEST_KEY_INDEX:
	{
	    if(pibAttributeValue.macAutoRequestKeyIndex < 0x01)
	    {
		t_status = mac_INVALID_PARAMETER;
	    }
	    else
	    {
		mpib.macAutoRequestKeyIndex = pibAttributeValue.macAutoRequestKeyIndex;
	    }
	    break;
	}
	case MAC_PAN_COORD_EXTENDED_ADDRESS:
	{
	    mpib.macPANCoordExtendedAddress = pibAttributeValue.macPANCoordExtendedAddress;
	    break;
	}
	case MAC_PAN_COORD_SHORT_ADDRESS:
	{
	    mpib.macPANCoordShortAddress = pibAttributeValue.macPANCoordShortAddress;
	    break;
	}
	    // Security-related MAC PIB attributes - Std 802.15.4e-2012 (table 60) page 188
	case MAC_FRAME_COUNTER_MODE:
	{
	    if(pibAttributeValue.macFrameCounterMode < 0x04 || pibAttributeValue.macFrameCounterMode > 0x05)
	    {
		t_status = mac_INVALID_PARAMETER;
	    }
	    else
	    {
		mpib.macFrameCounterMode = pibAttributeValue.macFrameCounterMode;
	    }
	    break;
	}
	    // General MAC PIB attributes for functional organization - Std 802.15.4e-2012 (table 52a) page 173
	case MAC_TSCH_CAPABLE:
	{
	    mpib.macTSCHcapable = pibAttributeValue.macTSCHcapable;
	    break;
	}
	case MAC_LL_CAPABLE:
	{
	    mpib.macLLcapable = pibAttributeValue.macLLcapable;
	    break;
	}
	case MAC_DSME_CAPABLE:
	{
	    mpib.macDSMEcapable = pibAttributeValue.macDSMEcapable;
	    break;
	}
	case MAC_LE_CAPABLE:
	{
	    mpib.macLEcapable = pibAttributeValue.macLEcapable;
	    break;
	}
	case MAC_RFID_CAPABLE:
	{
	    mpib.macRFIDcapable = pibAttributeValue.macRFIDcapable;
	    break;
	}
	case MAC_HOPPING_CAPABLE:
	{
	    mpib.macHoppingCapable = pibAttributeValue.macHoppingCapable;
	    break;
	}
	case MAC_AMCA_CAPABLE:
	{
	    mpib.macAMCACapable = pibAttributeValue.macAMCACapable;
	    break;
	}
	case MAC_METRICS_CAPABLE:
	{
	    mpib.macMetricsCapable = pibAttributeValue.macMetricsCapable;
	    break;
	}
	case MAC_TSCH_ENABLED:
	{
	    mpib.macTSCHenabled = pibAttributeValue.macTSCHenabled;
	    break;
	}
	case MAC_LL_ENABLED:
	{
	    mpib.macLLenabled = pibAttributeValue.macLLenabled;
	    break;
	}
	case MAC_DSME_ENABLED:
	{
	    mpib.macDSMEenabled = pibAttributeValue.macDSMEenabled;
	    break;
	}
	case MAC_LE_ENABLED:
	{
	    mpib.macLEenabled = pibAttributeValue.macLEenabled;
	    break;
	}
	case MAC_RFID_ENABLED:
	{
	    mpib.macRFIDenabled = pibAttributeValue.macRFIDenabled;
	    break;
	}
	case MAC_HOPPING_ENABLED:
	{
	    mpib.macHoppingEnabled = pibAttributeValue.macHoppingEnabled;
	    break;
	}
	case MAC_AMCA_ENABLED:
	{
	    mpib.macAMCAenabled = pibAttributeValue.macAMCAenabled;
	    break;
	}
	case MAC_METRCIS_ENABLED:
	{
	    mpib.macMetricsEnabled = pibAttributeValue.macMetricsEnabled;
	    break;
	}
	    // TSCH-specific MAC PIB attributes - Std 802.15.4e-2012 (table 52b) page 174
	case MAC_DISCONNECT_TIME:
	{
	    mpib.macDisconnectTime = pibAttributeValue.macDisconnectTime;
	    break;
	}
	case MAC_JOIN_PRIORITY:
	{
	    if(pibAttributeValue.macJoinPriority > 0x3f)
	    {
		t_status = mac_INVALID_PARAMETER;
	    }
	    else
	    {
		mpib.macJoinPriority = pibAttributeValue.macJoinPriority;
	    }
	    break;
	}
	case MAC_ASN:
	{
	    mpib.macASN = pibAttributeValue.macASN;
	    break;
	}
	case MAC_NO_HL_BUFFERS:
	{
	    mpib.macNoHLBuffers = pibAttributeValue.macNoHLBuffers;
	    break;
	}

	default:
	{
	    t_status = mac_UNSUPPORTED_ATTRIBUTE;
	    break;
	}
    }
    MLME_SET_confirm(t_status, pibAttribute);
}

/**@author: 2014    Stefan Reis
 * brief: MLME-SET.confirm primitive reports the results of an attempt to write a value to a PIB attribute
 *
 * The MLME-SET.confirm primitive reports the results of an attempt to write a value to a PIB attribute.
 *
 * note: see Std 802.15.4-2011 (6.2.11.2) page 103
 *
 * param[in]    status          Enumeration     The result of the request to write the PIB attribute.
 *              PIBAttribute    Octet string    The name of the PIB attribute that was written. */
void Ieee802154eMac::MLME_SET_confirm(MACenum status, MACPIBenum pibAttribute)
{
    Ieee802154eNetworkCtrlInfo *primitive = new Ieee802154eNetworkCtrlInfo();
    primitive->setKind(TP_MLME_SET_CONFIRM);
    primitive->setStatus(status);
    primitive->setPibAttribute(pibAttribute);

    EV << "[MAC]: sending a MLME_SET.confirm to NETWORK" << endl;
    send(primitive, upperLayerOut);
}

/**@author: 2014    Stefan Reis
 * brief: The MLME-START.request primitive is used by the PAN coordinator to initiate a new PAN or to begin
 *        using a new superframe configuration. This primitive is also used by a device already associated with an
 *        existing PAN to begin using a new superframe configuration.
 *
 * The MLME-SYNC-LOSS.indication primitive is generated by the MLME of a device and issued to its next
 * higher layer in the event of a loss of synchronization with the coordinator. It is also generated by the MLME
 * of the PAN coordinator and issued to its next higher layer in the event of either a PAN ID conflict or an
 * overlap between the outgoing superframe and the incoming superframe, as described in 5.1.1.2.
 *
 * note: see Std 802.15.4-2011 (table 36) page 104
 *
 * param1[in]:    UINT_16                   panId:                          The PAN identifier to be used by the device
 * param2[in]:    UINT_8                    channelNumber:                  The channel number to use
 * param3[in]:    UINT_8                    channelPage:                    The channel page to use
 * param4[in]:    UINT_32                   startTime:                      The time at which to begin transmitting beacons.
 * param5[in]:    UINT_8                    beaconOrder:                    Indicates the frequency with which the beacon is transmitted
 * param6[in]:    UINT_8                    superframeOrder:                The length of the active portion of the superframe, including the beacon frame
 * param7[in]:    bool                      panCoordinator:                 If this value is TRUE, the device will become the PAN coordinator of a new PAN. If this value is FALSE, the
 *                                                                          device will begin using a new superframe configuration on the PAN with which it is associated.
 * param8[in]:    bool                      batteryLifeExtension:           If this value is TRUE, the receiver of the beaconing device is disabled mac-BattLifeExtPeriods full backoff periods
 *                                                                          after the interframe spacing (IFS) period following the beacon frame. If this value is FALSE, the receiver of
 *                                                                          the beaconing device remains enabled for the entire CAP. This parameter is ignored if the BeaconOrder parameter
 *                                                                          has a value of 15.
 * param9[in]:    bool                      coordRealignment:               TRUE if a coordinator realignment command is to be transmitted prior to changing the superframe configuration or FALSE otherwise.
 * param10[in]:   UINT_8                    coordRealignSecurityLevel:      The security level to be used for coordinator realignment command frames, as described in Table 58.
 * param11[in]:   UINT_8                    coordRealignKeyIdMode:          The mode used to identify the key to be used, as described in 7.4.1.2. This parameter is ignored if the CoordRealignSecurityLevel
 *                                                                          parameter is set to 0x00
 * param12[in]:   UINT_64                   coordRealignKeySource:          The originator of the key to be used, as described in 7.4.3.1. This parameter is ignored if the CoordRealign-KeyIdMode parameter
 *                                                                          is ignored or is set to 0x00.
 * param13[in]:   UINT_8                    coordRealignKeyIndex:           The index of the key to be used, as described in 7.4.3.2. This parameter is ignored if the CoordRealignKeyId-Mode parameter
 *                                                                          is ignored or is set to 0x00.
 * param14[in]:   UINT_8                    beaconSecurityLevel:            The security level to be used for beacon frames, as described in Table 58.
 * param15[in]:   UINT_8                    beaconKeyIdMode:                The mode used to identify the key to be used, as described in Table 59. This parameter is ignored if the BeaconSecurityLevel
 *                                                                          parameter is set to 0x00.
 * param16[in]:   UINT_64                   beaconKeySource:                The originator of the key to be used, as described in 7.4.3.1. This parameter is ignored if the BeaconKeyId-
 *                                                                          Mode parameter is ignored or set to 0x00.
 * param17[in]:   UINT_8                    beaconKeyIndex:                 The index of the key to be used, as described in 7.4.3.2. This parameter is ignored if the BeaconKeyIdMode
 *                                                                          parameter is ignored or set to 0x00.
 * param18[in]:   IE_DSME_PAN               dsmeSuperframeSpecification:    Specifies the superframe configuration in the DSME-enabled PAN. Refer to 5.2.4.9.1.
 * param18[in]:   IE_DSME_PAN_BeaconBitmap  beaconBitmap:                   Specifies beacon bitmap. Refer to 5.2.4.9.3.
 * param19[in]:   HOPP_DESC                 hoppingDescriptor:              Specifies channel hopping information. Refer to Table 34a. */
void Ieee802154eMac::handle_MLME_START_request(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *startRe = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    if(startRe->getPanCoordinator())
    {
	mpib.macBeaconOrder = par("BO");
	mpib.macSuperframeOrder = par("SO");
	PLME_SET_TRX_STATE_request(phy_TRX_OFF);
	// transmit one enhanced beacon, that all the devices have the time and the ASN
	//constructBCN(); // construct beacon, save it in the frame queue
	// start ASN
	BE = mpib.macMinBE; // for shared links (TSCH CSMA-CA)
	scheduleAt(simTime() + (double)startRe->getStartTime(), asnTimer);
	//startDevice();
    }
    else
    {
	startDevice();
    }

    MLME_START_confirm(mac_SUCCESS);
    delete startRe;
}

/**@author: 2014    Stefan Reis
 * brief: The MLME-START.confirm primitive reports the results of the attempt to start using a new superframe
 *        configuration.
 *
 * The MLME-START.confirm primitive is generated by the MLME and issued to its next higher layer in
 * response to an MLME-START.request primitive. The MLME-START.confirm primitive returns a status of
 * either SUCCESS, indicating that the MAC sublayer has started using the new superframe configuration, or
 * the appropriate error code as follows:
 *  � NO_SHORT_ADDRESS � The macShortAddress is set to 0xffff.
 *  � CHANNEL_ACCESS_FAILURE � The transmission of the coordinator realignment frame failed.
 *  � FRAME_TOO_LONG � The length of the beacon frame exceeds aMaxPHYPacketSize.
 *  � SUPERFRAME_OVERLAP � The outgoing superframe overlaps the incoming superframe.
 *  � TRACKING_OFF � The StartTime parameter is nonzero, and the MLME is not currently tracking
 *                   the beacon of the coordinator through which it is associated.
 * � A security error code, as defined in 7.2.
 *
 * note: see Std 802.15.4-2011 (table 35) page 106
 *
 * param1[in]:    MACenum        state:      The result of the attempt to start using an updated superframe configuration. */
void Ieee802154eMac::MLME_START_confirm(MACenum status)
{
    Ieee802154eNetworkCtrlInfo *startCo = new Ieee802154eNetworkCtrlInfo("StartConfirm", TP_MLME_START_CONFIRM);
    startCo->setStatus(status);
    if(par("isPANCoor").boolValue())
    {
	startCo->setBeaconOrder(mpib.macBeaconOrder);
	startCo->setSuperframeOrder(mpib.macSuperframeOrder);
    }
    startCo->setPanCoordinator(isPANCoor);
    send(startCo->dup(), mSchedulerOut);

    delete startCo;
}

/**@author: 2014    Stefan Reis
 * brief: The MLME-SYNC.request primitive requests to synchronize with the coordinator by acquiring and, if specified, tracking its beacons.
 *
 * If the MLME-SYNC.request primitive is received by the MLME on a beacon-enabled PAN, it will first set
 * phyCurrentPage and phyCurrentChannel equal to the values of the ChannelPage and ChannelNumber
 * parameters, respectively. If the TrackBeacon parameter is equal to TRUE, the MLME will track the beacon,
 * i.e., enable its receiver just before the expected time of each beacon so that the beacon frame can be
 * processed. If the TrackBeacon parameter is equal to FALSE, the MLME will locate the beacon but not
 * continue to track it.
 * If this primitive is received by the MLME while it is currently tracking the beacon, the MLME will not
 * discard the primitive but will treat it as a new synchronization request.
 *
 * note:see Std 802.15.4-2011 (table 36) page 107
 *
 * param1[in]:    UINT_8    channelNumber:  Channel number on which to attempt coordinator synchronization
 * param2[in]:    UINT_8    channelPage:    The channel page on which to attempt coordinator synchronization
 * param3[in]:    bool      trackBeacon:    TRUE if the MLME is to synchronize with the next beacon and attempts to track all future
 *                                          beacons. FALSE if the MLME is to synchronize with only the next beacon. */
void Ieee802154eMac::handle_MLME_SYNC_request(UINT_8 channelNumber, UINT_8 channelPage, bool trackBeacon)
{

}

/**@author: 2014    Stefan Reis
 * brief: The MLME-SYNC-LOSS.indication primitive indicates the loss of synchronization with a coordinator.
 *
 * The MLME-SYNC-LOSS.indication primitive is generated by the MLME of a device and issued to its next
 * higher layer in the event of a loss of synchronization with the coordinator. It is also generated by the MLME
 * of the PAN coordinator and issued to its next higher layer in the event of either a PAN ID conflict or an
 * overlap between the outgoing superframe and the incoming superframe, as described in 5.1.1.2.
 *
 * note: see Std 802.15.4-2011 (table 37) page 108
 *
 * param1[in]:    MACenum    lossReason:    The reason that synchronization was lost.
 * param2[in]:    UINT_16    panId:         The PAN identifier with which the device lost synchronization or to which it was realigned.
 * param3[in]:    UINT_8     channelNumber: The channel number on which the device lost synchronization or to which it was realigned
 * param4[in]:    UINT_8     channelPage:   The channel page on which the device lost synchronization or to which it was realigned
 * param5[in]:    UINT_8     securityLevel:
 * param6[in]:    UINT_8     keyIdMode:
 * param7[in]:    UINT_64    keySource:
 * param8[in]:    UINT_8     keyIndex: */
void Ieee802154eMac::MLME_SYNC_LOSS_indication(MACenum lossReason, UINT_16 panId, UINT_8 channelNumber, UINT_8 channelPage, UINT_8 securityLevel, UINT_8 keyIdMode, UINT_64 keySource, UINT_8 keyIndex)
{
    Ieee802154eNetworkCtrlInfo *primitive = new Ieee802154eNetworkCtrlInfo();
    primitive->setKind(TP_MLME_SYNC_LOSS_INDICATION);
    primitive->setLossReason(lossReason);
    primitive->setPanId(panId);
    primitive->setChannelNumber(channelNumber);
    primitive->setChannelPage(channelPage);
    primitive->setSecurityLevel(securityLevel);
    primitive->setKeyIdMode(keyIdMode);
    primitive->setKeySource(keySource);
    primitive->setKeyIndex(keyIndex);

    EV << "[MAC]: sending a MLME_SYNC_LOSS.indication to NETWORK" << endl;
    send(primitive, mSchedulerOut);
}

/**@author: 2014    Stefan Reis
 * brief: The MLME-POLL.request primitive prompts the device to request data from the coordinator.
 *
 * On receipt of the MLME-POLL.request primitive, the MLME requests data from the coordinator, as
 * described in 5.1.6.3. If the poll is directed to the PAN coordinator, the data request command may be
 * generated without any destination address information present. Otherwise, the data request command is
 * always generated with the destination address information in the CoordPANId and CoordAddress
 * parameters.
 *
 * note: see Std 802.15.4-2011 (table 38) page 110
 *
 * param1[in]:    Ieee802154eAddrMode    coordAddrMode:     The addressing mode of the coordinator to which the poll is intended.
 * param2[in]:    UINT_16                coordPANId:        The PAN identifier of the coordinator to which the poll is intended.
 * param3[in]:    IE3ADDR                coordAddress:      The address of the coordinator to which the poll is intended.
 * param4[in]:    UINT_8                 securityLevel:     AS defined in Table 46
 * param5[in]:    UINT_8                 keyIdMode:         As defined in Table 46
 * param6[in]:    UINT_64                keySource:         As defined in Table 46
 * param7[in]:    UINT_8                 keyIndex:          As defined in Table 46 */
void Ieee802154eMac::handle_MLME_POLL_request(Ieee802154eAddrMode coordAddrMode, UINT_16 coordPANId, IE3ADDR coordAddress, UINT_8 securityLevel, UINT_8 keyIdMode, UINT_64 keySource, UINT_8 keyIndex)
{

}

/**@author: 2014    Stefan Reis
 * brief: The MLME-POLL.confirm primitive reports the results of a request to poll the coordinator for data.
 *
 * The MLME-POLL.confirm primitive is generated by the MLME and issued to its next higher layer in
 * response to an MLME-POLL.request primitive. If the request was successful, the status parameter will be
 * equal to SUCCESS, indicating a successful poll for data. Otherwise, the status parameter indicates the
 * appropriate error code. The status values are fully described in 6.2.14.1.
 *
 * note: If the Frame Pending field of the acknowledgment frame is set to zero, the MLME will issue the MLMEPOLL.
 *       confirm primitive with a status of NO_DATA.
 *       If a frame is received from the coordinator with a zero length payload or if the frame is a MAC command
 *       frame, the MLME will issue the MLME-POLL.confirm primitive with a status of NO_DATA. If a frame is
 *       received from the coordinator with nonzero length payload, the MLME will issue the MLMEPOLL.
 *       confirm primitive with a status of SUCCESS. In this case, the actual data are indicated to the next
 *       higher layer using the MCPS-DATA.indication primitive, as described in 6.3.3.
 *       If a frame is not received within macMaxFrameTotalWaitTime even though the acknowledgment to the data
 *       request command has its Frame Pending field set to one, the MLME will issue the MLME-POLL.confirm
 *       primitive with a status of NO_DATA.
 *
 * note: see Std 802.15.4-2011 (table 39) page 110
 *
 * param1[in]: MACenum status: The status of the data request */
void Ieee802154eMac::MLME_POLL_confirm(MACenum status)
{
    Ieee802154eNetworkCtrlInfo *primitive = new Ieee802154eNetworkCtrlInfo();
    primitive->setKind(TP_MLME_POLL_CONFIRM);
    primitive->setStatus(status);

    EV << "[MAC]: sending a MLME_POLL.confirm to NETWORK" << endl;
    send(primitive, mSchedulerOut);
}

/**@author: 2014    Stefan Reis
 * brief: The MLME-DPS.request primitive allows the next higher layer to request that the PHY utilize a given pair
 *        of preamble codes for a single use pending expiration of the DPSIndexDuration.
 *
 * note:  This primitive may also be generated to cancel a previously generated request to enable the transmitter and
 *        receiver dynamic preambles. The use of the index for the transmitter and receiver is enabled or disabled
 *        exactly once per primitive request.
 *        The MLME starts the timer that assures that the device returns to a normal operating state with default
 *        preambles if a following MCPS-DATA.request primitive does not occur. After starting the timer, the
 *        MLME responds with a MLME-DPS.confirm primitive with the appropriate status parameter.
 *
 * note: see Std 802.15.4-2011 (table 40) page 112
 *
 * param1[in]:      UINT_8      txDPSIndex:         Range: 0,13-16,21-24; */
void Ieee802154eMac::handle_MLME_DPS_request(UINT_8 txDPSIndex, UINT_8 rxDPSIndex, UINT_32 dpsIndexDuration)
{

}

/**@author: 2014    Stefan Reis
 * brief: The MLME-DPS.indication primitive indicates the expiration of the DPSIndexDuration and the resetting of
 *        the DPS values in the PHY.
 *
 * note:  If a MCPS-DATA.request primitive is not received before the timer expires, the MLME issues the
 *        MLMEDPS.indication primitive to the next higher layer.
 *
 * note: Std 802.15.4-2011 (table XX) page 113 */
void Ieee802154eMac::MLME_DPS_indication()
{

}

/**@author: 2014    Stefan Reis
 * brief: The MLME-DPS.confirm primitive reports the results of the attempt to enable or disable the DPS.
 *
 * The MLME-DPS.indication primitive indicates the expiration of the DPSIndexDuration and the resetting of
 * the DPS values in the PHY.
 *
 * note:  The MLME-DPS.confirm primitive is generated by the MLME and issued to its next higher layer in
 *        response to an MLME-DPS.request primitive.
 *        If any parameter in the MLME-DPS.request primitive is not supported or is out of range, the status of
 *        DPS_NOT_SUPPORTED is returned. If the request to enable or disable the DPS was successful, the
 *        MLME issues the MLME-DPS.confirm primitive with a status of SUCCESS.
 *
 * note: see Std 802.15.4-2011 (table 41) page 112
 *
 * param1[in]:  MACenum     status:     SUCCESS, DPS_NOT_SUPPORTED */
void Ieee802154eMac::MLME_DPS_confirm(MACenum status)
{

}

/**@author: 2014    Stefan Reis
 * brief: The MLME-SOUNDING.request primitive is used by the next higher layer to request that the PHY respond
 *        with channel sounding information.
 *
 * note:  The MLME-SOUNDING.request primitive is used by the next higher layer to request that the PHY respond
 *        with channel sounding information. The MLME-SOUNDING.request primitive shall be supported by all RDEVs;
 *        however, the underlying sounding capability is optional in all cases.
 *
 * note: see Std 802.15.4-2011 (table XX) page 112 */
void Ieee802154eMac::handle_MLME_SOUNDING_request()
{

}

/**@author: 2014    Stefan Reis
 * brief: MLME_SOUNDING_confirm reports a request to the PHY
 *
 * note:  The MLME-CHANNEL.confirm primitive reports the result of a request to the PHY to provide channel
 *        sounding information. The MLME-SOUNDING.confirm primitive shall be supported by all RDEVs;
 *        however, the underlying sounding capability is optional in all cases.
 *
 * note: see Std 802.15.4-2011 (table XX) page 113
 *
 * param1[in]    vector<int>    soundingList:       List of sounding points, List include SoundingTime and SoundingAmplitude
 * param2[in]    enum           status:             The status of the attempt to return sounding data */
void Ieee802154eMac::MLME_SOUNDING_confirm(std::vector<int> soundingList, MACenum status)
{

}

//FOR UWB PHY not needed yet
/*
 void Ieee802154eMac::MLME_CALIBRATE_request(){

 }

 void Ieee802154eMac::MLME_CALIBRATE_confirm(){

 }
 */

/**@author: 2014    Stefan Reis
 * brief: MLME-BEACON.request primitive requests the generation of a beacon or enhanced beacon in a nonbeacon-enabled PAN
 *
 * The MLME-BEACON.request primitive requests the generation of a beacon or enhanced beacon in a nonbeacon-enabled
 * PAN, either in response to a beacon request command when macBeaconAutoRespond is FALSE, or on demand,
 * e.g., to send beacons to enable a TSCH passive scan.
 *
 * note: see Std 802.15.4e-2012 (6.2.18.1) page 140
 *
 * param[in]
 * UINT_8 beaconType, UINT_8 channel, UINT_8 channelPage, UINT_8 superFrameOrder,
 UINT_8 beaconSecurityLevel, UINT_8 beaconKeyIdMode, UINT_64 beaconKeySource,
 UINT_8 beaconKeyIndex, UINT_8 dstAddrMode, IE3ADDR dstAddr,bool bsnSuppression
 * */
void Ieee802154eMac::handle_MLME_BEACON_request(cMessage *msg)
{
    //constructBCN();
    //constructBCN();
    txBeaconNow = true;
    MLME_BEACON_confirm(mac_SUCCESS);
    delete msg;
}

/**@author: 2014    Stefan Reis
 * brief: MLME-BEACON.confirm primitive reports the results from an MLME-BEACON.request primitive
 *
 * The MLME-BEACON.confirm primitive is generated by the MAC sublayer entity in response to an MLME-BEACON.request primitive.
 *
 * note: see Std 802.15.4e-2012 (6.2.18.2) page 142
 *
 * param[in] */
void Ieee802154eMac::MLME_BEACON_confirm(MACenum status)
{
    Ieee802154eNetworkCtrlInfo *primitive = new Ieee802154eNetworkCtrlInfo();
    primitive->setKind(TP_MLME_BEACON_CONFIRM);
    primitive->setStatus(status);
    primitive->setBeaconOrder(mpib.macBeaconOrder);
    primitive->setSuperframeOrder(mpib.macSuperframeOrder);

    EV << "[MAC]: sending a MLME_BEACON.confirm to NETWORK" << endl;
    send(primitive->dup(), mSchedulerOut);
    delete primitive;
}

/**@author: 2014    Stefan Reis
 * brief: MLME-BEACON-REQUEST.indication primitive indicates the receipt of a beacon request
 *
 * The MLME-BEACON-REQUEST.indication primitive indicates the receipt of a beacon request.
 * It is only available when macBeaconAutoRespond is FALSE.
 *
 * note: see Std 802.15.4e-2012 (6.2.18.3) page 142
 *
 * param[in]*/
void Ieee802154eMac::MLME_BEACON_REQUEST_indication(Ieee802154eBeaconType beaconType, UINT_8 srcAddrMode, IE3ADDR srcAddr, UINT_16 dstPANID, std::vector<IE_LIST> ieList)
{

}

//-------------------------------------------------------------------------------/
/**** <TSCH MAC management service - Std 802.15.4e-2012 (table 8a) page 123> ****/
//-------------------------------------------------------------------------------/
/**@author: 2014    Stefan Reis
 * brief: MLME-SET-SLOTFRAME.request to add, delete, or modify a slotframe
 *
 * An MLME-SET-SLOTFRAME.request may be used by a higher layer to add, delete, or modify a slotframe
 * at the MAC sublayer
 *
 * note: only for TSCH, see Std 802.15.4e-2012 (6.2.19.1) page 143
 *
 * param[in] */
void Ieee802154eMac::handle_MLME_SET_SLOTFRAME_request(cMessage *msg)
{
    handleEB(msg->dup());
    delete msg;
}

/**@author: 2014    Stefan Reis
 * brief: MLME-SET-SLOTFRAME.confirm reports the results of the MLME-SETSLOTFRAME.request command
 *
 * The MLME-SET-SLOTFRAME.confirm primitive reports the results of the MLME-SETSLOTFRAME.request
 * command. The slotframeHandle is that which was supplied by a higher layer in the prior call
 * to the MLME-SET-SLOTFRAME.request.
 *
 * note: only for TSCH, see Std 802.15.4e-2012 (6.2.19.2) page 14
 *
 * param[in] */
void Ieee802154eMac::MLME_SET_SLOTFRAME_confirm(UINT_8 slotframeHandle, MACenum status)
{

}

/**@author: 2014    Stefan Reis
 * brief: MLME-SET-LINK.request to add, delete, or modify a link
 *
 * The MLME-SET-LINK.request primitive requests to add a new link, or delete or modify an existing link at
 * the MAC sublayer. The slotframeHandle and linkHandle are supplied by a higher layer.
 *
 * note: only for TSCH, see Std 802.15.4e-2012 (6.2.19.3) page 145
 *
 * param[in] */
void Ieee802154eMac::handle_MLME_SET_LINK_request(Ieee802154eOperation operation, UINT_16 linkHandle, UINT_8 slotframeHandle, UINT_16 timeslot, UINT_16 channelOffset, MACTSCHLinkOptions linkOptions, MACTSCHLinkType linkType, UINT_16 nodeAddr)
{

}

/**@author: 2014    Stefan Reis
 * brief: MLME-SET-LINK.confirm reports the results of the MLME-SET-LINK.request command
 *
 * The MLME-SET-LINK.confirm primitive indicates the result of add, delete, or modify link operation. The
 * linkHandle and slotframeHandle are those that were supplied by a higher layer in the prior call to MLMESET-LINK.request
 *
 * note: only for TSCH, see Std 802.15.4e-2012 (6.2.19.4) page 146
 *
 * param[in] */
void Ieee802154eMac::MLME_SET_LINK_confirm(MACenum status, UINT_16 linkHandle, UINT_8 slotframeHandle)
{

}

/**@author: 2014    Stefan Reis
 * brief: MLME-TSCH-MODE.request requests to put the MAC into or out of the TSCH mode
 *
 * The MLME-TSCH-MODE.request requests to put the MAC into or out of the TSCH mode.
 * note: only for TSCH, see Std 802.15.4e-2012 (6.2.19.5) page 147
 *
 * param[in] */
void Ieee802154eMac::handle_MLME_TSCH_MODE_request(Ieee802154eTSCHMode TSCHMode)
{

}

/**@author: 2014    Stefan Reis
 * brief: MLME-TSCH-MODE.confirm reports the results of the MLME-TSCH-MODE.request command
 *
 * The MLME-TSCH-MODE.confirm primitive reports the result of the MLME-TSCH-MODE.request primitive.
 *
 * note: only for TSCH, see Std 802.15.4e-2012 (6.2.19.6) page 148
 *
 * param[in] */
void Ieee802154eMac::MLME_TSCH_MODE_confirm(Ieee802154eTSCHMode TSCHMode, MACenum status)
{

}

/**@author: 2014    Stefan Reis
 * brief: MLME-SET-SLOTFRAME.request to add, delete, or modify a slotframe
 *
 * An MLME-SET-SLOTFRAME.request may be used by a higher layer to add, delete, or modify a slotframe
 * at the MAC sublayer
 *
 * note: only for TSCH, see Std 802.15.4e-2012 (6.2.19.7) page 148
 *
 * param[in] */
void Ieee802154eMac::handle_MLME_KEEP_ALIVE_request(UINT_16 dstAddr, UINT_16 keepAlivePeriod)
{

}

/**@author: 2014    Stefan Reis
 * brief: MLME-KEEP-ALIVE.confirm reports the results of the MLME-KEEP-ALIVE.request command
 *
 * The MLME-KEEP-ALIVE.confirm primitive reports the results of a request that frames be sent to a device
 * with a minimum period.
 *
 * note: only for TSCH, see Std 802.15.4e-2012 (6.2.19.8) page 149
 *
 * param[in] */
void Ieee802154eMac::MLME_KEEP_ALIVE_confirm(MACenum status)
{

}

//-------------------------------------------------------------------------------/
/***************************** <Timer starter> **********************************/
//-------------------------------------------------------------------------------/
void Ieee802154eMac::startBackoffTimer(simtime_t wtime)
{
    EV << "[CSMA-CA]: #" << (int)tmpCsmaca->getSeqNmbr() << ": starting backoff for " << wtime << " s" << endl;

    if(backoffTimer->isScheduled())
	cancelEvent(backoffTimer);

    scheduleAt(simTime() + wtime, backoffTimer);
}

void Ieee802154eMac::startDeferCCATimer(simtime_t wtime)
{
    if(deferCCATimer->isScheduled())
	cancelEvent(deferCCATimer);

    scheduleAt(simTime() + wtime, deferCCATimer);
    EV << "[CSMA-CA]: delay " << wtime << " s before performing CCA" << endl;
}

void Ieee802154eMac::startBcnRxTimer()
{
    simtime_t rxBI, t_bcnRxTime, now, len12s, wtime, tmpf;
    now = simTime();

    rxBI = rxSfSpec.BI / phy_symbolrate;
    t_bcnRxTime = bcnRxTime;

    while(now - t_bcnRxTime > rxBI)
	t_bcnRxTime += rxBI;
    len12s = Ieee802154ePhy::aTurnaroundTime / phy_symbolrate;

    tmpf = (now - t_bcnRxTime);
    wtime = rxBI - tmpf;

    if(wtime >= len12s)
	wtime -= len12s;

    tmpf = now + wtime;
    if(tmpf - lastTime_bcnRxTimer < rxBI - len12s)
    {
	tmpf = 2 * rxBI;
	tmpf = tmpf - now;
	tmpf = tmpf + t_bcnRxTime;
	wtime = tmpf - len12s;
	//wtime = 2* BI - (now - bcnRxTime) - len12s;
    }
    lastTime_bcnRxTimer = now + wtime;

    if(bcnRxTimer->isScheduled())
	cancelEvent(bcnRxTimer);

    scheduleAt(now + wtime, bcnRxTimer);
}

/**@author: 2014    Stefan Reis     (modified) */
void Ieee802154eMac::startBcnTxTimer(bool txFirstBcn, simtime_t startTime)
{
    simtime_t wtime, now, tmpf;
    now = simTime();

    if(txFirstBcn)     // tx my first beacon
    {
	if(mpib.macTSCHenabled)
	{   // Std 802.15.4e-2012 TSCH mode
	    txNow_bcnTxTimer = true;
	    beaconWaitingTx = true;
	    handleBcnTxTimer();
	    return;
	}
	else
	{   // Std 802.15.4-2006/2011
	    ASSERT(mpib.macBeaconOrder != 15);
	    if(startTime == 0.0)   // transmit beacon right now
	    {
		txNow_bcnTxTimer = true;
		beaconWaitingTx = true;
		handleBcnTxTimer(); // no delay
		return;
	    }
	    else        // relative to bcnRxTime
	    {
		txNow_bcnTxTimer = false;
		// wtime = startTime - (now - bcnRxTime) - 12.0/phy_symbolrate;
		tmpf = now - bcnRxTime;
		tmpf = startTime - tmpf;
		wtime = tmpf - Ieee802154ePhy::aTurnaroundTime / phy_symbolrate;
		ASSERT(wtime >= 0.0);

		if(bcnTxTimer->isScheduled())
		    cancelEvent(bcnTxTimer);

		scheduleAt(now + wtime, bcnTxTimer);
		return;
	    }
	}
    }

    if(!txNow_bcnTxTimer)
    {
	if(bcnTxTimer->isScheduled())
	    cancelEvent(bcnTxTimer);
	scheduleAt(now + Ieee802154ePhy::aTurnaroundTime / phy_symbolrate, bcnTxTimer);
    }
    else if(mpib.macBeaconOrder != 15 && !mpib.macTSCHenabled) // Std 802.15.4-2006/2011
    {
	wtime = (aBaseSuperframeDuration * (1 << mpib.macBeaconOrder) - Ieee802154ePhy::aTurnaroundTime) / phy_symbolrate;
	if(bcnTxTimer->isScheduled())
	    cancelEvent(bcnTxTimer);
	scheduleAt(now + wtime, bcnTxTimer);
    }

    txNow_bcnTxTimer = (!txNow_bcnTxTimer);
}

void Ieee802154eMac::startAckTimeoutTimer()
{
    if(ackTimeoutTimer->isScheduled())
	cancelEvent(ackTimeoutTimer);

    scheduleAt(simTime() + mpib.macAckWaitDuration / phy_symbolrate, ackTimeoutTimer);
}

void Ieee802154eMac::startTxAckBoundTimer(simtime_t wtime)
{
    if(txAckBoundTimer->isScheduled())
	cancelEvent(txAckBoundTimer);

    scheduleAt(simTime() + wtime, txAckBoundTimer);
}

void Ieee802154eMac::startTxCmdDataBoundTimer(simtime_t wtime)
{
    if(txCmdDataBoundTimer->isScheduled())
	cancelEvent(txCmdDataBoundTimer);

    scheduleAt(simTime() + wtime, txCmdDataBoundTimer);
}

void Ieee802154eMac::startIfsTimer(bool sifs)
{
    simtime_t wtime;
    if(sifs)                   // SIFS
	wtime = mpib.macSIFSPeriod / phy_symbolrate;
    else
	// LIFS
	wtime = mpib.macLIFSPeriod / phy_symbolrate;

    if(ifsTimer->isScheduled())
	cancelEvent(ifsTimer);

    scheduleAt(simTime() + wtime, ifsTimer);
}

void Ieee802154eMac::startTxSDTimer()
// this function should be called when txing a new beacon is received in <handleBcnTxTimer()>
{
    ASSERT(!inTxSD_txSDTimer);
    simtime_t wtime;
    // calculate CAP duration
    wtime = aNumSuperframeSlots * txSfSlotDuration / phy_symbolrate;

    if(txSDTimer->isScheduled())
	cancelEvent(txSDTimer);

    scheduleAt(simTime() + wtime, txSDTimer);
    inTxSD_txSDTimer = true;
}

void Ieee802154eMac::startRxSDTimer()
// this function should be called when a  new beacon is received in <handleBeacon()>
{
    ASSERT(!inRxSD_rxSDTimer);
    simtime_t wtime;
    // calculate the remaining time in current CAP
    wtime = aNumSuperframeSlots * rxSfSlotDuration / phy_symbolrate;
    wtime = wtime + bcnRxTime - simTime();
    ASSERT(wtime > 0.0);

    if(rxSDTimer->isScheduled())
	cancelEvent(rxSDTimer);

    scheduleAt(simTime() + wtime, rxSDTimer);
    //EV << "The current SD will end at " << simTime() + wtime << endl;
    inRxSD_rxSDTimer = true;
}

//--------------------------------------------------------------------------------/
/******************************* <Timer Handler> ********************************/
//--------------------------------------------------------------------------------/
void Ieee802154eMac::handleBackoffTimer()
{
    EV << "[CSMA-CA]: before sending CCA primitive, tell PHY to turn on the receiver" << endl;
    taskP.taskStatus(TP_RX_ON_CSMACA) = true;
    PLME_SET_TRX_STATE_request(phy_RX_ON);
}

void Ieee802154eMac::handleDeferCCATimer()
{
    taskP.taskStatus(TP_CCA_CSMACA) = true;
    EV << "[MAC-TASK]: TP_CCA_CSMACA = true" << endl;
    PLME_CCA_request();
}

void Ieee802154eMac::handleBcnRxTimer()
{
    if(rxBO != 15)     //beacon enabled (do nothing if beacon not enabled)
    {
	if(txAck)
	{
	    delete txAck;
	    txAck = NULL;
	}
	//enable the receiver
	PLME_SET_TRX_STATE_request(phy_RX_ON);

	if(bcnLossCounter != 0)
	{
	    numCollision++;     //
	}
	bcnLossCounter++;
	startBcnRxTimer();
    }
}

/**@author: 2014    Stefan Reis     (modified)
 * construct the bcn at a specific time */
void Ieee802154eMac::handleBcnTxTimer()
{
    /*TRANSACLINK *tmp;
     int i;*/

    if(mpib.macBeaconOrder != 15 || mpib.macTSCHenabled) //beacon enabled or TSCH mode
    {
	if(!txNow_bcnTxTimer)          //enable the transmitter
	{
	    beaconWaitingTx = true;
	    if(txAck)      // transmitting beacon has highest priority
	    {
		delete txAck;
		txAck = NULL;
	    }
	    PLME_SET_TRX_STATE_request(phy_TX_ON);
	}
	else        // it's time to transmit beacon
	{
	    ASSERT(txBeacon == NULL);

	    constructBCN();

	    startTxSDTimer();

	    // schedule for GTS if they exist
	    if(gtsCount > 0)
	    {
		index_gtsTimer = gtsCount - 1; // index of GTS in the list that will come first
		gtsScheduler();
	    }
	}
    }
    startBcnTxTimer(); //don't disable this even beacon not enabled (beacon may be temporarily disabled like in channel scan, but it will be enabled again)
}

void Ieee802154eMac::handleAckTimeoutTimer()
{
    //ASSERT(txBcnCmd||txBcnCmdUpper||txData);
    bool error = true;
    if(txBcnCmd)
	error = false;
    else if(txBcnCmdUpper)
	error = false;
    else if(txData)
	error = false;
    else if(txGTS && waitGTSAck)
	error = false;
    if(error)
	opp_error("handleAckTimeoutTimer");

    dispatch(phy_BUSY, __FUNCTION__);    //the status p_BUSY will be ignore
}

void Ieee802154eMac::handleTxAckBoundTimer()
{
    if(!beaconWaitingTx)
	if(txAck)      //<txAck> may have been cancelled by <macBeaconRxTimer>
	{
	    txPkt = txAck;
	    sendDown(check_and_cast<Ieee802154eFrame *>(txAck->dup()));
	}
}

void Ieee802154eMac::handleTxCmdDataBoundTimer()
// called when txCmdDataBoundTimer expires or directly by <handle_PLME_SET_TRX_STATE_confirm()>
// data or cmd is sent to Phy layer here
{
    //int i;

    // TBD: TP_mlme_scan_request
    /*
     if (taskP.taskStatus(TP_mlme_scan_request))
     if (txBcnCmd2 != txCsmaca)
     return;         //terminate all other transmissions (will resume afte channel scan)
     */

#ifdef test_802154_INDIRECT_TRANS
    // need to update first to see if this transaction expired already
    if (check_and_cast<Ieee802154eFrame *>(txCsmaca)->indirect)
    {
	i = updateTransacLinkByPktOrHandle(tr_oper_est,&transacLink1,&transacLink2,txCsmaca); // if expired, txCsmaca will be deleted here
	if (i != 0)//transaction expired
	{
	    resetTRX();
	    if (txBcnCmd == txCsmaca)
	    txBcnCmd = NULL;
	    else if (txBcnCmd2 == txCsmaca)
	    txBcnCmd2 = NULL;
	    else if (txData == txCsmaca)
	    txData = NULL;
	    //Packet::free(txCsmaca);   //don't do this, since the packet will be automatically deleted when expired
	    csmacaResume();
	    return;
	}
    }
#endif

    if(txBcnCmd == txCsmaca)
    {
	txPkt = txBcnCmd;
	sendDown(check_and_cast<Ieee802154eFrame *>(txBcnCmd->dup()));
    }
    else if(txBcnCmdUpper == txCsmaca)
    {
	txPkt = txBcnCmdUpper;
	sendDown(check_and_cast<Ieee802154eFrame *>(txBcnCmdUpper->dup()));
    }
    else if(txData == txCsmaca)
    {
	txPkt = txData;
	PD_DATA_request(txData->getByteLength(), check_and_cast<Ieee802154eFrame *>(txData->dup()));
    }
}

/**@author: 2014    Stefan Reis     (modified)
 * handle the IFS delay */
void Ieee802154eMac::handleIfsTimer()
// this function is called after delay of ifs following sending out the ack requested by reception of a data or cmd pkt
// or following reception of a data pkt requiring no ACK
// other cmd pkts requiring no ACK are processed in <handleCommand()>
{
    ASSERT(rxData != NULL || rxCmd != NULL || txGTS != NULL);

    if(rxCmd)      // MAC command, all commands requiring ACK are handled here
    {
    }
    else if(rxData)
    {
	MCPS_DATA_indication((Ieee802154eAddrMode)rxData->getFrmCtrl().srcAddrMode, rxData->getSrcPanId(), rxData->getSrcAddr(), (Ieee802154eAddrMode)rxData->getFrmCtrl().dstAddrMode, rxData->getDstPanId(), rxData->getDstAddr(), rxData->getByteLength(), rxData, 0 /*mpduLinkQuality*/, rxData->getSeqNmbr(), (UINT_32)rxData->getTimestamp().raw(), rxData->getAuxSecHd().secLvl, rxData->getAuxSecHd().keyIdentMode, rxData->getAuxSecHd().keySource, rxData->getAuxSecHd().keyIndex, 0 /*uwbPRF*/, (Ieee802154eUWBFType)0 /*uwbPreambleSymbolRepetitions*/, 0 /*dataRate*/, (RangingControl)0 /*rangingReceived*/, 0 /*rangingCounterStart*/, 0 /*rangingCounterStop*/, 0 /*rangingTrackingInterval*/, 0 /*rangingOffset*/, 0 /*rangingFOM*/);

	rxData = NULL;
    }
    else if(txGTS)
    {
	ASSERT(taskP.taskStatus(TP_MCPS_DATA_REQUEST) && (taskP.taskStep(TP_MCPS_DATA_REQUEST) == 4));
	FSM_MCPS_DATA_request();
    }
}

void Ieee802154eMac::handleSDTimer() // we must make sure that outgoing CAP and incoming CAP never overlap
{
    inTxSD_txSDTimer = false;
    inRxSD_rxSDTimer = false;

    // If PAN coordinator uses GTS, this is the end of CFP, reset indexCurrGts
    if(isPANCoor)
	indexCurrGts = 99;
    EV << "[MAC]: entering inactive period, turn off radio and go to sleep" << endl;
    if(!txAck)
    {
	PLME_SET_TRX_STATE_request(phy_TRX_OFF);
    }
    else
	numTxAckInactive++;
    /*if ((!waitBcnCmdAck) && (!waitBcnCmdUpperAck) && (!waitDataAck))      // not waiting for ACK
     {
     EV << "[MAC]: entering inactive period, turn off radio and go to sleep" << endl;
     PLME_SET_TRX_STATE_request(phy_TRX_OFF);
     }
     else
     EV << "[MAC]: entering inactive period, but can not go to sleep" << endl;*/
}

//-------------------------------------------------------------------------------/
/********************** <FSM and Pending Task Processing> ***********************/
//-------------------------------------------------------------------------------/
void Ieee802154eMac::taskSuccess(char type, bool csmacaRes)
{
    UINT_16 t_CAP;
    uint16_t ifs;
    simtime_t tmpf;

    if(type == 'b')    //beacon
    {
	EV << "[MAC]: taskSuccess for sending beacon" << endl;
	/*if (!txBeacon)
	 {
	 assert(txBcnCmdUpper);
	 txBeacon = txBcnCmdUpper;
	 txBcnCmdUpper = 0;
	 }*/
	//--- calculate CAP ---
	if(txBeacon->getByteLength() <= aMaxSIFSFrameSize)
	    ifs = mpib.macSIFSPeriod;
	else
	    ifs = mpib.macLIFSPeriod;

	// calculate <txBcnDuration>
	tmpf = calDuration(txBeacon) * phy_symbolrate;
	tmpf += ifs;
	txBcnDuration = (uint16_t)(SIMTIME_DBL(tmpf) / aUnitBackoffPeriod);
	if(fmod(tmpf, aUnitBackoffPeriod) > 0.0)
	    txBcnDuration++;
	EV << "[MAC]: calculating txBcnDuration = " << (int)txBcnDuration << endl;

	t_CAP = (txSfSpec.finalCap + 1) * txSfSlotDuration / aUnitBackoffPeriod - txBcnDuration;

	if(t_CAP == 0) // it's possible that there is no time left in current CAP after txing beacon
	{
	    csmacaRes = false;
	    PLME_SET_TRX_STATE_request(phy_TRX_OFF);
	    EV << "[MAC]: no time left in current CAP after txing beacon, trying to turn off radio" << endl;
	}
	else
	{
	    PLME_SET_TRX_STATE_request(phy_RX_ON);
	    EV << "[MAC]: turn on radio receiver after txing beacon" << endl;
	}
	//CSMA-CA may be waiting for the new beacon
	if(backoffStatus == 99)
	    csmacaTrxBeacon('t');
	//----------------------
	beaconWaitingTx = false;
	delete txBeacon;
	txBeacon = NULL;
	/*
	 //send out delayed ack.
	 if (txAck)
	 {
	 csmacaRes = false;
	 plme_set_trx_state_request(p_TX_ON);
	 }
	 */
	numTxBcnPkt++;
    }
    else if(type == 'a')   //ack.
    {
	numTxAckPkt++;
	ASSERT(txAck);
	delete txAck;
	txAck = NULL;
    }
    /*else if (task == 'c') //command
     {
     ASSERT(txBcnCmd);
     //if it is a pending packet, delete it from the pending list
     updateTransacLinkByPktOrHandle(tr_oper_del,&transacLink1,&transacLink2,txBcnCmd);
     delete txBcnCmd;
     txBcnCmd = NULL;
     }
     else if (task == 'C')   //command
     {
     ASSERT(txBcnCmdUpper);
     delete txBcnCmdUpper;
     txBcnCmdUpper = NULL;
     }*/
    else if(type == 'd')   //data
    {
	ASSERT(txData);
	EV << "[MAC]: taskSuccess for sending : " << txData->getName() << ":#" << (int)txData->getSeqNmbr() << endl;
	//Packet *p = txData;

	MCPS_DATA_confirm(0, txData->getArrivalTime().getScale(), false, 0, 0, 0, 0, 0, mac_SUCCESS, 0, 0, txData);

	delete txData;
	txData = NULL;
	numTxDataSucc++;
	// request another msg from ifq (if it exists), only when MAC is idle for data transmission
	if(!taskP.taskStatus(TP_MCPS_DATA_REQUEST))
	    reqtMsgFromIFq();
	//if it is a pending packet, delete it from the pending list
	//updateTransacLinkByPktOrHandle(tr_oper_del,&transacLink1,&transacLink2,p);
    }
    else if(type == 'g')   // GTS
    {
	csmacaRes = false;
	ASSERT(txGTS);
	EV << "[GTS]: taskSuccess for sending : " << txGTS->getName() << ":#" << (int)txGTS->getSeqNmbr() << endl;
	// if PAN coordinator, need to clear isTxPending in corresponding GTS descriptor
	if(isPANCoor)
	{
	    ASSERT(gtsList[indexCurrGts].isTxPending);
	    gtsList[indexCurrGts].isTxPending = false;
	}

	MCPS_DATA_confirm(0, txGTS->getArrivalTime().getScale(), false, 0, 0, 0, 0, 0, mac_SUCCESS, 0, 0, txGTS);

	delete txGTS;
	txGTS = NULL;
	numTxGTSSucc++;
	// request another msg from ifq (if it exists), only when MAC is idle for data transmission
	if(!taskP.taskStatus(TP_MCPS_DATA_REQUEST))
	    reqtMsgFromIFq();
    }
    else
	ASSERT(0);

    if(csmacaRes)
	csmacaResume();
}

void Ieee802154eMac::taskFailed(char type, MACenum status, bool csmacaRes)
{
    if((type == 'b')   //beacon
    || (type == 'a')    //ack.
	    || (type == 'c'))   //command
    {
	ASSERT(0);  //we don't handle the above failures here
    }
    else if(type == 'C')   //command from Upper
    {
	delete txBcnCmdUpper;
	txBcnCmdUpper = NULL;
    }
    else if(type == 'd')   //data
    {
	ASSERT(txData);
	EV << "[MAC]: taskFailed for sending : " << txData->getName() << ":#" << (int)txData->getSeqNmbr() << endl;

	MCPS_DATA_confirm(txData->getSeqNmbr(), 0, false, 0, 0, 0, 0, 0, status, 0, 0, NULL);

	delete txData;
	txData = NULL;
	numTxDataFail++;

	if(csmacaRes)
	    csmacaResume();
	// request another msg from ifq (if it exists), only when MAC is idle for data transmission
	if(!taskP.taskStatus(TP_MCPS_DATA_REQUEST))
	    reqtMsgFromIFq();
    }
    else if(type == 'g')   // GTS
    {
	ASSERT(txGTS);
	EV << "[MAC]: taskFailed for sending : " << txGTS->getName() << ":#" << (int)txGTS->getSeqNmbr() << endl;
	// if PAN coordinator, need to clear isTxPending in corresponding GTS descriptor
	if(isPANCoor)
	{
	    ASSERT(gtsList[indexCurrGts].isTxPending);
	    gtsList[indexCurrGts].isTxPending = false;
	}

	MCPS_DATA_confirm(0, txGTS->getArrivalTime().getScale(), false, 0, 0, 0, 0, 0, status, 0, 0, txGTS);

	delete txGTS;
	txGTS = NULL;
	numTxGTSFail++;

	// request another msg from ifq (if it exists), only when MAC is idle for data transmission
	if(!taskP.taskStatus(TP_MCPS_DATA_REQUEST))
	    reqtMsgFromIFq();
    }
}

void Ieee802154eMac::checkTaskOverflow(Ieee802154eMacTaskType task)
// check if there is a task of the specified type is now pending
{
    if(taskP.taskStatus(task))
	error("[MAC-TASK]: task overflow: %d!", task);
    else
    {
	taskP.taskStep(task) = 0;
	(taskP.taskFrFunc(task))[0] = 0;
    }
}

void Ieee802154eMac::FSM_MCPS_DATA_request(PHYenum pStatus, MACenum mStatus)
{
    Ieee802154eMacTaskType task = TP_MCPS_DATA_REQUEST;
    Ieee802154eTxOption txOption = taskP.mcps_data_request_TxOption;

    if(txOption == DIRECT_TRANS)
    {
	switch(taskP.taskStep(task))
	{
	    case 0:
		// impossible happen here
		break;

	    case 1:
		if(pStatus == phy_IDLE)        // CSMA/CA succeeds
		{
		    taskP.taskStep(task)++;strcpy
		    (taskP.taskFrFunc(task), "handle_PD_DATA_confirm");
		    //enable the transmitter
		    PLME_SET_TRX_STATE_request(phy_TX_ON);
		}
		else // CSMA/CA reports channel access failure, should report to SSCS through MCPS_DATA_confirm in spec
		     // here simply retry
		{
		    csmacaResume();
		}
		break;

	    case 2:
		if(txData->getFrmCtrl().ackReq)    //ack. required
		{
		    taskP.taskStep(task)++;strcpy
		    (taskP.taskFrFunc(task), "handleAck");
		    //enable the receiver
		    PLME_SET_TRX_STATE_request(phy_RX_ON);
		    startAckTimeoutTimer();
		    waitDataAck = true;
		}
		else        //assume success if ack. not required
		{
		    taskP.taskStatus(task) = false; // task ends successfully
		    EV << "[MAC-TASK-SUCCESS]: reset TP_MCPS_DATA_REQUEST" << endl;
		    resetTRX();
		    taskSuccess('d');
		}
		break;

	    case 3:
		if(pStatus == phy_SUCCESS) //ack. received
		{
		    EV << "[MAC]: ACK for " << txData->getName() << ":#" << (int)txData->getSeqNmbr() << " received successfully" << endl;
		    taskP.taskStatus(task) = false; // task ends successfully
		    EV << "[MAC-TASK-SUCCESS]: reset TP_MCPS_DATA_REQUEST" << endl;
		    waitDataAck = false;    // debug
		    resetTRX();
		    taskSuccess('d');
		}
		else                // time out when waiting for ack.
		{
		    EV << "[MAC]: ACK timeout for " << txData->getName() << ":#" << (int)txData->getSeqNmbr() << endl;
		    numDataRetry++;
		    if(numDataRetry <= mpib.macMaxFrameRetries)
		    {
			EV << "[MAC]: starting " << numDataRetry << "the retry for " << txData->getName() << ":#" << (int)txData->getSeqNmbr() << endl;
			taskP.taskStep(task) = 1;   // go back to step 1
			strcpy(taskP.taskFrFunc(task), "csmacaCallBack");
			waitDataAck = false;
			csmacaResume();
		    }
		    else
		    {
			EV << "[MAC]: the max number of retries reached" << endl;
			taskP.taskStatus(task) = false; //task fails
			EV << "[MAC-TASK-FAIL]: reset TP_MCPS_DATA_REQUEST" << endl;
			waitDataAck = false;        // debug
			resetTRX();
			taskFailed('d', mac_NO_ACK);
		    }
		}
		break;

	    default:
		break;
	}
    }
    else if(txOption == INDIRECT_TRANS)
    {
	//TODO TBD
    }
    else if(txOption == GTS_TRANS)
    {
	switch(taskP.taskStep(task))
	{
	    case 0:
		// impossible happen here
		break;

	    case 1:
		// two possible callbacks, one from handleGtsTimer() at the starting of one GTS
		// the other directly from MCPS_DATA_request(), only possible for devices when receiving a data from upper layer during the GTS
		taskP.taskStep(task)++;strcpy
		(taskP.taskFrFunc(task), "handle_PD_DATA_confirm");
		// should transmit right now, since the timing is very strictly controlled in GTS,
		// we can simply use phy_FORCE_TRX_OFF and then phy_TX_ON to turn on transmitter immediately
		//PLME_SET_TRX_STATE_request(phy_FORCE_TRX_OFF); // [SR] need to be test
		PLME_SET_TRX_STATE_request(phy_TX_ON);
		// data will be sent to PHY in handle_PLME_SET_TRX_STATE_confirm()
		break;

	    case 2: // data successfully transmitted
		if(txGTS->getFrmCtrl().ackReq) //ack. required
		{
		    taskP.taskStep(task)++;strcpy
		    (taskP.taskFrFunc(task), "handleAck");
		    //enable the receiver
		    EV << "[GTS]: data successfully transmitted, turn on radio and wait for ACK" << endl;
		    PLME_SET_TRX_STATE_request(phy_RX_ON);
		    startAckTimeoutTimer();
		    waitGTSAck = true;
		}
		else        //assume success if ack. not required
		{
		    EV << "[GTS]: data successfully transmitted, no ACK required, turn off radio now" << endl;
		    PLME_SET_TRX_STATE_request(phy_FORCE_TRX_OFF);
		    EV << "[GTS]: need to delay IFS before next GTS transmission can proceed" << endl;
		    taskP.taskStep(task) = 4;
		    strcpy(taskP.taskFrFunc(task), "handleIfsTimer");
		    if(txGTS->getByteLength() <= aMaxSIFSFrameSize)
			startIfsTimer(true);
		    else
			startIfsTimer(false);
		}
		break;

	    case 3:
		if(pStatus == phy_SUCCESS) //ack. received
		{
		    waitGTSAck = false;
		    EV << "[GTS]: ACK for " << txGTS->getName() << ":#" << (int)txGTS->getSeqNmbr() << " received successfully, turn off radio now" << endl;
		    PLME_SET_TRX_STATE_request(phy_FORCE_TRX_OFF);
		    EV << "[GTS]: need to delay IFS before next GTS transmission can proceed" << endl;
		    taskP.taskStep(task)++;strcpy
		    (taskP.taskFrFunc(task), "handleIfsTimer");
		    if(txGTS->getByteLength() <= aMaxSIFSFrameSize)
			startIfsTimer(true);
		    else
			startIfsTimer(false);
		}
		else // time out when waiting for ack, normally impossible in GTS
		{
		    EV << "[MAC]: ACK timeout for " << txGTS->getName() << ":#" << (int)txGTS->getSeqNmbr() << endl;
		    numGTSRetry++;
		    if(numGTSRetry <= mpib.macMaxFrameRetries)
		    {
			// retry in next GTS
			EV << "[GTS]: retry in this GTS of next superframe, turn off radio now" << endl;
			taskP.taskStep(task) = 1;   // go back to step 1
			strcpy(taskP.taskFrFunc(task), "handleGtsTimer");
			waitGTSAck = false;
			// to avoid several consecutive PLME_SET_TRX_STATE_request are called at the same time, which may lead to error operation,
			// use phy_FORCE_TRX_OFF to turn off radio, because PHY will not send back a confirm from it
			PLME_SET_TRX_STATE_request(phy_FORCE_TRX_OFF);
		    }
		    else
		    {
			EV << "[GTS]: the max num of retries reached, task failed" << endl;
			taskP.taskStatus(task) = false; //task fails
			EV << "[MAC-TASK-FAIL]: reset TP_MCPS_DATA_REQUEST" << endl;
			waitGTSAck = false;
			// to avoid several consecutive PLME_SET_TRX_STATE_request are called at the same time, which may lead to error operation,
			// use phy_FORCE_TRX_OFF to turn off radio, because PHY will not send back a confirm from it
			PLME_SET_TRX_STATE_request(phy_FORCE_TRX_OFF);
			taskFailed('g', mac_NO_ACK);
		    }
		}
		break;

	    case 4:
		taskP.taskStatus(task) = false; // task ends successfully
		EV << "[GTS]: GTS transmission completes successfully, prepared for next GTS request" << endl;
		taskSuccess('g');
		break;

	    default:
		break;
	}
    }
    else
	error("[MAC]: undefined txOption: %d!", txOption);
}

void Ieee802154eMac::dispatch(PHYenum pStatus, const char *frFunc, PHYenum req_state, MACenum mStatus)
{
    FrameCtrl frmCtrl;
    bool isSIFS = false;
    //int i;

    /*****************************************/
    /************<csmacaCallBack()>**********/
    /***************************************/
    if(strcmp(frFunc, "csmacaCallBack") == 0)
    {
	if(txCsmaca == txBcnCmdUpper)
	{
	}
	else if(txCsmaca == txData)
	{
	    ASSERT(taskP.taskStatus(TP_MCPS_DATA_REQUEST) && (strcmp(taskP.taskFrFunc(TP_MCPS_DATA_REQUEST), frFunc) == 0));

	    FSM_MCPS_DATA_request(pStatus); // mStatus ignored
	}
    }

    /*****************************************/
    /*******<handle_PD_DATA_confirm()>*******/
    /***************************************/
    else if(strcmp(frFunc, "handle_PD_DATA_confirm") == 0) // only process phy_SUCCESS here
    {
	if(txPkt == txBeacon)
	{
	    {
		resetTRX();
		taskSuccess('b');   // beacon transmitted successfully
	    }
	}
	else if(txPkt == txAck)
	{
	    if(rxCmd != NULL)      // ack for cmd
	    {
		if(rxCmd->getByteLength() <= aMaxSIFSFrameSize)
		    isSIFS = true;
		startIfsTimer(isSIFS);
		resetTRX();
		taskSuccess('a');
	    }
	    else if(rxData != NULL) //default handling (virtually the only handling needed) for <rxData>
	    {
		if(rxData->getByteLength() <= aMaxSIFSFrameSize)
		    isSIFS = true;
		startIfsTimer(isSIFS);
//                if (rxData->getIsGTS()) // received in GTS
//                {
//                    if (isPANCoor)
//                    {
//                        // the device may transmit more pkts in this GTS, turn on radio
//                        PLME_SET_TRX_STATE_request(phy_RX_ON);
//                    }
//                    else
//                    {
//                        // PAN coordinator can transmit only one pkt to me in my GTS, turn off radio now
//                        PLME_SET_TRX_STATE_request(phy_TRX_OFF);
//                    }
//                }
//                else
		resetTRX();
		taskSuccess('a');
	    }
	    else    //ack. for duplicated packet
	    {
		resetTRX();
		taskSuccess('a');
	    }
	}
	else if(txPkt == txData)
	{
	    ASSERT((taskP.taskStatus(TP_MCPS_DATA_REQUEST)) && (strcmp(taskP.taskFrFunc(TP_MCPS_DATA_REQUEST), frFunc) == 0));

	    frmCtrl = txData->getFrmCtrl();
	    if(taskP.taskStatus(TP_MCPS_DATA_REQUEST))
	    {
		FSM_MCPS_DATA_request(pStatus); // mStatus ignored
	    }
	}
	else if(txPkt == txGTS)
	{
	    ASSERT((taskP.taskStatus(TP_MCPS_DATA_REQUEST)) && (strcmp(taskP.taskFrFunc(TP_MCPS_DATA_REQUEST), frFunc) == 0));
	    ASSERT(taskP.mcps_data_request_TxOption == GTS_TRANS);
	    // hand over back to FSM
	    FSM_MCPS_DATA_request(pStatus); // mStatus ignored
	}
	//else      //may be purged from pending list
    }

    /***************************************/
    /**************<handleAck()>************/
    /***************************************/
    else if(strcmp(frFunc, "handleAck") == 0) //always check the task status if the dispatch comes from recvAck()
    {
	if(txPkt == txData)
	{
	    if((taskP.taskStatus(TP_MCPS_DATA_REQUEST)) && (strcmp(taskP.taskFrFunc(TP_MCPS_DATA_REQUEST), frFunc) == 0))
		FSM_MCPS_DATA_request(pStatus); // mStatus ignored
	    else    //default handling for <txData>
	    {
		if(taskP.taskStatus(TP_MCPS_DATA_REQUEST)) //seems late ACK received
		    taskP.taskStatus(TP_MCPS_DATA_REQUEST) = false;
		resetTRX();
		taskSuccess('d');
	    }
	}
	else if(txPkt == txGTS)
	{
	    if((taskP.taskStatus(TP_MCPS_DATA_REQUEST)) && (strcmp(taskP.taskFrFunc(TP_MCPS_DATA_REQUEST), frFunc) == 0))
		FSM_MCPS_DATA_request(pStatus); // mStatus ignored
	}
    }

    /*****************************************/
    /*******<handleAckTimeoutTimer()>********/
    /***************************************/
    else if(strcmp(frFunc, "handleAckTimeoutTimer") == 0) //always check the task status if the dispatch comes from a timer handler
    {
	if(txPkt == txData)
	{
	    if((!taskP.taskStatus(TP_MCPS_DATA_REQUEST)) || (strcmp(taskP.taskFrFunc(TP_MCPS_DATA_REQUEST), "handleAck") != 0))
		return;

	    if(taskP.taskStatus(TP_MCPS_DATA_REQUEST))
		FSM_MCPS_DATA_request(phy_BUSY); // mStatus ignored, pStatus can be anything but phy_SUCCESS
	}
	else if(txPkt == txGTS)
	{
	    ASSERT((taskP.taskStatus(TP_MCPS_DATA_REQUEST)) && (strcmp(taskP.taskFrFunc(TP_MCPS_DATA_REQUEST), "handleAck") == 0));
	    ASSERT(taskP.mcps_data_request_TxOption == GTS_TRANS);
	    FSM_MCPS_DATA_request(phy_BUSY); // mStatus ignored, pStatus can be anything but phy_SUCCESS
	}
    }
}

//--------------------------------------------------------------------------------/
/****************************** <Utility Functions> *****************************/
//-------------------------------------------------------------------------------/
/**@author: 2014    Stefan Reis     (modified)
 * calculate the frame length in byte */
int Ieee802154eMac::calFrmByteLength(Ieee802154eFrame* frame) // TODO: extend it with the 802.15.4e frames [SR]
{
    //EV << "[MAC]: calculating size of " << frame->getName() << endl;

    //Ieee802154eFrame* tmpFrame = check_and_cast<Ieee802154eFrame *>(frame);

    Ieee802154eFrameType frmType = Ieee802154eFrameType(frame->getFrmCtrl().frameType);
    int byteLength = 0; // MHR + MAC payload + MFR
    int MHRLength = calMHRByteLength(frame->getFrmCtrl().srcAddrMode, frame->getFrmCtrl().dstAddrMode);

    if(frame->getDstPanId() == 0) // PAN ID not present
	MHRLength = MHRLength - 2;

    if(frame->getSrcPanId() == 0) // PAN ID not present
	MHRLength = MHRLength - 2;

    if(frame->getFrmCtrl().securityEnabled) // security field - Std 802.15.4-2006 (table 96) page 212
    {
	if(frame->getAuxSecHd().keyIdentMode == 0x00)
	{
	    MHRLength = MHRLength + 1 + 4;
	}
	else if(frame->getAuxSecHd().keyIdentMode == 0x01)
	{
	    MHRLength = MHRLength + 1 + 4 + 1;
	}
	else if(frame->getAuxSecHd().keyIdentMode == 0x02)
	{
	    MHRLength = MHRLength + 1 + 4 + 5;
	}
	else if(frame->getAuxSecHd().keyIdentMode == 0x03)
	{
	    MHRLength = MHRLength + 1 + 4 + 9;
	}
    }

    if(frmType == Ieee802154e_BEACON)   // Std 802.15.4-2006 (figure 10) page 21
    {
	if(frame->getFrmCtrl().frameVersion == Ieee802154_compatible)
	{
	    // Enhanced beacon frame - Std 802.15.4e-2012 (5.2.2.1 Beacon frame format) page 64
	    Ieee802154EnhancedBeaconFrame* enhBeaconFrm = check_and_cast<Ieee802154EnhancedBeaconFrame *>(frame);
	    if(enhBeaconFrm->getFrmCtrl().presIElist)
	    {
		MHRLength += (enhBeaconFrm->getIeHeaderArraySize() > 0) ? 1:0;
		for(unsigned int i = 0; i < enhBeaconFrm->getIeHeaderArraySize(); i++)
		{
		    MHRLength = MHRLength + enhBeaconFrm->getIeHeader(i).ieLenght;
		}
		MHRLength += (enhBeaconFrm->getIePayloadArraySize() > 0) ? 1:0;
		for(unsigned int i = 0; i < enhBeaconFrm->getIePayloadArraySize(); i++)
		{
		    MHRLength = MHRLength + enhBeaconFrm->getIePayload(i).ieLenght;
		}
	    }
	    // beacon payload
	    byteLength = MHRLength + (frame->getFrmCtrl().seqNbSup ? 0:1) + 2; //SequencNumber + FCS (2 byte)
	}
	else
	{
	    Ieee802154eBeaconFrame* beaconFrm = check_and_cast<Ieee802154eBeaconFrame *>(frame);
	    //byteLength = MHRLength + 2 + (2+ gtsCount * 3) + (1 + beaconFrm->getPaFields().numShortAddr * 2 + beaconFrm->getPaFields().numExtendedAddr * 8) + 2;
	    // see Std 802.15.4-2006 (figure 44) page 143
	    byteLength = MHRLength + 2 + (beaconFrm->getGtsField().gtsDescrCnt == 0 ? 1:2) + beaconFrm->getGtsField().gtsDescrCnt * 3 + 1 + beaconFrm->getPendingAdrrFld().numShortAddr * 2 + beaconFrm->getPendingAdrrFld().numExtendedAddr * 8 + 2;
	}
    }
    else if(frmType == Ieee802154e_DATA) // Std 802.15.4-2006 (figure 11) page 22 , MAC payload not included here, will be added automatically while encapsulation later on
    { // 2 Frame Control + 1 Sequence Number + 0-20 Addressing fields + 0-14 Auxiliary Security Header + 2 FCS - Std 802.15.4-2011 (figure 46) page 65
	byteLength = MHRLength + (frame->getFrmCtrl().seqNbSup ? 0:1) + 2;
    }
    else if(frmType == Ieee802154e_MULTI) // Std 802.15.4e-2012 (figure 48j) page 74, MAC payload not included here, will be added automatically while encapsulation later on
    { // 1-2 Frame Control + 0-1 Sequence Number + 0-20 Addressing fields + 0-14 Auxiliary Security Header + ? Information Elements + 2 FCS
	Ieee802154eMPFrame* tmpMPFrame = check_and_cast<Ieee802154eMPFrame *>(frame);
	//TODO: [SR] calc the size of the Information-Elements
	byteLength = MHRLength + (tmpMPFrame->getMpFrmCtrl().lngFrameCtrl ? 1:0) + (tmpMPFrame->getFrmCtrl().seqNbSup ? 0:1) + (!tmpMPFrame->getMpFrmCtrl().presPANID ? -4:0) + 2;
    }
    else if(frmType == Ieee802154e_ACK) // Std 802.15.4-2006 (figure 12) page 22
    {
	if(frame->getFrmCtrl().frameVersion == Ieee802154_compatible)
	{
	    // Enhanced acknowledge frame - Std 802.15.4e-2012 (5.2.2.3 Acknowledgment frame format) page 66
	    Ieee802154eEnhancedAckFrame* enhAckFrm = check_and_cast<Ieee802154eEnhancedAckFrame *>(frame);
	    if(enhAckFrm->getFrmCtrl().presIElist)
	    {
		MHRLength += (enhAckFrm->getIeHeaderArraySize() > 0) ? 1:0;
		for(unsigned int i = 0; i < enhAckFrm->getIeHeaderArraySize(); i++)
		{
		    MHRLength = MHRLength + enhAckFrm->getIeHeader(i).ieLenght;
		}
		MHRLength += (enhAckFrm->getIePayloadArraySize() > 0) ? 1:0;
		for(unsigned int i = 0; i < enhAckFrm->getIePayloadArraySize(); i++)
		{
		    MHRLength = MHRLength + enhAckFrm->getIePayload(i).ieLenght;
		}
	    }
	    byteLength = MHRLength + (frame->getFrmCtrl().seqNbSup ? 0:1) + 2;
	}
	else
	    // Acknowledge frame - Std 802.15.4-2006/2011
	    byteLength = SIZE_OF_802154_ACK;
    }
    else if(frmType == Ieee802154e_CMD) // Std 802.15.4-2006 (figure 13) page 23
    {
	Ieee802154eCmdFrame* cmdFrm = check_and_cast<Ieee802154eCmdFrame *>(frame);
	switch(cmdFrm->getCmdType())
	{
	    case ASSOCIATION_REQUEST: // Fig 48: MHR (17/23) + Payload (2) + FCS (2)
		byteLength = MHRLength + 4;
		break;

	    case ASSOCIATION_RESPONSE:
		byteLength = SIZE_OF_802154_ASSOCIATION_RESPONSE;
		break;

	    case DISASSOCIATION_NOTIFICATION:
		byteLength = SIZE_OF_802154_DISASSOCIATION_NOTIFICATION;
		break;

	    case DATA_REQUEST: // Fig 52: MHR (7/11/13/17) + Payload (1) + FCS (2)
		byteLength = MHRLength + 3;
		break;

	    case PAN_ID_CONFLICT_NOTIFICATION:
		byteLength = SIZE_OF_802154_PANID_CONFLICT_NOTIFICATION;
		break;

	    case ORPHAN_NOTIFICATION:
		byteLength = SIZE_OF_802154_ORPHAN_NOTIFICATION;
		break;

	    case BEACON_REQUEST:
		byteLength = SIZE_OF_802154_BEACON_REQUEST;
		break;

	    case COORDINATOR_REALIGNMENT: // Fig 56: MHR (17/23) + Payload (8) + FCS (2)
		byteLength = MHRLength + 10;
		break;

	    case GTS_REQUEST:
		byteLength = SIZE_OF_802154_GTS_REQUEST;
		break;

	    default:
		error("[MAC]: cannot calculate the size of a MAC command frame with unknown type!");
		break;
	}
    }
    else
	error("[MAC]: cannot calculate the size of a MAC frame with unknown type!");

    //EV << "[MAC]: header size is " << MHRLength << " Bytes" << endl;
    //EV << "[MAC]: MAC frame size is " << byteLength << " Bytes" << endl;
    return byteLength;
}

/**@author: 2014    Stefan Reis     (modified)
 * calculate the MHR byte length depending on the source and destiny address mode */
int Ieee802154eMac::calMHRByteLength(uint8_t addrModeSrc, uint8_t addrModeDst)
{
    int sum = 2; // frame control - see Std 802.15.4e-2012 (5.2.1.1) page 59
    switch(addrModeSrc)
    {
	case 0:
	    sum += 0;
	    break;
	case 1:
	    sum += 2 + 1;
	    break;
	case 2:
	    sum += 2 + 2;
	    break;
	case 3:
	    sum += 2 + 8;
	    break;
	default:
	    error("[MAC]: impossible address src mode sum!");
	    break;
    }
    switch(addrModeDst)
    {
	case 0:
	    sum += 0;
	    break;
	case 1:
	    sum += 2 + 1;
	    break;
	case 2:
	    sum += 2 + 2;
	    break;
	case 3:
	    sum += 2 + 8;
	    break;
	default:
	    error("[MAC]: impossible address dst mode sum!");
	    break;
    }

    return sum;
}

/**@author: 2014    Stefan Reis
 * calculate the state of the PAN ID compression - Std 802.15.4e-2012 (table 2a) page 61 */
bool Ieee802154eMac::getPANIDComp(FrameCtrl frmCtrl, UINT_16 srcPanId, UINT_16 dstPanId)
{
    if(frmCtrl.srcAddrMode == defFrmCtrl_AddrModeNone && frmCtrl.dstAddrMode == defFrmCtrl_AddrModeNone && frmCtrl.frameVersion == Ieee802154_compatible && srcPanId == 0 && dstPanId != 0)
    {
	return true;
    }
    else if(frmCtrl.srcAddrMode != defFrmCtrl_AddrModeNone && frmCtrl.dstAddrMode == defFrmCtrl_AddrModeNone && frmCtrl.frameVersion == Ieee802154_compatible && srcPanId == 0 && dstPanId == 0)
    {
	return true;
    }
    else if(frmCtrl.srcAddrMode == defFrmCtrl_AddrModeNone && frmCtrl.dstAddrMode != defFrmCtrl_AddrModeNone && frmCtrl.frameVersion == Ieee802154_compatible && srcPanId == 0 && dstPanId == 0 && frmCtrl.frameType != Ieee802154e_MULTI)
    {
	return true;
    }
    else if(frmCtrl.srcAddrMode != defFrmCtrl_AddrModeNone && frmCtrl.dstAddrMode != defFrmCtrl_AddrModeNone && (frmCtrl.frameVersion == Ieee802154_2003_compatible || frmCtrl.frameVersion == Ieee802154_2006_introduced) && srcPanId == 0 && dstPanId != 0)
    {
	return true;
    }
    else if(frmCtrl.srcAddrMode != defFrmCtrl_AddrModeNone && frmCtrl.dstAddrMode != defFrmCtrl_AddrModeNone && frmCtrl.frameVersion == Ieee802154_compatible && srcPanId == 0 && dstPanId == 0)
    {
	return true;
    }
    else
    {
	return false;
    }
}

/** calculated the transmission duration */
simtime_t Ieee802154eMac::calDuration(Ieee802154eFrame* frame)
{
    return (def_phyHeaderLength * 8 + frame->getBitLength()) / phy_bitrate;
}

/** calculated the bitrate */
double Ieee802154eMac::getRate(char bitOrSymbol)
{
    double rate;

    if(ppib.phyCurrentChannel == 0)
    {
	if(bitOrSymbol == 'b')
	    rate = BR_868M;
	else
	    rate = SR_868M;
    }
    else if(ppib.phyCurrentChannel <= 10)
    {
	if(bitOrSymbol == 'b')
	    rate = BR_915M;
	else
	    rate = SR_915M;
    }
    else
    {
	if(bitOrSymbol == 'b')
	    rate = BR_2_4G;
	else
	    rate = SR_2_4G;
    }
    return (rate * 1000);
}

/**@author: 2014    Stefan Reis     (modified)
 * check if the msg goes to the PAN coordinator */
bool Ieee802154eMac::toParent(Ieee802154eFrame* frame)
{
    Ieee802154eFrame* tmpFrame = check_and_cast<Ieee802154eFrame *>(frame);

    if(((tmpFrame->getFrmCtrl().dstAddrMode == defFrmCtrl_AddrMode16) && (tmpFrame->getDstAddr().getInt() == mpib.macCoordShortAddress)) || ((tmpFrame->getFrmCtrl().dstAddrMode == defFrmCtrl_AddrMode64) && (tmpFrame->getDstAddr() == mpib.macCoordExtendedAddress)) || (tmpFrame->getDstAddr() == (IE3ADDR)0xffff))
    {
	return true;
    }
    else
    {
	return false;
    }
}

/**@author: 2014    Stefan Reis
 * calculate the time when the first bit of the frame was received */
simtime_t Ieee802154eMac::getArrivalTime(Ieee802154eFrame* frame)
{
    simtime_t now, duration;
    now = simTime();

    //calculate the time when the first bit of the frame was received
    duration = calDuration(frame);
    return (now - duration);
}

/**@author: 2014    Stefan Reis
 * calculate the time correction */
void Ieee802154eMac::calcTimeCorr(Ieee802154eFrame* frame)
{
    double correction = (startTimeslot + (timeslotTable->getTemplate(useTimeslotID)->getRxOffsetDbl() + (timeslotTable->getTemplate(useTimeslotID)->getRxWaitDbl() / 2)) - getArrivalTime(frame)).dbl();
    ;

    // transfer the double value in the range of -2048e-6 sec to 2047e-6 sec
    double tmpTime = correction;
    UINT_16 tmpCorr = 0x00;

    if(correction <= -0.000001 || correction <= 0.000001)
    {
	tmpTime = 0.0;
    }
    else if(correction >= 0.0)
    {
	// cut all values higher then 9999e-6 sec
	tmpCorr = tmpTime * 100;
	tmpTime = tmpTime - (double(tmpCorr) / 100);

	tmpCorr = tmpTime * 100000;
	tmpTime = tmpCorr > 2047 ? 0.002047:(double(tmpCorr) / 100000);

	if(tmpCorr > 0x7ff) // max value for the time synch info is -2048 to 2047 micro seconds
	    tmpCorr = 0x7ff;
    }
    else
    {
	// cut all values higher then -9999e-6 sec
	tmpCorr = tmpTime * 100;
	tmpTime = tmpTime + (double(tmpCorr) / 100);

	tmpCorr = tmpTime * 100000;
	tmpTime = tmpCorr > 2048 ? -0.002048:-(double(tmpCorr) / 100000);

	if(tmpCorr > 0x7ff)
	    tmpCorr = 0x7ff;

	tmpCorr += 0x800;
    }
    timeError = tmpCorr;
    correction = tmpTime;

//    if(correction != 0.0)
//	int test = 0;

    if(!isPANCoor) // made only the calculation if the
	timeCorrection = (timeCorrection == 0.0) ? correction:((timeCorrection + correction) / 2);
    else
	timeCorrection = 0.0;
}

//-----------------------------------------------------------------------------------------------------------/
/****************************** <GTS Functions> *****************************/
//----------------------------------------------------------------------------------------------------------/
/**
 * PAN coordinator uses this function to schedule for starting of each GTS in the GTS list
 */
void Ieee802154eMac::gtsScheduler()
{
//    uint16_t t_SO;
    simtime_t w_time, tmpf;

    ASSERT(isPANCoor);
    tmpf = mpib.macBeaconTxTime + gtsList[index_gtsTimer].startSlot * txSfSlotDuration / phy_symbolrate;
    w_time = tmpf - simTime();

    // should turn on radio receiver aTurnaroundTime symbols before GTS starts, if it is a transmit GTS relative to device
    if(!gtsList[index_gtsTimer].isRecvGTS)
	w_time = w_time - Ieee802154ePhy::aTurnaroundTime / phy_symbolrate;

    EV << "[GTS]: schedule for starting of GTS index: " << index_gtsTimer << ", slot:#" << (int)gtsList[index_gtsTimer].startSlot << " with " << w_time << " s" << endl;
    startGtsTimer(w_time);
}

void Ieee802154eMac::startGtsTimer(simtime_t w_time)
{
    if(gtsTimer->isScheduled())
	cancelEvent(gtsTimer);

    scheduleAt(simTime() + w_time, gtsTimer);
}

void Ieee802154eMac::handleGtsTimer()
{
    simtime_t w_time;

    // for PAN coordinator
    if(isPANCoor)
    {
	indexCurrGts = index_gtsTimer;
	EV << "[GTS]: GTS with index = " << (int)indexCurrGts << " in my GTS list starts now!" << endl;
	EV << "allocated for MAC address = " << (int)gtsList[indexCurrGts].devShortAddr << ", startSlot = " << (int)gtsList[indexCurrGts].startSlot << ", length = " << (int)gtsList[indexCurrGts].length << ", isRecvGTS = " << gtsList[indexCurrGts].isRecvGTS << ", isTxPending = " << gtsList[indexCurrGts].isTxPending << endl;

	// is transmit GTS relative to device , turn on radio receiver
	if(!gtsList[indexCurrGts].isRecvGTS)
	{
	    EV << "[GTS]: tell PHY to turn on radio receiver and prepare for receiving pkts from device" << endl;
	    PLME_SET_TRX_STATE_request(phy_RX_ON);
	}
	// my time to transmit pkts to certain device
	else
	{
	    // if there is a data buffered for transmission in current GTS
	    if(txGTS && gtsList[indexCurrGts].isTxPending)
	    {
		ASSERT(taskP.taskStatus(TP_MCPS_DATA_REQUEST) && taskP.taskStep(TP_MCPS_DATA_REQUEST) == 1);
		// hand over to FSM, which will go to next step
		// no need to call gtsCanProceed() at this time, timing is already guaranteed when allocating GTS
		EV << "[GTS]: a data pending for this GTS found in the buffer, starting GTS transmission now" << endl;
		FSM_MCPS_DATA_request();    // state parameters are ignored
	    }
	    // turn off radio
	    else
	    {
		EV << "[GTS]: no data pending for current transmit GTS, turn off radio now" << endl;
		PLME_SET_TRX_STATE_request(phy_TRX_OFF);
	    }
	}

	if(index_gtsTimer > 0)
	{
	    index_gtsTimer--;
	    gtsScheduler();
	}
	// index_gtsTimer == 0, now is the starting of the last GTS in current CFP. need to do nothing here
	// at the end of CFP, indexCurrGts will be reset in handleSDTimer()
    }

    // for devices
    else
    {
	// my GTS starts
	if(index_gtsTimer != 99)
	{
	    EV << "[GTS]: my GTS starts now, isRecvGTS = " << isRecvGTS << endl;

	    // is receive GTS, turn on radio receiver
	    if(isRecvGTS)
	    {
		EV << "[GTS]: tell PHY to turn on radio receiver and prepare for receiving pkts from PAN coordinator" << endl;
		PLME_SET_TRX_STATE_request(phy_RX_ON);
	    }
	    // my GTS to transmit pkts to PAN coordinator
	    else
	    {
		// each device can have at most one GTS
		if(txGTS)
		{
		    ASSERT(taskP.taskStatus(TP_MCPS_DATA_REQUEST) && taskP.taskStep(TP_MCPS_DATA_REQUEST) == 1);
		    // hand over to FSM, which will go to next step
		    // no need to call gtsCanProceed() at this time, timing is already guaranteed when applying for GTS
		    EV << "[GTS]: a data is pending for this GTS in the buffer, starting GTS transmission now" << endl;
		    FSM_MCPS_DATA_request();    // state parameters are ignored
		}
		else
		{
		    EV << "[GTS]: no data pending for GTS transmission, turn off radio now" << endl;
		    // to avoid several consecutive PLME_SET_TRX_STATE_request are called at the same time, which may lead to error operation,
		    // use phy_FORCE_TRX_OFF to turn off radio, because PHY will not send back a confirm from it
		    PLME_SET_TRX_STATE_request(phy_FORCE_TRX_OFF);
		    // if data from upper layer arrives during this GTS, radio may be turned on again
		}
	    }

	    // schedule for the end of my GTS slot in order to put radio into sleep when GTS of current CFP expires
	    index_gtsTimer = 99;
	    // calculate the duration of my GTS
	    w_time = gtsLength * rxSfSlotDuration / phy_symbolrate;
	    if(isRecvGTS)
		w_time += Ieee802154ePhy::aTurnaroundTime / phy_symbolrate;
	    EV << "[GTS]: scheduling for the end of my GTS slot" << endl;
	    startGtsTimer(w_time);
	}
	// my GTS expired, turn off radio
	else
	{
	    index_gtsTimer = 0;
	    EV << "[GTS]: now is the end of my GTS, turn off radio now" << endl;
	    PLME_SET_TRX_STATE_request(phy_TRX_OFF);
	}
    }
}

/**
 *  This function accepts GTS request from devices and allocates GTS slots
 *  The return value indicates the GTS start slot for corresponding device
 *  Note: devices are allowed to call this function only in CAP
 */
uint16_t Ieee802154eMac::gts_request_cmd(UINT_16 macShortAddr, uint16_t length, bool isReceive)
{
    Enter_Method_Silent
    ();
    uint16_t startSlot;
    UINT_32 t_cap;
    ASSERT(isPANCoor);
    EV << "[GTS]: the PAN coordinator is processing GTS request from MAC address " << macShortAddr << endl;
    // check whether this device is device list
    if(deviceList.find(macShortAddr) == deviceList.end())
    {
	EV << "[GTS]: the address " << macShortAddr << " not found in the device list, no GTS allocated" << endl;
	return 0;
    }
    else if(gtsCount >= 7)
    {
	EV << "[GTS]: max number of GTSs reached, no GTS available for the MAC address " << macShortAddr << endl;
	return 0;
    }

    // check if the min CAP length can still be satisfied after this allocation
    t_cap = txSfSlotDuration * (tmp_finalCap - length + 1);
    if(t_cap < aMinCAPLength)
    {
	EV << "[GTS]: violation of the min CAP length, no GTS allocated for the MAC address " << macShortAddr << endl;
	return 0;
    }

    // update final CAP and calculate start slot for this GTS
    tmp_finalCap = tmp_finalCap - length;
    startSlot = tmp_finalCap + 1;
    // add new GTS descriptor
    gtsList[gtsCount].devShortAddr = macShortAddr;
    gtsList[gtsCount].startSlot = startSlot;
    gtsList[gtsCount].length = length;
    gtsList[gtsCount].isRecvGTS = isReceive;
    EV << "[GTS]: add a new GTS descriptor with index = " << (int)gtsCount << " for MAC address = " << macShortAddr << ", startSlot = " << (int)startSlot << ", length = " << (int)length << ", isRecvGTS = " << isReceive << endl;
    gtsCount++;
    // new parameters put into effect when txing next beacon
    //isGtsUpdate = true;
    return startSlot;
}

void Ieee802154eMac::startFinalCapTimer(simtime_t w_time)
{
    if(finalCAPTimer->isScheduled())
	cancelEvent(finalCAPTimer);

    scheduleAt(simTime() + w_time, finalCAPTimer);
}

void Ieee802154eMac::handleFinalCapTimer()
{
    // only be called when my GTS is not the first one in the CFP
    ASSERT(gtsStartSlot > rxSfSpec.finalCap + 1);
    EV << "[GTS]: it's now the end of CAP, turn off radio and wait for my GTS coming" << endl;
    PLME_SET_TRX_STATE_request(phy_TRX_OFF);
}

UINT_16 Ieee802154eMac::associate_request_cmd(IE3ADDR extAddr, const DevCapability& capaInfo)
{
    Enter_Method_Silent
    ();
    // store MAC address and capability info
    // here simply assign short address with its extended address
    UINT_16 shortAddr = getShortAddress(extAddr);
    deviceList[shortAddr] = capaInfo;
    EV << "[MAC]: associate request received from " << capaInfo.hostName << " with MAC address: " << extAddr << endl;
    return shortAddr;
}

/**
 *  Devices should check if the requested GTS transaction can be finished  before the end of corresponding GTS
 *  no need for PAN coordinator to do this, because it only transmits at the starting of the GTS,
 *  of which the timing is already guaranteed by device side when requesting for receive GTS
 */
bool Ieee802154eMac::gtsCanProceed()
{
    simtime_t t_left;
    EV << "[GTS]: checking if the data transaction can be finished before the end of current GTS ... ";
    // to be called only by the device in its GTS
    ASSERT(index_gtsTimer == 99 && gtsTimer->isScheduled());
    // calculate left time in current GTS
    t_left = gtsTimer->getArrivalTime() - simTime();

    if(t_left > gtsTransDuration)
    {
	EV << "yes" << endl;
	return true;
    }
    else
    {
	EV << "no" << endl;
	return false;
    }
}

/** give a MACAddress from a UINT_16 address back */
MACAddress Ieee802154eMac::getLongAddress(UINT_16 val)
{
    MACAddress tmp;

    unsigned char addrbytes[MAC_ADDRESS_SIZE64];
    addrbytes[0] = 0x00;
    addrbytes[1] = 0x00;
    addrbytes[2] = 0x00;
    addrbytes[3] = 0xff;
    addrbytes[4] = 0xfe;
    addrbytes[5] = 0x00;
    addrbytes[6] = (val >> 8) & 0xff;
    addrbytes[7] = (val >> 0) & 0xff;
    tmp.setFlagEui64(true);
    tmp.setAddressBytes(addrbytes);
    return tmp;
}

/**@author: 2014    Stefan Reis
 * gives the active timeslot back, depending at the condition:
 * When,for any given timeslot, a device has links in multiple slotframes, transmissions take precedence over
 * receives, and lower slotframeHandle slotframes takes precedence over higher slotframeHandle slotframes.
 * - for TSCH links
 * see Std 802.15.4e-2012 (5.1.1.5.4 Multiple slotframes) page 18 */
macLinkTableEntry* Ieee802154eMac::getLinkEntry(UINT_64 asnCounter)
{
    if(mpib.macTSCHenabled)
    {
	// collect all links for this ASN
	std::list<macLinkTableEntry*> tmpLinkVector;
	macSlotframeTableEntry* tmpSlotframe;
	macLinkTableEntry* tmpLink = new macLinkTableEntry();
	for(int i = 0; i < slotframeTable->getNumSlotframes(); i++)
	{
	    tmpSlotframe = slotframeTable->getSlotframe(i);
	    linkTable->getLinksByTimeslotSlotframe((asnCounter) % tmpSlotframe->getSlotframeSize(), i, &tmpLinkVector);
	    if(tmpLink != NULL)
	    {
		if(tmpLink->getLinkOption() == LNK_OP_TRANSMIT)
		    tmpLinkVector.push_front(tmpLink);
		else if(tmpLink->getLinkOption() == LNK_OP_RECEIVE)
		    tmpLinkVector.push_back(tmpLink);
		else if(tmpLink->getLinkOption() == LNK_OP_SHARED)
		    tmpLinkVector.push_back(tmpLink);
		else if(tmpLink->getLinkOption() == LNK_OP_RECEIVE_TIMEKEEPING)
		    tmpLinkVector.push_back(tmpLink);
		else if(tmpLink->getLinkOption() == LNK_OP_SHARED_RECEIVE)
		    tmpLinkVector.push_back(tmpLink);
	    }
	}

	// if no link if found, give a reserved link back, for go back to sleep
	if(tmpLinkVector.size() == 0)
	{
	    return NULL;
	}
	else
	{ // filter the links - Std 802.15.4e-2012 (5.1.1.5.4 Multiple slotframes) page 18
	  // note: transmissions take over receives, and lower slotframeHandle slotframes takes precedence over higher slotframeHandle slotframes
	    macLinkTableEntry* tmpEntry = 0;
	    for(std::list<macLinkTableEntry*>::iterator i = tmpLinkVector.begin(); i != tmpLinkVector.end(); ++i)
	    {
		if(tmpEntry == NULL)
		{
		    if(queueModule->requestSpcPacket((IE3ADDR)(*i)->getNodeAddress()) != NULL)
			return (*i);
		    else
			tmpEntry = (*i);
		}

		else if(tmpEntry->getLinkOption() > (*i)->getLinkOption() || (tmpEntry->getLinkOption() == (*i)->getLinkOption() && tmpEntry->getSlotframeId() > (*i)->getSlotframeId()))

		{

		    if(queueModule->requestSpcPacket((IE3ADDR)tmpEntry->getNodeAddress()) != NULL)
			return (tmpEntry);
		    else
			tmpEntry = (*i);

		}
		else
		{
		    if(queueModule->requestSpcPacket((IE3ADDR)tmpEntry->getNodeAddress()) != NULL)
			return (tmpEntry);
		    else
			tmpEntry = (*i);
		}
	    }
	    return tmpEntry;
	}
    }
    return NULL;
}

/**@author: 2014    Stefan Reis
 * gives the next active timeslot and recalculated the ASN for this timeslot
 * - for TSCH links
 * see Std 802.15.4e-2012 (5.1.1.5.3 Links) page 17 */
double Ieee802154eMac::getNextActiveTimeslot()
{
    if(mpib.macTSCHenabled)
    {
	double waitTime = timeslotTable->getTemplate(useTimeslotID)->getTimeslotLengthDbl();
	waitTime = (waitTime + (timeCorrection / 1000000)); //note: timeSource have a timeCorrection = 0
	int count = 0;
	UINT_64 asn = mpib.macASN;
	UINT_64 maxLoop = 0xfffffffffff;
	asn++;
	while(getLinkEntry(asn) == NULL && maxLoop > 0)
	{
	    count++;
	    asn++;
	    maxLoop--;
	}

	if(maxLoop == 0)
	{ // if no active link is found, wake up next timeslot and search again
	    asn = mpib.macASN + 1;
	    count = 1;
	}

	if(asn > 0xffffffffff)
	    asn = asn - 0xffffffffff;

	mpib.macASN = asn;
	return waitTime + count * waitTime;
    }
    return 0.0;
}

/**@author: 2014    Stefan Reis
 * handle the Header and Payload IE's field
 * for Std 802.15.4e-2012 Enhanced Data, Enhanced Beacon and Enhanced Acknowledge frames */
bool Ieee802154eMac::handleIEfield(Ieee802154eFrame* frame)
{
    if(mpib.macTSCHcapable)
    {
	Ieee802154eFrame *tmpFrame = check_and_cast<Ieee802154eFrame *>(frame);
	bool isNACK = false;

	if(tmpFrame->getFrmCtrl().presIElist)
	{
	    for(unsigned int i = 0; i < tmpFrame->getIeHeaderArraySize(); i++)
	    {
		//TODO:[SR] include the Std 802.15.4e-2012 (table 4b) page 81
		IE_Header tmpHeader = tmpFrame->getIeHeader(i);
		if(tmpHeader.typeDescriptor)
		    break;

		if(tmpHeader.elementID == IE_HD_LE_CSL)
		{
		    break;
		}
		else if(tmpHeader.elementID == IE_HD_LE_RIT)
		{
		    break;
		}
		else if(tmpHeader.elementID == IE_HD_DSME_PAN_Descriptor)
		{
		    break;
		}
		else if(tmpHeader.elementID == IE_HD_RZ_TIME)
		{
		    break;
		}
		else if(tmpHeader.elementID == IE_HD_ACK_NACK_TIME_CORRECTION)
		{
		    double tmpTime = 0.0;
		    UINT_16 tmpTSI = 0x0000;
		    if(isPANCoor || neighborTable->isNeighborTimeSource(getShortAddress(frame->getSrcAddr())))
			break;
		    else
		    {
			if(tmpHeader.ieACKNACKTimeCorrc != NULL && tmpHeader.ieACKNACKTimeCorrc->timeSyncInfo > 0)
			{
			    tmpTSI = tmpHeader.ieACKNACKTimeCorrc->timeSyncInfo;
			}
			if(tmpTSI > 0x8000)
			{ //NACK
			    isNACK = true;
			    tmpTSI = tmpTSI - 0x8000;
			    // positive / negative time correction
			    tmpTime = (tmpTSI > 0x07ff) ? (tmpTSI - 0x800):tmpTSI;
			    tmpTime = tmpTime / 1000000;
			    tmpTime = (tmpTSI > 0x07ff) ? -tmpTime:tmpTime;
			}
			else
			{
			    // positive / negative time correction
			    tmpTime = (tmpTSI > 0x07ff) ? (tmpTSI - 0x800):tmpTSI;
			    tmpTime = tmpTime / 1000000;
			    tmpTime = (tmpTSI > 0x07ff) ? -tmpTime:tmpTime;
			}
			// calculate the average time correction
			if(timeCorrection == 0.0)
			    timeCorrection = tmpTime;
			else
			    timeCorrection = (tmpTime + timeCorrection) / 2;
		    }
		}
		else if(tmpHeader.elementID == IE_HD_GROUP_ACK)
		{
		    break;
		}
		else if(tmpHeader.elementID == IE_HD_LOW_LATENCY_NETWORK_INFO)
		{
		    break;
		}
		else if(tmpHeader.elementID == IE_HD_LIST_TERMINATION_1)
		{ // followed by payload IE's
		    break;
		}
		else if(tmpHeader.elementID == IE_HD_LIST_TERMINATION_2)
		{ // followed by unformatted payload
		    break;
		}
	    }

	    for(unsigned int i = 0; i < tmpFrame->getIePayloadArraySize(); i++)
	    {
		IE_Payload tmpPayload = tmpFrame->getIePayload(i);
		if(!tmpPayload.typeDescriptor)
		    break;

		if(tmpPayload.groupID == IE_PL_ESDU)
		{
		    //XXX:[SR] ESDU not implemented
		    //IE_ESDU* tmpESDU = tmpPayload.ieESDU;
		}
		else if(tmpPayload.groupID == IE_PL_MLME)
		{
		    IE_MLME* tmpMLME = tmpPayload.ieMLME;
		    std::list<IE_MLME_Sub*>::iterator it;

		    for(it = tmpMLME->ieMLMESub.begin(); it != tmpMLME->ieMLMESub.end(); ++it)
		    {
			if((*it)->ieMLMEtype == false) //short
			{
			    if((*it)->ieMLMESubID == IE_MLME_SHORT_TSCH_SYNCHRONISATION)
			    {
				IE_TSCH_SYNC* tmpSync = (*it)->ieTSCHSync;
				mpib.macASN = tmpSync->tschASN;
				// set the ASN to the next timeslot
				mpib.macASN = mpib.macASN + 1;

				if(mpib.macASN > 0xffffffffff)
				    mpib.macASN = 0;

				//XXX:[SR] handle Joint priority (RPL rank)
			    }
			    else if((*it)->ieMLMESubID == IE_MLME_SHORT_TSCH_SLOTFRAME_LINK)
			    {
				//TODO:[SR] handle SLOTFRAME_LINK
				//for(int i = 0; i < (*it)->ieTSCHSLotfrmLink->)
				slotframeLinkIEEntry *tmpSlotLink = (*it)->ieTSCHSLotfrmLink->getSlotframeLinkIEEntry();
				for(int i = 0; i < tmpSlotLink->getNumSlotframes(); i++)
				{
				    macSlotframeTableEntry *tmpSlot = new macSlotframeTableEntry();
				    tmpSlot->setSlotframeId(tmpSlotLink->getSlotframeIE(i)->getSlotframeId());
				    tmpSlot->setSlotframeSize(tmpSlotLink->getSlotframeIE(i)->getSlotframeSize());
				    if(slotframeTable->getSlotframeById(tmpSlotLink->getSlotframeIE(i)->getSlotframeId()) == NULL)
					slotframeTable->addSlotframe(tmpSlot);
				    else
				    {
					slotframeTable->editSlotframe(tmpSlot);
				    }
				    for(int k = 0; k < tmpSlotLink->getSlotframeIE(i)->getNumLinks(); k++)
				    {
					if(linkTable->getLinkByTimeslotSlotframe(tmpSlotLink->getSlotframeIE(i)->getLinkIE(k)->getTimeslot(), tmpSlotLink->getSlotframeIE(i)->getSlotframeId()) == NULL)
					{
					    macLinkTableEntry *tmpLink = new macLinkTableEntry();
					    tmpLink->setChannelOffset(tmpSlotLink->getSlotframeIE(i)->getLinkIE(k)->getChannelOffset());
					    tmpLink->setLinkOption(tmpSlotLink->getSlotframeIE(i)->getLinkIE(k)->getLinkOption());

					    tmpLink->setLinkType(tmpSlotLink->getSlotframeIE(i)->getLinkIE(k)->getLinkTyp());

					    if(tmpSlotLink->getSlotframeIE(i)->getLinkIE(k)->getLinkTyp() == (LNK_TP_ADVERTISING || LNK_TP_JOIN))
						tmpLink->setNodeAddress(0xffff);
					    else
						tmpLink->setNodeAddress(getShortAddress(tmpFrame->getSrcAddr()));

					    tmpLink->setSlotframeId(tmpSlotLink->getSlotframeIE(i)->getSlotframeId());
					    tmpLink->setTimeslot(tmpSlotLink->getSlotframeIE(i)->getLinkIE(k)->getTimeslot());
					    tmpLink->setLinkId(linkTable->getNumLinks());
					    tmpLink->setMacLinkTable(linkTable);
					    linkTable->addLink(tmpLink);
					}
					else
					{
					    macLinkTableEntry *tmpLink = new macLinkTableEntry();
					    tmpLink->setChannelOffset(tmpSlotLink->getSlotframeIE(i)->getLinkIE(k)->getChannelOffset());
					    tmpLink->setLinkOption(tmpSlotLink->getSlotframeIE(i)->getLinkIE(k)->getLinkOption());
					    tmpLink->setLinkType(tmpSlotLink->getSlotframeIE(i)->getLinkIE(k)->getLinkTyp());
					    if(tmpSlotLink->getSlotframeIE(i)->getLinkIE(k)->getLinkTyp() == (LNK_TP_ADVERTISING || LNK_TP_JOIN))
						tmpLink->setNodeAddress(0xffff);
					    else
						tmpLink->setNodeAddress(getShortAddress(tmpFrame->getSrcAddr()));
					    tmpLink->setSlotframeId(tmpSlotLink->getSlotframeIE(i)->getSlotframeId());
					    tmpLink->setTimeslot(tmpSlotLink->getSlotframeIE(i)->getLinkIE(k)->getTimeslot());
					    tmpLink->setLinkId(linkTable->getLinkByTimeslotSlotframe(tmpSlotLink->getSlotframeIE(i)->getLinkIE(k)->getTimeslot(), tmpSlotLink->getSlotframeIE(i)->getSlotframeId())->getLinkId());
					    tmpLink->setMacLinkTable(linkTable);
					    linkTable->editLink(tmpLink);
					}

				    }

				}
			    }
			    else if((*it)->ieMLMESubID == IE_MLME_SHORT_TSCH_TIMESLOT)
			    {
				macTimeslotTableEntry* tmpTimeslot = (*it)->ieTSCHTimeslt;

				if(timeslotTable->getNumTemplates() == 0)
				{
				    tmpTimeslot->setTemplateId(timeslotTable->getNumTemplates() + 1);
				    timeslotTable->addTemplate(tmpTimeslot);
				    useTimeslotID = tmpTimeslot->getTemplateId();

				}
				else if(tmpTimeslot->getTemplateId() + 1 > timeslotTable->getNumTemplates())
				{
				    EV << "[MAC]handleBcn (enhanced beacon): ERROR wrong timeslot ID" << endl;
				}
				else
				{
				    useTimeslotID = tmpTimeslot->getTemplateId();
				}

				if((*it)->ieNestedMLMELength > 1)
				{
				    timeslotTable->editTimeslotTemplate(tmpTimeslot);
				}
			    }
			    else if((*it)->ieMLMESubID == IE_MLME_SHORT_HOPPING_TIMING)
			    {

			    }
			    else if((*it)->ieMLMESubID == IE_MLME_SHORT_EB_FILTER)
			    {

			    }
			    else if((*it)->ieMLMESubID == IE_MLME_SHORT_MAC_METRICS_1)
			    {

			    }
			    else if((*it)->ieMLMESubID == IE_MLME_SHORT_MAC_METRICS_2)
			    {

			    }
			}
			else    // long
			{
			    if((*it)->ieMLMESubID == IE_MLME_LONG_CHANNEL_HOPPING_SEQUENCE)
			    {
				//fixed
				IE_CHANNEL_HOPPING* tmpChannel = (*it)->ieChannelHop;
				macHoppingSequenceListEntry *tmpHopE = new macHoppingSequenceListEntry;
				tmpHopE->setChannelPage(tmpChannel->channelPage);
				tmpHopE->setCurrentHop(tmpChannel->currentHop);
				tmpHopE->setExtendedBitmap(tmpChannel->extBitmap);
				tmpHopE->setHopDwellTime(0);
				tmpHopE->setHoppingSequenceId(tmpChannel->hoppingSeqID);
				tmpHopE->setHoppingSequenceLength(tmpChannel->hoppingSeqLength);
				tmpHopE->setHoppingSequenceList(tmpChannel->hoppingSeqLst);
				tmpHopE->setNumberOfChannels(tmpChannel->numberOfChannels);
				tmpHopE->setPhyConfiguration(tmpChannel->phyConfig);

				if(hoppingSequenceList->getEntryById(tmpHopE->getHoppingSequenceId()) == NULL)
				    hoppingSequenceList->addEntry(tmpHopE);
				else
				    hoppingSequenceList->editHoppingSequenceListEntry(tmpHopE);
				useHoppingSequenceID = tmpHopE->getHoppingSequenceId();
			    }
			}
		    }
		}
		else if(tmpPayload.groupID == IE_PL_LIST_TERMINATION)
		{
		    break;
		}
		else
		{
		    // TODO:[SR] unmanaged or reserved
		}
	    }
	}
	return isNACK;
    }
    return false;
}

//-----------------------------------------------------------------------------------------------------------/
/****************************** <TSCH Functions> *****************************/
//----------------------------------------------------------------------------------------------------------/
/**@author: 2014    Stefan Reis
 * started the timeslots timer - for TSCH Std 802.15.4e-2012 */
void Ieee802154eMac::startAsnTimer(simtime_t w_time)
{
    if(mpib.macTSCHcapable)
    {
	if(txData == NULL)
	    BE = mpib.macMinBE; // for shared links (TSCH CSMA-CA)

	if(asnTimer->isScheduled())
	    cancelEvent(asnTimer);

	simtime_t tmpTime = w_time + timeslotTable->getTemplate(useTimeslotID)->getTimeslotLengthDbl();
	EV << "[TSCH] ASN-StartTimer=" << tmpTime << "s" << endl;

	scheduleAt(tmpTime, asnTimer);
    }
}

/**@author: 2014    Stefan Reis
 * handle the time for the timeslots - for TSCH Std 802.15.4e-2012 */
void Ieee802154eMac::handleAsnTimer()
{ //[SR] some dummy values and a fixed timeslot duration is implemented
    if(mpib.macTSCHenabled && !taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) && !taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA))
    {
	startTimeslot = simTime(); // start time of this timeslot

	// set the timer for the next timeslot, depending at the ASN
	if(asnTimer->isScheduled())
	    cancelEvent(asnTimer);

	// get the active LinkEntry
	int tmpASN = mpib.macASN;
	activeLinkEntry = getLinkEntry(mpib.macASN);
	curASN = mpib.macASN;

	// wake up to the next active timeslot
	double nextTime = getNextActiveTimeslot();
	scheduleAt(simTime() + nextTime, asnTimer);

	EV << "[TSCH CCA]-Time: ASN=" << tmpASN << "; timeCorrection=" << timeCorrection << "s; next ASN count=" << (int)mpib.macASN << " next active time=" << simTime() + nextTime << "s" << endl;

	if(activeLinkEntry == NULL)
	    return;

	EV << activeLinkEntry->info() << endl;

	//check if beacon is ready
	if(activeLinkEntry->getLinkType() == LNK_TP_ADVERTISING && txBeaconNow)
	{
	    constructBCN();
	    txBeaconNow = false;
	}

	// request the frame for this dest address
	cMessage *tmpMsg = NULL;
	switch(activeLinkEntry->getLinkType())
	{
	    case LNK_TP_NORMAL:
		tmpMsg = queueModule->requestSpcPacket((IE3ADDR)activeLinkEntry->getNodeAddress());
		break;
	    case LNK_TP_ADVERTISING:
		tmpMsg = queueModule->requestBeaconPacket();
		break;
	    case LNK_TP_JOIN:
		tmpMsg = queueModule->requestAdvPacket();
		if(tmpMsg == NULL)
		{
		    tmpMsg = queueModule->requestSchdulePacket();
		}
		if(tmpMsg == NULL)
		    tmpMsg = NULL;
		break;
	    default:
		tmpMsg = NULL;
		break;

	}

//	if(activeLinkEntry->getLinkType() == LNK_TP_NORMAL)
//	    tmpMsg = queueModule->requestSpcPacket((IE3ADDR)activeLinkEntry->getNodeAddress());
//
//	if(activeLinkEntry)
//
//	if(tmpMsg == NULL && activeLinkEntry->getLinkType() == LNK_TP_ADVERTISING)
//	{
//	    //check if packet is available for advertisment
//	    tmpMsg = queueModule->requestAdvPacket();
//	}
//	if(tmpMsg == NULL && activeLinkEntry->getLinkType() == LNK_TP_ADVERTISING)
//	{
//	    tmpMsg = queueModule->requestSchdulePacket();
//	}

	if(tmpMsg != NULL)
	{
	    txData = check_and_cast<Ieee802154eFrame *>(tmpMsg);
//	    if(txData->getFrmCtrl().frameType == Ieee802154e_BEACON)
//	    {
//		//check if asn is still up to date
//		checkBeaconASN(txData);
//
//	    }

	}

	else
	    txData = NULL;

	MACTSCHLinkOptions linkOption = activeLinkEntry->getLinkOption();

	// if we have nothing to send in a shared_receive timeslot, set the LinkOption to receive
	if(linkOption == LNK_OP_SHARED_RECEIVE && txData == NULL)
	    linkOption = LNK_OP_RECEIVE; // set the LinkOption to receive, if no frame is to send
	else if(linkOption == LNK_OP_SHARED_RECEIVE && txData != NULL)
	    linkOption = LNK_OP_SHARED; // set the LinkOption to shared, if one frame is to send

	/** check the linkOption of the link for this timeslot
	 *  see Std 802.15.4e-2012 (Figure 22b - Timeslot diagram of acknowledged transmission) page 28 */
	if(linkOption == LNK_OP_TRANSMIT || linkOption == LNK_OP_SHARED) // TSCH CCA algorithm OR TSCH CSMA-CA algorithm for shared links
	{
	    tschSharedLink = (linkOption == LNK_OP_SHARED) ? true:false; // for shared links

	    if(txData != NULL)
	    {
		int retries = txData->getRetries();
		activeNeighborEntry = NULL;
		UINT_16 destAddr = getShortAddress(txData->getDstAddr());
		if(destAddr != 0xffff) // if is not the broadcast address
		{
		    activeNeighborEntry = neighborTable->getNeighborBySAddr(destAddr);

		    if(activeNeighborEntry == NULL)
			return; // Cancel the transmission if we have no neighbor for this data pkt
		}

		string tmpTxt = (txData->getFrmCtrl().frameType == Ieee802154e_DATA) ? txData->getEncapsulatedPacket()->getName():frameTypeTxt[txData->getFrmCtrl().frameType];
		EV << "[TSCH CCA]-Transmitter: " << retries << " retries for this " << txData->getName() << "(" << tmpTxt << ") frame" << endl;

		if(tschSharedLink && backoffMethod == EXPONENTIAL && retries <= mpib.macMaxFrameRetries && retries > 0 && activeNeighborEntry != NULL)
		{ // in TSCH mode the device shall use an exponential backoff mechanism - Std 802.15.4e-2012 (5.1.1.4.3 TSCH CSMA-CA algorithm) page 14
		    int td = activeNeighborEntry->getTransDelay(); // transmission delayed

		    if(td < 0)
		    { // calculation from the cmsa802154.cc file
			double d = pow((double)2, (int)BE);
			int v = (int)d - 1;
			int r = intuniform(0, v, 0);
			td = r;
			activeNeighborEntry->setTransDelay(r);
			EV << "[TSCH CCA]-Transmitter (shared links): retransmission delayed for " << r << " for shared links" << endl;
		    }

		    // check if we need to delay the transmission
		    if(td > 0)
		    {
			EV << "[TSCH CCA]-Transmitter (shared links): transmission delay for " << td << " shared links" << endl;
			activeNeighborEntry->decrTransDelay(); // reduce the transmission delay for the next shared link
//			resetTRX();
//			return;
			//We have to wait for transmit the packet, so turn on the receiver to listen;
			taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) = true;
			taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) = 1;
			// start the macTsRxOffset timer
			if(tsRxOffsetTimer->isScheduled())
			    cancelEvent(tsRxOffsetTimer);

			EV << "[TSCH CCA]-Receiver:[1] wait the TsRxOffset" << endl;
			scheduleAt(simTime() + timeslotTable->getTemplate(useTimeslotID)->getRxOffsetDbl(), tsRxOffsetTimer);
			return;
		    }
		    else
			// for the case td == 0, to have the td == -1 for the recalculation of the transmission delay
			activeNeighborEntry->decrTransDelay();
		}

		taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) = true;
		taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 1;

		if(useCCA)
		{
		    taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 1;

		    // start the macTsCCAOffset timer
		    if(tsCCAOffsetTimer->isScheduled())
			cancelEvent(tsCCAOffsetTimer);

		    EV << "[TSCH CCA]-Transmitter:[1] use CCA, wait the TsCCAOffsetTime" << endl;
		    scheduleAt(simTime() + timeslotTable->getTemplate(useTimeslotID)->getCCAOffsetDbl(), tsCCAOffsetTimer);
		    // next step in handleTsCCAOffset()
		}
		else
		{
		    taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 100;

		    // start the macTsCCAOffset timer
		    if(tsTxOffsetTimer->isScheduled())
			cancelEvent(tsTxOffsetTimer);

		    EV << "[TSCH CCA]-Transmitter:[100] no CCA, wait the TsTxOffsetTime" << endl;
		    scheduleAt(simTime() + timeslotTable->getTemplate(useTimeslotID)->getTxOffsetDbl(), tsTxOffsetTimer);
		    // next step in handleTsTxOffset()
		}
	    }
	    else
	    {
		EV << "[TSCH CCA]-Transmitter: no pkt to send, go back to sleep" << endl;
		taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) = false;
		taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 0;
		resetTRX();
	    }
	}
	else if(linkOption == LNK_OP_RECEIVE || linkOption == LNK_OP_RECEIVE_TIMEKEEPING) // Receiving Link
	{
	    taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) = true;
	    taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) = 1;
	    // start the macTsRxOffset timer
	    if(tsRxOffsetTimer->isScheduled())
		cancelEvent(tsRxOffsetTimer);

	    EV << "[TSCH CCA]-Receiver:[1] wait the TsRxOffset" << endl;
	    scheduleAt(simTime() + timeslotTable->getTemplate(useTimeslotID)->getRxOffsetDbl(), tsRxOffsetTimer);
	    // next step in handleTsRxOffset()
	}
	else    // Reserved
	{
	    EV << "[TSCH CCA]-Reserved: nothing to do, go back to sleep" << endl;
	    resetTRX();
	}
    }
}
void Ieee802154eMac::checkBeaconASN(Ieee802154eFrame *msg)
{
    if(msg->getFrmCtrl().presIElist)
    {
	for(unsigned int i = 0; i < msg->getIePayloadArraySize(); i++)
	{
	    IE_Payload tmpPayload = msg->getIePayload(i);
	    if(!tmpPayload.typeDescriptor)
		break;

	    if(tmpPayload.groupID == IE_PL_ESDU)
	    {
		//XXX:[SR] ESDU not implemented
		//IE_ESDU* tmpESDU = tmpPayload.ieESDU;
	    }
	    else if(tmpPayload.groupID == IE_PL_MLME)
	    {
		IE_MLME* tmpMLME = tmpPayload.ieMLME;
		std::list<IE_MLME_Sub*>::iterator it;

		for(it = tmpMLME->ieMLMESub.begin(); it != tmpMLME->ieMLMESub.end(); ++it)
		{
		    if((*it)->ieMLMEtype == false) //short
		    {
			if((*it)->ieMLMESubID == IE_MLME_SHORT_TSCH_SYNCHRONISATION)
			{
			    IE_TSCH_SYNC* tmpSync = (*it)->ieTSCHSync;
			    if(tmpSync->tschASN != mpib.macASN)
			    {
				tmpSync->tschASN = curASN;
				return;
			    }

			}
		    }
		}
	    }
	}
    }
}
/**@author: 2014    Stefan Reis
 * Transmission CCA Offset */
void Ieee802154eMac::handleTsCCAOffset()
{
    if(taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 1)
    {
	taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 2;

	PHY_PIB tmpPIB;
	std::vector<UINT_16> tmpHoppingList = hoppingSequenceList->getEntryById(useHoppingSequenceID)->getHoppingSequenceList();
	int nextChannel = tmpHoppingList[(curASN + activeLinkEntry->getChannelOffset()) % tmpHoppingList.size()];
	tmpPIB.phyCurrentChannel = nextChannel;
	EV << "[TSCH CCA]-Transmitter:[2] before sending CCA primitive, tell the PHY layer to change the current Channel (" << nextChannel << ")" << endl;

	PLME_SET_request(PHY_CURRENT_CHANNEL, tmpPIB);
	// next step in handle_PLME_SET_confirm()
    }
}

/**@author: 2014    Stefan Reis
 * Transmission no CCA - TsTXOffsetTimer */
void Ieee802154eMac::handleTsTxOffset()
{
    if(taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 100)
    {
	taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 101;
	EV << "[TSCH CCA]-Transmitter:[101](without CCA) tell the PHY layer to change the current Channel" << endl;
	PHY_PIB tmpPIB;
	std::vector<UINT_16> tmpHoppingList = hoppingSequenceList->getEntryById(useHoppingSequenceID)->getHoppingSequenceList();
	tmpPIB.phyCurrentChannel = tmpHoppingList[(curASN + activeLinkEntry->getChannelOffset()) % tmpHoppingList.size()];
	PLME_SET_request(PHY_CURRENT_CHANNEL, tmpPIB);
	// next step in handle_PLME_SET_confirm()
    }
}

/**@author: 2014    Stefan Reis
 * Transmission delay before wait for ack */
void Ieee802154eMac::handleTsRxAckDelay()
{
    if(taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 7)
    {
	taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 8;
	EV << "[TSCH CCA]-Transmitter:[8] set the TRX_STATE to RX_ON and wait for the Ack" << endl;
	PLME_SET_TRX_STATE_request(phy_RX_ON);
	// next step in PLME_SET_TRX_STATE_confirm()
    }
}

/**@author: 2014    Stefan Reis
 * Transmission delay before wait for ack */
void Ieee802154eMac::handleTsAckWait()
{
    if(taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) && (taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 5 || taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 9 || taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 11 || taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 99))
    {
	ASSERT(txData);

	taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) = false;
	taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 0;

	// see Std 802.15.4e-2012 (5.1.1.4.3 TSCH CSMA-CA algorithm) page 14 or (figure 11b11b�TSCH CSMA-CA backoff) page 16
	int retries = txData->getRetries();

	// increment retry counter
	txData->setRetries(++retries);

	/** for shared links
	 *   calculate the backoff after a first transmission failure */
	if(tschSharedLink && retries > 1)
	{

	    BE++;
	    BE = (BE <= mpib.macMaxBE) ? BE:mpib.macMaxBE;
	}

	if(retries > mpib.macMaxFrameRetries) // failed retransmission; max retransmission for this packet
	{
	    EV << "[TSCH CCA]-Transmitter:[12] no Ack for this transmission arrived in the TsAckWaitTime, maximum retries arrived, drop this frame" << endl;

	    // record for the statistics (with slotframe size 100 and sim end time=10000, we can use the ASN number direct)
	    int timeslot = FWMath::modulo(curASN, slotframeTable->getSlotframe(activeLinkEntry->getSlotframeId())->getSlotframeSize());
	    emit(sucTimeslot, -timeslot);

	    // to many retries, send MCPS-DATA.confirm with NO_ACK and delete the pkt from the queue
	    MCPS_DATA_confirm(txData->getSeqNmbr(), 0, false, 0, 0, 0, 0, 0, mac_NO_ACK, txData->getRetries(), 0, NULL);
	    queueModule->deleteMsgQueue(txData->getDstAddr(), false);

	    if(txData->getFrmCtrl().frameType == Ieee802154e_SCHEDULER_REQUEST)
	    {

		SCHEDULE_confirm(txData->dup(), false);
	    }

//	    if(txData->getFrmCtrl().frameType == Ieee802154e_SCHEDULER_RESPONCE)
//	    {
//		macLinkTableEntry *entry = new macLinkTableEntry();
//		Ieee802154eNetworkCtrlInfo *cntrl = check_and_cast<Ieee802154eNetworkCtrlInfo *>(&check_and_cast<Ieee802154eScheduleFrame *>(txData)->getCntrlInfo());
//		//fixme:
//		entry = linkTable->getLinkByTimeslotOffsetAddress(cntrl->getTimeslot(), cntrl->getChannelOffset(), cntrl->getDeviceAddress());
//		if(entry != NULL)
//		    linkTable->deleteLink(entry);
//	    }
	    delete txData;
	    txData = NULL;

	    // count the failed retransmission
	    if(mpib.macMetricsEnabled)
		mpib.macTXFailCount++;
	}
	else
	{ // failed retransmission: retry the next time
	    if(taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 5)
		EV << "[TSCH CCA]-Transmitter:[12] data transmission failed, go back to sleep, try the next timeslot to the dest address" << endl;
	    else if(taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 99)
		EV << "[TSCH CCA]-Transmitter:[12] CCA failed, go back to sleep, try the next timeslot to the dest address" << endl;
	    else
		EV << "[TSCH CCA]-Transmitter:[12] no Ack for this transmission arrived in the TsAckWaitTime, try the next timeslot to the dest address" << endl;
	}

	// delete the pkt
	if(txPkt == txBeacon)
	{
	    beaconWaitingTx = false;
	    delete txBeacon;
	    txBeacon = NULL;
	}
	else if(txPkt == txAck)
	{
	    delete txAck;
	    txAck = NULL;
	}
	else if(txPkt == txData)
	{
	    txData = NULL;
	}

	resetTRX();
	// finish
    }
}

/**@author: 2014    Stefan Reis
 * Receiving CCA Offset */
void Ieee802154eMac::handleTsRxOffset()
{
    if(taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) == 1)
    {
	taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) = 2;

	EV << "[TSCH CCA]-Receiver:[2] tell the PHY layer to change the current Channel" << endl;

	PHY_PIB tmpPIB;
	std::vector<UINT_16> tmpHoppingList = hoppingSequenceList->getEntryById(useHoppingSequenceID)->getHoppingSequenceList();
	tmpPIB.phyCurrentChannel = tmpHoppingList[(curASN + activeLinkEntry->getChannelOffset()) % tmpHoppingList.size()];
	PLME_SET_request(PHY_CURRENT_CHANNEL, tmpPIB);
	// next step in handle_PLME_SET_confirm()
    }
}

/**@author: 2014    Stefan Reis
 * Receiving wait for transmission */
void Ieee802154eMac::handleTsRxWait()
{
    if(taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) && (taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) == 4 || taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) == 6))
    {
	taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) = false;
	taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) = 0;

	if(mpib.macMetricsEnabled)
	{
	    if(taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) == 5)
		mpib.macFCSErrorCount++;
	}

	EV << "[TSCH CCA]-Receiver:[6] no correct frame arrived in the TsRxWaitTime" << endl;
	resetTRX();
    }
}

/**@author: 2014    Stefan Reis
 * Receiving wait for transmission of the ack */
void Ieee802154eMac::handleTsTxAckDelay()
{
    if(taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) == 7)
    {
	taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) = 8;
	EV << "[TSCH CCA]-Receiver:[8] set the TRX_STATE to TX_ON" << endl;
	PLME_SET_TRX_STATE_request(phy_TX_ON);
	// next step in PLME_SET_TRX_STATE_confirm()
    }
}

void Ieee802154eMac::MPIBcopyStandard()
{
    mpib.macAMCACapable = MPIB.macAMCACapable;
    mpib.macAMCAenabled = MPIB.macAMCAenabled;
    mpib.macASN = MPIB.macASN;
    mpib.macAckWaitDuration = MPIB.macAckWaitDuration;
    mpib.macAssociatedPANCoord = MPIB.macAssociatedPANCoord;
    mpib.macAssociationPermit = MPIB.macAssociationPermit;
    mpib.macAutoRequest = MPIB.macAutoRequest;
    mpib.macAutoRequestKeyIdMode = MPIB.macAutoRequestKeyIdMode;
    mpib.macAutoRequestKeyIndex = MPIB.macAutoRequestKeyIndex;
    mpib.macAutoRequestSecurityLevel = MPIB.macAutoRequestSecurityLevel;
    mpib.macBSN = MPIB.macBSN;
    mpib.macBattLifeExt = MPIB.macBattLifeExt;
    mpib.macBattLifeExtPeriods = MPIB.macBattLifeExtPeriods;
    mpib.macBeaconOrder = MPIB.macBeaconOrder;
    for(int i = 0; i < 257; i++)
	mpib.macBeaconPayload[i] = MPIB.macBeaconPayload[i];
    mpib.macBeaconPayloadLength = MPIB.macBeaconPayloadLength;
    mpib.macBeaconTxTime = MPIB.macBeaconTxTime;
    mpib.macCoordExtendedAddress = MPIB.macCoordExtendedAddress;
    mpib.macCoordShortAddress = MPIB.macCoordShortAddress;
    mpib.macCounterOctets = MPIB.macCounterOctets;
    mpib.macDSMEcapable = MPIB.macDSMEcapable;
    mpib.macDSMEenabled = MPIB.macDSMEenabled;
    mpib.macDSN = MPIB.macDSN;
    mpib.macDisconnectTime = MPIB.macDisconnectTime;
    mpib.macDuplicateFrameCount = MPIB.macDuplicateFrameCount;
    mpib.macEBAutoSA = MPIB.macEBAutoSA;
    mpib.macEBFilteringEnabled = MPIB.macEBFilteringEnabled;
    mpib.macEBSN = MPIB.macEBSN;
    mpib.macEnhAckWaitDuration = MPIB.macEnhAckWaitDuration;
    mpib.macExtendedAddress = MPIB.macExtendedAddress;
    mpib.macFCSErrorCount = MPIB.macFCSErrorCount;
    mpib.macFrameCounter = MPIB.macFrameCounter;
    mpib.macFrameCounterMode = MPIB.macFrameCounterMode;
    mpib.macGTSPermit = MPIB.macGTSPermit;
    mpib.macHoppingCapable = MPIB.macHoppingCapable;
    mpib.macHoppingEnabled = MPIB.macHoppingEnabled;
    mpib.macImplicitBroadcast = MPIB.macImplicitBroadcast;
    mpib.macJoinPriority = MPIB.macJoinPriority;
    mpib.macLEcapable = MPIB.macLEcapable;
    mpib.macLEenabled = MPIB.macLEenabled;
    mpib.macLIFSPeriod = MPIB.macLIFSPeriod;
    mpib.macLLcapable = MPIB.macLLcapable;
    mpib.macLLenabled = MPIB.macLLenabled;
    mpib.macMaxBE = MPIB.macMaxBE;
    mpib.macMaxCSMABackoffs = MPIB.macMaxCSMABackoffs;
    mpib.macMaxFrameRetries = MPIB.macMaxFrameRetries;
    mpib.macMetricsCapable = MPIB.macMetricsCapable;
    mpib.macMetricsEnabled = MPIB.macMetricsEnabled;
    mpib.macMinBE = MPIB.macMinBE;
    mpib.macMultipleRetryCount = MPIB.macMultipleRetryCount;
    mpib.macNACKcount = MPIB.macNACKcount;
    mpib.macNoHLBuffers = MPIB.macNoHLBuffers;
    mpib.macPANCoordExtendedAddress = MPIB.macPANCoordExtendedAddress;
    mpib.macPANCoordShortAddress = MPIB.macPANCoordShortAddress;
    mpib.macPANId = MPIB.macPANId;
    mpib.macPromiscuousMode = MPIB.macPromiscuousMode;
    mpib.macRFIDcapable = MPIB.macRFIDcapable;
    mpib.macRFIDenabled = MPIB.macRFIDenabled;
    mpib.macRXSuccessCount = MPIB.macRXSuccessCount;
    mpib.macResponseWaitTime = MPIB.macResponseWaitTime;
    mpib.macRetryCount = MPIB.macRetryCount;
    mpib.macRxOnWhenIdle = MPIB.macRxOnWhenIdle;
    mpib.macSIFSPeriod = MPIB.macSIFSPeriod;
    mpib.macSecurityEnabled = MPIB.macSecurityEnabled;
    mpib.macSecurityFailure = MPIB.macSecurityFailure;
    mpib.macShortAddress = MPIB.macShortAddress;
    mpib.macSimpleAddress = MPIB.macSimpleAddress;
    mpib.macSuperframeOrder = MPIB.macSuperframeOrder;
    mpib.macSyncSymbolOffset = MPIB.macSyncSymbolOffset;
    mpib.macTSCHcapable = MPIB.macTSCHcapable;
    mpib.macTSCHenabled = MPIB.macTSCHenabled;
    mpib.macTXFailCount = MPIB.macTXFailCount;
    mpib.macTXSuccessCount = MPIB.macTXSuccessCount;
    mpib.macTimestampSupported = MPIB.macTimestampSupported;
    mpib.macTransactionPersistenceTime = MPIB.macTransactionPersistenceTime;
    mpib.macTxControlActiveDuration = MPIB.macTxControlActiveDuration;
    mpib.macTxControlPauseDuration = MPIB.macTxControlPauseDuration;
    mpib.macTxTotalDuration = MPIB.macTxTotalDuration;
    mpib.macUseEnhancedBeacon = MPIB.macUseEnhancedBeacon;
}
void Ieee802154eMac::handle_SCHEDULE_request(cMessage *msg)
{
    Ieee802154eScheduleFrame *tmpSchedul = new Ieee802154eScheduleFrame("SchedulerRequest", TP_SCHEDULE_REQUEST);
    Ieee802154eNetworkCtrlInfo *tmpNetCn = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    FrameCtrl frmCtrl;
    frmCtrl.frameType = Ieee802154e_SCHEDULER_REQUEST;
    frmCtrl.securityEnabled = secuData;
    frmCtrl.frmPending = (txData == NULL) ? false:true;
    frmCtrl.ackReq = true;
    frmCtrl.seqNbSup = false;
    frmCtrl.dstAddrMode = tmpNetCn->getDeviceAddrMode();
    frmCtrl.frameVersion = Ieee802154_compatible;
    if(mpib.macEBAutoSA == 0)  // NONE Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrModeNone;
    else if(mpib.macEBAutoSA == 1)  // SHORT Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrMode16;
    else if(mpib.macEBAutoSA == 2)  // FULL Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrMode64;

    IE3ADDR tmpDstAddr;
    IE3ADDR tmpSrcAddr;
    UINT_16 tmpDstPanId;
    UINT_16 tmpSrcPanId;

    if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode16)
    { // 16 bit address
	tmpDstPanId = mpib.macPANId;

	tmpDstAddr = (IE3ADDR)getShortAddress(MACAddress(tmpNetCn->getDeviceAddress()));
    }
    else if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode64)
    { // 64 bit address
	tmpDstPanId = mpib.macPANId;
	tmpDstAddr = (IE3ADDR)tmpNetCn->getDeviceAddress();
    }

    if(frmCtrl.srcAddrMode == defFrmCtrl_AddrMode16)
    { // 16 bit address
	tmpSrcPanId = mpib.macPANId;
	tmpSrcAddr = (IE3ADDR)mpib.macShortAddress;
    }
    else
    { // 64 bit address
	tmpSrcPanId = mpib.macPANId;
	tmpSrcAddr = (IE3ADDR)mpib.macExtendedAddress;
    }

    tmpSchedul->setDstPanId(tmpDstPanId);
    tmpSchedul->setDstAddr(tmpDstAddr);
    tmpSchedul->setSrcPanId(tmpSrcPanId);
    tmpSchedul->setSrcAddr(tmpSrcAddr);
    frmCtrl.compPanID = getPANIDComp(frmCtrl, tmpSchedul->getSrcPanId(), tmpSchedul->getDstPanId());

    AuxiliarySecurityHeader auxSecHd;
    auxSecHd.secLvl = ASH_SLVL_NONE;
    auxSecHd.keyIdentMode = 0;
    auxSecHd.keySource = 0;
    auxSecHd.keyIndex = 0;

    tmpSchedul->setCntrlInfo(*tmpNetCn->dup());
    tmpSchedul->setFrmCtrl(frmCtrl);
    tmpSchedul->setAuxSecHd(auxSecHd);
    tmpSchedul->setSeqNmbr(mpib.macDSN++);

    if(!queueModule->existSchedReq(tmpDstAddr))
	queueModule->insertInQueue(tmpSchedul->dup());


    delete tmpNetCn;
    delete tmpSchedul;

}

void Ieee802154eMac::SCHEDULE_indication(cMessage *msg)
{
    Ieee802154eScheduleFrame *scheduler = check_and_cast<Ieee802154eScheduleFrame *>(msg);
    Ieee802154eNetworkCtrlInfo *Ctrl = new Ieee802154eNetworkCtrlInfo("SchedulerRequest", TP_SCHEDULE_INDICATION);
    Ctrl->setTimeslot(scheduler->getCntrlInfo().getTimeslot());
    Ctrl->setChannelOffset(scheduler->getCntrlInfo().getChannelOffset());

    if(scheduler->getCntrlInfo().getDeviceAddrMode() == defFrmCtrl_AddrMode16)
	Ctrl->setDeviceAddress(getShortAddress(scheduler->getSrcAddr()));
    else if(scheduler->getCntrlInfo().getDeviceAddrMode() == defFrmCtrl_AddrMode64)
	Ctrl->setDeviceAddress(getShortAddress(scheduler->getSrcAddr()));

    if(!queueModule->existSchedRes(scheduler->getSrcAddr()))
	send(Ctrl->dup(), mSchedulerOut);

    delete Ctrl;
    delete scheduler;

}

void Ieee802154eMac::handle_SCHEDULE_response(cMessage *msg)
{
    Ieee802154eScheduleFrame *tmpSchedul = new Ieee802154eScheduleFrame("SchedulerResponse", TP_SCHEDULE_RESPONSE);
    Ieee802154eNetworkCtrlInfo *tmpNetCn = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    FrameCtrl frmCtrl;
    frmCtrl.frameType = Ieee802154e_SCHEDULER_RESPONCE;
    frmCtrl.securityEnabled = secuData;
    frmCtrl.frmPending = (txData == NULL) ? false:true;
    frmCtrl.ackReq = true;
    frmCtrl.seqNbSup = false;
    frmCtrl.dstAddrMode = defFrmCtrl_AddrMode64;
    frmCtrl.frameVersion = Ieee802154_compatible;
    if(mpib.macEBAutoSA == 0)  // NONE Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrModeNone;
    else if(mpib.macEBAutoSA == 1)  // SHORT Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrMode16;
    else if(mpib.macEBAutoSA == 2)  // FULL Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrMode64;

    IE3ADDR tmpDstAddr;
    IE3ADDR tmpSrcAddr;
    UINT_16 tmpDstPanId;
    UINT_16 tmpSrcPanId;

    if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode16)
    { // 16 bit address
	tmpDstPanId = mpib.macPANId;
	tmpDstAddr = (IE3ADDR)getShortAddress(MACAddress(tmpNetCn->getDeviceAddress()));
    }
    else if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode64)
    { // 64 bit address
	tmpDstPanId = mpib.macPANId;
	tmpDstAddr = (IE3ADDR)tmpNetCn->getDeviceAddress();
    }

    if(frmCtrl.srcAddrMode == defFrmCtrl_AddrMode16)
    { // 16 bit address
	tmpSrcPanId = mpib.macPANId;
	tmpSrcAddr = (IE3ADDR)mpib.macShortAddress;
    }
    else
    { // 64 bit address
	tmpSrcPanId = mpib.macPANId;
	tmpSrcAddr = (IE3ADDR)mpib.macExtendedAddress;
    }

    tmpSchedul->setDstPanId(tmpDstPanId);
    tmpSchedul->setDstAddr(tmpDstAddr);
    tmpSchedul->setSrcPanId(tmpSrcPanId);
    tmpSchedul->setSrcAddr(tmpSrcAddr);
    frmCtrl.compPanID = getPANIDComp(frmCtrl, tmpSchedul->getSrcPanId(), tmpSchedul->getDstPanId());

    AuxiliarySecurityHeader auxSecHd;
    auxSecHd.secLvl = ASH_SLVL_NONE;
    auxSecHd.keyIdentMode = 0;
    auxSecHd.keySource = 0;
    auxSecHd.keyIndex = 0;

    tmpSchedul->setCntrlInfo(*tmpNetCn->dup());
    tmpSchedul->setFrmCtrl(frmCtrl);
    tmpSchedul->setAuxSecHd(auxSecHd);
    tmpSchedul->setSeqNmbr(mpib.macDSN++);
    queueModule->insertInQueue(tmpSchedul->dup());

    delete tmpSchedul;
    delete tmpNetCn;
}

void Ieee802154eMac::SCHEDULE_confirm(cMessage *msg, bool ack)
{

    Ieee802154eScheduleFrame *scheduler = check_and_cast<Ieee802154eScheduleFrame *>(msg);
    Ieee802154eNetworkCtrlInfo *Ctrl = check_and_cast<Ieee802154eNetworkCtrlInfo *>(&scheduler->getCntrlInfo());
    if(scheduler->getFrmCtrl().frameType == Ieee802154e_SCHEDULER_RESPONCE)
    {

	Ctrl->setName("SchedulerRequest");
	Ctrl->setKind(TP_SCHEDULE_CONFIRM);
	Ctrl->setReceivedByACK(ack);
	send(Ctrl->dup(), mSchedulerOut);

    }

    if(scheduler->getFrmCtrl().frameType == Ieee802154e_SCHEDULER_REQUEST)
    {
	Ctrl->setStatus(mac_DENIED);
	Ctrl->setName("SchedulerRequest");
	Ctrl->setKind(TP_SCHEDULE_CONFIRM);
	Ctrl->setReceivedByACK(ack);
	send(Ctrl->dup(), mSchedulerOut);

    }

    //delete Ctrl;
    delete scheduler;

}
//    if(isPANCoor)
//    {
//	Ieee802154eScheduleFrame *scheduler = check_and_cast<Ieee802154eScheduleFrame *>(msg);
//	Ieee802154eNetworkCtrlInfo *Ctrl = new Ieee802154eNetworkCtrlInfo("SchedulerRequest", TP_SCHEDULE_CONFIRM);
//	Ctrl->setTimeslot(scheduler->getCntrlInfo().getTimeslot());
//	Ctrl->setChannelOffset(scheduler->getCntrlInfo().getChannelOffset());
//	Ctrl->setDeviceAddress(getShortAddress(scheduler->getDstAddr()));
//	send(Ctrl->dup(), mSchedulerOut);
//
//	delete Ctrl;
//	delete scheduler;
//    }
//
//    else
//    {
//	Ieee802154eScheduleFrame *scheduler = check_and_cast<Ieee802154eScheduleFrame *>(msg);
//	Ieee802154eNetworkCtrlInfo *Ctrl = new Ieee802154eNetworkCtrlInfo("SchedulerRequest", TP_SCHEDULE_CONFIRM);
//	Ctrl->setTimeslot(scheduler->getCntrlInfo().getTimeslot());
//	Ctrl->setChannelOffset(scheduler->getCntrlInfo().getChannelOffset());
//	Ctrl->setDeviceAddress(getShortAddress(scheduler->getSrcAddr()));
//	send(Ctrl->dup(), mSchedulerOut);
//
//	delete Ctrl;
//	delete scheduler;
//
//    }
//}
void Ieee802154eMac::handleSchedulerMsg(cMessage *msg)
{
    switch(msg->getKind())
    {
	case TP_MLME_START_REQUEST:
	{
	    handle_MLME_START_request(msg->dup());
	    delete msg;
	    break;
	}
	case TP_MLME_BEACON_REQUEST:
	{
	    handle_MLME_BEACON_request(msg->dup());
	    delete msg;
	    break;
	}
	case TP_MLME_SCAN_REQUEST:
	{
	    handle_MLME_SCAN_request(msg->dup());
	    delete msg;
	    break;
	}
	case TP_MLME_SET_BEACON_REQUEST:
	{
	    handleEB(msg->dup());
	    delete msg;
	    break;
	}
	case TP_MLME_ASSOCIATE_REQUEST:
	{
	    handle_MLME_ASSOCIATE_request(msg->dup());
	    delete msg;
	    break;
	}
	case TP_MLME_ASSOCIATE_RESPONSE:
	{
	    handle_MLME_ASSOCIATE_responce(msg->dup());
	    delete msg;
	    break;
	}
	case TP_SCHEDULE_REQUEST:
	{
	    handle_SCHEDULE_request(msg->dup());
	    delete msg;
	    break;
	}
	case TP_SCHEDULE_RESPONSE:
	{
	    handle_SCHEDULE_response(msg->dup());
	    delete msg;
	    break;
	}
	default:
	{
	    if(ev.isGUI())
	    {
		EV << "Unknown command, delete frame" << endl;
	    }
	    delete msg;
	}
    }
}

void Ieee802154eMac::updateNeighbor(Ieee802154eFrame *frame, FrameCtrl frmCtrl)
{
    if(!neighborTable->isNeighborBySAddr(getShortAddress(frame->getSrcAddr()))) // if not in neighborTable add it
    {
	macNeighborTableEntry* tmpEntry = new macNeighborTableEntry();
	tmpEntry->setShortAddress(getShortAddress(frame->getSrcAddr()));

	tmpEntry->setExtendedAddress(frame->getSrcAddr());
	tmpEntry->setSDIndex(0);            // not use in TSCH
	tmpEntry->setChannelOffset(0);      // not use in TSCH
	tmpEntry->setTrackBeacon(false);    // not use in TSCH
	tmpEntry->setBeaconLostCount(0);    // not use in TSCH
	tmpEntry->setNumTxData(0);
	tmpEntry->setNumTxDataAck(0);
	tmpEntry->setNumRxData(frmCtrl.ackReq ? 0:1);
	tmpEntry->setNumRxDataAck(frmCtrl.ackReq ? 1:0);
	tmpEntry->setLastASN(mpib.macASN);
	tmpEntry->setRPLrank(0);            // for RPL routing
	/* A network device uses a neighbor as a time source when it has received
	 * links to that neighbor with the Timekeeping bit set in the maclinkOptions.
	 * A higher layer may add or change time source neighbors at any time.*/
	if(frame->getFrmCtrl().frameType == Ieee802154e_BEACON)
	{
	    tmpEntry->setIsTimeSource(true);
	}
	else
	{
	    tmpEntry->setIsTimeSource(linkTable->isTimescource(getShortAddress(frame->getSrcAddr())));
	}
	tmpEntry->setRPL_OF(0);             // for RPL routing
	neighborTable->addNeighbor(tmpEntry);
    }
    else // update the neighbor states
    {
	macNeighborTableEntry* tmpEntry = neighborTable->getNeighborBySAddr(getShortAddress(frame->getSrcAddr()));
	tmpEntry->setExtendedAddress(frame->getSrcAddr());
	frmCtrl.ackReq ? tmpEntry->incrNumRxData():tmpEntry->incrNumRxData();
	tmpEntry->setLastASN(curASN);
	//	if(frame->getFrmCtrl().frameType == Ieee802154e_BEACON)
	//	{
	//	    tmpEntry->setIsTimeSource(true);
	//	}
	//	else
	//	{
	//	    tmpEntry->setIsTimeSource(linkTable->isTimescource(getShortAddress(frame->getSrcAddr())));
	//	}
	//neighborTable->editNeighbor(tmpEntry);
    }
}

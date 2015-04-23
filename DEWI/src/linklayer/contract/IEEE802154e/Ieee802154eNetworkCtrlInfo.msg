//
// IEEE802.15.4 network control info: 
//
cplusplus {{
    using namespace std;
    #include "Ieee802154eField.h"
    #include "Ieee802154eEnum.h"
}}

struct MAC_PIB;

enum MACenum;

message Ieee802154eNetworkCtrlInfo
{
 // MAC layer is responsible for MAC address translation
    bool 	 toParent;
    string   destName;
    int64_t  netwAddr=-1; 
    
    // for MLME_ASSOCIATE.request - Std 802.15.4-2011 (table 9) page 79
    uint8_t  channelNumber;	// Channel Number
    uint8_t	 channelPage;		// Channel Page
    uint8_t  coordAddrMode;	//defined in Enum.h -> Ieee802154eAddrMode
   	uint16_t coordPANId;
   	uint64_t coordAddress;
   	uint8_t  capabilityInformation; // As defined in 5.3.1.2 - Specifies the operational capabilities of the associating device.
   	
   	uint8_t  securityLevel;
   	uint8_t	 keyIdMode;
   	uint64_t keySource;
   	uint8_t	 keyIndex;
   	// Std 802.15.4e-2012 (table 9) page 125
   	uint64_t lowLatencyNetworkInfo;
   	uint16_t channelOffset;
   	uint8_t  hoppingSequenceID;  	

	// for MLME_ASSOCIATE.confirm - Std 802.15.4-2011 (table 12) page 82
	uint16_t AssocShortAddress;
	short    status @enum(MACenum);
   	
   	// for MLME-DISASSOCIATE.request - Std 802.15.4-2011 (table 13) page 84
   	uint8_t  deviceAddrMode;	//defined in Enum.h -> Ieee802154eAddrMode
   	uint16_t devicePANId;
   	uint64_t deviceAddress;
   	uint8_t  disassociateReason; // The reason for the disassociation, as described in 5.3.2.3.
   	bool     txIndirect;
   	
   	// MLME-GET.request - Std 802.15.4-2011 (table 19) page 90
   	uint8_t  pibAttribute;
   	
   	// MLME-GTS.request - Std 802.15.4-2011 (table 21) page 92
   	uint8_t  gtsCharacteristic; // As defined in 5.3.9.2 - The characteristics of the GTS request, including whether the request is for the allocation of a new GTS or the deallocation of an existing GTS
   	
   	// MLME-RESET.request - Std 802.15.4-2011 (table 26) page 96
   	bool 	 setDefaultPIB;
   	
   	// MLME-RX-ENABLE.request - Std 802.15.4-2011 (table 28) page 98
	bool     deferPermit;
	uint32_t rxOnTime;
   	uint32_t rxOnDuration;
   	uint8_t  rangingRxControl; // see Enum.h -> RangingControl
   	
   	// MLME-SCAN.request - Std 802.15.4-2011 (table 30) page 100
   	uint8_t  scanType;	// see Enum.h -> Ieee802154eChannelScanType
   	uint64_t scanChannels;
   	uint8_t	 scanDuration;
   	// Std 802.15.4e-2012 (table 30) page 134
   	bool    linkQualityScan; 
   	bool    frameControlOptions[]; // List of Boolean; page 134
   	uint8_t headerIElist[];
   	uint8_t payloadIElist[];
   	
   	// MLME-SET.request - Std 802.15.4-2011 (table 32) page 102
   	MAC_PIB pibAttributeValue;
   	
   	// MLME-START.request - Std 802.15.4-2011 (table 34) page 104
   	uint16_t panId;
   	
   	uint32_t startTime;
   	uint8_t  beaconOrder;
   	uint8_t  superframeOrder;
   	bool     panCoordinator;
   	bool     batteryLifeExtension;
   	bool     coordRealignment;
   	uint8_t  coordRealignSecurityLevel;
   	uint8_t  coordRealignKeyIdMode;
   	uint64_t coordRealignKeySource;
   	uint8_t  coordRealignKeyIndex;
   	uint8_t  beaconSecurityLevel;
   	uint8_t  beaconKeyIdMode;
   	uint64_t beaconKeySource;
   	uint8_t  beaconKeyIndex;
   	// Std 802.15.4e-2012 (table 34) page 137
   	uint8_t  dsmeSuperframeSpecification;
   	uint64_t beaconBitmap;
   	//HoppingDescriptor - Std 802.15.4e-2012 (table 34a) page 139
   	uint16_t hoppingSequenceLength; 
   	uint8_t  hoppingSequence[]; 

   	uint16_t channelOffsetBitmapLength;
   	uint8_t  channelOffsetBitmap[];
   	
   	// MLME-SYNC.request - Std 802.15.4.-2011 (table 36) page 108
   	bool trackBeacon;
   	
   	// MLME-POLL.request - Std 802.15.4-2011 page 110
   	
   	// MLME-DPS.request - Std 802.15.4-2011 (table 40) page 112
   	uint8_t  txDPSIndex;
   	uint8_t  rxDPSIndex;
   	uint32_t dpsIndexDuration;
   	
   	// MLME-BEACON.request - Std 802.15.4e-2012 (table 44a) page 141
   	uint8_t 	beaconType;
   	uint8_t 	channel;
   	uint8_t 	dstAddrMode; 
   	int64_t 	dstAddr;
   	bool		bsnSuppression;
   	
   	// MLME-BEACON-NOTIFY.indication - Std 802.15.4-2011 (table 16) page 87
	uint8_t 	bsn;
	uint8_t		finalCAPSlot;
	bool		associationPermit;
	bool 		gtsPermit;
	uint8_t 	linkQuality;
	uint64_t	timeStamp;
	short    	securityStatus @enum(MACenum);
	int			codeList[];
	// Pending Address Specification field
	uint8_t		nmbrShortAddrPending;	// Number of Short Addresses Pending
	uint8_t		nmbrExtAddrPending;		// Number of Extended Addresses Pending
	uint64_t	addrList[];
	int			sduLength;
	uint8_t		sdu[];
	uint8_t		ebsn;					// Std 802.15.4e-2012
   	   	
   	// MLME-COMM_STATUS.indication - Std 802.15.4-2011 (table 18) page 88
   	uint8_t		srcAddrMode;
   	uint64_t	srcAddr;
   	
   	// MLME-GTS.confirm - Std 802.15.4-2011 (table 22) page 92
   	uint8_t     gtsLength;      	// GTS Length
    bool        gtsDirection;  		// GTS Direction
    bool        gtsCharType;    	// Characteristics Type
    
    // MLME-SYNC-LOSS.indication - Std 802.15.4-2011 (table 37) page 109
    uint8_t		lossReason;
   	   	
   	// for Primitives for TSCH
   	// MLME-SET-SLOTFRAME.requ.st - Std 802.15.4e-2012 (table 44d) page 144
   	uint8_t slotframeHandle;
   	uint8_t operation;
   	uint16_t size;
   	
   	// MLME-SET-LINK.request - Std 802.15.4e-2012 (table 44f) page 146
   	uint16_t linkHandle; 
   	uint16_t timeslot;
   	uint8_t  linkOptions;
	uint8_t  linkType;
	uint16_t nodeAddr;

	// MLME-TSCH-MODE.request - Std 802.15.4e-2012 (table 44h) page 147
   	uint8_t tschMode;
   	
   	// MLME-KEEP-ALIVE.request - Std 802.15.4e-2012 (table 44j) page 149	
   	uint16_t keepAlivePeriod;
   	
   	// for Primitives for LLDN
   	// MLME-LLDN-DISCOVERY.request - Std 802.15.4e-2012 (table 44l) page 150	
   	uint8_t lowLatencyNetworkConfiguration[];
 
 	// MLME-LLDN-CONFIGURATION.request - Std 802.15.4e-2012 (table 44n) page 151
 	// MLME-LLDN-ONLINE.request - Std 802.15.4e-2012
 	
   // Primitives for DSME
   // MLME-DSME-GTS.request - Std 802.15.4e-2012 (table 44q) page 154
   uint8_t  managementType;
   uint8_t  direction;
   uint8_t  prioritizedChannelAccess;
   uint8_t  NumSlot;
   uint16_t preferredSuperframeID; 
   uint8_t  preferredSlotID;
   uint8_t dsmeSABSpecification[];
   
   // MLME-DSME-INFO.request - Std 802.15.4e-2012 (table 44u) page 161
   uint8_t info;
   uint8_t dsmeSABSubBlockLength;
   uint8_t dsmeSABSubBlockIndex;
   
   // MLME-DSME-LINKSTATUSRPT.request - Std 802.15.4e-2012 (table 44x) page 165
   uint32_t ReportPeriod;

	// MCPS-DATA.confirm - Std 802.15.4-2011 (table 48) page 122
	uint8_t 	msduHandle;
	bool		rangingReceived;
	uint32_t	rangingCounterStart;
	uint32_t	rangingCounterStop;
	uint32_t	rangingTrackingInterval;
	uint32_t	rangingOffset;
	uint8_t		rangingFOM;
	// Std 802.15.4e-2012 (table 47) page 170
	uint8_t		numBackoffs;
	uint8_t		ackPayload[];
	
	bool receivedByACK;
	
	
	int stage;
	
	int WaitConstant;
	
}
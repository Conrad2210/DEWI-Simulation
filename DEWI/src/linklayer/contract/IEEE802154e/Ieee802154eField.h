/*
 * Ieee802154eField.h
 *
 *  Created on: 2 May 2014
 *      Author: R00118979
 */

#ifndef IEEE802154EFIELD_H_
#define IEEE802154EFIELD_H_

#include "Ieee802154eDef.h"
#include "Ieee802154Def.h"
#include "Ieee802154eEnum.h"
#include "Ieee802154eConst.h"
#include <vector>
#include "SlotframeLinkIE.h"

// Addressing mode
#define             defFrmCtrl_AddrModeNone     0x00
#define             defFrmCtrl_AddrMode8        0x01
#define             defFrmCtrl_AddrMode16       0x02
#define             defFrmCtrl_AddrMode64       0x03

// Frame Version
#define             defFrmVs_802154_2003        0x00
#define             defFrmVs_802154_2006        0x01
#define             defFrmVs_802154             0x10

// Transmission option
//#define             defTxOp_Acked               0x01
//#define             defTxOp_GTS                 0x02
//#define             defTxOp_Indirect            0x04
//#define             defTxOp_SecEnabled          0x08

struct SuperframeSpec
{
        UINT_8          BO;         // beacon order
        UINT_32         BI;         // beacon interval
        UINT_8          SO;         // superframe order
        UINT_32         SD;         // superframe duration
        UINT_8          finalCap;       // final superframe slot utilized by the CAP
        bool            battLifeExt;    // battery life extention (BLE)
        bool            panCoor;        // PAN coordinator
        bool            assoPmt;        // association permit
};

//Elements of PAN descriptor (Table 41)
struct PAN_ELE
{
        // Std 802.15.4-2011 (table 17) page 87
            Ieee802154eAddrMode  coordAddrMode;
            UINT_16         coordPANId;

            IE3ADDR         coordAddress_16_or_64;      // shared by both 16 bit short address or 64 bit extended address

            // not in the Std
            UINT_8          logicalChannel;

            UINT_8          channelNumber;
            UINT_8          channelPage;

            SuperframeSpec  superframeSpec;
            bool            gtsPermit;
            UINT_8          linkQuality;
            simtime_t     timeStamp;              // ignored, use bcnRxTime instead [SR]TODO: need to check!!
            MACenum        securityStatus;
            UINT_8          securityLevel;
            UINT_8          keyIdMode;
            UINT_64         keySource;
            UINT_8          keyIndex;
            std::vector<int>    CodeList;

            // Std 802.15.4e-2012 (table 17) page 132
//            IE_DSME_PAN DSMESuperframeSpecification;
//            IE_DSME_PAN_TimeSyncSpec TimeSynchronizationSpecification;
//            IE_DSME_PAN_BeaconBitmap BeaconBitmap;
//            IE_DSME_PAN_ChannelHoppingSpec  ChannelHoppingSpecification;
//            IE_DSME_PAN_GroupACKSpec    GroupACKSpecification;

            // not defined in Std 802.15.4-2011 and Std 802.15.4e-2012
            UINT_8          ACLEntry;
            bool            SecurityFailure;
            //add one field for cluster tree
            //UINT_16   clusTreeDepth;
};

struct PHY_PIB
{

    int             phyCurrentChannel;
    UINT_32         phyChannelsSupported;
    double          phyTransmitPower; // Std 802.15.4-2006 (table 23) page 46
    UINT_8          phyTXPowerTolerance;    // Std 802.15.4-2011 (table 71) page 156
    signed int      phyTXPower;
    UINT_8          phyCCAMode;
    UINT_8          phyCurrentPage;     // Std 802.15.4-2006 (table 2) page 30
    UINT_16         phyMaxFrameDuration;
    UINT_8          phySHRDuration;
    float           phySymbolsPerOctet;
    UINT_8          phyPreambleSymbolLength;
    UINT_64         phyUWBDataRatesSupported;
    bool            phyCSSLowDataRateSupported;
    bool            phyUWBCoUSupported;
    bool            phyUWBCSSupported;
    bool            phyUWBLCPSupported;
    UINT_8          phyUWBCurrentPulseShape;
    UINT_8          phyUWBCoUpulse;
    UINT_8          phyUWBCSpulse;
    signed int      phyUWBLCPWeight1;
    signed int      phyUWBLCPWeight2;
    signed int      phyUWBLCPWeight3;
    signed int      phyUWBLCPWeight4;
    UINT_8          phyUWBLCPDelay2;
    UINT_8          phyUWBLCPDelay3;
    UINT_8          phyUWBLCPDelay4;
    bool            phyRanging;
    bool            phyRangingCrystalOffset;
    bool            phyRangingDPS;
    UINT_8          phyCurrentCode;
    UINT_8          phyNativePRF;
    UINT_8          phyUWBScanBinsPerChannel;
    UINT_8          phyUWBInsertedPreambleInterval;
    UINT_32         phyTXRMARKEROffset;
    UINT_32         phyRXRMARKEROffset;
    UINT_8          phyRFRAMEProcessingTime;
    UINT_16         phyCCADuration;

};

// MAC PIB attributes - Std 802.15.4-2011 (table 52) page 126
struct MAC_PIB
{
        IE3ADDR         macExtendedAddress;
        UINT_8          macAckWaitDuration;
        bool            macAssociatedPANCoord;
        bool            macAssociationPermit;
        bool            macAutoRequest;
        bool            macBattLifeExt;
        UINT_8          macBattLifeExtPeriods;
        UINT_8          macBeaconPayload[aMaxBeaconPayloadLength + 1];
        UINT_8          macBeaconPayloadLength;
        UINT_8          macBeaconOrder;
        double          macBeaconTxTime;            // we use actual time in double instead of integer in spec
        UINT_8          macBSN;                     // sequence number for beacon pkt
        IE3ADDR         macCoordExtendedAddress;
        UINT_16         macCoordShortAddress;
        UINT_8          macDSN;                     // sequence number for data or cmd pkt
        bool            macGTSPermit;
        UINT_8          macMaxBE;               // range 3-8; except for LLDN mode = 3; max value of the backoff exponent in the CSMA-CA algorithm
        UINT_8          macMaxCSMABackoffs;     // range 0-5; except for LLDN mode = 0
        UINT_8          macMaxFrameRetries;     //max # range 0-7; of retries allowed after a transmission failures
        UINT_8          macMinBE;
        UINT_8          macLIFSPeriod;  //min # of symbols comprising a LIFS period; PHY dependent
        UINT_8          macSIFSPeriod;  //min # of symbols comprising a SIFS period; PHY dependent
        UINT_16         macPANId;
        bool            macPromiscuousMode;
        UINT_8          macResponseWaitTime;
        bool            macRxOnWhenIdle;    // for non-beacon or supporting direct transmission in beacon-enabled
        bool            macSecurityEnabled;
        UINT_16         macEnhAckWaitDuration;  //max # (864 µs) max time (in μs) to wait for the PHY header of an enhanced ACK frame to arrive following a transmitted data frame
        bool            macImplicitBroadcast;
        UINT_8          macSimpleAddress;
        UINT_16         macShortAddress;
        UINT_8          macSuperframeOrder;
        UINT_16         macSyncSymbolOffset;
        bool            macTimestampSupported;
        UINT_16         macTransactionPersistenceTime;
        UINT_32         macTxControlActiveDuration;
        UINT_16         macTxControlPauseDuration;
        UINT_32         macTxTotalDuration;     // total transmit duration (including PHY header and FCS) specified in symbols

        // Security-related MAC PIB attributes - Std 802.15.4-2011 (table 60) page 142
        // macKeyTable
        // macDeviceTable
        // macSecurityLevelTable
        UINT_64         macFrameCounter;
        UINT_8          macAutoRequestSecurityLevel;
        UINT_8          macAutoRequestKeyIdMode;
        // macAutoRequestKeySource
        UINT_8          macAutoRequestKeyIndex;
        // macDefaultKeySource
        IE3ADDR         macPANCoordExtendedAddress;
        UINT_16         macPANCoordShortAddress;
        // Security-related MAC PIB attributes - Std 802.15.4e-2012 (table 60) page 188
        UINT_8          macFrameCounterMode;

        // General MAC PIB attributes for functional organization - Std 802.15.4e-2012 (table 52a) page 173
        bool            macTSCHcapable;
        bool            macLLcapable;
        bool            macDSMEcapable;
        bool            macLEcapable;
        bool            macRFIDcapable;
        bool            macHoppingCapable;
        bool            macAMCACapable;
        bool            macMetricsCapable;
        bool            macTSCHenabled;
        bool            macLLenabled;
        bool            macDSMEenabled;
        bool            macLEenabled;
        bool            macRFIDenabled;
        bool            macHoppingEnabled;
        bool            macAMCAenabled;
        bool            macMetricsEnabled;

        // TSCH-specific MAC PIB attributes - Std 802.15.4e-2012 (table 52b) page 174
        UINT_8          macDisconnectTime;      // Timeslots to send out Disassociate frames before disconnecting
        UINT_8          macJoinPriority;        // The lowest join priority from the TSCH Synchronization IE in an Enhanced beacon
        UINT_64         macASN;                 // Absolute Slot Number
        bool            macNoHLBuffers;         // If FALSE, higher layer accept frame payload; if TRUE, higher layer cannot buffer it, NACK must be send

        // Metrics-specific MAC PIB attributes - Std 802.15.4e-2012 (table 52I) page 183
        UINT_64         macCounterOctets;       // Defines the counter size in octets for attributes 0x91 through 0x99. // note: here fix to uint64_t
        UINT_64         macRetryCount;          // The number of transmitted frames that required exactly one retry before acknowledgment.
        UINT_64         macMultipleRetryCount;  // The number of transmitted frames that required more than one retry before acknowledgment.
        UINT_64         macTXFailCount;         // The number of transmitted frames that did not result in an acknowledgment after macMaxFrameRetries.
        UINT_64         macTXSuccessCount;      // The number of transmitted frames that were acknowledged within macAckWaitDuration after the initial data frame transmission.
        UINT_64         macFCSErrorCount;       // The number of received frames that were discarded due to an incorrect FCS.
        UINT_64         macSecurityFailure;     // The number of received data frames that were returned from the procedure described in 7.2.3 with any status other than �SUCCESS.�
        UINT_64         macDuplicateFrameCount; // The number of received data frames that contained the same sequence number as a frame previously received (accounting for wrap-around of macDSN).
        UINT_64         macRXSuccessCount;      // The number of received data frames that were received correctly.
        UINT_64         macNACKcount;           // The number of transmitted frames that were acknowledged with a ACK/NACK timing correction IE indicating a NACK. Frames acknowledged in this manner also should increment macRetryCount or macMultipleRetryCount.

        // Enhanced Beacon-specific MAC PIB attributes - Std 802.15.4e-2012 (table 52n) page 185
        bool            macUseEnhancedBeacon;
        // macEBIEList
        bool            macEBFilteringEnabled;  // Indicates if devices should perform filtering in response to EBR.
        UINT_8          macEBSN;                // Beacon Sequence Number used for Enhanced Beacon Frames (separate from BSN).
        UINT_8          macEBAutoSA;            // 0: NONE; 1:SHORT; 2:FULL (default). Indicates if beacons generated by the MAC in response to EB include Source Address field
        // macEAckIElist
};

// TSCH-MAC PIB attributes for macSlotframeTable - Std 802.15.4e-2012 (figure 52c) page 175
//struct MAC_SlotframeTable
//{
//    UINT_8      macSlotframeHandle;
//    UINT_16     macSlotframeSize;
//};
//
//// TSCH-MAC PIB attributes for macSlotframeTable - Std 802.15.4e-2012 (figure 52d) page 175
//struct MAC_LinkTable
//{
//    UINT_16     macLinkHandle;
//    UINT_8      macLinkOptions;  // bitmap; b0 = Transmit, b1 = Receive, b2 = Shared, b3 = Timekeeping, b4–b7 reserved
//    MACTSCHLinkType macLinkType;
//    UINT_8      SlotframeHandle;  // Identifier of Slotframe to which this link belongs
//    UINT_16     macNodeAddress;
//    UINT_16     macTimeslot;
//    UINT_16     macChannelOffset;
//};

//// TSCH-MAC PIB attributes for macTimeslotTemplate - Std 802.15.4e-2012 (figure 52e) page 176
//struct MAC_TimeslotTemplate
//{
//    UINT_8      macTimeslotTemplateId;
//    UINT_16     macTsCCAOffset;
//    UINT_16     macTsCCA;
//    UINT_16     macTsTxOffset;
//    UINT_16     macTsRxOffset;
//    UINT_16     macTsRxAckDelay;
//    UINT_16     macTsTxAckDelay;
//    UINT_16     macTsRxWait;
//    UINT_16     macTsAckWait;
//    UINT_16     macTsRxTx;
//    UINT_16     macTsMaxAck;
//    UINT_16     macTsMaxTx;
//    UINT_16     macTsTimeslotLength;
//};

// MAC PIB attributes for Hopping Sequence - Std 802.15.4e-2012 (figure 52f) page 177
//struct MAC_Hopping_Sequence
//{
//    UINT_8      macHoppingSequenceID;
//    UINT_8      macChannelPage;
//    UINT_16     macNumberOfChannels;
//    UINT_32     macPhyConfiguration;
//    UINT_32     macExtendedBitmap;          // (variable)
//    std::vector<int>     macHoppingSequenceLength;
//    UINT_16     macHoppingSequenceList;
//    UINT_16     macCurrentHop;
//    UINT_16     hopDwellTime;
//};

// LLDN MAC PIB attributes - Std 802.15.4e-2012 (figure 52g) page 178
//struct MAC_LLDN
//{
//    UINT_8      macLLDNnumTimeSlots;
//    UINT_8      macLLDNnumUplinkTS;
//    UINT_8      macLLDNnumRetransmitTS;
//    UINT_8      macLLDNnumBidirectionalTS;
//    bool        macLLDNmgmtTS;
//    UINT_8      macLLDNlowLatencyNWid;
//    //UINT_16   macLLDNtimeSlotInfo;        // Implementation specific
//    UINT_16     macLLDNdiscoveryModeTimeout;
//    bool        macLLDNcoordinator;
//};

// Elements of Neighbor Information - Std 802.15.4e-2012 (figure 52i) page 181
//struct MAC_Neighbor_Information
//{
//    UINT_16     ShortAddress;
//    IE3ADDR     ExtendedAddress;
//    UINT_16     SDIndex;
//    UINT_16     ChannelOffset;
//    bool        TrackBeacon;
//    UINT_8      BeaconLostCount;
//};

// DSME-specific MAC PIB attributes - Std 802.15.4e-2012 (figure 52h) page 178
//struct MAC_DSME_SPEC
//{
//    UINT_8      macChannelIndex;
//    UINT_8      macAvgLQI;
//    UINT_8      macAvgRSSI;
//    UINT_32     macLinkStatusStatisticPeriod;  // Implementation specific
//    bool        macGACKFlag;
//    bool        macCAPReductionFlag;
//    UINT_8      macChannelDiversityMode;    // 0x00 = Channel Adaptation; 0x01 = Channel Hopping
//    UINT_8      macMultisuperframeOrder;
//    bool        macConnecDev;
//    UINT_8      macDSMESAB; // Refer to Figure 59q and Figure 59r
//    UINT_8      macDSMEACT; // table 1a
//    UINT_16     macSDindex;
//    UINT_8      macSDBitmap;    // Refer to 5.2.4.9.3
//    UINT_16     macChannelOffset;
//    bool        macDeferredBeaconUsed;
//    IE3ADDR     macSyncParentExtendedAddress;
//    UINT_16     macSyncParentShortAddress;
//    UINT_16     macSyncParentSDIndex;
//    UINT_8      macChannelStatus;   // table 44y
//    UINT_16     macBeaconSlotLength;
//    UINT_8      macDSMEGTSExpirationTime;
//    UINT_8      macChannelOffsetBitmapLength;
//    std::vector<UINT_8> macChannelOffsetBitmap;
//    UINT_8      macPANCoordinatorBSN;
//    std::vector<MAC_Neighbor_Information>   macNeighborInformationTable; // table 52i
//};

// LE-specific MAC PIB attributes - Std 802.15.4e-2012 (figure 52j) page 181
struct MAC_LE_SPEC
{
    UINT_16     macCSLPeriod;
    UINT_16     macCSLMaxPeriod;
    UINT_32     macCSLChannelMask;
    UINT_16     macCSLFramePendingWaitT;
    UINT_32     macRITPeriod;
    UINT_8      macRITDataWaitDuration;
    UINT_32     macRITTxWaitDuration;
    bool        macLowEnergySuperframeSupported;  // Implementation specific
    UINT_16     macLowEnergySuperframeSyncInterval;  // Implementation specific
};

// AMCA-specific MAC PIB attributes - Std 802.15.4e-2012 (figure 52k) page 182
//struct MAC_AMCA_SPEC
//{
//    UINT_8      macDesignatedListeningChannel;
//};

// Metrics-specific MAC PIB attributes - Std 802.15.4e-2012 (figure 52l) page 183
struct MAC_METRICS_SPEC
{
    UINT_8      macCounterOctets;
    UINT_32     macRetryCount;
    UINT_32     macMultipleRetryCount;
    UINT_32     macTXFailCount;
    UINT_32     macTXSuccessCount;
    UINT_32     macFCSErrorCount;
    UINT_32     macSecurityFailure;
    UINT_32     macDuplicateFrameCount;
    UINT_32     macRXSuccessCount;
    UINT_32     macNACKcount;
};

// EBR-specific MAC PIB attributes - Std 802.15.4e-2012 (figure 52m) page 184
struct MAC_EBR_SPEC
{
    bool        EBRPermitJoining;
    std::vector<UINT_8>     macEBRFilters;
    UINT_8      macEBRLinkQuality;
    UINT_8      macEBRPercentFilter;
    UINT_8      macEBRattributeList;
    bool        macBeaconAutoRespond;
};

//Elements of ACL entry descriptor (Table 73)
struct MAC_ACL
{
    IE3ADDR         ACLExtendedAddress;
    UINT_16         ACLShortAddress;
    UINT_16         ACLPANId;
    UINT_8          ACLSecurityMaterialLength;
    UINT_8*         ACLSecurityMaterial;
    UINT_8          ACLSecuritySuite;
};

// Frame Control field in MHR (Figure 35) - Std 802.15.4e-2012 (figure 36) page 59
struct FrameCtrl
{
    Ieee802154eFrameType        frameType;
    bool            securityEnabled;            // Security Enabled
    bool            frmPending;                 // Frame Pending
    bool            ackReq;                     // Acknowledgment Request
    bool            compPanID;                  // PAN ID Compression
    bool            seqNbSup;                   // Sequence Number Suppression
    bool            presIElist;                 // IE List Present
    UINT_8          dstAddrMode;                // Dest. Addressing Mode
    Ieee802154eFrameVersion frameVersion;       // Frame Version
    UINT_8          srcAddrMode;                // Source Addressing Mode
};

// Multipurpose Frame Control field - Std 802.15.4e-2012 (figure 48k) page 75
//struct MPFrameCtrl
//{
//    Ieee802154eFrameType        frmType;
//    bool            lngFrameCtrl;
//    UINT_8          dstAddrMode;
//    UINT_8          srcAddrMode;
//    bool            presPANID;
//    bool            secEnable;
//    bool            seqNmbrSup;
//    bool            frmPeding;
//    bool            frmVs;
//    bool            ackReq;
//    bool            presIElist;
//};

// Superframe Specification field (16 bits) - Std 802.15.4-2011 (figure 41) page 62
//struct SuperframeSpec
//{
//    UINT_8          BO;         // beacon order
//    UINT_32         BI;         // beacon interval
//    UINT_8          SO;         // superframe order
//    UINT_32         SD;         // superframe duration
//    UINT_8          finalCap;       // final superframe slot utilized by the CAP
//    bool            battLifeExt;    // battery life extention (BLE)
//    bool            panCoor;        // PAN coordinator
//    bool            assoPmt;        // association permit
//};

// Capability Information field - Std 802.15.4-2011 (figure 50) page 69
struct DevCapability
{
    bool            alterPANCoor;   // not in Std
    bool            FFD;
    bool            powerSource;
    bool            recvOnWhenIdle;
    bool            secuCapable;
    bool            alloShortAddr;
    const char*     hostName;       // only for convenience
};

// for GTS information field - GTS Directions field - Std 802.15.4-2011 (figure 43) page 63
// (Guaranteed Time Slot)
//struct GTSDescriptor
//{
//    UINT_16         devShortAddr;   // device short address
//    UINT_8          startSlot;      // starting slot
//    UINT_8          length;         // length in slots
//    bool            isRecvGTS;      // transmit or receive in GTS, not defined here is spec, but we put it here just for convenience
//    bool            isTxPending;    // there is a data pending for txing
//};

// GTS information field - Std 802.15.4-2011 (figure 39) page 62
// (Guaranteed Time Slot)
//struct  GTSFields
//{
//    // GTS Specification
//    UINT_8      gtsDescrCnt;
//    bool        gtsPermit;
//    // GTS Directions
//    UINT_8      gtsDirMask;
//    // GTS List
//    GTSDescriptor      gtsDescr[7];   // Std 802.15.4-2011 - 5.2.2.1.5 - limit to seven
//};

// Pending Address Specification field - Std 802.15.4-2011 (figure 45) page 64
//struct PendingAddrFields
//{
//    UINT_8          numShortAddr;       // num of short addresses pending
//    UINT_8          numExtendedAddr;    // num of extended addresses pending
//    IE3ADDR         addrList[7];        // pending address list (shared by short/extended addresses)
//};

// Auxiliary security header - Std 802.15.4-2011 (figure 62) page 139
struct AuxiliarySecurityHeader
{
    // Security Control field
    // Security Level field
    Ieee802154eASHSeclvlType    secLvl;
    // Key Identifier Mode
    UINT_8          keyIdentMode;

    UINT_32          frmCounter;
    // Key Identifier
    // Key Source
    UINT_64         keySource;
    UINT_8            keyIndex;
};

// for GTS information field - GTS Directions field - Std 802.15.4-2011 (figure 43) page 63
// (Guaranteed Time Slot)
struct GTSDescriptor
{
    UINT_16         devShortAddr;   // device short address
    UINT_8          startSlot;      // starting slot
    UINT_8          length;         // length in slots
    bool            isRecvGTS;      // transmit or receive in GTS, not defined here is spec, but we put it here just for convenience
    bool            isTxPending;    // there is a data pending for txing
};
// GTS information field - Std 802.15.4-2011 (figure 39) page 62
// (Guaranteed Time Slot)
struct  GTSFields
{
    // GTS Specification
    UINT_8      gtsDescrCnt;
    bool        gtsPermit;
    // GTS Directions
    UINT_8      gtsDirMask;
    // GTS List
    GTSDescriptor      gtsDescr[7];   // Std 802.15.4-2011 - 5.2.2.1.5 - limit to seven
};
// Pending Address Specification field - Std 802.15.4-2011 (figure 45) page 64
struct PendingAddrFields
{
    UINT_8          numShortAddr;       // num of short addresses pending
    UINT_8          numExtendedAddr;    // num of extended addresses pending
    IE3ADDR         addrList[7];        // pending address list (shared by short/extended addresses)
};

// Superframe Specification field (16 bits) - Std 802.15.4-2011 (figure 41) page 62



// Frame Control field (LLDN frame) - Std 802.15.4e-2012 (figure 48b) page 68
// (Low Latency Deterministic Network)
//struct LLDNFrameCtrl
//{
//    Ieee802154eFrameType        frmType;
//    bool                        securityEnabled;
//    bool                        frmVs;
//    bool                        ackRequest;
//    Ieee802154eLLDNFrameSubtype subFrameType;
//};

// LLDN beacon frame - Flags - Std 802.15.4e-2012 (figure 48d) page 70
// (Low Latency Deterministic Network)
//struct LLDNBeaconFrameFlgs
//{
//    Ieee802154eLLDNBeaconFlgsTStype        trnsState;
//    bool                        trnsDirec;
//    UINT_8                      nmbBaseTimesltMngTimeslt;
//};

// LE CSL Information Element field - Std 802.15.4e-2012 (figure 48t) page 84
// (Low Energy Coordinated Sampled Listening)
struct IE_LE_CSL
{
    UINT_16          cslPhase;
    UINT_16          cslPeriod;
};

// LE RIT Information Element field - Std 802.15.4e-2012 (figure 48u) page 84
// (Low Energy Receiver Initiated Transmission)
struct IE_LE_RIT
{
    UINT_8          timeToFirstListen;
    UINT_8          nmbOfRepeatListen;
    UINT_16         repeatListenInterval;
};

// for DSME PAN IE - DSME Superframe Specification - Std 802.15.4e-2012 (figure 48w) page 85
//struct IE_DSME_PAN_SuperframeSpec
//{
//   UINT_8           multiSuperframeOrder;
//   bool             channelDiersityMode;
//   bool             gackFlag;
//   bool             capReductionFlag;
//   bool             deferredBeaconFlag;
//};

// for DSME PAN IE - Time Synchronization specification - Std 802.15.4e-2012 (figure 48x) page 86
//struct IE_DSME_PAN_TimeSyncSpec
//{
//   UINT_64          beaconTimestamp;
//   UINT_16          beaconOffsetTimestamp;
//};

// for DSME PAN IE - Beacon bitmap - Std 802.15.4e-2012 (figure 48y) page 86
//struct IE_DSME_PAN_BeaconBitmap
//{
//   UINT_16          sdIndex;
//   UINT_16          sdBitmapLength;
//};

// for DSME PAN IE - Channel Hopping Specification - Std 802.15.4e-2012 (figure 48z) page 87
//struct IE_DSME_PAN_ChannelHoppingSpec
//{
//   UINT_8           hoppingSequenceID;
//   UINT_8           panCoordinatorBSN;
//   UINT_16          channelOffset;
//   UINT_8           channelOffsetBitmapLength;
//};

// for DSME PAN IE - Group ACK Specification - Std 802.15.4e-2012 (figure 48aa) page 87
//struct IE_DSME_PAN_GroupACKSpec
//{
//   UINT_16          gACK1SuperframeID;
//   UINT_8           gACK1SlotID;
//   UINT_8           gACK1ChannelID;
//   UINT_16          gACK2SuperframeID;
//   UINT_8           gACK2SlotID;
//   UINT_8           gACK2ChannelID;
//};
struct IE_DSME_PAN_SuperframeSpec
{
   UINT_8           multiSuperframeOrder;
   bool             channelDiersityMode;
   bool             gackFlag;
   bool             capReductionFlag;
   bool             deferredBeaconFlag;
};

// for DSME PAN IE - Time Synchronization specification - Std 802.15.4e-2012 (figure 48x) page 86
struct IE_DSME_PAN_TimeSyncSpec
{
   UINT_64          beaconTimestamp;
   UINT_16          beaconOffsetTimestamp;
};

// for DSME PAN IE - Beacon bitmap - Std 802.15.4e-2012 (figure 48y) page 86
struct IE_DSME_PAN_BeaconBitmap
{
   UINT_16          sdIndex;
   UINT_16          sdBitmapLength;
   std::vector<UINT_8> sdBitmap;
};

// for DSME PAN IE - Channel Hopping Specification - Std 802.15.4e-2012 (figure 48z) page 87
struct IE_DSME_PAN_ChannelHoppingSpec
{
   UINT_8           hoppingSequenceID;
   UINT_8           panCoordinatorBSN;
   UINT_16          channelOffset;
   UINT_8           channelOffsetBitmapLength;
};

// for DSME PAN IE - Group ACK Specification - Std 802.15.4e-2012 (figure 48aa) page 87
struct IE_DSME_PAN_GroupACKSpec
{
   UINT_16          gACK1SuperframeID;
   UINT_8           gACK1SlotID;
   UINT_8           gACK1ChannelID;
   UINT_16          gACK2SuperframeID;
   UINT_8           gACK2SlotID;
   UINT_8           gACK2ChannelID;
};
// DSME PAN descriptor Information Element field - Std 802.15.4e-2012 (figure 48v) page 85
// (Deterministic and Synchronous Multi-channel Extension)
struct IE_DSME_PAN
{
    SuperframeSpec              superframeSpec;
    PendingAddrFields           pendingAdrField;
    IE_DSME_PAN_SuperframeSpec  ieDSMEsuperframeSpec;
    IE_DSME_PAN_TimeSyncSpec    ieDSMEtimeSyncSpec;
    IE_DSME_PAN_BeaconBitmap    ieDSMEbeaconBitmap;
    IE_DSME_PAN_ChannelHoppingSpec  ieDSMEchannelHoppingSpec;
    IE_DSME_PAN_GroupACKSpec    ieDSMEgroupACKSpec;
};



// RZ Time Information Element - Std 802.15.4e-2012 page 88
struct IE_RZ_TIME
{
    // Rendezvous Time
    UINT_16         rzTime;
};

// ACK/NACK time correction Information Element - Std 802.15.4e-2012 (figure 48bb) page 88
struct IE_ACK_NACK_TIME_CORREC
{
    UINT_16         timeSyncInfo;       // Time Sync Info
};

// Group ACK Information Element - Std 802.15.4e-2012 (figure 48cc) page 89
//struct IE_GROUP_ACK
//{
//    // Group ACK Control Field
//    UINT_8              payloadSize;        // in octets
//    UINT_8              bitmapSize;         // in octets
//
//    UINT_32             gACKBitmap;         // max 32 bits
//    std::vector<int>    gACKDevLst;     // GACK Device List (variable)
//    std::vector<int>    gAckindex;      // GACK Index (variable)
//    std::vector<int>    gACKGTSDirec;   // GTS Directions (variable)
//};

// TODO: Missing specification for Low Latency Network Info IE (Element ID == 0x20)(table 4b) page 81

// TSCH synchronization Information Element - Std 802.15.4e-2012 (figure 48ee) page 90
struct IE_TSCH_SYNC
{
    UINT_64         tschASN;    // Absolute Slot Number
    UINT_8          joinPriotity;
};

// for TSCH slotframe and link IE - Slotframe and link fields - Std 802.15.4e-2012 (figure 48hh) page 91
struct IE_TSCH_SLOTFRAME_LINK_INFO
{
    UINT_16         linkTimeslots;
    UINT_16         linkChannelOffset;
    UINT_8          linkOptions;
};

// for TSCH slotframe and link IE - Link Information fields - Std 802.15.4e-2012 (figure 48gg) page 91
struct IE_TSCH_SLOTFRAME_LINK_FIELD
{
    UINT_8          slotfrmHandle;                  // Slotframe Handle
    UINT_16         slotfrmSize;                    // Slotframe Size
    UINT_8          nmbrLinks;                      // Number of Links
    std::vector<IE_TSCH_SLOTFRAME_LINK_INFO>   linkInfos;     // Link Information for each Link
};

// TSCH slotframe and link Information Element - Std 802.15.4e-2012 (figure 48ff) page 90
struct IE_TSCH_SLOTFRAME_LINK
{
    UINT_8          nmbrSlotfrm;                            // Numbers of Slotframes
    std::vector<IE_TSCH_SLOTFRAME_LINK_FIELD>    slotFrm;    // Slotframe and link fields
};

// TSCH Timeslot Information Element - Std 802.15.4e-2012 (figure 48ii) page 92
struct IE_TSCH_TIMESLOT
{
    UINT_8          timeslotID;
    UINT_16         macTsCCAOffset;
    UINT_16         macTsCCA;
    UINT_16         macTsTxOffset;
    UINT_16         macTsRxOffset;
    UINT_16         macTsRxAckDelay;
    UINT_16         macTsTxAckDelay;
    UINT_16         macTsRxWait;
    UINT_16         macTsAckWait;
    UINT_16         macTsRxTX;
    UINT_16         macTsMaxAck;
    UINT_16         macTsMaxTx;
    UINT_16         macTsTimeslotLengt;
};

// Channel hopping Information Element - Std 802.15.4e-2012 (figure 48jj) page 92
struct IE_CHANNEL_HOPPING
{
    UINT_8              hoppingSeqID;
    UINT_8              channelPage;
    UINT_16             numberOfChannels;
    UINT_32             phyConfig;
    std::vector<int>    extBitmap;          // extended bitmap (variable)
    UINT_16             hoppingSeqLength;
    std::vector<int>    hoppingSeqLst;      // Hopping sequence list (variable)
    UINT_16             currentHop;
};

// Hopping timing Information Element - Std 802.15.4e-2012 (figure 48kk) page 92
struct IE_HOPPING_TIMING
{
    UINT_32         presentHopTimeOffset;   // in units of 1 µs
    UINT_16         hopDwellTime;           // MAC PIB attributes for Hopping Sequence
};

// EB Filter Information Element - Std 802.15.4e-2012 (figure 48ll) page 93
struct IE_EB_FILTER
{
    bool            permitJoiningOn;
    bool            includeLinkQualityFilter;
    bool            includePercentFilter;
    UINT_8          nmbrOfEntriesPIBIdentifierList;
    UINT_8          linkQuality;
    UINT_8          percentFilter;
    UINT_32         listPidId;
};

// MAC Metrics Information Element - Std 802.15.4e-2012 (figure 48mm) page 94
struct IE_MAC_METRICS
{
    Ieee802154eIEMACMtype   metricID;
    UINT_32         metricCount;
};

// AllMAC Metrics Information Element - Std 802.15.4e-2012 (figure 48nn) page 94
struct IE_ALLMAC_METRICS
{
    UINT_32         macCounterOctets;
    UINT_32         macRetryCount;
    UINT_32         macMultipleRetryCount;
    UINT_32         macTXFailCount;
    UINT_32         macTXSuccessCount;
    UINT_32         macFCSErrorCount;
    UINT_32         macSecurityFailure;
    UINT_32         macDuplicateFrameCount;
    UINT_32         macRXSuccessCount;
    UINT_32         macNACKcount;
};

// Encapsulated Service Data Unit Information Elements - STd 802.15.4e-2012 (figure 48p) page 82
struct IE_ESDU
{
    UINT_16                     ieESDULength;
    Ieee802154eIEPayloadType    groupID;
    bool                        typeDescriptor;
    int                         ieContent[2047];    // IE Content; length 0...2047
};

struct  IE_MLME_Sub
{
    UINT_8                      ieNestedMLMELength;
    Ieee802154eIEMLMESubtype    ieMLMESubID;
    bool                        ieMLMEtype;     // zero -> short content (0...255); one -> long content (0...2047)

    // MLME IE Sub Content - short
    // TSCH Synchronization
    IE_TSCH_SYNC*                ieTSCHSync;
    // TSCH Slotframe and Link
    //IE_TSCH_SLOTFRAME_LINK*      ieTSCHSLotfrmLink; //[SR] use of [CD] IMacSlotframeTable
    SlotframeLinkIE*             ieTSCHSLotfrmLink;
    // TSCH Timeslot
    macTimeslotTableEntry*       ieTSCHTimeslt;
    // Hopping Timing
    IE_HOPPING_TIMING*           ieHoppingTiming;
    // EB Filter
    IE_EB_FILTER*                ieEBFilter;
    // MAC Metrics 1
    IE_MAC_METRICS*              ieMACMetrics1;
    // MAC Metrics 2
    IE_ALLMAC_METRICS*           ieMACMetrics2;

    // MLME IE Sub Content - long
    // Channel Hopping Sequence
    IE_CHANNEL_HOPPING*          ieChannelHop;
};

// MLME Information Element field - Std 802.15.4e-2012 (figure 48r) page 82
struct IE_MLME
{
    // Sub-IE descriptor
    std::list<IE_MLME_Sub*>    ieMLMESub;
};


// Header Information Element field - Std 802.15.4e-2012 (figure 48n) page 80
struct IE_Header
{
        UINT_16                     ieLenght;       // 7 bits for IE Header; 11 bits for IE Payload
           Ieee802154eIEHeaderType     elementID;
           bool                        typeDescriptor; // must be zero for IE Header
           // IE Header Content:
           // LE CSL
           IE_LE_CSL*                  ieLECSL;
           // LE RIT
           IE_LE_RIT*                  ieLERIT;
           // DSME PAN Descriptor
           //IE_DSME_PAN*                ieDSMEPAN;
           // RZ Time
           IE_RZ_TIME*                 ieRZTime;
           // ACK/NACK Time-Correction
           IE_ACK_NACK_TIME_CORREC*    ieACKNACKTimeCorrc;
           // Group ACK (GACK)
           //IE_GROUP_ACK*               ieGroupACK;
};

// Payload Information Element field - Std 802.15.4e-2012 (figure 48o) page 81
struct IE_Payload
{
    UINT_16                     ieLenght;       // 7 bits for IE Header; 11 bits for IE Payload
    Ieee802154eIEPayloadType    groupID;
    bool                        typeDescriptor; // must be one for IE Payload
    // IE Payload Content
    // Encapsulated Service Data Unit (ESDU)
    IE_ESDU*                     ieESDU;
    // MLME (Nested)
    IE_MLME*                     ieMLME;
};

// Information Element List field - Std 802.15.4e-2012 (figure 36u) page 63
struct IE_LIST
{
    std::vector<IE_Header>  ieHeader;
    std::vector<IE_Payload> iePayload;
};

// EB-specific MAC PIB attributes - Std 802.15.4e-2012 (figure 52n) page 185
struct MAC_EB_SPEC
{
    bool        macUseEnhancedBeacon;
    std::vector<IE_LIST>     macEBIEList;
    bool        macEBFilteringEnabled;
    UINT_8      macEBSN;
    MACEAckIElist      macEBAutoSA;
    std::vector<IE_LIST> macEAckIElist;
};

// Capability Information field - Std 802.15.4e-2012 (figure 50a) page 96
struct CAPABILITY_INFO
{
    bool            altPANCoord;
    bool            deviceType;
    bool            powerDevice;
    bool            receiverOnWhenIdle;
    bool            associationType;
    bool            securityCapability;
    bool            allocateAddr;
};

// DSME-GTS Management field - Std 802.15.4e-2012 (figure 59j) page 107
//struct DSME_GTS_MANAGEMENT
//{
//    Ieee802154eDSMEGTSMgmttype  managementType;
//    bool            direction;
//    bool            prioChannelAccess;
//    UINT_8          status;         // see text Std 802.15.4e-2012 page 110
//};

// DSMESABSpecification field - Std 802.15.4e-2012 (figure 59k) page 109
//struct DSME_GTS_DSMESABSPEC
//{
//    UINT_8              dsmeSEBsubBlockLngth;
//    UINT_16             dsmeSABsubBlockindex;
//    std::vector<int>    dsmeSABsubBlock;        // DSME SEB sub-block (variable)
//};

// DSME Link Status Specification - Link status Descriptor - Std 802.15.4e-2012 (figure 59w) page 117
//struct DSME_LINKSTAT_SPEC_DESC_LINK
//{
//    UINT_8          linkStatChannel;
//    UINT_8          linkStatavgLQI;
//    UINT_8          linkStatavgRSSI;
//};

// DSME Link Status Specification - Std 802.15.4e-2012 (figure 59v) page 117
//struct DSME_LINKSTAT_SPEC_DESC
//{
//    UINT_8          linkStatDesCnt;
//    std::vector<DSME_LINKSTAT_SPEC_DESC_LINK>   linkStatList;     // Link status Descriptor List
//};

// AMCA Channel Probe Specification - Std 802.15.4e-2012 (figure 59dd) page 120
//struct AMCA_CHANNEL_PROB_SPEC
//{
//    Ieee802154eAMCAChannelProbSubtype   channelProbeSubType;
//    UINT_8          desListiningChannel;
//    UINT_8          probeChannel;
//};

struct MPFrameCtrl
{
    bool            lngFrameCtrl;               // Long Frame Control field
    // long frame control -  the other fields are in the normal frame control
    bool            presPANID;                  // PAN ID Present field
    bool            presIElist;                 // IEs List Present filed
};



// Frame Control Options - Std 802.15.4e-2012 (table 46) page 168
struct FrameCrlOptions
{
    bool            panIDsuppressed;            // PAN ID suppressed
    bool            iesIncluded;                // IEs included
    bool            seqSuppressed;              // SEQ # suppressed
};

// Superframe Specification field (16 bits) - Std 802.15.4-2011 (figure 41) page 62

struct HOPP_DESC
{
    UINT_8          HoppingSequenceID;
    UINT_16         HoppingSequenceLength;
    std::vector<UINT_16>    HoppingSequence;
    UINT_16         ChannelOffset;
    UINT_16         ChannelOffsetBitmapLength;
    std::vector<UINT_8> ChannelOffsetBitmap;
};
#endif /* IEEE802154EFIELD_H_ */

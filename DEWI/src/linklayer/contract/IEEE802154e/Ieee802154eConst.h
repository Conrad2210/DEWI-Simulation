#ifndef IEEE_802154_E_CONST_H
#define IEEE_802154_E_CONST_H

#include <stdint.h>
#include "Ieee802154eDef.h"

//--- PHY constants - Std 802.15.4-2011 (table 70) page 155 ---
// def in Ieee802154ePhy.h
const UINT_16 aMaxPHYPacketSize  = 127;      //max PSDU size (in bytes) the PHY shall be able to receive
//const UINT_16 aTurnaroundTime    = 12;       //Rx-to-Tx or Tx-to-Rx turnaround time (in symbol periods)

//---Frequency bands and data rates - Std 802.15.4-2011 (Table 66) page 147---
const UINT_16 BR_868M      = 20;       //20    kb/s    -- ch 0
const UINT_16 BR_915M      = 40;       //40    kb/s    -- ch 1,2,3,...,10
const UINT_16 BR_2_4G      = 250;      //250   kb/s    -- ch 11,12,13,...,26
const UINT_16 SR_868M      = 20;       //20    ks/s
const UINT_16 SR_915M      = 40;       //40    ks/s
const double SR_2_4G       = 62.5;     //62.5  ks/s

const double max_pDelay     = 100.0/200000000.0;    //maximum propagation delay
// PHY header
#define def_phyHeaderLength 6     // in byte

//--- PHY PIB attributes - Std 802.15.4-2011 (tables 71) page 156 ---
//All the default values are not given in the draft.
//They are chosen for sake of a value
const UINT_16 def_phyCurrentChannel         = 11;
const UINT_32 def_phyChannelsSupported      = 0x07ffffff;
//const UINT_8 def_phyTransmitPower         = 0                //dBm - Std 802.15.4-2006 page 266
//const UINT_8 def_phyTXPowerTolerance                      // Std 802.15.4-2011 (table 71) page 156 - Enumeration; range 1_dB, 3_dB, 6_dB
//const signed int def_phyTXPower                           // The transmit power of the device in dBm
const UINT_16 def_phyCCAMode                = 3;
const UINT_16 def_phyCurrentPage            = 0;
//const UINT_16 def_phyMaxFrameDuration                     // phySHRDuration250k + 3 × ceiling(1/3 × [1.5 + aMaxPHYPacketSize]) × phySymbolsPerOctet250k
//const UINT_16 def_phySHRDuration                          // Range: 3, 7, 10, 40; The duration of the synchronization header (SHR) in symbols for the current
//const float def_phySymbolsPerOctet                        // Range: 0.4, 1.3, 1.6, 2, 5.3, 8; For the CSS PHY, 1.3 corresponds to 1 Mb/s while 5.3 corresponds to 250 kb/s
//const std::vector<int> def_phyUWBDataRatesSupported
const bool def_phyCSSLowDataRateSupported   = false;        // Present for CSS PHY; no default
const bool def_phyUWBCoUSupported           = false;        // no default in the Std
const bool def_phyUWBCSSupported            = false;        // no default in the Std
const bool def_phyUWBLCPSupported           = false;        // no default in the Std
//const int def_phyUWBCurrentPulseShape
//const int def_phyUWBCoUpulse
//const int def_phyUWBCSpulse
//const signed int def_phyUWBLCPWeight1
//const signed int def_phyUWBLCPWeight2
//const signed int def_phyUWBLCPWeight3
//const signed int def_phyUWBLCPWeight4
//const int def_phyUWBLCPDelay2
//const int def_phyUWBLCPDelay3
//const int def_phyUWBLCPDelay4
const bool def_phyRanging                   = false;
const bool def_phyRangingCrystalOffset      = false;
const bool def_phyRangingDPS                = false;
const UINT_8 def_phyCurrentCode             = 0;
const UINT_8 def_phyNativePRF               = 0;
const UINT_8 def_phyUWBScanBinsPerChannel   = 0;
const UINT_8 def_phyUWBInsertedPreambleInterval = 0;
//const UINT_32 def_phyTXRMARKEROffset
//const UINT_32 def_phyRXRMARKEROffset
//const UINT_8 def_phyRFRAMEProcessingTime
//const UINT_16 def_phyCCADuration              // only for the 950 MHz band


// MAC Commands with fixed size
#define SIZE_OF_802154_ASSOCIATION_RESPONSE             29      // Fig 50: MHR (23) + Payload (4) + FCS (2)
#define SIZE_OF_802154_DISASSOCIATION_NOTIFICATION      21      // Fig 51: MHR (17) + Payload (2) + FCS (2)
#define SIZE_OF_802154_PANID_CONFLICT_NOTIFICATION      26      // Fig 53: MHR (23) + Payload (1) + FCS (2)
#define SIZE_OF_802154_ORPHAN_NOTIFICATION              20      // Fig 54: MHR (17) + Payload (1) + FCS (2)
#define SIZE_OF_802154_BEACON_REQUEST                   10      // Fig 55: MHR (7) + Payload (1) + FCS (2)
#define SIZE_OF_802154_GTS_REQUEST                      11      // Fig 57: MHR (7) + Payload (2) + FCS (2)
#define SIZE_OF_802154_ACK                              5       // Fig 46: MHR (3) + FCS (2)

//---MAC sublayer constants - Std 802.15.4-2011 (tables 51) page 125 ---
const UINT_16  aBaseSlotDuration            = 60;           //# of symbols comprising a superframe slot of order 0
const UINT_16  aNumSuperframeSlots          = 16;           //# of slots contained in a superframe
const UINT_16  aBaseSuperframeDuration      = aBaseSlotDuration * aNumSuperframeSlots;      //# of symbols comprising a superframe of order 0
const UINT_16  aGTSDescPersistenceTime      = 4;            //# of superframes that a GTS descriptor exists in the beacon frame of a PAN coordinator
// def in Ieee802154ePhy.h
const UINT_16  aMaxBeaconOverhead           = 75;           //max # of octets added by the MAC sublayer to the payload of its beacon frame
const UINT_16  aMaxBeaconPayloadLength      = aMaxPHYPacketSize - aMaxBeaconOverhead;       //max size, in octets, of a beacon payload
const UINT_16  aMaxLostBeacons              = 4;            //max # of consecutive beacons the MAC sublayer can miss w/o declaring a loss of synchronization
const UINT_16  aMaxMPDUUnsecuredOverhead    = 25;           //max # number of octets added by the MAC sublayer to the PSDU without security
const UINT_16  aMaxMACSafePayloadSize       = aMaxPHYPacketSize - aMaxMPDUUnsecuredOverhead;
const UINT_16  aMaxSIFSFrameSize            = 18;           //max size of a frame, in octets, that can be followed by a SIFS period
const UINT_16  aMinCAPLength                = 440;          //min # of symbols comprising the CAP
const UINT_16  aMinMPDUOverhead             = 9;            //min # number of octets added by the MAC sublayer to the PSDU
const UINT_16  aUnitBackoffPeriod           = 20;           // # of symbols comprising the basic time period used by the CSMA-CA algorithm

// TODO: Not defined in the Std 802.15.4-2011
const UINT_16  aMaxFrameResponseTime        = 1220;         //max # of symbols (or CAP symbols) to wait for a response frame
const UINT_16  aResponseWaitTime            = 32 * aBaseSuperframeDuration;             //max # of symbols a device shall wait for a response command following a request command

//---MAC PIB attributes - Std 802.15.4-2011 (table 52) page 126 ---
//#define def_macExtendedAddress                            // 64-bit (IEEE) address assigned to the device (device specific) - Implementation specific
#define def_macAckWaitDuration                  54          // 22(ack) + 20(backoff slot) + 12(turnaround); propagation delay ignored?; PHY dependent
#define def_macAssociatedPANCoord               false
#define def_macAssociationPermit                false
#define def_macAutoRequest                      true
#define def_macBattLifeExt                      false
#define def_macBattLifeExtPeriods               6           // PHY dependent
#define def_macBeaconPayload                    ""
#define def_macBeaconPayloadLength              0
#define def_macBeaconOrder                      15
#define def_macBeaconTxTime                     0x000000
//#define def_macBSN                            Random::random() % 0x100
#define def_macCoordExtendedAddress             0xffff
#define def_macCoordShortAddress                0xffff
//#define def_macDSN                            Random::random() % 0x100
#define def_macGTSPermit                        true
#define def_macMaxBE                            5           // range 3-8; except for LLDN mode = 3; max value of the backoff exponent in the CSMA-CA algorithm
#define def_macMaxCSMABackoffs                  4           // range 0-5; except for LLDN mode = 0
//#define def_macMaxFrameTotalWaitTime                      // PHY dependent
#define def_macMaxFrameRetries                  3           //max # range 0-7; of retries allowed after a transmission failures
#define def_macMinBE                            3
#define def_macLIFSPeriod                       40          //min # of symbols comprising a LIFS period; PHY dependent - see 8.1.3 Minimum LIFS and SIFS periods
#define def_macSIFSPeriod                       12          //min # of symbols comprising a SIFS period; PHY dependent - see 8.1.3 Minimum LIFS and SIFS periods
#define def_macPANId                            0xffff
#define def_macPromiscuousMode                  false
//#define def_macRangingSupported                           // Implementation specific
#define def_macResponseWaitTime                 32
#define def_macRxOnWhenIdle                     true        // for non-beacon or supporting direct transmission in beacon-enabled // default: FALSE
#define def_macSecurityEnabled                  false
//---MAC PIB attributes - Std 802.15.4e-2012 (table 52) page 171 ---
#define def_macEnhAckWaitDuration               0x0360      //max # (864 µs) max time (in μs) to wait for the PHY hader of an enhanced ACK frame to arrive following a transmitted data frame
#define def_macImplicitBroadcast                false
#define def_macSimpleAddress                    0xff

#define def_macShortAddress                     0xffff
#define def_macSuperframeOrder                  15
//#define def_macSyncSymbolOffset                           // Implementation specific
//#define def_macTimestampSupported                         // Implementation specific
#define def_macTransactionPersistenceTime       0x01f4
//#define def_macTxControlActiveDuration                    // PHY dependent
//#define def_macTxControlPauseDuration                     // PHY dependent
#define def_macTxTotalDuration                  0           // total transmit duration (including PHY header and FCS) specified in symbols

// Security-related MAC PIB attributes - Std 802.15.4-2011 (tables 60) page 142
//#define def_macKeyTable                                   //empty
//#define def_macDeviceTable                                //empty
//#define def_macSecurityLevelTable                         //empty
#define def_macFrameCounter                     0x00000000
#define def_macAutoRequestSecurityLevel         0x06
#define def_macAutoRequestKeyIdMode             0x00
//#define def_macAutoRequestKeySource                       // All octets 0xff
#define def_macAutoRequestKeyIndex              0xff
//#define def_macDefaultKeySource                           // All octets 0xff
//#define def_macPANCoordExtendedAddress                    // An extended IEEE address
#define def_macPANCoordShortAddress             0x0000
// Security-related MAC PIB attributes - Std 802.15.4e-2012 (tables 60) page 188
#define def_macFrameCounterMode                 0x04        // 0x04 = 4 octets; 0x05 = 5 octets

//attributes from Table 72 (security attributes) //TODO: No Idea where are the values stand in the Std 802.15.4-2011
#define def_macACLEntryDescriptorSet            NULL
#define def_macACLEntryDescriptorSetSize        0x00
#define def_macDefaultSecurity                  false
#define def_macACLDefaultSecurityMaterialLength 0x15
#define def_macDefaultSecurityMaterial          NULL
#define def_macDefaultSecuritySuite             0x00
#define def_macSecurityMode                     0x00

//---General MAC PIB attributes for functional organization - Std 802.15.4e-2012 (table 52a) page 173 ---
#define def_macTSCHcapable                      false        // Implementation specific
#define def_macLLcapable                        false        // Implementation specific
#define def_macDSMEcapable                      false        // Implementation specific
#define def_macLEcapable                        false        // Implementation specific
#define def_macRFIDcapable                      false        // Implementation specific
#define def_macHoppingCapable                   false        // Implementation specific
#define def_macAMCACapable                      false        // Implementation specific
#define def_macMetricsCapable                   false        // Implementation specific
#define def_macTSCHenabled                      false        // Implementation specific
#define def_macLLenabled                        false        // Implementation specific
#define def_macDSMEenabled                      false        // Implementation specific
#define def_macLEenabled                        false        // Implementation specific
#define def_macRFIDenabled                      false        // Implementation specific
#define def_macHoppingEnabled                   false        // Implementation specific
#define def_macAMCAenabled                      false        // Implementation specific
#define def_macMetricsEnabled                   false        // Implementation specific

//---TSCH-specific MAC PIB attributes - Std 802.15.4e-2012 (table 52b) page 174 ---
// for TSCH MODE: macMinBE = 1(default) and macMaxBE = 7(default)
#define def_macDisconnectTime                   0xff            // Timeslots to send out Disassociate frames before disconnecting
#define def_macJoinPriority                     1               // The lowest join priority from the TSCH Synchronization IE in an Enhanced beacon
#define def_macASN                              0x0000000000    // Absolute Slot Number
#define def_macNoHLBuffers                      false           // If FALSE, higher layer accept frame payload; if TRUE, higher layer cannot buffer it, NACK must be send



//---TSCH-MAC PIB attributes for macTimeslotTemplate - Std 802.15.4e-2012 (table 52b) page 174 ---
#define def_macTimeslotTemplateId               0
#define def_macTsCCAOffset                      1800
#define def_macTsCCA                            128
#define def_macTsTxOffset                       2120
#define def_macTsRxOffset                       1120
#define def_macTsRxAckDelay                     1000
#define def_macTsTxAckDelay                     1000
#define def_macTsRxWait                         2000
#define def_macTsAckWait                        400
#define def_macTsRxTx                           192
#define def_macTsMaxAck                         2400
#define def_macTsMaxTx                          4256
#define def_macTsTimeslotLength                 10000

//--- MAC PIB attributes for Hopping Sequence - Std 802.15.4e-2012 (figure 52f) page 177 ---
#define def_macHoppingSequenceID                0
//#define def_macChannelPage                    // no default
//#define def_macNumberOfChannels               // no default
//#define def_macPhyConfiguration               // no default
//#define def_macExtendedBitmap                 // no default
//#define def_macHoppingSequenceLength          // no default
//#define def_macHoppingSequenceList            // no default
//#define def_macCurrentHop                     // no default
//#define def_hopDwellTime                      // no default


//--- LLDN MAC PIB attributes - Std 802.15.4e-2012 (figure 52g) page 178 ---
#define def_macLLDNnumTimeSlots                 20
#define def_macLLDNnumUplinkTS                  20
#define def_macLLDNnumRetransmitTS              0
#define def_macLLDNnumBidirectionalTS           0
#define def_macLLDNmgmtTS                       false
#define def_macLLDNlowLatencyNWid               0xff
//#define def_macLLDNtimeSlotInfo                       // Implementation specific
#define def_macLLDNdiscoveryModeTimeout         256
#define def_macLLDNcoordinator                  false

//--- DSME-specific MAC PIB attributes - Std 802.15.4e-2012 (figure 52h) page 178 ---
//#define def_macChannelIndex                       // no default
//#define def_macAvgLQI                             // no default
//#define def_macAvgRSSI                            // no default
//#define def_macLinkStatusStatisticPeriod          // Implementation specific
#define def_macGACKFlag                         false
#define def_macCAPReductionFlag                 false
#define def_macChannelDiversityMode             0x00    // 0x00 = Channel Adaptation; 0x01 = Channel Hopping
#define def_macMultisuperframeOrder             15
#define def_macConnecDev                        false
#define def_macDSMESAB                          0   // Refer to Figure 59q and Figure 59r
#define def_macDSMEACT                          0   // table 1a
#define def_macSDindex                          0x0000
//#define def_macSDBitmap                           // Dependent on currently beacon allocation; Refer to 5.2.4.9.3
#define def_macChannelOffset                    0
#define def_macDeferredBeaconUsed               false
//#define def_macSyncParentExtendedAddress          // no default
#define def_macSyncParentShortAddress           0xffff
#define def_macSyncParentSDIndex                0
//#define def_macChannelStatus;                     // no default; table 44y
#define def_macBeaconSlotLength                 60
#define def_macDSMEGTSExpirationTime            7
//#define def_macChannelOffsetBitmapLength          // no default
//#define def_macChannelOffsetBitmap                // no default
//#define def_macPANCoordinatorBSN                  // no default
//#define def_macNeighborInformationTable         ""  // table 52i

//--- LE-specific MAC PIB attributes - Std 802.15.4e-2012 (figure 52j) page 181 ---
#define def_macCSLPeriod                        0
#define def_macCSLMaxPeriod                     0   // macCSLPeriod
#define def_macCSLChannelMask                   0x00000000
//#define def_macCSLFramePendingWaitT               // no default
#define def_macRITPeriod                        0
#define def_macRITDataWaitDuration              0
#define def_macRITTxWaitDuration                0
//#define def_macLowEnergySuperframeSupported       // Implementation specific
//#define def_macLowEnergySuperframeSyncInterval    // Implementation specific

//--- AMCA-specific MAC PIB attributes - Std 802.15.4e-2012 (figure 52k) page 182 ---
#define def_macDesignatedListeningChannel       0

// Metrics-specific MAC PIB attributes - Std 802.15.4e-2012 (figure 52l) page 183
//#define def_macCounterOctets                      // Implementation dependent
#define def_macRetryCount                       0
#define def_macMultipleRetryCount               0
#define def_macTXFailCount                      0
#define def_macTXSuccessCount                   0
#define def_macFCSErrorCount                    0
#define def_macSecurityFailure                  0
#define def_macDuplicateFrameCount              0
#define def_macRXSuccessCount                   0
#define def_macNACKcount                        0

// EBR-specific MAC PIB attributes - Std 802.15.4e-2012 (figure 52m) page 183
//#define def_EBRPermitJoining                      // no default
//#define def_macEBRFilters                         // no default
//#define def_macEBRLinkQuality                     // no default
//#define def_macEBRPercentFilter                   // no default
//#define def_macEBRattributeList                   // no default
#define def_macBeaconAutoRespond                true

// EB-specific MAC PIB attributes - Std 802.15.4e-2012 (figure 52n) page 185
#define def_macUseEnhancedBeacon                false    // no default
//#define def_macEBIEList                           // no default
#define def_macEBFilteringEnabled               false    // no default
//#define def_macEBSN                               // no default
#define def_macEBAutoSA                         2   // enum: FULL
//#define def_macEAckIElist                      ""


#endif

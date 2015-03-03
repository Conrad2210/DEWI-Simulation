/*
 * Ieee802154eEnum.h
 *
 *  Created on: 2 May 2014
 *      Author: R00118979
 */

#ifndef IEEE802154EENUM_H_
#define IEEE802154EENUM_H_


//TSCH LINK Type
typedef enum
{
    LNK_TP_NORMAL                  = 0,
    LNK_TP_ADVERTISING             = 1,
    LNK_TP_JOIN                    = 2,     // [CD] copied from the openWSN
    LNK_TP_undefinedType           = 3      // [CD]
}MACTSCHLinkType;

//TSCH LINK option
typedef enum
{
    LNK_OP_TRANSMIT             = 0,
    LNK_OP_RECEIVE              = 1,
    LNK_OP_SHARED               = 2,
    LNK_OP_RECEIVE_TIMEKEEPING  = 3,
    LNK_OP_SHARED_RECEIVE       = 4,
    LNK_OP_RESERVED             = 5,
    LNK_OP_undefinedOption      = 6     // [CD]

}MACTSCHLinkOptions;

// Phy-Mac primitive type
enum Ieee802154eMacPhyPrimitiveType
{
    PD_DATA_REQUEST             = 1,
    PD_DATA_CONFIRM,
    PD_DATA_INDICATION,
    PLME_CCA_REQUEST,
    PLME_CCA_CONFIRM,
    PLME_ED_REQUEST,
    PLME_ED_CONFIRM,
    PLME_GET_REQUEST,
    PLME_GET_CONFIRM,
    PLME_SET_TRX_STATE_REQUEST,
    PLME_SET_TRX_STATE_CONFIRM,
    PLME_SET_REQUEST,
    PLME_SET_CONFIRM,
    PLME_GET_BITRATE
};

//PHY enumerations description (Table 16) - Std 802.15.4-2006 (table 18)
typedef enum
{
    phy_BUSY = 1,
    phy_BUSY_RX = 2,
    phy_BUSY_TX = 3,
    phy_FORCE_TRX_OFF = 4,
    phy_IDLE = 5,
    phy_INVALID_PARAMETER = 6,
    phy_RX_ON = 7,
    phy_SUCCESS = 8,
    phy_TRX_OFF = 9,
    phy_TX_ON = 10,
    phy_UNSUPPORT_ATTRIBUTE = 11,
    phy_READ_ONLY = 12

} PHYenum;


// PHY PIB attributes
typedef enum
{
    PHY_INVALID_PARAMETER       = 0,    // for invalid parameter (not in the Std)
    PHY_CURRENT_CHANNEL         = 1,
    PHY_CHANNELS_SUPPORTED,
    PHY_TRANSMIT_POWER,                 // Std 802.15.4-2006 (table 23) page 46
    PHY_TX_POWER_TOLERANCE,
    PHY_TX_POWER,                       // Std 802.15.4-2011 (table 71) page 156
    PHY_CCA_MODE,
    PHY_CURRENT_PAGE,
    PHY_MAX_FRAME_DURATION,
    PHY_SHR_DURATION,
    PHY_SYMBOLS_PER_OCTET,
    PHY_PREAMBLE_SYMBOL_LENGTH,
    PHY_UWB_DATA_RATES_SUPPORTED,
    PHY_CSS_LOW_DATA_RATE_SUPPORTED,
    PHY_UWB_COU_SUPPORTED,
    PHY_UWB_CS_SUPPORTED,
    PHY_UWB_LCP_SUPPORTED,
    PHY_UWB_CURRENT_PULSE_SHAPE,
    PHY_UWB_COU_PULSE,
    PHY_UWB_CS_PULSE,
    PHY_UWB_LCP_WEIGHT_1,
    PHY_UWB_LCP_WEIGHT_2,
    PHY_UWB_LCP_WEIGHT_3,
    PHY_UWB_LCP_WEIGHT_4,
    PHY_UWB_LCP_DELAY_2,
    PHY_UWB_LCP_DELAY_3,
    PHY_UWB_LCP_DELAY_4,
    PHY_RANGING,
    PHY_RANGING_CRYSTAL_OFFSET,
    PHY_RANGING_DPS,
    PHY_CURRENT_CODE,
    PHY_NATIVE_PRF,
    PHY_UWB_SCAN_BINS_PER_CHANNEL,
    PHY_UWB_INSERTED_PREAMBLE_INTERVAL,
    PHY_TX_RMARKER_OFFSET,
    PHY_RX_RMARKER_OFFSET,
    PHY_RFRAME_PROCESSING_TIME,
    PHY_CCA_DURATION

} PHYPIBenum;

enum Ieee802154ePhyTimerType
{
    PHY_CCA_TIMER,
    PHY_ED_TIMER,
    PHY_TRX_TIMER,
    PHY_TX_OVER_TIMER,
    PHY_RX_OVER_TIMER               // dynamic timer
};

// packet error type
enum Ieee802154ePktErrorType   // for msgKind set by PHY layer
{
    PACKETOK=0,
    BITERROR_FORCE_TRX_OFF,
    RX_DURING_CCA                   // pkts received during CCA need to be discarded
};

typedef enum {
    TX_OVER = 0

} additionalData;

/********************************/
/****** MAC Layer Enumeration ******/
/*******************************/

// MAC enumerations description - Std 802.15.4-2006 (table 78) page 135 - Std 802.15.4-2011 (table 6) page 70 - Std 802.15.4e-2012 (table 6) page 97
typedef enum
{
    mac_SUCCESS                     = 0x00,

    //--- Std 802.15.4-2006 (table 83)  ---
    // Association Status - Std 802.15.4-2011 (table 6) page 70 or Std 802.15.4e-2012 (table 6) page 97
    mac_PAN_at_capacity             = 0x01,
    mac_PAN_access_denied           = 0x02,

    // Std 802.15.4e-2012 (table 6) page 97
    mac_Hopping_Seq_Offset_dupli    = 0x03, // Hopping Sequence offset duplication
    mac_FastA_successful            = 0x80,
    //--------------------------------

    mac_BEACON_LOSS                 = 0xe0,
    mac_CHANNEL_ACCESS_FAILURE      = 0xe1,
    mac_COUNTER_ERROR               = 0xdb,
    mac_DENIED                      = 0xe2,
    mac_DISABLE_TRX_FAILURE         = 0xe3,
    mac_FAILED_SECURITY_CHECK       = 0xe4,            // not in the Std [not from us]
    mac_FRAME_TOO_LONG              = 0xe5,
    mac_IMPROPER_KEY_TYPE           = 0xdc,
    mac_IMPROPER_SECURITY_LEVEL     = 0xdd,
    mac_INVALID_ADDRESS             = 0xf5,
    mac_INVALID_GTS                 = 0xe6,
    mac_INVALID_HANDLE              = 0xe7,
    mac_INVALID_INDEX               = 0xf9,
    mac_INVALID_PARAMETER           = 0xe8,
    mac_LIMIT_REACHED               = 0xfa,
    mac_NO_ACK                      = 0xe9,
    mac_NO_BEACON                   = 0xea,
    mac_NO_DATA                     = 0xeb,
    mac_NO_SHORT_ADDRESS            = 0xec,
    mac_OUT_OF_CAP                  = 0xed,
    mac_PAN_ID_CONFLICT             = 0xee,
    mac_PAST_TIME                   = 0xf7,
    mac_READ_ONLY                   = 0xfb,
    mac_REALIGNMENT                 = 0xef,
    mac_SCAN_IN_PROGRESS            = 0xfc,
    mac_SECURITY_ERROR              = 0xe4,
    mac_SUPERFRAME_OVERLAP          = 0xfd,
    mac_TRACKING_OFF                = 0xf8,
    mac_TRANSACTION_EXPIRED         = 0xf0,
    mac_TRANSACTION_OVERFLOW        = 0xf1,
    mac_TX_ACTIVE                   = 0xf2,
    mac_UNAVAILABLE_KEY             = 0xf3,
    mac_UNSUPPORTED_ATTRIBUTE       = 0xf4,
    mac_UNSUPPORTED_LEGACY          = 0xde,
    mac_UNSUPPORTED_SECURITY        = 0xdf,
    mac_UNSUPPORTED_FEATURE         = 0x81,     // value not defined in the Std 802.15.4e-2012
    mac_BAD_CHANNEL                 = 0x82,     // value not defined in the Std 802.15.4e-2012
    mac_SLOTFRAME_NOT_FOUND         = 0x83,     // MLME-SET-SLOTFRAME.confirm - Std 802.15.4e-2012 (table 44e) page 145
    mac_MAX_SLOTFRAMES_EXCEEDED     = 0x84,     // MLME-SET-SLOTFRAME.confirm - Std 802.15.4e-2012 (table 44e) page 145
    mac_UNKNOWN_LINK                = 0x85,     // MLME-SET-LINK.confirm - Std 802.15.4e-2012 (table 44g) page 147
    mac_MAX_LINKS_EXCEEDED          = 0x86,     // MLME-SET-LINK.confirm - Std 802.15.4e-2012 (table 44g) page 147
    mac_NO_SYNC                     = 0x87,     // MLME-TSCH-MODE.confirm - Std 802.15.4e-2012 (table 44i) page 148
    mac_RANGING_NOT_SUPPORTED       = 0x88,     // MLME-RX-ENABLE.confirm - Std 802.15.4-2011 (table 29) page 99
    mac_BEACON_LOST                 = 0x89,     // MLME-SYNC-Loss.indication - Std 802.15.4-2011 (table 37) page 109
    mac_DPS_NOT_SUPPORTED           = 0x8a,     // MLME-DPS.confirm - Std 802.15.4-2011 (table 41) page 113
    mac_SOUNDING_NOT_SUPPORTED      = 0x8b,     // MLME-SOUNDING.confirm - Std 802.15.4-2011 (table 42) page 114
    mac_COMPUTATION_NEEDED          = 0x8c,     // MLME-CALIBRATE.confirm - Std 802.15.4-2011 (table 44) page 115
    mac_ACK_RCVD_NODSN_NOSA         = 0x8d,      // MCPS-DATA.confirm - Std 802.15.4e-2012 (table 57) page 170
    mac_LINK_EXISTS		    = 0x8e

} MACenum;

//MAC PIB attributes - Std 802.15.4-2011 (table 52) page 126 - Std 802.15.4-2006 (table 86)
typedef enum
{
    MAC_EXTENDED_ADDRESS,
    MAC_ACK_WAIT_DURATION,
    MAC_ASSOCIATED_PAN_COORD,
    MAC_ASSOCIATION_PERMIT,
    MAC_AUTO_REQUEST,
    MAC_BATT_LIFE_EXT,
    MAC_BATT_LIFE_EXT_PERIODS,
    MAC_BEACON_PAYLOAD,
    MAC_BEACON_PAYLOAD_LENGTH,
    MAC_BEACON_ORDER,
    MAC_BEACON_TX_TIME,
    MAC_BSN,
    MAC_COORD_EXTENDED_ADDRESS,
    MAC_COORD_SHORT_ADDRESS,
    MAC_DSN,
    MAC_GTS_PERMIT,
    MAC_MAX_BE,
    MAC_MAX_CSMA_BACKOFFS,
    MAC_MAX_FRAME_RETRIES,
    MAC_MIN_BE,
    MAC_LIFS_PERIOD,
    MAC_SIFS_PERIOD,
    MAC_PAN_ID,
    MAC_PROMISCUOUS_MODE,
    MAC_RESPONSE_WAIT_TIME,
    MAC_RX_ON_WHEN_IDLE,
    MAC_SECURITY_ENABLED,
    MAC_ENH_ACK_WAIT_DURATION,
    MAC_IMPLICIT_BROADCAST,
    MAC_SIMPLE_ADDRESS,
    MAC_SHORT_ADDRESS,
    MAC_SUPERFRAME_ORDER,
    MAC_SYNC_SYMBOL_OFFSET,
    MAC_TIMESTAMP_SUPPORTED,
    MAC_TRANSACTION_PERSISTENCE_TIME,
    MAC_TX_CONTROL_ACTIVE_DURATION,
    MAC_TX_CONTROL_PAUSE_DURATION,
    MAC_TX_TOTAL_DURATION,
    // Security-related MAC PIB attributes - Std 802.15.4-2011 (table 60) page 142
    //MAC_KEY_TABLE,
    //MAC_DEVICE_TABLE,
    //MAC_SECURITY_LEVEL_TABLE,
    MAC_FRAME_COUNTER,
    MAC_AUTO_REQUEST_SECURITY_LEVEL,
    MAC_AUTO_REQUEST_KEY_ID_MODE,
    //MAC_AUTO_REQUEST_KEY_SOURCE,
    MAC_AUTO_REQUEST_KEY_INDEX,
    //MAC_DEFAULT_KEY_SOURCE,
    MAC_PAN_COORD_EXTENDED_ADDRESS,
    MAC_PAN_COORD_SHORT_ADDRESS,

    // Security-related MAC PIB attributes - Std 802.15.4e-2012 (table 60) page 188
    MAC_FRAME_COUNTER_MODE,
    // General MAC PIB attributes for functional organization - Std 802.15.4e-2012 (table 52a) page 173
    MAC_TSCH_CAPABLE,
    MAC_LL_CAPABLE,
    MAC_DSME_CAPABLE,
    MAC_LE_CAPABLE,
    MAC_RFID_CAPABLE,
    MAC_HOPPING_CAPABLE,
    MAC_AMCA_CAPABLE,
    MAC_METRICS_CAPABLE,
    MAC_TSCH_ENABLED,
    MAC_LL_ENABLED,
    MAC_DSME_ENABLED,
    MAC_LE_ENABLED,
    MAC_RFID_ENABLED,
    MAC_HOPPING_ENABLED,
    MAC_AMCA_ENABLED,
    MAC_METRCIS_ENABLED,
    // TSCH-specific MAC PIB attributes - Std 802.15.4e-2012 (table 52b) page 174
    MAC_DISCONNECT_TIME,
    MAC_JOIN_PRIORITY,
    MAC_ASN,
    MAC_NO_HL_BUFFERS

} MACPIBenum;

// TSCH-MAC PIB attributes for macLinkTable - macLinkType - Std 802.15.4e-2012 (table 52d) page 175


// EB-specific MAC PIB attributes- macEAckIElist - Std 802.15.4e-2012 (table 52n) page 185
typedef enum
{
    EAckIE_LST_NONE                    = 0,
    EAckIE_LST_SHORT                   = 1,
    EAckIE_LST_FULL                    = 2

} MACEAckIElist;

// TSCH Slotframe - Operation type - Std 802.15.4e-2012 (table 44d) page 144
typedef enum
{
    SF_OP_ADD                     = 0,
    SF_OP_DELETE                  = 1,
    SF_OP_MODIFY                  = 2
} Ieee802154eSlotframeOperation;

// TSCH Slotframe - Status type - Std 802.15.4e-2012 (table 44e) page 145
typedef enum
{
    SF_STATUS_SUCCESS                 = 0,
    SF_STATUS_INVALID_PARAMETER       = 1,
    SF_STATUS_SLOTFRAME_NOT_FOUND     = 2,
    SF_STATUS_MAX_SLOTFRAMES_EXCEEDED = 3
} Ieee802154eSlotframeStatus;

// TSCH Set Link - Operation type - Std 802.15.4e-2012 (table 44d) page 144
typedef enum
{
    LNK_OP_ADD_LINK                = 0,
    LNK_OP_DELETE_LINK             = 1,
    LNK_OP_MODIFY_MODIFY_LINK      = 2
} Ieee802154eLinkOperation;

// TSCH Set Link - Link type - Std 802.15.4e-2012 (table 44d) page 144
//typedef enum
//{
//    LNK_TP_NORMAL                  = 0,
//    LNK_TP_ADVERTISING             = 1
//} Ieee802154eLinkType;

// TSCH Link - Status type - Std 802.15.4e-2012 (table 44g) page 147
typedef enum
{
    LNK_STATUS_SUCCESS                 = 0,
    LNK_STATUS_INVALID_PARAMETER       = 1,
    LNK_STATUS_UNKNOWN_LINK            = 2,
    LNK_STATUS_MAX_LINKS_EXCEEDED      = 3
} Ieee802154eLinkStatus;

// TSCH Mode - TSCHMode type - Std 802.15.4e-2012 (table 44h) page 147
typedef enum
{
    TSCH_MODE_OFF                     = 0,
    TSCH_MODE_ON                      = 1
} Ieee802154eTSCHMode;

// TSCH Mode - TSCHMode Status type - Std 802.15.4e-2012 (table 44i) page 148
typedef enum
{
    TSCH_MODE_STATUS_SUCCESS                 = 0,
    TSCH_MODE_STATUS_NO_SYNC                 = 1
} Ieee802154eTSCHModeStatus;

// TSCH KEEP ALIVE - Status type - Std 802.15.4e-2012 (table 44i) page 148
typedef enum
{
    KA_STATUS_SUCCESS                 = 0,
    KA_STATUS_INVALID_PARAMETER       = 1
} Ieee802154eKeepAliveStatus;

// MAC frame type - Table 79 - Std 802.15.4e-2012 (table 2) page 60
typedef enum
{
    //Ieee802154_UNDEFINED_FRM  = 0,
    Ieee802154e_BEACON       = 1,       // MAC Beacon
    Ieee802154e_DATA         = 2,       // MAC Data
    Ieee802154e_ACK          = 3,       // MAC ACK
    Ieee802154e_CMD          = 4,       // MAC command
    Ieee802154e_LLDN         = 5,       // LLDN (Low Latency Deterministic Network)
    Ieee802154e_MULTI        = 6,        // Multipurpose
    Ieee802154e_ASSOCIATION_REQUEST = 7,  //ASSOCIATION_REQUEST
    Ieee802154e_ASSOCIATION_RESPONCE = 8,
    Ieee802154e_SCHEDULER_REQUEST = 9,
    Ieee802154e_SCHEDULER_RESPONCE = 10,

} Ieee802154eFrameType;

// MAC command type - Table 67 - Std 802.15.4e-2012 (table 5) page 95
typedef enum
{
    ASSOCIATION_REQUEST             = 1,
    ASSOCIATION_RESPONSE,
    DISASSOCIATION_NOTIFICATION,
    DATA_REQUEST,
    PAN_ID_CONFLICT_NOTIFICATION,
    ORPHAN_NOTIFICATION,
    BEACON_REQUEST,
    COORDINATOR_REALIGNMENT,
    GTS_REQUEST,
    START_REQUEST,
    START_CONFIRM,

    // Std 802.15.4e-2012 (table 5) page 95
    LL_DISCOVERY_RESPONSE,
    LL_CONFIG_STATUS,
    LL_CONFIG_REQUEST,
    LL_CTS_SHARED_GROUP,
    LL_RTS,                         // Request To Send
    LL_CTS,                         // Clear To Send
    DSME_ASSOCIATION_REQUEST,
    DSME_ASSOCIATION_RESPONSE,
    DSME_GTS_REQUEST,
    DSME_GTS_REPLY,
    DSME_GTS_NOTIFY,
    DSME_INFO_REQUEST,
    DSME_INFO_REPLEY,
    DSME_BEACON_ALLOCATION_NOTIF,
    DSME_BEACON_COLLISION_NOTIF,
    DSME_LINK_STATUS_REPORT,
    AMCE_BEACON_REQUEST,
    AMCE_HELLO,
    AMCE_CHANNEL_PROBE,
    LE_RIT_DATA_REQUEST

} Ieee802154eMacCmdType;

// Pkt tx options
//typedef enum
//{
//    //UNDEFINED         = 0,
//    DIRECT_TRANS        = 1,
//    INDIRECT_TRANS      = 2,
//    GTS_TRANS           = 3
//
//} Ieee802154TxOption;

// MAC timer type
enum Ieee802154eMacTimerType
{
    START_PAN_COOR_TIMER,       // dynamic timer
    START_TIMER,
    ASSOCIATION_TIMER,
    ASSOCIATION_WAIT_TIMER,
    SCHEDULE_TIMER,
    SCHEDULE_WAIT_TIMER,
    MAC_SCAN_TIMER,
    MAC_BACKOFF_TIMER,
    MAC_DEFER_CCA_TIMER,
    MAC_BCN_RX_TIMER,
    MAC_BCN_TX_TIMER,
    //MAC_TX_OVER_TIMER,
    MAC_ACK_TIMEOUT_TIMER,
    MAC_TX_ACK_BOUND_TIMER,
    MAC_TX_CMD_DATA_BOUND_TIMER,
    MAC_IFS_TIMER,
    MAC_TX_SD_TIMER,
    MAC_RX_SD_TIMER,
    MAC_FINAL_CAP_TIMER,
    MAC_GTS_TIMER,
    // for TSCH Std 802.15.4e-2012
    MAC_ASN_TIMER,
    // TSCH transmission
    MAC_TS_CCA_OFFSET_TIMER,
    MAC_TS_TX_OFFSET_TIMER,
    MAC_TS_RX_ACK_DELAY_TIMER,
    MAC_TS_ACK_WAIT_TIMER,
    // TSCH receiving
    MAC_TS_RX_OFFSET_TIMER,
    MAC_TS_RX_WAIT_TIMER,
    MAC_TS_TX_ACK_DELAY_TIMER,
    MAC_TS_MAX_ACK_TIMER
};

typedef enum
{
    //UNDEFINED         = 0,
    DIRECT_TRANS        = 1,
    INDIRECT_TRANS      = 2,
    GTS_TRANS           = 3

} Ieee802154eTxOption;


// Channel scan type - table 53
//typedef enum
//{
//    //UNDEFINED         = 0,
//    ED_SCAN             = 1,
//    ACTIVE_SCAN         = 2,
//    PASSIVE_SCAN        = 3,
//    ORPHAN_SCAN         = 4
//
//} Ieee802154eChannelScanType;

// MAC task pending type
typedef enum
{

    TP_MCPS_DATA_REQUEST           = 1,
    TP_MCPS_DATA_CONFIRM,
    TP_MCPS_DATA_INDICATION,
    TP_MCPS_PURGE_REQUEST,
    TP_MCPS_PURGE_CONFIRM,
    TP_MLME_ASSOCIATE_REQUEST,
    TP_MLME_ASSOCIATE_INDICATION,
    TP_MLME_ASSOCIATE_RESPONSE,
    TP_MLME_ASSOCIATE_CONFIRM,
    TP_MLME_DISASSOCIATE_REQUEST,
    TP_MLME_DISASSOCIATE_INDICATION,
    TP_MLME_DISASSOCIATE_CONFIRM,
    TP_MLME_BEACON_NOTIFY_INDICATION,
    TP_MLME_COMM_STATUS_INDICATION,
    TP_MLME_GET_REQUEST,
    TP_MLME_GET_CONFIRM,
    TP_MLME_GTS_REQUEST,
    TP_MLME_GTS_CONFIRM,
    TP_MLME_GTS_INDICATION,
    TP_MLME_RX_ENABLE,
    TP_MLME_ORPHAN_INDICATION,
    TP_MLME_ORPHAN_RESPONSE,
    TP_MLME_RESET_REQUEST,
    TP_MLME_RESET_CONFIRM,
    TP_MLME_RX_ENABLE_REQUEST,
    TP_MLME_RX_ENABLE_CONFIRM,
    TP_MLME_SCAN_REQUEST,
    TP_MLME_SCAN_CONFIRM,
    TP_MLME_SET_REQUEST,
    TP_MLME_SET_CONFIRM,
    TP_MLME_START_REQUEST,
    TP_MLME_START_CONFIRM,
    TP_MLME_SYNC_REQUEST,
    TP_MLME_SYNC_LOSS_INDICATION,
    TP_MLME_POLL_REQUEST,
    TP_MLME_POLL_CONFIRM,
    TP_MLME_DPS_REQUEST,
    TP_MLME_DPS_INDICATION,
    TP_MLME_DPS_CONFIRM,
    TP_MLME_SOUNDING_REQUEST,
    TP_MLME_SOUNDING_CONFIRM,
    TP_MLME_CALIBRATE_REQUEST,  // UWB, not supported
    TP_MLME_CALIBRATE_CONFIRM,  // UWB, not supported
    // Std 802.15.4e-2012 (6.2.18.1) page 140
    TP_MLME_BEACON_REQUEST,
    TP_MLME_BEACON_CONFIRM,
    TP_MLME_BEACON_REQUEST_INDICATION,

    // TSCH MAC management service - Std 802.15.4e-2012 (table 8a) page 123
    TP_MLME_SET_SLOTFRAME_REQUEST,
    TP_MLME_SET_SLOTFRAME_CONFIRM,
    TP_MLME_SET_LINK_REQUEST,
    TP_MLME_SET_LINK_CONFIRM,
    TP_MLME_TSCH_MODE_REQUEST,
    TP_MLME_TSCH_MODE_CONFIRM,
    TP_MLME_KEEP_ALIVE_REQUEST,
    TP_MLME_KEEP_ALIVE_CONFIRM,
    TP_MLME_SET_BEACON_REQUEST,
    TP_MLME_SET_BEACON_CONFIRM,
    // LL-MAC management service - Std 802.15.4e-2012 (table 8b) page 123
    TP_MLME_LLDN_DISCOVERY_REQUEST,
    TP_MLME_LLDN_DISCOVERY_CONFIRM,
    TP_MLME_LLDN_CONFIGURATION_REQUEST,
    TP_MLME_LLDN_CONFIGURATION_CONFIRM,
    TP_MLME_LLDN_ONLINE_REQUEST,
    TP_MLME_LLDN_ONLINE_INDICATION,
    // DSME MAC management service - Std 802.15.4e-2012 (table 8c) page 124
    TP_MLME_DSME_GTS_REQUEST,
    TP_MLME_DSME_GTS_INDICATION,
    TP_MLME_DSME_GTS_RESPONSE,
    TP_MLME_DSME_GTS_CONFIRM,
    TP_MLME_DSME_INFO_REQUEST,
    TP_MLME_DSME_INFO_INDICATION,
    TP_MLME_DSME_INFO_CONFIRM,
    TP_MLME_DSME_LINKSTATUSRPT_REQUEST,
    TP_MLME_DSME_LINKSTATUSRPT_INDICATION,
    TP_MLME_DSME_LINKSTATUSRPT_CONFIRM,

    // for CSMA-CCA
    TP_CCA_CSMACA,
    TP_RX_ON_CSMACA,
    // for TSCH CCA - Transmitter
    TP_TS_TX_CCA_TSCHCCA,
    // for TSCH CCA - Receiver
    TP_TS_RX_CCA_TSCHCCA,
    TP_SCHEDULE_REQUEST,
    TP_SCHEDULE_INDICATION,
    TP_SCHEDULE_RESPONSE,
    TP_SCHEDULE_CONFIRM

} Ieee802154eMacTaskType;

// Auxiliary security header - Security Control field - Security Level field
typedef enum
{
    ASH_SLVL_NONE           = 0x00,
    ASH_SLVL_MIC_32         = 0x01,
    ASH_SLVL_MIC_64         = 0x02,
    ASH_SLVL_MIC_128        = 0x03,
    ASH_SLVL_ENC            = 0x04,
    ASH_SLVL_ENC_MIC_32     = 0x05,
    ASH_SLVL_ENC_MIC_64     = 0x06,
    ASH_SLVL_ENC_MIC_128    = 0x07

} Ieee802154eASHSeclvlType;

// Disassociation Reason - disassociation reason codes - Std 802.15.4-2011 (table 7) page 71
typedef enum
{
    DNC_COORD_W_DEVICE_L_PAN    = 0x01,
    DNC_DEVICE_W_L_PAN          = 0x02
} Ieee802154eDNCType;


// LLDN frame - Frame Subtype - Std 802.15.4e-2012 (table 3c)
//typedef enum
//{
//    LLDN_FSUB_LL_BEACON         = 0x01,
//    LLDN_FSUB_LL_DATA           = 0x02,
//    LLDN_FSUB_LL_ACK            = 0x03,
//    LLDN_FSUB_LL_MAC_COMMAND    = 0x04
//
//} Ieee802154eLLDNFrameSubtype;

// LLDN beacon frame - flags - Transmission State - Std 802.15.4e-2012 (table 3d)
//typedef enum
//{
//    LLDN_BEACON_FLGS_TS_ONLINE         = 0x01,
//    LLDN_BEACON_FLGS_TS_DISCOVERY      = 0x02,
//    LLDN_BEACON_FLGS_TS_CONFIGURATION  = 0x03,
//    LLDN_BEACON_FLGS_TS_STATE_RESET    = 0x04
//
//} Ieee802154eLLDNBeaconFlgsTStype;

// LLDN ACK frame - ACK type - Std 802.15.4e-2012 (table 3d)
//typedef enum
//{
//    LLDN_ACK_CONFIGURATION      = 0x01,     // no ACK payload
//    LLDN_ACK_DATA               = 0x02,     // no ACK payload
//    LLDN_ACK_DATA_GROUP_ACK     = 0x03,     // ACK payload
//    LLDN_ACK_DISCOVERY_RESPONSE = 0x04      // no ACK payload
//
//} Ieee802154eLLDNACKtype;

// Information Element Header - Std 802.15.4e-2012 (table 4b)
typedef enum
{
    IE_HD_LE_CSL                    = 0x1a,
    IE_HD_LE_RIT                    = 0x1b,
    IE_HD_DSME_PAN_Descriptor       = 0x1c,
    IE_HD_RZ_TIME                   = 0x1d,
    IE_HD_ACK_NACK_TIME_CORRECTION  = 0x1e,
    IE_HD_GROUP_ACK                 = 0x1f,
    IE_HD_LOW_LATENCY_NETWORK_INFO  = 0x20,
    IE_HD_LIST_TERMINATION_1        = 0x7e,
    IE_HD_LIST_TERMINATION_2        = 0x7f

} Ieee802154eIEHeaderType;

// Information Element Header - Std 802.15.4e-2012 (table 4c)
typedef enum
{
    IE_PL_ESDU              = 0x00, // Encapsulated Service Data Unit
    IE_PL_MLME              = 0x01, // MLME (Nested)
    IE_PL_LIST_TERMINATION  = 0x0f

} Ieee802154eIEPayloadType;

// MLME Information Element - short and long format - Std 802.15.4e-2012 (table 4d & 4e)
typedef enum
{
    // short format
    IE_MLME_SHORT_TSCH_SYNCHRONISATION      = 0x1a,
    IE_MLME_SHORT_TSCH_SLOTFRAME_LINK       = 0x1b,
    IE_MLME_SHORT_TSCH_TIMESLOT             = 0x1c,
    IE_MLME_SHORT_HOPPING_TIMING            = 0x1d,
    IE_MLME_SHORT_EB_FILTER                 = 0x1e,
    IE_MLME_SHORT_MAC_METRICS_1             = 0x1f,
    IE_MLME_SHORT_MAC_METRICS_2             = 0x20,
    // long format
    IE_MLME_LONG_CHANNEL_HOPPING_SEQUENCE   = 0x09

} Ieee802154eIEMLMESubtype;

// MAC Metrics Information Element - Std 802.15.4e-2012 (table 4i) page 94
typedef enum
{
    IE_MACM_macCounterOctets                = 0x01,
    IE_MACM_macRetryCount                   = 0x02,
    IE_MACM_macMultipleRetryCount           = 0x03,
    IE_MACM_macTXFailCount                  = 0x04,
    IE_MACM_macTXSuccessCount               = 0x05,
    IE_MACM_macFCSErrorCount                = 0x06,
    IE_MACM_macSecurityFailure              = 0x07,
    IE_MACM_macDuplicateFrameCount          = 0x08,
    IE_MACM_macRXSuccessCount               = 0x09,
    IE_MACM_macNACKcount                    = 0x0a

} Ieee802154eIEMACMtype;

// DSME-GTS Management Type - Std 802.15.4e-2012 (table 7a) page 108
//typedef enum
//{
//    DSME_GTS_MGMT_DEALLOCATION              = 0x00,
//    DSME_GTS_MGMT_ALLOCATION                = 0x01,
//    DSME_GTS_MGMT_DUPLI_ALLOCATION_NOTIF    = 0x02,
//    DSME_GTS_MGMT_REDUCE                    = 0x03,
//    DSME_GTS_MGMT_RESTART                   = 0x04,
//    DSME_GTS_MGMT_DSME_GTS_EXPIRATION       = 0x05
//
//} Ieee802154eDSMEGTSMgmttype;

// AMCA Channel Probe Subtype - Std 802.15.4e-2012 (table 7b) page 120
//typedef enum
//{
//    AMCA_CP_SUBT_REQUEST                    = 0x00,
//    AMCA_CP_SUBT_REPLEY                     = 0x01,
//    AMCE_CP_SUBT_PROBE                      = 0x02
//
//} Ieee802154eAMCAChannelProbSubtype;

/** @brief The different back-off methods.*/
enum backoff_methods
{
    /** @brief Constant back-off time.*/
    CONSTANT = 0,
    /** @brief Linear increasing back-off time.*/
    LINEAR,
    /** @brief Exponentially increasing back-off time.*/
    EXPONENTIAL,
};

// Frame Version field - Std 802.15.4e-2012 (table 3a) page 62
typedef enum
{
    Ieee802154_2003_compatible  = 0x00,
    Ieee802154_2006_introduced  = 0x01,
    Ieee802154_compatible       = 0x02

} Ieee802154eFrameVersion;

// MAC timer type
typedef enum
{
    AddrMode_NO_ADRESS          = 0x00,
    AddrMode_SIMPLE_ADDRESS     = 0x01, // Std 802.15.4e-2012 (table 46) page 168
    AddrMode_SHORT_ADDRESS      = 0x02,
    AddrMode_EXTENDED_ADDRESS   = 0x03

} Ieee802154eAddrMode;

//MAC PIB attributes - Std 802.15.4-2006 (table 86) page 160 - Std 802.15.4-2011 (table 52) page 126 - Std 802.15.4e-2012 (table 52) page 171
// UWBPRF Type - Std 802.15.4-2011 (table 46) page 117
typedef enum
{
    UWBPRF_PRF_OFF          = 0,
    UWBPRF_NOMINAL_4_M      = 1,
    UWBPRF_NOMINAL_16_M     = 2,
    UWBPRF_NOMINAL_64_M     = 3

} Ieee802154eUWBFType;


// RangingControl - Std 802.15.4-2011 (table 28) page 98 [0-1] & (table 46) page 117 [2-4] & (table 48) page 123 [5-7]
typedef enum
{
    RANGING_OFF                         = 0,
    RANGING_ON                          = 1,
    NON_RANGING                         = 2,
    ALL_RANGING                         = 3,
    PHY_HEADER_ONLY                     = 4,
    NO_RANGING_REQUESTED                = 5,
    RANGING_ACTIVE                      = 6,
    RANGING_REQUESTED_BUT_NOT_SUPPORTED = 7

} RangingControl;

//typedef enum
//{
//    LNK_OP_TRANSMIT             = 0,
//    LNK_OP_RECEIVE              = 1,
//    LNK_OP_SHARED               = 2,
//    LNK_OP_RECEIVE_TIMEKEEPING  = 3,
//    LNK_OP_SHARED_RECEIVE       = 4,
//    LNK_OP_RESERVED             = 5,
//    LNK_OP_undefinedOption      = 6     // [CD]
//
//} MACTSCHLinkOptions;

typedef enum
{
    BeaconType_BEACON           = 0,
    BeaconType_ENHANCED_BEACON  = 1

} Ieee802154eBeaconType;

typedef enum
{
    OP_ADD                      = 0,
    OP_DELETE                   = 1,
    OP_MODIFY                   = 2,
    OP_ADD_LINK                 = 3,
    OP_DELETE_LINK              = 4,
    OP_MODIFY_LINK              = 5

} Ieee802154eOperation;

#endif /* IEEE802154EENUM_H_ */


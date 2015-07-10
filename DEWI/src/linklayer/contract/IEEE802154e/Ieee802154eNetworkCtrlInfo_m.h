//
// Generated file, do not edit! Created by nedtool 4.6 from src/linklayer/contract/IEEE802154e/Ieee802154eNetworkCtrlInfo.msg.
//

#ifndef _IEEE802154ENETWORKCTRLINFO_M_H_
#define _IEEE802154ENETWORKCTRLINFO_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
    using namespace std;
    #include "Ieee802154eField.h"
    #include "Ieee802154eEnum.h"
// }}

/**
 * Class generated from <tt>src/linklayer/contract/IEEE802154e/Ieee802154eNetworkCtrlInfo.msg:15</tt> by nedtool.
 * <pre>
 * message Ieee802154eNetworkCtrlInfo
 * {
 *     // MAC layer is responsible for MAC address translation
 *     bool toParent;
 *     string destName;
 *     int64_t netwAddr = -1;
 * 
 *     // for MLME_ASSOCIATE.request - Std 802.15.4-2011 (table 9) page 79
 *     uint8_t channelNumber;	// Channel Number
 *     uint8_t channelPage;		// Channel Page
 *     uint8_t coordAddrMode;	//defined in Enum.h -> Ieee802154eAddrMode
 *     uint16_t coordPANId;
 *     uint64_t coordAddress;
 *     uint8_t capabilityInformation; // As defined in 5.3.1.2 - Specifies the operational capabilities of the associating device.
 * 
 *     uint8_t securityLevel;
 *     uint8_t keyIdMode;
 *     uint64_t keySource;
 *     uint8_t keyIndex;
 *     // Std 802.15.4e-2012 (table 9) page 125
 *     uint64_t lowLatencyNetworkInfo;
 *     uint16_t channelOffset;
 *     uint8_t hoppingSequenceID;
 * 
 *     // for MLME_ASSOCIATE.confirm - Std 802.15.4-2011 (table 12) page 82
 *     uint16_t AssocShortAddress;
 *     short status @enum(MACenum);
 * 
 *     // for MLME-DISASSOCIATE.request - Std 802.15.4-2011 (table 13) page 84
 *     uint8_t deviceAddrMode;	//defined in Enum.h -> Ieee802154eAddrMode
 *     uint16_t devicePANId;
 *     uint64_t deviceAddress;
 *     uint8_t disassociateReason; // The reason for the disassociation, as described in 5.3.2.3.
 *     bool txIndirect;
 * 
 *     // MLME-GET.request - Std 802.15.4-2011 (table 19) page 90
 *     uint8_t pibAttribute;
 * 
 *     // MLME-GTS.request - Std 802.15.4-2011 (table 21) page 92
 *     uint8_t gtsCharacteristic; // As defined in 5.3.9.2 - The characteristics of the GTS request, including whether the request is for the allocation of a new GTS or the deallocation of an existing GTS
 * 
 *     // MLME-RESET.request - Std 802.15.4-2011 (table 26) page 96
 *     bool setDefaultPIB;
 * 
 *     // MLME-RX-ENABLE.request - Std 802.15.4-2011 (table 28) page 98
 *     bool deferPermit;
 *     uint32_t rxOnTime;
 *     uint32_t rxOnDuration;
 *     uint8_t rangingRxControl; // see Enum.h -> RangingControl
 * 
 *     // MLME-SCAN.request - Std 802.15.4-2011 (table 30) page 100
 *     uint8_t scanType;	// see Enum.h -> Ieee802154eChannelScanType
 *     uint64_t scanChannels;
 *     uint8_t scanDuration;
 *     // Std 802.15.4e-2012 (table 30) page 134
 *     bool linkQualityScan;
 *     bool frameControlOptions[]; // List of Boolean; page 134
 *     uint8_t headerIElist[];
 *     uint8_t payloadIElist[];
 * 
 *     // MLME-SET.request - Std 802.15.4-2011 (table 32) page 102
 *     MAC_PIB pibAttributeValue;
 * 
 *     // MLME-START.request - Std 802.15.4-2011 (table 34) page 104
 *     uint16_t panId;
 * 
 *     uint32_t startTime;
 *     uint8_t beaconOrder;
 *     uint8_t superframeOrder;
 *     bool panCoordinator;
 *     bool batteryLifeExtension;
 *     bool coordRealignment;
 *     uint8_t coordRealignSecurityLevel;
 *     uint8_t coordRealignKeyIdMode;
 *     uint64_t coordRealignKeySource;
 *     uint8_t coordRealignKeyIndex;
 *     uint8_t beaconSecurityLevel;
 *     uint8_t beaconKeyIdMode;
 *     uint64_t beaconKeySource;
 *     uint8_t beaconKeyIndex;
 *     // Std 802.15.4e-2012 (table 34) page 137
 *     uint8_t dsmeSuperframeSpecification;
 *     uint64_t beaconBitmap;
 *     //HoppingDescriptor - Std 802.15.4e-2012 (table 34a) page 139
 *     uint16_t hoppingSequenceLength;
 *     uint8_t hoppingSequence[];
 * 
 *     uint16_t channelOffsetBitmapLength;
 *     uint8_t channelOffsetBitmap[];
 * 
 *     // MLME-SYNC.request - Std 802.15.4.-2011 (table 36) page 108
 *     bool trackBeacon;
 * 
 *    	// MLME-POLL.request - Std 802.15.4-2011 page 110
 * 
 *     // MLME-DPS.request - Std 802.15.4-2011 (table 40) page 112
 *     uint8_t txDPSIndex;
 *     uint8_t rxDPSIndex;
 *     uint32_t dpsIndexDuration;
 * 
 *     // MLME-BEACON.request - Std 802.15.4e-2012 (table 44a) page 141
 *     uint8_t beaconType;
 *     uint8_t channel;
 *     uint8_t dstAddrMode;
 *     int64_t dstAddr;
 *     bool bsnSuppression;
 * 
 *     // MLME-BEACON-NOTIFY.indication - Std 802.15.4-2011 (table 16) page 87
 *     uint8_t bsn;
 *     uint8_t finalCAPSlot;
 *     bool associationPermit;
 *     bool gtsPermit;
 *     uint8_t linkQuality;
 *     uint64_t timeStamp;
 *     short securityStatus @enum(MACenum);
 *     int codeList[];
 *     // Pending Address Specification field
 *     uint8_t nmbrShortAddrPending;	// Number of Short Addresses Pending
 *     uint8_t nmbrExtAddrPending;		// Number of Extended Addresses Pending
 *     uint64_t addrList[];
 *     int sduLength;
 *     uint8_t sdu[];
 *     uint8_t ebsn;					// Std 802.15.4e-2012
 * 
 *     // MLME-COMM_STATUS.indication - Std 802.15.4-2011 (table 18) page 88
 *     uint8_t srcAddrMode;
 *     uint64_t srcAddr;
 * 
 *     // MLME-GTS.confirm - Std 802.15.4-2011 (table 22) page 92
 *     uint8_t gtsLength;      	// GTS Length
 *     bool gtsDirection;  		// GTS Direction
 *     bool gtsCharType;    	// Characteristics Type
 * 
 *     // MLME-SYNC-LOSS.indication - Std 802.15.4-2011 (table 37) page 109
 *     uint8_t lossReason;
 * 
 *     // for Primitives for TSCH
 *     // MLME-SET-SLOTFRAME.requ.st - Std 802.15.4e-2012 (table 44d) page 144
 *     uint8_t slotframeHandle;
 *     uint8_t operation;
 *     uint16_t size;
 * 
 *     // MLME-SET-LINK.request - Std 802.15.4e-2012 (table 44f) page 146
 *     uint16_t linkHandle;
 *     uint16_t timeslot;
 *     uint8_t linkOptions;
 *     uint8_t linkType;
 *     uint16_t nodeAddr;
 * 
 *     // MLME-TSCH-MODE.request - Std 802.15.4e-2012 (table 44h) page 147
 *     uint8_t tschMode;
 * 
 *     // MLME-KEEP-ALIVE.request - Std 802.15.4e-2012 (table 44j) page 149	
 *     uint16_t keepAlivePeriod;
 * 
 *     // for Primitives for LLDN
 *     // MLME-LLDN-DISCOVERY.request - Std 802.15.4e-2012 (table 44l) page 150	
 *     uint8_t lowLatencyNetworkConfiguration[];
 * 
 *  	// MLME-LLDN-CONFIGURATION.request - Std 802.15.4e-2012 (table 44n) page 151
 *  	// MLME-LLDN-ONLINE.request - Std 802.15.4e-2012
 * 
 *     // Primitives for DSME
 *     // MLME-DSME-GTS.request - Std 802.15.4e-2012 (table 44q) page 154
 *     uint8_t managementType;
 *     uint8_t direction;
 *     uint8_t prioritizedChannelAccess;
 *     uint8_t NumSlot;
 *     uint16_t preferredSuperframeID;
 *     uint8_t preferredSlotID;
 *     uint8_t dsmeSABSpecification[];
 * 
 *     // MLME-DSME-INFO.request - Std 802.15.4e-2012 (table 44u) page 161
 *     uint8_t info;
 *     uint8_t dsmeSABSubBlockLength;
 *     uint8_t dsmeSABSubBlockIndex;
 * 
 *     // MLME-DSME-LINKSTATUSRPT.request - Std 802.15.4e-2012 (table 44x) page 165
 *     uint32_t ReportPeriod;
 * 
 *     // MCPS-DATA.confirm - Std 802.15.4-2011 (table 48) page 122
 *     uint8_t msduHandle;
 *     bool rangingReceived;
 *     uint32_t rangingCounterStart;
 *     uint32_t rangingCounterStop;
 *     uint32_t rangingTrackingInterval;
 *     uint32_t rangingOffset;
 *     uint8_t rangingFOM;
 *     // Std 802.15.4e-2012 (table 47) page 170
 *     uint8_t numBackoffs;
 *     uint8_t ackPayload[];
 * 
 *     bool receivedByACK;
 *     int stage;
 *     int WaitConstant;
 * 
 *     bool txCS;
 *     bool txLowerCH;
 *     bool txHigherCH;
 * 
 *     uint8_t numberCH;
 *     int nChannel1 = -1;
 *     int nChannel2 = -1;
 *     int nChannel3 = -1;
 *     int nChannel4 = -1;
 *     int nChannel5 = -1;
 *     int nChannel6 = -1;
 *     int nChannel7 = -1;
 *     int nChannel8 = -1;
 *     int nChannel10 = -1;
 *     int nChannel11 = -1;
 * 
 * }
 * </pre>
 */
class Ieee802154eNetworkCtrlInfo : public ::cMessage
{
  protected:
    bool toParent_var;
    opp_string destName_var;
    int64_t netwAddr_var;
    uint8_t channelNumber_var;
    uint8_t channelPage_var;
    uint8_t coordAddrMode_var;
    uint16_t coordPANId_var;
    uint64_t coordAddress_var;
    uint8_t capabilityInformation_var;
    uint8_t securityLevel_var;
    uint8_t keyIdMode_var;
    uint64_t keySource_var;
    uint8_t keyIndex_var;
    uint64_t lowLatencyNetworkInfo_var;
    uint16_t channelOffset_var;
    uint8_t hoppingSequenceID_var;
    uint16_t AssocShortAddress_var;
    short status_var;
    uint8_t deviceAddrMode_var;
    uint16_t devicePANId_var;
    uint64_t deviceAddress_var;
    uint8_t disassociateReason_var;
    bool txIndirect_var;
    uint8_t pibAttribute_var;
    uint8_t gtsCharacteristic_var;
    bool setDefaultPIB_var;
    bool deferPermit_var;
    uint32_t rxOnTime_var;
    uint32_t rxOnDuration_var;
    uint8_t rangingRxControl_var;
    uint8_t scanType_var;
    uint64_t scanChannels_var;
    uint8_t scanDuration_var;
    bool linkQualityScan_var;
    bool *frameControlOptions_var; // array ptr
    unsigned int frameControlOptions_arraysize;
    uint8_t *headerIElist_var; // array ptr
    unsigned int headerIElist_arraysize;
    uint8_t *payloadIElist_var; // array ptr
    unsigned int payloadIElist_arraysize;
    MAC_PIB pibAttributeValue_var;
    uint16_t panId_var;
    uint32_t startTime_var;
    uint8_t beaconOrder_var;
    uint8_t superframeOrder_var;
    bool panCoordinator_var;
    bool batteryLifeExtension_var;
    bool coordRealignment_var;
    uint8_t coordRealignSecurityLevel_var;
    uint8_t coordRealignKeyIdMode_var;
    uint64_t coordRealignKeySource_var;
    uint8_t coordRealignKeyIndex_var;
    uint8_t beaconSecurityLevel_var;
    uint8_t beaconKeyIdMode_var;
    uint64_t beaconKeySource_var;
    uint8_t beaconKeyIndex_var;
    uint8_t dsmeSuperframeSpecification_var;
    uint64_t beaconBitmap_var;
    uint16_t hoppingSequenceLength_var;
    uint8_t *hoppingSequence_var; // array ptr
    unsigned int hoppingSequence_arraysize;
    uint16_t channelOffsetBitmapLength_var;
    uint8_t *channelOffsetBitmap_var; // array ptr
    unsigned int channelOffsetBitmap_arraysize;
    bool trackBeacon_var;
    uint8_t txDPSIndex_var;
    uint8_t rxDPSIndex_var;
    uint32_t dpsIndexDuration_var;
    uint8_t beaconType_var;
    uint8_t channel_var;
    uint8_t dstAddrMode_var;
    int64_t dstAddr_var;
    bool bsnSuppression_var;
    uint8_t bsn_var;
    uint8_t finalCAPSlot_var;
    bool associationPermit_var;
    bool gtsPermit_var;
    uint8_t linkQuality_var;
    uint64_t timeStamp_var;
    short securityStatus_var;
    int *codeList_var; // array ptr
    unsigned int codeList_arraysize;
    uint8_t nmbrShortAddrPending_var;
    uint8_t nmbrExtAddrPending_var;
    uint64_t *addrList_var; // array ptr
    unsigned int addrList_arraysize;
    int sduLength_var;
    uint8_t *sdu_var; // array ptr
    unsigned int sdu_arraysize;
    uint8_t ebsn_var;
    uint8_t srcAddrMode_var;
    uint64_t srcAddr_var;
    uint8_t gtsLength_var;
    bool gtsDirection_var;
    bool gtsCharType_var;
    uint8_t lossReason_var;
    uint8_t slotframeHandle_var;
    uint8_t operation_var;
    uint16_t size_var;
    uint16_t linkHandle_var;
    uint16_t timeslot_var;
    uint8_t linkOptions_var;
    uint8_t linkType_var;
    uint16_t nodeAddr_var;
    uint8_t tschMode_var;
    uint16_t keepAlivePeriod_var;
    uint8_t *lowLatencyNetworkConfiguration_var; // array ptr
    unsigned int lowLatencyNetworkConfiguration_arraysize;
    uint8_t managementType_var;
    uint8_t direction_var;
    uint8_t prioritizedChannelAccess_var;
    uint8_t NumSlot_var;
    uint16_t preferredSuperframeID_var;
    uint8_t preferredSlotID_var;
    uint8_t *dsmeSABSpecification_var; // array ptr
    unsigned int dsmeSABSpecification_arraysize;
    uint8_t info_var;
    uint8_t dsmeSABSubBlockLength_var;
    uint8_t dsmeSABSubBlockIndex_var;
    uint32_t ReportPeriod_var;
    uint8_t msduHandle_var;
    bool rangingReceived_var;
    uint32_t rangingCounterStart_var;
    uint32_t rangingCounterStop_var;
    uint32_t rangingTrackingInterval_var;
    uint32_t rangingOffset_var;
    uint8_t rangingFOM_var;
    uint8_t numBackoffs_var;
    uint8_t *ackPayload_var; // array ptr
    unsigned int ackPayload_arraysize;
    bool receivedByACK_var;
    int stage_var;
    int WaitConstant_var;
    bool txCS_var;
    bool txLowerCH_var;
    bool txHigherCH_var;
    uint8_t numberCH_var;
    int nChannel1_var;
    int nChannel2_var;
    int nChannel3_var;
    int nChannel4_var;
    int nChannel5_var;
    int nChannel6_var;
    int nChannel7_var;
    int nChannel8_var;
    int nChannel10_var;
    int nChannel11_var;

  private:
    void copy(const Ieee802154eNetworkCtrlInfo& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Ieee802154eNetworkCtrlInfo&);

  public:
    Ieee802154eNetworkCtrlInfo(const char *name=NULL, int kind=0);
    Ieee802154eNetworkCtrlInfo(const Ieee802154eNetworkCtrlInfo& other);
    virtual ~Ieee802154eNetworkCtrlInfo();
    Ieee802154eNetworkCtrlInfo& operator=(const Ieee802154eNetworkCtrlInfo& other);
    virtual Ieee802154eNetworkCtrlInfo *dup() const {return new Ieee802154eNetworkCtrlInfo(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual bool getToParent() const;
    virtual void setToParent(bool toParent);
    virtual const char * getDestName() const;
    virtual void setDestName(const char * destName);
    virtual int64_t getNetwAddr() const;
    virtual void setNetwAddr(int64_t netwAddr);
    virtual uint8_t getChannelNumber() const;
    virtual void setChannelNumber(uint8_t channelNumber);
    virtual uint8_t getChannelPage() const;
    virtual void setChannelPage(uint8_t channelPage);
    virtual uint8_t getCoordAddrMode() const;
    virtual void setCoordAddrMode(uint8_t coordAddrMode);
    virtual uint16_t getCoordPANId() const;
    virtual void setCoordPANId(uint16_t coordPANId);
    virtual uint64_t getCoordAddress() const;
    virtual void setCoordAddress(uint64_t coordAddress);
    virtual uint8_t getCapabilityInformation() const;
    virtual void setCapabilityInformation(uint8_t capabilityInformation);
    virtual uint8_t getSecurityLevel() const;
    virtual void setSecurityLevel(uint8_t securityLevel);
    virtual uint8_t getKeyIdMode() const;
    virtual void setKeyIdMode(uint8_t keyIdMode);
    virtual uint64_t getKeySource() const;
    virtual void setKeySource(uint64_t keySource);
    virtual uint8_t getKeyIndex() const;
    virtual void setKeyIndex(uint8_t keyIndex);
    virtual uint64_t getLowLatencyNetworkInfo() const;
    virtual void setLowLatencyNetworkInfo(uint64_t lowLatencyNetworkInfo);
    virtual uint16_t getChannelOffset() const;
    virtual void setChannelOffset(uint16_t channelOffset);
    virtual uint8_t getHoppingSequenceID() const;
    virtual void setHoppingSequenceID(uint8_t hoppingSequenceID);
    virtual uint16_t getAssocShortAddress() const;
    virtual void setAssocShortAddress(uint16_t AssocShortAddress);
    virtual short getStatus() const;
    virtual void setStatus(short status);
    virtual uint8_t getDeviceAddrMode() const;
    virtual void setDeviceAddrMode(uint8_t deviceAddrMode);
    virtual uint16_t getDevicePANId() const;
    virtual void setDevicePANId(uint16_t devicePANId);
    virtual uint64_t getDeviceAddress() const;
    virtual void setDeviceAddress(uint64_t deviceAddress);
    virtual uint8_t getDisassociateReason() const;
    virtual void setDisassociateReason(uint8_t disassociateReason);
    virtual bool getTxIndirect() const;
    virtual void setTxIndirect(bool txIndirect);
    virtual uint8_t getPibAttribute() const;
    virtual void setPibAttribute(uint8_t pibAttribute);
    virtual uint8_t getGtsCharacteristic() const;
    virtual void setGtsCharacteristic(uint8_t gtsCharacteristic);
    virtual bool getSetDefaultPIB() const;
    virtual void setSetDefaultPIB(bool setDefaultPIB);
    virtual bool getDeferPermit() const;
    virtual void setDeferPermit(bool deferPermit);
    virtual uint32_t getRxOnTime() const;
    virtual void setRxOnTime(uint32_t rxOnTime);
    virtual uint32_t getRxOnDuration() const;
    virtual void setRxOnDuration(uint32_t rxOnDuration);
    virtual uint8_t getRangingRxControl() const;
    virtual void setRangingRxControl(uint8_t rangingRxControl);
    virtual uint8_t getScanType() const;
    virtual void setScanType(uint8_t scanType);
    virtual uint64_t getScanChannels() const;
    virtual void setScanChannels(uint64_t scanChannels);
    virtual uint8_t getScanDuration() const;
    virtual void setScanDuration(uint8_t scanDuration);
    virtual bool getLinkQualityScan() const;
    virtual void setLinkQualityScan(bool linkQualityScan);
    virtual void setFrameControlOptionsArraySize(unsigned int size);
    virtual unsigned int getFrameControlOptionsArraySize() const;
    virtual bool getFrameControlOptions(unsigned int k) const;
    virtual void setFrameControlOptions(unsigned int k, bool frameControlOptions);
    virtual void setHeaderIElistArraySize(unsigned int size);
    virtual unsigned int getHeaderIElistArraySize() const;
    virtual uint8_t getHeaderIElist(unsigned int k) const;
    virtual void setHeaderIElist(unsigned int k, uint8_t headerIElist);
    virtual void setPayloadIElistArraySize(unsigned int size);
    virtual unsigned int getPayloadIElistArraySize() const;
    virtual uint8_t getPayloadIElist(unsigned int k) const;
    virtual void setPayloadIElist(unsigned int k, uint8_t payloadIElist);
    virtual MAC_PIB& getPibAttributeValue();
    virtual const MAC_PIB& getPibAttributeValue() const {return const_cast<Ieee802154eNetworkCtrlInfo*>(this)->getPibAttributeValue();}
    virtual void setPibAttributeValue(const MAC_PIB& pibAttributeValue);
    virtual uint16_t getPanId() const;
    virtual void setPanId(uint16_t panId);
    virtual uint32_t getStartTime() const;
    virtual void setStartTime(uint32_t startTime);
    virtual uint8_t getBeaconOrder() const;
    virtual void setBeaconOrder(uint8_t beaconOrder);
    virtual uint8_t getSuperframeOrder() const;
    virtual void setSuperframeOrder(uint8_t superframeOrder);
    virtual bool getPanCoordinator() const;
    virtual void setPanCoordinator(bool panCoordinator);
    virtual bool getBatteryLifeExtension() const;
    virtual void setBatteryLifeExtension(bool batteryLifeExtension);
    virtual bool getCoordRealignment() const;
    virtual void setCoordRealignment(bool coordRealignment);
    virtual uint8_t getCoordRealignSecurityLevel() const;
    virtual void setCoordRealignSecurityLevel(uint8_t coordRealignSecurityLevel);
    virtual uint8_t getCoordRealignKeyIdMode() const;
    virtual void setCoordRealignKeyIdMode(uint8_t coordRealignKeyIdMode);
    virtual uint64_t getCoordRealignKeySource() const;
    virtual void setCoordRealignKeySource(uint64_t coordRealignKeySource);
    virtual uint8_t getCoordRealignKeyIndex() const;
    virtual void setCoordRealignKeyIndex(uint8_t coordRealignKeyIndex);
    virtual uint8_t getBeaconSecurityLevel() const;
    virtual void setBeaconSecurityLevel(uint8_t beaconSecurityLevel);
    virtual uint8_t getBeaconKeyIdMode() const;
    virtual void setBeaconKeyIdMode(uint8_t beaconKeyIdMode);
    virtual uint64_t getBeaconKeySource() const;
    virtual void setBeaconKeySource(uint64_t beaconKeySource);
    virtual uint8_t getBeaconKeyIndex() const;
    virtual void setBeaconKeyIndex(uint8_t beaconKeyIndex);
    virtual uint8_t getDsmeSuperframeSpecification() const;
    virtual void setDsmeSuperframeSpecification(uint8_t dsmeSuperframeSpecification);
    virtual uint64_t getBeaconBitmap() const;
    virtual void setBeaconBitmap(uint64_t beaconBitmap);
    virtual uint16_t getHoppingSequenceLength() const;
    virtual void setHoppingSequenceLength(uint16_t hoppingSequenceLength);
    virtual void setHoppingSequenceArraySize(unsigned int size);
    virtual unsigned int getHoppingSequenceArraySize() const;
    virtual uint8_t getHoppingSequence(unsigned int k) const;
    virtual void setHoppingSequence(unsigned int k, uint8_t hoppingSequence);
    virtual uint16_t getChannelOffsetBitmapLength() const;
    virtual void setChannelOffsetBitmapLength(uint16_t channelOffsetBitmapLength);
    virtual void setChannelOffsetBitmapArraySize(unsigned int size);
    virtual unsigned int getChannelOffsetBitmapArraySize() const;
    virtual uint8_t getChannelOffsetBitmap(unsigned int k) const;
    virtual void setChannelOffsetBitmap(unsigned int k, uint8_t channelOffsetBitmap);
    virtual bool getTrackBeacon() const;
    virtual void setTrackBeacon(bool trackBeacon);
    virtual uint8_t getTxDPSIndex() const;
    virtual void setTxDPSIndex(uint8_t txDPSIndex);
    virtual uint8_t getRxDPSIndex() const;
    virtual void setRxDPSIndex(uint8_t rxDPSIndex);
    virtual uint32_t getDpsIndexDuration() const;
    virtual void setDpsIndexDuration(uint32_t dpsIndexDuration);
    virtual uint8_t getBeaconType() const;
    virtual void setBeaconType(uint8_t beaconType);
    virtual uint8_t getChannel() const;
    virtual void setChannel(uint8_t channel);
    virtual uint8_t getDstAddrMode() const;
    virtual void setDstAddrMode(uint8_t dstAddrMode);
    virtual int64_t getDstAddr() const;
    virtual void setDstAddr(int64_t dstAddr);
    virtual bool getBsnSuppression() const;
    virtual void setBsnSuppression(bool bsnSuppression);
    virtual uint8_t getBsn() const;
    virtual void setBsn(uint8_t bsn);
    virtual uint8_t getFinalCAPSlot() const;
    virtual void setFinalCAPSlot(uint8_t finalCAPSlot);
    virtual bool getAssociationPermit() const;
    virtual void setAssociationPermit(bool associationPermit);
    virtual bool getGtsPermit() const;
    virtual void setGtsPermit(bool gtsPermit);
    virtual uint8_t getLinkQuality() const;
    virtual void setLinkQuality(uint8_t linkQuality);
    virtual uint64_t getTimeStamp() const;
    virtual void setTimeStamp(uint64_t timeStamp);
    virtual short getSecurityStatus() const;
    virtual void setSecurityStatus(short securityStatus);
    virtual void setCodeListArraySize(unsigned int size);
    virtual unsigned int getCodeListArraySize() const;
    virtual int getCodeList(unsigned int k) const;
    virtual void setCodeList(unsigned int k, int codeList);
    virtual uint8_t getNmbrShortAddrPending() const;
    virtual void setNmbrShortAddrPending(uint8_t nmbrShortAddrPending);
    virtual uint8_t getNmbrExtAddrPending() const;
    virtual void setNmbrExtAddrPending(uint8_t nmbrExtAddrPending);
    virtual void setAddrListArraySize(unsigned int size);
    virtual unsigned int getAddrListArraySize() const;
    virtual uint64_t getAddrList(unsigned int k) const;
    virtual void setAddrList(unsigned int k, uint64_t addrList);
    virtual int getSduLength() const;
    virtual void setSduLength(int sduLength);
    virtual void setSduArraySize(unsigned int size);
    virtual unsigned int getSduArraySize() const;
    virtual uint8_t getSdu(unsigned int k) const;
    virtual void setSdu(unsigned int k, uint8_t sdu);
    virtual uint8_t getEbsn() const;
    virtual void setEbsn(uint8_t ebsn);
    virtual uint8_t getSrcAddrMode() const;
    virtual void setSrcAddrMode(uint8_t srcAddrMode);
    virtual uint64_t getSrcAddr() const;
    virtual void setSrcAddr(uint64_t srcAddr);
    virtual uint8_t getGtsLength() const;
    virtual void setGtsLength(uint8_t gtsLength);
    virtual bool getGtsDirection() const;
    virtual void setGtsDirection(bool gtsDirection);
    virtual bool getGtsCharType() const;
    virtual void setGtsCharType(bool gtsCharType);
    virtual uint8_t getLossReason() const;
    virtual void setLossReason(uint8_t lossReason);
    virtual uint8_t getSlotframeHandle() const;
    virtual void setSlotframeHandle(uint8_t slotframeHandle);
    virtual uint8_t getOperation() const;
    virtual void setOperation(uint8_t operation);
    virtual uint16_t getSize() const;
    virtual void setSize(uint16_t size);
    virtual uint16_t getLinkHandle() const;
    virtual void setLinkHandle(uint16_t linkHandle);
    virtual uint16_t getTimeslot() const;
    virtual void setTimeslot(uint16_t timeslot);
    virtual uint8_t getLinkOptions() const;
    virtual void setLinkOptions(uint8_t linkOptions);
    virtual uint8_t getLinkType() const;
    virtual void setLinkType(uint8_t linkType);
    virtual uint16_t getNodeAddr() const;
    virtual void setNodeAddr(uint16_t nodeAddr);
    virtual uint8_t getTschMode() const;
    virtual void setTschMode(uint8_t tschMode);
    virtual uint16_t getKeepAlivePeriod() const;
    virtual void setKeepAlivePeriod(uint16_t keepAlivePeriod);
    virtual void setLowLatencyNetworkConfigurationArraySize(unsigned int size);
    virtual unsigned int getLowLatencyNetworkConfigurationArraySize() const;
    virtual uint8_t getLowLatencyNetworkConfiguration(unsigned int k) const;
    virtual void setLowLatencyNetworkConfiguration(unsigned int k, uint8_t lowLatencyNetworkConfiguration);
    virtual uint8_t getManagementType() const;
    virtual void setManagementType(uint8_t managementType);
    virtual uint8_t getDirection() const;
    virtual void setDirection(uint8_t direction);
    virtual uint8_t getPrioritizedChannelAccess() const;
    virtual void setPrioritizedChannelAccess(uint8_t prioritizedChannelAccess);
    virtual uint8_t getNumSlot() const;
    virtual void setNumSlot(uint8_t NumSlot);
    virtual uint16_t getPreferredSuperframeID() const;
    virtual void setPreferredSuperframeID(uint16_t preferredSuperframeID);
    virtual uint8_t getPreferredSlotID() const;
    virtual void setPreferredSlotID(uint8_t preferredSlotID);
    virtual void setDsmeSABSpecificationArraySize(unsigned int size);
    virtual unsigned int getDsmeSABSpecificationArraySize() const;
    virtual uint8_t getDsmeSABSpecification(unsigned int k) const;
    virtual void setDsmeSABSpecification(unsigned int k, uint8_t dsmeSABSpecification);
    virtual uint8_t getInfo() const;
    virtual void setInfo(uint8_t info);
    virtual uint8_t getDsmeSABSubBlockLength() const;
    virtual void setDsmeSABSubBlockLength(uint8_t dsmeSABSubBlockLength);
    virtual uint8_t getDsmeSABSubBlockIndex() const;
    virtual void setDsmeSABSubBlockIndex(uint8_t dsmeSABSubBlockIndex);
    virtual uint32_t getReportPeriod() const;
    virtual void setReportPeriod(uint32_t ReportPeriod);
    virtual uint8_t getMsduHandle() const;
    virtual void setMsduHandle(uint8_t msduHandle);
    virtual bool getRangingReceived() const;
    virtual void setRangingReceived(bool rangingReceived);
    virtual uint32_t getRangingCounterStart() const;
    virtual void setRangingCounterStart(uint32_t rangingCounterStart);
    virtual uint32_t getRangingCounterStop() const;
    virtual void setRangingCounterStop(uint32_t rangingCounterStop);
    virtual uint32_t getRangingTrackingInterval() const;
    virtual void setRangingTrackingInterval(uint32_t rangingTrackingInterval);
    virtual uint32_t getRangingOffset() const;
    virtual void setRangingOffset(uint32_t rangingOffset);
    virtual uint8_t getRangingFOM() const;
    virtual void setRangingFOM(uint8_t rangingFOM);
    virtual uint8_t getNumBackoffs() const;
    virtual void setNumBackoffs(uint8_t numBackoffs);
    virtual void setAckPayloadArraySize(unsigned int size);
    virtual unsigned int getAckPayloadArraySize() const;
    virtual uint8_t getAckPayload(unsigned int k) const;
    virtual void setAckPayload(unsigned int k, uint8_t ackPayload);
    virtual bool getReceivedByACK() const;
    virtual void setReceivedByACK(bool receivedByACK);
    virtual int getStage() const;
    virtual void setStage(int stage);
    virtual int getWaitConstant() const;
    virtual void setWaitConstant(int WaitConstant);
    virtual bool getTxCS() const;
    virtual void setTxCS(bool txCS);
    virtual bool getTxLowerCH() const;
    virtual void setTxLowerCH(bool txLowerCH);
    virtual bool getTxHigherCH() const;
    virtual void setTxHigherCH(bool txHigherCH);
    virtual uint8_t getNumberCH() const;
    virtual void setNumberCH(uint8_t numberCH);
    virtual int getNChannel1() const;
    virtual void setNChannel1(int nChannel1);
    virtual int getNChannel2() const;
    virtual void setNChannel2(int nChannel2);
    virtual int getNChannel3() const;
    virtual void setNChannel3(int nChannel3);
    virtual int getNChannel4() const;
    virtual void setNChannel4(int nChannel4);
    virtual int getNChannel5() const;
    virtual void setNChannel5(int nChannel5);
    virtual int getNChannel6() const;
    virtual void setNChannel6(int nChannel6);
    virtual int getNChannel7() const;
    virtual void setNChannel7(int nChannel7);
    virtual int getNChannel8() const;
    virtual void setNChannel8(int nChannel8);
    virtual int getNChannel10() const;
    virtual void setNChannel10(int nChannel10);
    virtual int getNChannel11() const;
    virtual void setNChannel11(int nChannel11);
};

inline void doPacking(cCommBuffer *b, Ieee802154eNetworkCtrlInfo& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, Ieee802154eNetworkCtrlInfo& obj) {obj.parsimUnpack(b);}


#endif // ifndef _IEEE802154ENETWORKCTRLINFO_M_H_


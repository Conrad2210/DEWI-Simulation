//
// Generated file, do not edit! Created by nedtool 4.6 from src/linklayer/contract/IEEE802154e/Ieee802154eNetworkCtrlInfo.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "Ieee802154eNetworkCtrlInfo_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

Register_Class(Ieee802154eNetworkCtrlInfo);

Ieee802154eNetworkCtrlInfo::Ieee802154eNetworkCtrlInfo(const char *name, int kind) : ::cMessage(name,kind)
{
    this->toParent_var = 0;
    this->destName_var = 0;
    this->netwAddr_var = -1;
    this->channelNumber_var = 0;
    this->channelPage_var = 0;
    this->coordAddrMode_var = 0;
    this->coordPANId_var = 0;
    this->coordAddress_var = 0;
    this->capabilityInformation_var = 0;
    this->securityLevel_var = 0;
    this->keyIdMode_var = 0;
    this->keySource_var = 0;
    this->keyIndex_var = 0;
    this->lowLatencyNetworkInfo_var = 0;
    this->channelOffset_var = 0;
    this->hoppingSequenceID_var = 0;
    this->AssocShortAddress_var = 0;
    this->status_var = 0;
    this->deviceAddrMode_var = 0;
    this->devicePANId_var = 0;
    this->deviceAddress_var = 0;
    this->disassociateReason_var = 0;
    this->txIndirect_var = 0;
    this->pibAttribute_var = 0;
    this->gtsCharacteristic_var = 0;
    this->setDefaultPIB_var = 0;
    this->deferPermit_var = 0;
    this->rxOnTime_var = 0;
    this->rxOnDuration_var = 0;
    this->rangingRxControl_var = 0;
    this->scanType_var = 0;
    this->scanChannels_var = 0;
    this->scanDuration_var = 0;
    this->linkQualityScan_var = 0;
    frameControlOptions_arraysize = 0;
    this->frameControlOptions_var = 0;
    headerIElist_arraysize = 0;
    this->headerIElist_var = 0;
    payloadIElist_arraysize = 0;
    this->payloadIElist_var = 0;
    this->panId_var = 0;
    this->startTime_var = 0;
    this->beaconOrder_var = 0;
    this->superframeOrder_var = 0;
    this->panCoordinator_var = 0;
    this->batteryLifeExtension_var = 0;
    this->coordRealignment_var = 0;
    this->coordRealignSecurityLevel_var = 0;
    this->coordRealignKeyIdMode_var = 0;
    this->coordRealignKeySource_var = 0;
    this->coordRealignKeyIndex_var = 0;
    this->beaconSecurityLevel_var = 0;
    this->beaconKeyIdMode_var = 0;
    this->beaconKeySource_var = 0;
    this->beaconKeyIndex_var = 0;
    this->dsmeSuperframeSpecification_var = 0;
    this->beaconBitmap_var = 0;
    this->hoppingSequenceLength_var = 0;
    hoppingSequence_arraysize = 0;
    this->hoppingSequence_var = 0;
    this->channelOffsetBitmapLength_var = 0;
    channelOffsetBitmap_arraysize = 0;
    this->channelOffsetBitmap_var = 0;
    this->trackBeacon_var = 0;
    this->txDPSIndex_var = 0;
    this->rxDPSIndex_var = 0;
    this->dpsIndexDuration_var = 0;
    this->beaconType_var = 0;
    this->channel_var = 0;
    this->dstAddrMode_var = 0;
    this->dstAddr_var = 0;
    this->bsnSuppression_var = 0;
    this->bsn_var = 0;
    this->finalCAPSlot_var = 0;
    this->associationPermit_var = 0;
    this->gtsPermit_var = 0;
    this->linkQuality_var = 0;
    this->timeStamp_var = 0;
    this->securityStatus_var = 0;
    codeList_arraysize = 0;
    this->codeList_var = 0;
    this->nmbrShortAddrPending_var = 0;
    this->nmbrExtAddrPending_var = 0;
    addrList_arraysize = 0;
    this->addrList_var = 0;
    this->sduLength_var = 0;
    sdu_arraysize = 0;
    this->sdu_var = 0;
    this->ebsn_var = 0;
    this->srcAddrMode_var = 0;
    this->srcAddr_var = 0;
    this->gtsLength_var = 0;
    this->gtsDirection_var = 0;
    this->gtsCharType_var = 0;
    this->lossReason_var = 0;
    this->slotframeHandle_var = 0;
    this->operation_var = 0;
    this->size_var = 0;
    this->linkHandle_var = 0;
    this->timeslot_var = 0;
    this->linkOptions_var = 0;
    this->linkType_var = 0;
    this->nodeAddr_var = 0;
    this->tschMode_var = 0;
    this->keepAlivePeriod_var = 0;
    lowLatencyNetworkConfiguration_arraysize = 0;
    this->lowLatencyNetworkConfiguration_var = 0;
    this->managementType_var = 0;
    this->direction_var = 0;
    this->prioritizedChannelAccess_var = 0;
    this->NumSlot_var = 0;
    this->preferredSuperframeID_var = 0;
    this->preferredSlotID_var = 0;
    dsmeSABSpecification_arraysize = 0;
    this->dsmeSABSpecification_var = 0;
    this->info_var = 0;
    this->dsmeSABSubBlockLength_var = 0;
    this->dsmeSABSubBlockIndex_var = 0;
    this->ReportPeriod_var = 0;
    this->msduHandle_var = 0;
    this->rangingReceived_var = 0;
    this->rangingCounterStart_var = 0;
    this->rangingCounterStop_var = 0;
    this->rangingTrackingInterval_var = 0;
    this->rangingOffset_var = 0;
    this->rangingFOM_var = 0;
    this->numBackoffs_var = 0;
    ackPayload_arraysize = 0;
    this->ackPayload_var = 0;
    this->receivedByACK_var = 0;
    this->stage_var = 0;
    this->WaitConstant_var = 0;
    this->txCS_var = 0;
    this->txLowerCH_var = 0;
    this->txHigherCH_var = 0;
    this->numberCH_var = 0;
    this->channelOffset10_var = 0;
    this->channelOffset11_var = 0;
}

Ieee802154eNetworkCtrlInfo::Ieee802154eNetworkCtrlInfo(const Ieee802154eNetworkCtrlInfo& other) : ::cMessage(other)
{
    frameControlOptions_arraysize = 0;
    this->frameControlOptions_var = 0;
    headerIElist_arraysize = 0;
    this->headerIElist_var = 0;
    payloadIElist_arraysize = 0;
    this->payloadIElist_var = 0;
    hoppingSequence_arraysize = 0;
    this->hoppingSequence_var = 0;
    channelOffsetBitmap_arraysize = 0;
    this->channelOffsetBitmap_var = 0;
    codeList_arraysize = 0;
    this->codeList_var = 0;
    addrList_arraysize = 0;
    this->addrList_var = 0;
    sdu_arraysize = 0;
    this->sdu_var = 0;
    lowLatencyNetworkConfiguration_arraysize = 0;
    this->lowLatencyNetworkConfiguration_var = 0;
    dsmeSABSpecification_arraysize = 0;
    this->dsmeSABSpecification_var = 0;
    ackPayload_arraysize = 0;
    this->ackPayload_var = 0;
    copy(other);
}

Ieee802154eNetworkCtrlInfo::~Ieee802154eNetworkCtrlInfo()
{
    delete [] frameControlOptions_var;
    delete [] headerIElist_var;
    delete [] payloadIElist_var;
    delete [] hoppingSequence_var;
    delete [] channelOffsetBitmap_var;
    delete [] codeList_var;
    delete [] addrList_var;
    delete [] sdu_var;
    delete [] lowLatencyNetworkConfiguration_var;
    delete [] dsmeSABSpecification_var;
    delete [] ackPayload_var;
}

Ieee802154eNetworkCtrlInfo& Ieee802154eNetworkCtrlInfo::operator=(const Ieee802154eNetworkCtrlInfo& other)
{
    if (this==&other) return *this;
    ::cMessage::operator=(other);
    copy(other);
    return *this;
}

void Ieee802154eNetworkCtrlInfo::copy(const Ieee802154eNetworkCtrlInfo& other)
{
    this->toParent_var = other.toParent_var;
    this->destName_var = other.destName_var;
    this->netwAddr_var = other.netwAddr_var;
    this->channelNumber_var = other.channelNumber_var;
    this->channelPage_var = other.channelPage_var;
    this->coordAddrMode_var = other.coordAddrMode_var;
    this->coordPANId_var = other.coordPANId_var;
    this->coordAddress_var = other.coordAddress_var;
    this->capabilityInformation_var = other.capabilityInformation_var;
    this->securityLevel_var = other.securityLevel_var;
    this->keyIdMode_var = other.keyIdMode_var;
    this->keySource_var = other.keySource_var;
    this->keyIndex_var = other.keyIndex_var;
    this->lowLatencyNetworkInfo_var = other.lowLatencyNetworkInfo_var;
    this->channelOffset_var = other.channelOffset_var;
    this->hoppingSequenceID_var = other.hoppingSequenceID_var;
    this->AssocShortAddress_var = other.AssocShortAddress_var;
    this->status_var = other.status_var;
    this->deviceAddrMode_var = other.deviceAddrMode_var;
    this->devicePANId_var = other.devicePANId_var;
    this->deviceAddress_var = other.deviceAddress_var;
    this->disassociateReason_var = other.disassociateReason_var;
    this->txIndirect_var = other.txIndirect_var;
    this->pibAttribute_var = other.pibAttribute_var;
    this->gtsCharacteristic_var = other.gtsCharacteristic_var;
    this->setDefaultPIB_var = other.setDefaultPIB_var;
    this->deferPermit_var = other.deferPermit_var;
    this->rxOnTime_var = other.rxOnTime_var;
    this->rxOnDuration_var = other.rxOnDuration_var;
    this->rangingRxControl_var = other.rangingRxControl_var;
    this->scanType_var = other.scanType_var;
    this->scanChannels_var = other.scanChannels_var;
    this->scanDuration_var = other.scanDuration_var;
    this->linkQualityScan_var = other.linkQualityScan_var;
    delete [] this->frameControlOptions_var;
    this->frameControlOptions_var = (other.frameControlOptions_arraysize==0) ? NULL : new bool[other.frameControlOptions_arraysize];
    frameControlOptions_arraysize = other.frameControlOptions_arraysize;
    for (unsigned int i=0; i<frameControlOptions_arraysize; i++)
        this->frameControlOptions_var[i] = other.frameControlOptions_var[i];
    delete [] this->headerIElist_var;
    this->headerIElist_var = (other.headerIElist_arraysize==0) ? NULL : new uint8_t[other.headerIElist_arraysize];
    headerIElist_arraysize = other.headerIElist_arraysize;
    for (unsigned int i=0; i<headerIElist_arraysize; i++)
        this->headerIElist_var[i] = other.headerIElist_var[i];
    delete [] this->payloadIElist_var;
    this->payloadIElist_var = (other.payloadIElist_arraysize==0) ? NULL : new uint8_t[other.payloadIElist_arraysize];
    payloadIElist_arraysize = other.payloadIElist_arraysize;
    for (unsigned int i=0; i<payloadIElist_arraysize; i++)
        this->payloadIElist_var[i] = other.payloadIElist_var[i];
    this->pibAttributeValue_var = other.pibAttributeValue_var;
    this->panId_var = other.panId_var;
    this->startTime_var = other.startTime_var;
    this->beaconOrder_var = other.beaconOrder_var;
    this->superframeOrder_var = other.superframeOrder_var;
    this->panCoordinator_var = other.panCoordinator_var;
    this->batteryLifeExtension_var = other.batteryLifeExtension_var;
    this->coordRealignment_var = other.coordRealignment_var;
    this->coordRealignSecurityLevel_var = other.coordRealignSecurityLevel_var;
    this->coordRealignKeyIdMode_var = other.coordRealignKeyIdMode_var;
    this->coordRealignKeySource_var = other.coordRealignKeySource_var;
    this->coordRealignKeyIndex_var = other.coordRealignKeyIndex_var;
    this->beaconSecurityLevel_var = other.beaconSecurityLevel_var;
    this->beaconKeyIdMode_var = other.beaconKeyIdMode_var;
    this->beaconKeySource_var = other.beaconKeySource_var;
    this->beaconKeyIndex_var = other.beaconKeyIndex_var;
    this->dsmeSuperframeSpecification_var = other.dsmeSuperframeSpecification_var;
    this->beaconBitmap_var = other.beaconBitmap_var;
    this->hoppingSequenceLength_var = other.hoppingSequenceLength_var;
    delete [] this->hoppingSequence_var;
    this->hoppingSequence_var = (other.hoppingSequence_arraysize==0) ? NULL : new uint8_t[other.hoppingSequence_arraysize];
    hoppingSequence_arraysize = other.hoppingSequence_arraysize;
    for (unsigned int i=0; i<hoppingSequence_arraysize; i++)
        this->hoppingSequence_var[i] = other.hoppingSequence_var[i];
    this->channelOffsetBitmapLength_var = other.channelOffsetBitmapLength_var;
    delete [] this->channelOffsetBitmap_var;
    this->channelOffsetBitmap_var = (other.channelOffsetBitmap_arraysize==0) ? NULL : new uint8_t[other.channelOffsetBitmap_arraysize];
    channelOffsetBitmap_arraysize = other.channelOffsetBitmap_arraysize;
    for (unsigned int i=0; i<channelOffsetBitmap_arraysize; i++)
        this->channelOffsetBitmap_var[i] = other.channelOffsetBitmap_var[i];
    this->trackBeacon_var = other.trackBeacon_var;
    this->txDPSIndex_var = other.txDPSIndex_var;
    this->rxDPSIndex_var = other.rxDPSIndex_var;
    this->dpsIndexDuration_var = other.dpsIndexDuration_var;
    this->beaconType_var = other.beaconType_var;
    this->channel_var = other.channel_var;
    this->dstAddrMode_var = other.dstAddrMode_var;
    this->dstAddr_var = other.dstAddr_var;
    this->bsnSuppression_var = other.bsnSuppression_var;
    this->bsn_var = other.bsn_var;
    this->finalCAPSlot_var = other.finalCAPSlot_var;
    this->associationPermit_var = other.associationPermit_var;
    this->gtsPermit_var = other.gtsPermit_var;
    this->linkQuality_var = other.linkQuality_var;
    this->timeStamp_var = other.timeStamp_var;
    this->securityStatus_var = other.securityStatus_var;
    delete [] this->codeList_var;
    this->codeList_var = (other.codeList_arraysize==0) ? NULL : new int[other.codeList_arraysize];
    codeList_arraysize = other.codeList_arraysize;
    for (unsigned int i=0; i<codeList_arraysize; i++)
        this->codeList_var[i] = other.codeList_var[i];
    this->nmbrShortAddrPending_var = other.nmbrShortAddrPending_var;
    this->nmbrExtAddrPending_var = other.nmbrExtAddrPending_var;
    delete [] this->addrList_var;
    this->addrList_var = (other.addrList_arraysize==0) ? NULL : new uint64_t[other.addrList_arraysize];
    addrList_arraysize = other.addrList_arraysize;
    for (unsigned int i=0; i<addrList_arraysize; i++)
        this->addrList_var[i] = other.addrList_var[i];
    this->sduLength_var = other.sduLength_var;
    delete [] this->sdu_var;
    this->sdu_var = (other.sdu_arraysize==0) ? NULL : new uint8_t[other.sdu_arraysize];
    sdu_arraysize = other.sdu_arraysize;
    for (unsigned int i=0; i<sdu_arraysize; i++)
        this->sdu_var[i] = other.sdu_var[i];
    this->ebsn_var = other.ebsn_var;
    this->srcAddrMode_var = other.srcAddrMode_var;
    this->srcAddr_var = other.srcAddr_var;
    this->gtsLength_var = other.gtsLength_var;
    this->gtsDirection_var = other.gtsDirection_var;
    this->gtsCharType_var = other.gtsCharType_var;
    this->lossReason_var = other.lossReason_var;
    this->slotframeHandle_var = other.slotframeHandle_var;
    this->operation_var = other.operation_var;
    this->size_var = other.size_var;
    this->linkHandle_var = other.linkHandle_var;
    this->timeslot_var = other.timeslot_var;
    this->linkOptions_var = other.linkOptions_var;
    this->linkType_var = other.linkType_var;
    this->nodeAddr_var = other.nodeAddr_var;
    this->tschMode_var = other.tschMode_var;
    this->keepAlivePeriod_var = other.keepAlivePeriod_var;
    delete [] this->lowLatencyNetworkConfiguration_var;
    this->lowLatencyNetworkConfiguration_var = (other.lowLatencyNetworkConfiguration_arraysize==0) ? NULL : new uint8_t[other.lowLatencyNetworkConfiguration_arraysize];
    lowLatencyNetworkConfiguration_arraysize = other.lowLatencyNetworkConfiguration_arraysize;
    for (unsigned int i=0; i<lowLatencyNetworkConfiguration_arraysize; i++)
        this->lowLatencyNetworkConfiguration_var[i] = other.lowLatencyNetworkConfiguration_var[i];
    this->managementType_var = other.managementType_var;
    this->direction_var = other.direction_var;
    this->prioritizedChannelAccess_var = other.prioritizedChannelAccess_var;
    this->NumSlot_var = other.NumSlot_var;
    this->preferredSuperframeID_var = other.preferredSuperframeID_var;
    this->preferredSlotID_var = other.preferredSlotID_var;
    delete [] this->dsmeSABSpecification_var;
    this->dsmeSABSpecification_var = (other.dsmeSABSpecification_arraysize==0) ? NULL : new uint8_t[other.dsmeSABSpecification_arraysize];
    dsmeSABSpecification_arraysize = other.dsmeSABSpecification_arraysize;
    for (unsigned int i=0; i<dsmeSABSpecification_arraysize; i++)
        this->dsmeSABSpecification_var[i] = other.dsmeSABSpecification_var[i];
    this->info_var = other.info_var;
    this->dsmeSABSubBlockLength_var = other.dsmeSABSubBlockLength_var;
    this->dsmeSABSubBlockIndex_var = other.dsmeSABSubBlockIndex_var;
    this->ReportPeriod_var = other.ReportPeriod_var;
    this->msduHandle_var = other.msduHandle_var;
    this->rangingReceived_var = other.rangingReceived_var;
    this->rangingCounterStart_var = other.rangingCounterStart_var;
    this->rangingCounterStop_var = other.rangingCounterStop_var;
    this->rangingTrackingInterval_var = other.rangingTrackingInterval_var;
    this->rangingOffset_var = other.rangingOffset_var;
    this->rangingFOM_var = other.rangingFOM_var;
    this->numBackoffs_var = other.numBackoffs_var;
    delete [] this->ackPayload_var;
    this->ackPayload_var = (other.ackPayload_arraysize==0) ? NULL : new uint8_t[other.ackPayload_arraysize];
    ackPayload_arraysize = other.ackPayload_arraysize;
    for (unsigned int i=0; i<ackPayload_arraysize; i++)
        this->ackPayload_var[i] = other.ackPayload_var[i];
    this->receivedByACK_var = other.receivedByACK_var;
    this->stage_var = other.stage_var;
    this->WaitConstant_var = other.WaitConstant_var;
    this->txCS_var = other.txCS_var;
    this->txLowerCH_var = other.txLowerCH_var;
    this->txHigherCH_var = other.txHigherCH_var;
    this->numberCH_var = other.numberCH_var;
    this->channelOffset10_var = other.channelOffset10_var;
    this->channelOffset11_var = other.channelOffset11_var;
}

void Ieee802154eNetworkCtrlInfo::parsimPack(cCommBuffer *b)
{
    ::cMessage::parsimPack(b);
    doPacking(b,this->toParent_var);
    doPacking(b,this->destName_var);
    doPacking(b,this->netwAddr_var);
    doPacking(b,this->channelNumber_var);
    doPacking(b,this->channelPage_var);
    doPacking(b,this->coordAddrMode_var);
    doPacking(b,this->coordPANId_var);
    doPacking(b,this->coordAddress_var);
    doPacking(b,this->capabilityInformation_var);
    doPacking(b,this->securityLevel_var);
    doPacking(b,this->keyIdMode_var);
    doPacking(b,this->keySource_var);
    doPacking(b,this->keyIndex_var);
    doPacking(b,this->lowLatencyNetworkInfo_var);
    doPacking(b,this->channelOffset_var);
    doPacking(b,this->hoppingSequenceID_var);
    doPacking(b,this->AssocShortAddress_var);
    doPacking(b,this->status_var);
    doPacking(b,this->deviceAddrMode_var);
    doPacking(b,this->devicePANId_var);
    doPacking(b,this->deviceAddress_var);
    doPacking(b,this->disassociateReason_var);
    doPacking(b,this->txIndirect_var);
    doPacking(b,this->pibAttribute_var);
    doPacking(b,this->gtsCharacteristic_var);
    doPacking(b,this->setDefaultPIB_var);
    doPacking(b,this->deferPermit_var);
    doPacking(b,this->rxOnTime_var);
    doPacking(b,this->rxOnDuration_var);
    doPacking(b,this->rangingRxControl_var);
    doPacking(b,this->scanType_var);
    doPacking(b,this->scanChannels_var);
    doPacking(b,this->scanDuration_var);
    doPacking(b,this->linkQualityScan_var);
    b->pack(frameControlOptions_arraysize);
    doPacking(b,this->frameControlOptions_var,frameControlOptions_arraysize);
    b->pack(headerIElist_arraysize);
    doPacking(b,this->headerIElist_var,headerIElist_arraysize);
    b->pack(payloadIElist_arraysize);
    doPacking(b,this->payloadIElist_var,payloadIElist_arraysize);
    doPacking(b,this->pibAttributeValue_var);
    doPacking(b,this->panId_var);
    doPacking(b,this->startTime_var);
    doPacking(b,this->beaconOrder_var);
    doPacking(b,this->superframeOrder_var);
    doPacking(b,this->panCoordinator_var);
    doPacking(b,this->batteryLifeExtension_var);
    doPacking(b,this->coordRealignment_var);
    doPacking(b,this->coordRealignSecurityLevel_var);
    doPacking(b,this->coordRealignKeyIdMode_var);
    doPacking(b,this->coordRealignKeySource_var);
    doPacking(b,this->coordRealignKeyIndex_var);
    doPacking(b,this->beaconSecurityLevel_var);
    doPacking(b,this->beaconKeyIdMode_var);
    doPacking(b,this->beaconKeySource_var);
    doPacking(b,this->beaconKeyIndex_var);
    doPacking(b,this->dsmeSuperframeSpecification_var);
    doPacking(b,this->beaconBitmap_var);
    doPacking(b,this->hoppingSequenceLength_var);
    b->pack(hoppingSequence_arraysize);
    doPacking(b,this->hoppingSequence_var,hoppingSequence_arraysize);
    doPacking(b,this->channelOffsetBitmapLength_var);
    b->pack(channelOffsetBitmap_arraysize);
    doPacking(b,this->channelOffsetBitmap_var,channelOffsetBitmap_arraysize);
    doPacking(b,this->trackBeacon_var);
    doPacking(b,this->txDPSIndex_var);
    doPacking(b,this->rxDPSIndex_var);
    doPacking(b,this->dpsIndexDuration_var);
    doPacking(b,this->beaconType_var);
    doPacking(b,this->channel_var);
    doPacking(b,this->dstAddrMode_var);
    doPacking(b,this->dstAddr_var);
    doPacking(b,this->bsnSuppression_var);
    doPacking(b,this->bsn_var);
    doPacking(b,this->finalCAPSlot_var);
    doPacking(b,this->associationPermit_var);
    doPacking(b,this->gtsPermit_var);
    doPacking(b,this->linkQuality_var);
    doPacking(b,this->timeStamp_var);
    doPacking(b,this->securityStatus_var);
    b->pack(codeList_arraysize);
    doPacking(b,this->codeList_var,codeList_arraysize);
    doPacking(b,this->nmbrShortAddrPending_var);
    doPacking(b,this->nmbrExtAddrPending_var);
    b->pack(addrList_arraysize);
    doPacking(b,this->addrList_var,addrList_arraysize);
    doPacking(b,this->sduLength_var);
    b->pack(sdu_arraysize);
    doPacking(b,this->sdu_var,sdu_arraysize);
    doPacking(b,this->ebsn_var);
    doPacking(b,this->srcAddrMode_var);
    doPacking(b,this->srcAddr_var);
    doPacking(b,this->gtsLength_var);
    doPacking(b,this->gtsDirection_var);
    doPacking(b,this->gtsCharType_var);
    doPacking(b,this->lossReason_var);
    doPacking(b,this->slotframeHandle_var);
    doPacking(b,this->operation_var);
    doPacking(b,this->size_var);
    doPacking(b,this->linkHandle_var);
    doPacking(b,this->timeslot_var);
    doPacking(b,this->linkOptions_var);
    doPacking(b,this->linkType_var);
    doPacking(b,this->nodeAddr_var);
    doPacking(b,this->tschMode_var);
    doPacking(b,this->keepAlivePeriod_var);
    b->pack(lowLatencyNetworkConfiguration_arraysize);
    doPacking(b,this->lowLatencyNetworkConfiguration_var,lowLatencyNetworkConfiguration_arraysize);
    doPacking(b,this->managementType_var);
    doPacking(b,this->direction_var);
    doPacking(b,this->prioritizedChannelAccess_var);
    doPacking(b,this->NumSlot_var);
    doPacking(b,this->preferredSuperframeID_var);
    doPacking(b,this->preferredSlotID_var);
    b->pack(dsmeSABSpecification_arraysize);
    doPacking(b,this->dsmeSABSpecification_var,dsmeSABSpecification_arraysize);
    doPacking(b,this->info_var);
    doPacking(b,this->dsmeSABSubBlockLength_var);
    doPacking(b,this->dsmeSABSubBlockIndex_var);
    doPacking(b,this->ReportPeriod_var);
    doPacking(b,this->msduHandle_var);
    doPacking(b,this->rangingReceived_var);
    doPacking(b,this->rangingCounterStart_var);
    doPacking(b,this->rangingCounterStop_var);
    doPacking(b,this->rangingTrackingInterval_var);
    doPacking(b,this->rangingOffset_var);
    doPacking(b,this->rangingFOM_var);
    doPacking(b,this->numBackoffs_var);
    b->pack(ackPayload_arraysize);
    doPacking(b,this->ackPayload_var,ackPayload_arraysize);
    doPacking(b,this->receivedByACK_var);
    doPacking(b,this->stage_var);
    doPacking(b,this->WaitConstant_var);
    doPacking(b,this->txCS_var);
    doPacking(b,this->txLowerCH_var);
    doPacking(b,this->txHigherCH_var);
    doPacking(b,this->numberCH_var);
    doPacking(b,this->channelOffset10_var);
    doPacking(b,this->channelOffset11_var);
}

void Ieee802154eNetworkCtrlInfo::parsimUnpack(cCommBuffer *b)
{
    ::cMessage::parsimUnpack(b);
    doUnpacking(b,this->toParent_var);
    doUnpacking(b,this->destName_var);
    doUnpacking(b,this->netwAddr_var);
    doUnpacking(b,this->channelNumber_var);
    doUnpacking(b,this->channelPage_var);
    doUnpacking(b,this->coordAddrMode_var);
    doUnpacking(b,this->coordPANId_var);
    doUnpacking(b,this->coordAddress_var);
    doUnpacking(b,this->capabilityInformation_var);
    doUnpacking(b,this->securityLevel_var);
    doUnpacking(b,this->keyIdMode_var);
    doUnpacking(b,this->keySource_var);
    doUnpacking(b,this->keyIndex_var);
    doUnpacking(b,this->lowLatencyNetworkInfo_var);
    doUnpacking(b,this->channelOffset_var);
    doUnpacking(b,this->hoppingSequenceID_var);
    doUnpacking(b,this->AssocShortAddress_var);
    doUnpacking(b,this->status_var);
    doUnpacking(b,this->deviceAddrMode_var);
    doUnpacking(b,this->devicePANId_var);
    doUnpacking(b,this->deviceAddress_var);
    doUnpacking(b,this->disassociateReason_var);
    doUnpacking(b,this->txIndirect_var);
    doUnpacking(b,this->pibAttribute_var);
    doUnpacking(b,this->gtsCharacteristic_var);
    doUnpacking(b,this->setDefaultPIB_var);
    doUnpacking(b,this->deferPermit_var);
    doUnpacking(b,this->rxOnTime_var);
    doUnpacking(b,this->rxOnDuration_var);
    doUnpacking(b,this->rangingRxControl_var);
    doUnpacking(b,this->scanType_var);
    doUnpacking(b,this->scanChannels_var);
    doUnpacking(b,this->scanDuration_var);
    doUnpacking(b,this->linkQualityScan_var);
    delete [] this->frameControlOptions_var;
    b->unpack(frameControlOptions_arraysize);
    if (frameControlOptions_arraysize==0) {
        this->frameControlOptions_var = 0;
    } else {
        this->frameControlOptions_var = new bool[frameControlOptions_arraysize];
        doUnpacking(b,this->frameControlOptions_var,frameControlOptions_arraysize);
    }
    delete [] this->headerIElist_var;
    b->unpack(headerIElist_arraysize);
    if (headerIElist_arraysize==0) {
        this->headerIElist_var = 0;
    } else {
        this->headerIElist_var = new uint8_t[headerIElist_arraysize];
        doUnpacking(b,this->headerIElist_var,headerIElist_arraysize);
    }
    delete [] this->payloadIElist_var;
    b->unpack(payloadIElist_arraysize);
    if (payloadIElist_arraysize==0) {
        this->payloadIElist_var = 0;
    } else {
        this->payloadIElist_var = new uint8_t[payloadIElist_arraysize];
        doUnpacking(b,this->payloadIElist_var,payloadIElist_arraysize);
    }
    doUnpacking(b,this->pibAttributeValue_var);
    doUnpacking(b,this->panId_var);
    doUnpacking(b,this->startTime_var);
    doUnpacking(b,this->beaconOrder_var);
    doUnpacking(b,this->superframeOrder_var);
    doUnpacking(b,this->panCoordinator_var);
    doUnpacking(b,this->batteryLifeExtension_var);
    doUnpacking(b,this->coordRealignment_var);
    doUnpacking(b,this->coordRealignSecurityLevel_var);
    doUnpacking(b,this->coordRealignKeyIdMode_var);
    doUnpacking(b,this->coordRealignKeySource_var);
    doUnpacking(b,this->coordRealignKeyIndex_var);
    doUnpacking(b,this->beaconSecurityLevel_var);
    doUnpacking(b,this->beaconKeyIdMode_var);
    doUnpacking(b,this->beaconKeySource_var);
    doUnpacking(b,this->beaconKeyIndex_var);
    doUnpacking(b,this->dsmeSuperframeSpecification_var);
    doUnpacking(b,this->beaconBitmap_var);
    doUnpacking(b,this->hoppingSequenceLength_var);
    delete [] this->hoppingSequence_var;
    b->unpack(hoppingSequence_arraysize);
    if (hoppingSequence_arraysize==0) {
        this->hoppingSequence_var = 0;
    } else {
        this->hoppingSequence_var = new uint8_t[hoppingSequence_arraysize];
        doUnpacking(b,this->hoppingSequence_var,hoppingSequence_arraysize);
    }
    doUnpacking(b,this->channelOffsetBitmapLength_var);
    delete [] this->channelOffsetBitmap_var;
    b->unpack(channelOffsetBitmap_arraysize);
    if (channelOffsetBitmap_arraysize==0) {
        this->channelOffsetBitmap_var = 0;
    } else {
        this->channelOffsetBitmap_var = new uint8_t[channelOffsetBitmap_arraysize];
        doUnpacking(b,this->channelOffsetBitmap_var,channelOffsetBitmap_arraysize);
    }
    doUnpacking(b,this->trackBeacon_var);
    doUnpacking(b,this->txDPSIndex_var);
    doUnpacking(b,this->rxDPSIndex_var);
    doUnpacking(b,this->dpsIndexDuration_var);
    doUnpacking(b,this->beaconType_var);
    doUnpacking(b,this->channel_var);
    doUnpacking(b,this->dstAddrMode_var);
    doUnpacking(b,this->dstAddr_var);
    doUnpacking(b,this->bsnSuppression_var);
    doUnpacking(b,this->bsn_var);
    doUnpacking(b,this->finalCAPSlot_var);
    doUnpacking(b,this->associationPermit_var);
    doUnpacking(b,this->gtsPermit_var);
    doUnpacking(b,this->linkQuality_var);
    doUnpacking(b,this->timeStamp_var);
    doUnpacking(b,this->securityStatus_var);
    delete [] this->codeList_var;
    b->unpack(codeList_arraysize);
    if (codeList_arraysize==0) {
        this->codeList_var = 0;
    } else {
        this->codeList_var = new int[codeList_arraysize];
        doUnpacking(b,this->codeList_var,codeList_arraysize);
    }
    doUnpacking(b,this->nmbrShortAddrPending_var);
    doUnpacking(b,this->nmbrExtAddrPending_var);
    delete [] this->addrList_var;
    b->unpack(addrList_arraysize);
    if (addrList_arraysize==0) {
        this->addrList_var = 0;
    } else {
        this->addrList_var = new uint64_t[addrList_arraysize];
        doUnpacking(b,this->addrList_var,addrList_arraysize);
    }
    doUnpacking(b,this->sduLength_var);
    delete [] this->sdu_var;
    b->unpack(sdu_arraysize);
    if (sdu_arraysize==0) {
        this->sdu_var = 0;
    } else {
        this->sdu_var = new uint8_t[sdu_arraysize];
        doUnpacking(b,this->sdu_var,sdu_arraysize);
    }
    doUnpacking(b,this->ebsn_var);
    doUnpacking(b,this->srcAddrMode_var);
    doUnpacking(b,this->srcAddr_var);
    doUnpacking(b,this->gtsLength_var);
    doUnpacking(b,this->gtsDirection_var);
    doUnpacking(b,this->gtsCharType_var);
    doUnpacking(b,this->lossReason_var);
    doUnpacking(b,this->slotframeHandle_var);
    doUnpacking(b,this->operation_var);
    doUnpacking(b,this->size_var);
    doUnpacking(b,this->linkHandle_var);
    doUnpacking(b,this->timeslot_var);
    doUnpacking(b,this->linkOptions_var);
    doUnpacking(b,this->linkType_var);
    doUnpacking(b,this->nodeAddr_var);
    doUnpacking(b,this->tschMode_var);
    doUnpacking(b,this->keepAlivePeriod_var);
    delete [] this->lowLatencyNetworkConfiguration_var;
    b->unpack(lowLatencyNetworkConfiguration_arraysize);
    if (lowLatencyNetworkConfiguration_arraysize==0) {
        this->lowLatencyNetworkConfiguration_var = 0;
    } else {
        this->lowLatencyNetworkConfiguration_var = new uint8_t[lowLatencyNetworkConfiguration_arraysize];
        doUnpacking(b,this->lowLatencyNetworkConfiguration_var,lowLatencyNetworkConfiguration_arraysize);
    }
    doUnpacking(b,this->managementType_var);
    doUnpacking(b,this->direction_var);
    doUnpacking(b,this->prioritizedChannelAccess_var);
    doUnpacking(b,this->NumSlot_var);
    doUnpacking(b,this->preferredSuperframeID_var);
    doUnpacking(b,this->preferredSlotID_var);
    delete [] this->dsmeSABSpecification_var;
    b->unpack(dsmeSABSpecification_arraysize);
    if (dsmeSABSpecification_arraysize==0) {
        this->dsmeSABSpecification_var = 0;
    } else {
        this->dsmeSABSpecification_var = new uint8_t[dsmeSABSpecification_arraysize];
        doUnpacking(b,this->dsmeSABSpecification_var,dsmeSABSpecification_arraysize);
    }
    doUnpacking(b,this->info_var);
    doUnpacking(b,this->dsmeSABSubBlockLength_var);
    doUnpacking(b,this->dsmeSABSubBlockIndex_var);
    doUnpacking(b,this->ReportPeriod_var);
    doUnpacking(b,this->msduHandle_var);
    doUnpacking(b,this->rangingReceived_var);
    doUnpacking(b,this->rangingCounterStart_var);
    doUnpacking(b,this->rangingCounterStop_var);
    doUnpacking(b,this->rangingTrackingInterval_var);
    doUnpacking(b,this->rangingOffset_var);
    doUnpacking(b,this->rangingFOM_var);
    doUnpacking(b,this->numBackoffs_var);
    delete [] this->ackPayload_var;
    b->unpack(ackPayload_arraysize);
    if (ackPayload_arraysize==0) {
        this->ackPayload_var = 0;
    } else {
        this->ackPayload_var = new uint8_t[ackPayload_arraysize];
        doUnpacking(b,this->ackPayload_var,ackPayload_arraysize);
    }
    doUnpacking(b,this->receivedByACK_var);
    doUnpacking(b,this->stage_var);
    doUnpacking(b,this->WaitConstant_var);
    doUnpacking(b,this->txCS_var);
    doUnpacking(b,this->txLowerCH_var);
    doUnpacking(b,this->txHigherCH_var);
    doUnpacking(b,this->numberCH_var);
    doUnpacking(b,this->channelOffset10_var);
    doUnpacking(b,this->channelOffset11_var);
}

bool Ieee802154eNetworkCtrlInfo::getToParent() const
{
    return toParent_var;
}

void Ieee802154eNetworkCtrlInfo::setToParent(bool toParent)
{
    this->toParent_var = toParent;
}

const char * Ieee802154eNetworkCtrlInfo::getDestName() const
{
    return destName_var.c_str();
}

void Ieee802154eNetworkCtrlInfo::setDestName(const char * destName)
{
    this->destName_var = destName;
}

int64_t Ieee802154eNetworkCtrlInfo::getNetwAddr() const
{
    return netwAddr_var;
}

void Ieee802154eNetworkCtrlInfo::setNetwAddr(int64_t netwAddr)
{
    this->netwAddr_var = netwAddr;
}

uint8_t Ieee802154eNetworkCtrlInfo::getChannelNumber() const
{
    return channelNumber_var;
}

void Ieee802154eNetworkCtrlInfo::setChannelNumber(uint8_t channelNumber)
{
    this->channelNumber_var = channelNumber;
}

uint8_t Ieee802154eNetworkCtrlInfo::getChannelPage() const
{
    return channelPage_var;
}

void Ieee802154eNetworkCtrlInfo::setChannelPage(uint8_t channelPage)
{
    this->channelPage_var = channelPage;
}

uint8_t Ieee802154eNetworkCtrlInfo::getCoordAddrMode() const
{
    return coordAddrMode_var;
}

void Ieee802154eNetworkCtrlInfo::setCoordAddrMode(uint8_t coordAddrMode)
{
    this->coordAddrMode_var = coordAddrMode;
}

uint16_t Ieee802154eNetworkCtrlInfo::getCoordPANId() const
{
    return coordPANId_var;
}

void Ieee802154eNetworkCtrlInfo::setCoordPANId(uint16_t coordPANId)
{
    this->coordPANId_var = coordPANId;
}

uint64_t Ieee802154eNetworkCtrlInfo::getCoordAddress() const
{
    return coordAddress_var;
}

void Ieee802154eNetworkCtrlInfo::setCoordAddress(uint64_t coordAddress)
{
    this->coordAddress_var = coordAddress;
}

uint8_t Ieee802154eNetworkCtrlInfo::getCapabilityInformation() const
{
    return capabilityInformation_var;
}

void Ieee802154eNetworkCtrlInfo::setCapabilityInformation(uint8_t capabilityInformation)
{
    this->capabilityInformation_var = capabilityInformation;
}

uint8_t Ieee802154eNetworkCtrlInfo::getSecurityLevel() const
{
    return securityLevel_var;
}

void Ieee802154eNetworkCtrlInfo::setSecurityLevel(uint8_t securityLevel)
{
    this->securityLevel_var = securityLevel;
}

uint8_t Ieee802154eNetworkCtrlInfo::getKeyIdMode() const
{
    return keyIdMode_var;
}

void Ieee802154eNetworkCtrlInfo::setKeyIdMode(uint8_t keyIdMode)
{
    this->keyIdMode_var = keyIdMode;
}

uint64_t Ieee802154eNetworkCtrlInfo::getKeySource() const
{
    return keySource_var;
}

void Ieee802154eNetworkCtrlInfo::setKeySource(uint64_t keySource)
{
    this->keySource_var = keySource;
}

uint8_t Ieee802154eNetworkCtrlInfo::getKeyIndex() const
{
    return keyIndex_var;
}

void Ieee802154eNetworkCtrlInfo::setKeyIndex(uint8_t keyIndex)
{
    this->keyIndex_var = keyIndex;
}

uint64_t Ieee802154eNetworkCtrlInfo::getLowLatencyNetworkInfo() const
{
    return lowLatencyNetworkInfo_var;
}

void Ieee802154eNetworkCtrlInfo::setLowLatencyNetworkInfo(uint64_t lowLatencyNetworkInfo)
{
    this->lowLatencyNetworkInfo_var = lowLatencyNetworkInfo;
}

uint16_t Ieee802154eNetworkCtrlInfo::getChannelOffset() const
{
    return channelOffset_var;
}

void Ieee802154eNetworkCtrlInfo::setChannelOffset(uint16_t channelOffset)
{
    this->channelOffset_var = channelOffset;
}

uint8_t Ieee802154eNetworkCtrlInfo::getHoppingSequenceID() const
{
    return hoppingSequenceID_var;
}

void Ieee802154eNetworkCtrlInfo::setHoppingSequenceID(uint8_t hoppingSequenceID)
{
    this->hoppingSequenceID_var = hoppingSequenceID;
}

uint16_t Ieee802154eNetworkCtrlInfo::getAssocShortAddress() const
{
    return AssocShortAddress_var;
}

void Ieee802154eNetworkCtrlInfo::setAssocShortAddress(uint16_t AssocShortAddress)
{
    this->AssocShortAddress_var = AssocShortAddress;
}

short Ieee802154eNetworkCtrlInfo::getStatus() const
{
    return status_var;
}

void Ieee802154eNetworkCtrlInfo::setStatus(short status)
{
    this->status_var = status;
}

uint8_t Ieee802154eNetworkCtrlInfo::getDeviceAddrMode() const
{
    return deviceAddrMode_var;
}

void Ieee802154eNetworkCtrlInfo::setDeviceAddrMode(uint8_t deviceAddrMode)
{
    this->deviceAddrMode_var = deviceAddrMode;
}

uint16_t Ieee802154eNetworkCtrlInfo::getDevicePANId() const
{
    return devicePANId_var;
}

void Ieee802154eNetworkCtrlInfo::setDevicePANId(uint16_t devicePANId)
{
    this->devicePANId_var = devicePANId;
}

uint64_t Ieee802154eNetworkCtrlInfo::getDeviceAddress() const
{
    return deviceAddress_var;
}

void Ieee802154eNetworkCtrlInfo::setDeviceAddress(uint64_t deviceAddress)
{
    this->deviceAddress_var = deviceAddress;
}

uint8_t Ieee802154eNetworkCtrlInfo::getDisassociateReason() const
{
    return disassociateReason_var;
}

void Ieee802154eNetworkCtrlInfo::setDisassociateReason(uint8_t disassociateReason)
{
    this->disassociateReason_var = disassociateReason;
}

bool Ieee802154eNetworkCtrlInfo::getTxIndirect() const
{
    return txIndirect_var;
}

void Ieee802154eNetworkCtrlInfo::setTxIndirect(bool txIndirect)
{
    this->txIndirect_var = txIndirect;
}

uint8_t Ieee802154eNetworkCtrlInfo::getPibAttribute() const
{
    return pibAttribute_var;
}

void Ieee802154eNetworkCtrlInfo::setPibAttribute(uint8_t pibAttribute)
{
    this->pibAttribute_var = pibAttribute;
}

uint8_t Ieee802154eNetworkCtrlInfo::getGtsCharacteristic() const
{
    return gtsCharacteristic_var;
}

void Ieee802154eNetworkCtrlInfo::setGtsCharacteristic(uint8_t gtsCharacteristic)
{
    this->gtsCharacteristic_var = gtsCharacteristic;
}

bool Ieee802154eNetworkCtrlInfo::getSetDefaultPIB() const
{
    return setDefaultPIB_var;
}

void Ieee802154eNetworkCtrlInfo::setSetDefaultPIB(bool setDefaultPIB)
{
    this->setDefaultPIB_var = setDefaultPIB;
}

bool Ieee802154eNetworkCtrlInfo::getDeferPermit() const
{
    return deferPermit_var;
}

void Ieee802154eNetworkCtrlInfo::setDeferPermit(bool deferPermit)
{
    this->deferPermit_var = deferPermit;
}

uint32_t Ieee802154eNetworkCtrlInfo::getRxOnTime() const
{
    return rxOnTime_var;
}

void Ieee802154eNetworkCtrlInfo::setRxOnTime(uint32_t rxOnTime)
{
    this->rxOnTime_var = rxOnTime;
}

uint32_t Ieee802154eNetworkCtrlInfo::getRxOnDuration() const
{
    return rxOnDuration_var;
}

void Ieee802154eNetworkCtrlInfo::setRxOnDuration(uint32_t rxOnDuration)
{
    this->rxOnDuration_var = rxOnDuration;
}

uint8_t Ieee802154eNetworkCtrlInfo::getRangingRxControl() const
{
    return rangingRxControl_var;
}

void Ieee802154eNetworkCtrlInfo::setRangingRxControl(uint8_t rangingRxControl)
{
    this->rangingRxControl_var = rangingRxControl;
}

uint8_t Ieee802154eNetworkCtrlInfo::getScanType() const
{
    return scanType_var;
}

void Ieee802154eNetworkCtrlInfo::setScanType(uint8_t scanType)
{
    this->scanType_var = scanType;
}

uint64_t Ieee802154eNetworkCtrlInfo::getScanChannels() const
{
    return scanChannels_var;
}

void Ieee802154eNetworkCtrlInfo::setScanChannels(uint64_t scanChannels)
{
    this->scanChannels_var = scanChannels;
}

uint8_t Ieee802154eNetworkCtrlInfo::getScanDuration() const
{
    return scanDuration_var;
}

void Ieee802154eNetworkCtrlInfo::setScanDuration(uint8_t scanDuration)
{
    this->scanDuration_var = scanDuration;
}

bool Ieee802154eNetworkCtrlInfo::getLinkQualityScan() const
{
    return linkQualityScan_var;
}

void Ieee802154eNetworkCtrlInfo::setLinkQualityScan(bool linkQualityScan)
{
    this->linkQualityScan_var = linkQualityScan;
}

void Ieee802154eNetworkCtrlInfo::setFrameControlOptionsArraySize(unsigned int size)
{
    bool *frameControlOptions_var2 = (size==0) ? NULL : new bool[size];
    unsigned int sz = frameControlOptions_arraysize < size ? frameControlOptions_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        frameControlOptions_var2[i] = this->frameControlOptions_var[i];
    for (unsigned int i=sz; i<size; i++)
        frameControlOptions_var2[i] = 0;
    frameControlOptions_arraysize = size;
    delete [] this->frameControlOptions_var;
    this->frameControlOptions_var = frameControlOptions_var2;
}

unsigned int Ieee802154eNetworkCtrlInfo::getFrameControlOptionsArraySize() const
{
    return frameControlOptions_arraysize;
}

bool Ieee802154eNetworkCtrlInfo::getFrameControlOptions(unsigned int k) const
{
    if (k>=frameControlOptions_arraysize) throw cRuntimeError("Array of size %d indexed by %d", frameControlOptions_arraysize, k);
    return frameControlOptions_var[k];
}

void Ieee802154eNetworkCtrlInfo::setFrameControlOptions(unsigned int k, bool frameControlOptions)
{
    if (k>=frameControlOptions_arraysize) throw cRuntimeError("Array of size %d indexed by %d", frameControlOptions_arraysize, k);
    this->frameControlOptions_var[k] = frameControlOptions;
}

void Ieee802154eNetworkCtrlInfo::setHeaderIElistArraySize(unsigned int size)
{
    uint8_t *headerIElist_var2 = (size==0) ? NULL : new uint8_t[size];
    unsigned int sz = headerIElist_arraysize < size ? headerIElist_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        headerIElist_var2[i] = this->headerIElist_var[i];
    for (unsigned int i=sz; i<size; i++)
        headerIElist_var2[i] = 0;
    headerIElist_arraysize = size;
    delete [] this->headerIElist_var;
    this->headerIElist_var = headerIElist_var2;
}

unsigned int Ieee802154eNetworkCtrlInfo::getHeaderIElistArraySize() const
{
    return headerIElist_arraysize;
}

uint8_t Ieee802154eNetworkCtrlInfo::getHeaderIElist(unsigned int k) const
{
    if (k>=headerIElist_arraysize) throw cRuntimeError("Array of size %d indexed by %d", headerIElist_arraysize, k);
    return headerIElist_var[k];
}

void Ieee802154eNetworkCtrlInfo::setHeaderIElist(unsigned int k, uint8_t headerIElist)
{
    if (k>=headerIElist_arraysize) throw cRuntimeError("Array of size %d indexed by %d", headerIElist_arraysize, k);
    this->headerIElist_var[k] = headerIElist;
}

void Ieee802154eNetworkCtrlInfo::setPayloadIElistArraySize(unsigned int size)
{
    uint8_t *payloadIElist_var2 = (size==0) ? NULL : new uint8_t[size];
    unsigned int sz = payloadIElist_arraysize < size ? payloadIElist_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        payloadIElist_var2[i] = this->payloadIElist_var[i];
    for (unsigned int i=sz; i<size; i++)
        payloadIElist_var2[i] = 0;
    payloadIElist_arraysize = size;
    delete [] this->payloadIElist_var;
    this->payloadIElist_var = payloadIElist_var2;
}

unsigned int Ieee802154eNetworkCtrlInfo::getPayloadIElistArraySize() const
{
    return payloadIElist_arraysize;
}

uint8_t Ieee802154eNetworkCtrlInfo::getPayloadIElist(unsigned int k) const
{
    if (k>=payloadIElist_arraysize) throw cRuntimeError("Array of size %d indexed by %d", payloadIElist_arraysize, k);
    return payloadIElist_var[k];
}

void Ieee802154eNetworkCtrlInfo::setPayloadIElist(unsigned int k, uint8_t payloadIElist)
{
    if (k>=payloadIElist_arraysize) throw cRuntimeError("Array of size %d indexed by %d", payloadIElist_arraysize, k);
    this->payloadIElist_var[k] = payloadIElist;
}

MAC_PIB& Ieee802154eNetworkCtrlInfo::getPibAttributeValue()
{
    return pibAttributeValue_var;
}

void Ieee802154eNetworkCtrlInfo::setPibAttributeValue(const MAC_PIB& pibAttributeValue)
{
    this->pibAttributeValue_var = pibAttributeValue;
}

uint16_t Ieee802154eNetworkCtrlInfo::getPanId() const
{
    return panId_var;
}

void Ieee802154eNetworkCtrlInfo::setPanId(uint16_t panId)
{
    this->panId_var = panId;
}

uint32_t Ieee802154eNetworkCtrlInfo::getStartTime() const
{
    return startTime_var;
}

void Ieee802154eNetworkCtrlInfo::setStartTime(uint32_t startTime)
{
    this->startTime_var = startTime;
}

uint8_t Ieee802154eNetworkCtrlInfo::getBeaconOrder() const
{
    return beaconOrder_var;
}

void Ieee802154eNetworkCtrlInfo::setBeaconOrder(uint8_t beaconOrder)
{
    this->beaconOrder_var = beaconOrder;
}

uint8_t Ieee802154eNetworkCtrlInfo::getSuperframeOrder() const
{
    return superframeOrder_var;
}

void Ieee802154eNetworkCtrlInfo::setSuperframeOrder(uint8_t superframeOrder)
{
    this->superframeOrder_var = superframeOrder;
}

bool Ieee802154eNetworkCtrlInfo::getPanCoordinator() const
{
    return panCoordinator_var;
}

void Ieee802154eNetworkCtrlInfo::setPanCoordinator(bool panCoordinator)
{
    this->panCoordinator_var = panCoordinator;
}

bool Ieee802154eNetworkCtrlInfo::getBatteryLifeExtension() const
{
    return batteryLifeExtension_var;
}

void Ieee802154eNetworkCtrlInfo::setBatteryLifeExtension(bool batteryLifeExtension)
{
    this->batteryLifeExtension_var = batteryLifeExtension;
}

bool Ieee802154eNetworkCtrlInfo::getCoordRealignment() const
{
    return coordRealignment_var;
}

void Ieee802154eNetworkCtrlInfo::setCoordRealignment(bool coordRealignment)
{
    this->coordRealignment_var = coordRealignment;
}

uint8_t Ieee802154eNetworkCtrlInfo::getCoordRealignSecurityLevel() const
{
    return coordRealignSecurityLevel_var;
}

void Ieee802154eNetworkCtrlInfo::setCoordRealignSecurityLevel(uint8_t coordRealignSecurityLevel)
{
    this->coordRealignSecurityLevel_var = coordRealignSecurityLevel;
}

uint8_t Ieee802154eNetworkCtrlInfo::getCoordRealignKeyIdMode() const
{
    return coordRealignKeyIdMode_var;
}

void Ieee802154eNetworkCtrlInfo::setCoordRealignKeyIdMode(uint8_t coordRealignKeyIdMode)
{
    this->coordRealignKeyIdMode_var = coordRealignKeyIdMode;
}

uint64_t Ieee802154eNetworkCtrlInfo::getCoordRealignKeySource() const
{
    return coordRealignKeySource_var;
}

void Ieee802154eNetworkCtrlInfo::setCoordRealignKeySource(uint64_t coordRealignKeySource)
{
    this->coordRealignKeySource_var = coordRealignKeySource;
}

uint8_t Ieee802154eNetworkCtrlInfo::getCoordRealignKeyIndex() const
{
    return coordRealignKeyIndex_var;
}

void Ieee802154eNetworkCtrlInfo::setCoordRealignKeyIndex(uint8_t coordRealignKeyIndex)
{
    this->coordRealignKeyIndex_var = coordRealignKeyIndex;
}

uint8_t Ieee802154eNetworkCtrlInfo::getBeaconSecurityLevel() const
{
    return beaconSecurityLevel_var;
}

void Ieee802154eNetworkCtrlInfo::setBeaconSecurityLevel(uint8_t beaconSecurityLevel)
{
    this->beaconSecurityLevel_var = beaconSecurityLevel;
}

uint8_t Ieee802154eNetworkCtrlInfo::getBeaconKeyIdMode() const
{
    return beaconKeyIdMode_var;
}

void Ieee802154eNetworkCtrlInfo::setBeaconKeyIdMode(uint8_t beaconKeyIdMode)
{
    this->beaconKeyIdMode_var = beaconKeyIdMode;
}

uint64_t Ieee802154eNetworkCtrlInfo::getBeaconKeySource() const
{
    return beaconKeySource_var;
}

void Ieee802154eNetworkCtrlInfo::setBeaconKeySource(uint64_t beaconKeySource)
{
    this->beaconKeySource_var = beaconKeySource;
}

uint8_t Ieee802154eNetworkCtrlInfo::getBeaconKeyIndex() const
{
    return beaconKeyIndex_var;
}

void Ieee802154eNetworkCtrlInfo::setBeaconKeyIndex(uint8_t beaconKeyIndex)
{
    this->beaconKeyIndex_var = beaconKeyIndex;
}

uint8_t Ieee802154eNetworkCtrlInfo::getDsmeSuperframeSpecification() const
{
    return dsmeSuperframeSpecification_var;
}

void Ieee802154eNetworkCtrlInfo::setDsmeSuperframeSpecification(uint8_t dsmeSuperframeSpecification)
{
    this->dsmeSuperframeSpecification_var = dsmeSuperframeSpecification;
}

uint64_t Ieee802154eNetworkCtrlInfo::getBeaconBitmap() const
{
    return beaconBitmap_var;
}

void Ieee802154eNetworkCtrlInfo::setBeaconBitmap(uint64_t beaconBitmap)
{
    this->beaconBitmap_var = beaconBitmap;
}

uint16_t Ieee802154eNetworkCtrlInfo::getHoppingSequenceLength() const
{
    return hoppingSequenceLength_var;
}

void Ieee802154eNetworkCtrlInfo::setHoppingSequenceLength(uint16_t hoppingSequenceLength)
{
    this->hoppingSequenceLength_var = hoppingSequenceLength;
}

void Ieee802154eNetworkCtrlInfo::setHoppingSequenceArraySize(unsigned int size)
{
    uint8_t *hoppingSequence_var2 = (size==0) ? NULL : new uint8_t[size];
    unsigned int sz = hoppingSequence_arraysize < size ? hoppingSequence_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        hoppingSequence_var2[i] = this->hoppingSequence_var[i];
    for (unsigned int i=sz; i<size; i++)
        hoppingSequence_var2[i] = 0;
    hoppingSequence_arraysize = size;
    delete [] this->hoppingSequence_var;
    this->hoppingSequence_var = hoppingSequence_var2;
}

unsigned int Ieee802154eNetworkCtrlInfo::getHoppingSequenceArraySize() const
{
    return hoppingSequence_arraysize;
}

uint8_t Ieee802154eNetworkCtrlInfo::getHoppingSequence(unsigned int k) const
{
    if (k>=hoppingSequence_arraysize) throw cRuntimeError("Array of size %d indexed by %d", hoppingSequence_arraysize, k);
    return hoppingSequence_var[k];
}

void Ieee802154eNetworkCtrlInfo::setHoppingSequence(unsigned int k, uint8_t hoppingSequence)
{
    if (k>=hoppingSequence_arraysize) throw cRuntimeError("Array of size %d indexed by %d", hoppingSequence_arraysize, k);
    this->hoppingSequence_var[k] = hoppingSequence;
}

uint16_t Ieee802154eNetworkCtrlInfo::getChannelOffsetBitmapLength() const
{
    return channelOffsetBitmapLength_var;
}

void Ieee802154eNetworkCtrlInfo::setChannelOffsetBitmapLength(uint16_t channelOffsetBitmapLength)
{
    this->channelOffsetBitmapLength_var = channelOffsetBitmapLength;
}

void Ieee802154eNetworkCtrlInfo::setChannelOffsetBitmapArraySize(unsigned int size)
{
    uint8_t *channelOffsetBitmap_var2 = (size==0) ? NULL : new uint8_t[size];
    unsigned int sz = channelOffsetBitmap_arraysize < size ? channelOffsetBitmap_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        channelOffsetBitmap_var2[i] = this->channelOffsetBitmap_var[i];
    for (unsigned int i=sz; i<size; i++)
        channelOffsetBitmap_var2[i] = 0;
    channelOffsetBitmap_arraysize = size;
    delete [] this->channelOffsetBitmap_var;
    this->channelOffsetBitmap_var = channelOffsetBitmap_var2;
}

unsigned int Ieee802154eNetworkCtrlInfo::getChannelOffsetBitmapArraySize() const
{
    return channelOffsetBitmap_arraysize;
}

uint8_t Ieee802154eNetworkCtrlInfo::getChannelOffsetBitmap(unsigned int k) const
{
    if (k>=channelOffsetBitmap_arraysize) throw cRuntimeError("Array of size %d indexed by %d", channelOffsetBitmap_arraysize, k);
    return channelOffsetBitmap_var[k];
}

void Ieee802154eNetworkCtrlInfo::setChannelOffsetBitmap(unsigned int k, uint8_t channelOffsetBitmap)
{
    if (k>=channelOffsetBitmap_arraysize) throw cRuntimeError("Array of size %d indexed by %d", channelOffsetBitmap_arraysize, k);
    this->channelOffsetBitmap_var[k] = channelOffsetBitmap;
}

bool Ieee802154eNetworkCtrlInfo::getTrackBeacon() const
{
    return trackBeacon_var;
}

void Ieee802154eNetworkCtrlInfo::setTrackBeacon(bool trackBeacon)
{
    this->trackBeacon_var = trackBeacon;
}

uint8_t Ieee802154eNetworkCtrlInfo::getTxDPSIndex() const
{
    return txDPSIndex_var;
}

void Ieee802154eNetworkCtrlInfo::setTxDPSIndex(uint8_t txDPSIndex)
{
    this->txDPSIndex_var = txDPSIndex;
}

uint8_t Ieee802154eNetworkCtrlInfo::getRxDPSIndex() const
{
    return rxDPSIndex_var;
}

void Ieee802154eNetworkCtrlInfo::setRxDPSIndex(uint8_t rxDPSIndex)
{
    this->rxDPSIndex_var = rxDPSIndex;
}

uint32_t Ieee802154eNetworkCtrlInfo::getDpsIndexDuration() const
{
    return dpsIndexDuration_var;
}

void Ieee802154eNetworkCtrlInfo::setDpsIndexDuration(uint32_t dpsIndexDuration)
{
    this->dpsIndexDuration_var = dpsIndexDuration;
}

uint8_t Ieee802154eNetworkCtrlInfo::getBeaconType() const
{
    return beaconType_var;
}

void Ieee802154eNetworkCtrlInfo::setBeaconType(uint8_t beaconType)
{
    this->beaconType_var = beaconType;
}

uint8_t Ieee802154eNetworkCtrlInfo::getChannel() const
{
    return channel_var;
}

void Ieee802154eNetworkCtrlInfo::setChannel(uint8_t channel)
{
    this->channel_var = channel;
}

uint8_t Ieee802154eNetworkCtrlInfo::getDstAddrMode() const
{
    return dstAddrMode_var;
}

void Ieee802154eNetworkCtrlInfo::setDstAddrMode(uint8_t dstAddrMode)
{
    this->dstAddrMode_var = dstAddrMode;
}

int64_t Ieee802154eNetworkCtrlInfo::getDstAddr() const
{
    return dstAddr_var;
}

void Ieee802154eNetworkCtrlInfo::setDstAddr(int64_t dstAddr)
{
    this->dstAddr_var = dstAddr;
}

bool Ieee802154eNetworkCtrlInfo::getBsnSuppression() const
{
    return bsnSuppression_var;
}

void Ieee802154eNetworkCtrlInfo::setBsnSuppression(bool bsnSuppression)
{
    this->bsnSuppression_var = bsnSuppression;
}

uint8_t Ieee802154eNetworkCtrlInfo::getBsn() const
{
    return bsn_var;
}

void Ieee802154eNetworkCtrlInfo::setBsn(uint8_t bsn)
{
    this->bsn_var = bsn;
}

uint8_t Ieee802154eNetworkCtrlInfo::getFinalCAPSlot() const
{
    return finalCAPSlot_var;
}

void Ieee802154eNetworkCtrlInfo::setFinalCAPSlot(uint8_t finalCAPSlot)
{
    this->finalCAPSlot_var = finalCAPSlot;
}

bool Ieee802154eNetworkCtrlInfo::getAssociationPermit() const
{
    return associationPermit_var;
}

void Ieee802154eNetworkCtrlInfo::setAssociationPermit(bool associationPermit)
{
    this->associationPermit_var = associationPermit;
}

bool Ieee802154eNetworkCtrlInfo::getGtsPermit() const
{
    return gtsPermit_var;
}

void Ieee802154eNetworkCtrlInfo::setGtsPermit(bool gtsPermit)
{
    this->gtsPermit_var = gtsPermit;
}

uint8_t Ieee802154eNetworkCtrlInfo::getLinkQuality() const
{
    return linkQuality_var;
}

void Ieee802154eNetworkCtrlInfo::setLinkQuality(uint8_t linkQuality)
{
    this->linkQuality_var = linkQuality;
}

uint64_t Ieee802154eNetworkCtrlInfo::getTimeStamp() const
{
    return timeStamp_var;
}

void Ieee802154eNetworkCtrlInfo::setTimeStamp(uint64_t timeStamp)
{
    this->timeStamp_var = timeStamp;
}

short Ieee802154eNetworkCtrlInfo::getSecurityStatus() const
{
    return securityStatus_var;
}

void Ieee802154eNetworkCtrlInfo::setSecurityStatus(short securityStatus)
{
    this->securityStatus_var = securityStatus;
}

void Ieee802154eNetworkCtrlInfo::setCodeListArraySize(unsigned int size)
{
    int *codeList_var2 = (size==0) ? NULL : new int[size];
    unsigned int sz = codeList_arraysize < size ? codeList_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        codeList_var2[i] = this->codeList_var[i];
    for (unsigned int i=sz; i<size; i++)
        codeList_var2[i] = 0;
    codeList_arraysize = size;
    delete [] this->codeList_var;
    this->codeList_var = codeList_var2;
}

unsigned int Ieee802154eNetworkCtrlInfo::getCodeListArraySize() const
{
    return codeList_arraysize;
}

int Ieee802154eNetworkCtrlInfo::getCodeList(unsigned int k) const
{
    if (k>=codeList_arraysize) throw cRuntimeError("Array of size %d indexed by %d", codeList_arraysize, k);
    return codeList_var[k];
}

void Ieee802154eNetworkCtrlInfo::setCodeList(unsigned int k, int codeList)
{
    if (k>=codeList_arraysize) throw cRuntimeError("Array of size %d indexed by %d", codeList_arraysize, k);
    this->codeList_var[k] = codeList;
}

uint8_t Ieee802154eNetworkCtrlInfo::getNmbrShortAddrPending() const
{
    return nmbrShortAddrPending_var;
}

void Ieee802154eNetworkCtrlInfo::setNmbrShortAddrPending(uint8_t nmbrShortAddrPending)
{
    this->nmbrShortAddrPending_var = nmbrShortAddrPending;
}

uint8_t Ieee802154eNetworkCtrlInfo::getNmbrExtAddrPending() const
{
    return nmbrExtAddrPending_var;
}

void Ieee802154eNetworkCtrlInfo::setNmbrExtAddrPending(uint8_t nmbrExtAddrPending)
{
    this->nmbrExtAddrPending_var = nmbrExtAddrPending;
}

void Ieee802154eNetworkCtrlInfo::setAddrListArraySize(unsigned int size)
{
    uint64_t *addrList_var2 = (size==0) ? NULL : new uint64_t[size];
    unsigned int sz = addrList_arraysize < size ? addrList_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        addrList_var2[i] = this->addrList_var[i];
    for (unsigned int i=sz; i<size; i++)
        addrList_var2[i] = 0;
    addrList_arraysize = size;
    delete [] this->addrList_var;
    this->addrList_var = addrList_var2;
}

unsigned int Ieee802154eNetworkCtrlInfo::getAddrListArraySize() const
{
    return addrList_arraysize;
}

uint64_t Ieee802154eNetworkCtrlInfo::getAddrList(unsigned int k) const
{
    if (k>=addrList_arraysize) throw cRuntimeError("Array of size %d indexed by %d", addrList_arraysize, k);
    return addrList_var[k];
}

void Ieee802154eNetworkCtrlInfo::setAddrList(unsigned int k, uint64_t addrList)
{
    if (k>=addrList_arraysize) throw cRuntimeError("Array of size %d indexed by %d", addrList_arraysize, k);
    this->addrList_var[k] = addrList;
}

int Ieee802154eNetworkCtrlInfo::getSduLength() const
{
    return sduLength_var;
}

void Ieee802154eNetworkCtrlInfo::setSduLength(int sduLength)
{
    this->sduLength_var = sduLength;
}

void Ieee802154eNetworkCtrlInfo::setSduArraySize(unsigned int size)
{
    uint8_t *sdu_var2 = (size==0) ? NULL : new uint8_t[size];
    unsigned int sz = sdu_arraysize < size ? sdu_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        sdu_var2[i] = this->sdu_var[i];
    for (unsigned int i=sz; i<size; i++)
        sdu_var2[i] = 0;
    sdu_arraysize = size;
    delete [] this->sdu_var;
    this->sdu_var = sdu_var2;
}

unsigned int Ieee802154eNetworkCtrlInfo::getSduArraySize() const
{
    return sdu_arraysize;
}

uint8_t Ieee802154eNetworkCtrlInfo::getSdu(unsigned int k) const
{
    if (k>=sdu_arraysize) throw cRuntimeError("Array of size %d indexed by %d", sdu_arraysize, k);
    return sdu_var[k];
}

void Ieee802154eNetworkCtrlInfo::setSdu(unsigned int k, uint8_t sdu)
{
    if (k>=sdu_arraysize) throw cRuntimeError("Array of size %d indexed by %d", sdu_arraysize, k);
    this->sdu_var[k] = sdu;
}

uint8_t Ieee802154eNetworkCtrlInfo::getEbsn() const
{
    return ebsn_var;
}

void Ieee802154eNetworkCtrlInfo::setEbsn(uint8_t ebsn)
{
    this->ebsn_var = ebsn;
}

uint8_t Ieee802154eNetworkCtrlInfo::getSrcAddrMode() const
{
    return srcAddrMode_var;
}

void Ieee802154eNetworkCtrlInfo::setSrcAddrMode(uint8_t srcAddrMode)
{
    this->srcAddrMode_var = srcAddrMode;
}

uint64_t Ieee802154eNetworkCtrlInfo::getSrcAddr() const
{
    return srcAddr_var;
}

void Ieee802154eNetworkCtrlInfo::setSrcAddr(uint64_t srcAddr)
{
    this->srcAddr_var = srcAddr;
}

uint8_t Ieee802154eNetworkCtrlInfo::getGtsLength() const
{
    return gtsLength_var;
}

void Ieee802154eNetworkCtrlInfo::setGtsLength(uint8_t gtsLength)
{
    this->gtsLength_var = gtsLength;
}

bool Ieee802154eNetworkCtrlInfo::getGtsDirection() const
{
    return gtsDirection_var;
}

void Ieee802154eNetworkCtrlInfo::setGtsDirection(bool gtsDirection)
{
    this->gtsDirection_var = gtsDirection;
}

bool Ieee802154eNetworkCtrlInfo::getGtsCharType() const
{
    return gtsCharType_var;
}

void Ieee802154eNetworkCtrlInfo::setGtsCharType(bool gtsCharType)
{
    this->gtsCharType_var = gtsCharType;
}

uint8_t Ieee802154eNetworkCtrlInfo::getLossReason() const
{
    return lossReason_var;
}

void Ieee802154eNetworkCtrlInfo::setLossReason(uint8_t lossReason)
{
    this->lossReason_var = lossReason;
}

uint8_t Ieee802154eNetworkCtrlInfo::getSlotframeHandle() const
{
    return slotframeHandle_var;
}

void Ieee802154eNetworkCtrlInfo::setSlotframeHandle(uint8_t slotframeHandle)
{
    this->slotframeHandle_var = slotframeHandle;
}

uint8_t Ieee802154eNetworkCtrlInfo::getOperation() const
{
    return operation_var;
}

void Ieee802154eNetworkCtrlInfo::setOperation(uint8_t operation)
{
    this->operation_var = operation;
}

uint16_t Ieee802154eNetworkCtrlInfo::getSize() const
{
    return size_var;
}

void Ieee802154eNetworkCtrlInfo::setSize(uint16_t size)
{
    this->size_var = size;
}

uint16_t Ieee802154eNetworkCtrlInfo::getLinkHandle() const
{
    return linkHandle_var;
}

void Ieee802154eNetworkCtrlInfo::setLinkHandle(uint16_t linkHandle)
{
    this->linkHandle_var = linkHandle;
}

uint16_t Ieee802154eNetworkCtrlInfo::getTimeslot() const
{
    return timeslot_var;
}

void Ieee802154eNetworkCtrlInfo::setTimeslot(uint16_t timeslot)
{
    this->timeslot_var = timeslot;
}

uint8_t Ieee802154eNetworkCtrlInfo::getLinkOptions() const
{
    return linkOptions_var;
}

void Ieee802154eNetworkCtrlInfo::setLinkOptions(uint8_t linkOptions)
{
    this->linkOptions_var = linkOptions;
}

uint8_t Ieee802154eNetworkCtrlInfo::getLinkType() const
{
    return linkType_var;
}

void Ieee802154eNetworkCtrlInfo::setLinkType(uint8_t linkType)
{
    this->linkType_var = linkType;
}

uint16_t Ieee802154eNetworkCtrlInfo::getNodeAddr() const
{
    return nodeAddr_var;
}

void Ieee802154eNetworkCtrlInfo::setNodeAddr(uint16_t nodeAddr)
{
    this->nodeAddr_var = nodeAddr;
}

uint8_t Ieee802154eNetworkCtrlInfo::getTschMode() const
{
    return tschMode_var;
}

void Ieee802154eNetworkCtrlInfo::setTschMode(uint8_t tschMode)
{
    this->tschMode_var = tschMode;
}

uint16_t Ieee802154eNetworkCtrlInfo::getKeepAlivePeriod() const
{
    return keepAlivePeriod_var;
}

void Ieee802154eNetworkCtrlInfo::setKeepAlivePeriod(uint16_t keepAlivePeriod)
{
    this->keepAlivePeriod_var = keepAlivePeriod;
}

void Ieee802154eNetworkCtrlInfo::setLowLatencyNetworkConfigurationArraySize(unsigned int size)
{
    uint8_t *lowLatencyNetworkConfiguration_var2 = (size==0) ? NULL : new uint8_t[size];
    unsigned int sz = lowLatencyNetworkConfiguration_arraysize < size ? lowLatencyNetworkConfiguration_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        lowLatencyNetworkConfiguration_var2[i] = this->lowLatencyNetworkConfiguration_var[i];
    for (unsigned int i=sz; i<size; i++)
        lowLatencyNetworkConfiguration_var2[i] = 0;
    lowLatencyNetworkConfiguration_arraysize = size;
    delete [] this->lowLatencyNetworkConfiguration_var;
    this->lowLatencyNetworkConfiguration_var = lowLatencyNetworkConfiguration_var2;
}

unsigned int Ieee802154eNetworkCtrlInfo::getLowLatencyNetworkConfigurationArraySize() const
{
    return lowLatencyNetworkConfiguration_arraysize;
}

uint8_t Ieee802154eNetworkCtrlInfo::getLowLatencyNetworkConfiguration(unsigned int k) const
{
    if (k>=lowLatencyNetworkConfiguration_arraysize) throw cRuntimeError("Array of size %d indexed by %d", lowLatencyNetworkConfiguration_arraysize, k);
    return lowLatencyNetworkConfiguration_var[k];
}

void Ieee802154eNetworkCtrlInfo::setLowLatencyNetworkConfiguration(unsigned int k, uint8_t lowLatencyNetworkConfiguration)
{
    if (k>=lowLatencyNetworkConfiguration_arraysize) throw cRuntimeError("Array of size %d indexed by %d", lowLatencyNetworkConfiguration_arraysize, k);
    this->lowLatencyNetworkConfiguration_var[k] = lowLatencyNetworkConfiguration;
}

uint8_t Ieee802154eNetworkCtrlInfo::getManagementType() const
{
    return managementType_var;
}

void Ieee802154eNetworkCtrlInfo::setManagementType(uint8_t managementType)
{
    this->managementType_var = managementType;
}

uint8_t Ieee802154eNetworkCtrlInfo::getDirection() const
{
    return direction_var;
}

void Ieee802154eNetworkCtrlInfo::setDirection(uint8_t direction)
{
    this->direction_var = direction;
}

uint8_t Ieee802154eNetworkCtrlInfo::getPrioritizedChannelAccess() const
{
    return prioritizedChannelAccess_var;
}

void Ieee802154eNetworkCtrlInfo::setPrioritizedChannelAccess(uint8_t prioritizedChannelAccess)
{
    this->prioritizedChannelAccess_var = prioritizedChannelAccess;
}

uint8_t Ieee802154eNetworkCtrlInfo::getNumSlot() const
{
    return NumSlot_var;
}

void Ieee802154eNetworkCtrlInfo::setNumSlot(uint8_t NumSlot)
{
    this->NumSlot_var = NumSlot;
}

uint16_t Ieee802154eNetworkCtrlInfo::getPreferredSuperframeID() const
{
    return preferredSuperframeID_var;
}

void Ieee802154eNetworkCtrlInfo::setPreferredSuperframeID(uint16_t preferredSuperframeID)
{
    this->preferredSuperframeID_var = preferredSuperframeID;
}

uint8_t Ieee802154eNetworkCtrlInfo::getPreferredSlotID() const
{
    return preferredSlotID_var;
}

void Ieee802154eNetworkCtrlInfo::setPreferredSlotID(uint8_t preferredSlotID)
{
    this->preferredSlotID_var = preferredSlotID;
}

void Ieee802154eNetworkCtrlInfo::setDsmeSABSpecificationArraySize(unsigned int size)
{
    uint8_t *dsmeSABSpecification_var2 = (size==0) ? NULL : new uint8_t[size];
    unsigned int sz = dsmeSABSpecification_arraysize < size ? dsmeSABSpecification_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        dsmeSABSpecification_var2[i] = this->dsmeSABSpecification_var[i];
    for (unsigned int i=sz; i<size; i++)
        dsmeSABSpecification_var2[i] = 0;
    dsmeSABSpecification_arraysize = size;
    delete [] this->dsmeSABSpecification_var;
    this->dsmeSABSpecification_var = dsmeSABSpecification_var2;
}

unsigned int Ieee802154eNetworkCtrlInfo::getDsmeSABSpecificationArraySize() const
{
    return dsmeSABSpecification_arraysize;
}

uint8_t Ieee802154eNetworkCtrlInfo::getDsmeSABSpecification(unsigned int k) const
{
    if (k>=dsmeSABSpecification_arraysize) throw cRuntimeError("Array of size %d indexed by %d", dsmeSABSpecification_arraysize, k);
    return dsmeSABSpecification_var[k];
}

void Ieee802154eNetworkCtrlInfo::setDsmeSABSpecification(unsigned int k, uint8_t dsmeSABSpecification)
{
    if (k>=dsmeSABSpecification_arraysize) throw cRuntimeError("Array of size %d indexed by %d", dsmeSABSpecification_arraysize, k);
    this->dsmeSABSpecification_var[k] = dsmeSABSpecification;
}

uint8_t Ieee802154eNetworkCtrlInfo::getInfo() const
{
    return info_var;
}

void Ieee802154eNetworkCtrlInfo::setInfo(uint8_t info)
{
    this->info_var = info;
}

uint8_t Ieee802154eNetworkCtrlInfo::getDsmeSABSubBlockLength() const
{
    return dsmeSABSubBlockLength_var;
}

void Ieee802154eNetworkCtrlInfo::setDsmeSABSubBlockLength(uint8_t dsmeSABSubBlockLength)
{
    this->dsmeSABSubBlockLength_var = dsmeSABSubBlockLength;
}

uint8_t Ieee802154eNetworkCtrlInfo::getDsmeSABSubBlockIndex() const
{
    return dsmeSABSubBlockIndex_var;
}

void Ieee802154eNetworkCtrlInfo::setDsmeSABSubBlockIndex(uint8_t dsmeSABSubBlockIndex)
{
    this->dsmeSABSubBlockIndex_var = dsmeSABSubBlockIndex;
}

uint32_t Ieee802154eNetworkCtrlInfo::getReportPeriod() const
{
    return ReportPeriod_var;
}

void Ieee802154eNetworkCtrlInfo::setReportPeriod(uint32_t ReportPeriod)
{
    this->ReportPeriod_var = ReportPeriod;
}

uint8_t Ieee802154eNetworkCtrlInfo::getMsduHandle() const
{
    return msduHandle_var;
}

void Ieee802154eNetworkCtrlInfo::setMsduHandle(uint8_t msduHandle)
{
    this->msduHandle_var = msduHandle;
}

bool Ieee802154eNetworkCtrlInfo::getRangingReceived() const
{
    return rangingReceived_var;
}

void Ieee802154eNetworkCtrlInfo::setRangingReceived(bool rangingReceived)
{
    this->rangingReceived_var = rangingReceived;
}

uint32_t Ieee802154eNetworkCtrlInfo::getRangingCounterStart() const
{
    return rangingCounterStart_var;
}

void Ieee802154eNetworkCtrlInfo::setRangingCounterStart(uint32_t rangingCounterStart)
{
    this->rangingCounterStart_var = rangingCounterStart;
}

uint32_t Ieee802154eNetworkCtrlInfo::getRangingCounterStop() const
{
    return rangingCounterStop_var;
}

void Ieee802154eNetworkCtrlInfo::setRangingCounterStop(uint32_t rangingCounterStop)
{
    this->rangingCounterStop_var = rangingCounterStop;
}

uint32_t Ieee802154eNetworkCtrlInfo::getRangingTrackingInterval() const
{
    return rangingTrackingInterval_var;
}

void Ieee802154eNetworkCtrlInfo::setRangingTrackingInterval(uint32_t rangingTrackingInterval)
{
    this->rangingTrackingInterval_var = rangingTrackingInterval;
}

uint32_t Ieee802154eNetworkCtrlInfo::getRangingOffset() const
{
    return rangingOffset_var;
}

void Ieee802154eNetworkCtrlInfo::setRangingOffset(uint32_t rangingOffset)
{
    this->rangingOffset_var = rangingOffset;
}

uint8_t Ieee802154eNetworkCtrlInfo::getRangingFOM() const
{
    return rangingFOM_var;
}

void Ieee802154eNetworkCtrlInfo::setRangingFOM(uint8_t rangingFOM)
{
    this->rangingFOM_var = rangingFOM;
}

uint8_t Ieee802154eNetworkCtrlInfo::getNumBackoffs() const
{
    return numBackoffs_var;
}

void Ieee802154eNetworkCtrlInfo::setNumBackoffs(uint8_t numBackoffs)
{
    this->numBackoffs_var = numBackoffs;
}

void Ieee802154eNetworkCtrlInfo::setAckPayloadArraySize(unsigned int size)
{
    uint8_t *ackPayload_var2 = (size==0) ? NULL : new uint8_t[size];
    unsigned int sz = ackPayload_arraysize < size ? ackPayload_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        ackPayload_var2[i] = this->ackPayload_var[i];
    for (unsigned int i=sz; i<size; i++)
        ackPayload_var2[i] = 0;
    ackPayload_arraysize = size;
    delete [] this->ackPayload_var;
    this->ackPayload_var = ackPayload_var2;
}

unsigned int Ieee802154eNetworkCtrlInfo::getAckPayloadArraySize() const
{
    return ackPayload_arraysize;
}

uint8_t Ieee802154eNetworkCtrlInfo::getAckPayload(unsigned int k) const
{
    if (k>=ackPayload_arraysize) throw cRuntimeError("Array of size %d indexed by %d", ackPayload_arraysize, k);
    return ackPayload_var[k];
}

void Ieee802154eNetworkCtrlInfo::setAckPayload(unsigned int k, uint8_t ackPayload)
{
    if (k>=ackPayload_arraysize) throw cRuntimeError("Array of size %d indexed by %d", ackPayload_arraysize, k);
    this->ackPayload_var[k] = ackPayload;
}

bool Ieee802154eNetworkCtrlInfo::getReceivedByACK() const
{
    return receivedByACK_var;
}

void Ieee802154eNetworkCtrlInfo::setReceivedByACK(bool receivedByACK)
{
    this->receivedByACK_var = receivedByACK;
}

int Ieee802154eNetworkCtrlInfo::getStage() const
{
    return stage_var;
}

void Ieee802154eNetworkCtrlInfo::setStage(int stage)
{
    this->stage_var = stage;
}

int Ieee802154eNetworkCtrlInfo::getWaitConstant() const
{
    return WaitConstant_var;
}

void Ieee802154eNetworkCtrlInfo::setWaitConstant(int WaitConstant)
{
    this->WaitConstant_var = WaitConstant;
}

bool Ieee802154eNetworkCtrlInfo::getTxCS() const
{
    return txCS_var;
}

void Ieee802154eNetworkCtrlInfo::setTxCS(bool txCS)
{
    this->txCS_var = txCS;
}

bool Ieee802154eNetworkCtrlInfo::getTxLowerCH() const
{
    return txLowerCH_var;
}

void Ieee802154eNetworkCtrlInfo::setTxLowerCH(bool txLowerCH)
{
    this->txLowerCH_var = txLowerCH;
}

bool Ieee802154eNetworkCtrlInfo::getTxHigherCH() const
{
    return txHigherCH_var;
}

void Ieee802154eNetworkCtrlInfo::setTxHigherCH(bool txHigherCH)
{
    this->txHigherCH_var = txHigherCH;
}

uint8_t Ieee802154eNetworkCtrlInfo::getNumberCH() const
{
    return numberCH_var;
}

void Ieee802154eNetworkCtrlInfo::setNumberCH(uint8_t numberCH)
{
    this->numberCH_var = numberCH;
}

int Ieee802154eNetworkCtrlInfo::getChannelOffset10() const
{
    return channelOffset10_var;
}

void Ieee802154eNetworkCtrlInfo::setChannelOffset10(int channelOffset10)
{
    this->channelOffset10_var = channelOffset10;
}

int Ieee802154eNetworkCtrlInfo::getChannelOffset11() const
{
    return channelOffset11_var;
}

void Ieee802154eNetworkCtrlInfo::setChannelOffset11(int channelOffset11)
{
    this->channelOffset11_var = channelOffset11;
}

class Ieee802154eNetworkCtrlInfoDescriptor : public cClassDescriptor
{
  public:
    Ieee802154eNetworkCtrlInfoDescriptor();
    virtual ~Ieee802154eNetworkCtrlInfoDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(Ieee802154eNetworkCtrlInfoDescriptor);

Ieee802154eNetworkCtrlInfoDescriptor::Ieee802154eNetworkCtrlInfoDescriptor() : cClassDescriptor("Ieee802154eNetworkCtrlInfo", "cMessage")
{
}

Ieee802154eNetworkCtrlInfoDescriptor::~Ieee802154eNetworkCtrlInfoDescriptor()
{
}

bool Ieee802154eNetworkCtrlInfoDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Ieee802154eNetworkCtrlInfo *>(obj)!=NULL;
}

const char *Ieee802154eNetworkCtrlInfoDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int Ieee802154eNetworkCtrlInfoDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 128+basedesc->getFieldCount(object) : 128;
}

unsigned int Ieee802154eNetworkCtrlInfoDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<128) ? fieldTypeFlags[field] : 0;
}

const char *Ieee802154eNetworkCtrlInfoDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "toParent",
        "destName",
        "netwAddr",
        "channelNumber",
        "channelPage",
        "coordAddrMode",
        "coordPANId",
        "coordAddress",
        "capabilityInformation",
        "securityLevel",
        "keyIdMode",
        "keySource",
        "keyIndex",
        "lowLatencyNetworkInfo",
        "channelOffset",
        "hoppingSequenceID",
        "AssocShortAddress",
        "status",
        "deviceAddrMode",
        "devicePANId",
        "deviceAddress",
        "disassociateReason",
        "txIndirect",
        "pibAttribute",
        "gtsCharacteristic",
        "setDefaultPIB",
        "deferPermit",
        "rxOnTime",
        "rxOnDuration",
        "rangingRxControl",
        "scanType",
        "scanChannels",
        "scanDuration",
        "linkQualityScan",
        "frameControlOptions",
        "headerIElist",
        "payloadIElist",
        "pibAttributeValue",
        "panId",
        "startTime",
        "beaconOrder",
        "superframeOrder",
        "panCoordinator",
        "batteryLifeExtension",
        "coordRealignment",
        "coordRealignSecurityLevel",
        "coordRealignKeyIdMode",
        "coordRealignKeySource",
        "coordRealignKeyIndex",
        "beaconSecurityLevel",
        "beaconKeyIdMode",
        "beaconKeySource",
        "beaconKeyIndex",
        "dsmeSuperframeSpecification",
        "beaconBitmap",
        "hoppingSequenceLength",
        "hoppingSequence",
        "channelOffsetBitmapLength",
        "channelOffsetBitmap",
        "trackBeacon",
        "txDPSIndex",
        "rxDPSIndex",
        "dpsIndexDuration",
        "beaconType",
        "channel",
        "dstAddrMode",
        "dstAddr",
        "bsnSuppression",
        "bsn",
        "finalCAPSlot",
        "associationPermit",
        "gtsPermit",
        "linkQuality",
        "timeStamp",
        "securityStatus",
        "codeList",
        "nmbrShortAddrPending",
        "nmbrExtAddrPending",
        "addrList",
        "sduLength",
        "sdu",
        "ebsn",
        "srcAddrMode",
        "srcAddr",
        "gtsLength",
        "gtsDirection",
        "gtsCharType",
        "lossReason",
        "slotframeHandle",
        "operation",
        "size",
        "linkHandle",
        "timeslot",
        "linkOptions",
        "linkType",
        "nodeAddr",
        "tschMode",
        "keepAlivePeriod",
        "lowLatencyNetworkConfiguration",
        "managementType",
        "direction",
        "prioritizedChannelAccess",
        "NumSlot",
        "preferredSuperframeID",
        "preferredSlotID",
        "dsmeSABSpecification",
        "info",
        "dsmeSABSubBlockLength",
        "dsmeSABSubBlockIndex",
        "ReportPeriod",
        "msduHandle",
        "rangingReceived",
        "rangingCounterStart",
        "rangingCounterStop",
        "rangingTrackingInterval",
        "rangingOffset",
        "rangingFOM",
        "numBackoffs",
        "ackPayload",
        "receivedByACK",
        "stage",
        "WaitConstant",
        "txCS",
        "txLowerCH",
        "txHigherCH",
        "numberCH",
        "channelOffset10",
        "channelOffset11",
    };
    return (field>=0 && field<128) ? fieldNames[field] : NULL;
}

int Ieee802154eNetworkCtrlInfoDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "toParent")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destName")==0) return base+1;
    if (fieldName[0]=='n' && strcmp(fieldName, "netwAddr")==0) return base+2;
    if (fieldName[0]=='c' && strcmp(fieldName, "channelNumber")==0) return base+3;
    if (fieldName[0]=='c' && strcmp(fieldName, "channelPage")==0) return base+4;
    if (fieldName[0]=='c' && strcmp(fieldName, "coordAddrMode")==0) return base+5;
    if (fieldName[0]=='c' && strcmp(fieldName, "coordPANId")==0) return base+6;
    if (fieldName[0]=='c' && strcmp(fieldName, "coordAddress")==0) return base+7;
    if (fieldName[0]=='c' && strcmp(fieldName, "capabilityInformation")==0) return base+8;
    if (fieldName[0]=='s' && strcmp(fieldName, "securityLevel")==0) return base+9;
    if (fieldName[0]=='k' && strcmp(fieldName, "keyIdMode")==0) return base+10;
    if (fieldName[0]=='k' && strcmp(fieldName, "keySource")==0) return base+11;
    if (fieldName[0]=='k' && strcmp(fieldName, "keyIndex")==0) return base+12;
    if (fieldName[0]=='l' && strcmp(fieldName, "lowLatencyNetworkInfo")==0) return base+13;
    if (fieldName[0]=='c' && strcmp(fieldName, "channelOffset")==0) return base+14;
    if (fieldName[0]=='h' && strcmp(fieldName, "hoppingSequenceID")==0) return base+15;
    if (fieldName[0]=='A' && strcmp(fieldName, "AssocShortAddress")==0) return base+16;
    if (fieldName[0]=='s' && strcmp(fieldName, "status")==0) return base+17;
    if (fieldName[0]=='d' && strcmp(fieldName, "deviceAddrMode")==0) return base+18;
    if (fieldName[0]=='d' && strcmp(fieldName, "devicePANId")==0) return base+19;
    if (fieldName[0]=='d' && strcmp(fieldName, "deviceAddress")==0) return base+20;
    if (fieldName[0]=='d' && strcmp(fieldName, "disassociateReason")==0) return base+21;
    if (fieldName[0]=='t' && strcmp(fieldName, "txIndirect")==0) return base+22;
    if (fieldName[0]=='p' && strcmp(fieldName, "pibAttribute")==0) return base+23;
    if (fieldName[0]=='g' && strcmp(fieldName, "gtsCharacteristic")==0) return base+24;
    if (fieldName[0]=='s' && strcmp(fieldName, "setDefaultPIB")==0) return base+25;
    if (fieldName[0]=='d' && strcmp(fieldName, "deferPermit")==0) return base+26;
    if (fieldName[0]=='r' && strcmp(fieldName, "rxOnTime")==0) return base+27;
    if (fieldName[0]=='r' && strcmp(fieldName, "rxOnDuration")==0) return base+28;
    if (fieldName[0]=='r' && strcmp(fieldName, "rangingRxControl")==0) return base+29;
    if (fieldName[0]=='s' && strcmp(fieldName, "scanType")==0) return base+30;
    if (fieldName[0]=='s' && strcmp(fieldName, "scanChannels")==0) return base+31;
    if (fieldName[0]=='s' && strcmp(fieldName, "scanDuration")==0) return base+32;
    if (fieldName[0]=='l' && strcmp(fieldName, "linkQualityScan")==0) return base+33;
    if (fieldName[0]=='f' && strcmp(fieldName, "frameControlOptions")==0) return base+34;
    if (fieldName[0]=='h' && strcmp(fieldName, "headerIElist")==0) return base+35;
    if (fieldName[0]=='p' && strcmp(fieldName, "payloadIElist")==0) return base+36;
    if (fieldName[0]=='p' && strcmp(fieldName, "pibAttributeValue")==0) return base+37;
    if (fieldName[0]=='p' && strcmp(fieldName, "panId")==0) return base+38;
    if (fieldName[0]=='s' && strcmp(fieldName, "startTime")==0) return base+39;
    if (fieldName[0]=='b' && strcmp(fieldName, "beaconOrder")==0) return base+40;
    if (fieldName[0]=='s' && strcmp(fieldName, "superframeOrder")==0) return base+41;
    if (fieldName[0]=='p' && strcmp(fieldName, "panCoordinator")==0) return base+42;
    if (fieldName[0]=='b' && strcmp(fieldName, "batteryLifeExtension")==0) return base+43;
    if (fieldName[0]=='c' && strcmp(fieldName, "coordRealignment")==0) return base+44;
    if (fieldName[0]=='c' && strcmp(fieldName, "coordRealignSecurityLevel")==0) return base+45;
    if (fieldName[0]=='c' && strcmp(fieldName, "coordRealignKeyIdMode")==0) return base+46;
    if (fieldName[0]=='c' && strcmp(fieldName, "coordRealignKeySource")==0) return base+47;
    if (fieldName[0]=='c' && strcmp(fieldName, "coordRealignKeyIndex")==0) return base+48;
    if (fieldName[0]=='b' && strcmp(fieldName, "beaconSecurityLevel")==0) return base+49;
    if (fieldName[0]=='b' && strcmp(fieldName, "beaconKeyIdMode")==0) return base+50;
    if (fieldName[0]=='b' && strcmp(fieldName, "beaconKeySource")==0) return base+51;
    if (fieldName[0]=='b' && strcmp(fieldName, "beaconKeyIndex")==0) return base+52;
    if (fieldName[0]=='d' && strcmp(fieldName, "dsmeSuperframeSpecification")==0) return base+53;
    if (fieldName[0]=='b' && strcmp(fieldName, "beaconBitmap")==0) return base+54;
    if (fieldName[0]=='h' && strcmp(fieldName, "hoppingSequenceLength")==0) return base+55;
    if (fieldName[0]=='h' && strcmp(fieldName, "hoppingSequence")==0) return base+56;
    if (fieldName[0]=='c' && strcmp(fieldName, "channelOffsetBitmapLength")==0) return base+57;
    if (fieldName[0]=='c' && strcmp(fieldName, "channelOffsetBitmap")==0) return base+58;
    if (fieldName[0]=='t' && strcmp(fieldName, "trackBeacon")==0) return base+59;
    if (fieldName[0]=='t' && strcmp(fieldName, "txDPSIndex")==0) return base+60;
    if (fieldName[0]=='r' && strcmp(fieldName, "rxDPSIndex")==0) return base+61;
    if (fieldName[0]=='d' && strcmp(fieldName, "dpsIndexDuration")==0) return base+62;
    if (fieldName[0]=='b' && strcmp(fieldName, "beaconType")==0) return base+63;
    if (fieldName[0]=='c' && strcmp(fieldName, "channel")==0) return base+64;
    if (fieldName[0]=='d' && strcmp(fieldName, "dstAddrMode")==0) return base+65;
    if (fieldName[0]=='d' && strcmp(fieldName, "dstAddr")==0) return base+66;
    if (fieldName[0]=='b' && strcmp(fieldName, "bsnSuppression")==0) return base+67;
    if (fieldName[0]=='b' && strcmp(fieldName, "bsn")==0) return base+68;
    if (fieldName[0]=='f' && strcmp(fieldName, "finalCAPSlot")==0) return base+69;
    if (fieldName[0]=='a' && strcmp(fieldName, "associationPermit")==0) return base+70;
    if (fieldName[0]=='g' && strcmp(fieldName, "gtsPermit")==0) return base+71;
    if (fieldName[0]=='l' && strcmp(fieldName, "linkQuality")==0) return base+72;
    if (fieldName[0]=='t' && strcmp(fieldName, "timeStamp")==0) return base+73;
    if (fieldName[0]=='s' && strcmp(fieldName, "securityStatus")==0) return base+74;
    if (fieldName[0]=='c' && strcmp(fieldName, "codeList")==0) return base+75;
    if (fieldName[0]=='n' && strcmp(fieldName, "nmbrShortAddrPending")==0) return base+76;
    if (fieldName[0]=='n' && strcmp(fieldName, "nmbrExtAddrPending")==0) return base+77;
    if (fieldName[0]=='a' && strcmp(fieldName, "addrList")==0) return base+78;
    if (fieldName[0]=='s' && strcmp(fieldName, "sduLength")==0) return base+79;
    if (fieldName[0]=='s' && strcmp(fieldName, "sdu")==0) return base+80;
    if (fieldName[0]=='e' && strcmp(fieldName, "ebsn")==0) return base+81;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcAddrMode")==0) return base+82;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcAddr")==0) return base+83;
    if (fieldName[0]=='g' && strcmp(fieldName, "gtsLength")==0) return base+84;
    if (fieldName[0]=='g' && strcmp(fieldName, "gtsDirection")==0) return base+85;
    if (fieldName[0]=='g' && strcmp(fieldName, "gtsCharType")==0) return base+86;
    if (fieldName[0]=='l' && strcmp(fieldName, "lossReason")==0) return base+87;
    if (fieldName[0]=='s' && strcmp(fieldName, "slotframeHandle")==0) return base+88;
    if (fieldName[0]=='o' && strcmp(fieldName, "operation")==0) return base+89;
    if (fieldName[0]=='s' && strcmp(fieldName, "size")==0) return base+90;
    if (fieldName[0]=='l' && strcmp(fieldName, "linkHandle")==0) return base+91;
    if (fieldName[0]=='t' && strcmp(fieldName, "timeslot")==0) return base+92;
    if (fieldName[0]=='l' && strcmp(fieldName, "linkOptions")==0) return base+93;
    if (fieldName[0]=='l' && strcmp(fieldName, "linkType")==0) return base+94;
    if (fieldName[0]=='n' && strcmp(fieldName, "nodeAddr")==0) return base+95;
    if (fieldName[0]=='t' && strcmp(fieldName, "tschMode")==0) return base+96;
    if (fieldName[0]=='k' && strcmp(fieldName, "keepAlivePeriod")==0) return base+97;
    if (fieldName[0]=='l' && strcmp(fieldName, "lowLatencyNetworkConfiguration")==0) return base+98;
    if (fieldName[0]=='m' && strcmp(fieldName, "managementType")==0) return base+99;
    if (fieldName[0]=='d' && strcmp(fieldName, "direction")==0) return base+100;
    if (fieldName[0]=='p' && strcmp(fieldName, "prioritizedChannelAccess")==0) return base+101;
    if (fieldName[0]=='N' && strcmp(fieldName, "NumSlot")==0) return base+102;
    if (fieldName[0]=='p' && strcmp(fieldName, "preferredSuperframeID")==0) return base+103;
    if (fieldName[0]=='p' && strcmp(fieldName, "preferredSlotID")==0) return base+104;
    if (fieldName[0]=='d' && strcmp(fieldName, "dsmeSABSpecification")==0) return base+105;
    if (fieldName[0]=='i' && strcmp(fieldName, "info")==0) return base+106;
    if (fieldName[0]=='d' && strcmp(fieldName, "dsmeSABSubBlockLength")==0) return base+107;
    if (fieldName[0]=='d' && strcmp(fieldName, "dsmeSABSubBlockIndex")==0) return base+108;
    if (fieldName[0]=='R' && strcmp(fieldName, "ReportPeriod")==0) return base+109;
    if (fieldName[0]=='m' && strcmp(fieldName, "msduHandle")==0) return base+110;
    if (fieldName[0]=='r' && strcmp(fieldName, "rangingReceived")==0) return base+111;
    if (fieldName[0]=='r' && strcmp(fieldName, "rangingCounterStart")==0) return base+112;
    if (fieldName[0]=='r' && strcmp(fieldName, "rangingCounterStop")==0) return base+113;
    if (fieldName[0]=='r' && strcmp(fieldName, "rangingTrackingInterval")==0) return base+114;
    if (fieldName[0]=='r' && strcmp(fieldName, "rangingOffset")==0) return base+115;
    if (fieldName[0]=='r' && strcmp(fieldName, "rangingFOM")==0) return base+116;
    if (fieldName[0]=='n' && strcmp(fieldName, "numBackoffs")==0) return base+117;
    if (fieldName[0]=='a' && strcmp(fieldName, "ackPayload")==0) return base+118;
    if (fieldName[0]=='r' && strcmp(fieldName, "receivedByACK")==0) return base+119;
    if (fieldName[0]=='s' && strcmp(fieldName, "stage")==0) return base+120;
    if (fieldName[0]=='W' && strcmp(fieldName, "WaitConstant")==0) return base+121;
    if (fieldName[0]=='t' && strcmp(fieldName, "txCS")==0) return base+122;
    if (fieldName[0]=='t' && strcmp(fieldName, "txLowerCH")==0) return base+123;
    if (fieldName[0]=='t' && strcmp(fieldName, "txHigherCH")==0) return base+124;
    if (fieldName[0]=='n' && strcmp(fieldName, "numberCH")==0) return base+125;
    if (fieldName[0]=='c' && strcmp(fieldName, "channelOffset10")==0) return base+126;
    if (fieldName[0]=='c' && strcmp(fieldName, "channelOffset11")==0) return base+127;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *Ieee802154eNetworkCtrlInfoDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "bool",
        "string",
        "int64_t",
        "uint8_t",
        "uint8_t",
        "uint8_t",
        "uint16_t",
        "uint64_t",
        "uint8_t",
        "uint8_t",
        "uint8_t",
        "uint64_t",
        "uint8_t",
        "uint64_t",
        "uint16_t",
        "uint8_t",
        "uint16_t",
        "short",
        "uint8_t",
        "uint16_t",
        "uint64_t",
        "uint8_t",
        "bool",
        "uint8_t",
        "uint8_t",
        "bool",
        "bool",
        "uint32_t",
        "uint32_t",
        "uint8_t",
        "uint8_t",
        "uint64_t",
        "uint8_t",
        "bool",
        "bool",
        "uint8_t",
        "uint8_t",
        "MAC_PIB",
        "uint16_t",
        "uint32_t",
        "uint8_t",
        "uint8_t",
        "bool",
        "bool",
        "bool",
        "uint8_t",
        "uint8_t",
        "uint64_t",
        "uint8_t",
        "uint8_t",
        "uint8_t",
        "uint64_t",
        "uint8_t",
        "uint8_t",
        "uint64_t",
        "uint16_t",
        "uint8_t",
        "uint16_t",
        "uint8_t",
        "bool",
        "uint8_t",
        "uint8_t",
        "uint32_t",
        "uint8_t",
        "uint8_t",
        "uint8_t",
        "int64_t",
        "bool",
        "uint8_t",
        "uint8_t",
        "bool",
        "bool",
        "uint8_t",
        "uint64_t",
        "short",
        "int",
        "uint8_t",
        "uint8_t",
        "uint64_t",
        "int",
        "uint8_t",
        "uint8_t",
        "uint8_t",
        "uint64_t",
        "uint8_t",
        "bool",
        "bool",
        "uint8_t",
        "uint8_t",
        "uint8_t",
        "uint16_t",
        "uint16_t",
        "uint16_t",
        "uint8_t",
        "uint8_t",
        "uint16_t",
        "uint8_t",
        "uint16_t",
        "uint8_t",
        "uint8_t",
        "uint8_t",
        "uint8_t",
        "uint8_t",
        "uint16_t",
        "uint8_t",
        "uint8_t",
        "uint8_t",
        "uint8_t",
        "uint8_t",
        "uint32_t",
        "uint8_t",
        "bool",
        "uint32_t",
        "uint32_t",
        "uint32_t",
        "uint32_t",
        "uint8_t",
        "uint8_t",
        "uint8_t",
        "bool",
        "int",
        "int",
        "bool",
        "bool",
        "bool",
        "uint8_t",
        "int",
        "int",
    };
    return (field>=0 && field<128) ? fieldTypeStrings[field] : NULL;
}

const char *Ieee802154eNetworkCtrlInfoDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 17:
            if (!strcmp(propertyname,"enum")) return "MACenum";
            return NULL;
        case 74:
            if (!strcmp(propertyname,"enum")) return "MACenum";
            return NULL;
        default: return NULL;
    }
}

int Ieee802154eNetworkCtrlInfoDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eNetworkCtrlInfo *pp = (Ieee802154eNetworkCtrlInfo *)object; (void)pp;
    switch (field) {
        case 34: return pp->getFrameControlOptionsArraySize();
        case 35: return pp->getHeaderIElistArraySize();
        case 36: return pp->getPayloadIElistArraySize();
        case 56: return pp->getHoppingSequenceArraySize();
        case 58: return pp->getChannelOffsetBitmapArraySize();
        case 75: return pp->getCodeListArraySize();
        case 78: return pp->getAddrListArraySize();
        case 80: return pp->getSduArraySize();
        case 98: return pp->getLowLatencyNetworkConfigurationArraySize();
        case 105: return pp->getDsmeSABSpecificationArraySize();
        case 118: return pp->getAckPayloadArraySize();
        default: return 0;
    }
}

std::string Ieee802154eNetworkCtrlInfoDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eNetworkCtrlInfo *pp = (Ieee802154eNetworkCtrlInfo *)object; (void)pp;
    switch (field) {
        case 0: return bool2string(pp->getToParent());
        case 1: return oppstring2string(pp->getDestName());
        case 2: return int642string(pp->getNetwAddr());
        case 3: return ulong2string(pp->getChannelNumber());
        case 4: return ulong2string(pp->getChannelPage());
        case 5: return ulong2string(pp->getCoordAddrMode());
        case 6: return ulong2string(pp->getCoordPANId());
        case 7: return uint642string(pp->getCoordAddress());
        case 8: return ulong2string(pp->getCapabilityInformation());
        case 9: return ulong2string(pp->getSecurityLevel());
        case 10: return ulong2string(pp->getKeyIdMode());
        case 11: return uint642string(pp->getKeySource());
        case 12: return ulong2string(pp->getKeyIndex());
        case 13: return uint642string(pp->getLowLatencyNetworkInfo());
        case 14: return ulong2string(pp->getChannelOffset());
        case 15: return ulong2string(pp->getHoppingSequenceID());
        case 16: return ulong2string(pp->getAssocShortAddress());
        case 17: return long2string(pp->getStatus());
        case 18: return ulong2string(pp->getDeviceAddrMode());
        case 19: return ulong2string(pp->getDevicePANId());
        case 20: return uint642string(pp->getDeviceAddress());
        case 21: return ulong2string(pp->getDisassociateReason());
        case 22: return bool2string(pp->getTxIndirect());
        case 23: return ulong2string(pp->getPibAttribute());
        case 24: return ulong2string(pp->getGtsCharacteristic());
        case 25: return bool2string(pp->getSetDefaultPIB());
        case 26: return bool2string(pp->getDeferPermit());
        case 27: return ulong2string(pp->getRxOnTime());
        case 28: return ulong2string(pp->getRxOnDuration());
        case 29: return ulong2string(pp->getRangingRxControl());
        case 30: return ulong2string(pp->getScanType());
        case 31: return uint642string(pp->getScanChannels());
        case 32: return ulong2string(pp->getScanDuration());
        case 33: return bool2string(pp->getLinkQualityScan());
        case 34: return bool2string(pp->getFrameControlOptions(i));
        case 35: return ulong2string(pp->getHeaderIElist(i));
        case 36: return ulong2string(pp->getPayloadIElist(i));
        case 37: {std::stringstream out; out << pp->getPibAttributeValue(); return out.str();}
        case 38: return ulong2string(pp->getPanId());
        case 39: return ulong2string(pp->getStartTime());
        case 40: return ulong2string(pp->getBeaconOrder());
        case 41: return ulong2string(pp->getSuperframeOrder());
        case 42: return bool2string(pp->getPanCoordinator());
        case 43: return bool2string(pp->getBatteryLifeExtension());
        case 44: return bool2string(pp->getCoordRealignment());
        case 45: return ulong2string(pp->getCoordRealignSecurityLevel());
        case 46: return ulong2string(pp->getCoordRealignKeyIdMode());
        case 47: return uint642string(pp->getCoordRealignKeySource());
        case 48: return ulong2string(pp->getCoordRealignKeyIndex());
        case 49: return ulong2string(pp->getBeaconSecurityLevel());
        case 50: return ulong2string(pp->getBeaconKeyIdMode());
        case 51: return uint642string(pp->getBeaconKeySource());
        case 52: return ulong2string(pp->getBeaconKeyIndex());
        case 53: return ulong2string(pp->getDsmeSuperframeSpecification());
        case 54: return uint642string(pp->getBeaconBitmap());
        case 55: return ulong2string(pp->getHoppingSequenceLength());
        case 56: return ulong2string(pp->getHoppingSequence(i));
        case 57: return ulong2string(pp->getChannelOffsetBitmapLength());
        case 58: return ulong2string(pp->getChannelOffsetBitmap(i));
        case 59: return bool2string(pp->getTrackBeacon());
        case 60: return ulong2string(pp->getTxDPSIndex());
        case 61: return ulong2string(pp->getRxDPSIndex());
        case 62: return ulong2string(pp->getDpsIndexDuration());
        case 63: return ulong2string(pp->getBeaconType());
        case 64: return ulong2string(pp->getChannel());
        case 65: return ulong2string(pp->getDstAddrMode());
        case 66: return int642string(pp->getDstAddr());
        case 67: return bool2string(pp->getBsnSuppression());
        case 68: return ulong2string(pp->getBsn());
        case 69: return ulong2string(pp->getFinalCAPSlot());
        case 70: return bool2string(pp->getAssociationPermit());
        case 71: return bool2string(pp->getGtsPermit());
        case 72: return ulong2string(pp->getLinkQuality());
        case 73: return uint642string(pp->getTimeStamp());
        case 74: return long2string(pp->getSecurityStatus());
        case 75: return long2string(pp->getCodeList(i));
        case 76: return ulong2string(pp->getNmbrShortAddrPending());
        case 77: return ulong2string(pp->getNmbrExtAddrPending());
        case 78: return uint642string(pp->getAddrList(i));
        case 79: return long2string(pp->getSduLength());
        case 80: return ulong2string(pp->getSdu(i));
        case 81: return ulong2string(pp->getEbsn());
        case 82: return ulong2string(pp->getSrcAddrMode());
        case 83: return uint642string(pp->getSrcAddr());
        case 84: return ulong2string(pp->getGtsLength());
        case 85: return bool2string(pp->getGtsDirection());
        case 86: return bool2string(pp->getGtsCharType());
        case 87: return ulong2string(pp->getLossReason());
        case 88: return ulong2string(pp->getSlotframeHandle());
        case 89: return ulong2string(pp->getOperation());
        case 90: return ulong2string(pp->getSize());
        case 91: return ulong2string(pp->getLinkHandle());
        case 92: return ulong2string(pp->getTimeslot());
        case 93: return ulong2string(pp->getLinkOptions());
        case 94: return ulong2string(pp->getLinkType());
        case 95: return ulong2string(pp->getNodeAddr());
        case 96: return ulong2string(pp->getTschMode());
        case 97: return ulong2string(pp->getKeepAlivePeriod());
        case 98: return ulong2string(pp->getLowLatencyNetworkConfiguration(i));
        case 99: return ulong2string(pp->getManagementType());
        case 100: return ulong2string(pp->getDirection());
        case 101: return ulong2string(pp->getPrioritizedChannelAccess());
        case 102: return ulong2string(pp->getNumSlot());
        case 103: return ulong2string(pp->getPreferredSuperframeID());
        case 104: return ulong2string(pp->getPreferredSlotID());
        case 105: return ulong2string(pp->getDsmeSABSpecification(i));
        case 106: return ulong2string(pp->getInfo());
        case 107: return ulong2string(pp->getDsmeSABSubBlockLength());
        case 108: return ulong2string(pp->getDsmeSABSubBlockIndex());
        case 109: return ulong2string(pp->getReportPeriod());
        case 110: return ulong2string(pp->getMsduHandle());
        case 111: return bool2string(pp->getRangingReceived());
        case 112: return ulong2string(pp->getRangingCounterStart());
        case 113: return ulong2string(pp->getRangingCounterStop());
        case 114: return ulong2string(pp->getRangingTrackingInterval());
        case 115: return ulong2string(pp->getRangingOffset());
        case 116: return ulong2string(pp->getRangingFOM());
        case 117: return ulong2string(pp->getNumBackoffs());
        case 118: return ulong2string(pp->getAckPayload(i));
        case 119: return bool2string(pp->getReceivedByACK());
        case 120: return long2string(pp->getStage());
        case 121: return long2string(pp->getWaitConstant());
        case 122: return bool2string(pp->getTxCS());
        case 123: return bool2string(pp->getTxLowerCH());
        case 124: return bool2string(pp->getTxHigherCH());
        case 125: return ulong2string(pp->getNumberCH());
        case 126: return long2string(pp->getChannelOffset10());
        case 127: return long2string(pp->getChannelOffset11());
        default: return "";
    }
}

bool Ieee802154eNetworkCtrlInfoDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eNetworkCtrlInfo *pp = (Ieee802154eNetworkCtrlInfo *)object; (void)pp;
    switch (field) {
        case 0: pp->setToParent(string2bool(value)); return true;
        case 1: pp->setDestName((value)); return true;
        case 2: pp->setNetwAddr(string2int64(value)); return true;
        case 3: pp->setChannelNumber(string2ulong(value)); return true;
        case 4: pp->setChannelPage(string2ulong(value)); return true;
        case 5: pp->setCoordAddrMode(string2ulong(value)); return true;
        case 6: pp->setCoordPANId(string2ulong(value)); return true;
        case 7: pp->setCoordAddress(string2uint64(value)); return true;
        case 8: pp->setCapabilityInformation(string2ulong(value)); return true;
        case 9: pp->setSecurityLevel(string2ulong(value)); return true;
        case 10: pp->setKeyIdMode(string2ulong(value)); return true;
        case 11: pp->setKeySource(string2uint64(value)); return true;
        case 12: pp->setKeyIndex(string2ulong(value)); return true;
        case 13: pp->setLowLatencyNetworkInfo(string2uint64(value)); return true;
        case 14: pp->setChannelOffset(string2ulong(value)); return true;
        case 15: pp->setHoppingSequenceID(string2ulong(value)); return true;
        case 16: pp->setAssocShortAddress(string2ulong(value)); return true;
        case 17: pp->setStatus(string2long(value)); return true;
        case 18: pp->setDeviceAddrMode(string2ulong(value)); return true;
        case 19: pp->setDevicePANId(string2ulong(value)); return true;
        case 20: pp->setDeviceAddress(string2uint64(value)); return true;
        case 21: pp->setDisassociateReason(string2ulong(value)); return true;
        case 22: pp->setTxIndirect(string2bool(value)); return true;
        case 23: pp->setPibAttribute(string2ulong(value)); return true;
        case 24: pp->setGtsCharacteristic(string2ulong(value)); return true;
        case 25: pp->setSetDefaultPIB(string2bool(value)); return true;
        case 26: pp->setDeferPermit(string2bool(value)); return true;
        case 27: pp->setRxOnTime(string2ulong(value)); return true;
        case 28: pp->setRxOnDuration(string2ulong(value)); return true;
        case 29: pp->setRangingRxControl(string2ulong(value)); return true;
        case 30: pp->setScanType(string2ulong(value)); return true;
        case 31: pp->setScanChannels(string2uint64(value)); return true;
        case 32: pp->setScanDuration(string2ulong(value)); return true;
        case 33: pp->setLinkQualityScan(string2bool(value)); return true;
        case 34: pp->setFrameControlOptions(i,string2bool(value)); return true;
        case 35: pp->setHeaderIElist(i,string2ulong(value)); return true;
        case 36: pp->setPayloadIElist(i,string2ulong(value)); return true;
        case 38: pp->setPanId(string2ulong(value)); return true;
        case 39: pp->setStartTime(string2ulong(value)); return true;
        case 40: pp->setBeaconOrder(string2ulong(value)); return true;
        case 41: pp->setSuperframeOrder(string2ulong(value)); return true;
        case 42: pp->setPanCoordinator(string2bool(value)); return true;
        case 43: pp->setBatteryLifeExtension(string2bool(value)); return true;
        case 44: pp->setCoordRealignment(string2bool(value)); return true;
        case 45: pp->setCoordRealignSecurityLevel(string2ulong(value)); return true;
        case 46: pp->setCoordRealignKeyIdMode(string2ulong(value)); return true;
        case 47: pp->setCoordRealignKeySource(string2uint64(value)); return true;
        case 48: pp->setCoordRealignKeyIndex(string2ulong(value)); return true;
        case 49: pp->setBeaconSecurityLevel(string2ulong(value)); return true;
        case 50: pp->setBeaconKeyIdMode(string2ulong(value)); return true;
        case 51: pp->setBeaconKeySource(string2uint64(value)); return true;
        case 52: pp->setBeaconKeyIndex(string2ulong(value)); return true;
        case 53: pp->setDsmeSuperframeSpecification(string2ulong(value)); return true;
        case 54: pp->setBeaconBitmap(string2uint64(value)); return true;
        case 55: pp->setHoppingSequenceLength(string2ulong(value)); return true;
        case 56: pp->setHoppingSequence(i,string2ulong(value)); return true;
        case 57: pp->setChannelOffsetBitmapLength(string2ulong(value)); return true;
        case 58: pp->setChannelOffsetBitmap(i,string2ulong(value)); return true;
        case 59: pp->setTrackBeacon(string2bool(value)); return true;
        case 60: pp->setTxDPSIndex(string2ulong(value)); return true;
        case 61: pp->setRxDPSIndex(string2ulong(value)); return true;
        case 62: pp->setDpsIndexDuration(string2ulong(value)); return true;
        case 63: pp->setBeaconType(string2ulong(value)); return true;
        case 64: pp->setChannel(string2ulong(value)); return true;
        case 65: pp->setDstAddrMode(string2ulong(value)); return true;
        case 66: pp->setDstAddr(string2int64(value)); return true;
        case 67: pp->setBsnSuppression(string2bool(value)); return true;
        case 68: pp->setBsn(string2ulong(value)); return true;
        case 69: pp->setFinalCAPSlot(string2ulong(value)); return true;
        case 70: pp->setAssociationPermit(string2bool(value)); return true;
        case 71: pp->setGtsPermit(string2bool(value)); return true;
        case 72: pp->setLinkQuality(string2ulong(value)); return true;
        case 73: pp->setTimeStamp(string2uint64(value)); return true;
        case 74: pp->setSecurityStatus(string2long(value)); return true;
        case 75: pp->setCodeList(i,string2long(value)); return true;
        case 76: pp->setNmbrShortAddrPending(string2ulong(value)); return true;
        case 77: pp->setNmbrExtAddrPending(string2ulong(value)); return true;
        case 78: pp->setAddrList(i,string2uint64(value)); return true;
        case 79: pp->setSduLength(string2long(value)); return true;
        case 80: pp->setSdu(i,string2ulong(value)); return true;
        case 81: pp->setEbsn(string2ulong(value)); return true;
        case 82: pp->setSrcAddrMode(string2ulong(value)); return true;
        case 83: pp->setSrcAddr(string2uint64(value)); return true;
        case 84: pp->setGtsLength(string2ulong(value)); return true;
        case 85: pp->setGtsDirection(string2bool(value)); return true;
        case 86: pp->setGtsCharType(string2bool(value)); return true;
        case 87: pp->setLossReason(string2ulong(value)); return true;
        case 88: pp->setSlotframeHandle(string2ulong(value)); return true;
        case 89: pp->setOperation(string2ulong(value)); return true;
        case 90: pp->setSize(string2ulong(value)); return true;
        case 91: pp->setLinkHandle(string2ulong(value)); return true;
        case 92: pp->setTimeslot(string2ulong(value)); return true;
        case 93: pp->setLinkOptions(string2ulong(value)); return true;
        case 94: pp->setLinkType(string2ulong(value)); return true;
        case 95: pp->setNodeAddr(string2ulong(value)); return true;
        case 96: pp->setTschMode(string2ulong(value)); return true;
        case 97: pp->setKeepAlivePeriod(string2ulong(value)); return true;
        case 98: pp->setLowLatencyNetworkConfiguration(i,string2ulong(value)); return true;
        case 99: pp->setManagementType(string2ulong(value)); return true;
        case 100: pp->setDirection(string2ulong(value)); return true;
        case 101: pp->setPrioritizedChannelAccess(string2ulong(value)); return true;
        case 102: pp->setNumSlot(string2ulong(value)); return true;
        case 103: pp->setPreferredSuperframeID(string2ulong(value)); return true;
        case 104: pp->setPreferredSlotID(string2ulong(value)); return true;
        case 105: pp->setDsmeSABSpecification(i,string2ulong(value)); return true;
        case 106: pp->setInfo(string2ulong(value)); return true;
        case 107: pp->setDsmeSABSubBlockLength(string2ulong(value)); return true;
        case 108: pp->setDsmeSABSubBlockIndex(string2ulong(value)); return true;
        case 109: pp->setReportPeriod(string2ulong(value)); return true;
        case 110: pp->setMsduHandle(string2ulong(value)); return true;
        case 111: pp->setRangingReceived(string2bool(value)); return true;
        case 112: pp->setRangingCounterStart(string2ulong(value)); return true;
        case 113: pp->setRangingCounterStop(string2ulong(value)); return true;
        case 114: pp->setRangingTrackingInterval(string2ulong(value)); return true;
        case 115: pp->setRangingOffset(string2ulong(value)); return true;
        case 116: pp->setRangingFOM(string2ulong(value)); return true;
        case 117: pp->setNumBackoffs(string2ulong(value)); return true;
        case 118: pp->setAckPayload(i,string2ulong(value)); return true;
        case 119: pp->setReceivedByACK(string2bool(value)); return true;
        case 120: pp->setStage(string2long(value)); return true;
        case 121: pp->setWaitConstant(string2long(value)); return true;
        case 122: pp->setTxCS(string2bool(value)); return true;
        case 123: pp->setTxLowerCH(string2bool(value)); return true;
        case 124: pp->setTxHigherCH(string2bool(value)); return true;
        case 125: pp->setNumberCH(string2ulong(value)); return true;
        case 126: pp->setChannelOffset10(string2long(value)); return true;
        case 127: pp->setChannelOffset11(string2long(value)); return true;
        default: return false;
    }
}

const char *Ieee802154eNetworkCtrlInfoDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 37: return opp_typename(typeid(MAC_PIB));
        default: return NULL;
    };
}

void *Ieee802154eNetworkCtrlInfoDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eNetworkCtrlInfo *pp = (Ieee802154eNetworkCtrlInfo *)object; (void)pp;
    switch (field) {
        case 37: return (void *)(&pp->getPibAttributeValue()); break;
        default: return NULL;
    }
}



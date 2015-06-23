//
// Generated file, do not edit! Created by nedtool 4.6 from src/linklayer/contract/IEEE802154e/RLLControlInfo.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "RLLControlInfo_m.h"

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

Register_Class(RLLControlInfo);

RLLControlInfo::RLLControlInfo(const char *name, int kind) : ::cMessage(name,kind)
{
    this->toParent_var = 0;
    this->destName_var = 0;
    this->dstAddrMode_var = 0;
    this->dstAddr_var = 0;
    this->srcAddrMode_var = 0;
    this->srcAddr_var = 0;
    this->panCoordinator_var = 0;
    this->deviceAddress_var = 0;
    this->status_var = 0;
    this->channelOffset_var = 0;
    this->assocShortAddress_var = 0;
    this->beaconType_var = 0;
    this->channel_var = 0;
    this->beaconOrder_var = 0;
    this->superframeOrder_var = 0;
    this->startTime_var = 0;
    this->scanType_var = 0;
}

RLLControlInfo::RLLControlInfo(const RLLControlInfo& other) : ::cMessage(other)
{
    copy(other);
}

RLLControlInfo::~RLLControlInfo()
{
}

RLLControlInfo& RLLControlInfo::operator=(const RLLControlInfo& other)
{
    if (this==&other) return *this;
    ::cMessage::operator=(other);
    copy(other);
    return *this;
}

void RLLControlInfo::copy(const RLLControlInfo& other)
{
    this->toParent_var = other.toParent_var;
    this->destName_var = other.destName_var;
    this->dstAddrMode_var = other.dstAddrMode_var;
    this->dstAddr_var = other.dstAddr_var;
    this->srcAddrMode_var = other.srcAddrMode_var;
    this->srcAddr_var = other.srcAddr_var;
    this->panCoordinator_var = other.panCoordinator_var;
    this->deviceAddress_var = other.deviceAddress_var;
    this->status_var = other.status_var;
    this->channelOffset_var = other.channelOffset_var;
    this->assocShortAddress_var = other.assocShortAddress_var;
    this->beaconType_var = other.beaconType_var;
    this->channel_var = other.channel_var;
    this->beaconOrder_var = other.beaconOrder_var;
    this->superframeOrder_var = other.superframeOrder_var;
    this->startTime_var = other.startTime_var;
    this->scanType_var = other.scanType_var;
}

void RLLControlInfo::parsimPack(cCommBuffer *b)
{
    ::cMessage::parsimPack(b);
    doPacking(b,this->toParent_var);
    doPacking(b,this->destName_var);
    doPacking(b,this->dstAddrMode_var);
    doPacking(b,this->dstAddr_var);
    doPacking(b,this->srcAddrMode_var);
    doPacking(b,this->srcAddr_var);
    doPacking(b,this->panCoordinator_var);
    doPacking(b,this->deviceAddress_var);
    doPacking(b,this->status_var);
    doPacking(b,this->channelOffset_var);
    doPacking(b,this->assocShortAddress_var);
    doPacking(b,this->beaconType_var);
    doPacking(b,this->channel_var);
    doPacking(b,this->beaconOrder_var);
    doPacking(b,this->superframeOrder_var);
    doPacking(b,this->startTime_var);
    doPacking(b,this->scanType_var);
}

void RLLControlInfo::parsimUnpack(cCommBuffer *b)
{
    ::cMessage::parsimUnpack(b);
    doUnpacking(b,this->toParent_var);
    doUnpacking(b,this->destName_var);
    doUnpacking(b,this->dstAddrMode_var);
    doUnpacking(b,this->dstAddr_var);
    doUnpacking(b,this->srcAddrMode_var);
    doUnpacking(b,this->srcAddr_var);
    doUnpacking(b,this->panCoordinator_var);
    doUnpacking(b,this->deviceAddress_var);
    doUnpacking(b,this->status_var);
    doUnpacking(b,this->channelOffset_var);
    doUnpacking(b,this->assocShortAddress_var);
    doUnpacking(b,this->beaconType_var);
    doUnpacking(b,this->channel_var);
    doUnpacking(b,this->beaconOrder_var);
    doUnpacking(b,this->superframeOrder_var);
    doUnpacking(b,this->startTime_var);
    doUnpacking(b,this->scanType_var);
}

bool RLLControlInfo::getToParent() const
{
    return toParent_var;
}

void RLLControlInfo::setToParent(bool toParent)
{
    this->toParent_var = toParent;
}

const char * RLLControlInfo::getDestName() const
{
    return destName_var.c_str();
}

void RLLControlInfo::setDestName(const char * destName)
{
    this->destName_var = destName;
}

uint8_t RLLControlInfo::getDstAddrMode() const
{
    return dstAddrMode_var;
}

void RLLControlInfo::setDstAddrMode(uint8_t dstAddrMode)
{
    this->dstAddrMode_var = dstAddrMode;
}

int64_t RLLControlInfo::getDstAddr() const
{
    return dstAddr_var;
}

void RLLControlInfo::setDstAddr(int64_t dstAddr)
{
    this->dstAddr_var = dstAddr;
}

uint8_t RLLControlInfo::getSrcAddrMode() const
{
    return srcAddrMode_var;
}

void RLLControlInfo::setSrcAddrMode(uint8_t srcAddrMode)
{
    this->srcAddrMode_var = srcAddrMode;
}

int64_t RLLControlInfo::getSrcAddr() const
{
    return srcAddr_var;
}

void RLLControlInfo::setSrcAddr(int64_t srcAddr)
{
    this->srcAddr_var = srcAddr;
}

bool RLLControlInfo::getPanCoordinator() const
{
    return panCoordinator_var;
}

void RLLControlInfo::setPanCoordinator(bool panCoordinator)
{
    this->panCoordinator_var = panCoordinator;
}

uint64_t RLLControlInfo::getDeviceAddress() const
{
    return deviceAddress_var;
}

void RLLControlInfo::setDeviceAddress(uint64_t deviceAddress)
{
    this->deviceAddress_var = deviceAddress;
}

short RLLControlInfo::getStatus() const
{
    return status_var;
}

void RLLControlInfo::setStatus(short status)
{
    this->status_var = status;
}

uint16_t RLLControlInfo::getChannelOffset() const
{
    return channelOffset_var;
}

void RLLControlInfo::setChannelOffset(uint16_t channelOffset)
{
    this->channelOffset_var = channelOffset;
}

uint16_t RLLControlInfo::getAssocShortAddress() const
{
    return assocShortAddress_var;
}

void RLLControlInfo::setAssocShortAddress(uint16_t assocShortAddress)
{
    this->assocShortAddress_var = assocShortAddress;
}

uint8_t RLLControlInfo::getBeaconType() const
{
    return beaconType_var;
}

void RLLControlInfo::setBeaconType(uint8_t beaconType)
{
    this->beaconType_var = beaconType;
}

uint8_t RLLControlInfo::getChannel() const
{
    return channel_var;
}

void RLLControlInfo::setChannel(uint8_t channel)
{
    this->channel_var = channel;
}

uint8_t RLLControlInfo::getBeaconOrder() const
{
    return beaconOrder_var;
}

void RLLControlInfo::setBeaconOrder(uint8_t beaconOrder)
{
    this->beaconOrder_var = beaconOrder;
}

uint8_t RLLControlInfo::getSuperframeOrder() const
{
    return superframeOrder_var;
}

void RLLControlInfo::setSuperframeOrder(uint8_t superframeOrder)
{
    this->superframeOrder_var = superframeOrder;
}

uint32_t RLLControlInfo::getStartTime() const
{
    return startTime_var;
}

void RLLControlInfo::setStartTime(uint32_t startTime)
{
    this->startTime_var = startTime;
}

uint8_t RLLControlInfo::getScanType() const
{
    return scanType_var;
}

void RLLControlInfo::setScanType(uint8_t scanType)
{
    this->scanType_var = scanType;
}

class RLLControlInfoDescriptor : public cClassDescriptor
{
  public:
    RLLControlInfoDescriptor();
    virtual ~RLLControlInfoDescriptor();

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

Register_ClassDescriptor(RLLControlInfoDescriptor);

RLLControlInfoDescriptor::RLLControlInfoDescriptor() : cClassDescriptor("RLLControlInfo", "cMessage")
{
}

RLLControlInfoDescriptor::~RLLControlInfoDescriptor()
{
}

bool RLLControlInfoDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<RLLControlInfo *>(obj)!=NULL;
}

const char *RLLControlInfoDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int RLLControlInfoDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 17+basedesc->getFieldCount(object) : 17;
}

unsigned int RLLControlInfoDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<17) ? fieldTypeFlags[field] : 0;
}

const char *RLLControlInfoDescriptor::getFieldName(void *object, int field) const
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
        "dstAddrMode",
        "dstAddr",
        "srcAddrMode",
        "srcAddr",
        "panCoordinator",
        "deviceAddress",
        "status",
        "channelOffset",
        "assocShortAddress",
        "beaconType",
        "channel",
        "beaconOrder",
        "superframeOrder",
        "startTime",
        "scanType",
    };
    return (field>=0 && field<17) ? fieldNames[field] : NULL;
}

int RLLControlInfoDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "toParent")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destName")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "dstAddrMode")==0) return base+2;
    if (fieldName[0]=='d' && strcmp(fieldName, "dstAddr")==0) return base+3;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcAddrMode")==0) return base+4;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcAddr")==0) return base+5;
    if (fieldName[0]=='p' && strcmp(fieldName, "panCoordinator")==0) return base+6;
    if (fieldName[0]=='d' && strcmp(fieldName, "deviceAddress")==0) return base+7;
    if (fieldName[0]=='s' && strcmp(fieldName, "status")==0) return base+8;
    if (fieldName[0]=='c' && strcmp(fieldName, "channelOffset")==0) return base+9;
    if (fieldName[0]=='a' && strcmp(fieldName, "assocShortAddress")==0) return base+10;
    if (fieldName[0]=='b' && strcmp(fieldName, "beaconType")==0) return base+11;
    if (fieldName[0]=='c' && strcmp(fieldName, "channel")==0) return base+12;
    if (fieldName[0]=='b' && strcmp(fieldName, "beaconOrder")==0) return base+13;
    if (fieldName[0]=='s' && strcmp(fieldName, "superframeOrder")==0) return base+14;
    if (fieldName[0]=='s' && strcmp(fieldName, "startTime")==0) return base+15;
    if (fieldName[0]=='s' && strcmp(fieldName, "scanType")==0) return base+16;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *RLLControlInfoDescriptor::getFieldTypeString(void *object, int field) const
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
        "uint8_t",
        "int64_t",
        "uint8_t",
        "int64_t",
        "bool",
        "uint64_t",
        "short",
        "uint16_t",
        "uint16_t",
        "uint8_t",
        "uint8_t",
        "uint8_t",
        "uint8_t",
        "uint32_t",
        "uint8_t",
    };
    return (field>=0 && field<17) ? fieldTypeStrings[field] : NULL;
}

const char *RLLControlInfoDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 8:
            if (!strcmp(propertyname,"enum")) return "MACenum";
            return NULL;
        default: return NULL;
    }
}

int RLLControlInfoDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    RLLControlInfo *pp = (RLLControlInfo *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string RLLControlInfoDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    RLLControlInfo *pp = (RLLControlInfo *)object; (void)pp;
    switch (field) {
        case 0: return bool2string(pp->getToParent());
        case 1: return oppstring2string(pp->getDestName());
        case 2: return ulong2string(pp->getDstAddrMode());
        case 3: return int642string(pp->getDstAddr());
        case 4: return ulong2string(pp->getSrcAddrMode());
        case 5: return int642string(pp->getSrcAddr());
        case 6: return bool2string(pp->getPanCoordinator());
        case 7: return uint642string(pp->getDeviceAddress());
        case 8: return long2string(pp->getStatus());
        case 9: return ulong2string(pp->getChannelOffset());
        case 10: return ulong2string(pp->getAssocShortAddress());
        case 11: return ulong2string(pp->getBeaconType());
        case 12: return ulong2string(pp->getChannel());
        case 13: return ulong2string(pp->getBeaconOrder());
        case 14: return ulong2string(pp->getSuperframeOrder());
        case 15: return ulong2string(pp->getStartTime());
        case 16: return ulong2string(pp->getScanType());
        default: return "";
    }
}

bool RLLControlInfoDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    RLLControlInfo *pp = (RLLControlInfo *)object; (void)pp;
    switch (field) {
        case 0: pp->setToParent(string2bool(value)); return true;
        case 1: pp->setDestName((value)); return true;
        case 2: pp->setDstAddrMode(string2ulong(value)); return true;
        case 3: pp->setDstAddr(string2int64(value)); return true;
        case 4: pp->setSrcAddrMode(string2ulong(value)); return true;
        case 5: pp->setSrcAddr(string2int64(value)); return true;
        case 6: pp->setPanCoordinator(string2bool(value)); return true;
        case 7: pp->setDeviceAddress(string2uint64(value)); return true;
        case 8: pp->setStatus(string2long(value)); return true;
        case 9: pp->setChannelOffset(string2ulong(value)); return true;
        case 10: pp->setAssocShortAddress(string2ulong(value)); return true;
        case 11: pp->setBeaconType(string2ulong(value)); return true;
        case 12: pp->setChannel(string2ulong(value)); return true;
        case 13: pp->setBeaconOrder(string2ulong(value)); return true;
        case 14: pp->setSuperframeOrder(string2ulong(value)); return true;
        case 15: pp->setStartTime(string2ulong(value)); return true;
        case 16: pp->setScanType(string2ulong(value)); return true;
        default: return false;
    }
}

const char *RLLControlInfoDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *RLLControlInfoDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    RLLControlInfo *pp = (RLLControlInfo *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}



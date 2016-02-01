//
// Generated file, do not edit! Created by nedtool 4.6 from src/networklayer/CIDERNet/CIDER/common/CIDERFrame.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "CIDERFrame_m.h"

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

Register_Class(CIDERFrame);

CIDERFrame::CIDERFrame(const char *name, int kind) : ::cPacket(name,kind)
{
    this->LPDevice_var = 0;
    this->rxPower_var = 0;
    this->clusterDegree_var = 0;
    this->nodeDegree_var = 0;
    this->LPDegree_var = 0;
    this->txPower_var = 0;
    this->weight_var = 0;
}

CIDERFrame::CIDERFrame(const CIDERFrame& other) : ::cPacket(other)
{
    copy(other);
}

CIDERFrame::~CIDERFrame()
{
}

CIDERFrame& CIDERFrame::operator=(const CIDERFrame& other)
{
    if (this==&other) return *this;
    ::cPacket::operator=(other);
    copy(other);
    return *this;
}

void CIDERFrame::copy(const CIDERFrame& other)
{
    this->macAddressesList_var = other.macAddressesList_var;
    this->srcAddress_var = other.srcAddress_var;
    this->dstAddress_var = other.dstAddress_var;
    this->LPDevice_var = other.LPDevice_var;
    this->rxPower_var = other.rxPower_var;
    this->clusterDegree_var = other.clusterDegree_var;
    this->nodeDegree_var = other.nodeDegree_var;
    this->LPDegree_var = other.LPDegree_var;
    this->txPower_var = other.txPower_var;
    this->weight_var = other.weight_var;
}

void CIDERFrame::parsimPack(cCommBuffer *b)
{
    ::cPacket::parsimPack(b);
    doPacking(b,this->macAddressesList_var);
    doPacking(b,this->srcAddress_var);
    doPacking(b,this->dstAddress_var);
    doPacking(b,this->LPDevice_var);
    doPacking(b,this->rxPower_var);
    doPacking(b,this->clusterDegree_var);
    doPacking(b,this->nodeDegree_var);
    doPacking(b,this->LPDegree_var);
    doPacking(b,this->txPower_var);
    doPacking(b,this->weight_var);
}

void CIDERFrame::parsimUnpack(cCommBuffer *b)
{
    ::cPacket::parsimUnpack(b);
    doUnpacking(b,this->macAddressesList_var);
    doUnpacking(b,this->srcAddress_var);
    doUnpacking(b,this->dstAddress_var);
    doUnpacking(b,this->LPDevice_var);
    doUnpacking(b,this->rxPower_var);
    doUnpacking(b,this->clusterDegree_var);
    doUnpacking(b,this->nodeDegree_var);
    doUnpacking(b,this->LPDegree_var);
    doUnpacking(b,this->txPower_var);
    doUnpacking(b,this->weight_var);
}

macVector& CIDERFrame::getMacAddressesList()
{
    return macAddressesList_var;
}

void CIDERFrame::setMacAddressesList(const macVector& macAddressesList)
{
    this->macAddressesList_var = macAddressesList;
}

MACAddress& CIDERFrame::getSrcAddress()
{
    return srcAddress_var;
}

void CIDERFrame::setSrcAddress(const MACAddress& srcAddress)
{
    this->srcAddress_var = srcAddress;
}

MACAddress& CIDERFrame::getDstAddress()
{
    return dstAddress_var;
}

void CIDERFrame::setDstAddress(const MACAddress& dstAddress)
{
    this->dstAddress_var = dstAddress;
}

bool CIDERFrame::getLPDevice() const
{
    return LPDevice_var;
}

void CIDERFrame::setLPDevice(bool LPDevice)
{
    this->LPDevice_var = LPDevice;
}

double CIDERFrame::getRxPower() const
{
    return rxPower_var;
}

void CIDERFrame::setRxPower(double rxPower)
{
    this->rxPower_var = rxPower;
}

int CIDERFrame::getClusterDegree() const
{
    return clusterDegree_var;
}

void CIDERFrame::setClusterDegree(int clusterDegree)
{
    this->clusterDegree_var = clusterDegree;
}

int CIDERFrame::getNodeDegree() const
{
    return nodeDegree_var;
}

void CIDERFrame::setNodeDegree(int nodeDegree)
{
    this->nodeDegree_var = nodeDegree;
}

int CIDERFrame::getLPDegree() const
{
    return LPDegree_var;
}

void CIDERFrame::setLPDegree(int LPDegree)
{
    this->LPDegree_var = LPDegree;
}

double CIDERFrame::getTxPower() const
{
    return txPower_var;
}

void CIDERFrame::setTxPower(double txPower)
{
    this->txPower_var = txPower;
}

double CIDERFrame::getWeight() const
{
    return weight_var;
}

void CIDERFrame::setWeight(double weight)
{
    this->weight_var = weight;
}

class CIDERFrameDescriptor : public cClassDescriptor
{
  public:
    CIDERFrameDescriptor();
    virtual ~CIDERFrameDescriptor();

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

Register_ClassDescriptor(CIDERFrameDescriptor);

CIDERFrameDescriptor::CIDERFrameDescriptor() : cClassDescriptor("CIDERFrame", "cPacket")
{
}

CIDERFrameDescriptor::~CIDERFrameDescriptor()
{
}

bool CIDERFrameDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CIDERFrame *>(obj)!=NULL;
}

const char *CIDERFrameDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CIDERFrameDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 10+basedesc->getFieldCount(object) : 10;
}

unsigned int CIDERFrameDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<10) ? fieldTypeFlags[field] : 0;
}

const char *CIDERFrameDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "macAddressesList",
        "srcAddress",
        "dstAddress",
        "LPDevice",
        "rxPower",
        "clusterDegree",
        "nodeDegree",
        "LPDegree",
        "txPower",
        "weight",
    };
    return (field>=0 && field<10) ? fieldNames[field] : NULL;
}

int CIDERFrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='m' && strcmp(fieldName, "macAddressesList")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcAddress")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "dstAddress")==0) return base+2;
    if (fieldName[0]=='L' && strcmp(fieldName, "LPDevice")==0) return base+3;
    if (fieldName[0]=='r' && strcmp(fieldName, "rxPower")==0) return base+4;
    if (fieldName[0]=='c' && strcmp(fieldName, "clusterDegree")==0) return base+5;
    if (fieldName[0]=='n' && strcmp(fieldName, "nodeDegree")==0) return base+6;
    if (fieldName[0]=='L' && strcmp(fieldName, "LPDegree")==0) return base+7;
    if (fieldName[0]=='t' && strcmp(fieldName, "txPower")==0) return base+8;
    if (fieldName[0]=='w' && strcmp(fieldName, "weight")==0) return base+9;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CIDERFrameDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "macVector",
        "MACAddress",
        "MACAddress",
        "bool",
        "double",
        "int",
        "int",
        "int",
        "double",
        "double",
    };
    return (field>=0 && field<10) ? fieldTypeStrings[field] : NULL;
}

const char *CIDERFrameDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int CIDERFrameDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CIDERFrame *pp = (CIDERFrame *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CIDERFrameDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CIDERFrame *pp = (CIDERFrame *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getMacAddressesList(); return out.str();}
        case 1: {std::stringstream out; out << pp->getSrcAddress(); return out.str();}
        case 2: {std::stringstream out; out << pp->getDstAddress(); return out.str();}
        case 3: return bool2string(pp->getLPDevice());
        case 4: return double2string(pp->getRxPower());
        case 5: return long2string(pp->getClusterDegree());
        case 6: return long2string(pp->getNodeDegree());
        case 7: return long2string(pp->getLPDegree());
        case 8: return double2string(pp->getTxPower());
        case 9: return double2string(pp->getWeight());
        default: return "";
    }
}

bool CIDERFrameDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CIDERFrame *pp = (CIDERFrame *)object; (void)pp;
    switch (field) {
        case 3: pp->setLPDevice(string2bool(value)); return true;
        case 4: pp->setRxPower(string2double(value)); return true;
        case 5: pp->setClusterDegree(string2long(value)); return true;
        case 6: pp->setNodeDegree(string2long(value)); return true;
        case 7: pp->setLPDegree(string2long(value)); return true;
        case 8: pp->setTxPower(string2double(value)); return true;
        case 9: pp->setWeight(string2double(value)); return true;
        default: return false;
    }
}

const char *CIDERFrameDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0: return opp_typename(typeid(macVector));
        case 1: return opp_typename(typeid(MACAddress));
        case 2: return opp_typename(typeid(MACAddress));
        default: return NULL;
    };
}

void *CIDERFrameDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CIDERFrame *pp = (CIDERFrame *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getMacAddressesList()); break;
        case 1: return (void *)(&pp->getSrcAddress()); break;
        case 2: return (void *)(&pp->getDstAddress()); break;
        default: return NULL;
    }
}



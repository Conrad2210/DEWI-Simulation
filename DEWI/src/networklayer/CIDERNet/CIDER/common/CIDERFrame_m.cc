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
    this->rxPower_var = 0;
    this->nodeDegree_var = 0;
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
    this->address_var = other.address_var;
    this->rxPower_var = other.rxPower_var;
    this->nodeDegree_var = other.nodeDegree_var;
    this->txPower_var = other.txPower_var;
    this->weight_var = other.weight_var;
}

void CIDERFrame::parsimPack(cCommBuffer *b)
{
    ::cPacket::parsimPack(b);
    doPacking(b,this->address_var);
    doPacking(b,this->rxPower_var);
    doPacking(b,this->nodeDegree_var);
    doPacking(b,this->txPower_var);
    doPacking(b,this->weight_var);
}

void CIDERFrame::parsimUnpack(cCommBuffer *b)
{
    ::cPacket::parsimUnpack(b);
    doUnpacking(b,this->address_var);
    doUnpacking(b,this->rxPower_var);
    doUnpacking(b,this->nodeDegree_var);
    doUnpacking(b,this->txPower_var);
    doUnpacking(b,this->weight_var);
}

MACAddress& CIDERFrame::getAddress()
{
    return address_var;
}

void CIDERFrame::setAddress(const MACAddress& address)
{
    this->address_var = address;
}

double CIDERFrame::getRxPower() const
{
    return rxPower_var;
}

void CIDERFrame::setRxPower(double rxPower)
{
    this->rxPower_var = rxPower;
}

int CIDERFrame::getNodeDegree() const
{
    return nodeDegree_var;
}

void CIDERFrame::setNodeDegree(int nodeDegree)
{
    this->nodeDegree_var = nodeDegree;
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
    return basedesc ? 5+basedesc->getFieldCount(object) : 5;
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
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
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
        "address",
        "rxPower",
        "nodeDegree",
        "txPower",
        "weight",
    };
    return (field>=0 && field<5) ? fieldNames[field] : NULL;
}

int CIDERFrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='a' && strcmp(fieldName, "address")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "rxPower")==0) return base+1;
    if (fieldName[0]=='n' && strcmp(fieldName, "nodeDegree")==0) return base+2;
    if (fieldName[0]=='t' && strcmp(fieldName, "txPower")==0) return base+3;
    if (fieldName[0]=='w' && strcmp(fieldName, "weight")==0) return base+4;
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
        "MACAddress",
        "double",
        "int",
        "double",
        "double",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : NULL;
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
        case 0: {std::stringstream out; out << pp->getAddress(); return out.str();}
        case 1: return double2string(pp->getRxPower());
        case 2: return long2string(pp->getNodeDegree());
        case 3: return double2string(pp->getTxPower());
        case 4: return double2string(pp->getWeight());
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
        case 1: pp->setRxPower(string2double(value)); return true;
        case 2: pp->setNodeDegree(string2long(value)); return true;
        case 3: pp->setTxPower(string2double(value)); return true;
        case 4: pp->setWeight(string2double(value)); return true;
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
        case 0: return opp_typename(typeid(MACAddress));
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
        case 0: return (void *)(&pp->getAddress()); break;
        default: return NULL;
    }
}



//
// Generated file, do not edit! Created by opp_msgc 4.4 from src/applications/udpapp/BurstMSG.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "BurstMSG_m.h"

USING_NAMESPACE

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(BurstMSG);

BurstMSG::BurstMSG(const char *name, int kind) : ::cPacket(name,kind)
{
    this->hopCount_var = 0;
}

BurstMSG::BurstMSG(const BurstMSG& other) : ::cPacket(other)
{
    copy(other);
}

BurstMSG::~BurstMSG()
{
}

BurstMSG& BurstMSG::operator=(const BurstMSG& other)
{
    if (this==&other) return *this;
    ::cPacket::operator=(other);
    copy(other);
    return *this;
}

void BurstMSG::copy(const BurstMSG& other)
{
    this->hopCount_var = other.hopCount_var;
}

void BurstMSG::parsimPack(cCommBuffer *b)
{
    ::cPacket::parsimPack(b);
    doPacking(b,this->hopCount_var);
}

void BurstMSG::parsimUnpack(cCommBuffer *b)
{
    ::cPacket::parsimUnpack(b);
    doUnpacking(b,this->hopCount_var);
}

int BurstMSG::getHopCount() const
{
    return hopCount_var;
}

void BurstMSG::setHopCount(int hopCount)
{
    this->hopCount_var = hopCount;
}

class BurstMSGDescriptor : public cClassDescriptor
{
  public:
    BurstMSGDescriptor();
    virtual ~BurstMSGDescriptor();

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

Register_ClassDescriptor(BurstMSGDescriptor);

BurstMSGDescriptor::BurstMSGDescriptor() : cClassDescriptor("BurstMSG", "cPacket")
{
}

BurstMSGDescriptor::~BurstMSGDescriptor()
{
}

bool BurstMSGDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<BurstMSG *>(obj)!=NULL;
}

const char *BurstMSGDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int BurstMSGDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int BurstMSGDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *BurstMSGDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "hopCount",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int BurstMSGDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopCount")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *BurstMSGDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *BurstMSGDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int BurstMSGDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    BurstMSG *pp = (BurstMSG *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string BurstMSGDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    BurstMSG *pp = (BurstMSG *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getHopCount());
        default: return "";
    }
}

bool BurstMSGDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    BurstMSG *pp = (BurstMSG *)object; (void)pp;
    switch (field) {
        case 0: pp->setHopCount(string2long(value)); return true;
        default: return false;
    }
}

const char *BurstMSGDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
    };
    return (field>=0 && field<1) ? fieldStructNames[field] : NULL;
}

void *BurstMSGDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    BurstMSG *pp = (BurstMSG *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}



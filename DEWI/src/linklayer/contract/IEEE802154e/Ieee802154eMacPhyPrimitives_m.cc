//
// Generated file, do not edit! Created by opp_msgc 4.4 from src/linklayer/contract/IEEE802154e/Ieee802154eMacPhyPrimitives.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "Ieee802154eMacPhyPrimitives_m.h"

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




Register_Class(Ieee802154eMacPhyPrimitives);

Ieee802154eMacPhyPrimitives::Ieee802154eMacPhyPrimitives(const char *name, int kind) : ::cMessage(name,kind)
{
    this->status_var = 0;
    this->pibAttribute_var = 0;
    this->psduLength_var = 0;
    this->bitRate_var = 0;
    this->additional_var = -1;
    this->energyLevel_var = 0;
    this->channelNumber_var = 0;
}

Ieee802154eMacPhyPrimitives::Ieee802154eMacPhyPrimitives(const Ieee802154eMacPhyPrimitives& other) : ::cMessage(other)
{
    copy(other);
}

Ieee802154eMacPhyPrimitives::~Ieee802154eMacPhyPrimitives()
{
}

Ieee802154eMacPhyPrimitives& Ieee802154eMacPhyPrimitives::operator=(const Ieee802154eMacPhyPrimitives& other)
{
    if (this==&other) return *this;
    ::cMessage::operator=(other);
    copy(other);
    return *this;
}

void Ieee802154eMacPhyPrimitives::copy(const Ieee802154eMacPhyPrimitives& other)
{
    this->status_var = other.status_var;
    this->pibAttribute_var = other.pibAttribute_var;
    this->pibAttributeValue_var = other.pibAttributeValue_var;
    this->psduLength_var = other.psduLength_var;
    this->bitRate_var = other.bitRate_var;
    this->additional_var = other.additional_var;
    this->energyLevel_var = other.energyLevel_var;
    this->channelNumber_var = other.channelNumber_var;
}

void Ieee802154eMacPhyPrimitives::parsimPack(cCommBuffer *b)
{
    ::cMessage::parsimPack(b);
    doPacking(b,this->status_var);
    doPacking(b,this->pibAttribute_var);
    doPacking(b,this->pibAttributeValue_var);
    doPacking(b,this->psduLength_var);
    doPacking(b,this->bitRate_var);
    doPacking(b,this->additional_var);
    doPacking(b,this->energyLevel_var);
    doPacking(b,this->channelNumber_var);
}

void Ieee802154eMacPhyPrimitives::parsimUnpack(cCommBuffer *b)
{
    ::cMessage::parsimUnpack(b);
    doUnpacking(b,this->status_var);
    doUnpacking(b,this->pibAttribute_var);
    doUnpacking(b,this->pibAttributeValue_var);
    doUnpacking(b,this->psduLength_var);
    doUnpacking(b,this->bitRate_var);
    doUnpacking(b,this->additional_var);
    doUnpacking(b,this->energyLevel_var);
    doUnpacking(b,this->channelNumber_var);
}

short Ieee802154eMacPhyPrimitives::getStatus() const
{
    return status_var;
}

void Ieee802154eMacPhyPrimitives::setStatus(short status)
{
    this->status_var = status;
}

uint8_t Ieee802154eMacPhyPrimitives::getPibAttribute() const
{
    return pibAttribute_var;
}

void Ieee802154eMacPhyPrimitives::setPibAttribute(uint8_t pibAttribute)
{
    this->pibAttribute_var = pibAttribute;
}

PHY_PIB& Ieee802154eMacPhyPrimitives::getPibAttributeValue()
{
    return pibAttributeValue_var;
}

void Ieee802154eMacPhyPrimitives::setPibAttributeValue(const PHY_PIB& pibAttributeValue)
{
    this->pibAttributeValue_var = pibAttributeValue;
}

uint8_t Ieee802154eMacPhyPrimitives::getPsduLength() const
{
    return psduLength_var;
}

void Ieee802154eMacPhyPrimitives::setPsduLength(uint8_t psduLength)
{
    this->psduLength_var = psduLength;
}

double Ieee802154eMacPhyPrimitives::getBitRate() const
{
    return bitRate_var;
}

void Ieee802154eMacPhyPrimitives::setBitRate(double bitRate)
{
    this->bitRate_var = bitRate;
}

short Ieee802154eMacPhyPrimitives::getAdditional() const
{
    return additional_var;
}

void Ieee802154eMacPhyPrimitives::setAdditional(short additional)
{
    this->additional_var = additional;
}

unsigned char Ieee802154eMacPhyPrimitives::getEnergyLevel() const
{
    return energyLevel_var;
}

void Ieee802154eMacPhyPrimitives::setEnergyLevel(unsigned char energyLevel)
{
    this->energyLevel_var = energyLevel;
}

int Ieee802154eMacPhyPrimitives::getChannelNumber() const
{
    return channelNumber_var;
}

void Ieee802154eMacPhyPrimitives::setChannelNumber(int channelNumber)
{
    this->channelNumber_var = channelNumber;
}

class Ieee802154eMacPhyPrimitivesDescriptor : public cClassDescriptor
{
  public:
    Ieee802154eMacPhyPrimitivesDescriptor();
    virtual ~Ieee802154eMacPhyPrimitivesDescriptor();

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

Register_ClassDescriptor(Ieee802154eMacPhyPrimitivesDescriptor);

Ieee802154eMacPhyPrimitivesDescriptor::Ieee802154eMacPhyPrimitivesDescriptor() : cClassDescriptor("Ieee802154eMacPhyPrimitives", "cMessage")
{
}

Ieee802154eMacPhyPrimitivesDescriptor::~Ieee802154eMacPhyPrimitivesDescriptor()
{
}

bool Ieee802154eMacPhyPrimitivesDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Ieee802154eMacPhyPrimitives *>(obj)!=NULL;
}

const char *Ieee802154eMacPhyPrimitivesDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int Ieee802154eMacPhyPrimitivesDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 8+basedesc->getFieldCount(object) : 8;
}

unsigned int Ieee802154eMacPhyPrimitivesDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<8) ? fieldTypeFlags[field] : 0;
}

const char *Ieee802154eMacPhyPrimitivesDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "status",
        "pibAttribute",
        "pibAttributeValue",
        "psduLength",
        "bitRate",
        "additional",
        "energyLevel",
        "channelNumber",
    };
    return (field>=0 && field<8) ? fieldNames[field] : NULL;
}

int Ieee802154eMacPhyPrimitivesDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "status")==0) return base+0;
    if (fieldName[0]=='p' && strcmp(fieldName, "pibAttribute")==0) return base+1;
    if (fieldName[0]=='p' && strcmp(fieldName, "pibAttributeValue")==0) return base+2;
    if (fieldName[0]=='p' && strcmp(fieldName, "psduLength")==0) return base+3;
    if (fieldName[0]=='b' && strcmp(fieldName, "bitRate")==0) return base+4;
    if (fieldName[0]=='a' && strcmp(fieldName, "additional")==0) return base+5;
    if (fieldName[0]=='e' && strcmp(fieldName, "energyLevel")==0) return base+6;
    if (fieldName[0]=='c' && strcmp(fieldName, "channelNumber")==0) return base+7;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *Ieee802154eMacPhyPrimitivesDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "short",
        "uint8_t",
        "PHY_PIB",
        "uint8_t",
        "double",
        "short",
        "unsigned char",
        "int",
    };
    return (field>=0 && field<8) ? fieldTypeStrings[field] : NULL;
}

const char *Ieee802154eMacPhyPrimitivesDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "PHYenum";
            return NULL;
        default: return NULL;
    }
}

int Ieee802154eMacPhyPrimitivesDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eMacPhyPrimitives *pp = (Ieee802154eMacPhyPrimitives *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string Ieee802154eMacPhyPrimitivesDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eMacPhyPrimitives *pp = (Ieee802154eMacPhyPrimitives *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getStatus());
        case 1: return ulong2string(pp->getPibAttribute());
        case 2: {std::stringstream out; out << pp->getPibAttributeValue(); return out.str();}
        case 3: return ulong2string(pp->getPsduLength());
        case 4: return double2string(pp->getBitRate());
        case 5: return long2string(pp->getAdditional());
        case 6: return ulong2string(pp->getEnergyLevel());
        case 7: return long2string(pp->getChannelNumber());
        default: return "";
    }
}

bool Ieee802154eMacPhyPrimitivesDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eMacPhyPrimitives *pp = (Ieee802154eMacPhyPrimitives *)object; (void)pp;
    switch (field) {
        case 0: pp->setStatus(string2long(value)); return true;
        case 1: pp->setPibAttribute(string2ulong(value)); return true;
        case 3: pp->setPsduLength(string2ulong(value)); return true;
        case 4: pp->setBitRate(string2double(value)); return true;
        case 5: pp->setAdditional(string2long(value)); return true;
        case 6: pp->setEnergyLevel(string2ulong(value)); return true;
        case 7: pp->setChannelNumber(string2long(value)); return true;
        default: return false;
    }
}

const char *Ieee802154eMacPhyPrimitivesDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        "PHY_PIB",
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<8) ? fieldStructNames[field] : NULL;
}

void *Ieee802154eMacPhyPrimitivesDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eMacPhyPrimitives *pp = (Ieee802154eMacPhyPrimitives *)object; (void)pp;
    switch (field) {
        case 2: return (void *)(&pp->getPibAttributeValue()); break;
        default: return NULL;
    }
}



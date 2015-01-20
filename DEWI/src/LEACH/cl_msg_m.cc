//
// Generated file, do not edit! Created by nedtool 4.6 from src/LEACH/cl_msg.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "cl_msg_m.h"

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

Register_Class(ClusterMessage);

ClusterMessage::ClusterMessage(const char *name, int kind) : ::cMessage(name,kind)
{
    this->proto_var = 0;
    this->srcAddress_var = 0;
    this->destAddress_var = 0;
}

ClusterMessage::ClusterMessage(const ClusterMessage& other) : ::cMessage(other)
{
    copy(other);
}

ClusterMessage::~ClusterMessage()
{
}

ClusterMessage& ClusterMessage::operator=(const ClusterMessage& other)
{
    if (this==&other) return *this;
    ::cMessage::operator=(other);
    copy(other);
    return *this;
}

void ClusterMessage::copy(const ClusterMessage& other)
{
    this->proto_var = other.proto_var;
    this->srcAddress_var = other.srcAddress_var;
    this->destAddress_var = other.destAddress_var;
}

void ClusterMessage::parsimPack(cCommBuffer *b)
{
    ::cMessage::parsimPack(b);
    doPacking(b,this->proto_var);
    doPacking(b,this->srcAddress_var);
    doPacking(b,this->destAddress_var);
}

void ClusterMessage::parsimUnpack(cCommBuffer *b)
{
    ::cMessage::parsimUnpack(b);
    doUnpacking(b,this->proto_var);
    doUnpacking(b,this->srcAddress_var);
    doUnpacking(b,this->destAddress_var);
}

int ClusterMessage::getProto() const
{
    return proto_var;
}

void ClusterMessage::setProto(int proto)
{
    this->proto_var = proto;
}

int ClusterMessage::getSrcAddress() const
{
    return srcAddress_var;
}

void ClusterMessage::setSrcAddress(int srcAddress)
{
    this->srcAddress_var = srcAddress;
}

int ClusterMessage::getDestAddress() const
{
    return destAddress_var;
}

void ClusterMessage::setDestAddress(int destAddress)
{
    this->destAddress_var = destAddress;
}

class ClusterMessageDescriptor : public cClassDescriptor
{
  public:
    ClusterMessageDescriptor();
    virtual ~ClusterMessageDescriptor();

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

Register_ClassDescriptor(ClusterMessageDescriptor);

ClusterMessageDescriptor::ClusterMessageDescriptor() : cClassDescriptor("ClusterMessage", "cMessage")
{
}

ClusterMessageDescriptor::~ClusterMessageDescriptor()
{
}

bool ClusterMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ClusterMessage *>(obj)!=NULL;
}

const char *ClusterMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ClusterMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int ClusterMessageDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *ClusterMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "proto",
        "srcAddress",
        "destAddress",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int ClusterMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "proto")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcAddress")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "destAddress")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ClusterMessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *ClusterMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int ClusterMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ClusterMessage *pp = (ClusterMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string ClusterMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ClusterMessage *pp = (ClusterMessage *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getProto());
        case 1: return long2string(pp->getSrcAddress());
        case 2: return long2string(pp->getDestAddress());
        default: return "";
    }
}

bool ClusterMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ClusterMessage *pp = (ClusterMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setProto(string2long(value)); return true;
        case 1: pp->setSrcAddress(string2long(value)); return true;
        case 2: pp->setDestAddress(string2long(value)); return true;
        default: return false;
    }
}

const char *ClusterMessageDescriptor::getFieldStructName(void *object, int field) const
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

void *ClusterMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ClusterMessage *pp = (ClusterMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(Start);

Start::Start(const char *name, int kind) : ::ClusterMessage(name,kind)
{
    this->round_var = 0;
}

Start::Start(const Start& other) : ::ClusterMessage(other)
{
    copy(other);
}

Start::~Start()
{
}

Start& Start::operator=(const Start& other)
{
    if (this==&other) return *this;
    ::ClusterMessage::operator=(other);
    copy(other);
    return *this;
}

void Start::copy(const Start& other)
{
    this->round_var = other.round_var;
}

void Start::parsimPack(cCommBuffer *b)
{
    ::ClusterMessage::parsimPack(b);
    doPacking(b,this->round_var);
}

void Start::parsimUnpack(cCommBuffer *b)
{
    ::ClusterMessage::parsimUnpack(b);
    doUnpacking(b,this->round_var);
}

int Start::getRound() const
{
    return round_var;
}

void Start::setRound(int round)
{
    this->round_var = round;
}

class StartDescriptor : public cClassDescriptor
{
  public:
    StartDescriptor();
    virtual ~StartDescriptor();

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

Register_ClassDescriptor(StartDescriptor);

StartDescriptor::StartDescriptor() : cClassDescriptor("Start", "ClusterMessage")
{
}

StartDescriptor::~StartDescriptor()
{
}

bool StartDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Start *>(obj)!=NULL;
}

const char *StartDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int StartDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int StartDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *StartDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "round",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int StartDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='r' && strcmp(fieldName, "round")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *StartDescriptor::getFieldTypeString(void *object, int field) const
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

const char *StartDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int StartDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Start *pp = (Start *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string StartDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Start *pp = (Start *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getRound());
        default: return "";
    }
}

bool StartDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Start *pp = (Start *)object; (void)pp;
    switch (field) {
        case 0: pp->setRound(string2long(value)); return true;
        default: return false;
    }
}

const char *StartDescriptor::getFieldStructName(void *object, int field) const
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

void *StartDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Start *pp = (Start *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(Status2BSMessage);

Status2BSMessage::Status2BSMessage(const char *name, int kind) : ::ClusterMessage(name,kind)
{
    this->energy_var = 0;
    this->status_var = 0;
    this->cluster_var = 0;
    this->xpos_var = 0;
    this->ypos_var = 0;
}

Status2BSMessage::Status2BSMessage(const Status2BSMessage& other) : ::ClusterMessage(other)
{
    copy(other);
}

Status2BSMessage::~Status2BSMessage()
{
}

Status2BSMessage& Status2BSMessage::operator=(const Status2BSMessage& other)
{
    if (this==&other) return *this;
    ::ClusterMessage::operator=(other);
    copy(other);
    return *this;
}

void Status2BSMessage::copy(const Status2BSMessage& other)
{
    this->energy_var = other.energy_var;
    this->status_var = other.status_var;
    this->cluster_var = other.cluster_var;
    this->xpos_var = other.xpos_var;
    this->ypos_var = other.ypos_var;
}

void Status2BSMessage::parsimPack(cCommBuffer *b)
{
    ::ClusterMessage::parsimPack(b);
    doPacking(b,this->energy_var);
    doPacking(b,this->status_var);
    doPacking(b,this->cluster_var);
    doPacking(b,this->xpos_var);
    doPacking(b,this->ypos_var);
}

void Status2BSMessage::parsimUnpack(cCommBuffer *b)
{
    ::ClusterMessage::parsimUnpack(b);
    doUnpacking(b,this->energy_var);
    doUnpacking(b,this->status_var);
    doUnpacking(b,this->cluster_var);
    doUnpacking(b,this->xpos_var);
    doUnpacking(b,this->ypos_var);
}

int Status2BSMessage::getEnergy() const
{
    return energy_var;
}

void Status2BSMessage::setEnergy(int energy)
{
    this->energy_var = energy;
}

int Status2BSMessage::getStatus() const
{
    return status_var;
}

void Status2BSMessage::setStatus(int status)
{
    this->status_var = status;
}

int Status2BSMessage::getCluster() const
{
    return cluster_var;
}

void Status2BSMessage::setCluster(int cluster)
{
    this->cluster_var = cluster;
}

int Status2BSMessage::getXpos() const
{
    return xpos_var;
}

void Status2BSMessage::setXpos(int xpos)
{
    this->xpos_var = xpos;
}

int Status2BSMessage::getYpos() const
{
    return ypos_var;
}

void Status2BSMessage::setYpos(int ypos)
{
    this->ypos_var = ypos;
}

class Status2BSMessageDescriptor : public cClassDescriptor
{
  public:
    Status2BSMessageDescriptor();
    virtual ~Status2BSMessageDescriptor();

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

Register_ClassDescriptor(Status2BSMessageDescriptor);

Status2BSMessageDescriptor::Status2BSMessageDescriptor() : cClassDescriptor("Status2BSMessage", "ClusterMessage")
{
}

Status2BSMessageDescriptor::~Status2BSMessageDescriptor()
{
}

bool Status2BSMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Status2BSMessage *>(obj)!=NULL;
}

const char *Status2BSMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int Status2BSMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount(object) : 5;
}

unsigned int Status2BSMessageDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *Status2BSMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "energy",
        "status",
        "cluster",
        "xpos",
        "ypos",
    };
    return (field>=0 && field<5) ? fieldNames[field] : NULL;
}

int Status2BSMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='e' && strcmp(fieldName, "energy")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "status")==0) return base+1;
    if (fieldName[0]=='c' && strcmp(fieldName, "cluster")==0) return base+2;
    if (fieldName[0]=='x' && strcmp(fieldName, "xpos")==0) return base+3;
    if (fieldName[0]=='y' && strcmp(fieldName, "ypos")==0) return base+4;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *Status2BSMessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : NULL;
}

const char *Status2BSMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int Status2BSMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Status2BSMessage *pp = (Status2BSMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string Status2BSMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Status2BSMessage *pp = (Status2BSMessage *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getEnergy());
        case 1: return long2string(pp->getStatus());
        case 2: return long2string(pp->getCluster());
        case 3: return long2string(pp->getXpos());
        case 4: return long2string(pp->getYpos());
        default: return "";
    }
}

bool Status2BSMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Status2BSMessage *pp = (Status2BSMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setEnergy(string2long(value)); return true;
        case 1: pp->setStatus(string2long(value)); return true;
        case 2: pp->setCluster(string2long(value)); return true;
        case 3: pp->setXpos(string2long(value)); return true;
        case 4: pp->setYpos(string2long(value)); return true;
        default: return false;
    }
}

const char *Status2BSMessageDescriptor::getFieldStructName(void *object, int field) const
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

void *Status2BSMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Status2BSMessage *pp = (Status2BSMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(ClusterHeadMessage);

ClusterHeadMessage::ClusterHeadMessage(const char *name, int kind) : ::ClusterMessage(name,kind)
{
    for (unsigned int i=0; i<103; i++)
        this->cHead_var[i] = 0;
}

ClusterHeadMessage::ClusterHeadMessage(const ClusterHeadMessage& other) : ::ClusterMessage(other)
{
    copy(other);
}

ClusterHeadMessage::~ClusterHeadMessage()
{
}

ClusterHeadMessage& ClusterHeadMessage::operator=(const ClusterHeadMessage& other)
{
    if (this==&other) return *this;
    ::ClusterMessage::operator=(other);
    copy(other);
    return *this;
}

void ClusterHeadMessage::copy(const ClusterHeadMessage& other)
{
    for (unsigned int i=0; i<103; i++)
        this->cHead_var[i] = other.cHead_var[i];
}

void ClusterHeadMessage::parsimPack(cCommBuffer *b)
{
    ::ClusterMessage::parsimPack(b);
    doPacking(b,this->cHead_var,103);
}

void ClusterHeadMessage::parsimUnpack(cCommBuffer *b)
{
    ::ClusterMessage::parsimUnpack(b);
    doUnpacking(b,this->cHead_var,103);
}

unsigned int ClusterHeadMessage::getCHeadArraySize() const
{
    return 103;
}

int ClusterHeadMessage::getCHead(unsigned int k) const
{
    if (k>=103) throw cRuntimeError("Array of size 103 indexed by %lu", (unsigned long)k);
    return cHead_var[k];
}

void ClusterHeadMessage::setCHead(unsigned int k, int cHead)
{
    if (k>=103) throw cRuntimeError("Array of size 103 indexed by %lu", (unsigned long)k);
    this->cHead_var[k] = cHead;
}

class ClusterHeadMessageDescriptor : public cClassDescriptor
{
  public:
    ClusterHeadMessageDescriptor();
    virtual ~ClusterHeadMessageDescriptor();

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

Register_ClassDescriptor(ClusterHeadMessageDescriptor);

ClusterHeadMessageDescriptor::ClusterHeadMessageDescriptor() : cClassDescriptor("ClusterHeadMessage", "ClusterMessage")
{
}

ClusterHeadMessageDescriptor::~ClusterHeadMessageDescriptor()
{
}

bool ClusterHeadMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ClusterHeadMessage *>(obj)!=NULL;
}

const char *ClusterHeadMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ClusterHeadMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int ClusterHeadMessageDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *ClusterHeadMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "cHead",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int ClusterHeadMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "cHead")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ClusterHeadMessageDescriptor::getFieldTypeString(void *object, int field) const
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

const char *ClusterHeadMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int ClusterHeadMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ClusterHeadMessage *pp = (ClusterHeadMessage *)object; (void)pp;
    switch (field) {
        case 0: return 103;
        default: return 0;
    }
}

std::string ClusterHeadMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ClusterHeadMessage *pp = (ClusterHeadMessage *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getCHead(i));
        default: return "";
    }
}

bool ClusterHeadMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ClusterHeadMessage *pp = (ClusterHeadMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setCHead(i,string2long(value)); return true;
        default: return false;
    }
}

const char *ClusterHeadMessageDescriptor::getFieldStructName(void *object, int field) const
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

void *ClusterHeadMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ClusterHeadMessage *pp = (ClusterHeadMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(TDMAMessage);

TDMAMessage::TDMAMessage(const char *name, int kind) : ::ClusterMessage(name,kind)
{
    this->frames_var = 0;
    this->frameTime_var = 0;
    for (unsigned int i=0; i<103; i++)
        this->tdma_var[i] = 0;
}

TDMAMessage::TDMAMessage(const TDMAMessage& other) : ::ClusterMessage(other)
{
    copy(other);
}

TDMAMessage::~TDMAMessage()
{
}

TDMAMessage& TDMAMessage::operator=(const TDMAMessage& other)
{
    if (this==&other) return *this;
    ::ClusterMessage::operator=(other);
    copy(other);
    return *this;
}

void TDMAMessage::copy(const TDMAMessage& other)
{
    this->frames_var = other.frames_var;
    this->frameTime_var = other.frameTime_var;
    for (unsigned int i=0; i<103; i++)
        this->tdma_var[i] = other.tdma_var[i];
}

void TDMAMessage::parsimPack(cCommBuffer *b)
{
    ::ClusterMessage::parsimPack(b);
    doPacking(b,this->frames_var);
    doPacking(b,this->frameTime_var);
    doPacking(b,this->tdma_var,103);
}

void TDMAMessage::parsimUnpack(cCommBuffer *b)
{
    ::ClusterMessage::parsimUnpack(b);
    doUnpacking(b,this->frames_var);
    doUnpacking(b,this->frameTime_var);
    doUnpacking(b,this->tdma_var,103);
}

int TDMAMessage::getFrames() const
{
    return frames_var;
}

void TDMAMessage::setFrames(int frames)
{
    this->frames_var = frames;
}

int TDMAMessage::getFrameTime() const
{
    return frameTime_var;
}

void TDMAMessage::setFrameTime(int frameTime)
{
    this->frameTime_var = frameTime;
}

unsigned int TDMAMessage::getTdmaArraySize() const
{
    return 103;
}

int TDMAMessage::getTdma(unsigned int k) const
{
    if (k>=103) throw cRuntimeError("Array of size 103 indexed by %lu", (unsigned long)k);
    return tdma_var[k];
}

void TDMAMessage::setTdma(unsigned int k, int tdma)
{
    if (k>=103) throw cRuntimeError("Array of size 103 indexed by %lu", (unsigned long)k);
    this->tdma_var[k] = tdma;
}

class TDMAMessageDescriptor : public cClassDescriptor
{
  public:
    TDMAMessageDescriptor();
    virtual ~TDMAMessageDescriptor();

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

Register_ClassDescriptor(TDMAMessageDescriptor);

TDMAMessageDescriptor::TDMAMessageDescriptor() : cClassDescriptor("TDMAMessage", "ClusterMessage")
{
}

TDMAMessageDescriptor::~TDMAMessageDescriptor()
{
}

bool TDMAMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<TDMAMessage *>(obj)!=NULL;
}

const char *TDMAMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int TDMAMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int TDMAMessageDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *TDMAMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "frames",
        "frameTime",
        "tdma",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int TDMAMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "frames")==0) return base+0;
    if (fieldName[0]=='f' && strcmp(fieldName, "frameTime")==0) return base+1;
    if (fieldName[0]=='t' && strcmp(fieldName, "tdma")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *TDMAMessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *TDMAMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int TDMAMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    TDMAMessage *pp = (TDMAMessage *)object; (void)pp;
    switch (field) {
        case 2: return 103;
        default: return 0;
    }
}

std::string TDMAMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    TDMAMessage *pp = (TDMAMessage *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getFrames());
        case 1: return long2string(pp->getFrameTime());
        case 2: return long2string(pp->getTdma(i));
        default: return "";
    }
}

bool TDMAMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    TDMAMessage *pp = (TDMAMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setFrames(string2long(value)); return true;
        case 1: pp->setFrameTime(string2long(value)); return true;
        case 2: pp->setTdma(i,string2long(value)); return true;
        default: return false;
    }
}

const char *TDMAMessageDescriptor::getFieldStructName(void *object, int field) const
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

void *TDMAMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    TDMAMessage *pp = (TDMAMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(DataToCHMessage);

DataToCHMessage::DataToCHMessage(const char *name, int kind) : ::ClusterMessage(name,kind)
{
    this->data_var = 0;
}

DataToCHMessage::DataToCHMessage(const DataToCHMessage& other) : ::ClusterMessage(other)
{
    copy(other);
}

DataToCHMessage::~DataToCHMessage()
{
}

DataToCHMessage& DataToCHMessage::operator=(const DataToCHMessage& other)
{
    if (this==&other) return *this;
    ::ClusterMessage::operator=(other);
    copy(other);
    return *this;
}

void DataToCHMessage::copy(const DataToCHMessage& other)
{
    this->data_var = other.data_var;
}

void DataToCHMessage::parsimPack(cCommBuffer *b)
{
    ::ClusterMessage::parsimPack(b);
    doPacking(b,this->data_var);
}

void DataToCHMessage::parsimUnpack(cCommBuffer *b)
{
    ::ClusterMessage::parsimUnpack(b);
    doUnpacking(b,this->data_var);
}

int DataToCHMessage::getData() const
{
    return data_var;
}

void DataToCHMessage::setData(int data)
{
    this->data_var = data;
}

class DataToCHMessageDescriptor : public cClassDescriptor
{
  public:
    DataToCHMessageDescriptor();
    virtual ~DataToCHMessageDescriptor();

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

Register_ClassDescriptor(DataToCHMessageDescriptor);

DataToCHMessageDescriptor::DataToCHMessageDescriptor() : cClassDescriptor("DataToCHMessage", "ClusterMessage")
{
}

DataToCHMessageDescriptor::~DataToCHMessageDescriptor()
{
}

bool DataToCHMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<DataToCHMessage *>(obj)!=NULL;
}

const char *DataToCHMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int DataToCHMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int DataToCHMessageDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *DataToCHMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "data",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int DataToCHMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='d' && strcmp(fieldName, "data")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *DataToCHMessageDescriptor::getFieldTypeString(void *object, int field) const
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

const char *DataToCHMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int DataToCHMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    DataToCHMessage *pp = (DataToCHMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string DataToCHMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    DataToCHMessage *pp = (DataToCHMessage *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getData());
        default: return "";
    }
}

bool DataToCHMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    DataToCHMessage *pp = (DataToCHMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setData(string2long(value)); return true;
        default: return false;
    }
}

const char *DataToCHMessageDescriptor::getFieldStructName(void *object, int field) const
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

void *DataToCHMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    DataToCHMessage *pp = (DataToCHMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(Data2BSMessage);

Data2BSMessage::Data2BSMessage(const char *name, int kind) : ::ClusterMessage(name,kind)
{
    this->data_var = 0;
}

Data2BSMessage::Data2BSMessage(const Data2BSMessage& other) : ::ClusterMessage(other)
{
    copy(other);
}

Data2BSMessage::~Data2BSMessage()
{
}

Data2BSMessage& Data2BSMessage::operator=(const Data2BSMessage& other)
{
    if (this==&other) return *this;
    ::ClusterMessage::operator=(other);
    copy(other);
    return *this;
}

void Data2BSMessage::copy(const Data2BSMessage& other)
{
    this->data_var = other.data_var;
}

void Data2BSMessage::parsimPack(cCommBuffer *b)
{
    ::ClusterMessage::parsimPack(b);
    doPacking(b,this->data_var);
}

void Data2BSMessage::parsimUnpack(cCommBuffer *b)
{
    ::ClusterMessage::parsimUnpack(b);
    doUnpacking(b,this->data_var);
}

int Data2BSMessage::getData() const
{
    return data_var;
}

void Data2BSMessage::setData(int data)
{
    this->data_var = data;
}

class Data2BSMessageDescriptor : public cClassDescriptor
{
  public:
    Data2BSMessageDescriptor();
    virtual ~Data2BSMessageDescriptor();

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

Register_ClassDescriptor(Data2BSMessageDescriptor);

Data2BSMessageDescriptor::Data2BSMessageDescriptor() : cClassDescriptor("Data2BSMessage", "ClusterMessage")
{
}

Data2BSMessageDescriptor::~Data2BSMessageDescriptor()
{
}

bool Data2BSMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Data2BSMessage *>(obj)!=NULL;
}

const char *Data2BSMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int Data2BSMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int Data2BSMessageDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *Data2BSMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "data",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int Data2BSMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='d' && strcmp(fieldName, "data")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *Data2BSMessageDescriptor::getFieldTypeString(void *object, int field) const
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

const char *Data2BSMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int Data2BSMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Data2BSMessage *pp = (Data2BSMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string Data2BSMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Data2BSMessage *pp = (Data2BSMessage *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getData());
        default: return "";
    }
}

bool Data2BSMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Data2BSMessage *pp = (Data2BSMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setData(string2long(value)); return true;
        default: return false;
    }
}

const char *Data2BSMessageDescriptor::getFieldStructName(void *object, int field) const
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

void *Data2BSMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Data2BSMessage *pp = (Data2BSMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}



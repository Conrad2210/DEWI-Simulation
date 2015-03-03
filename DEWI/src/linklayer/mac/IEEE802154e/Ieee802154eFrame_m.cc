//
// Generated file, do not edit! Created by nedtool 4.6 from src/linklayer/mac/IEEE802154e/Ieee802154eFrame.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "Ieee802154eFrame_m.h"

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

Register_Class(Ieee802154eBasicFrame);

Ieee802154eBasicFrame::Ieee802154eBasicFrame(const char *name, int kind) : ::cPacket(name,kind)
{
    this->bdsn_var = 0;
}

Ieee802154eBasicFrame::Ieee802154eBasicFrame(const Ieee802154eBasicFrame& other) : ::cPacket(other)
{
    copy(other);
}

Ieee802154eBasicFrame::~Ieee802154eBasicFrame()
{
}

Ieee802154eBasicFrame& Ieee802154eBasicFrame::operator=(const Ieee802154eBasicFrame& other)
{
    if (this==&other) return *this;
    ::cPacket::operator=(other);
    copy(other);
    return *this;
}

void Ieee802154eBasicFrame::copy(const Ieee802154eBasicFrame& other)
{
    this->bdsn_var = other.bdsn_var;
    this->auxSecHd_var = other.auxSecHd_var;
}

void Ieee802154eBasicFrame::parsimPack(cCommBuffer *b)
{
    ::cPacket::parsimPack(b);
    doPacking(b,this->bdsn_var);
    doPacking(b,this->auxSecHd_var);
}

void Ieee802154eBasicFrame::parsimUnpack(cCommBuffer *b)
{
    ::cPacket::parsimUnpack(b);
    doUnpacking(b,this->bdsn_var);
    doUnpacking(b,this->auxSecHd_var);
}

unsigned char Ieee802154eBasicFrame::getBdsn() const
{
    return bdsn_var;
}

void Ieee802154eBasicFrame::setBdsn(unsigned char bdsn)
{
    this->bdsn_var = bdsn;
}

AuxiliarySecurityHeader& Ieee802154eBasicFrame::getAuxSecHd()
{
    return auxSecHd_var;
}

void Ieee802154eBasicFrame::setAuxSecHd(const AuxiliarySecurityHeader& auxSecHd)
{
    this->auxSecHd_var = auxSecHd;
}

class Ieee802154eBasicFrameDescriptor : public cClassDescriptor
{
  public:
    Ieee802154eBasicFrameDescriptor();
    virtual ~Ieee802154eBasicFrameDescriptor();

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

Register_ClassDescriptor(Ieee802154eBasicFrameDescriptor);

Ieee802154eBasicFrameDescriptor::Ieee802154eBasicFrameDescriptor() : cClassDescriptor("Ieee802154eBasicFrame", "cPacket")
{
}

Ieee802154eBasicFrameDescriptor::~Ieee802154eBasicFrameDescriptor()
{
}

bool Ieee802154eBasicFrameDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Ieee802154eBasicFrame *>(obj)!=NULL;
}

const char *Ieee802154eBasicFrameDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int Ieee802154eBasicFrameDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int Ieee802154eBasicFrameDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *Ieee802154eBasicFrameDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "bdsn",
        "auxSecHd",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int Ieee802154eBasicFrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='b' && strcmp(fieldName, "bdsn")==0) return base+0;
    if (fieldName[0]=='a' && strcmp(fieldName, "auxSecHd")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *Ieee802154eBasicFrameDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned char",
        "AuxiliarySecurityHeader",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *Ieee802154eBasicFrameDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int Ieee802154eBasicFrameDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eBasicFrame *pp = (Ieee802154eBasicFrame *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string Ieee802154eBasicFrameDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eBasicFrame *pp = (Ieee802154eBasicFrame *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getBdsn());
        case 1: {std::stringstream out; out << pp->getAuxSecHd(); return out.str();}
        default: return "";
    }
}

bool Ieee802154eBasicFrameDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eBasicFrame *pp = (Ieee802154eBasicFrame *)object; (void)pp;
    switch (field) {
        case 0: pp->setBdsn(string2ulong(value)); return true;
        default: return false;
    }
}

const char *Ieee802154eBasicFrameDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 1: return opp_typename(typeid(AuxiliarySecurityHeader));
        default: return NULL;
    };
}

void *Ieee802154eBasicFrameDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eBasicFrame *pp = (Ieee802154eBasicFrame *)object; (void)pp;
    switch (field) {
        case 1: return (void *)(&pp->getAuxSecHd()); break;
        default: return NULL;
    }
}

Register_Class(Ieee802154eFrame);

Ieee802154eFrame::Ieee802154eFrame(const char *name, int kind) : ::Ieee802154eBasicFrame(name,kind)
{
    this->seqNmbr_var = 0;
    this->dstPanId_var = 0;
    this->srcPanId_var = 0;
    this->retries_var = 0;
    ieHeader_arraysize = 0;
    this->ieHeader_var = 0;
    iePayload_arraysize = 0;
    this->iePayload_var = 0;
}

Ieee802154eFrame::Ieee802154eFrame(const Ieee802154eFrame& other) : ::Ieee802154eBasicFrame(other)
{
    ieHeader_arraysize = 0;
    this->ieHeader_var = 0;
    iePayload_arraysize = 0;
    this->iePayload_var = 0;
    copy(other);
}

Ieee802154eFrame::~Ieee802154eFrame()
{
    delete [] ieHeader_var;
    delete [] iePayload_var;
}

Ieee802154eFrame& Ieee802154eFrame::operator=(const Ieee802154eFrame& other)
{
    if (this==&other) return *this;
    ::Ieee802154eBasicFrame::operator=(other);
    copy(other);
    return *this;
}

void Ieee802154eFrame::copy(const Ieee802154eFrame& other)
{
    this->frmCtrl_var = other.frmCtrl_var;
    this->seqNmbr_var = other.seqNmbr_var;
    this->dstPanId_var = other.dstPanId_var;
    this->dstAddr_var = other.dstAddr_var;
    this->srcPanId_var = other.srcPanId_var;
    this->srcAddr_var = other.srcAddr_var;
    this->retries_var = other.retries_var;
    delete [] this->ieHeader_var;
    this->ieHeader_var = (other.ieHeader_arraysize==0) ? NULL : new IE_Header[other.ieHeader_arraysize];
    ieHeader_arraysize = other.ieHeader_arraysize;
    for (unsigned int i=0; i<ieHeader_arraysize; i++)
        this->ieHeader_var[i] = other.ieHeader_var[i];
    delete [] this->iePayload_var;
    this->iePayload_var = (other.iePayload_arraysize==0) ? NULL : new IE_Payload[other.iePayload_arraysize];
    iePayload_arraysize = other.iePayload_arraysize;
    for (unsigned int i=0; i<iePayload_arraysize; i++)
        this->iePayload_var[i] = other.iePayload_var[i];
}

void Ieee802154eFrame::parsimPack(cCommBuffer *b)
{
    ::Ieee802154eBasicFrame::parsimPack(b);
    doPacking(b,this->frmCtrl_var);
    doPacking(b,this->seqNmbr_var);
    doPacking(b,this->dstPanId_var);
    doPacking(b,this->dstAddr_var);
    doPacking(b,this->srcPanId_var);
    doPacking(b,this->srcAddr_var);
    doPacking(b,this->retries_var);
    b->pack(ieHeader_arraysize);
    doPacking(b,this->ieHeader_var,ieHeader_arraysize);
    b->pack(iePayload_arraysize);
    doPacking(b,this->iePayload_var,iePayload_arraysize);
}

void Ieee802154eFrame::parsimUnpack(cCommBuffer *b)
{
    ::Ieee802154eBasicFrame::parsimUnpack(b);
    doUnpacking(b,this->frmCtrl_var);
    doUnpacking(b,this->seqNmbr_var);
    doUnpacking(b,this->dstPanId_var);
    doUnpacking(b,this->dstAddr_var);
    doUnpacking(b,this->srcPanId_var);
    doUnpacking(b,this->srcAddr_var);
    doUnpacking(b,this->retries_var);
    delete [] this->ieHeader_var;
    b->unpack(ieHeader_arraysize);
    if (ieHeader_arraysize==0) {
        this->ieHeader_var = 0;
    } else {
        this->ieHeader_var = new IE_Header[ieHeader_arraysize];
        doUnpacking(b,this->ieHeader_var,ieHeader_arraysize);
    }
    delete [] this->iePayload_var;
    b->unpack(iePayload_arraysize);
    if (iePayload_arraysize==0) {
        this->iePayload_var = 0;
    } else {
        this->iePayload_var = new IE_Payload[iePayload_arraysize];
        doUnpacking(b,this->iePayload_var,iePayload_arraysize);
    }
}

FrameCtrl& Ieee802154eFrame::getFrmCtrl()
{
    return frmCtrl_var;
}

void Ieee802154eFrame::setFrmCtrl(const FrameCtrl& frmCtrl)
{
    this->frmCtrl_var = frmCtrl;
}

unsigned char Ieee802154eFrame::getSeqNmbr() const
{
    return seqNmbr_var;
}

void Ieee802154eFrame::setSeqNmbr(unsigned char seqNmbr)
{
    this->seqNmbr_var = seqNmbr;
}

uint16_t Ieee802154eFrame::getDstPanId() const
{
    return dstPanId_var;
}

void Ieee802154eFrame::setDstPanId(uint16_t dstPanId)
{
    this->dstPanId_var = dstPanId;
}

MACAddress& Ieee802154eFrame::getDstAddr()
{
    return dstAddr_var;
}

void Ieee802154eFrame::setDstAddr(const MACAddress& dstAddr)
{
    this->dstAddr_var = dstAddr;
}

uint16_t Ieee802154eFrame::getSrcPanId() const
{
    return srcPanId_var;
}

void Ieee802154eFrame::setSrcPanId(uint16_t srcPanId)
{
    this->srcPanId_var = srcPanId;
}

MACAddress& Ieee802154eFrame::getSrcAddr()
{
    return srcAddr_var;
}

void Ieee802154eFrame::setSrcAddr(const MACAddress& srcAddr)
{
    this->srcAddr_var = srcAddr;
}

uint8_t Ieee802154eFrame::getRetries() const
{
    return retries_var;
}

void Ieee802154eFrame::setRetries(uint8_t retries)
{
    this->retries_var = retries;
}

void Ieee802154eFrame::setIeHeaderArraySize(unsigned int size)
{
    IE_Header *ieHeader_var2 = (size==0) ? NULL : new IE_Header[size];
    unsigned int sz = ieHeader_arraysize < size ? ieHeader_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        ieHeader_var2[i] = this->ieHeader_var[i];
    ieHeader_arraysize = size;
    delete [] this->ieHeader_var;
    this->ieHeader_var = ieHeader_var2;
}

unsigned int Ieee802154eFrame::getIeHeaderArraySize() const
{
    return ieHeader_arraysize;
}

IE_Header& Ieee802154eFrame::getIeHeader(unsigned int k)
{
    if (k>=ieHeader_arraysize) throw cRuntimeError("Array of size %d indexed by %d", ieHeader_arraysize, k);
    return ieHeader_var[k];
}

void Ieee802154eFrame::setIeHeader(unsigned int k, const IE_Header& ieHeader)
{
    if (k>=ieHeader_arraysize) throw cRuntimeError("Array of size %d indexed by %d", ieHeader_arraysize, k);
    this->ieHeader_var[k] = ieHeader;
}

void Ieee802154eFrame::setIePayloadArraySize(unsigned int size)
{
    IE_Payload *iePayload_var2 = (size==0) ? NULL : new IE_Payload[size];
    unsigned int sz = iePayload_arraysize < size ? iePayload_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        iePayload_var2[i] = this->iePayload_var[i];
    iePayload_arraysize = size;
    delete [] this->iePayload_var;
    this->iePayload_var = iePayload_var2;
}

unsigned int Ieee802154eFrame::getIePayloadArraySize() const
{
    return iePayload_arraysize;
}

IE_Payload& Ieee802154eFrame::getIePayload(unsigned int k)
{
    if (k>=iePayload_arraysize) throw cRuntimeError("Array of size %d indexed by %d", iePayload_arraysize, k);
    return iePayload_var[k];
}

void Ieee802154eFrame::setIePayload(unsigned int k, const IE_Payload& iePayload)
{
    if (k>=iePayload_arraysize) throw cRuntimeError("Array of size %d indexed by %d", iePayload_arraysize, k);
    this->iePayload_var[k] = iePayload;
}

class Ieee802154eFrameDescriptor : public cClassDescriptor
{
  public:
    Ieee802154eFrameDescriptor();
    virtual ~Ieee802154eFrameDescriptor();

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

Register_ClassDescriptor(Ieee802154eFrameDescriptor);

Ieee802154eFrameDescriptor::Ieee802154eFrameDescriptor() : cClassDescriptor("Ieee802154eFrame", "Ieee802154eBasicFrame")
{
}

Ieee802154eFrameDescriptor::~Ieee802154eFrameDescriptor()
{
}

bool Ieee802154eFrameDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Ieee802154eFrame *>(obj)!=NULL;
}

const char *Ieee802154eFrameDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int Ieee802154eFrameDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 9+basedesc->getFieldCount(object) : 9;
}

unsigned int Ieee802154eFrameDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISCOMPOUND,
        FD_ISARRAY | FD_ISCOMPOUND,
    };
    return (field>=0 && field<9) ? fieldTypeFlags[field] : 0;
}

const char *Ieee802154eFrameDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "frmCtrl",
        "seqNmbr",
        "dstPanId",
        "dstAddr",
        "srcPanId",
        "srcAddr",
        "retries",
        "ieHeader",
        "iePayload",
    };
    return (field>=0 && field<9) ? fieldNames[field] : NULL;
}

int Ieee802154eFrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "frmCtrl")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "seqNmbr")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "dstPanId")==0) return base+2;
    if (fieldName[0]=='d' && strcmp(fieldName, "dstAddr")==0) return base+3;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcPanId")==0) return base+4;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcAddr")==0) return base+5;
    if (fieldName[0]=='r' && strcmp(fieldName, "retries")==0) return base+6;
    if (fieldName[0]=='i' && strcmp(fieldName, "ieHeader")==0) return base+7;
    if (fieldName[0]=='i' && strcmp(fieldName, "iePayload")==0) return base+8;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *Ieee802154eFrameDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "FrameCtrl",
        "unsigned char",
        "uint16_t",
        "MACAddress",
        "uint16_t",
        "MACAddress",
        "uint8_t",
        "IE_Header",
        "IE_Payload",
    };
    return (field>=0 && field<9) ? fieldTypeStrings[field] : NULL;
}

const char *Ieee802154eFrameDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int Ieee802154eFrameDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eFrame *pp = (Ieee802154eFrame *)object; (void)pp;
    switch (field) {
        case 7: return pp->getIeHeaderArraySize();
        case 8: return pp->getIePayloadArraySize();
        default: return 0;
    }
}

std::string Ieee802154eFrameDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eFrame *pp = (Ieee802154eFrame *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getFrmCtrl(); return out.str();}
        case 1: return ulong2string(pp->getSeqNmbr());
        case 2: return ulong2string(pp->getDstPanId());
        case 3: {std::stringstream out; out << pp->getDstAddr(); return out.str();}
        case 4: return ulong2string(pp->getSrcPanId());
        case 5: {std::stringstream out; out << pp->getSrcAddr(); return out.str();}
        case 6: return ulong2string(pp->getRetries());
        case 7: {std::stringstream out; out << pp->getIeHeader(i); return out.str();}
        case 8: {std::stringstream out; out << pp->getIePayload(i); return out.str();}
        default: return "";
    }
}

bool Ieee802154eFrameDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eFrame *pp = (Ieee802154eFrame *)object; (void)pp;
    switch (field) {
        case 1: pp->setSeqNmbr(string2ulong(value)); return true;
        case 2: pp->setDstPanId(string2ulong(value)); return true;
        case 4: pp->setSrcPanId(string2ulong(value)); return true;
        case 6: pp->setRetries(string2ulong(value)); return true;
        default: return false;
    }
}

const char *Ieee802154eFrameDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0: return opp_typename(typeid(FrameCtrl));
        case 3: return opp_typename(typeid(MACAddress));
        case 5: return opp_typename(typeid(MACAddress));
        case 7: return opp_typename(typeid(IE_Header));
        case 8: return opp_typename(typeid(IE_Payload));
        default: return NULL;
    };
}

void *Ieee802154eFrameDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eFrame *pp = (Ieee802154eFrame *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getFrmCtrl()); break;
        case 3: return (void *)(&pp->getDstAddr()); break;
        case 5: return (void *)(&pp->getSrcAddr()); break;
        case 7: return (void *)(&pp->getIeHeader(i)); break;
        case 8: return (void *)(&pp->getIePayload(i)); break;
        default: return NULL;
    }
}

Register_Class(Ieee802154eBeaconFrame);

Ieee802154eBeaconFrame::Ieee802154eBeaconFrame(const char *name, int kind) : ::Ieee802154eFrame(name,kind)
{
}

Ieee802154eBeaconFrame::Ieee802154eBeaconFrame(const Ieee802154eBeaconFrame& other) : ::Ieee802154eFrame(other)
{
    copy(other);
}

Ieee802154eBeaconFrame::~Ieee802154eBeaconFrame()
{
}

Ieee802154eBeaconFrame& Ieee802154eBeaconFrame::operator=(const Ieee802154eBeaconFrame& other)
{
    if (this==&other) return *this;
    ::Ieee802154eFrame::operator=(other);
    copy(other);
    return *this;
}

void Ieee802154eBeaconFrame::copy(const Ieee802154eBeaconFrame& other)
{
    this->sfSpec_var = other.sfSpec_var;
    this->gtsField_var = other.gtsField_var;
    this->pendingAdrrFld_var = other.pendingAdrrFld_var;
}

void Ieee802154eBeaconFrame::parsimPack(cCommBuffer *b)
{
    ::Ieee802154eFrame::parsimPack(b);
    doPacking(b,this->sfSpec_var);
    doPacking(b,this->gtsField_var);
    doPacking(b,this->pendingAdrrFld_var);
}

void Ieee802154eBeaconFrame::parsimUnpack(cCommBuffer *b)
{
    ::Ieee802154eFrame::parsimUnpack(b);
    doUnpacking(b,this->sfSpec_var);
    doUnpacking(b,this->gtsField_var);
    doUnpacking(b,this->pendingAdrrFld_var);
}

SuperframeSpec& Ieee802154eBeaconFrame::getSfSpec()
{
    return sfSpec_var;
}

void Ieee802154eBeaconFrame::setSfSpec(const SuperframeSpec& sfSpec)
{
    this->sfSpec_var = sfSpec;
}

GTSFields& Ieee802154eBeaconFrame::getGtsField()
{
    return gtsField_var;
}

void Ieee802154eBeaconFrame::setGtsField(const GTSFields& gtsField)
{
    this->gtsField_var = gtsField;
}

PendingAddrFields& Ieee802154eBeaconFrame::getPendingAdrrFld()
{
    return pendingAdrrFld_var;
}

void Ieee802154eBeaconFrame::setPendingAdrrFld(const PendingAddrFields& pendingAdrrFld)
{
    this->pendingAdrrFld_var = pendingAdrrFld;
}

class Ieee802154eBeaconFrameDescriptor : public cClassDescriptor
{
  public:
    Ieee802154eBeaconFrameDescriptor();
    virtual ~Ieee802154eBeaconFrameDescriptor();

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

Register_ClassDescriptor(Ieee802154eBeaconFrameDescriptor);

Ieee802154eBeaconFrameDescriptor::Ieee802154eBeaconFrameDescriptor() : cClassDescriptor("Ieee802154eBeaconFrame", "Ieee802154eFrame")
{
}

Ieee802154eBeaconFrameDescriptor::~Ieee802154eBeaconFrameDescriptor()
{
}

bool Ieee802154eBeaconFrameDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Ieee802154eBeaconFrame *>(obj)!=NULL;
}

const char *Ieee802154eBeaconFrameDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int Ieee802154eBeaconFrameDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int Ieee802154eBeaconFrameDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *Ieee802154eBeaconFrameDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "sfSpec",
        "gtsField",
        "pendingAdrrFld",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int Ieee802154eBeaconFrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sfSpec")==0) return base+0;
    if (fieldName[0]=='g' && strcmp(fieldName, "gtsField")==0) return base+1;
    if (fieldName[0]=='p' && strcmp(fieldName, "pendingAdrrFld")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *Ieee802154eBeaconFrameDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "SuperframeSpec",
        "GTSFields",
        "PendingAddrFields",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *Ieee802154eBeaconFrameDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int Ieee802154eBeaconFrameDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eBeaconFrame *pp = (Ieee802154eBeaconFrame *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string Ieee802154eBeaconFrameDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eBeaconFrame *pp = (Ieee802154eBeaconFrame *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getSfSpec(); return out.str();}
        case 1: {std::stringstream out; out << pp->getGtsField(); return out.str();}
        case 2: {std::stringstream out; out << pp->getPendingAdrrFld(); return out.str();}
        default: return "";
    }
}

bool Ieee802154eBeaconFrameDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eBeaconFrame *pp = (Ieee802154eBeaconFrame *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *Ieee802154eBeaconFrameDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0: return opp_typename(typeid(SuperframeSpec));
        case 1: return opp_typename(typeid(GTSFields));
        case 2: return opp_typename(typeid(PendingAddrFields));
        default: return NULL;
    };
}

void *Ieee802154eBeaconFrameDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eBeaconFrame *pp = (Ieee802154eBeaconFrame *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getSfSpec()); break;
        case 1: return (void *)(&pp->getGtsField()); break;
        case 2: return (void *)(&pp->getPendingAdrrFld()); break;
        default: return NULL;
    }
}

Register_Class(Ieee802154EnhancedBeaconFrame);

Ieee802154EnhancedBeaconFrame::Ieee802154EnhancedBeaconFrame(const char *name, int kind) : ::Ieee802154eFrame(name,kind)
{
}

Ieee802154EnhancedBeaconFrame::Ieee802154EnhancedBeaconFrame(const Ieee802154EnhancedBeaconFrame& other) : ::Ieee802154eFrame(other)
{
    copy(other);
}

Ieee802154EnhancedBeaconFrame::~Ieee802154EnhancedBeaconFrame()
{
}

Ieee802154EnhancedBeaconFrame& Ieee802154EnhancedBeaconFrame::operator=(const Ieee802154EnhancedBeaconFrame& other)
{
    if (this==&other) return *this;
    ::Ieee802154eFrame::operator=(other);
    copy(other);
    return *this;
}

void Ieee802154EnhancedBeaconFrame::copy(const Ieee802154EnhancedBeaconFrame& other)
{
}

void Ieee802154EnhancedBeaconFrame::parsimPack(cCommBuffer *b)
{
    ::Ieee802154eFrame::parsimPack(b);
}

void Ieee802154EnhancedBeaconFrame::parsimUnpack(cCommBuffer *b)
{
    ::Ieee802154eFrame::parsimUnpack(b);
}

class Ieee802154EnhancedBeaconFrameDescriptor : public cClassDescriptor
{
  public:
    Ieee802154EnhancedBeaconFrameDescriptor();
    virtual ~Ieee802154EnhancedBeaconFrameDescriptor();

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

Register_ClassDescriptor(Ieee802154EnhancedBeaconFrameDescriptor);

Ieee802154EnhancedBeaconFrameDescriptor::Ieee802154EnhancedBeaconFrameDescriptor() : cClassDescriptor("Ieee802154EnhancedBeaconFrame", "Ieee802154eFrame")
{
}

Ieee802154EnhancedBeaconFrameDescriptor::~Ieee802154EnhancedBeaconFrameDescriptor()
{
}

bool Ieee802154EnhancedBeaconFrameDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Ieee802154EnhancedBeaconFrame *>(obj)!=NULL;
}

const char *Ieee802154EnhancedBeaconFrameDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int Ieee802154EnhancedBeaconFrameDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int Ieee802154EnhancedBeaconFrameDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *Ieee802154EnhancedBeaconFrameDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int Ieee802154EnhancedBeaconFrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *Ieee802154EnhancedBeaconFrameDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *Ieee802154EnhancedBeaconFrameDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int Ieee802154EnhancedBeaconFrameDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154EnhancedBeaconFrame *pp = (Ieee802154EnhancedBeaconFrame *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string Ieee802154EnhancedBeaconFrameDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154EnhancedBeaconFrame *pp = (Ieee802154EnhancedBeaconFrame *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool Ieee802154EnhancedBeaconFrameDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154EnhancedBeaconFrame *pp = (Ieee802154EnhancedBeaconFrame *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *Ieee802154EnhancedBeaconFrameDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *Ieee802154EnhancedBeaconFrameDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154EnhancedBeaconFrame *pp = (Ieee802154EnhancedBeaconFrame *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(Ieee802154eDataFrame);

Ieee802154eDataFrame::Ieee802154eDataFrame(const char *name, int kind) : ::Ieee802154eFrame(name,kind)
{
}

Ieee802154eDataFrame::Ieee802154eDataFrame(const Ieee802154eDataFrame& other) : ::Ieee802154eFrame(other)
{
    copy(other);
}

Ieee802154eDataFrame::~Ieee802154eDataFrame()
{
}

Ieee802154eDataFrame& Ieee802154eDataFrame::operator=(const Ieee802154eDataFrame& other)
{
    if (this==&other) return *this;
    ::Ieee802154eFrame::operator=(other);
    copy(other);
    return *this;
}

void Ieee802154eDataFrame::copy(const Ieee802154eDataFrame& other)
{
}

void Ieee802154eDataFrame::parsimPack(cCommBuffer *b)
{
    ::Ieee802154eFrame::parsimPack(b);
}

void Ieee802154eDataFrame::parsimUnpack(cCommBuffer *b)
{
    ::Ieee802154eFrame::parsimUnpack(b);
}

class Ieee802154eDataFrameDescriptor : public cClassDescriptor
{
  public:
    Ieee802154eDataFrameDescriptor();
    virtual ~Ieee802154eDataFrameDescriptor();

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

Register_ClassDescriptor(Ieee802154eDataFrameDescriptor);

Ieee802154eDataFrameDescriptor::Ieee802154eDataFrameDescriptor() : cClassDescriptor("Ieee802154eDataFrame", "Ieee802154eFrame")
{
}

Ieee802154eDataFrameDescriptor::~Ieee802154eDataFrameDescriptor()
{
}

bool Ieee802154eDataFrameDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Ieee802154eDataFrame *>(obj)!=NULL;
}

const char *Ieee802154eDataFrameDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int Ieee802154eDataFrameDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int Ieee802154eDataFrameDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *Ieee802154eDataFrameDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int Ieee802154eDataFrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *Ieee802154eDataFrameDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *Ieee802154eDataFrameDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int Ieee802154eDataFrameDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eDataFrame *pp = (Ieee802154eDataFrame *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string Ieee802154eDataFrameDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eDataFrame *pp = (Ieee802154eDataFrame *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool Ieee802154eDataFrameDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eDataFrame *pp = (Ieee802154eDataFrame *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *Ieee802154eDataFrameDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *Ieee802154eDataFrameDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eDataFrame *pp = (Ieee802154eDataFrame *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(Ieee802154eAckFrame);

Ieee802154eAckFrame::Ieee802154eAckFrame(const char *name, int kind) : ::Ieee802154eFrame(name,kind)
{
}

Ieee802154eAckFrame::Ieee802154eAckFrame(const Ieee802154eAckFrame& other) : ::Ieee802154eFrame(other)
{
    copy(other);
}

Ieee802154eAckFrame::~Ieee802154eAckFrame()
{
}

Ieee802154eAckFrame& Ieee802154eAckFrame::operator=(const Ieee802154eAckFrame& other)
{
    if (this==&other) return *this;
    ::Ieee802154eFrame::operator=(other);
    copy(other);
    return *this;
}

void Ieee802154eAckFrame::copy(const Ieee802154eAckFrame& other)
{
}

void Ieee802154eAckFrame::parsimPack(cCommBuffer *b)
{
    ::Ieee802154eFrame::parsimPack(b);
}

void Ieee802154eAckFrame::parsimUnpack(cCommBuffer *b)
{
    ::Ieee802154eFrame::parsimUnpack(b);
}

class Ieee802154eAckFrameDescriptor : public cClassDescriptor
{
  public:
    Ieee802154eAckFrameDescriptor();
    virtual ~Ieee802154eAckFrameDescriptor();

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

Register_ClassDescriptor(Ieee802154eAckFrameDescriptor);

Ieee802154eAckFrameDescriptor::Ieee802154eAckFrameDescriptor() : cClassDescriptor("Ieee802154eAckFrame", "Ieee802154eFrame")
{
}

Ieee802154eAckFrameDescriptor::~Ieee802154eAckFrameDescriptor()
{
}

bool Ieee802154eAckFrameDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Ieee802154eAckFrame *>(obj)!=NULL;
}

const char *Ieee802154eAckFrameDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int Ieee802154eAckFrameDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int Ieee802154eAckFrameDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *Ieee802154eAckFrameDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int Ieee802154eAckFrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *Ieee802154eAckFrameDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *Ieee802154eAckFrameDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int Ieee802154eAckFrameDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eAckFrame *pp = (Ieee802154eAckFrame *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string Ieee802154eAckFrameDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eAckFrame *pp = (Ieee802154eAckFrame *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool Ieee802154eAckFrameDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eAckFrame *pp = (Ieee802154eAckFrame *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *Ieee802154eAckFrameDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *Ieee802154eAckFrameDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eAckFrame *pp = (Ieee802154eAckFrame *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(Ieee802154eEnhancedAckFrame);

Ieee802154eEnhancedAckFrame::Ieee802154eEnhancedAckFrame(const char *name, int kind) : ::Ieee802154eFrame(name,kind)
{
}

Ieee802154eEnhancedAckFrame::Ieee802154eEnhancedAckFrame(const Ieee802154eEnhancedAckFrame& other) : ::Ieee802154eFrame(other)
{
    copy(other);
}

Ieee802154eEnhancedAckFrame::~Ieee802154eEnhancedAckFrame()
{
}

Ieee802154eEnhancedAckFrame& Ieee802154eEnhancedAckFrame::operator=(const Ieee802154eEnhancedAckFrame& other)
{
    if (this==&other) return *this;
    ::Ieee802154eFrame::operator=(other);
    copy(other);
    return *this;
}

void Ieee802154eEnhancedAckFrame::copy(const Ieee802154eEnhancedAckFrame& other)
{
}

void Ieee802154eEnhancedAckFrame::parsimPack(cCommBuffer *b)
{
    ::Ieee802154eFrame::parsimPack(b);
}

void Ieee802154eEnhancedAckFrame::parsimUnpack(cCommBuffer *b)
{
    ::Ieee802154eFrame::parsimUnpack(b);
}

class Ieee802154eEnhancedAckFrameDescriptor : public cClassDescriptor
{
  public:
    Ieee802154eEnhancedAckFrameDescriptor();
    virtual ~Ieee802154eEnhancedAckFrameDescriptor();

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

Register_ClassDescriptor(Ieee802154eEnhancedAckFrameDescriptor);

Ieee802154eEnhancedAckFrameDescriptor::Ieee802154eEnhancedAckFrameDescriptor() : cClassDescriptor("Ieee802154eEnhancedAckFrame", "Ieee802154eFrame")
{
}

Ieee802154eEnhancedAckFrameDescriptor::~Ieee802154eEnhancedAckFrameDescriptor()
{
}

bool Ieee802154eEnhancedAckFrameDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Ieee802154eEnhancedAckFrame *>(obj)!=NULL;
}

const char *Ieee802154eEnhancedAckFrameDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int Ieee802154eEnhancedAckFrameDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int Ieee802154eEnhancedAckFrameDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *Ieee802154eEnhancedAckFrameDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int Ieee802154eEnhancedAckFrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *Ieee802154eEnhancedAckFrameDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *Ieee802154eEnhancedAckFrameDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int Ieee802154eEnhancedAckFrameDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eEnhancedAckFrame *pp = (Ieee802154eEnhancedAckFrame *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string Ieee802154eEnhancedAckFrameDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eEnhancedAckFrame *pp = (Ieee802154eEnhancedAckFrame *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool Ieee802154eEnhancedAckFrameDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eEnhancedAckFrame *pp = (Ieee802154eEnhancedAckFrame *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *Ieee802154eEnhancedAckFrameDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *Ieee802154eEnhancedAckFrameDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eEnhancedAckFrame *pp = (Ieee802154eEnhancedAckFrame *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(Ieee802154eAssociationFrame);

Ieee802154eAssociationFrame::Ieee802154eAssociationFrame(const char *name, int kind) : ::Ieee802154eFrame(name,kind)
{
}

Ieee802154eAssociationFrame::Ieee802154eAssociationFrame(const Ieee802154eAssociationFrame& other) : ::Ieee802154eFrame(other)
{
    copy(other);
}

Ieee802154eAssociationFrame::~Ieee802154eAssociationFrame()
{
}

Ieee802154eAssociationFrame& Ieee802154eAssociationFrame::operator=(const Ieee802154eAssociationFrame& other)
{
    if (this==&other) return *this;
    ::Ieee802154eFrame::operator=(other);
    copy(other);
    return *this;
}

void Ieee802154eAssociationFrame::copy(const Ieee802154eAssociationFrame& other)
{
    this->CntrlInfo_var = other.CntrlInfo_var;
}

void Ieee802154eAssociationFrame::parsimPack(cCommBuffer *b)
{
    ::Ieee802154eFrame::parsimPack(b);
    doPacking(b,this->CntrlInfo_var);
}

void Ieee802154eAssociationFrame::parsimUnpack(cCommBuffer *b)
{
    ::Ieee802154eFrame::parsimUnpack(b);
    doUnpacking(b,this->CntrlInfo_var);
}

Ieee802154eNetworkCtrlInfo& Ieee802154eAssociationFrame::getCntrlInfo()
{
    return CntrlInfo_var;
}

void Ieee802154eAssociationFrame::setCntrlInfo(const Ieee802154eNetworkCtrlInfo& CntrlInfo)
{
    this->CntrlInfo_var = CntrlInfo;
}

class Ieee802154eAssociationFrameDescriptor : public cClassDescriptor
{
  public:
    Ieee802154eAssociationFrameDescriptor();
    virtual ~Ieee802154eAssociationFrameDescriptor();

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

Register_ClassDescriptor(Ieee802154eAssociationFrameDescriptor);

Ieee802154eAssociationFrameDescriptor::Ieee802154eAssociationFrameDescriptor() : cClassDescriptor("Ieee802154eAssociationFrame", "Ieee802154eFrame")
{
}

Ieee802154eAssociationFrameDescriptor::~Ieee802154eAssociationFrameDescriptor()
{
}

bool Ieee802154eAssociationFrameDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Ieee802154eAssociationFrame *>(obj)!=NULL;
}

const char *Ieee802154eAssociationFrameDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int Ieee802154eAssociationFrameDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int Ieee802154eAssociationFrameDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *Ieee802154eAssociationFrameDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "CntrlInfo",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int Ieee802154eAssociationFrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='C' && strcmp(fieldName, "CntrlInfo")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *Ieee802154eAssociationFrameDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "Ieee802154eNetworkCtrlInfo",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *Ieee802154eAssociationFrameDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int Ieee802154eAssociationFrameDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eAssociationFrame *pp = (Ieee802154eAssociationFrame *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string Ieee802154eAssociationFrameDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eAssociationFrame *pp = (Ieee802154eAssociationFrame *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getCntrlInfo(); return out.str();}
        default: return "";
    }
}

bool Ieee802154eAssociationFrameDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eAssociationFrame *pp = (Ieee802154eAssociationFrame *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *Ieee802154eAssociationFrameDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0: return opp_typename(typeid(Ieee802154eNetworkCtrlInfo));
        default: return NULL;
    };
}

void *Ieee802154eAssociationFrameDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eAssociationFrame *pp = (Ieee802154eAssociationFrame *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getCntrlInfo()); break;
        default: return NULL;
    }
}

Register_Class(Ieee802154eScheduleFrame);

Ieee802154eScheduleFrame::Ieee802154eScheduleFrame(const char *name, int kind) : ::Ieee802154eFrame(name,kind)
{
}

Ieee802154eScheduleFrame::Ieee802154eScheduleFrame(const Ieee802154eScheduleFrame& other) : ::Ieee802154eFrame(other)
{
    copy(other);
}

Ieee802154eScheduleFrame::~Ieee802154eScheduleFrame()
{
}

Ieee802154eScheduleFrame& Ieee802154eScheduleFrame::operator=(const Ieee802154eScheduleFrame& other)
{
    if (this==&other) return *this;
    ::Ieee802154eFrame::operator=(other);
    copy(other);
    return *this;
}

void Ieee802154eScheduleFrame::copy(const Ieee802154eScheduleFrame& other)
{
    this->CntrlInfo_var = other.CntrlInfo_var;
}

void Ieee802154eScheduleFrame::parsimPack(cCommBuffer *b)
{
    ::Ieee802154eFrame::parsimPack(b);
    doPacking(b,this->CntrlInfo_var);
}

void Ieee802154eScheduleFrame::parsimUnpack(cCommBuffer *b)
{
    ::Ieee802154eFrame::parsimUnpack(b);
    doUnpacking(b,this->CntrlInfo_var);
}

Ieee802154eNetworkCtrlInfo& Ieee802154eScheduleFrame::getCntrlInfo()
{
    return CntrlInfo_var;
}

void Ieee802154eScheduleFrame::setCntrlInfo(const Ieee802154eNetworkCtrlInfo& CntrlInfo)
{
    this->CntrlInfo_var = CntrlInfo;
}

class Ieee802154eScheduleFrameDescriptor : public cClassDescriptor
{
  public:
    Ieee802154eScheduleFrameDescriptor();
    virtual ~Ieee802154eScheduleFrameDescriptor();

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

Register_ClassDescriptor(Ieee802154eScheduleFrameDescriptor);

Ieee802154eScheduleFrameDescriptor::Ieee802154eScheduleFrameDescriptor() : cClassDescriptor("Ieee802154eScheduleFrame", "Ieee802154eFrame")
{
}

Ieee802154eScheduleFrameDescriptor::~Ieee802154eScheduleFrameDescriptor()
{
}

bool Ieee802154eScheduleFrameDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Ieee802154eScheduleFrame *>(obj)!=NULL;
}

const char *Ieee802154eScheduleFrameDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int Ieee802154eScheduleFrameDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int Ieee802154eScheduleFrameDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *Ieee802154eScheduleFrameDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "CntrlInfo",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int Ieee802154eScheduleFrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='C' && strcmp(fieldName, "CntrlInfo")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *Ieee802154eScheduleFrameDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "Ieee802154eNetworkCtrlInfo",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *Ieee802154eScheduleFrameDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int Ieee802154eScheduleFrameDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eScheduleFrame *pp = (Ieee802154eScheduleFrame *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string Ieee802154eScheduleFrameDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eScheduleFrame *pp = (Ieee802154eScheduleFrame *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getCntrlInfo(); return out.str();}
        default: return "";
    }
}

bool Ieee802154eScheduleFrameDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eScheduleFrame *pp = (Ieee802154eScheduleFrame *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *Ieee802154eScheduleFrameDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0: return opp_typename(typeid(Ieee802154eNetworkCtrlInfo));
        default: return NULL;
    };
}

void *Ieee802154eScheduleFrameDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eScheduleFrame *pp = (Ieee802154eScheduleFrame *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getCntrlInfo()); break;
        default: return NULL;
    }
}

Register_Class(Ieee802154eCmdFrame);

Ieee802154eCmdFrame::Ieee802154eCmdFrame(const char *name, int kind) : ::Ieee802154eFrame(name,kind)
{
    this->cmdType_var = 0;
}

Ieee802154eCmdFrame::Ieee802154eCmdFrame(const Ieee802154eCmdFrame& other) : ::Ieee802154eFrame(other)
{
    copy(other);
}

Ieee802154eCmdFrame::~Ieee802154eCmdFrame()
{
}

Ieee802154eCmdFrame& Ieee802154eCmdFrame::operator=(const Ieee802154eCmdFrame& other)
{
    if (this==&other) return *this;
    ::Ieee802154eFrame::operator=(other);
    copy(other);
    return *this;
}

void Ieee802154eCmdFrame::copy(const Ieee802154eCmdFrame& other)
{
    this->cmdType_var = other.cmdType_var;
}

void Ieee802154eCmdFrame::parsimPack(cCommBuffer *b)
{
    ::Ieee802154eFrame::parsimPack(b);
    doPacking(b,this->cmdType_var);
}

void Ieee802154eCmdFrame::parsimUnpack(cCommBuffer *b)
{
    ::Ieee802154eFrame::parsimUnpack(b);
    doUnpacking(b,this->cmdType_var);
}

int Ieee802154eCmdFrame::getCmdType() const
{
    return cmdType_var;
}

void Ieee802154eCmdFrame::setCmdType(int cmdType)
{
    this->cmdType_var = cmdType;
}

class Ieee802154eCmdFrameDescriptor : public cClassDescriptor
{
  public:
    Ieee802154eCmdFrameDescriptor();
    virtual ~Ieee802154eCmdFrameDescriptor();

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

Register_ClassDescriptor(Ieee802154eCmdFrameDescriptor);

Ieee802154eCmdFrameDescriptor::Ieee802154eCmdFrameDescriptor() : cClassDescriptor("Ieee802154eCmdFrame", "Ieee802154eFrame")
{
}

Ieee802154eCmdFrameDescriptor::~Ieee802154eCmdFrameDescriptor()
{
}

bool Ieee802154eCmdFrameDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Ieee802154eCmdFrame *>(obj)!=NULL;
}

const char *Ieee802154eCmdFrameDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int Ieee802154eCmdFrameDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int Ieee802154eCmdFrameDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *Ieee802154eCmdFrameDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "cmdType",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int Ieee802154eCmdFrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "cmdType")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *Ieee802154eCmdFrameDescriptor::getFieldTypeString(void *object, int field) const
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

const char *Ieee802154eCmdFrameDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "Ieee802154eMacCmdType";
            return NULL;
        default: return NULL;
    }
}

int Ieee802154eCmdFrameDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eCmdFrame *pp = (Ieee802154eCmdFrame *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string Ieee802154eCmdFrameDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eCmdFrame *pp = (Ieee802154eCmdFrame *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getCmdType());
        default: return "";
    }
}

bool Ieee802154eCmdFrameDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eCmdFrame *pp = (Ieee802154eCmdFrame *)object; (void)pp;
    switch (field) {
        case 0: pp->setCmdType(string2long(value)); return true;
        default: return false;
    }
}

const char *Ieee802154eCmdFrameDescriptor::getFieldStructName(void *object, int field) const
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

void *Ieee802154eCmdFrameDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eCmdFrame *pp = (Ieee802154eCmdFrame *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(Ieee802154eMPFrame);

Ieee802154eMPFrame::Ieee802154eMPFrame(const char *name, int kind) : ::Ieee802154eFrame(name,kind)
{
}

Ieee802154eMPFrame::Ieee802154eMPFrame(const Ieee802154eMPFrame& other) : ::Ieee802154eFrame(other)
{
    copy(other);
}

Ieee802154eMPFrame::~Ieee802154eMPFrame()
{
}

Ieee802154eMPFrame& Ieee802154eMPFrame::operator=(const Ieee802154eMPFrame& other)
{
    if (this==&other) return *this;
    ::Ieee802154eFrame::operator=(other);
    copy(other);
    return *this;
}

void Ieee802154eMPFrame::copy(const Ieee802154eMPFrame& other)
{
    this->mpFrmCtrl_var = other.mpFrmCtrl_var;
}

void Ieee802154eMPFrame::parsimPack(cCommBuffer *b)
{
    ::Ieee802154eFrame::parsimPack(b);
    doPacking(b,this->mpFrmCtrl_var);
}

void Ieee802154eMPFrame::parsimUnpack(cCommBuffer *b)
{
    ::Ieee802154eFrame::parsimUnpack(b);
    doUnpacking(b,this->mpFrmCtrl_var);
}

MPFrameCtrl& Ieee802154eMPFrame::getMpFrmCtrl()
{
    return mpFrmCtrl_var;
}

void Ieee802154eMPFrame::setMpFrmCtrl(const MPFrameCtrl& mpFrmCtrl)
{
    this->mpFrmCtrl_var = mpFrmCtrl;
}

class Ieee802154eMPFrameDescriptor : public cClassDescriptor
{
  public:
    Ieee802154eMPFrameDescriptor();
    virtual ~Ieee802154eMPFrameDescriptor();

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

Register_ClassDescriptor(Ieee802154eMPFrameDescriptor);

Ieee802154eMPFrameDescriptor::Ieee802154eMPFrameDescriptor() : cClassDescriptor("Ieee802154eMPFrame", "Ieee802154eFrame")
{
}

Ieee802154eMPFrameDescriptor::~Ieee802154eMPFrameDescriptor()
{
}

bool Ieee802154eMPFrameDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Ieee802154eMPFrame *>(obj)!=NULL;
}

const char *Ieee802154eMPFrameDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int Ieee802154eMPFrameDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int Ieee802154eMPFrameDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *Ieee802154eMPFrameDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "mpFrmCtrl",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int Ieee802154eMPFrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='m' && strcmp(fieldName, "mpFrmCtrl")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *Ieee802154eMPFrameDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "MPFrameCtrl",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *Ieee802154eMPFrameDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int Ieee802154eMPFrameDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eMPFrame *pp = (Ieee802154eMPFrame *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string Ieee802154eMPFrameDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eMPFrame *pp = (Ieee802154eMPFrame *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getMpFrmCtrl(); return out.str();}
        default: return "";
    }
}

bool Ieee802154eMPFrameDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eMPFrame *pp = (Ieee802154eMPFrame *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *Ieee802154eMPFrameDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0: return opp_typename(typeid(MPFrameCtrl));
        default: return NULL;
    };
}

void *Ieee802154eMPFrameDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Ieee802154eMPFrame *pp = (Ieee802154eMPFrame *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getMpFrmCtrl()); break;
        default: return NULL;
    }
}



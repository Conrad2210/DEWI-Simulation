//
// Generated file, do not edit! Created by nedtool 4.6 from src/linklayer/mac/CCSMA/CCSMAFrame.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "CCSMAFrame_m.h"

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

Register_Class(CCSMAFrame);

CCSMAFrame::CCSMAFrame(const char *name, int kind) : ::Ieee802154Frame(name,kind)
{
    this->HopCount_var = 0;
}

CCSMAFrame::CCSMAFrame(const CCSMAFrame& other) : ::Ieee802154Frame(other)
{
    copy(other);
}

CCSMAFrame::~CCSMAFrame()
{
}

CCSMAFrame& CCSMAFrame::operator=(const CCSMAFrame& other)
{
    if (this==&other) return *this;
    ::Ieee802154Frame::operator=(other);
    copy(other);
    return *this;
}

void CCSMAFrame::copy(const CCSMAFrame& other)
{
    this->HopCount_var = other.HopCount_var;
}

void CCSMAFrame::parsimPack(cCommBuffer *b)
{
    ::Ieee802154Frame::parsimPack(b);
    doPacking(b,this->HopCount_var);
}

void CCSMAFrame::parsimUnpack(cCommBuffer *b)
{
    ::Ieee802154Frame::parsimUnpack(b);
    doUnpacking(b,this->HopCount_var);
}

int CCSMAFrame::getHopCount() const
{
    return HopCount_var;
}

void CCSMAFrame::setHopCount(int HopCount)
{
    this->HopCount_var = HopCount;
}

class CCSMAFrameDescriptor : public cClassDescriptor
{
  public:
    CCSMAFrameDescriptor();
    virtual ~CCSMAFrameDescriptor();

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

Register_ClassDescriptor(CCSMAFrameDescriptor);

CCSMAFrameDescriptor::CCSMAFrameDescriptor() : cClassDescriptor("CCSMAFrame", "Ieee802154Frame")
{
}

CCSMAFrameDescriptor::~CCSMAFrameDescriptor()
{
}

bool CCSMAFrameDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CCSMAFrame *>(obj)!=NULL;
}

const char *CCSMAFrameDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CCSMAFrameDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int CCSMAFrameDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *CCSMAFrameDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "HopCount",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int CCSMAFrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='H' && strcmp(fieldName, "HopCount")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CCSMAFrameDescriptor::getFieldTypeString(void *object, int field) const
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

const char *CCSMAFrameDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int CCSMAFrameDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CCSMAFrame *pp = (CCSMAFrame *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CCSMAFrameDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CCSMAFrame *pp = (CCSMAFrame *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getHopCount());
        default: return "";
    }
}

bool CCSMAFrameDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CCSMAFrame *pp = (CCSMAFrame *)object; (void)pp;
    switch (field) {
        case 0: pp->setHopCount(string2long(value)); return true;
        default: return false;
    }
}

const char *CCSMAFrameDescriptor::getFieldStructName(void *object, int field) const
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

void *CCSMAFrameDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CCSMAFrame *pp = (CCSMAFrame *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}



//
// Generated file, do not edit! Created by nedtool 4.6 from src/networklayer/ipv4/IGMPMessage.msg.
//

#ifndef _IGMPMESSAGE_M_H_
#define _IGMPMESSAGE_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
#include "INETDefs.h"
#include "IPv4Address.h"
// }}

/**
 * Enum generated from <tt>src/networklayer/ipv4/IGMPMessage.msg:27</tt> by nedtool.
 * <pre>
 * enum IGMPType
 * {
 * 
 *     IGMP_MEMBERSHIP_QUERY = 0x11;
 *     IGMPV1_MEMBERSHIP_REPORT = 0x12;
 *     IGMPV2_MEMBERSHIP_REPORT = 0x16;
 *     IGMPV2_LEAVE_GROUP = 0x17;
 *     IGMPV3_MEMBERSHIP_REPORT = 0x22;
 * }
 * </pre>
 */
enum IGMPType {
    IGMP_MEMBERSHIP_QUERY = 0x11,
    IGMPV1_MEMBERSHIP_REPORT = 0x12,
    IGMPV2_MEMBERSHIP_REPORT = 0x16,
    IGMPV2_LEAVE_GROUP = 0x17,
    IGMPV3_MEMBERSHIP_REPORT = 0x22
};

/**
 * Class generated from <tt>src/networklayer/ipv4/IGMPMessage.msg:36</tt> by nedtool.
 * <pre>
 * packet IGMPMessage
 * {
 *     int type @enum(IGMPType);
 *     int maxRespTime;
 *     IPv4Address groupAddress;
 * }
 * </pre>
 */
class IGMPMessage : public ::cPacket
{
  protected:
    int type_var;
    int maxRespTime_var;
    IPv4Address groupAddress_var;

  private:
    void copy(const IGMPMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const IGMPMessage&);

  public:
    IGMPMessage(const char *name=NULL, int kind=0);
    IGMPMessage(const IGMPMessage& other);
    virtual ~IGMPMessage();
    IGMPMessage& operator=(const IGMPMessage& other);
    virtual IGMPMessage *dup() const {return new IGMPMessage(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getType() const;
    virtual void setType(int type);
    virtual int getMaxRespTime() const;
    virtual void setMaxRespTime(int maxRespTime);
    virtual IPv4Address& getGroupAddress();
    virtual const IPv4Address& getGroupAddress() const {return const_cast<IGMPMessage*>(this)->getGroupAddress();}
    virtual void setGroupAddress(const IPv4Address& groupAddress);
};

inline void doPacking(cCommBuffer *b, IGMPMessage& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, IGMPMessage& obj) {obj.parsimUnpack(b);}


#endif // ifndef _IGMPMESSAGE_M_H_


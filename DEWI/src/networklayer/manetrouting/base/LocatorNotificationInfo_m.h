//
// Generated file, do not edit! Created by opp_msgc 4.4 from src/networklayer/manetrouting/base/LocatorNotificationInfo.msg.
//

#ifndef _LOCATORNOTIFICATIONINFO_M_H_
#define _LOCATORNOTIFICATIONINFO_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0404
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif

// cplusplus {{
#include "MACAddress.h"
#include "IPv4Address.h"
// }}



/**
 * Class generated from <tt>src/networklayer/manetrouting/base/LocatorNotificationInfo.msg</tt> by opp_msgc.
 * <pre>
 * class LocatorNotificationInfo
 * {
 *       MACAddress macAddr;   
 *       IPv4Address ipAddr;
 * }
 * </pre>
 */
class LocatorNotificationInfo : public ::cObject
{
  protected:
    MACAddress macAddr_var;
    IPv4Address ipAddr_var;

  private:
    void copy(const LocatorNotificationInfo& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const LocatorNotificationInfo&);

  public:
    LocatorNotificationInfo();
    LocatorNotificationInfo(const LocatorNotificationInfo& other);
    virtual ~LocatorNotificationInfo();
    LocatorNotificationInfo& operator=(const LocatorNotificationInfo& other);
    virtual LocatorNotificationInfo *dup() const {return new LocatorNotificationInfo(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual MACAddress& getMacAddr();
    virtual const MACAddress& getMacAddr() const {return const_cast<LocatorNotificationInfo*>(this)->getMacAddr();}
    virtual void setMacAddr(const MACAddress& macAddr);
    virtual IPv4Address& getIpAddr();
    virtual const IPv4Address& getIpAddr() const {return const_cast<LocatorNotificationInfo*>(this)->getIpAddr();}
    virtual void setIpAddr(const IPv4Address& ipAddr);
};

inline void doPacking(cCommBuffer *b, LocatorNotificationInfo& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, LocatorNotificationInfo& obj) {obj.parsimUnpack(b);}


#endif // _LOCATORNOTIFICATIONINFO_M_H_

//
// Generated file, do not edit! Created by nedtool 4.6 from src/networklayer/contract/IPv6ControlInfo.msg.
//

#ifndef _IPV6CONTROLINFO_M_H_
#define _IPV6CONTROLINFO_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
#include "IPv6Address.h"
#include "IPProtocolId_m.h"

class IPv6ExtensionHeader;
typedef IPv6ExtensionHeader *IPv6ExtensionHeaderPtr;
std::ostream& operator<<(std::ostream& os, IPv6ExtensionHeaderPtr eh);

#ifndef WITH_IPv6
inline std::ostream& operator<<(std::ostream& os, IPv6ExtensionHeaderPtr eh) {return os;}
#endif
// }}

/**
 * Class generated from <tt>src/networklayer/contract/IPv6ControlInfo.msg:64</tt> by nedtool.
 * <pre>
 * //
 * // Control information for sending/receiving packets over IPv6.
 * //
 * // To send a packet over IPv6, fill in an ~IPv6ControlInfo object,
 * // attach it to the packet with the C++ method setControlInfo(),
 * // the send it to the ~IPv6 module.
 * //
 * // When sending, the following fields are required:
 * // - protocol: a value from ~IPProtocolId
 * // - destAddr
 * //
 * // Optional fields:
 * // - srcAddr: it will be set to the address of the outgoing interface
 * // - timeToLive
 * //
 * // When ~IPv6 delivers a packet to higher layers, it also attaches an
 * // ~IPv6ControlInfo to it. It fills in the following fields:
 * //  - srcAddr, destAddr, protocol, hopLimit: values from the original datagram
 * //  - interfaceId: the interface on which the datagram arrived, or -1 if it was
 * //    created locally
 * //
 * // ~IPv6 also puts the original datagram object into the control info, because
 * // it may be needed by higher layers.
 * //
 * class IPv6ControlInfo
 * {
 *     @customize(true);
 *     int protocol @enum(IPProtocolId);  // transport layer protocol
 *     IPv6Address destAddr; // destination IPv6 address
 *     IPv6Address srcAddr;  // source IPv6 address
 *     unsigned char trafficClass;  // Traffic Class byte (low 6 bits: DSCP; highest 2 bits: ECN)
 *     abstract int diffServCodePoint;  // maps to bits 0-5 of trafficClass
 *     abstract int explicitCongestionNotification;  // maps to bits 6-7 of trafficClass
 *     short hopLimit;       // hop limit
 *     int interfaceId = -1; // interface on which the datagram was received, or
 *                           // should be sent (see ~InterfaceTable)
 *     bool multicastLoop;   // if true, sent multicast datagrams will be looped back
 *     abstract IPv6ExtensionHeaderPtr extensionHeader[]; // array of extension headers, subclassed from ~IPv6ExtensionHeader
 * }
 * </pre>
 *
 * IPv6ControlInfo_Base is only useful if it gets subclassed, and IPv6ControlInfo is derived from it.
 * The minimum code to be written for IPv6ControlInfo is the following:
 *
 * <pre>
 * class IPv6ControlInfo : public IPv6ControlInfo_Base
 * {
 *   private:
 *     void copy(const IPv6ControlInfo& other) { ... }

 *   public:
 *     IPv6ControlInfo() : IPv6ControlInfo_Base() {}
 *     IPv6ControlInfo(const IPv6ControlInfo& other) : IPv6ControlInfo_Base(other) {copy(other);}
 *     IPv6ControlInfo& operator=(const IPv6ControlInfo& other) {if (this==&other) return *this; IPv6ControlInfo_Base::operator=(other); copy(other); return *this;}
 *     virtual IPv6ControlInfo *dup() const {return new IPv6ControlInfo(*this);}
 *     // ADD CODE HERE to redefine and implement pure virtual functions from IPv6ControlInfo_Base
 * };
 * </pre>
 *
 * The following should go into a .cc (.cpp) file:
 *
 * <pre>
 * Register_Class(IPv6ControlInfo);
 * </pre>
 */
class IPv6ControlInfo_Base : public ::cObject
{
  protected:
    int protocol_var;
    IPv6Address destAddr_var;
    IPv6Address srcAddr_var;
    unsigned char trafficClass_var;
    short hopLimit_var;
    int interfaceId_var;
    bool multicastLoop_var;

  private:
    void copy(const IPv6ControlInfo_Base& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const IPv6ControlInfo_Base&);
    // make constructors protected to avoid instantiation
    IPv6ControlInfo_Base();
    IPv6ControlInfo_Base(const IPv6ControlInfo_Base& other);
    // make assignment operator protected to force the user override it
    IPv6ControlInfo_Base& operator=(const IPv6ControlInfo_Base& other);

  public:
    virtual ~IPv6ControlInfo_Base();
    virtual IPv6ControlInfo_Base *dup() const {throw cRuntimeError("You forgot to manually add a dup() function to class IPv6ControlInfo");}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getProtocol() const;
    virtual void setProtocol(int protocol);
    virtual IPv6Address& getDestAddr();
    virtual const IPv6Address& getDestAddr() const {return const_cast<IPv6ControlInfo_Base*>(this)->getDestAddr();}
    virtual void setDestAddr(const IPv6Address& destAddr);
    virtual IPv6Address& getSrcAddr();
    virtual const IPv6Address& getSrcAddr() const {return const_cast<IPv6ControlInfo_Base*>(this)->getSrcAddr();}
    virtual void setSrcAddr(const IPv6Address& srcAddr);
    virtual unsigned char getTrafficClass() const;
    virtual void setTrafficClass(unsigned char trafficClass);
    virtual int getDiffServCodePoint() const = 0;
    virtual void setDiffServCodePoint(int diffServCodePoint) = 0;
    virtual int getExplicitCongestionNotification() const = 0;
    virtual void setExplicitCongestionNotification(int explicitCongestionNotification) = 0;
    virtual short getHopLimit() const;
    virtual void setHopLimit(short hopLimit);
    virtual int getInterfaceId() const;
    virtual void setInterfaceId(int interfaceId);
    virtual bool getMulticastLoop() const;
    virtual void setMulticastLoop(bool multicastLoop);
    virtual void setExtensionHeaderArraySize(unsigned int size) = 0;
    virtual unsigned int getExtensionHeaderArraySize() const = 0;
    virtual IPv6ExtensionHeaderPtr& getExtensionHeader(unsigned int k) = 0;
    virtual const IPv6ExtensionHeaderPtr& getExtensionHeader(unsigned int k) const {return const_cast<IPv6ControlInfo_Base*>(this)->getExtensionHeader(k);}
    virtual void setExtensionHeader(unsigned int k, const IPv6ExtensionHeaderPtr& extensionHeader) = 0;
};


#endif // ifndef _IPV6CONTROLINFO_M_H_


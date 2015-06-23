//
// Generated file, do not edit! Created by nedtool 4.6 from src/linklayer/contract/IEEE802154e/RLLControlInfo.msg.
//

#ifndef _RLLCONTROLINFO_M_H_
#define _RLLCONTROLINFO_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
    using namespace std;
    #include "Ieee802154eField.h"
    #include "Ieee802154eEnum.h"
// }}

/**
 * Class generated from <tt>src/linklayer/contract/IEEE802154e/RLLControlInfo.msg:23</tt> by nedtool.
 * <pre>
 * message RLLControlInfo
 * {
 *     bool toParent;
 *     string destName;
 * 
 *     uint8_t dstAddrMode;
 *     int64_t dstAddr;
 *     uint8_t srcAddrMode;
 *     int64_t srcAddr;
 * 
 *     bool panCoordinator;
 * 
 *     uint64_t deviceAddress;
 * 
 *     short status @enum(MACenum);
 *     uint16_t channelOffset;
 *     uint16_t assocShortAddress;
 * 
 *     uint8_t beaconType;
 *     uint8_t channel;
 *     uint8_t beaconOrder;
 *     uint8_t superframeOrder;
 * 
 *     uint32_t startTime;
 *     uint8_t scanType;
 * 
 * 
 * 
 * 
 * }
 * </pre>
 */
class RLLControlInfo : public ::cMessage
{
  protected:
    bool toParent_var;
    opp_string destName_var;
    uint8_t dstAddrMode_var;
    int64_t dstAddr_var;
    uint8_t srcAddrMode_var;
    int64_t srcAddr_var;
    bool panCoordinator_var;
    uint64_t deviceAddress_var;
    short status_var;
    uint16_t channelOffset_var;
    uint16_t assocShortAddress_var;
    uint8_t beaconType_var;
    uint8_t channel_var;
    uint8_t beaconOrder_var;
    uint8_t superframeOrder_var;
    uint32_t startTime_var;
    uint8_t scanType_var;

  private:
    void copy(const RLLControlInfo& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const RLLControlInfo&);

  public:
    RLLControlInfo(const char *name=NULL, int kind=0);
    RLLControlInfo(const RLLControlInfo& other);
    virtual ~RLLControlInfo();
    RLLControlInfo& operator=(const RLLControlInfo& other);
    virtual RLLControlInfo *dup() const {return new RLLControlInfo(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual bool getToParent() const;
    virtual void setToParent(bool toParent);
    virtual const char * getDestName() const;
    virtual void setDestName(const char * destName);
    virtual uint8_t getDstAddrMode() const;
    virtual void setDstAddrMode(uint8_t dstAddrMode);
    virtual int64_t getDstAddr() const;
    virtual void setDstAddr(int64_t dstAddr);
    virtual uint8_t getSrcAddrMode() const;
    virtual void setSrcAddrMode(uint8_t srcAddrMode);
    virtual int64_t getSrcAddr() const;
    virtual void setSrcAddr(int64_t srcAddr);
    virtual bool getPanCoordinator() const;
    virtual void setPanCoordinator(bool panCoordinator);
    virtual uint64_t getDeviceAddress() const;
    virtual void setDeviceAddress(uint64_t deviceAddress);
    virtual short getStatus() const;
    virtual void setStatus(short status);
    virtual uint16_t getChannelOffset() const;
    virtual void setChannelOffset(uint16_t channelOffset);
    virtual uint16_t getAssocShortAddress() const;
    virtual void setAssocShortAddress(uint16_t assocShortAddress);
    virtual uint8_t getBeaconType() const;
    virtual void setBeaconType(uint8_t beaconType);
    virtual uint8_t getChannel() const;
    virtual void setChannel(uint8_t channel);
    virtual uint8_t getBeaconOrder() const;
    virtual void setBeaconOrder(uint8_t beaconOrder);
    virtual uint8_t getSuperframeOrder() const;
    virtual void setSuperframeOrder(uint8_t superframeOrder);
    virtual uint32_t getStartTime() const;
    virtual void setStartTime(uint32_t startTime);
    virtual uint8_t getScanType() const;
    virtual void setScanType(uint8_t scanType);
};

inline void doPacking(cCommBuffer *b, RLLControlInfo& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, RLLControlInfo& obj) {obj.parsimUnpack(b);}


#endif // ifndef _RLLCONTROLINFO_M_H_

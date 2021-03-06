//
// Generated file, do not edit! Created by nedtool 4.6 from src/linklayer/contract/PhyControlInfo.msg.
//

#ifndef _PHYCONTROLINFO_M_H_
#define _PHYCONTROLINFO_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
#include "INETDefs.h"
// }}

/**
 * Enum generated from <tt>src/linklayer/contract/PhyControlInfo.msg:31</tt> by nedtool.
 * <pre>
 * //
 * // Command codes for controlling the physical layer (the radio). These constants
 * // should be set as message kind on a message sent to the radio module.
 * //
 * // @see ~PhyControlInfo
 * //
 * enum PhyCommandCode
 * {
 * 
 *     PHY_C_CONFIGURERADIO = 1;
 *     PHY_C_CHANGETRANSMITTERPOWER = 2;
 * }
 * </pre>
 */
enum PhyCommandCode {
    PHY_C_CONFIGURERADIO = 1,
    PHY_C_CHANGETRANSMITTERPOWER = 2
};

/**
 * Class generated from <tt>src/linklayer/contract/PhyControlInfo.msg:40</tt> by nedtool.
 * <pre>
 * //
 * // Control info for controlling the physical layer (the radio).
 * //
 * class PhyControlInfo
 * {
 *     int channelNumber = -1; // with PHY_C_CONFIGURERADIO: the channel to switch to
 *     double bitrate = -1; // with PHY_C_CONFIGURERADIO: the bitrate to switch to
 *     bool adaptiveSensitivity = false;
 *     double transmitterPower = -1; // With PHY_C_CHANGETRANSMITTERPOWER: the transmission power to swith to
 * }
 * </pre>
 */
class PhyControlInfo : public ::cObject
{
  protected:
    int channelNumber_var;
    double bitrate_var;
    bool adaptiveSensitivity_var;
    double transmitterPower_var;

  private:
    void copy(const PhyControlInfo& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const PhyControlInfo&);

  public:
    PhyControlInfo();
    PhyControlInfo(const PhyControlInfo& other);
    virtual ~PhyControlInfo();
    PhyControlInfo& operator=(const PhyControlInfo& other);
    virtual PhyControlInfo *dup() const {return new PhyControlInfo(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getChannelNumber() const;
    virtual void setChannelNumber(int channelNumber);
    virtual double getBitrate() const;
    virtual void setBitrate(double bitrate);
    virtual bool getAdaptiveSensitivity() const;
    virtual void setAdaptiveSensitivity(bool adaptiveSensitivity);
    virtual double getTransmitterPower() const;
    virtual void setTransmitterPower(double transmitterPower);
};

inline void doPacking(cCommBuffer *b, PhyControlInfo& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, PhyControlInfo& obj) {obj.parsimUnpack(b);}

/**
 * Enum generated from <tt>src/linklayer/contract/PhyControlInfo.msg:53</tt> by nedtool.
 * <pre>
 * //
 * // Sent up from the radio as message kind value
 * //
 * // NOTE: make sure it does not conflict with _802_11frameType in Consts80211.h
 * //
 * enum PhyIndication
 * {
 * 
 *     FRAMEOK = 0;
 *     BITERROR = -1; // the phy has recognized a bit error in the packet
 *     COLLISION = 9; // packet lost due to collision
 * }
 * </pre>
 */
enum PhyIndication {
    FRAMEOK = 0,
    BITERROR = -1,
    COLLISION = 9
};


#endif // ifndef _PHYCONTROLINFO_M_H_


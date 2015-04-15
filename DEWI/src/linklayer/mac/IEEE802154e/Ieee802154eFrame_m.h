//
// Generated file, do not edit! Created by nedtool 4.6 from src/linklayer/mac/IEEE802154e/Ieee802154eFrame.msg.
//

#ifndef _IEEE802154EFRAME_M_H_
#define _IEEE802154EFRAME_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
    #include "Ieee802154eConst.h"
    #include "Ieee802154eDef.h"
    #include "Ieee802154eEnum.h"
    #include "Ieee802154eField.h"
    #include "Ieee802154eNetworkCtrlInfo_m.h"
// }}

/**
 * Class generated from <tt>src/linklayer/mac/IEEE802154e/Ieee802154eFrame.msg:50</tt> by nedtool.
 * <pre>
 * packet Ieee802154eBasicFrame
 * {
 *     unsigned char bdsn;
 * 
 *     AuxiliarySecurityHeader auxSecHd;
 * }
 * </pre>
 */
class Ieee802154eBasicFrame : public ::cPacket
{
  protected:
    unsigned char bdsn_var;
    AuxiliarySecurityHeader auxSecHd_var;

  private:
    void copy(const Ieee802154eBasicFrame& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Ieee802154eBasicFrame&);

  public:
    Ieee802154eBasicFrame(const char *name=NULL, int kind=0);
    Ieee802154eBasicFrame(const Ieee802154eBasicFrame& other);
    virtual ~Ieee802154eBasicFrame();
    Ieee802154eBasicFrame& operator=(const Ieee802154eBasicFrame& other);
    virtual Ieee802154eBasicFrame *dup() const {return new Ieee802154eBasicFrame(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual unsigned char getBdsn() const;
    virtual void setBdsn(unsigned char bdsn);
    virtual AuxiliarySecurityHeader& getAuxSecHd();
    virtual const AuxiliarySecurityHeader& getAuxSecHd() const {return const_cast<Ieee802154eBasicFrame*>(this)->getAuxSecHd();}
    virtual void setAuxSecHd(const AuxiliarySecurityHeader& auxSecHd);
};

inline void doPacking(cCommBuffer *b, Ieee802154eBasicFrame& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, Ieee802154eBasicFrame& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>src/linklayer/mac/IEEE802154e/Ieee802154eFrame.msg:57</tt> by nedtool.
 * <pre>
 * message Ieee802154eFrame extends Ieee802154eBasicFrame
 * {
 *     //	MHR
 *     //	Frame control field (FCF)(16 bits) - Fig 35
 *     FrameCtrl frmCtrl;
 * 	    //	Sequence number field (8 bits)
 *     unsigned char seqNmbr;
 *     //	Addressing fields
 *     uint16_t dstPanId;
 *     MACAddress dstAddr;
 *     uint16_t srcPanId;
 *     MACAddress srcAddr;
 * 
 *     uint8_t retries;
 *     IE_Header ieHeader[];
 *     IE_Payload iePayload[];
 * }
 * </pre>
 */
class Ieee802154eFrame : public ::Ieee802154eBasicFrame
{
  protected:
    FrameCtrl frmCtrl_var;
    unsigned char seqNmbr_var;
    uint16_t dstPanId_var;
    MACAddress dstAddr_var;
    uint16_t srcPanId_var;
    MACAddress srcAddr_var;
    uint8_t retries_var;
    IE_Header *ieHeader_var; // array ptr
    unsigned int ieHeader_arraysize;
    IE_Payload *iePayload_var; // array ptr
    unsigned int iePayload_arraysize;

  private:
    void copy(const Ieee802154eFrame& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Ieee802154eFrame&);

  public:
    Ieee802154eFrame(const char *name=NULL, int kind=0);
    Ieee802154eFrame(const Ieee802154eFrame& other);
    virtual ~Ieee802154eFrame();
    Ieee802154eFrame& operator=(const Ieee802154eFrame& other);
    virtual Ieee802154eFrame *dup() const {return new Ieee802154eFrame(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual FrameCtrl& getFrmCtrl();
    virtual const FrameCtrl& getFrmCtrl() const {return const_cast<Ieee802154eFrame*>(this)->getFrmCtrl();}
    virtual void setFrmCtrl(const FrameCtrl& frmCtrl);
    virtual unsigned char getSeqNmbr() const;
    virtual void setSeqNmbr(unsigned char seqNmbr);
    virtual uint16_t getDstPanId() const;
    virtual void setDstPanId(uint16_t dstPanId);
    virtual MACAddress& getDstAddr();
    virtual const MACAddress& getDstAddr() const {return const_cast<Ieee802154eFrame*>(this)->getDstAddr();}
    virtual void setDstAddr(const MACAddress& dstAddr);
    virtual uint16_t getSrcPanId() const;
    virtual void setSrcPanId(uint16_t srcPanId);
    virtual MACAddress& getSrcAddr();
    virtual const MACAddress& getSrcAddr() const {return const_cast<Ieee802154eFrame*>(this)->getSrcAddr();}
    virtual void setSrcAddr(const MACAddress& srcAddr);
    virtual uint8_t getRetries() const;
    virtual void setRetries(uint8_t retries);
    virtual void setIeHeaderArraySize(unsigned int size);
    virtual unsigned int getIeHeaderArraySize() const;
    virtual IE_Header& getIeHeader(unsigned int k);
    virtual const IE_Header& getIeHeader(unsigned int k) const {return const_cast<Ieee802154eFrame*>(this)->getIeHeader(k);}
    virtual void setIeHeader(unsigned int k, const IE_Header& ieHeader);
    virtual void setIePayloadArraySize(unsigned int size);
    virtual unsigned int getIePayloadArraySize() const;
    virtual IE_Payload& getIePayload(unsigned int k);
    virtual const IE_Payload& getIePayload(unsigned int k) const {return const_cast<Ieee802154eFrame*>(this)->getIePayload(k);}
    virtual void setIePayload(unsigned int k, const IE_Payload& iePayload);
};

inline void doPacking(cCommBuffer *b, Ieee802154eFrame& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, Ieee802154eFrame& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>src/linklayer/mac/IEEE802154e/Ieee802154eFrame.msg:75</tt> by nedtool.
 * <pre>
 * message Ieee802154eBeaconFrame extends Ieee802154eFrame
 * {
 *     // MAC payload in beacon frames
 *     SuperframeSpec sfSpec;		// Superframe Specification
 * 
 *     // GTS information fields
 *     GTSFields gtsField;
 * 
 *     // Pending Address Information field
 *     PendingAddrFields pendingAdrrFld;
 * }
 * </pre>
 */
class Ieee802154eBeaconFrame : public ::Ieee802154eFrame
{
  protected:
    SuperframeSpec sfSpec_var;
    GTSFields gtsField_var;
    PendingAddrFields pendingAdrrFld_var;

  private:
    void copy(const Ieee802154eBeaconFrame& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Ieee802154eBeaconFrame&);

  public:
    Ieee802154eBeaconFrame(const char *name=NULL, int kind=0);
    Ieee802154eBeaconFrame(const Ieee802154eBeaconFrame& other);
    virtual ~Ieee802154eBeaconFrame();
    Ieee802154eBeaconFrame& operator=(const Ieee802154eBeaconFrame& other);
    virtual Ieee802154eBeaconFrame *dup() const {return new Ieee802154eBeaconFrame(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual SuperframeSpec& getSfSpec();
    virtual const SuperframeSpec& getSfSpec() const {return const_cast<Ieee802154eBeaconFrame*>(this)->getSfSpec();}
    virtual void setSfSpec(const SuperframeSpec& sfSpec);
    virtual GTSFields& getGtsField();
    virtual const GTSFields& getGtsField() const {return const_cast<Ieee802154eBeaconFrame*>(this)->getGtsField();}
    virtual void setGtsField(const GTSFields& gtsField);
    virtual PendingAddrFields& getPendingAdrrFld();
    virtual const PendingAddrFields& getPendingAdrrFld() const {return const_cast<Ieee802154eBeaconFrame*>(this)->getPendingAdrrFld();}
    virtual void setPendingAdrrFld(const PendingAddrFields& pendingAdrrFld);
};

inline void doPacking(cCommBuffer *b, Ieee802154eBeaconFrame& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, Ieee802154eBeaconFrame& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>src/linklayer/mac/IEEE802154e/Ieee802154eFrame.msg:90</tt> by nedtool.
 * <pre>
 * // *****************************************
 * // Enhanced Beacon frame format - Std 802.15.4e-2012 (figure 40a) page 64
 * // *****************************************
 * message Ieee802154EnhancedBeaconFrame extends Ieee802154eFrame
 * {
 *     int BO;
 *     int SO;
 *     int stage;
 * }
 * </pre>
 */
class Ieee802154EnhancedBeaconFrame : public ::Ieee802154eFrame
{
  protected:
    int BO_var;
    int SO_var;
    int stage_var;

  private:
    void copy(const Ieee802154EnhancedBeaconFrame& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Ieee802154EnhancedBeaconFrame&);

  public:
    Ieee802154EnhancedBeaconFrame(const char *name=NULL, int kind=0);
    Ieee802154EnhancedBeaconFrame(const Ieee802154EnhancedBeaconFrame& other);
    virtual ~Ieee802154EnhancedBeaconFrame();
    Ieee802154EnhancedBeaconFrame& operator=(const Ieee802154EnhancedBeaconFrame& other);
    virtual Ieee802154EnhancedBeaconFrame *dup() const {return new Ieee802154EnhancedBeaconFrame(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getBO() const;
    virtual void setBO(int BO);
    virtual int getSO() const;
    virtual void setSO(int SO);
    virtual int getStage() const;
    virtual void setStage(int stage);
};

inline void doPacking(cCommBuffer *b, Ieee802154EnhancedBeaconFrame& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, Ieee802154EnhancedBeaconFrame& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>src/linklayer/mac/IEEE802154e/Ieee802154eFrame.msg:100</tt> by nedtool.
 * <pre>
 * // ********************************
 * // Data frame format - Std 802.15.4e-2012 (figure 46) page 65
 * // ********************************
 * message Ieee802154eDataFrame extends Ieee802154eFrame
 * {
 * }
 * </pre>
 */
class Ieee802154eDataFrame : public ::Ieee802154eFrame
{
  protected:

  private:
    void copy(const Ieee802154eDataFrame& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Ieee802154eDataFrame&);

  public:
    Ieee802154eDataFrame(const char *name=NULL, int kind=0);
    Ieee802154eDataFrame(const Ieee802154eDataFrame& other);
    virtual ~Ieee802154eDataFrame();
    Ieee802154eDataFrame& operator=(const Ieee802154eDataFrame& other);
    virtual Ieee802154eDataFrame *dup() const {return new Ieee802154eDataFrame(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
};

inline void doPacking(cCommBuffer *b, Ieee802154eDataFrame& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, Ieee802154eDataFrame& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>src/linklayer/mac/IEEE802154e/Ieee802154eFrame.msg:107</tt> by nedtool.
 * <pre>
 * // ******************************		
 * // Acknowledgment frame format - Std 802.15.4-2011 (figure 47) page 65
 * // ******************************
 * message Ieee802154eAckFrame extends Ieee802154eFrame
 * {
 * }
 * </pre>
 */
class Ieee802154eAckFrame : public ::Ieee802154eFrame
{
  protected:

  private:
    void copy(const Ieee802154eAckFrame& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Ieee802154eAckFrame&);

  public:
    Ieee802154eAckFrame(const char *name=NULL, int kind=0);
    Ieee802154eAckFrame(const Ieee802154eAckFrame& other);
    virtual ~Ieee802154eAckFrame();
    Ieee802154eAckFrame& operator=(const Ieee802154eAckFrame& other);
    virtual Ieee802154eAckFrame *dup() const {return new Ieee802154eAckFrame(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
};

inline void doPacking(cCommBuffer *b, Ieee802154eAckFrame& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, Ieee802154eAckFrame& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>src/linklayer/mac/IEEE802154e/Ieee802154eFrame.msg:114</tt> by nedtool.
 * <pre>
 * // ******************************		
 * // Enhanced Acknowledgement frame format - Std 802.15.4e-2012 (figure 47a) page 66
 * // ******************************
 * message Ieee802154eEnhancedAckFrame extends Ieee802154eFrame
 * {
 * }
 * </pre>
 */
class Ieee802154eEnhancedAckFrame : public ::Ieee802154eFrame
{
  protected:

  private:
    void copy(const Ieee802154eEnhancedAckFrame& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Ieee802154eEnhancedAckFrame&);

  public:
    Ieee802154eEnhancedAckFrame(const char *name=NULL, int kind=0);
    Ieee802154eEnhancedAckFrame(const Ieee802154eEnhancedAckFrame& other);
    virtual ~Ieee802154eEnhancedAckFrame();
    Ieee802154eEnhancedAckFrame& operator=(const Ieee802154eEnhancedAckFrame& other);
    virtual Ieee802154eEnhancedAckFrame *dup() const {return new Ieee802154eEnhancedAckFrame(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
};

inline void doPacking(cCommBuffer *b, Ieee802154eEnhancedAckFrame& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, Ieee802154eEnhancedAckFrame& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>src/linklayer/mac/IEEE802154e/Ieee802154eFrame.msg:118</tt> by nedtool.
 * <pre>
 * message Ieee802154eAssociationFrame extends Ieee802154eFrame
 * {
 *     Ieee802154eNetworkCtrlInfo CntrlInfo;
 * 
 *     int CoorStage;
 *     bool CS;
 *     bool CH;
 * }
 * </pre>
 */
class Ieee802154eAssociationFrame : public ::Ieee802154eFrame
{
  protected:
    Ieee802154eNetworkCtrlInfo CntrlInfo_var;
    int CoorStage_var;
    bool CS_var;
    bool CH_var;

  private:
    void copy(const Ieee802154eAssociationFrame& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Ieee802154eAssociationFrame&);

  public:
    Ieee802154eAssociationFrame(const char *name=NULL, int kind=0);
    Ieee802154eAssociationFrame(const Ieee802154eAssociationFrame& other);
    virtual ~Ieee802154eAssociationFrame();
    Ieee802154eAssociationFrame& operator=(const Ieee802154eAssociationFrame& other);
    virtual Ieee802154eAssociationFrame *dup() const {return new Ieee802154eAssociationFrame(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual Ieee802154eNetworkCtrlInfo& getCntrlInfo();
    virtual const Ieee802154eNetworkCtrlInfo& getCntrlInfo() const {return const_cast<Ieee802154eAssociationFrame*>(this)->getCntrlInfo();}
    virtual void setCntrlInfo(const Ieee802154eNetworkCtrlInfo& CntrlInfo);
    virtual int getCoorStage() const;
    virtual void setCoorStage(int CoorStage);
    virtual bool getCS() const;
    virtual void setCS(bool CS);
    virtual bool getCH() const;
    virtual void setCH(bool CH);
};

inline void doPacking(cCommBuffer *b, Ieee802154eAssociationFrame& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, Ieee802154eAssociationFrame& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>src/linklayer/mac/IEEE802154e/Ieee802154eFrame.msg:127</tt> by nedtool.
 * <pre>
 * message Ieee802154eDisassociationFrame extends Ieee802154eFrame
 * {
 *     Ieee802154eNetworkCtrlInfo CntrlInfo;
 * }
 * </pre>
 */
class Ieee802154eDisassociationFrame : public ::Ieee802154eFrame
{
  protected:
    Ieee802154eNetworkCtrlInfo CntrlInfo_var;

  private:
    void copy(const Ieee802154eDisassociationFrame& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Ieee802154eDisassociationFrame&);

  public:
    Ieee802154eDisassociationFrame(const char *name=NULL, int kind=0);
    Ieee802154eDisassociationFrame(const Ieee802154eDisassociationFrame& other);
    virtual ~Ieee802154eDisassociationFrame();
    Ieee802154eDisassociationFrame& operator=(const Ieee802154eDisassociationFrame& other);
    virtual Ieee802154eDisassociationFrame *dup() const {return new Ieee802154eDisassociationFrame(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual Ieee802154eNetworkCtrlInfo& getCntrlInfo();
    virtual const Ieee802154eNetworkCtrlInfo& getCntrlInfo() const {return const_cast<Ieee802154eDisassociationFrame*>(this)->getCntrlInfo();}
    virtual void setCntrlInfo(const Ieee802154eNetworkCtrlInfo& CntrlInfo);
};

inline void doPacking(cCommBuffer *b, Ieee802154eDisassociationFrame& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, Ieee802154eDisassociationFrame& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>src/linklayer/mac/IEEE802154e/Ieee802154eFrame.msg:132</tt> by nedtool.
 * <pre>
 * message Ieee802154eScheduleFrame extends Ieee802154eFrame
 * {
 *     Ieee802154eNetworkCtrlInfo CntrlInfo;
 * }
 * </pre>
 */
class Ieee802154eScheduleFrame : public ::Ieee802154eFrame
{
  protected:
    Ieee802154eNetworkCtrlInfo CntrlInfo_var;

  private:
    void copy(const Ieee802154eScheduleFrame& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Ieee802154eScheduleFrame&);

  public:
    Ieee802154eScheduleFrame(const char *name=NULL, int kind=0);
    Ieee802154eScheduleFrame(const Ieee802154eScheduleFrame& other);
    virtual ~Ieee802154eScheduleFrame();
    Ieee802154eScheduleFrame& operator=(const Ieee802154eScheduleFrame& other);
    virtual Ieee802154eScheduleFrame *dup() const {return new Ieee802154eScheduleFrame(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual Ieee802154eNetworkCtrlInfo& getCntrlInfo();
    virtual const Ieee802154eNetworkCtrlInfo& getCntrlInfo() const {return const_cast<Ieee802154eScheduleFrame*>(this)->getCntrlInfo();}
    virtual void setCntrlInfo(const Ieee802154eNetworkCtrlInfo& CntrlInfo);
};

inline void doPacking(cCommBuffer *b, Ieee802154eScheduleFrame& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, Ieee802154eScheduleFrame& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>src/linklayer/mac/IEEE802154e/Ieee802154eFrame.msg:138</tt> by nedtool.
 * <pre>
 * message Ieee802154eMulHoCluFrame extends Ieee802154eFrame
 * {
 *     Ieee802154eNetworkCtrlInfo CntrlInfo;
 * 
 *     int CoorStage;
 *     bool CS;
 *     bool CH;
 * 
 *     int ChannelOffset;
 *     int timeslot;
 *     int BO;
 *     int SO;
 * 
 * }
 * </pre>
 */
class Ieee802154eMulHoCluFrame : public ::Ieee802154eFrame
{
  protected:
    Ieee802154eNetworkCtrlInfo CntrlInfo_var;
    int CoorStage_var;
    bool CS_var;
    bool CH_var;
    int ChannelOffset_var;
    int timeslot_var;
    int BO_var;
    int SO_var;

  private:
    void copy(const Ieee802154eMulHoCluFrame& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Ieee802154eMulHoCluFrame&);

  public:
    Ieee802154eMulHoCluFrame(const char *name=NULL, int kind=0);
    Ieee802154eMulHoCluFrame(const Ieee802154eMulHoCluFrame& other);
    virtual ~Ieee802154eMulHoCluFrame();
    Ieee802154eMulHoCluFrame& operator=(const Ieee802154eMulHoCluFrame& other);
    virtual Ieee802154eMulHoCluFrame *dup() const {return new Ieee802154eMulHoCluFrame(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual Ieee802154eNetworkCtrlInfo& getCntrlInfo();
    virtual const Ieee802154eNetworkCtrlInfo& getCntrlInfo() const {return const_cast<Ieee802154eMulHoCluFrame*>(this)->getCntrlInfo();}
    virtual void setCntrlInfo(const Ieee802154eNetworkCtrlInfo& CntrlInfo);
    virtual int getCoorStage() const;
    virtual void setCoorStage(int CoorStage);
    virtual bool getCS() const;
    virtual void setCS(bool CS);
    virtual bool getCH() const;
    virtual void setCH(bool CH);
    virtual int getChannelOffset() const;
    virtual void setChannelOffset(int ChannelOffset);
    virtual int getTimeslot() const;
    virtual void setTimeslot(int timeslot);
    virtual int getBO() const;
    virtual void setBO(int BO);
    virtual int getSO() const;
    virtual void setSO(int SO);
};

inline void doPacking(cCommBuffer *b, Ieee802154eMulHoCluFrame& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, Ieee802154eMulHoCluFrame& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>src/linklayer/mac/IEEE802154e/Ieee802154eFrame.msg:156</tt> by nedtool.
 * <pre>
 * // ********************************				
 * // MAC command frame format - Std 802.15.4e-2012 (figure 48) page 67
 * // ********************************
 * message Ieee802154eCmdFrame extends Ieee802154eFrame
 * {
 *     // MAC Payload
 *     int cmdType @enum(Ieee802154eMacCmdType);		// Commend Frame Identifier
 * 
 *     // Command Payload (variable) 
 * }
 * </pre>
 */
class Ieee802154eCmdFrame : public ::Ieee802154eFrame
{
  protected:
    int cmdType_var;

  private:
    void copy(const Ieee802154eCmdFrame& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Ieee802154eCmdFrame&);

  public:
    Ieee802154eCmdFrame(const char *name=NULL, int kind=0);
    Ieee802154eCmdFrame(const Ieee802154eCmdFrame& other);
    virtual ~Ieee802154eCmdFrame();
    Ieee802154eCmdFrame& operator=(const Ieee802154eCmdFrame& other);
    virtual Ieee802154eCmdFrame *dup() const {return new Ieee802154eCmdFrame(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getCmdType() const;
    virtual void setCmdType(int cmdType);
};

inline void doPacking(cCommBuffer *b, Ieee802154eCmdFrame& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, Ieee802154eCmdFrame& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>src/linklayer/mac/IEEE802154e/Ieee802154eFrame.msg:164</tt> by nedtool.
 * <pre>
 * packet Ieee802154eMPFrame extends Ieee802154eFrame
 * {
 *     //	MHR
 *     //	Frame control field (FCF)(16 bits) (figure 48k)
 *     MPFrameCtrl mpFrmCtrl;
 * 
 * 	//	MFR
 * 	//	FCS is not modeled and only included in the length (2 bytes)
 * }
 * </pre>
 */
class Ieee802154eMPFrame : public ::Ieee802154eFrame
{
  protected:
    MPFrameCtrl mpFrmCtrl_var;

  private:
    void copy(const Ieee802154eMPFrame& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Ieee802154eMPFrame&);

  public:
    Ieee802154eMPFrame(const char *name=NULL, int kind=0);
    Ieee802154eMPFrame(const Ieee802154eMPFrame& other);
    virtual ~Ieee802154eMPFrame();
    Ieee802154eMPFrame& operator=(const Ieee802154eMPFrame& other);
    virtual Ieee802154eMPFrame *dup() const {return new Ieee802154eMPFrame(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual MPFrameCtrl& getMpFrmCtrl();
    virtual const MPFrameCtrl& getMpFrmCtrl() const {return const_cast<Ieee802154eMPFrame*>(this)->getMpFrmCtrl();}
    virtual void setMpFrmCtrl(const MPFrameCtrl& mpFrmCtrl);
};

inline void doPacking(cCommBuffer *b, Ieee802154eMPFrame& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, Ieee802154eMPFrame& obj) {obj.parsimUnpack(b);}


#endif // ifndef _IEEE802154EFRAME_M_H_


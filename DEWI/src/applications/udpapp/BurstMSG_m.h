//
// Generated file, do not edit! Created by nedtool 4.6 from src/applications/udpapp/BurstMSG.msg.
//

#ifndef _BURSTMSG_M_H_
#define _BURSTMSG_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>src/applications/udpapp/BurstMSG.msg:19</tt> by nedtool.
 * <pre>
 * //
 * // TODO generated message class
 * //
 * packet BurstMSG
 * {
 *     int hopCount;
 * }
 * </pre>
 */
class BurstMSG : public ::cPacket
{
  protected:
    int hopCount_var;

  private:
    void copy(const BurstMSG& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const BurstMSG&);

  public:
    BurstMSG(const char *name=NULL, int kind=0);
    BurstMSG(const BurstMSG& other);
    virtual ~BurstMSG();
    BurstMSG& operator=(const BurstMSG& other);
    virtual BurstMSG *dup() const {return new BurstMSG(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getHopCount() const;
    virtual void setHopCount(int hopCount);
};

inline void doPacking(cCommBuffer *b, BurstMSG& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, BurstMSG& obj) {obj.parsimUnpack(b);}


#endif // ifndef _BURSTMSG_M_H_


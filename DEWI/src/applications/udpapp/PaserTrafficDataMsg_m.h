//
// Generated file, do not edit! Created by nedtool 4.6 from src/applications/udpapp/PaserTrafficDataMsg.msg.
//

#ifndef _PASERTRAFFICDATAMSG_M_H_
#define _PASERTRAFFICDATAMSG_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>src/applications/udpapp/PaserTrafficDataMsg.msg:19</tt> by nedtool.
 * <pre>
 * //
 * // TODO generated message class
 * //
 * packet PaserTrafficDataMsg
 * {
 *     string srcId;
 *     int counter;
 *     int hops;
 *     double timestamp;
 * }
 * </pre>
 */
class PaserTrafficDataMsg : public ::cPacket
{
  protected:
    opp_string srcId_var;
    int counter_var;
    int hops_var;
    double timestamp_var;

  private:
    void copy(const PaserTrafficDataMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const PaserTrafficDataMsg&);

  public:
    PaserTrafficDataMsg(const char *name=NULL, int kind=0);
    PaserTrafficDataMsg(const PaserTrafficDataMsg& other);
    virtual ~PaserTrafficDataMsg();
    PaserTrafficDataMsg& operator=(const PaserTrafficDataMsg& other);
    virtual PaserTrafficDataMsg *dup() const {return new PaserTrafficDataMsg(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual const char * getSrcId() const;
    virtual void setSrcId(const char * srcId);
    virtual int getCounter() const;
    virtual void setCounter(int counter);
    virtual int getHops() const;
    virtual void setHops(int hops);
    virtual double getTimestamp() const;
    virtual void setTimestamp(double timestamp);
};

inline void doPacking(cCommBuffer *b, PaserTrafficDataMsg& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, PaserTrafficDataMsg& obj) {obj.parsimUnpack(b);}


#endif // ifndef _PASERTRAFFICDATAMSG_M_H_


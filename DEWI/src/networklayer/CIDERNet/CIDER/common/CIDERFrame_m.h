//
// Generated file, do not edit! Created by nedtool 4.6 from src/networklayer/CIDERNet/CIDER/common/CIDERFrame.msg.
//

#ifndef _CIDERFRAME_M_H_
#define _CIDERFRAME_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
    using namespace std;
// }}

/**
 * Class generated from <tt>src/networklayer/CIDERNet/CIDER/common/CIDERFrame.msg:24</tt> by nedtool.
 * <pre>
 * message CIDERFrame
 * {
 * }
 * </pre>
 */
class CIDERFrame : public ::cMessage
{
  protected:

  private:
    void copy(const CIDERFrame& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const CIDERFrame&);

  public:
    CIDERFrame(const char *name=NULL, int kind=0);
    CIDERFrame(const CIDERFrame& other);
    virtual ~CIDERFrame();
    CIDERFrame& operator=(const CIDERFrame& other);
    virtual CIDERFrame *dup() const {return new CIDERFrame(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
};

inline void doPacking(cCommBuffer *b, CIDERFrame& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, CIDERFrame& obj) {obj.parsimUnpack(b);}


#endif // ifndef _CIDERFRAME_M_H_


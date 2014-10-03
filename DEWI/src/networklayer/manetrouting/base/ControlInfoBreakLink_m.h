//
// Generated file, do not edit! Created by opp_msgc 4.4 from src/networklayer/manetrouting/base/ControlInfoBreakLink.msg.
//

#ifndef _CONTROLINFOBREAKLINK_M_H_
#define _CONTROLINFOBREAKLINK_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0404
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif

// cplusplus {{
#include "MACAddress.h"
// }}



/**
 * Class generated from <tt>src/networklayer/manetrouting/base/ControlInfoBreakLink.msg</tt> by opp_msgc.
 * <pre>
 * class ControlInfoBreakLink
 * {
 *       MACAddress dest;   
 * }
 * </pre>
 */
class ControlInfoBreakLink : public ::cObject
{
  protected:
    MACAddress dest_var;

  private:
    void copy(const ControlInfoBreakLink& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const ControlInfoBreakLink&);

  public:
    ControlInfoBreakLink();
    ControlInfoBreakLink(const ControlInfoBreakLink& other);
    virtual ~ControlInfoBreakLink();
    ControlInfoBreakLink& operator=(const ControlInfoBreakLink& other);
    virtual ControlInfoBreakLink *dup() const {return new ControlInfoBreakLink(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual MACAddress& getDest();
    virtual const MACAddress& getDest() const {return const_cast<ControlInfoBreakLink*>(this)->getDest();}
    virtual void setDest(const MACAddress& dest);
};

inline void doPacking(cCommBuffer *b, ControlInfoBreakLink& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, ControlInfoBreakLink& obj) {obj.parsimUnpack(b);}


#endif // _CONTROLINFOBREAKLINK_M_H_

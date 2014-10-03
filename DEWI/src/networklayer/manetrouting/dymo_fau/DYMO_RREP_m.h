//
// Generated file, do not edit! Created by opp_msgc 4.4 from src/networklayer/manetrouting/dymo_fau/DYMO_RREP.msg.
//

#ifndef _DYMO_RREP_M_H_
#define _DYMO_RREP_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0404
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif

// cplusplus {{
#include "DYMO_RM_m.h"
// }}



/**
 * Class generated from <tt>src/networklayer/manetrouting/dymo_fau/DYMO_RREP.msg</tt> by opp_msgc.
 * <pre>
 * message DYMO_RREP extends DYMO_RM
 * {
 * }
 * </pre>
 */
class DYMO_RREP : public ::DYMO_RM
{
  protected:

  private:
    void copy(const DYMO_RREP& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const DYMO_RREP&);

  public:
    DYMO_RREP(const char *name=NULL, int kind=0);
    DYMO_RREP(const DYMO_RREP& other);
    virtual ~DYMO_RREP();
    DYMO_RREP& operator=(const DYMO_RREP& other);
    virtual DYMO_RREP *dup() const {return new DYMO_RREP(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
};

inline void doPacking(cCommBuffer *b, DYMO_RREP& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, DYMO_RREP& obj) {obj.parsimUnpack(b);}


#endif // _DYMO_RREP_M_H_

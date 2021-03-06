//
// Generated file, do not edit! Created by nedtool 4.6 from src/applications/udpapp/UDPEchoAppMsg.msg.
//

#ifndef _UDPECHOAPPMSG_M_H_
#define _UDPECHOAPPMSG_M_H_

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
 * Class generated from <tt>src/applications/udpapp/UDPEchoAppMsg.msg:25</tt> by nedtool.
 * <pre>
 * //
 * // Message class used by ~UDPEchoApp.
 * //
 * packet UDPEchoAppMsg
 * {
 *     bool isRequest = true;
 * }
 * </pre>
 */
class UDPEchoAppMsg : public ::cPacket
{
  protected:
    bool isRequest_var;

  private:
    void copy(const UDPEchoAppMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const UDPEchoAppMsg&);

  public:
    UDPEchoAppMsg(const char *name=NULL, int kind=0);
    UDPEchoAppMsg(const UDPEchoAppMsg& other);
    virtual ~UDPEchoAppMsg();
    UDPEchoAppMsg& operator=(const UDPEchoAppMsg& other);
    virtual UDPEchoAppMsg *dup() const {return new UDPEchoAppMsg(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual bool getIsRequest() const;
    virtual void setIsRequest(bool isRequest);
};

inline void doPacking(cCommBuffer *b, UDPEchoAppMsg& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, UDPEchoAppMsg& obj) {obj.parsimUnpack(b);}


#endif // ifndef _UDPECHOAPPMSG_M_H_


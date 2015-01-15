//
// Generated file, do not edit! Created by nedtool 4.6 from src/applications/udpapp/VideoPacket.msg.
//

#ifndef _VIDEOPACKET_M_H_
#define _VIDEOPACKET_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>src/applications/udpapp/VideoPacket.msg:19</tt> by nedtool.
 * <pre>
 * //
 * // TODO generated message class
 * //
 * packet VideoPacket
 * {
 *     unsigned int seqNum;
 *     char type;
 *     unsigned long frameSize;
 * }
 * </pre>
 */
class VideoPacket : public ::cPacket
{
  protected:
    unsigned int seqNum_var;
    char type_var;
    unsigned long frameSize_var;

  private:
    void copy(const VideoPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const VideoPacket&);

  public:
    VideoPacket(const char *name=NULL, int kind=0);
    VideoPacket(const VideoPacket& other);
    virtual ~VideoPacket();
    VideoPacket& operator=(const VideoPacket& other);
    virtual VideoPacket *dup() const {return new VideoPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual unsigned int getSeqNum() const;
    virtual void setSeqNum(unsigned int seqNum);
    virtual char getType() const;
    virtual void setType(char type);
    virtual unsigned long getFrameSize() const;
    virtual void setFrameSize(unsigned long frameSize);
};

inline void doPacking(cCommBuffer *b, VideoPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, VideoPacket& obj) {obj.parsimUnpack(b);}


#endif // ifndef _VIDEOPACKET_M_H_


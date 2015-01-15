/*
 * ccsma.h
 *
 *  Created on: Jan 15, 2015
 *      Author: user
 */

#ifndef CCSMA_H_
#define CCSMA_H_

#include <csma802154.h>
#include "IPvXAddress.h"

class ccsma: public csma802154 {
public:
    ccsma();
    virtual ~ccsma();
    virtual void handleLowerMsg(cPacket*);
    virtual void handleSelfMsg(cMessage *msg);
    void sendDown(Ieee802154Frame* frame);

protected:
    typedef std::map<int,int> SourceSequence;
    SourceSequence ssSourceSequence;
    IPvXAddress ipDestAddr;
};

#endif /* CCSMA_H_ */

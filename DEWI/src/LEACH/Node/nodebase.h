#ifndef NODEBASE_H
#define NODEBASE_H

#include <omnetpp.h>
#include "leach.h"

/**
 * Base class for Node and BS.
 */
class NodeBase : public cSimpleModule
{


  protected:
    virtual void initialize() = 0;
    virtual void handleMessage(cMessage * msg) = 0;
    virtual void finish();
    void updateStats();
    void initStats();

  public:  
    cArray nHosts;              //  IT neighbouring hosts
    int myId;

    int myStatus;
    // stats
    int nrMsg;
    

    // how many gates
    int nrGates;
    int nrNodes;
};

extern void updateStats();

#endif


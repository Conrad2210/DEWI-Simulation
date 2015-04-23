/*
 * RLL.cc
 *
 *  Created on: 23 Apr 2015
 *      Author: R00118979
 */

#ifndef RLL_CC_
#define RLL_CC_
#include "RLL.h"

Define_Module(RLL);

RLL::RLL()
{

}

RLL::~RLL()
{

}

void RLL::initialize(int stage)
{
    cSimpleModule::initialize(stage);
    if(stage==4)
    {
	registerRoutingModule();
	linkLayerFeeback();
    }
}
void RLL::finish()
{
}

/** @brief Function called whenever a message arrives at the module */
void RLL::handleMessage(cMessage * msg)
{
    if(msg->isSelfMessage())
    {

    }
}

bool RLL::supportGetRoute()
{
    return false;
}

uint32_t RLL::getRoute(const ManetAddress& dest, std::vector<ManetAddress>& hopsList)
{
    return 0;
}
bool RLL::getNextHop(const ManetAddress& dest, ManetAddress& nextHop, int& ifaceId, double& cost)
{
    return false;
}
void RLL::setRefreshRoute(const ManetAddress& dest, const ManetAddress& nextHop, bool isReverse)
{
}
bool RLL::isProactive()
{
    return false;
}
bool RLL::isOurType(cPacket *pk)
{
    return false;
}
bool RLL::getDestAddress(cPacket *pk, ManetAddress &dest)
{
    return false;
}

#endif /* RLL_CC_ */

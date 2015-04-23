/*
 * RLL.h
 *
 *  Created on: 23 Apr 2015
 *      Author: R00118979
 */

#ifndef RLL_H_
#define RLL_H_

#include "INETDefs.h"

#include "IPv4Address.h"
#include "IPv4.h"
#include "IInterfaceTable.h"
#include "InterfaceTableAccess.h"
#include "IRoutingTable.h"
#include "RoutingTableAccess.h"
#include "Ieee802Ctrl_m.h"
#include "ICMPMessage.h"
#include "IPv4Datagram.h"

#include "ManetRoutingBase.h"

class RLL : public ManetRoutingBase
{
    public:
	RLL();
	~RLL();
    private:
	int numInitStages() const
	{
	    return 5;
	}
	void initialize(int);
	void finish();

	/** @brief Function called whenever a message arrives at the module */
	void handleMessage(cMessage * msg);

    protected:
	bool supportGetRoute();

	uint32_t getRoute(const ManetAddress& dest, std::vector<ManetAddress>& hopsList);
	bool getNextHop(const ManetAddress& dest, ManetAddress& nextHop, int& ifaceId, double& cost);
	void setRefreshRoute(const ManetAddress& dest, const ManetAddress& nextHop, bool isReverse);
	bool isProactive();
	bool isOurType(cPacket *pk);
	bool getDestAddress(cPacket *pk, ManetAddress &dest);
};

#endif /* RLL_H_ */

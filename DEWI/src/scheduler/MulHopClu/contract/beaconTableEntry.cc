/*
 * beaconTableEntry.cpp
 *
 *  Created on: 4 Mar 2015
 *      Author: R00118979
 */

#include <contract/beaconTableEntry.h>

beaconTableEntry::beaconTableEntry()
{
	id = -1;
	bcnMsg = NULL;
	srcAddrExtended = MACAddress::BROADCAST_ADDRESS;
	srcAddrShort = -1;
	recvPower = -1;
	rssi = -1;
	srcPanId = -1;
	receivedAt = -1;

}

beaconTableEntry::beaconTableEntry(int Eid, Ieee802154EnhancedBeaconFrame *msg, MACAddress extAddr, UINT16 shrtAdd, double rxPw, double r, UINT16 srcid, SimTime timestamp)
{
	id = Eid;
	bcnMsg = msg;
	srcAddrExtended = extAddr;
	srcAddrShort = shrtAdd;
	recvPower = rxPw;
	rssi = r;
	srcPanId = srcid;
	receivedAt = timestamp;

}

beaconTableEntry::~beaconTableEntry()
{
    // TODO Auto-generated destructor stub
}


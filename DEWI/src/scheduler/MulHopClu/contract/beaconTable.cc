/*
 * beaconTable.cpp
 *
 *  Created on: 4 Mar 2015
 *      Author: R00118979
 */

#include <contract/beaconTable.h>

beaconTable::beaconTable()
{
    // TODO Auto-generated constructor stub

}

beaconTable::~beaconTable()
{
    // TODO Auto-generated destructor stub
}

bool beaconTable::addEntry(Ieee802154EnhancedBeaconFrame *msg, MACAddress extAddr, UINT16 shrtAdd, double rxPw, double r, UINT16 srcid, SimTime timestamp)
{
    entryTable.push_back(new beaconTableEntry(entryTable.size(), msg, extAddr, shrtAdd, rxPw, r, srcid, timestamp));
    return true;
}

bool beaconTable::existBeaconEntry(MACAddress extAddr, UINT16 shrtAdd, UINT16 srcid)
{
    for(int i = 0; i < this->getNumberBeacons(); i++)
    {
	if(entryTable.at(i)->getsrcAddrExtended() == extAddr && entryTable.at(i)->getsrcAddrShort() == shrtAdd && entryTable.at(i)->getsrcPanId() == srcid)
	    return true;
    }

    return false;
}

bool beaconTable::updateBeaconEntry(Ieee802154EnhancedBeaconFrame *msg, MACAddress extAddr, UINT16 shrtAdd, double rxPw, double r, UINT16 srcid, SimTime timestamp)
{
    for(int i = 0; i < this->getNumberBeacons(); i++)
    {
	if(entryTable.at(i)->getsrcAddrExtended() == extAddr && entryTable.at(i)->getsrcAddrShort() == shrtAdd && entryTable.at(i)->getsrcPanId() == srcid)
	{
	    entryTable.at(i)->setrecvPower(rxPw);
	    entryTable.at(i)->setrssi(r);
	    entryTable.at(i)->setreceivedAt(timestamp);
	    entryTable.at(i)->setbcnMsg(msg);
	    return true;
	}
    }
    return false;

}

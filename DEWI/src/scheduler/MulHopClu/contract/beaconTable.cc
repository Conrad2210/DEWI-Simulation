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

bool beaconTable::addEntry(Ieee802154EnhancedBeaconFrame *msg, MACAddress extAddr, UINT16 shrtAdd, double rxPw, double r, UINT16 srcid, SimTime timestamp, double txPw,double dis)
{
    this->entryTable.push_back(new beaconTableEntry(entryTable.size(),msg,extAddr,shrtAdd,rxPw,r,srcid,timestamp,txPw,dis));
    return true;
}

Ieee802154EnhancedBeaconFrame *beaconTable::returnBestBeaconMsg(double *rssi, double *rxPower, double *txPo, double *dis)
{
    if(entryTable.size() != 0)
    {
	double tmpPwr = -1.0;
	int id = -1;
	for(int i = 0; i < (int) entryTable.size(); i++)
	{
	    if(entryTable.at(i)->getrecvPowermW() > tmpPwr)
	    {
		tmpPwr = entryTable.at(i)->getrecvPowermW();
		id = entryTable.at(i)->getid();
	    }
	}

	if(id != -1)
	{
	    *rssi = entryTable.at(id)->getrssi();
	    *rxPower = entryTable.at(id)->getrecvPowerdBm();
	    *txPo = entryTable.at(id)->getTxPowermW();
	    *dis = entryTable.at(id)->getDistance();
	    return entryTable.at(id)->getbcnMsg();
	}
	else
	    return NULL;
    }
    return NULL;
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

bool beaconTable::updateBeaconEntry(Ieee802154EnhancedBeaconFrame *msg, MACAddress extAddr, UINT16 shrtAdd, double rxPwMW, double r, UINT16 srcid, SimTime timestamp)
{
    for(int i = 0; i < this->getNumberBeacons(); i++)
    {
	if(entryTable.at(i)->getsrcAddrExtended() == extAddr && entryTable.at(i)->getsrcAddrShort() == shrtAdd && entryTable.at(i)->getsrcPanId() == srcid)
	{
	    entryTable.at(i)->setrecvPowermW(rxPwMW);
	    entryTable.at(i)->setrssi(r);
	    entryTable.at(i)->setreceivedAt(timestamp);
	    entryTable.at(i)->setbcnMsg(msg);
	    return true;
	}
    }
    return false;

}

void beaconTable::flushBeaconTable()
{
    while(entryTable.size() != 0)
    {
	entryTable.erase(entryTable.begin());
    }
    int i = -0;

    i = i +15;
}

bool beaconTable::CHinDistance(double rssi)
{
    for(int i = 0; i < (int) entryTable.size(); i++)
    {
	if(rssi < entryTable.at(i)->getrssi())
	    return true;
    }

    return false;
}

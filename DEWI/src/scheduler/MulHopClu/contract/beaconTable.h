/*
 * beaconTable.h
 *
 *  Created on: 4 Mar 2015
 *      Author: R00118979
 */

#ifndef BEACONTABLE_H_
#define BEACONTABLE_H_

#include "beaconTableEntry.h"

class beaconTable
{
    public:
	beaconTable();
	virtual ~beaconTable();

    private:
	std::vector<beaconTableEntry *>entryTable;

    public:

	bool addEntry(Ieee802154EnhancedBeaconFrame *msg, MACAddress extAddr, UINT16 shrtAdd, double rxPw, double r, UINT16 srcid, SimTime timestamp, double txPw,double dis);
	bool existBeaconEntry(MACAddress extAddr, UINT16 shrtAdd, UINT16 srcid);
	bool updateBeaconEntry(Ieee802154EnhancedBeaconFrame *msg, MACAddress extAddr, UINT16 shrtAdd, double rxPw, double r, UINT16 srcid, SimTime timestamp);
	beaconTableEntry *getEntryByPos(int pos){return entryTable.at(pos);}
	int getNumberBeacons(){return entryTable.size();}

	void flushBeaconTable();

	Ieee802154EnhancedBeaconFrame *returnBestBeaconMsg(double *rssi, double *rxPower, double *txPo, double *dis);

	bool CHinDistance(double rssi);



};

#endif /* BEACONTABLE_H_ */

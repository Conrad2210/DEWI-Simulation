//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef CBEACONTABLE_H_
#define CBEACONTABLE_H_

#include <csimplemodule.h>
#include "cBeaconTableEntry.h"

class cBeaconTable : public cSimpleModule
{
    public:
	cBeaconTable();
	virtual ~cBeaconTable();

    private:
	std::vector<cBeaconTableEntry *> entryTable;

    public:
	virtual std::string getFullPath() const
	{
	    return cSimpleModule::getFullPath();
	}

    protected:
	virtual void updateDisplayString();
	virtual int numInitStages() const
	{
	    return 3;
	}

	virtual void initialize(int stage);

	virtual void handleMessage(cMessage *msg);
    public:

	bool addEntry(Ieee802154EnhancedBeaconFrame *msg, MACAddress extAddr, UINT16 shrtAdd, double rxPw, double r, UINT16 srcid, SimTime timestamp, double txPw, double dis);
	bool existBeaconEntry(MACAddress extAddr, UINT16 shrtAdd, UINT16 srcid);
	bool updateBeaconEntry(Ieee802154EnhancedBeaconFrame *msg, MACAddress extAddr, UINT16 shrtAdd, double rxPw, double r, UINT16 srcid, SimTime timestamp);
	cBeaconTableEntry *getEntryByPos(int pos)
	{
	    return entryTable.at(pos);
	}
	int getNumberBeacons()
	{
	    return entryTable.size();
	}

	void flushBeaconTable();

	Ieee802154EnhancedBeaconFrame *returnBestBeaconMsg(double *rssi, double *rxPower, double *txPo, double *dis);

	bool CHinDistance(double rssi);
};

#endif /* CBEACONTABLE_H_ */

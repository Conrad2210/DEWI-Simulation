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

#include <beaconTable/cBeaconTable.h>

Define_Module(cBeaconTable);

std::ostream& operator<<(std::ostream& os, const cBeaconTableEntry& e)
{
    os << e.info();
    return os;
}

cBeaconTable::cBeaconTable()
{
    // TODO Auto-generated constructor stub

}

cBeaconTable::~cBeaconTable()
{
    // TODO Auto-generated destructor stub
}
void cBeaconTable::initialize(int stage)
{
    if(stage != 2)
	return;

    updateDisplayString();
    WATCH_PTRVECTOR(entryTable);
}

void cBeaconTable::handleMessage(cMessage *msg)
{
    throw cRuntimeError("this module doesn't process messages");
}

bool cBeaconTable::addEntry(Ieee802154EnhancedBeaconFrame *msg, MACAddress extAddr, UINT16 shrtAdd, double rxPw, double r, UINT16 srcid, SimTime timestamp, double txPw,double dis)
{
    this->entryTable.push_back(new cBeaconTableEntry(entryTable.size(),msg,extAddr,shrtAdd,rxPw,r,srcid,timestamp,txPw,dis));
    updateDisplayString();
    return true;
}
void cBeaconTable::updateDisplayString()
{
    //@author Conrad Dandelski
    if(!ev.isGUI())
	return;

    char buf[80];
    sprintf(buf, "%d Entries", getNumberBeacons());
    getDisplayString().setTagArg("t", 0, buf);
}

Ieee802154EnhancedBeaconFrame *cBeaconTable::returnBestBeaconMsg(double *rssi, double *rxPower, double *txPo, double *dis)
{
    Ieee802154EnhancedBeaconFrame *tmpMsg = NULL;
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
	    tmpMsg =  entryTable.at(id)->getbcnMsg();
	    entryTable.at(id)->setbcnMsg(NULL);
	}
	else
	    tmpMsg =  NULL;
    }
    return tmpMsg;
}

bool cBeaconTable::existBeaconEntry(MACAddress extAddr, UINT16 shrtAdd, UINT16 srcid)
{
    for(int i = 0; i < this->getNumberBeacons(); i++)
    {
	if(entryTable.at(i)->getsrcAddrExtended() == extAddr && entryTable.at(i)->getsrcAddrShort() == shrtAdd && entryTable.at(i)->getsrcPanId() == srcid)
	    return true;
    }

    return false;
}

bool cBeaconTable::updateBeaconEntry(Ieee802154EnhancedBeaconFrame *msg, MACAddress extAddr, UINT16 shrtAdd, double rxPwMW, double r, UINT16 srcid, SimTime timestamp)
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

void cBeaconTable::flushBeaconTable()
{
    while(entryTable.size() != 0)
    {

	//delete entryTable.at(0)->getbcnMsg();
	entryTable.at(0)->setbcnMsg(NULL);
	entryTable.erase(entryTable.begin());
    }
    updateDisplayString();
}

bool cBeaconTable::CHinDistance(double rssi)
{
    for(int i = 0; i < (int) entryTable.size(); i++)
    {
	if(rssi < entryTable.at(i)->getrssi())
	    return true;
    }

    return false;
}

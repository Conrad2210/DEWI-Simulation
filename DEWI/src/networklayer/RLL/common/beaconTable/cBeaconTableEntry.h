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

#ifndef CBEACONTABLEENTRY_H_
#define CBEACONTABLEENTRY_H_
#include "Ieee802154eFrame_m.h"

class cBeaconTableEntry
{
    public:
	cBeaconTableEntry();
	cBeaconTableEntry(int Eid, Ieee802154EnhancedBeaconFrame *msg, MACAddress extAddr, UINT16 shrtAdd, double rxPw, double r, UINT16 srcid, SimTime timestamp, double txPw, double dis);
	virtual ~cBeaconTableEntry();
    private:
	int id;
	Ieee802154EnhancedBeaconFrame *bcnMsg;
	MACAddress srcAddrExtended;
	UINT16 srcAddrShort;
	double recvPower;
	double rssi;
	UINT16 srcPanId;
	SimTime receivedAt;
	double txPower;
	double distance;

    public:

	virtual std::string info() const;

	Ieee802154EnhancedBeaconFrame *getbcnMsg()
	{
	    return bcnMsg;
	}
	void setbcnMsg(Ieee802154EnhancedBeaconFrame * msg)
	{
	    bcnMsg = msg;
	}

	MACAddress getsrcAddrExtended()
	{
	    return srcAddrExtended;
	}
	void setsrcAddrExtended(MACAddress addr)
	{
	    srcAddrExtended = addr;
	}

	UINT16 getsrcAddrShort()
	{
	    return srcAddrShort;
	}
	void setsrcAddrShort(UINT16 addr)
	{
	    srcAddrShort = addr;
	}

	double getrecvPowerdBm()
	{
	    return recvPower;
	}
	void setrecvPowerdBm(double power)
	{
	    recvPower = power;
	}

	double getrecvPowermW();
	void setrecvPowermW(double power);

	double getrssi()
	{
	    return rssi;
	}
	void setrssi(double d)
	{
	    rssi = d;
	}

	UINT16 getsrcPanId()
	{
	    return srcPanId;
	}
	void setsrcPanId(UINT16 id)
	{
	    srcPanId = id;
	}

	SimTime getreceivedAt()
	{
	    return receivedAt;
	}
	void setreceivedAt(SimTime at)
	{
	    receivedAt = at;
	}

	int getid()
	{
	    return id;
	}
	void setid(int i)
	{
	    id = i;
	}

	double getTxPowermW()
	{
	    return txPower;
	}
	void setTxPowermW(double d)
	{
	    txPower = d;
	}

	double getTxPowerdBm();
	void setTxPowerdBm(double d);

	double getDistance()
	{
	    return distance;
	}
	void setDistance(double d)
	{
	    distance = d;
	}

};

#endif /* CBEACONTABLEENTRY_H_ */

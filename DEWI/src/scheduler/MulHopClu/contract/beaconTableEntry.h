/*
 * beaconTableEntry.h
 *
 *  Created on: 4 Mar 2015
 *      Author: R00118979
 */

#ifndef BEACONTABLEENTRY_H_
#define BEACONTABLEENTRY_H_

#include "Ieee802154eFrame_m.h"

class beaconTableEntry
{
    public:
	beaconTableEntry();
	beaconTableEntry(int Eid, Ieee802154EnhancedBeaconFrame *msg, MACAddress extAddr, UINT16 shrtAdd, double rxPw, double r, UINT16 srcid, SimTime timestamp);
	virtual ~beaconTableEntry();

    private:
	int id;
	Ieee802154EnhancedBeaconFrame *bcnMsg;
	MACAddress srcAddrExtended;
	UINT16 srcAddrShort;
	double recvPower;
	double rssi;
	UINT16 srcPanId;
	SimTime receivedAt;

    public:

	Ieee802154EnhancedBeaconFrame *getbcnMsg(){return bcnMsg;}
	void setbcnMsg(Ieee802154EnhancedBeaconFrame * msg){bcnMsg = msg;}

	MACAddress getsrcAddrExtended(){return srcAddrExtended;}
	void setsrcAddrExtended(MACAddress addr){srcAddrExtended = addr;}

	UINT16 getsrcAddrShort(){return srcAddrShort;}
	void setsrcAddrShort(UINT16 addr){srcAddrShort = addr;}

	double getrecvPower(){return recvPower;}
	void setrecvPower(double power){recvPower = power;}

	double getrssi(){return rssi;}
	void setrssi(double d){rssi=d;}

	UINT16 getsrcPanId(){return srcPanId;}
	void setsrcPanId(UINT16 id){srcPanId = id;}

	SimTime getreceivedAt(){return receivedAt;}
	void setreceivedAt(SimTime at){receivedAt = at;}

};

#endif /* BEACONTABLEENTRY_H_ */

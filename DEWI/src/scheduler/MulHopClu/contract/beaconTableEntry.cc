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

beaconTableEntry::beaconTableEntry(int Eid, Ieee802154EnhancedBeaconFrame *msg, MACAddress extAddr, UINT16 shrtAdd, double rxPwMW, double r, UINT16 srcid, SimTime timestamp, double txPwmW, double dis)
{
	id = Eid;
	bcnMsg = msg;
	srcAddrExtended = extAddr;
	srcAddrShort = shrtAdd;
	setrecvPowermW(rxPwMW);
	rssi = r;
	srcPanId = srcid;
	receivedAt = timestamp;
	setTxPowermW(txPwmW);
	distance = dis;


}

beaconTableEntry::~beaconTableEntry()
{
    // TODO Auto-generated destructor stub
}


double beaconTableEntry::getrecvPowermW()
{
    return pow(10,recvPower/10)/1e3;
}



void beaconTableEntry::setrecvPowermW(double power)
{
    recvPower = 10 * log10((power * 1e3));
}

double beaconTableEntry::getTxPowerdBm()
{
    return 10*log10(txPower);
}
void beaconTableEntry::setTxPowerdBm(double d){
    txPower = 10*log10(d);
}


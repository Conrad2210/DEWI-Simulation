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

#include <beaconTable/cBeaconTableEntry.h>

cBeaconTableEntry::cBeaconTableEntry()
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

cBeaconTableEntry::cBeaconTableEntry(int Eid, Ieee802154EnhancedBeaconFrame *msg, MACAddress extAddr, UINT16 shrtAdd, double rxPwMW, double r, UINT16 srcid, SimTime timestamp, double txPwmW, double dis)
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

cBeaconTableEntry::~cBeaconTableEntry()
{
    delete bcnMsg;
}

std::string cBeaconTableEntry::info() const
{
    std::stringstream out;
    out << "Id: " << id;
    out << " Src Addr: " << srcAddrShort;
    out << " RxPower: " << recvPower;
    out << " RSSI: " << rssi;
    out << " SrcPanId: " << srcPanId;
    out << " receivedAt: " << receivedAt;
    out << " txPower: " << txPower;
    out << " distance: " << distance;

    return out.str();
}

double cBeaconTableEntry::getrecvPowermW()
{
    return pow(10, recvPower / 10) / 1e3;
}

void cBeaconTableEntry::setrecvPowermW(double power)
{
    recvPower = 10 * log10((power * 1e3));
}

double cBeaconTableEntry::getTxPowerdBm()
{
    return 10 * log10(txPower);
}
void cBeaconTableEntry::setTxPowerdBm(double d)
{
    txPower = 10 * log10(d);
}

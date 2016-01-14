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

/*
 * @author: Stefan Reis
 * Copyright(c) 2014: Stefan Reis
 * Copyright(c) 2014: Nimbus Centre for Embedded Systems Research, Cork, Ireland
 * Copyright(c) 2014: Cork Institute of Technology, Cork, Ireland
 */

#include "macNeighborTableEntry.h"
#include "macNeighborTable.h"
#include <stdio.h>
#include <sstream>

macNeighborTableEntry::macNeighborTableEntry()
{
    // TODO Auto-generated constructor stub
    ownerp = NULL;
    neighborId = -1;
    neighborSize = -1;
    ShortAddress = 0x0000;
    ExtendedAddress = MACAddress(0xffff);
    SDIndex = 0;
    ChannelOffset = 0;
    TrackBeacon = false;
    BeaconLostCount = 0;
    numTxData = 0;
    numTxDataAck = 0;
    numRxData = 0;
    numRxDataAck = 0;
    lastASN = 0;
    RPLrank = 0;
    isTimeSource = false;
    RPL_OF = 0;
    transDelay = 0;
    nextStageCH = false;
    sameStageCH = false;
    lowerStageCH = false;
    myCH = false;
    myCS = false;
    lastPktReceived = SimTime(0.0);
    this->nNewCoverage = -1;
    this->dCurTXPw = -1;
    this->dDistance = -1;
    this->nNodeDegree = -1;
    this->dRSSI = -1;
    this->dWeight = -1;
    this->nClusterDegree = -1;
    this->bPosCluster = false;
    this->dWeightSecond = -1;
    this->assigndTo = MACAddress::UNSPECIFIED_ADDRESS;
}

macNeighborTableEntry::~macNeighborTableEntry()
{
    // TODO Auto-generated destructor stub
    resetNeighbor();
}

std::string macNeighborTableEntry::info() const
{
    std::stringstream out;
    out<<"NeighborId: " << neighborId;
    out<<" NeighborSize: " << neighborSize;
    out<<" ShortAddress: "<< ShortAddress;
    out<<" ExtendedAddress: " << ExtendedAddress;
    out<<" TxPower: " << dCurTXPw;
    out<<" RSSI: " << dRSSI << "dB";
    out<<" Pos Cluster:" << (bPosCluster?"True":"False");
    out << " Weight: " << dWeight;
    out<<" Distance: " << dDistance << "m";
    out<<" Node Degree: " << nNodeDegree;
    out<<" Cluster Degree: " << nClusterDegree;
    out<< " new Coverage: " << nNewCoverage;
    out<<" Second Weight: " << dWeightSecond;
    out<<" Assigned To: " << assigndTo;
    out<<" MyCH: " << (myCH?"True":"False");
    out<<" MyCS: " << (myCS?"True":"False");
    out<<" CH same Stage: " << (sameStageCH?"True":"False");
    out<<" CH lower Stage: " << (lowerStageCH?"True":"False");
    out<<" CH next Stage: " << (nextStageCH?"True":"False");
    out<<" Last PKT: " << lastPktReceived << "s";
//    out<<" SDIndex: " << (int)SDIndex;
//    out<<" ChannelOffset: " << (int)ChannelOffset;
//    out<<" TrackBeacon: " << TrackBeacon;
//    out<<" BeaconLostCount: " << (int)BeaconLostCount;
//    out<<" numTxData: " << (int)numTxData;
//    out<<" numTXDataAck: " << (int)numTxDataAck;
//    out<<" numRxData: " << (int)numRxData;
//    out<<" numRxDataAck: " << (int)numRxDataAck;
//    out<<" lastASN: " << (int)lastASN;
//    out<<" RPLrank: " << (int)RPLrank;
//    out<<" isTimeSource: " << isTimeSource;
//    out<<" RPL_OF: " << (int)RPL_OF;
//    out<<" transDelay: " << (int)transDelay;
    return out.str();
}

std::string macNeighborTableEntry::getFullPath() const
{
    return ownerp == NULL ? getFullName() : ownerp -> getHostModule() -> getFullPath() + "." + getFullName();

}

void macNeighborTableEntry::changed(int category)
{
    if(ownerp)
        ownerp -> neighborChanged(this,category);
}

void macNeighborTableEntry::resetNeighbor()
{
    this -> neighborId = -1;
    this -> neighborSize = -1;
    this -> ShortAddress = 0x0000;
    this -> ExtendedAddress = MACAddress(0xffff);
    this -> SDIndex = 0;
    this -> ChannelOffset = 0;
    this -> TrackBeacon = false;
    this -> BeaconLostCount = 0;
    this -> numTxData = 0;
    this -> numTxDataAck = 0;
    this -> numRxData = 0;
    this -> numRxDataAck = 0;
    this -> lastASN = 0;
    this -> RPLrank = 0;
    this -> isTimeSource = false;
    this -> RPL_OF = 0;
    this -> transDelay = 0;
    this->nNewCoverage = -1;
    this->dCurTXPw = -1;
    this->dDistance = -1;
    this->nNodeDegree = -1;
    this->dRSSI = -1;
    this->dWeight = -1;
}

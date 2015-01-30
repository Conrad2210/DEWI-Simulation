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

#ifndef MACNEIGHBORTABLEENTRY_H_
#define MACNEIGHBORTABLEENTRY_H_

#include "INETDefs.h"
#include "NotifierConsts.h"
#include "Ieee802154eDef.h"
#include "Ieee802154eEnum.h"
#include "Ieee802154eDefSlotframe.h"

class  IMacNeighborTable;

class INET_API macNeighborTableEntry : public cObject
{
    public:
        enum State {UP, DOWN, GOING_UP, GOING_DOWN};
    protected:
        IMacNeighborTable *ownerp;
        cModule *neighborModule;
        int neighborId;
        int neighborSize; //number of Neighbors in the Neighbortable
        State state;

        //Elements of Neighbor Information - Std 802.15.4e-2012 (table 52i) page 181

        /* The 16-bit address of the neighbor device. */
        UINT_16 ShortAddress;

        /* The 64-bit (IEEE) address of the neighbor device. */
        MACAddress ExtendedAddress;

        /* The allocating Superframe Duration (SD) index number for beacon frame. (not use in TSCH)*/
        UINT_16 SDIndex;

        /* The offset value of ChannelHoppingSequence. */
        UINT_16 ChannelOffset;

        /* TRUE if the MLME is to track all future beacons of the neighbor device.
         * FALSE if the MLME is not to track beacon of the neighbor device. */
        bool TrackBeacon;

        /* The number of consecutive lost beacons. */
        UINT_8 BeaconLostCount;

        // from Minimal 6TiSCH Configuration - draft-ietf-6tisch-minimal-02
        // IETF 6tsch Working Group

        /* number of transmitted packets to that neighbor */
        UINT_16 numTxData;

        /* number of transmitted packets that have been acknowledged by that neighbor */
        UINT_16 numTxDataAck;

        /* number of received packets from that neighbor */
        UINT_16 numRxData;

        /* number of received packets that have been acknowledged for that neighbor */
        UINT_16 numRxDataAck;

        /* ASN when that neighbor was heard for the last time.
         * This can beused to trigger a keep-alive message. */
        UINT_64 lastASN;

        /* RPL rank of that neighbor. */
        UINT_16 RPLrank;

        /* A flag which indicates whether this neighbor is a time source neighbor. */
        bool isTimeSource;

        /* In addition of that information, each node has to be able to compute some
         * RPL objective function (OF) taking into account the neighbor and connectivity
         * statistics. An example RPL objective function is the ETX. */
        UINT_16 RPL_OF;

        /* Transmission Delay: for the Std 802.15.4e-2012 retransmission calculation. */
        int transDelay;

    private:
        macNeighborTableEntry(const macNeighborTableEntry& obj);
        macNeighborTableEntry& operator=(const macNeighborTableEntry& obj);

    protected:
        //change notification
        virtual void configChanged() {changed(NF_INTERFACE_CONFIG_CHANGED);}
        virtual void stateChanged() {changed(NF_INTERFACE_STATE_CHANGED);}
        virtual void changed(int category);

    public:
        macNeighborTableEntry();
        virtual ~macNeighborTableEntry();

        virtual void resetNeighbor();
        virtual std::string info() const;
        virtual std::string getFullPath() const;

        //Returns the NeighborTable this Neighbor lives in, or Null
        IMacNeighborTable *getNeighborTable() const{return ownerp;}

        //Returns the requested State of this Neighbor
        State getState() const{return state;}

        //Return function
        int getNeighborId() const {return neighborId;}
        cModule *getNeighborModule() const{return neighborModule;}
        int getNeighborSize() const{return neighborSize;}
        UINT_16 getShortAddress() const{return ShortAddress;}
        MACAddress getExtendedAddress() const{return ExtendedAddress;}
        UINT_16 getSDIndex() const{return SDIndex;}
        UINT_16 getChannelOffset() const{return ChannelOffset;}
        bool getTrackBeacon() const{return TrackBeacon;}
        UINT_8 getBeaconLostCount() const{return BeaconLostCount;}
        UINT_16 getNumTxData() const{return numTxData;}
        UINT_16 getNumTxDataAck() const{return numTxDataAck;}
        UINT_16 getNumRxData() const{return numRxData;}
        UINT_16 getNumRxDataAck() const{return numRxDataAck;}
        UINT_64 getLastASN() const{return lastASN;}
        UINT_16 getRPLrank() const{return RPLrank;}
        bool getIsTimeSource() const{return isTimeSource;}
        UINT_16 getRPL_OF() const{return RPL_OF;}
        int getTransDelay() const{return transDelay;}

        //Set function
        virtual void setNeighborTable(IMacNeighborTable* t) {this->ownerp = t;}
        virtual void setNeighborId(int id) {neighborId = id;}
        virtual void setNeighborSize(int size) {neighborSize = size;}
        virtual void setShortAddress(UINT_16 address) {ShortAddress = address;}
        virtual void setExtendedAddress(MACAddress address) {ExtendedAddress = address;}
        virtual void setSDIndex(UINT_16 index) {SDIndex = index;}
        virtual void setChannelOffset(UINT_16 channelOffset) {ChannelOffset = channelOffset;}
        virtual void setTrackBeacon(bool track) {TrackBeacon = track;}
        virtual void setBeaconLostCount(UINT_8 bcnLost) {BeaconLostCount = bcnLost;}
        virtual void setNumTxData(UINT_16 cntTXData) {numTxData = cntTXData;}
        virtual void setNumTxDataAck(UINT_16 cntTxDataAck) {numTxDataAck = cntTxDataAck;}
        virtual void setNumRxData(UINT_16 cntRxData) {numRxData = cntRxData;}
        virtual void setNumRxDataAck(UINT_16 cntRxDataAck) {numRxDataAck = cntRxDataAck;}
        virtual void setLastASN(UINT_64 lastAsn) {lastASN = lastAsn;}
        virtual void setRPLrank(UINT_16 rplRank) {RPLrank = rplRank;}
        virtual void setIsTimeSource(bool timeSource) {isTimeSource = timeSource;}
        virtual void setRPL_OF(UINT_16 rplOF) {RPL_OF = rplOF;}
        virtual void setTransDelay(int delay) {transDelay = delay;}

        virtual void incrBeaconLostCount() {BeaconLostCount = BeaconLostCount+1;}
        virtual void incrNumTxData() {numTxData = numTxData+1;}
        virtual void incrNumTxDataAck() {numTxDataAck = numTxDataAck+1;}
        virtual void incrNumRxData() {numRxData = numRxData+1;}
        virtual void incrNumRxDataAck() {numRxDataAck = numRxDataAck+1;}
        virtual void decrTransDelay() {transDelay = transDelay-1;}
};
#endif /* MACNEIGHBORTABLEENTRY_H_ */

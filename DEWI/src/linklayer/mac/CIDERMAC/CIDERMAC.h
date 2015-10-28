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

#ifndef CIDERMAC_H_
#define CIDERMAC_H_

#include <Ieee802154Def.h>
#include <MACAddress.h>
#include <RadioState.h>
#include <WirelessMacBase.h>
#include <string>

class Ieee802154eQueue;

class IMacLinkTable;
class macLinkTableEntry;
class macNeighborTableEntry;
class macTimeslotTableEntry;

class IMacSlotframeTable;
class ImacLinkTable;
class IMacTimeslotTable;
class IMacHoppingSequenceList;
class IMacNeighborTable;

using namespace std;
class CIDERMAC: public WirelessMacBase
{
public:
	CIDERMAC();
	virtual ~CIDERMAC();

	virtual int numInitStages() const
	{
		return 3;
	}
	void commonInitialize();
	virtual void intialize(int stage);
	virtual void initializeQueueModule();

	virtual void handleSelfMsg(cMessage *msg);

	virtual void handleMessage(cMessage *msg);

	virtual void handleLowerMsg(cPacket *pkt);

	virtual void handleUpperMsg(cPacket *pkt);

	virtual void handleCommand(cMessage *msg);

	virtual InterfaceEntry* createInterfaceEntry();

	virtual void flushQueue();

	/**
	 * should clear queue silently
	 */
	virtual void clearQueue();

	IE3ADDR getMacAddr()
	{
		return aExtendedAddress;
	}

	// Use to distinguish the radio module that send the event
	int radioModule;
	InterfaceEntry *iface;
	RadioState::State radioState;

	int getRadioModuleId()
	{
	    return radioModule;
	}

	MACAddress configurationMacAddress();

	UINT_16 getShortAddress(MACAddress v)
	{
		return (UINT_16) v.getInt();
	}
	MACAddress getLongAddress(UINT_16 v);

protected:

	//in/out gates
	int mUpperLayerIn;
	int mUpperLayerOut;
	int mLowerLayerIn;
	int mLowerLayerOut;
	int mQueueIn;
	int mQueueOut;

	/** @brief  pointer to the NotificationBoard module */
	NotificationBoard* mpNb;

	/** @brief pointer to the passive queue module */
	// IPassiveQueue* queueModule;
	Ieee802154eQueue* queueModule;

	//variables:
	bool debug;                    // debug switch
	// whether is PAN coordinator
	bool isPANCoor;                // whether is PAN coordinator
	// beacon order [0, 15] - default 15 (no beacon) - Std 802.15.4-2011 (table 52) page 127 - see 5.1.1.1
	double BO;                 // beacon order [0, 15]
	// superframe order [0, BO) - default 15 (no beacon) - Std 802.15.4-2011 (table 52) page 128 - see 5.1.1
	double SO;                 // superframe order [0, BO)
	double dataTransMode;      // 1: direct; 2: indirect; 3: GTS
	// name of optional external queue module

	string panCoorName;            // name of the PAN coordinator

	bool ack4Data;

	bool setRxOnWhenIdle;

	//TSCH VARIABLES
	bool TSCHcapable;

	bool useTSCH;

	// for the Std 802.15.4e-2014 TSCH mechanism - to set the Node as a timeScource
	bool timeSource;

	// for the Std 802.15.4e-2014 TSCH mechanism - to use or not the CCA during the TSCH CCA transmission
	bool useCCA;
	//
	int maxRetries; // default = 3; range = 0-7

	MACAddress macaddress;

	IE3ADDR aExtendedAddress;


	/** @brief slotframe table */
	IMacSlotframeTable* slotframeTable;

	/** @brief timeslot table */
	IMacTimeslotTable* timeslotTable;

	/** @brief buffer for one timeslot entry */
	macTimeslotTableEntry* timeslotTemplate;

	/** @brief the active used timeslot ID */
	UINT_8 useTimeslotID;

	/** @brief Link table */
	IMacLinkTable* linkTable;

	/** @brief active LinkEntry */
	macLinkTableEntry* activeLinkEntry;

	/** @brief Hopping Sequence List */
	IMacHoppingSequenceList* hoppingSequenceList;

	/** @brief the active used hopping sequence ID */
	UINT_8 useHoppingSequenceID;

	/** @brief neighbor information table */
	IMacNeighborTable* neighborTable;

	/** @brief active neighbor information entry */
	macNeighborTableEntry* activeNeighborEntry;
	//@}
};

#endif /* CIDERMAC_H_ */

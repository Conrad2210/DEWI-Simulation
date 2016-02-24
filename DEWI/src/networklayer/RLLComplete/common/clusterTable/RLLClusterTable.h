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

#ifndef RLLCLUSTERTABEL_H_
#define RLLCLUSTERTABEL_H_

#include <vector>

#include "INETDefs.h"

#include "IRLLClusterTable.h"
#include "RLLClusterTableEntry.h"


class INET_API RLLClusterTable : public cSimpleModule, public IRLLClusterTable
{

    protected:
	std::vector<RLLClusterTableEntry *>entryTable;

    public:
	virtual std::string getFullPath() const {return cSimpleModule::getFullPath();}

    protected:

	virtual void updateDisplayString();
	    virtual int numInitStages() const { return 3;}

	    virtual void initialize(int stage);

	    virtual void handleMessage(cMessage *msg);

	    int tmpNumEntries;

    public:
	RLLClusterTable();
	virtual ~RLLClusterTable();

	virtual cModule *getHostModule();

	virtual int getNumEntries();

	/*
	 * Adds an slotframe to macSlotframeTable. Slotframes are allowed to be modified
	 * while in the macSlotframeTable. (There is a notification mechanism that
	 * allows macSlotframeTable internals to be updated on a Slotframe entry change.)
	 */
	virtual bool addEntry(RLLClusterTableEntry *entry);
	virtual bool addEntry(int stage,UINT_16 address, char* name, bool is, UINT_16 id);

	/*
	 * Deletes the given slotframe from the macSlotframeTable.
	 * Returns true if the slotframe was deleted, and false if it was
	 * not in the macLinkTable
	 */
	virtual bool deleteEntry(RLLClusterTableEntry *entry);

	/*
	 * Edits the given slotframe
	 */
	virtual void editEntry(RLLClusterTableEntry *entry);

	/*
	 * To be called from link objects whenever a field changes. Used for
	 * maintaining internal data structures and firing "macLinkTable changed"
	 * notifications
	 */
	virtual void entryChanged();

	/*
	 * Returns the link from the macLinkTable
	 */
	virtual RLLClusterTableEntry *getEntry(int pos);

	virtual RLLClusterTableEntry *getEntryById(int id);

	virtual RLLClusterTableEntry *getEntryByShrtAddr(UINT_16 address);

	virtual bool existHigherCH(int stage);

	virtual bool existLowerCH(int stage);

	virtual bool existCS(int stage);

	virtual UINT_8 getNumberCH();

	//deletes all entries
	virtual void clearTable();

	channelList generateChannelList(UINT_16 address,channelList CHList);
	channelList getChannelList(UINT_16 address);
};

#endif /* RLLCLUSTERTABEL_H_ */

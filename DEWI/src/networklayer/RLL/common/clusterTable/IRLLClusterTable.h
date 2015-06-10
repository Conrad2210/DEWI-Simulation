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

#ifndef IRLLCLUSTERTABLE_H_
#define IRLLCLUSTERTABLE_H_

#include "INETDefs.h"

#include "RLLClusterTableEntry.h"


class INET_API IRLLClusterTable
{
    public:
	IRLLClusterTable();
	virtual ~IRLLClusterTable();

	virtual cModule *getHostModule() = 0;

	virtual int getNumEntries() = 0;

	/*
	 * Adds an slotframe to macSlotframeTable. Slotframes are allowed to be modified
	 * while in the macSlotframeTable. (There is a notification mechanism that
	 * allows macSlotframeTable internals to be updated on a Slotframe entry change.)
	 */
	virtual bool addEntry(RLLClusterTableEntry *entry) = 0;
	virtual bool addEntry(int stage,UINT_16 address, char* name, bool is) = 0;

	/*
	 * Deletes the given slotframe from the macSlotframeTable.
	 * Returns true if the slotframe was deleted, and false if it was
	 * not in the macLinkTable
	 */
	virtual bool deleteEntry(RLLClusterTableEntry *entry) = 0;

	/*
	 * Edits the given slotframe
	 */
	virtual void editEntry(RLLClusterTableEntry *entry) = 0;

	/*
	 * To be called from link objects whenever a field changes. Used for
	 * maintaining internal data structures and firing "macLinkTable changed"
	 * notifications
	 */
	virtual void entryChanged() = 0;

	/*
	 * Returns the link from the macLinkTable
	 */
	virtual RLLClusterTableEntry *getEntry(int pos) = 0;

	virtual RLLClusterTableEntry *getEntryById(int id) = 0;

	virtual RLLClusterTableEntry *getEntryByShrtAddr(UINT_16 address) = 0;

	//deletes all entries
	virtual void clearTable() = 0;
};

#endif /* IRLLCLUSTERTABLE_H_ */

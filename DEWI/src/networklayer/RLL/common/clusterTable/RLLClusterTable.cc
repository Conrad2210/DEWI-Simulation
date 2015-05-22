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

#include <common/clusterTable/RLLClusterTable.h>
Define_Module(RLLClusterTable);

RLLClusterTable::RLLClusterTable()
{
    // TODO Auto-generated constructor stub

}

RLLClusterTable::~RLLClusterTable()
{
    // TODO Auto-generated destructor stub
}

cModule *RLLClusterTable::getHostModule()
{
}

int RLLClusterTable::getNumSlotframes()
{
}

/*
 * Adds an slotframe to macSlotframeTable. Slotframes are allowed to be modified
 * while in the macSlotframeTable. (There is a notification mechanism that
 * allows macSlotframeTable internals to be updated on a Slotframe entry change.)
 */
bool RLLClusterTable::addEntry(RLLClusterTableEntry *entry)
{
}

/*
 * Deletes the given slotframe from the macSlotframeTable.
 * Returns true if the slotframe was deleted, and false if it was
 * not in the macLinkTable
 */
bool RLLClusterTable::deleteEntry(RLLClusterTableEntry *entry)
{
}

/*
 * Edits the given slotframe
 */
void RLLClusterTable::editEntry(RLLClusterTableEntry *entry)
{
}

/*
 * To be called from link objects whenever a field changes. Used for
 * maintaining internal data structures and firing "macLinkTable changed"
 * notifications
 */
void RLLClusterTable::entryChanged(RLLClusterTableEntry *entry, int category)
{
}

/*
 * Returns the link from the macLinkTable
 */
RLLClusterTableEntry *RLLClusterTable::getEntry(int pos)
{
}

RLLClusterTableEntry *RLLClusterTable::getEntryById(int id)
{
}
//deletes all entries
void RLLClusterTable::clearTable()
{
}

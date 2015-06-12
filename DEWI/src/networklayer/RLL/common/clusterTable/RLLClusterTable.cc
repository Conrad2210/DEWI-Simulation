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
#include "opp_utils.h"

Define_Module(RLLClusterTable);

std::ostream& operator<<(std::ostream& os, const RLLClusterTableEntry& e)
{
    os << e.info();
    return os;
}

RLLClusterTable::RLLClusterTable()
{
    // TODO Auto-generated constructor stub
    tmpNumEntries = -1;

}

RLLClusterTable::~RLLClusterTable()
{
    // TODO Auto-generated destructor stub
}

void RLLClusterTable::initialize(int stage)
{
    if(stage == 1)
    {
	WATCH_PTRVECTOR(entryTable);
    }
    else if(stage == 2)
    {
	updateDisplayString();
    }
}

void RLLClusterTable::updateDisplayString()
{
    //@author Conrad Dandelski
    if(!ev.isGUI())
	return;

    char buf[80];
    sprintf(buf, "%d Entries", getNumEntries());
    getDisplayString().setTagArg("t", 0, buf);
}

cModule *RLLClusterTable::getHostModule()
{

    // return findContainingNode(this);
    return NULL;
}

void RLLClusterTable::handleMessage(cMessage *msg)
{
    throw cRuntimeError("this module doesn't process messages");
}

int RLLClusterTable::getNumEntries()
{
    if(tmpNumEntries == -1)
    {
	int n = 0;
	int maxId = entryTable.size();
	for(int i = 0; i < maxId; i++)
	    n++;

	tmpNumEntries = n;
    }
    return tmpNumEntries;
}

/*
 * Adds an slotframe to macSlotframeTable. Slotframes are allowed to be modified
 * while in the macSlotframeTable. (There is a notification mechanism that
 * allows macSlotframeTable internals to be updated on a Slotframe entry change.)
 */
bool RLLClusterTable::addEntry(RLLClusterTableEntry *entry)
{

    entry->setEntryId(entryTable.size());

    if(getEntryById(entry->getEntryId()) != NULL)
    {
	throw cRuntimeError("addTable(): cluster entry '%d' already exists in clusterTable", entry->getEntryId());
	return false;
    }

    entryTable.push_back(entry);
    updateDisplayString();
    return true;

}

bool RLLClusterTable::addEntry(int stage, UINT_16 address, char* name, bool is, UINT_16 id)
{
    RLLClusterTableEntry *entry = new RLLClusterTableEntry();

    entry->setEntryId(entryTable.size());
    if(getEntryById(entry->getEntryId()) != NULL)
    {
	throw cRuntimeError("addTable(): cluster entry '%d' already exists in clusterTable", entry->getEntryId());
	return false;
    }

    entry->setStage(stage);
    entry->setAddress(address);
    entry->setNodeName(name);
    entry->setIsCH(is);
    entry->setPanId(id);
    entryTable.push_back(entry);

    return true;
}

/*
 * Deletes the given slotframe from the macSlotframeTable.
 * Returns true if the slotframe was deleted, and false if it was
 * not in the macLinkTable
 */
bool RLLClusterTable::deleteEntry(RLLClusterTableEntry *entry)
{
    if(entry == NULL)
	return false;

    if(entry != getEntryById(entry->getEntryId()))
	throw cRuntimeError("deleteEntry(): entry '%d' not found in clusterTable", entry->getEntryId());

    entryTable.erase(entryTable.begin()+entry->getEntryId());
    for(int i  = 0; i < (int)entryTable.size();i++)
	entryTable.at(i)->setEntryId(i);
    //delete entry;
    return true;
}

/*
 * Edits the given slotframe
 */
void RLLClusterTable::editEntry(RLLClusterTableEntry *entry)
{
    std::string nodename = getParentModule()->getFullName();
    bool tmp = false;
    int id = entry->getEntryId();
    if(getEntryById(id) == NULL)
    {
	if(ev.isGUI())
	    EV << nodename << "not possible to edit clusterEntry:" << entry->getEntryId() << ". Entry doesn't exist!!!" << "\n";

	return;
    }

    RLLClusterTableEntry *tmpEntry = getEntryById(id);

    if(tmpEntry->getAddress() != entry->getAddress())
    {
	getEntryById(id)->setAddress(entry->getAddress());
	tmp = true;
    }

    if(tmpEntry->getIsCH() != entry->getIsCH())
    {
	getEntry(id)->setIsCH(entry->getIsCH());
	tmp = true;
    }

    if(tmpEntry->getStage() != entry->getStage())
    {
	getEntry(id)->setStage(entry->getStage());
	tmp = true;
    }

    if(tmp)
    {
	if(ev.isGUI())
	{
	    EV << nodename << ": Cluster Table entry changed: " << entry->getEntryId() << endl;
	}
	entryChanged();
    }
    else
    {
	EV << nodename << ": Cluster Table entry NOTHING changed: " << entry->getEntryId() << endl;
    }

}

/*
 * To be called from link objects whenever a field changes. Used for
 * maintaining internal data structures and firing "macLinkTable changed"
 * notifications
 */
void RLLClusterTable::entryChanged()
{
    if(ev.isGUI())
	updateDisplayString();
}

/*
 * Returns the link from the macLinkTable
 */
RLLClusterTableEntry *RLLClusterTable::getEntry(int pos)
{
    int n = getNumEntries();

    if(pos < 0 || pos >= n)
	throw cRuntimeError("getEntry(): clusterEntry index %d out of range 0..%d", pos, n - 1);

    return entryTable.at(pos);
}

RLLClusterTableEntry *RLLClusterTable::getEntryById(int id)
{

    for(int i = 0; i < (int)entryTable.size(); i++)
    {
	if(entryTable.at(i)->getEntryId() == id)
	    return entryTable.at(i);
    }

    return NULL;
}
RLLClusterTableEntry *RLLClusterTable::getEntryByShrtAddr(UINT_16 address)
{
    for(int i = 0; i < (int)entryTable.size(); i++)
    {
	if(address == entryTable.at(i)->getAddress())
	    return entryTable.at(i);
    }
    return NULL;
}

//deletes all entries
void RLLClusterTable::clearTable()
{

    while(entryTable.size() != 0)
	entryTable.erase(entryTable.begin());
}

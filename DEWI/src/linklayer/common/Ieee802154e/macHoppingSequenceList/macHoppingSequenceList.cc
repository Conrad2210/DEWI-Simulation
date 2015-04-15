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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <algorithm>
#include <sstream>

#include "macHoppingSequenceList.h"

#include "IInterfaceTable.h"
#include "InterfaceTableAccess.h"
#include "NotificationBoard.h"
#include "NotifierConsts.h"
#include "macHoppingSequenceListParser.h"
#include "macHoppingSequenceListEntry.h"
#include "Ieee802154eDefSlotframe.h"
#include "opp_utils.h"

Define_Module(macHoppingSequenceList);

#define ENTRYIDS_START 0

std::ostream& operator<<(std::ostream& os, const macHoppingSequenceListEntry& e)
{
    os << e.info();
    return os;
}

macHoppingSequenceList::macHoppingSequenceList()
{
    nb = NULL;
    tmpNumEntries = -1;
    tmpEntryList = NULL;
}

macHoppingSequenceList::~macHoppingSequenceList()
{
    while(idToEntry.size() != 0)
	idToEntry.erase(idToEntry.begin());

    delete[] tmpEntryList;
}

void macHoppingSequenceList::initialize(int stage)
{
    if (stage == 0)
    {
        // get a pointer to the NotificationBoard module and IInterfaceTable
        nb = NotificationBoardAccess().get();
        ift = InterfaceTableAccess().get();

        readFromFile = par("readHoppingSequenceListFromFile");

        nb->subscribe(this, NF_HOPSEQ_CREATED);
        nb->subscribe(this, NF_HOPSEQ_DELETED);
        nb->subscribe(this, NF_HOPSEQ_CONFIG_CHANGED);
        nb->subscribe(this, NF_HOPSEQ_STATE_CHANGED);

        WATCH_PTRVECTOR(idToEntry);
    }
    else if (stage == 1)
    {

    }
    else if (stage == 2)
    {
        if (readFromFile)
        {
            const char* tmpHoppingSequenceFileName = par("hoppingSequenceListFilename");
            const char*tmpFolderName = par("hoppingSequenceListSubFolder");
            // file name generally for all
            std::string temp = (std::string) tmpHoppingSequenceFileName;
            const char* hoppingSequenceListFileName = const_cast<char*>(temp.c_str());
            // use for every node number the some hopping sequence list
            std::string pathHoppingSequenceListFiles = (std::string) tmpFolderName + (std::string) hoppingSequenceListFileName;
            const char *filename = pathHoppingSequenceListFiles.c_str();
            // read hopping sequence list file (and interface configuration)
            macHoppingSequenceListParser parser(ift, this);
            if (*filename && parser.readHoppingSequenceListFromFile(filename)==-1)
                error("Error reading hopping sequence list file %s", filename);
        }

        updateDisplayString();
    }
}

void macHoppingSequenceList::updateDisplayString()
{
    if (!ev.isGUI())
        return;

    char buf[80];
    sprintf(buf, "%d Entries", getNumEntries());
    getDisplayString().setTagArg("t", 0, buf);
}

void macHoppingSequenceList::handleMessage(cMessage *msg)
{
    throw cRuntimeError("this module doesn't process messages");
}

void macHoppingSequenceList::receiveChangeNotification(int category, const cObject *details)
{
    Enter_Method_Silent();
    printNotificationBanner(category, details);
}

cModule *macHoppingSequenceList::getHostModule()
{
    return findContainingNode(this);
}

int macHoppingSequenceList::getNumEntries()
{
    if (tmpNumEntries == -1)
    {
        int n = 0;
        int maxId = idToEntry.size();

        for (int i = 0; i < maxId; i++)
            n++;

        tmpNumEntries = n;
    }
    return tmpNumEntries;
}

macHoppingSequenceListEntry *macHoppingSequenceList::getEntry(int pos)
{
    int n = getNumEntries(); //also fills tmpNumEntrys;

    if (pos < 0 || pos >= n)
        throw cRuntimeError("getEntries(): Entry index %d out of range 0..%d", pos, n - 1);

    if (!tmpEntryList)
    {
        tmpEntryList = new macHoppingSequenceListEntry *[n];
        int k = 0;
        int maxId = idToEntry.size();

        for (int i = 0; i < maxId; i++)
        {
            if (idToEntry[i])
            {
                tmpEntryList[k++] = idToEntry[i];
            }
        }
    }
    return tmpEntryList[pos];
}

macHoppingSequenceListEntry *macHoppingSequenceList::getEntryById(int id)
{
    id -= ENTRYIDS_START;
    return (id < 0 || id >= (int) idToEntry.size()) ? NULL : idToEntry[id];
}

bool macHoppingSequenceList::addEntry(macHoppingSequenceListEntry *entry)
{
    if (!nb)
        throw cRuntimeError("HoppingSequenceList must precede for all Hopping Sequence Entries.");

    //insert Entry
    entry->setHoppingSequenceId(ENTRYIDS_START + idToEntry.size());

    //check name is unique
    if (getEntryById(entry->getHoppingSequenceId()) != NULL)
        throw cRuntimeError("addEntry(): Entry '%d' already registered", entry->getHoppingSequenceId());

    idToEntry.push_back(entry);
    invalidTmpHoppingSequenceList();

    nb->fireChangeNotification(NF_HOPSEQ_CREATED, entry);
    updateDisplayString();
    return true;
}

void macHoppingSequenceList::deleteEntry(macHoppingSequenceListEntry *entry)
{
    int id = entry->getHoppingSequenceId();

    if (entry != getEntryById(id))
        throw cRuntimeError("deleteEntry(): Entry '%d' not found in hopping sequence table", entry->getHoppingSequenceId());

    nb->fireChangeNotification(NF_HOPSEQ_DELETED, entry);
    idToEntry[id - ENTRYIDS_START] = NULL;
    delete entry;
    invalidTmpHoppingSequenceList();
    updateDisplayString();
}

void macHoppingSequenceList::editHoppingSequenceListEntry(macHoppingSequenceListEntry *entry)
{
    std::string nodename = getParentModule()->getFullName();
    bool tmp = false;

    if (getEntryById(entry->getHoppingSequenceId()) == NULL)
    {
        if (ev.isGUI())
            EV << nodename << "not possible to edit entry:"
                << entry->getHoppingSequenceId() << ", Entry doesn't exist!" << endl;

        return;
    }

    //Notification for change in Entry
    nb->fireChangeNotification(NF_HOPSEQ_CONFIG_CHANGED, getEntryById(entry->getHoppingSequenceId()));

    //change channelPage if different and if not 0
    if (entry->getChannelPage() != getEntryById(entry->getHoppingSequenceId())->getChannelPage() && entry->getChannelPage() != 0)
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Entry: " << entry->getHoppingSequenceId() << "; changed ChannelPage" << endl;

        getEntryById(entry->getHoppingSequenceId())->setChannelPage(entry->getChannelPage());
        tmp = true;
    }

    //change NumberOfChannels if different and if not 0
    if (entry->getNumberOfChannels() != getEntryById(entry->getHoppingSequenceId())->getNumberOfChannels() && entry->getNumberOfChannels() != 0)
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Entry: " << entry->getHoppingSequenceId() << "; changed NumberOfChannels" << endl;

        getEntryById(entry->getHoppingSequenceId())->setNumberOfChannels(entry->getNumberOfChannels());
        tmp = true;
    }

    //change phyConfiguration if different and if not 0
    if (entry->getPhyConfiguration() != getEntryById(entry->getHoppingSequenceId())->getPhyConfiguration() && entry->getPhyConfiguration() != 0)
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Entry: " << entry->getHoppingSequenceId() << "; changed PhyConfiguration" << endl;

        getEntryById(entry->getHoppingSequenceId())->setPhyConfiguration(entry->getPhyConfiguration());
        tmp = true;
    }

    //change ExtendedBitmap if different and if not 0
    if (entry->getExtendedBitmap() != getEntryById(entry->getHoppingSequenceId())->getExtendedBitmap() && entry->getExtendedBitmap() != 0)
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Entry: " << entry->getHoppingSequenceId() << "; changed ExtendedBitmap" << endl;

        getEntryById(entry->getHoppingSequenceId())->setExtendedBitmap(entry->getExtendedBitmap());
        tmp = true;
    }

    //change hoppingSequenceLength if different and if not 0
    if (entry->getHoppingSequenceLength() != getEntryById(entry->getHoppingSequenceId())->getHoppingSequenceLength() && entry->getHoppingSequenceLength() != 0)
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Entry: " << entry->getHoppingSequenceId() << "; changed HoppingSequenceId" << endl;

        getEntryById(entry->getHoppingSequenceId())->setHoppingSequenceId(entry->getHoppingSequenceId());
        tmp = true;
    }

    //change hoppingSequenceList if different and if not NULL
    if (entry->getHoppingSequenceList().size() != getEntryById(entry->getHoppingSequenceId())->getHoppingSequenceList().size()
            && entry->getHoppingSequenceLength() != entry->getHoppingSequenceList().size()
            && entry->getHoppingSequenceList().size() != 0)
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Entry: " << entry->getHoppingSequenceId() << "; changed HoppingSequenceList" << endl;

        getEntryById(entry->getHoppingSequenceId())->setHoppingSequenceList(entry->getHoppingSequenceList());
        tmp = true;
    }

    // change CurrentHop if different and if not -1
    if (entry->getCurrentHop() != getEntryById(entry->getHoppingSequenceId())->getCurrentHop() && entry->getCurrentHop() != 0)
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Entry: " << entry->getHoppingSequenceId() << "; changed CurrentHop" << endl;

        getEntryById(entry->getHoppingSequenceId())->setCurrentHop(entry->getCurrentHop());
        tmp = true;
    }

    //change AckWait if different and if not -1
    if (entry->getHopDwellTime() != getEntryById(entry->getHoppingSequenceId())->getHopDwellTime() && entry->getHopDwellTime() != 0)
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Entry: " << entry->getHoppingSequenceId() << "; changed HopDwellTime" << endl;

        getEntryById(entry->getHoppingSequenceId())->setHopDwellTime(entry->getHopDwellTime());
        tmp = true;
    }

    if (tmp)
    {
        nb->fireChangeNotification(NF_HOPSEQ_CONFIG_CHANGED, getEntryById(entry->getHoppingSequenceId()));
    }
    else if (ev.isGUI())
    {
        EV << nodename << ": Edited Entry: " << entry->getHoppingSequenceId() << "; Nothing changed" << endl;
    }
}

void macHoppingSequenceList::entryChanged(macHoppingSequenceListEntry *entry, int category)
{
    nb->fireChangeNotification(category,entry);

    if (ev.isGUI())
        updateDisplayString();
}

void macHoppingSequenceList::resetEntries()
{
    int n = idToEntry.size();

    for (int i = 0; i < n; i++)
    {
        if (idToEntry[i])
            idToEntry[i]->resetEntries();
    }
}

void macHoppingSequenceList::invalidTmpHoppingSequenceList()
{
    tmpNumEntries = -1;
    delete[] tmpEntryList;
    tmpEntryList = NULL;
}

void macHoppingSequenceList::clearTable()
{
    while(idToEntry.size() != 0)
	idToEntry.erase(idToEntry.begin());
}

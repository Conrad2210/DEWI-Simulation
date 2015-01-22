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
 * @author: Conrad Dandelski
 * Copyright(c) 2014: Conrad Dandelski
 * Copyright(c) 2014: Nimbus Centre for Embedded Systems Research, Cork, Ireland
 * Copyright(c) 2014: Cork Institute of Technology, Cork, Ireland
 *
 * @author: 2014    Stefan Reis     (modified - included the code from the [CD] MAC.cc)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <algorithm>
#include <sstream>

#include "macSlotframeTable.h"

#include "IInterfaceTable.h"
#include "InterfaceTableAccess.h"
#include "NotificationBoard.h"
#include "NotifierConsts.h"
#include "macSlotframeTableParser.h"
#include "macSlotframeTableEntry.h"
#include "Ieee802154eDefSlotframe.h"
#include "opp_utils.h"

Define_Module(macSlotframeTable);

#define SLOTFRAMEIDS_START 0

std::ostream& operator<<(std::ostream& os, const macSlotframeTableEntry& e)
{
    os << e.info();
    return os;
}

macSlotframeTable::macSlotframeTable()
{
    //@author Conrad Dandelski
    nb = NULL;
    tmpNumSlotframes = -1;
    tmpSlotframeList = NULL;
}

macSlotframeTable::~macSlotframeTable()
{
    //@author Conrad Dandelski
    for (int i = 0; i < (int) idToSlotframe.size(); i++)
        delete idToSlotframe[i];

    delete[] tmpSlotframeList;
}

void macSlotframeTable::initialize(int stage)
{
    //@author Conrad Dandelski
    //@author: 2014     Stefan Reis     (modified)
    if (stage == 0)
    {
        // get a pointer to the NotificationBoard module and IInterfaceTable
        nb = NotificationBoardAccess().get();
        ift = InterfaceTableAccess().get();

        readFromFile = par("readSlotframesFromFile");

        nb->subscribe(this, NF_SLOTFRAME_CONFIG_CHANGED);
        nb->subscribe(this, NF_SLOTFRAME_CREATED);
        nb->subscribe(this, NF_SLOTFRAME_DELETED);
        nb->subscribe(this, NF_SLOTFRAME_STATE_CHANGED);
    }
    else if (stage == 1)
    {
        WATCH_PTRVECTOR(idToSlotframe);
    }
    else if (stage == 2)
    {
        if (readFromFile)
        {
            const char* tmpSlotframeFileName = par("slotframeFilename");
            const char* tmpFolderName = par("slotframeTableSubFolder");
            const char* nodeName = getParentModule()->getFullName();
            // file name depending on the nodeName
            std::string temp = (std::string) tmpSlotframeFileName + "_" + nodeName;
            // file name generally for all
            //std::string temp = (std::string) tmpSlotframeFileName;
            const char* slotframeTableFileName = const_cast<char*>(tmpSlotframeFileName);

            // path depending on the number of the nodes
            //pathSlotframeTableFiles = (std::string) tmpFolderName + convertIntToString(numHosts) + "\\"
            //            + (std::string) slotframeTableFileName;

            // use for every node number the some slotframe table
            std::string pathSlotframeTableFiles = (std::string) tmpFolderName + (std::string)slotframeTableFileName;

            const char *filename = pathSlotframeTableFiles.c_str();

            // read routing table file (and interface configuration)
            macSlotframeTableParser parser(ift, this);
            if (*filename && parser.readSlotframeTableFromFile(filename)==-1)
                error("Error reading routing table file %s", filename);
        }

        updateDisplayString();
    }
}

void macSlotframeTable::updateDisplayString()
{
    //@author Conrad Dandelski
    if (!ev.isGUI())
        return;

    char buf[80];
    sprintf(buf, "%d Slotframes", getNumSlotframes());
    getDisplayString().setTagArg("t", 0, buf);
}

void macSlotframeTable::handleMessage(cMessage *msg)
{
    //@author Conrad Dandelski
    throw cRuntimeError("this module doesn't process messages");
}

void macSlotframeTable::receiveChangeNotification(int category, const cObject *details)
{
    //@author Conrad Dandelski
    Enter_Method_Silent();
    printNotificationBanner(category, details);
}

cModule *macSlotframeTable::getHostModule()
{
    //@author Conrad Dandelski
    return findContainingNode(this);
}

int macSlotframeTable::getNumSlotframes()
{
    //@author Conrad Dandelski
    if (tmpNumSlotframes == -1)
    {
        //count non-NULL elements
        int n = 0;
        int maxId = idToSlotframe.size();
        for (int i = 0; i < maxId; i++)
            n++;

        tmpNumSlotframes = n;
    }
    return tmpNumSlotframes;
}

macSlotframeTableEntry *macSlotframeTable::getSlotframe(int pos)
{
    //@author Conrad Dandelski
    int n = getNumSlotframes(); //also fills tmpSlotframeList[]

    if (pos < 0 || pos >= n)
        throw cRuntimeError("getSlotframe(): slotframe index %d out of range 0..%d", pos, n - 1);

    if (!tmpSlotframeList)
    {
        tmpSlotframeList = new macSlotframeTableEntry *[n];
        int k = 0;
        int maxId = idToSlotframe.size();

        for (int i = 0; i < maxId; i++)
        {
            if (idToSlotframe[i])
            {
                tmpSlotframeList[k++] = idToSlotframe[i];
            }
        }
    }
    return tmpSlotframeList[pos];

}

macSlotframeTableEntry *macSlotframeTable::getSlotframeById(int id)
{
    //@author Conrad Dandelski
    id -= SLOTFRAMEIDS_START;

//    //only if slotframe is initialized
//    if((int)idToSlotframe.size() > (id))
//    {
//    int n = getNumSlotframes(); //also fills tmpSlotframeList[]
//
//        if (id < 0 || id >= n)
//             throw cRuntimeError("getSlotframeById(): slotframe index %d doesn't exist in this SlotframeTable", id, n - 1);
//    }

    return (id < 0 || id >= (int) idToSlotframe.size()) ? NULL : idToSlotframe[id];
}

bool macSlotframeTable::addSlotframe(macSlotframeTableEntry *entry)
{
    //@author Conrad Dandelski
    //Enter_Method("addSlotframe(...)");
    if (!nb)
        throw cRuntimeError("SlotframeTable must precede all node slotframes");

    entry->setSlotframeId(SLOTFRAMEIDS_START + idToSlotframe.size());

    //check name is unique
    if (getSlotframeById(entry->getSlotframeId()) != NULL)
        throw cRuntimeError("addSlotframe(): slotframe '%d' not found in slotframe table", entry->getSlotframeId());

    idToSlotframe.push_back(entry);
    invalidTmpSlotframeList();

    nb->fireChangeNotification(NF_SLOTFRAME_CREATED, entry);
    updateDisplayString();
    return true;
}

bool macSlotframeTable::deleteSlotframe(macSlotframeTableEntry *entry)
{
    //@author Conrad Dandelski
    int id = entry->getSlotframeId();

    if (entry != getSlotframeById(id))
        throw cRuntimeError("deleteSlotframe(): slotframe '%d' not found in slotframe table", entry->getSlotframeId());

    nb->fireChangeNotification(NF_SLOTFRAME_DELETED, entry);
    idToSlotframe[id - SLOTFRAMEIDS_START] = NULL;
    delete entry;
    invalidTmpSlotframeList();
    return true;
}

void macSlotframeTable::editSlotframe(macSlotframeTableEntry *entry)
{
    std::string nodename = getParentModule()->getFullName();
    bool tmp = false; //variable for checking if something changed
    //check if slotframe exists
    if (getSlotframeById(entry->getSlotframeId()) == NULL)
    {
        if (ev.isGUI())
            EV << nodename << "not possible to edit Slotframe:"
                    << entry->getSlotframeId() << ". Slotframe doesn't exist!!!" << "\n";

        return;
    }

    //Slotframe before
    nb->fireChangeNotification(NF_SLOTFRAME_CONFIG_CHANGED, getSlotframeById(entry->getSlotframeId()));

    //change slotframeSize if different and != -1
    if (entry->getSlotframeSize() != getSlotframeById(entry->getSlotframeId())->getSlotframeSize()
            && entry->getSlotframeSize() != -1)
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Slotframe: "
                    << entry->getSlotframeId() << "; Changed slotframeSize." << endl;
        getSlotframeById(entry->getSlotframeId())->setSlotframeSize(entry->getSlotframeSize());

        tmp = true;
    }

    if (tmp)
    {
        //slotframe after
        nb->fireChangeNotification(NF_SLOTFRAME_CONFIG_CHANGED, getSlotframeById(entry->getSlotframeId()));
    }
    else if (ev.isGUI())
        EV << nodename << ": Edited Slotframe: " << entry->getSlotframeId()
                << "; Nothing changed" << endl;

}

void macSlotframeTable::slotframeChanged(macSlotframeTableEntry *entry, int category)
{
//@author Conrad Dandelski
    nb->fireChangeNotification(category, entry);

    if (ev.isGUI())
        updateDisplayString();
}

void macSlotframeTable::resetSlotframes()
{
//@author Conrad Dandelski
    int n = idToSlotframe.size();

    for (int i = 0; i < n; i++)
    {
        if (idToSlotframe[i])
            idToSlotframe[i]->resetSlotframe();
    }
}

void macSlotframeTable::invalidTmpSlotframeList()
{
//@author Conrad Dandelski
    tmpNumSlotframes = -1;
    delete[] tmpSlotframeList;
    tmpSlotframeList = NULL;
}


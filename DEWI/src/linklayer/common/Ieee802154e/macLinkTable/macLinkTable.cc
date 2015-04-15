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

#include "macLinkTable.h"

#include "IInterfaceTable.h"
#include "InterfaceTableAccess.h"
#include "NotificationBoard.h"
#include "NotifierConsts.h"
#include "macLinkTableParser.h"
#include "macLinkTableEntry.h"
#include "macSlotframeTableAccess.h"
#include "Ieee802154eDefSlotframe.h"
#include "opp_utils.h"

Define_Module(macLinkTable);

#define LINKIDS_START  0

std::ostream& operator<<(std::ostream& os, const macLinkTableEntry& e)
{
    //@author: Conrad Dandelski
    os << e.info();
    return os;
}

macLinkTable::macLinkTable()
{
    //@author: Conrad Dandelski
    nb = NULL;
    tmpNumLinks = -1;
    tmpLinkList = NULL;
}

macLinkTable::~macLinkTable()
{
    //@author: Conrad Dandelski
    while(idToLink.size() != 0)
	idToLink.erase(idToLink.begin());

    delete[] tmpLinkList;
}

void macLinkTable::initialize(int stage)
{
    //@author: Conrad Dandelski
    //@author: 2014     Stefan Reis     (modified)
    if(stage == 0)
    {
	// get a pointer to the NotificationBoard module and IInterfaceTable
	nb = NotificationBoardAccess().get();
	ift = InterfaceTableAccess().get();
	//slotframeTable = MacSlotframeTableAccess().get();

	readFromFile = par("readLinksFromFile");

	nb->subscribe(this, NF_LINK_CREATED);
	nb->subscribe(this, NF_LINK_DELETED);
	nb->subscribe(this, NF_LINK_STATE_CHANGED);
	nb->subscribe(this, NF_LINK_CONFIG_CHANGED);
    }
    else if(stage == 1)
    {
	WATCH_PTRVECTOR(idToLink);
    }
    else if(stage == 2)
    {
	slotframeTable = MacSlotframeTableAccess().get();
	if(readFromFile)
	{
	    const char* tmpLinkFileName = par("linkFilename");
	    const char* tmpFolderName = par("linktableFilesSubFolder");
	    const char* nodeName = getParentModule()->getFullName();
	    // file name depending on the nodeName
	    std::string temp = (std::string)tmpLinkFileName + "_" + nodeName;
	    // file name generally for all
	    //std::string temp = (std::string) tmpSlotframeFileName;
	    const char* linkTableFileName = const_cast<char*>(tmpLinkFileName);

	    // path depending on the number of the nodes
	    //pathSlotframeTableFiles = (std::string) tmpFolderName + convertIntToString(numHosts) + "\\"
	    //            + (std::string) slotframeTableFileName;

	    // use for every node number the some slotframe table
	    std::string pathLinkTableFiles = (std::string)tmpFolderName + (std::string)linkTableFileName;

	    const char *filename = pathLinkTableFiles.c_str();

	    // read routing table file (and interface configuration)
	    macLinkTableParser parser(ift, this);
	    if(*filename && parser.readLinkTableFromFile(filename) == -1)
		error("Error reading routing table file %s", filename);
	}

	updateDisplayString();
    }
}

void macLinkTable::updateDisplayString()
{
    //@author: Conrad Dandelski
    if(!ev.isGUI())
	return;

    char buf[80];
    sprintf(buf, "%d Links", getNumLinks());
    getDisplayString().setTagArg("t", 0, buf);
}

void macLinkTable::handleMessage(cMessage *msg)
{
    //@author: Conrad Dandelski
    throw cRuntimeError("this module doesn't process messages");
}

void macLinkTable::receiveChangeNotification(int category, const cObject *details)
{
    //@author: Conrad Dandelski
    // nothing needed here at the moment
    Enter_Method_Silent
    ();
    printNotificationBanner(category, details);
}

cModule *macLinkTable::getHostModule()
{
    //@author: Conrad Dandelski
    return findContainingNode(this);
}

int macLinkTable::getNumLinks()
{
    //@author: Conrad Dandelski
    if(tmpNumLinks == -1)
    {
	//count non-NULL elements
	int n = 0;
	int maxId = idToLink.size();
	for(int i = 0; i < maxId; i++)
	{
	    n++;
	}
	tmpNumLinks = n;
    }
    return tmpNumLinks;
}
macLinkTableEntry *macLinkTable::getLinkByTimeslotOffsetAddress(int timeslot, int offset, UINT_16 address)
{
    macLinkTableEntry *entry = new macLinkTableEntry();
    for(int i = 0; i < (int)idToLink.size(); i++)
    {
	entry = idToLink.at(i);
	if(entry->getTimeslot() == timeslot && entry->getChannelOffset() == offset && entry->getNodeAddress() == address)
	{
	    return idToLink.at(i);
	}
    }
    return NULL;
}



int macLinkTable::getTimeSlotByOffset(int offset)
{
    for(int i = 0; i < (int) idToLink.size(); i++)
    {
	if(idToLink.at(i)->getChannelOffset() == offset)
	{
	    return idToLink.at(i)->getTimeslot();
	}
    }
    return -1;
}

int macLinkTable::getOffsetByTimeslot(int timeslot)
{
    for(int i = 0; i < (int) idToLink.size(); i++)
    {
	if(idToLink.at(i)->getTimeslot() == timeslot)
	{
	    return idToLink.at(i)->getChannelOffset();
	}
    }
    return -1;
}


macLinkTableEntry *macLinkTable::getLink(int pos)
{
    //@author: Conrad Dandelski
    int n = getNumLinks(); //also fills tmpNumLinks

    if(pos < 0 || pos >= n)
	throw cRuntimeError("getLink(): Link index %d out of range 0..%d", pos, n - 1);

    if(!tmpLinkList)
    {
	tmpLinkList = new macLinkTableEntry *[n];
	int k = 0;
	int maxId = idToLink.size();

	for(int i = 0; i < maxId; i++)
	{
	    if(idToLink[i])
	    {
		tmpLinkList[k++] = idToLink[i];
	    }
	}
    }

    return tmpLinkList[pos];
}

macLinkTableEntry *macLinkTable::getLinkById(int id)
{
    //@author: Conrad Dandelski
    id -= LINKIDS_START;
    return (id < 0 || id >= (int)idToLink.size()) ? NULL:idToLink[id];
}

macLinkTableEntry *macLinkTable::getLinkByTimeslot(int timeslot)
{
    //@author: Conrad Dandelski
    for(int i = 0; i < (int)idToLink.size(); i++)
    {
	if(idToLink[i]->getTimeslot() == timeslot)
	    return idToLink[i];
    }
    return NULL;
}

macLinkTableEntry *macLinkTable::getLinkByTimeslotSlotframe(int timeslot, int slotframe)
{
    //@author: Stefan Reis
    for(int i = 0; i < (int)idToLink.size(); i++)
    {
	if(idToLink[i]->getTimeslot() == timeslot && idToLink[i]->getSlotframeId() == slotframe)
	    return idToLink[i];
    }
    return NULL;
}

//Return links by depending on timeslot and slotframe
void macLinkTable::getLinksByTimeslotSlotframe(int timeslot, int slotframe, std::list<macLinkTableEntry*> *list)
{
    for(int i = 0; i < (int)idToLink.size(); i++)
    {
	if(idToLink[i]->getTimeslot() == timeslot && idToLink[i]->getSlotframeId() == slotframe)
	    list->push_back(idToLink[i]);
    }
}

bool macLinkTable::existLink(UINT_16 address)
{
    //@author: Stefan Reis
    for(int i = 0; i < (int)idToLink.size(); i++)
    {
	if(idToLink[i]->getNodeAddress() == address && (idToLink[i]->getLinkOption() == LNK_OP_TRANSMIT || idToLink[i]->getLinkOption() == LNK_OP_SHARED || idToLink[i]->getLinkOption() == LNK_OP_SHARED_RECEIVE))
	    return true;
    }
    return false;
}

bool macLinkTable::existLink(macLinkTableEntry *link)
{
    for(int i = 0; i < (int)idToLink.size(); i++)
    {
	if(idToLink.at(i)->getNodeAddress() == link->getNodeAddress()
		&& idToLink.at(i)->getLinkType() == link->getLinkType()
		&& idToLink.at(i)->getTimeslot() == link->getTimeslot()
		&& idToLink.at(i)->getChannelOffset() == link->getChannelOffset()
		&& idToLink.at(i)->getLinkOption() == link->getLinkOption())
	    return true;
    }
    return false;
}

macLinkTableEntry *macLinkTable::getLinkByIdAndSlotframe(int LinkId, int SlotframeId)
{
    //@author Conrad Dandelski
    for(int i = 0; i < (int)idToLink.size(); i++)
    {
	if((idToLink[i]->getLinkId() == LinkId) && (idToLink[i]->getSlotframeId() == SlotframeId))
	    return idToLink[i];
    }
    return NULL;
}

macLinkTableEntry *macLinkTable::getLinkByNodeAddress(UINT_16 address)
{
    //@author: Conrad Dandelski
    for(int i = 0; i < (int)idToLink.size(); i++)
    {
	if(idToLink[i]->getNodeAddress() == address)
	    return idToLink[i];
    }
    return NULL;
}

macLinkTableEntry *macLinkTable::getLinkByNodeAddress(UINT_16 address, MACTSCHLinkOptions option)
{
    //@author: Conrad Dandelski
    for(int i = 0; i < (int)idToLink.size(); i++)
    {
	if((idToLink[i]->getNodeAddress() == address) && (idToLink[i]->getLinkOption() == option))
	    return idToLink[i];
    }
    return NULL;
}

int macLinkTable::getNumberLinksBySlotframe(int slotframeId)
{
    int n = 0;
    for(int i = 0; i < (int)this->getNumLinks(); i++)
    {
	if(this->getLink(i)->getSlotframeId() == slotframeId)
	{
	    n++;
	}
    }
    return n;
}

bool macLinkTable::isTimescource(UINT_16 address)
{
    for(int i = 0; i < (int)idToLink.size(); i++)
    {
	if(idToLink[i]->getNodeAddress() == address && idToLink[i]->getLinkOption() == LNK_OP_RECEIVE_TIMEKEEPING)
	    return true;
    }
    return false;
}

LinkTableVector *macLinkTable::getLinksBySlotframeId(int id)
{
    //@author: Conrad Dandelski
    LinkTableVector* tmpVector = NULL;
    for(int i = 0; i < (int)idToLink.size(); i++)
    {
	if(idToLink[i]->getSlotframeId() == id)
	    tmpVector->push_back(idToLink[i]);
    }

    return tmpVector;
}

bool macLinkTable::addLink(macLinkTableEntry *entry)
{
    //@author: Conrad Dandelski
    if(!nb)
	throw cRuntimeError("NotificationBoard must precede all node Linktables");

    //Check if slotframe which the timeslot belongs to exists
    if(slotframeTable->getSlotframeById(entry->getSlotframeId()) == NULL)
    {
	if(ev.isGUI())
	{
	    EV << getParentModule()->getParentModule()->getFullName() << "not possible to add link,  slotframe:" << entry->getSlotframeId() << "\n";
	}
	return false;
    }
    else
    {
	//check Id is set
	if(entry->getLinkId() == -1)
	{
	    if(ev.isGUI())
	    {
		EV << getParentModule()->getParentModule()->getFullName() << " addLink(): no LinkId set, add Link is not possible" << "\n";
	    }
	    return false;
	}

	//check name is unique for the slotframe
	if(getLinkByIdAndSlotframe(entry->getLinkId(), entry->getSlotframeId()) != NULL)
	{
	    if(ev.isGUI())
	    {
		EV << getParentModule()->getParentModule()->getFullName() << "not possible to add link, link already registered for this slotframe" << "\n";
	    }
	    return false;
	}

	//check if timeslot is already used
//        if (getLinkByTimeslot(entry->getTimeslot()) != NULL)
//        {
//            if (ev.isGUI())
//            {
//                EV << getParentModule()->getParentModule()->getFullName()
//                        << "not possible to add link, timeslot already used" << "\n";
//            }
//            return false;
//        }

	//check if timeslot is possible for this slotframe
	if(entry->getTimeslot() > slotframeTable->getSlotframeById(entry->getSlotframeId())->getSlotframeSize())
	{
	    if(ev.isGUI())
	    {
		EV << getParentModule()->getParentModule()->getFullName() << "not possible to add link, is not available for this slotframeId" << "\n";
	    }
	    return false;
	}

	if(existLink(entry))
	{
	    if(ev.isGUI())
	    {
		EV << getParentModule()->getParentModule()->getFullName() << "not possible to add link, identical link already exists" << "\n";
	    }
	    return false;
	}

	idToLink.push_back(entry);
	invalidTmpLinkList();
	updateDisplayString();
	nb->fireChangeNotification(NF_LINK_CREATED, entry);
	return true;
    }
}

bool macLinkTable::deleteLink(macLinkTableEntry *entry)
{
    //@author: Conrad Dandelski
    int id = entry->getLinkId();
    if(entry != getLinkById(id))
	throw cRuntimeError("deleteLink(): link '%d' not found in link table", entry->getLinkId());

    updateDisplayString();
    nb->fireChangeNotification(NF_LINK_DELETED, entry);

    std::vector<macLinkTableEntry *>::iterator it = idToLink.begin(); // = idToLink.at(id - LINKIDS_START);
    for(int i = 0; i < id - LINKIDS_START; i++)
    {
	it++;
    }
    idToLink.erase(it);
//    delete idToLink[id - LINKIDS_START];
//    idToLink[id - LINKIDS_START] = NULL;
//    /delete entry;
    invalidTmpLinkList();
    return true;
}

void macLinkTable::linkChanged(macLinkTableEntry *entry, int category)
{
    //@author: Conrad Dandelski
    nb->fireChangeNotification(category, entry);
    if(ev.isGUI())
	updateDisplayString();
}

void macLinkTable::resetLinks()
{
    //@author: Conrad Dandelski
    int n = idToLink.size();
    for(int i = 0; i < n; i++)
    {
	if(idToLink[i])
	{
	    idToLink[i]->resetLink();
	}
    }
}

void macLinkTable::invalidTmpLinkList()
{
    //@author: Conrad Dandelski
    tmpNumLinks = -1;
    delete[] tmpLinkList;
    tmpLinkList = NULL;
}

void macLinkTable::editLink(macLinkTableEntry *entry)
{
    bool tmp = false; //variable for checking if something changed
    std::string nodename = getParentModule()->getFullName();

    //check if link exists
    if(getLinkById(entry->getLinkId()) == NULL)
    {
	if(ev.isGUI())
	    EV << nodename << "not possible to edit link:" << entry->getLinkId() << ", because Link doesn't exist" << "\n";

	return;
    }

    //Link before
    nb->fireChangeNotification(NF_LINK_CONFIG_CHANGED, getLinkById(entry->getLinkId()));

    //change channeloffset if different and if set
    if(entry->getChannelOffset() != getLinkById(entry->getLinkId())->getChannelOffset() && entry->getChannelOffset() != -1)
    {
	if(ev.isGUI())
	    EV << nodename << ": Edited link: " << entry->getLinkId() << "; Changed channel offset" << endl;

	getLinkById(entry->getLinkId())->setChannelOffset(entry->getChannelOffset());
	tmp = true;
    }

    //change link option if different and if not undefined
    if(entry->getLinkOption() != getLinkById(entry->getLinkId())->getLinkOption() && entry->getLinkOption() != LNK_OP_undefinedOption)
    {
	EV << nodename << ": Edited link: " << entry->getLinkId() << "; Changed Link Option" << endl;
	getLinkById(entry->getLinkId())->setLinkOption(entry->getLinkOption());
	tmp = true;
    }

    //change link type if different and if not undefined
    if(entry->getLinkType() != getLinkById(entry->getLinkId())->getLinkType() && entry->getLinkType() != LNK_TP_undefinedType)
    {
	EV << nodename << ": Edited link: " << entry->getLinkId() << "; Changed link type" << endl;
	getLinkById(entry->getLinkId())->setLinkType(entry->getLinkType());
	tmp = true;
    }

    //change node address if different and if 0x00
    if(entry->getNodeAddress() != getLinkById(entry->getLinkId())->getNodeAddress() && entry->getNodeAddress() != 0x00)
    {
	EV << nodename << ": Edited link: " << entry->getLinkId() << "; Changed nodeAddress" << endl;
	getLinkById(entry->getLinkId())->setNodeAddress(entry->getNodeAddress());
	tmp = true;
    }

    //change slotframe id if different and if not -1
    if(entry->getSlotframeId() != getLinkById(entry->getLinkId())->getSlotframeId() && entry->getSlotframeId() != -1)
    {
	EV << nodename << ": Edited link: " << entry->getLinkId() << "; Changed SlotframeId" << endl;
	getLinkById(entry->getLinkId())->setSlotframeId(entry->getSlotframeId());
	tmp = true;
    }

    //change timeslot if different and if not -1
    if(entry->getTimeslot() != getLinkById(entry->getLinkId())->getTimeslot() && entry->getTimeslot() != -1)
    {
	EV << nodename << ": Edited link: " << entry->getLinkId() << "; Changed Timslot" << endl;
	getLinkById(entry->getLinkId())->setTimeslot(entry->getTimeslot());
	tmp = true;
    }

    if(tmp)
    {
	nb->fireChangeNotification(NF_LINK_CONFIG_CHANGED, getLinkById(entry->getLinkId()));
    }
    else if(ev.isGUI())
    {
	EV << nodename << ": Edited Link: " << entry->getLinkId() << "; Nothing changed" << endl;
    }
}

void macLinkTable::clearTable()
{
    while(idToLink.size() != 0)
	idToLink.erase(idToLink.begin());
}

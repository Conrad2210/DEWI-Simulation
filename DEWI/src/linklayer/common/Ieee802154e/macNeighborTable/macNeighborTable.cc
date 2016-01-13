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

#include "macNeighborTable.h"

#include "IInterfaceTable.h"
#include "InterfaceTableAccess.h"
#include "NotificationBoard.h"
#include "NotifierConsts.h"
#include "macNeighborTableParser.h"
#include "macNeighborTableEntry.h"
#include "Ieee802154eDefSlotframe.h"
#include "opp_utils.h"

Define_Module(macNeighborTable);

#define NEIGHBORIDS_START 0

std::ostream& operator<<(std::ostream& os, const macNeighborTableEntry& e)
{
    os << e.info();
    return os;
}

macNeighborTable::macNeighborTable()
{
    nb = NULL;
    tmpNumNeighbors = -1;
    tmpNeighborList = NULL;
}

macNeighborTable::~macNeighborTable()
{
    while (idToNeighbor.size() != 0)
        idToNeighbor.erase(idToNeighbor.begin());

    delete[] tmpNeighborList;
}

void macNeighborTable::initialize(int stage)
{
    //@author: 2014     Stefan Reis
    if (stage == 0)
    {
        // get a pointer to the NotificationBoard module and IInterfaceTable
        nb = NotificationBoardAccess().get();
        ift = InterfaceTableAccess().get();

        readFromFile = par("readNeighborFromFile");

        nb->subscribe(this, NF_NEIGHBOR_CONFIG_CHANGED);
        nb->subscribe(this, NF_NEIGHBOR_CREATED);
        nb->subscribe(this, NF_NEIGHBOR_DELETED);
        nb->subscribe(this, NF_NEIGHBOR_STATE_CHANGED);
    }
    else if (stage == 1)
    {
        WATCH_PTRVECTOR(idToNeighbor);
    }
    else if (stage == 2)
    {
        if (readFromFile)
        {
            const char* tmpNeighborFileName = par("NeighborFilename");
            const char* tmpFolderName = par("NeighborTableSubFolder");
            // file name depending on the nodeName
            //std::string temp = (std::string) tmpNeighborFileName + "_" + nodeName;
            // file name generally for all
            std::string temp = (std::string) tmpNeighborFileName;
            const char* NeighborTableFileName = const_cast<char*>(temp.c_str());

            // path depending on the number of the nodes
            //pathNeighborTableFiles = (std::string) tmpFolderName + convertIntToString(numHosts) + "\\"
            //            + (std::string) NeighborTableFileName;

            // use for every node number the some Neighbor table
            std::string pathNeighborTableFiles = (std::string) tmpFolderName + (std::string) NeighborTableFileName;

            const char *filename = pathNeighborTableFiles.c_str();

            // read routing table file (and interface configuration)
            macNeighborTableParser parser(ift, this);
            if (*filename && parser.readNeighborTableFromFile(filename) == -1)
                error("Error reading routing table file %s", filename);
        }

        updateDisplayString();
    }
}

void macNeighborTable::updateDisplayString()
{
    if (!ev.isGUI())
        return;

    char buf[80];
    sprintf(buf, "%d Neighbors", getNumNeighbors());
    getDisplayString().setTagArg("t", 0, buf);
}

void macNeighborTable::handleMessage(cMessage *msg)
{
    throw cRuntimeError("this module doesn't process messages");
}

void macNeighborTable::receiveChangeNotification(int category, const cObject *details)
{
    Enter_Method_Silent
    ();
    printNotificationBanner(category, details);
}

cModule *macNeighborTable::getHostModule()
{
    return findContainingNode(this);
}

int macNeighborTable::getNumNeighbors()
{
    if (tmpNumNeighbors == -1)
    {
        //count non-NULL elements
        int n = 0;
        int maxId = idToNeighbor.size();
        for (int i = 0; i < maxId; i++)
            n++;

        tmpNumNeighbors = n;
    }
    return tmpNumNeighbors;
}

macNeighborTableEntry *macNeighborTable::getNeighborByPos(int pos)
{
    int n = getNumNeighbors(); //also fills tmpNeighborList[]

    if (pos < 0 || pos >= n)
        throw cRuntimeError("getNeighbor(): Neighbor index %d out of range 0..%d", pos, n - 1);

    if (!tmpNeighborList)
    {
        tmpNeighborList = new macNeighborTableEntry *[n];
        int k = 0;
        int maxId = idToNeighbor.size();

        for (int i = 0; i < maxId; i++)
        {
            if (idToNeighbor[i])
            {
                tmpNeighborList[k++] = idToNeighbor[i];
            }
        }
    }
    return tmpNeighborList[pos];
}
macNeighborTableEntry *macNeighborTable::getNeighborByPos(int pos, bool filltmpNeighborList)
{
    if (!filltmpNeighborList)
    {

        if (idToNeighbor.at(pos))
        {
            return idToNeighbor.at(pos);
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        int n = getNumNeighbors(); //also fills tmpNeighborList[]

        if (pos < 0 || pos >= n)
            throw cRuntimeError("getNeighbor(): Neighbor index %d out of range 0..%d", pos, n - 1);

        if (!tmpNeighborList)
        {
            tmpNeighborList = new macNeighborTableEntry *[n];
            int k = 0;
            int maxId = idToNeighbor.size();

            for (int i = 0; i < maxId; i++)
            {
                if (idToNeighbor[i])
                {
                    tmpNeighborList[k++] = idToNeighbor[i];
                }
            }
        }
        return tmpNeighborList[pos];
    }
}

macNeighborTableEntry *macNeighborTable::getNeighborBySAddr(UINT_16 address)
{
    if (!tmpNeighborList)
    {
        for (int i = 0; i < (int) idToNeighbor.size(); i++)
        {
            if (idToNeighbor[i]->getShortAddress() == address)
                return idToNeighbor[i];
        }
    }
    return NULL;
}

macNeighborTableEntry *macNeighborTable::getNeighborByEAddr(MACAddress address)
{
  //  if (!tmpNeighborList)
  //  {
        for (int i = 0; idToNeighbor.size(); i++)
        {
            if(idToNeighbor[i]->getExtendedAddress().compareTo(address) == 0)
            {
                return idToNeighbor[i];
            }
//            if (idToNeighbor[i]->getExtendedAddress().getInt() == address.getInt())
//                return idToNeighbor[i];
        }
    //}
    return NULL;
}

macNeighborTableEntry *macNeighborTable::getNeighborById(int id)
{
    id -= NEIGHBORIDS_START;
    return (id < 0 || id >= (int) idToNeighbor.size()) ? NULL : idToNeighbor[id];
}

bool macNeighborTable::isNeighborBySAddr(UINT_16 address)
{
    return getNeighborBySAddr(address) != NULL ? true : false;
}

bool macNeighborTable::isNeighborByEAddr(MACAddress address)
{
    return getNeighborByEAddr(address) != NULL ? true : false;
}

bool macNeighborTable::isNeighborTimeSource(UINT_16 address)
{
    macNeighborTableEntry* tmpEntry = getNeighborBySAddr(address);
    if (tmpEntry != NULL)
    {
        return tmpEntry->getIsTimeSource();
    }
    else
        return false;
}

bool macNeighborTable::addNeighbor(macNeighborTableEntry *entry)
{
    if (!nb)
        throw cRuntimeError("NeighborTable must precede all node Neighbors");

    entry->setNeighborId(NEIGHBORIDS_START + idToNeighbor.size());

    //check name is unique
    if (getNeighborById(entry->getNeighborId()) != NULL)
        throw cRuntimeError("addNeighbor(): Neighbor '%d' not found in Neighbor table", entry->getNeighborId());

    idToNeighbor.push_back(entry);
    invalidTmpNeighborList();

    nb->fireChangeNotification(NF_NEIGHBOR_CREATED, entry);
    updateDisplayString();
    return true;
}

bool macNeighborTable::deleteNeighbor(macNeighborTableEntry *entry)
{
    int id = entry->getNeighborId();

    if (entry != getNeighborById(id))
        throw cRuntimeError("deleteNeighbor(): Neighbor '%d' not found in Neighbor table", entry->getNeighborId());

    nb->fireChangeNotification(NF_NEIGHBOR_DELETED, entry);
    idToNeighbor.erase(idToNeighbor.begin() + (id - NEIGHBORIDS_START));
    delete entry;
    invalidTmpNeighborList();
    return true;
}

void macNeighborTable::editNeighbor(macNeighborTableEntry *entry)
{
    std::string nodename = getParentModule()->getFullName();
    bool tmp = false; //variable for checking if something changed
    //check if Neighbor exists
    if (getNeighborById(entry->getNeighborId()) == NULL)
    {
        if (ev.isGUI())
            EV << nodename << "not possible to edit Neighbor:" << entry->getNeighborId()
                    << ". Neighbor doesn't exist!!!" << "\n";

        return;
    }

    //Neighbor before
    nb->fireChangeNotification(NF_NEIGHBOR_CONFIG_CHANGED, getNeighborById(entry->getNeighborId()));

    //change NeighborSize if different and != -1
    if (entry->getNeighborSize() != getNeighborById(entry->getNeighborId())->getNeighborSize()
            && entry->getNeighborSize() != -1)
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Neighbor: " << entry->getNeighborId() << "; Changed NeighborSize." << endl;
        getNeighborById(entry->getNeighborId())->setNeighborSize(entry->getNeighborSize());
        tmp = true;
    }

    //change ShortAddress if different
    if (entry->getShortAddress() != getNeighborById(entry->getNeighborId())->getShortAddress())
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Neighbor: " << entry->getNeighborId() << "; Changed ShortAddress." << endl;
        getNeighborById(entry->getNeighborId())->setShortAddress(entry->getShortAddress());
        tmp = true;
    }

    //change ExtendedAddress if different
    if (entry->getExtendedAddress() != getNeighborById(entry->getNeighborId())->getExtendedAddress())
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Neighbor: " << entry->getNeighborId() << "; Changed ExtendedAddress." << endl;
        getNeighborById(entry->getNeighborId())->setExtendedAddress(entry->getExtendedAddress());
        tmp = true;
    }

    //change SD-Index if different
    if (entry->getSDIndex() != getNeighborById(entry->getNeighborId())->getSDIndex())
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Neighbor: " << entry->getNeighborId() << "; Changed SD-Index." << endl;
        getNeighborById(entry->getNeighborId())->setSDIndex(entry->getSDIndex());
        tmp = true;
    }

    //change ChannelOffset if different
    if (entry->getChannelOffset() != getNeighborById(entry->getNeighborId())->getChannelOffset())
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Neighbor: " << entry->getNeighborId() << "; Changed ChannelOffset." << endl;
        getNeighborById(entry->getNeighborId())->setChannelOffset(entry->getChannelOffset());
        tmp = true;
    }

    //change TrackBeacon if different
    if (entry->getTrackBeacon() != getNeighborById(entry->getNeighborId())->getTrackBeacon())
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Neighbor: " << entry->getNeighborId() << "; Changed TrackBeacon." << endl;
        getNeighborById(entry->getNeighborId())->setTrackBeacon(entry->getTrackBeacon());
        tmp = true;
    }

    //change BeaconLostCount if different
    if (entry->getBeaconLostCount() != getNeighborById(entry->getNeighborId())->getBeaconLostCount())
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Neighbor: " << entry->getNeighborId() << "; Changed BeaconLostCount." << endl;
        getNeighborById(entry->getNeighborId())->setBeaconLostCount(entry->getBeaconLostCount());
        tmp = true;
    }

    //change NumTxData if different
    if (entry->getNumTxData() != getNeighborById(entry->getNeighborId())->getNumTxData())
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Neighbor: " << entry->getNeighborId() << "; Changed NumTxData." << endl;
        getNeighborById(entry->getNeighborId())->setNumTxData(entry->getNumTxData());
        tmp = true;
    }

    //change NumTxDataAck if different
    if (entry->getNumTxDataAck() != getNeighborById(entry->getNeighborId())->getNumTxDataAck())
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Neighbor: " << entry->getNeighborId() << "; Changed NumTxDataAck." << endl;
        getNeighborById(entry->getNeighborId())->setNumTxDataAck(entry->getNumTxDataAck());
        tmp = true;
    }

    //change NumRxData if different
    if (entry->getNumRxData() != getNeighborById(entry->getNeighborId())->getNumRxData())
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Neighbor: " << entry->getNeighborId() << "; Changed NumRxData." << endl;
        getNeighborById(entry->getNeighborId())->setNumRxData(entry->getNumRxData());
        tmp = true;
    }

    //change NumRxDataAck if different
    if (entry->getNumRxDataAck() != getNeighborById(entry->getNeighborId())->getNumRxDataAck())
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Neighbor: " << entry->getNeighborId() << "; Changed NumRxDataAck." << endl;
        getNeighborById(entry->getNeighborId())->setNumRxDataAck(entry->getNumRxDataAck());
        tmp = true;
    }

    //change LastASN if different
    if (entry->getLastASN() != getNeighborById(entry->getNeighborId())->getLastASN())
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Neighbor: " << entry->getNeighborId() << "; Changed LastASN." << endl;
        getNeighborById(entry->getNeighborId())->setLastASN(entry->getLastASN());
        tmp = true;
    }

    //change RPLrank if different
    if (entry->getRPLrank() != getNeighborById(entry->getNeighborId())->getRPLrank())
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Neighbor: " << entry->getNeighborId() << "; Changed RPLrank." << endl;
        getNeighborById(entry->getNeighborId())->setRPLrank(entry->getRPLrank());
        tmp = true;
    }

    //change IsTimeSource if different
    if (entry->getIsTimeSource() != getNeighborById(entry->getNeighborId())->getIsTimeSource())
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Neighbor: " << entry->getNeighborId() << "; Changed IsTimeSource." << endl;
        getNeighborById(entry->getNeighborId())->setIsTimeSource(entry->getIsTimeSource());
        tmp = true;
    }

    //change RPL_OF if different
    if (entry->getRPL_OF() != getNeighborById(entry->getNeighborId())->getRPL_OF())
    {
        if (ev.isGUI())
            EV << nodename << ": Edited Neighbor: " << entry->getNeighborId() << "; Changed RPL_OF." << endl;
        getNeighborById(entry->getNeighborId())->setRPL_OF(entry->getRPL_OF());
        tmp = true;
    }

    if (tmp)
    {
        //Neighbor after
        nb->fireChangeNotification(NF_NEIGHBOR_CONFIG_CHANGED, getNeighborById(entry->getNeighborId()));
    }
    else if (ev.isGUI())
        EV << nodename << ": Edited Neighbor: " << entry->getNeighborId() << "; Nothing changed" << endl;

}

void macNeighborTable::neighborChanged(macNeighborTableEntry *entry, int category)
{
    nb->fireChangeNotification(category, entry);

    if (ev.isGUI())
        updateDisplayString();
}

void macNeighborTable::resetNeighbors()
{
    int n = idToNeighbor.size();

    for (int i = 0; i < n; i++)
    {
        if (idToNeighbor[i])
            idToNeighbor[i]->resetNeighbor();
    }
}

void macNeighborTable::invalidTmpNeighborList()
{
    tmpNumNeighbors = -1;
    delete[] tmpNeighborList;
    tmpNeighborList = NULL;
}

MACAddress macNeighborTable::getAddressFromCH()
{
    for (int i = 0; i < (int) idToNeighbor.size(); i++)
    {
        if (idToNeighbor.at(i)->isMyCH())
            return idToNeighbor.at(i)->getExtendedAddress();
    }
    return MACAddress::UNSPECIFIED_ADDRESS;
}

void macNeighborTable::clearTable()
{
    while (idToNeighbor.size() != 0)
        idToNeighbor.erase(idToNeighbor.begin());
}

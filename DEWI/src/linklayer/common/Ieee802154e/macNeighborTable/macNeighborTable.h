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

#ifndef MACNEIGHBORTABLE_H_
#define MACNEIGHBORTABLE_H_

#include <vector>

#include "INETDefs.h"

#include "INotifiable.h"
#include "IMacNeighborTable.h"
#include "macNeighborTableEntry.h"
#include "Ieee802154eDefSlotframe.h"

class IInterfaceTable;
class NotificationBoard;
class macNeighborTableParser;

class INET_API macNeighborTable: public cSimpleModule, public IMacNeighborTable, protected INotifiable
{
    //member Variables
    protected:
        IInterfaceTable *ift; // cached pointer
        NotificationBoard *nb; // cached pointer

        bool readFromFile;

        NeighborTableVector idToNeighbor;

        //fields to support getNumNeighbors() and getNeighbor(pos)
        int tmpNumNeighbors; //caches number of non-NULL elements of idToNeighbor; -1 if invalid
        macNeighborTableEntry **tmpNeighborList; // caches non-NULL elements of idToNeighbor; NULL if invalid

    protected:
        //display summary above icon
        virtual void updateDisplayString();

        //internal
        virtual void invalidTmpNeighborList();

        virtual void resetNeighbors();

    public:
        macNeighborTable();
        virtual ~macNeighborTable();
        virtual std::string getFullPath() const {return cSimpleModule::getFullPath();}

    protected:
        virtual int numInitStages() const { return 3;}

        virtual void initialize(int stage);

        virtual void handleMessage(cMessage *msg);

    public:
        //Called by the NotificationBoard whenever a change of a category
        //occurs to which this client subscribed
        virtual void receiveChangeNotification(int category, const cObject *details);

        //returns the host this table lives in
        virtual cModule *getHostModule();

        //Adds an Neighbor to table
        virtual bool addNeighbor(macNeighborTableEntry *entry);

        //edits a Neighbor in table
        virtual void editNeighbor(macNeighborTableEntry *entry);

        //deletes an Neighbor from table
        virtual bool deleteNeighbor(macNeighborTableEntry *entry);

        //returns the number of Neighbors
        virtual int getNumNeighbors();

        //called from macNeighborTableEntry
        virtual void neighborChanged(macNeighborTableEntry *entry, int category);

        //Returns the Neighbor
        virtual macNeighborTableEntry *getNeighborByPos(int pos);
        virtual macNeighborTableEntry *getNeighborByPos(int pos, bool fillTmpNeighborList);
        virtual macNeighborTableEntry *getNeighborBySAddr(UINT_16 address);
        virtual macNeighborTableEntry *getNeighborByEAddr(MACAddress address);
        virtual macNeighborTableEntry *getNeighborById(int id);
        virtual MACAddress getAddressFromCH();

        virtual bool isNeighborBySAddr(UINT_16 address);
        virtual bool isNeighborByEAddr(MACAddress address);
        virtual bool isNeighborTimeSource(UINT_16 address);

	//deletes all entries
	virtual void clearTable();

};

#endif /* MACNEIGHBORTABLE_H_ */

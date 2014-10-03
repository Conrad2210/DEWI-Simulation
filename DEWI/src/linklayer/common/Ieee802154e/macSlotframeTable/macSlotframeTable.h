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
 */

#ifndef MACSLOTFRAMETABLE_H_
#define MACSLOTFRAMETABLE_H_

#include <vector>

#include "INETDefs.h"

#include "INotifiable.h"
#include "IMacSlotframeTable.h"
#include "macSlotframeTableEntry.h"
#include "Ieee802154eDefSlotframe.h"

class IInterfaceTable;
class NotificationBoard;
class macSlotframeTableParser;

class INET_API macSlotframeTable: public cSimpleModule, public IMacSlotframeTable, protected INotifiable
{
    //member Variables
    protected:
        IInterfaceTable *ift; // cached pointer
        NotificationBoard *nb; // cached pointer

        bool readFromFile;

        SlotframeTableVector idToSlotframe;

        //fields to support getNumSlotframes() and getSlotframe(pos)
        int tmpNumSlotframes; //caches number of non-NULL elements of idToSlotframe; -1 if invalid
        macSlotframeTableEntry **tmpSlotframeList; // caches non-NULL elements of idToSlotframe; NULL if invalid

    protected:
        //display summary above icon
        virtual void updateDisplayString();

        //internal
        virtual void invalidTmpSlotframeList();

        virtual void resetSlotframes();

    public:
        macSlotframeTable();
        virtual ~macSlotframeTable();
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

        //Adds an Slotframe to table
        virtual bool addSlotframe(macSlotframeTableEntry *entry);

        //edits a Slotframe in table
        virtual void editSlotframe(macSlotframeTableEntry *entry);

        //deletes an Slotframe from table
        virtual bool deleteSlotframe(macSlotframeTableEntry *entry);

        //returns the number of slotframes
        virtual int getNumSlotframes();

        //called from macSlotframeTableEntry
        virtual void slotframeChanged(macSlotframeTableEntry *entry, int category);

        //Returns the Slotframe
        virtual macSlotframeTableEntry *getSlotframe(int pos);

        //Returns slotframe by id
        virtual macSlotframeTableEntry *getSlotframeById(int id);
};

#endif /* MACSLOTFRAMETABLE_H_ */

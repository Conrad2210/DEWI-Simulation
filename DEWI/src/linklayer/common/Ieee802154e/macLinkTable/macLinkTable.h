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
 * @author: 2014    Stefan Reis     (modified - to solve the including problem)
 */

#ifndef MACLINKTABLE_H_
#define MACLINKTABLE_H_

#include <vector>

#include "INETDefs.h"

#include "INotifiable.h"
#include "IMacLinkTable.h"
#include "macLinkTableEntry.h"
#include "Ieee802154eDefSlotframe.h"

#include "IMacSlotframeTable.h"

class IInterfaceTable;
class NotificationBoard;
class macLinkTableParser;
class IMacSlotframeTable;

class INET_API macLinkTable : public cSimpleModule, public IMacLinkTable, protected INotifiable
{
        //member variables
    protected:
        IInterfaceTable *ift; // cached pointer
        NotificationBoard *nb; // cached pointer
        IMacSlotframeTable *slotframeTable; // cached pointer

        bool readFromFile;

        LinkTableVector idToLink;

        //fields to support getNumLinks() and getLink(pos)
        int tmpNumLinks; //caches number of non-Null elements of idToLinks; -1 if invalid
        macLinkTableEntry **tmpLinkList; //caches non-Null elements of idToLinks; NULL if invalid

    protected:
        //display summary above the icon
        virtual void updateDisplayString();

        //internal
        virtual void invalidTmpLinkList();

        virtual void resetLinks();
    public:
        macLinkTable();
        virtual ~macLinkTable();
        virtual std::string getFullPath() const {return cSimpleModule::getFullPath();}

    protected:
        virtual int numInitStages() const { return 3;}

        virtual void initialize(int stage);

        virtual void handleMessage(cMessage *msg);

    public:
        //Called by the NotificationBoard whenever a change of a category
        //occurs to which this client subscribed
        virtual void receiveChangeNotification(int category, const cObject *details);

        //returns the host or router this table lives in
        virtual cModule *getHostModule();

        //Adds a Link.
        virtual bool addLink(macLinkTableEntry *entry);

        //deletes a Link.
        virtual bool deleteLink(macLinkTableEntry *entry);

        //Returns the number of Links
        virtual int getNumLinks();

        //called from macLinkTableEntry
        virtual void linkChanged(macLinkTableEntry *entry, int category);

        //Returns the Link
        virtual macLinkTableEntry *getLink(int pos);

        //Returns Link by Id
        virtual macLinkTableEntry *getLinkById(int id);

        //Returns number of links used for specific Slotframe
        virtual int getNumberLinksBySlotframe(int slotframeId);

        //Returns Link by Timeslot
        virtual macLinkTableEntry *getLinkByTimeslot(int timeslot);

        //@author:  2014    Stefan Reis
        // get LinkTableEntry, depending on the timeslot and slotframe
        virtual macLinkTableEntry *getLinkByTimeslotSlotframe(int timeslot, int slotframe);

        //@author:  2014    Stefan Reis
        // check if a link exist to the dest address
        virtual bool existLink(UINT_16 address);

        //Returns link by linkId and slotframeId
        virtual macLinkTableEntry *getLinkByIdAndSlotframe(int LinkId, int SlotframeId);

        //Return first link which belongs to node address
        virtual macLinkTableEntry *getLinkByNodeAddress(UINT_16 address);

        //Returns first link which belongs to node address and Option
        virtual macLinkTableEntry *getLinkByNodeAddress(UINT_16 address,MACTSCHLinkOptions option);

        //Returns all links which belong to one slotframe
        virtual LinkTableVector *getLinksBySlotframeId(int id);

        //edits an link
        virtual void editLink(macLinkTableEntry *entry);

        //@author:  2014    Stefan Reis
        // check if a receive link has a timescoure
        virtual bool isTimescource(UINT_16 address);
};
#endif /* MACLINKTABLE_H_ */

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

#ifndef MACHOPPINGSEQUENCELIST_H_
#define MACHOPPINGSEQUENCELIST_H_

#include <vector>

#include "INETDefs.h"

#include "INotifiable.h"
#include "IMacHoppingSequenceList.h"
#include "macHoppingSequenceListEntry.h"
#include "Ieee802154eDefSlotframe.h"

class IInterfaceTable;
class NotificationBoard;
class macHoppingSequenceListParser;

class INET_API macHoppingSequenceList : public cSimpleModule, public IMacHoppingSequenceList, protected INotifiable
{
    //member variables
    protected:
        IInterfaceTable *ift; // cached pointer
        NotificationBoard *nb; // cached pointer

        bool readFromFile;

        HoppingSequenceListVector idToEntry;

        //fields to support getNumEntries() and getEntry(pos)
        int tmpNumEntries; //caches number of non-NULL elements of idToEntry;
        macHoppingSequenceListEntry **tmpEntryList; //caches non-NULL elements of idToEntry;

    protected:
        //displays summary above the icon
        virtual void updateDisplayString();

        //internal
        virtual void invalidTmpHoppingSequenceList();

        virtual void resetEntries();
    public:
        macHoppingSequenceList();
        virtual ~macHoppingSequenceList();

        virtual std::string getFullPath() const {return cSimpleModule::getFullPath();}

    protected:
        virtual int numInitStages() const{return 3;}
        virtual void initialize(int stage);
        virtual void handleMessage(cMessage *msg);

    public:
        //Called by the NotificationBoard whenever a change of a category
        //occurs to which this client subscribed
        virtual void receiveChangeNotification(int category, const cObject *details);

        //returns the host or router this list lives in
        virtual cModule *getHostModule();

        //Adds Entries
        virtual bool addEntry(macHoppingSequenceListEntry *entry);

        //deletes a Entry
        virtual void deleteEntry(macHoppingSequenceListEntry *entry);

        //returns the number of Entries
        virtual int getNumEntries();

        //called from macHoppingSequenceListEntry
        virtual void entryChanged(macHoppingSequenceListEntry *entry, int category);

        //Returns the Entry
        virtual macHoppingSequenceListEntry *getEntry(int pos);

        //Returns Entry by Id
        virtual macHoppingSequenceListEntry *getEntryById(int id);

        //Edits an HoppingSequenceListEntry
        virtual void editHoppingSequenceListEntry(macHoppingSequenceListEntry* entry);
};

#endif /* MACHOPPINGSEQUENCELIST_H_ */

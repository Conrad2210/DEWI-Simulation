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

#ifndef MACTIMESLOTTABLE_H_
#define MACTIMESLOTTABLE_H_

#include <vector>

#include "INETDefs.h"

#include "INotifiable.h"
#include "IMacTimeslotTable.h"
#include "macTimeslotTableEntry.h"
#include "Ieee802154eDefSlotframe.h"

class IInterfaceTable;
class NotificationBoard;
class macTimeslotTableParser;

class INET_API macTimeslotTable : public cSimpleModule, public IMacTimeslotTable, protected INotifiable
{
    //member variables
    protected:
        IInterfaceTable *ift; // cached pointer
        NotificationBoard *nb; // cached pointer

        bool readFromFile;

        TimeslotTableVector idToTemplate;

        //fields to support getNumTemplates() and getTemplate(pos)
        int tmpNumTemplates; //caches number of non-NULL elements of idToTemplate;
        macTimeslotTableEntry **tmpTemplateList; //caches non-NULL elements of idToTemplate;

    protected:
        //displays summary above the icon
        virtual void updateDisplayString();

        //internal
        virtual void invalidTmpTemplateList();

        virtual void resetTemplates();
    public:
        macTimeslotTable();
        virtual ~macTimeslotTable();

        virtual std::string getFullPath() const {return cSimpleModule::getFullPath();}

    protected:
        virtual int numInitStages() const{return 3;}
        virtual void initialize(int stage);
        virtual void handleMessage(cMessage *msg);

    public:
        //Called by the NotificationBoard whenever a change of a category
        //occurs to which this client subscribed
        virtual void receiveChangeNotification(int category, const cObject *details);

        //returns the host or router this table lives in
        virtual cModule *getHostModule();

        //Adds Templates
        virtual bool addTemplate(macTimeslotTableEntry *entry);

        //deletes a Template
        virtual void deleteTemplate(macTimeslotTableEntry *entry);

        //returns the number of templates
        virtual int getNumTemplates();

        //called from macTimeslotTemplateEntry
        virtual void templateChanged(macTimeslotTableEntry *entry, int category);

        //Returns the Template
        virtual macTimeslotTableEntry *getTemplate(int pos);

        //Returns Template by Id
        virtual macTimeslotTableEntry *getTemplateById(int id);

        //Edits an TimeslotTemplate
        virtual void editTimeslotTemplate(macTimeslotTableEntry* entry);

	//deletes all entries
	virtual void clearTable();
};

#endif /* MACTIMESLOTTABLE_H_ */

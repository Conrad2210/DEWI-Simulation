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

#ifndef MACTIMESLOTTEMPLATE_H_
#define MACTIMESLOTTEMPLATE_H_

#include "INETDefs.h"
#include <vector>
#include "macTimeslotTemplateEntry.h"
#include "NotificationBoard.h"
#include "INotifiable.h"
#include "IMacTimeslotTemplate.h"
#include "Ieee802154eDef.h"

class NotificationBoard;


class INET_API macTimeslotTemplate : public cSimpleModule, public IMacTimeslotTemplate, protected INotifiable
{
        //member variables
        NotificationBoard *nb;

        TemplateTableVector idToTemplate;

        //fields to support getNumTemplates() and getTemplate(pos)
        int tmpNumTemplates; //caches number of non-NULL elements of idToTemplate;
        macTimeslotTemplateEntry **tmpTemplateList; //caches non-NULL elements of idToTemplate;

    protected:
        //displays summary above the icon
        virtual void updateDisplayString();

        //internal
        virtual void invalidTmpTemplateList();

        virtual void resetTemplates();
    public:
        macTimeslotTemplate();
        virtual ~macTimeslotTemplate();

        virtual std::string getFullPath() const {return cSimpleModule::getFullPath();}

    protected:
        virtual int numInitStages() const{return 2;}
        virtual void initialize(int stage);
        virtual void handleMessage(cMessage *msg);

    public:
        //Called by the NotificationBoard whenever a change of a category
        //occurs to which this client subscribed
        virtual void receiveChangeNotification(int category, const cObject *details);


        //returns the host or router this table lives in
        virtual cModule *getHostModule();

        //Adds Templates
        virtual void addTemplate(macTimeslotTemplateEntry *entry);

        //deletes a Template
        virtual void deleteTemplate(macTimeslotTemplateEntry *entry);

        //returns the number of templates
        virtual int getNumTemplates();

        //called from macTimeslotTemplateEntry
        virtual void templateChanged(macTimeslotTemplateEntry *entry, int category);

        //Returns the Template
        virtual macTimeslotTemplateEntry *getTemplate(int pos);

        //Returns Template by Id
        virtual macTimeslotTemplateEntry *getTemplateById(int id);

        //Edits an TimeslotTemplate
        virtual void editTimeslotTemplate(macTimeslotTemplateEntry* entry);
};

#endif /* MACTIMESLOTTEMPLATE_H_ */

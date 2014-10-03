/*
 * IMacTimeslotTemplate.h
 *
 *  Created on: 4 May 2014
 *      Author: R00118979
 */

#ifndef IMACTIMESLOTTEMPLATE_H_
#define IMACTIMESLOTTEMPLATE_H_

/**
 * A C++ interface to abstract the functionality of of IMacTimeslotTemplate.
 * Referring to IMacTimeslotTemplate via this interface makes it possible to
 * transparently replace IMacTimeslotTemplate with a different implementation,
 * without any change to the base INET.
 *
 * @see IMacTimeslotTemplate
 * @author Conrad Dandelski
 */

/*
 * @author: Conrad Dandelski
 * Copyright(c) 2014: Conrad Dandelski
 * Copyright(c) 2014: Nimbus Centre for Embedded Systems Research, Cork, Ireland
 * Copyright(c) 2014: Cork Institute of Technology, Cork, Ireland
 */

#include "INETDefs.h"
#include "macTimeslotTemplateEntry.h"

class INET_API IMacTimeslotTemplate
{
        friend class macTimeslotTemplateEntry;

    public:
        virtual ~IMacTimeslotTemplate()
        {
        }

        /*
         * Returns the host this macLinktTable lives in
         */
        virtual cModule *getHostModule() = 0;

        /*
         * Returns the total number of links
         */

        virtual int getNumTemplates() = 0;

        /*
         * Adds an linkt to macLinkTable. Links are allowed to be modified
         * while in the macLinkTable. (There is a notification mechanism that
         * allows macLinkTable internals to be updated on a routing entry change.)
         */

        virtual void addTemplate(macTimeslotTemplateEntry *entry) = 0;

        /*
         * Deletes the given link from the macLinkTable.
         * Returns true if the route was deleted, and false if it was
         * not in the macLinkTable
         */

        virtual void deleteTemplate(macTimeslotTemplateEntry *entry) = 0;

        /*
         * To be called from link objects whenever a field changes. Used for
         * maintaining internal data structures and firing "macLinkTable changed"
         * notifications
         */

        virtual void templateChanged(macTimeslotTemplateEntry *entry, int category) = 0;

        /*
         * Returns the link from the macLnkTable
         */
        virtual macTimeslotTemplateEntry *getTemplate(int pos) = 0;

};

#endif /* IMACTIMESLOTTEMPLATE_H_ */

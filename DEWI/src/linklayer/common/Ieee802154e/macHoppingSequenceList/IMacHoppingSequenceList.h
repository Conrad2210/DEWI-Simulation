/*
 * IMacHoppingSequenceList.h
 *
 *  Created on: 02. June 2014
 *      Author: R00118963
 */

/*
 * @author: Stefan Reis
 * Copyright(c) 2014: Stefan Reis
 * Copyright(c) 2014: Nimbus Centre for Embedded Systems Research, Cork, Ireland
 * Copyright(c) 2014: Cork Institute of Technology, Cork, Ireland
 */

#ifndef IMACHOPPINGSEQUENCELIST_H_
#define IMACHOPPINGSEQUENCELIST_H_

#include "INETDefs.h"

#include "macHoppingSequenceListEntry.h"

/**
 * A C++ interface to abstract the functionality of IMacHoppingSequenceList.
 * Referring to IMacHoppingSequenceList via this interface makes it possible to
 * transparently replace IMacHoppingSequenceList with a different implementation,
 * without any change to the base INET.
 *
 * @see IMacHoppingSequenceList
 * @author Stefan Reis
 */
class INET_API IMacHoppingSequenceList
{
        friend class macHoppingSequenceListEntry;

    public:
        virtual ~IMacHoppingSequenceList() {};

        /*
         * Returns the host this macHoppingSequenceList lives in
         */
        virtual cModule *getHostModule() = 0;

        /*
         * Returns the total number of HoppingSequenceListEntries
         */
        virtual int getNumEntries() = 0;

        /*
         * Adds an link to HoppingSequenceList. Links are allowed to be modified
         * while in the HoppingSequenceList. (There is a notification mechanism that
         * allows HoppingSequenceList internals to be updated on a routing entry change.)
         */
        virtual bool addEntry(macHoppingSequenceListEntry *entry) = 0;

        /*
         * Deletes the given link from the HoppingSequenceList.
         * Returns true if the route was deleted, and false if it was
         * not in the macHoppingSequenceList
         */
        virtual void deleteEntry(macHoppingSequenceListEntry *entry) = 0;

        /*
         * To be called from link objects whenever a field changes. Used for
         * maintaining internal data structures and firing "macLinkTable changed"
         * notifications
         */
        virtual void entryChanged(macHoppingSequenceListEntry *entry, int category) = 0;

        /*
         * Returns the HoppingSequenceListEntry from the HoppingSequenceList
         */
        virtual macHoppingSequenceListEntry *getEntry(int pos) = 0;

        virtual macHoppingSequenceListEntry *getEntryById(int id) = 0;

        /*
         * Edits an HoppingSequenceListEntry
         */
        virtual void editHoppingSequenceListEntry(macHoppingSequenceListEntry* entry) = 0;
};
#endif /* IMACHOPPINGSEQUENCELIST_H_ */

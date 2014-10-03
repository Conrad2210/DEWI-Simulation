/*
 * IMacTimeslotTable.h
 *
 *  Created on: 4 May 2014
 *      Author: R00118979
 */
/*

 * @author: Conrad Dandelski
 * Copyright(c) 2014: Conrad Dandelski
 * Copyright(c) 2014: Nimbus Centre for Embedded Systems Research, Cork, Ireland
 * Copyright(c) 2014: Cork Institute of Technology, Cork, Ireland
 */

#ifndef IMACTIMESLOTTABLE_H_
#define IMACTIMESLOTTABLE_H_

#include "INETDefs.h"

#include "macTimeslotTableEntry.h"

/**
 * A C++ interface to abstract the functionality of IMacTimeslotTable.
 * Referring to IMacTimeslotTemplate via this interface makes it possible to
 * transparently replace IMacTimeslotTemplate with a different implementation,
 * without any change to the base INET.
 *
 * @see IMacTimeslotTable
 * @author Conrad Dandelski
 */
class INET_API IMacTimeslotTable
{
        friend class macTimeslotTableEntry;

    public:
        virtual ~IMacTimeslotTable() {};

        /*
         * Returns the host this macTimeslotTable lives in
         */
        virtual cModule *getHostModule() = 0;

        /*
         * Returns the total number of timeslots
         */
        virtual int getNumTemplates() = 0;

        /*
         * Adds an link to TimeslotTable. Links are allowed to be modified
         * while in the TimeslotTable. (There is a notification mechanism that
         * allows TimeslotTable internals to be updated on a routing entry change.)
         */
        virtual bool addTemplate(macTimeslotTableEntry *entry) = 0;

        /*
         * Deletes the given link from the TimeslotTable.
         * Returns true if the route was deleted, and false if it was
         * not in the macTimeslotTable
         */
        virtual void deleteTemplate(macTimeslotTableEntry *entry) = 0;

        /*
         * To be called from link objects whenever a field changes. Used for
         * maintaining internal data structures and firing "macTimeslotTable changed"
         * notifications
         */
        virtual void templateChanged(macTimeslotTableEntry *entry, int category) = 0;

        /*
         * Returns the TimeslotEntry from the TimeslotTable
         */
        virtual macTimeslotTableEntry *getTemplate(int pos) = 0;

        /*
         * Edits an TimeslotEntry
         */
        virtual void editTimeslotTemplate(macTimeslotTableEntry* entry) = 0;
};
#endif /* IMACTIMESLOTTABLE_H_ */

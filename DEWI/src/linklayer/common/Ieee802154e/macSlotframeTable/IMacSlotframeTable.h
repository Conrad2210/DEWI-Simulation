/*
 * IMacSlotframeTable.h
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

#ifndef IMACSLOTFRAMETABLE_H_
#define IMACSLOTFRAMETABLE_H_

#include "INETDefs.h"

#include "macSlotframeTableEntry.h"

/**
 * A C++ interface to abstract the functionality of of IMacSlotframeTable.
 * Referring to IMacSlotframeTable via this interface makes it possible to
 * transparently replace IMacSlotframeTable with a different implementation,
 * without any change to the base INET.
 *
 * @see ImacLinkTable
 * @author Conrad Dandelski
 */
class INET_API IMacSlotframeTable
{
        friend class macSlotframeTableEntry;

    public:
        virtual ~IMacSlotframeTable() {};

        /*
        * Returns the host this macLinktTable lives in
        */
        virtual cModule *getHostModule() = 0;

         /*
          * Returns the total number of links
          */
         virtual int getNumSlotframes()  = 0;

         /*
          * Adds an slotframe to macSlotframeTable. Slotframes are allowed to be modified
          * while in the macSlotframeTable. (There is a notification mechanism that
          * allows macSlotframeTable internals to be updated on a Slotframe entry change.)
          */
         virtual bool addSlotframe(macSlotframeTableEntry *entry) = 0;

         /*
          * Deletes the given slotframe from the macSlotframeTable.
          * Returns true if the slotframe was deleted, and false if it was
          * not in the macLinkTable
          */
         virtual bool deleteSlotframe(macSlotframeTableEntry *entry) = 0;

         /*
          * Edits the given slotframe
          */
         virtual void editSlotframe(macSlotframeTableEntry *entry) = 0;

         /*
          * To be called from link objects whenever a field changes. Used for
          * maintaining internal data structures and firing "macLinkTable changed"
          * notifications
          */
         virtual void slotframeChanged(macSlotframeTableEntry *entry, int category) = 0;

         /*
          * Returns the link from the macLinkTable
          */
         virtual macSlotframeTableEntry *getSlotframe(int pos) = 0;

         virtual macSlotframeTableEntry *getSlotframeById(int id) = 0;
};
#endif /* IMACSLOTFRAMETABLE_H_ */

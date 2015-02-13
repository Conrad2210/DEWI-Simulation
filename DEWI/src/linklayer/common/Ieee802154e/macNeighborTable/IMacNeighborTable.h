/*
 * IMacNeighborTable.h
 *
 *  Created on: 11 August 2014
 *      Author: R0011896.
 */

/*
 * @author: Stefan Reis
 * Copyright(c) 2014: Stefan Reis
 * Copyright(c) 2014: Nimbus Centre for Embedded Systems Research, Cork, Ireland
 * Copyright(c) 2014: Cork Institute of Technology, Cork, Ireland
 */

#ifndef IMACNEIGHBORTABLE_H_
#define IMACNEIGHBORTABLE_H_

#include "INETDefs.h"

#include "macNeighborTableEntry.h"

/**
 * A C++ interface to abstract the functionality of of IMacNeighborTable.
 * Referring to IMacNeighborTable via this interface makes it possible to
 * transparently replace IMacNeighborTable with a different implementation,
 * without any change to the base INET.
 *
 * @see ImacLinkTable
 * @author Stefan Reis
 */
class INET_API IMacNeighborTable
{
        friend class macNeighborTableEntry;

    public:
        virtual ~IMacNeighborTable() {};

        /*
        * Returns the host this macLinktTable lives in
        */
        virtual cModule *getHostModule() = 0;

         /*
          * Returns the total number of links
          */
         virtual int getNumNeighbors()  = 0;

         /*
          * Adds an neighbor to macNeighborTable. Neighbors are allowed to be modified
          * while in the macNeighborTable. (There is a notification mechanism that
          * allows macNeighborTable internals to be updated on a Neighbor entry change.)
          */
         virtual bool addNeighbor(macNeighborTableEntry *entry) = 0;

         /*
          * Deletes the given Neighbor from the macNeighborTable.
          * Returns true if the Neighbor was deleted, and false if it was
          * not in the macLinkTable
          */
         virtual bool deleteNeighbor(macNeighborTableEntry *entry) = 0;

         /*
          * Edits the given Neighbor
          */
         virtual void editNeighbor(macNeighborTableEntry *entry) = 0;

         /*
          * To be called from link objects whenever a field changes. Used for
          * maintaining internal data structures and firing "macLinkTable changed"
          * notifications
          */
         virtual void neighborChanged(macNeighborTableEntry *entry, int category) = 0;

         /*
          * Returns the link from the macLinkTable
          */
         virtual macNeighborTableEntry *getNeighborByPos(int pos) = 0;
         virtual macNeighborTableEntry *getNeighborByPos(int pos, bool fillTmpNeighborList) = 0;
         virtual macNeighborTableEntry *getNeighborBySAddr(UINT_16 addr) = 0;
         virtual macNeighborTableEntry *getNeighborByEAddr(MACAddress addr) = 0;
         virtual macNeighborTableEntry *getNeighborById(int id) = 0;
         virtual bool isNeighborBySAddr(UINT_16 address) = 0;
         virtual bool isNeighborByEAddr(MACAddress address) = 0;
         virtual bool isNeighborTimeSource(UINT_16 address) = 0;
};
#endif /* IMACNeighborTABLE_H_ */

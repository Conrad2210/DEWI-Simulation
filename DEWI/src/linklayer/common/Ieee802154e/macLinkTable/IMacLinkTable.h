/*
 * ImacLinkTable.h
 *
 *  Created on: 4 May 2014
 *      Author: R00118979
 */

/*
 * @author: Conrad Dandelski
 * Copyright(c) 2014: Conrad Dandelski
 * Copyright(c) 2014: Nimbus Centre for Embedded Systems Research, Cork, Ireland
 * Copyright(c) 2014: Cork Institute of Technology, Cork, Ireland
 *
 * @author: 2014    Stefan Reis     (modified - to solve the include problem)
 */

#ifndef IMACLINKTABLE_H_
#define IMACLINKTABLE_H_

#include "INETDefs.h"

#include "macLinkTableEntry.h"
#include "IMacSlotframeTable.h"

/**
 * A C++ interface to abstract the functionality of of ImacLinkTable.
 * Referring to ImacLinkTable via this interface makes it possible to
 * transparently replace ImacLinkTable with a different implementation,
 * without any change to the base INET.
 *
 * @see ImacLinkTable
 * @author Conrad Dandelski
 */
class IMacSlotframeTable;

class INET_API IMacLinkTable
{
	friend class macLinkTableEntry;

    public:
	virtual ~IMacLinkTable()
	{
	}
	;

	/*
	 * Returns the host this macLinktTable lives in
	 */
	virtual cModule *getHostModule() = 0;

	/*
	 * Returns the total number of links
	 */
	virtual int getNumLinks() = 0;

	/*
	 * Adds an linkt to macLinkTable. Links are allowed to be modified
	 * while in the macLinkTable. (There is a notification mechanism that
	 * allows macLinkTable internals to be updated on a routing entry change.)
	 */
	virtual bool addLink(macLinkTableEntry *entry) = 0;

	/*
	 * Deletes the given link from the macLinkTable.
	 * Returns true if the route was deleted, and false if it was
	 * not in the macLinkTable
	 */
	virtual bool deleteLink(macLinkTableEntry *entry) = 0;

	/*
	 * To be called from link objects whenever a field changes. Used for
	 * maintaining internal data structures and firing "macLinkTable changed"
	 * notifications
	 */
	virtual void linkChanged(macLinkTableEntry *entry, int category) = 0;

	/*
	 * Returns the link from the macLnkTable
	 */
	virtual macLinkTableEntry *getLink(int pos) = 0;

	//@author:  2014    Stefan Reis
	virtual macLinkTableEntry *getLinkByTimeslotSlotframe(int timeslot, int slotframe) = 0;

	//Return links by depending on timeslot and slotframe
	virtual void getLinksByTimeslotSlotframe(int timeslot, int slotframe, std::list<macLinkTableEntry*> *list) = 0;

	//@author: 2014     Stefan Reis
	virtual bool existLink(UINT_16 address) = 0;
	virtual bool existLink(macLinkTableEntry* link) = 0;

	virtual macLinkTableEntry *getLinkByTimeslotOffsetAddress(int timeslot, int offset, UINT_16 address) = 0;

	virtual int getTimeSlotByOffset(int offset) = 0;

	virtual macLinkTableEntry *getLinkByTimeslot(int timeslot) = 0;

	virtual int getOffsetByTimeslot(int timeslot) = 0;
	/*
	 * Edit existing link
	 */
	virtual void editLink(macLinkTableEntry *entry) = 0;

	/*
	 * returns number of links linked to a specific slotframe
	 */
	virtual int getNumberLinksBySlotframe(int slotframeId) = 0;

	//@author: 2014     Stefan Reis
	virtual bool isTimescource(UINT_16 address) = 0;

	//deletes all entries
	virtual void clearTable() = 0;
};
#endif /* IMACLINKTABLE_H_ */

/*
 * Ieee802154e.h
 *
 *  Created on: 1 May 2014
 *      Author: R00118979
 */

#ifndef IEEE802154E_H_
#define IEEE802154E_H_

#ifndef IEEE_802154_E_DEF_H
#define IEEE_802154_E_DEF_H

#include <stdint.h>
#include "MACAddress.h"
#include "macLinkTableEntry.h"
#include <vector>
#include <map>
class macLinkTableEntry;
class macSlotframeTableEntry;
class macTimeslotTemplateEntry;
class slotframeIEEntry;
class linkIEEntry;
class linkIE;

//Vectors for table
typedef std::vector<macLinkTableEntry *> LinkTableVector;
typedef std::vector<macSlotframeTableEntry *> SlotframeTableVector;
typedef std::vector<macTimeslotTemplateEntry *> TemplateTableVector;

//Vectors for Slotframe and link IE page 90 in IEEE802.15.4e-2012
typedef std::vector<linkIEEntry *> linkIEvector;
typedef std::vector<slotframeIEEntry *> slotframeIEvector;


#endif



#endif /* IEEE802154E_H_ */

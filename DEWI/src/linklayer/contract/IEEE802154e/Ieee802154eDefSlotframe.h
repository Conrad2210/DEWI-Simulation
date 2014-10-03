/*@author:  2014    Conrad Dandelski
 * @author: 2014    Stefan Reis         (modified for the integration to the STEFAN branch of WPAN)
 * Ieee802154e.h
 *
 *  Created on: 1 May 2014
 *      Author: R00118979 & R00118963
 */

#ifndef IEEE_802154_E_DEF_SLOTFRAME_H
#define IEEE_802154_E_DEF_SLOTFRAME_H

#include <stdint.h>
#include <vector>

#include "MACAddress.h"
#include "macLinkTableEntry.h"


class macLinkTableEntry;
class macSlotframeTableEntry;
class macTimeslotTableEntry;
class macHoppingSequenceListEntry;
class macNeighborTableEntry;

class slotframeIEEntry;
class linkIEEntry;
class linkIE;

//Vectors for table
typedef std::vector<macLinkTableEntry *> LinkTableVector;
typedef std::vector<macSlotframeTableEntry *> SlotframeTableVector;
typedef std::vector<macTimeslotTableEntry *> TimeslotTableVector;
typedef std::vector<macHoppingSequenceListEntry *> HoppingSequenceListVector;

//Vectors for Slotframe and link IE page 90 in IEEE802.15.4e-2012
typedef std::vector<linkIEEntry *> linkIEvector;
typedef std::vector<slotframeIEEntry *> slotframeIEvector;

//Vector for Neighbor Information Table - Std 802.15.4e-2012 (Table 52i) page 181
typedef std::vector<macNeighborTableEntry *> NeighborTableVector;

#endif

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

#ifndef SLOTFRAMELINKIE_H_
#define SLOTFRAMELINKIE_H_
#include "Ieee802154eDef.h"
#include "IMacLinkTable.h"
#include "IMacSlotframeTable.h"
#include "slotframeLinkIEEntry.h"

class SlotframeLinkIE
{
    protected:
        IMacSlotframeTable* slotframeTable;
        IMacLinkTable* linkTable;
        slotframeLinkIEEntry* entry;

    public:
        int fillSlotframeLinkIE();
        slotframeLinkIEEntry *getSlotframeLinkIEEntry();
    public:
        SlotframeLinkIE(IMacSlotframeTable* slTable, IMacLinkTable* liTable);
        virtual ~SlotframeLinkIE();
};

#endif /* SLOTFRAMELINKIE_H_ */

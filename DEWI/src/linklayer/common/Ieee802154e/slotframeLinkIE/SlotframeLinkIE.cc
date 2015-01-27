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

#include "SlotframeLinkIE.h"
#include "slotframeIEEntry.h"
#include "linkIEEntry.h"

SlotframeLinkIE::SlotframeLinkIE(IMacSlotframeTable* slTable, IMacLinkTable* liTable)
{
    // @author Conrad Dandelski
    slotframeTable = slTable;
    linkTable = liTable;
    entry = new slotframeLinkIEEntry();
}

SlotframeLinkIE::~SlotframeLinkIE()
{
    // @author Conrad Dandelski
}

int SlotframeLinkIE::fillSlotframeLinkIE()
{
    int length = 0;
    linkIEEntry* linkIeEntry;
    slotframeIEEntry* slotIeEntry;
    if (slotframeTable->getNumSlotframes() > 0)
    {
        length += 1 + slotframeTable->getNumSlotframes() * 5;
        entry->setNumSlotframes(slotframeTable->getNumSlotframes());

        for (int i = 0; i < entry->getNumSlotframes(); i++)
        {
            slotIeEntry = new slotframeIEEntry();
            slotIeEntry->setSlotframeId(slotframeTable->getSlotframe(i)->getSlotframeId());
            slotIeEntry->setSlotframeSize(slotframeTable->getSlotframe(i)->getSlotframeSize());
            slotIeEntry->setNumLinks(linkTable->getNumberLinksBySlotframe(slotIeEntry->getSlotframeId()));
            for(int k = 0; k < linkTable->getNumLinks(); k++)
            {
                linkIeEntry = new linkIEEntry();
                if(slotIeEntry->getSlotframeId() == linkTable->getLink(k)->getSlotframeId())
                {
                    length += 5;
                    linkIeEntry->setTimeslot(linkTable->getLink(k)->getTimeslot());
                    linkIeEntry->setChannelOffset(linkTable->getLink(k)->getChannelOffset());
                    linkIeEntry->setLinkOption(linkTable->getLink(k)->getLinkOption());
                    slotIeEntry->setLinkIE(linkIeEntry);
                }
            }
            entry->setSlotframeIE(slotIeEntry);
        }
    }
    return length;
}

slotframeLinkIEEntry *SlotframeLinkIE::getSlotframeLinkIEEntry()
{
    return entry;
}

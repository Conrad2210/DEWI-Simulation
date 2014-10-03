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

#ifndef SLOTFRAMEIEENTRY_H_
#define SLOTFRAMEIEENTRY_H_

#include "Ieee802154eDefSlotframe.h"
#include "linkIEEntry.h"

class slotframeIEEntry
{
    protected:
        int slotframeId;
        int slotframeSize;
        int numLinks;
        linkIEvector linkIE;
    public:
        virtual void setSlotframeId(int n){slotframeId = n;}
        virtual int getSlotframeId(){return slotframeId;}

        virtual void setSlotframeSize(int n){slotframeSize = n;}
        virtual int getSlotframeSize(){return slotframeSize;}

        virtual void setNumLinks(int n){numLinks = n;}
        virtual int getNumLinks(){return numLinks;}

        void setLinkIE(linkIEEntry *entry);
        linkIEEntry* getLinkIE(int pos);
    public:
        slotframeIEEntry();
        virtual ~slotframeIEEntry();
};

#endif /* SLOTFRAMEIEENTRY_H_ */

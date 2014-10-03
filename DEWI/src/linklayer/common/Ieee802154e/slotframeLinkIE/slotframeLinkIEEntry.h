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

#ifndef SLOTFRAMELINKIEENTRY_H_
#define SLOTFRAMELINKIEENTRY_H_
#include "Ieee802154eDef.h"
#include "slotframeIEEntry.h"


class slotframeLinkIEEntry
{
    protected:
        int numSlotframes;
        slotframeIEvector slotframeVector;

    public:
        virtual void setNumSlotframes(int n){numSlotframes = n;}
        virtual int getNumSlotframes(){return numSlotframes;}

        void setSlotframeIE(slotframeIEEntry *entry);
        slotframeIEEntry *getSlotframeIE(int pos);
    public:
        slotframeLinkIEEntry();
        virtual ~slotframeLinkIEEntry();
};

#endif /* SLOTFRAMELINKIEENTRY_H_ */

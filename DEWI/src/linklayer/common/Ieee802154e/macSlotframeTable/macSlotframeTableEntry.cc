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

/*
 * @author: Conrad Dandelski
 * Copyright(c) 2014: Conrad Dandelski
 * Copyright(c) 2014: Nimbus Centre for Embedded Systems Research, Cork, Ireland
 * Copyright(c) 2014: Cork Institute of Technology, Cork, Ireland
 */

#include "macSlotframeTableEntry.h"
#include "macSlotframeTable.h"
#include <stdio.h>
#include <sstream>

macSlotframeTableEntry::macSlotframeTableEntry()
{
    // TODO Auto-generated constructor stub
    ownerp = NULL;
    slotframeId = -1;
    slotframeSize = -1;
}

macSlotframeTableEntry::~macSlotframeTableEntry()
{
    // TODO Auto-generated destructor stub
    resetSlotframe();
}

std::string macSlotframeTableEntry::info() const
{
    std::stringstream out;
    out << "SlotframeId: " << slotframeId;
    out << " SlotframeSize: " << slotframeSize;
    return out.str();
}

std::string macSlotframeTableEntry::getFullPath() const
{
    return ownerp == NULL ? getFullName() : ownerp -> getHostModule() -> getFullPath() + "." + getFullName();

}

void macSlotframeTableEntry::changed(int category)
{
    if(ownerp)
        ownerp -> slotframeChanged(this,category);
}

void macSlotframeTableEntry::resetSlotframe()
{
    this -> slotframeId = -1;
    this -> slotframeSize = -1;
}

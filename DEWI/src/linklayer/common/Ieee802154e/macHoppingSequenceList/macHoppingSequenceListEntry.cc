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
 * @author: Stefan Reis
 * Copyright(c) 2014: Stefan Reis
 * Copyright(c) 2014: Nimbus Centre for Embedded Systems Research, Cork, Ireland
 * Copyright(c) 2014: Cork Institute of Technology, Cork, Ireland
 */

#include <stdio.h>
#include <sstream>

#include "macHoppingSequenceListEntry.h"
#include "macHoppingSequenceList.h"


macHoppingSequenceListEntry::macHoppingSequenceListEntry()
{
    ownerp = NULL;
    hoppingSequenceId       = 0;
    channelPage             = 0;
    numberOfChannels        = 0;
    phyConfiguration        = 0;
    extendedBitmap          = 0;
    hoppingSequenceLength   = 0;
    hoppingSequenceList     = std::vector<UINT_16>();
    currentHop              = 0;
    hopDwellTime            = 0;
}

macHoppingSequenceListEntry::~macHoppingSequenceListEntry()
{
    // TODO Auto-generated destructor stub
    resetEntries();
}

std::string macHoppingSequenceListEntry::info() const
{
    std::stringstream out;

    out << "Hopping Sequence Id: " << hoppingSequenceId;
    out << " Channel Page: " << (int)channelPage << " ";
    out << " Number Of Channels: " << (int)numberOfChannels << " ";
    out << " Phy Configuration: " << (int)phyConfiguration << " ";
    out << " Extended Bitmap: " << (int)extendedBitmap << " ";
    out << " Hopping Sequence Length: " << (int)hoppingSequenceLength << " ";
    out << " Hopping Sequence List: ";

    for (unsigned int i = 0; i != hoppingSequenceList.size(); ++i)
    {
        out << "[" << i << "]: " << hoppingSequenceList[i] << "; ";
    }

    out << " Current Hop: " << (int)currentHop << " ";
    out << " Hop Dwell Time: " << (int)hopDwellTime << " ";
    return out.str();
}

std::string macHoppingSequenceListEntry::getFullPath()const
{
    return ownerp == NULL ? getFullName() : ownerp->getHostModule()->getFullPath() + "." + getFullName();
}

void macHoppingSequenceListEntry::changed(int category)
{
    if(ownerp)
        ownerp->entryChanged(this,category);
}

void macHoppingSequenceListEntry::resetEntries()
{
    this->hoppingSequenceId     = 0;
    this->channelPage           = 0;
    this->numberOfChannels      = 0;
    this->phyConfiguration      = 0;
    this->extendedBitmap        = 0;
    this->hoppingSequenceLength = 0;
    this->hoppingSequenceList   = std::vector<UINT_16>();
    this->currentHop            = 0;
    this->hopDwellTime          = 0;
}

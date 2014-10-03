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

#include <slotframeIEEntry.h>

slotframeIEEntry::slotframeIEEntry()
{
    //@author Conrad Dandelski
    slotframeId = -1;
    slotframeSize = -1;
    numLinks = -1;
}

slotframeIEEntry::~slotframeIEEntry()
{
    //@author Conrad Dandelski

}

void slotframeIEEntry::setLinkIE(linkIEEntry *entry)
{
    linkIE.push_back(entry);
}

linkIEEntry *slotframeIEEntry::getLinkIE(int pos)
{
    return linkIE[pos];
}


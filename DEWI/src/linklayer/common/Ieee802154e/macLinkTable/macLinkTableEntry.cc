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
 *
 * @author: 2014    Stefan Reis     (modified for the integration with STEFAN branch from WPAN)
 */

#include "macLinkTableEntry.h"
#include "macLinkTable.h"
#include <stdio.h>
#include <sstream>

macLinkTableEntry::macLinkTableEntry()
{
    // TODO Auto-generated constructor stub
    ownerp = NULL;
    LinkId = -1;
    linkOption = LNK_OP_undefinedOption;
    linkType = LNK_TP_undefinedType;
    slotframeId = -1;
    nodeAddress = 0x00;
    timeslot = -1;
    channelOffset = -1;
}

macLinkTableEntry::~macLinkTableEntry()
{
    // TODO Auto-generated destructor stub
    resetLink();
}

std::string macLinkTableEntry::info() const
{
    std::stringstream out;
    //out << (getName()[0] ? getName() : "*");
    out << "LinkId: " << LinkId;
    switch(linkOption)
    {
        case LNK_OP_TRANSMIT:
            out << " LinkOption: transmit ";
            break;
        case LNK_OP_RECEIVE:
            out << " LinkOption: receive ";
            break;
        case LNK_OP_SHARED:
            out << " LinkOption: shared ";
            break;
        case LNK_OP_RECEIVE_TIMEKEEPING:
            out << " LinkOption: receive and timekeeping ";
            break;
        case LNK_OP_SHARED_RECEIVE:
            out << " LinkOption: shared and receive ";
            break;
        case LNK_OP_RESERVED:
            out << " LinkOption: reserved ";
            break;
        default:
            out << " LinkOption: undefined";
            break;
    }

    switch(linkType)
    {
        case LNK_TP_NORMAL:
            out << " LinkType: normal";
            break;
        case LNK_TP_JOIN:
            out << " LinkType: join";
            break;
        case LNK_TP_ADVERTISING:
            out << " LinkType: advertising";
            break;
        default:
            out << " LinkType: undefined";
            break;
    }

    out << " SlotframeId: " << slotframeId;
    out << " NodeAddress: " << nodeAddress;
    out << " Timeslot: " << timeslot;
    out << " ChannelOffset:" << channelOffset;

    return out.str();
}

std::string macLinkTableEntry::getFullPath() const
{
    return ownerp == NULL ? getFullName() : ownerp->getHostModule()->getFullPath() + "." + getFullName();
}

void macLinkTableEntry::changed(int category)
{
    if(ownerp)
        ownerp->linkChanged(this,category);
}

void macLinkTableEntry::resetLink()
{
    this->channelOffset = -1;
    this->linkOption = LNK_OP_undefinedOption;
    this->linkType = LNK_TP_undefinedType;
    this->nodeAddress = 0x00;
    this->slotframeId = -1;
    this->timeslot = -1;
}

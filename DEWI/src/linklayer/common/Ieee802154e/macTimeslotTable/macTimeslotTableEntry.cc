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
 * @author: 2014    Stefan Reis     (modified, changed the inti values to the default values from the Std 802.15.4e-2012 (table 52e) page 176)
 */

#include "macTimeslotTableEntry.h"
#include "macTimeslotTable.h"
#include "Ieee802154eConst.h"

macTimeslotTableEntry::macTimeslotTableEntry()
{
    ownerp = NULL;
    TemplateId      = def_macTimeslotTemplateId;
    TsCCAOffset     = def_macTsCCAOffset;
    TsCCA           = def_macTsCCA;
    TsTxOffset      = def_macTsTxOffset;
    TsRxOffset      = def_macTsRxOffset;
    TsRxAckDelay    = def_macTsRxAckDelay;
    TsTxAckDelay    = def_macTsTxAckDelay;
    TsRxWait        = def_macTsRxWait;
    TsAckWait       = def_macTsAckWait;
    TsRxTx          = def_macTsRxTx;
    TsMaxAck        = def_macTsMaxAck;
    TsMaxTx         = def_macTsMaxTx;
    TsTimeslotLength  = def_macTsTimeslotLength;

}

macTimeslotTableEntry::~macTimeslotTableEntry()
{
    // TODO Auto-generated destructor stub
    resetTemplate();
}

std::string macTimeslotTableEntry::info() const
{
    std::stringstream out;

    out << "TemplateId: " << TemplateId;
    out << " CCA Offset: " << TsCCAOffset << "\xC2\xB5s";
    out << " CCA: " << TsCCA << "\xC2\xB5s";
    out << " TxOffset: " << TsTxOffset << "\xC2\xB5s";
    out << " RxOffset: " << TsRxOffset << "\xC2\xB5s";
    out << " RxAckDelay: " << TsRxAckDelay << "\xC2\xB5s";
    out << " TxAckDelay: " << TsTxAckDelay << "\xC2\xB5s";
    out << " AckWait: " << TsAckWait << "\xC2\xB5s";
    out << " RxTx Turnaround: " << TsRxTx << "\xC2\xB5s";
    out << " MaxAck: " << TsMaxAck << "\xC2\xB5s";
    out << " MaxTx: " << TsMaxTx << "\xC2\xB5s";
    out << " TimeslotLength: " << TsTimeslotLength << "\xC2\xB5s";
    return out.str();
}

std::string macTimeslotTableEntry::getFullPath()const
{
    return ownerp == NULL ? getFullName() : ownerp->getHostModule()->getFullPath() + "." + getFullName();
}

void macTimeslotTableEntry::changed(int category)
{
    if(ownerp)
        ownerp->templateChanged(this,category);
}

void macTimeslotTableEntry::resetTemplate()
{
    this->TemplateId        = def_macTimeslotTemplateId;
    this->TsCCAOffset       = def_macTsCCAOffset;
    this->TsCCA             = def_macTsCCA;
    this->TsTxOffset        = def_macTsTxOffset;
    this->TsRxOffset        = def_macTsRxOffset;
    this->TsRxAckDelay      = def_macTsRxAckDelay;
    this->TsTxAckDelay      = def_macTsTxAckDelay;
    this->TsRxWait          = def_macTsRxWait;
    this->TsAckWait         = def_macTsAckWait;
    this->TsRxTx            = def_macTsRxTx;
    this->TsMaxAck          = def_macTsMaxAck;
    this->TsMaxTx           = def_macTsMaxTx;
    this->TsTimeslotLength  = def_macTsTimeslotLength;
}


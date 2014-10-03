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

#include <Ieee802154e/macTimeslotTemplate/macTimeslotTemplateEntry.h>
#include "macTimeslotTemplate.h"

macTimeslotTemplateEntry::macTimeslotTemplateEntry()
{
    ownerp = NULL;
    TemplateId = -1;
    TsCCAOffset = -1.0;
    TsCCA = -1.0;
    TsTxOffset = -1.0;
    TsRxOffset = -1.0;
    TsRxAckDelay = -1.0;
    TsTxAckDelay = -1.0;
    TsRxWait = -1.0;
    TsAckWait = -1.0;
    TsRxTx = -1.0;
    TsMaxAck = -1.0;
    TsMaxTx = -1.0;

}

macTimeslotTemplateEntry::~macTimeslotTemplateEntry()
{
    // TODO Auto-generated destructor stub
    resetTemplate();
}

std::string macTimeslotTemplateEntry::info() const
{
    std::stringstream out;

    out << "TemplateId: " << TemplateId;
    out << " CCA Offset: " << TsCCAOffset << "us";
    out << " CCA: " << TsCCA << "us";
    out << " TxOffset: " << TsTxOffset << "us";
    out << " RxOffset: " << TsRxOffset << "us";
    out << " RxAckDelay: " << TsRxAckDelay << "us";
    out << " TxAckDelay: " << TsTxAckDelay << "us";
    out << " AckWait: " << TsAckWait << "us";
    out << " RxTx Tunraround: " << TsRxTx << "us";
    out << " MaxAck: " << TsMaxAck << "us";
    out << " MaxTx: " << TsMaxTx << "us";
    out << " TimeslotLength: " << TsTimeslotLength << "us";
    return out.str();
}

std::string macTimeslotTemplateEntry::getFullPath()const
{
    return ownerp == NULL ? getFullName() : ownerp->getHostModule()->getFullPath() + "." + getFullName();
}

void macTimeslotTemplateEntry::changed(int category)
{
    if(ownerp)
        ownerp->templateChanged(this,category);
}

void macTimeslotTemplateEntry::resetTemplate()
{
    this->TemplateId = -1;
    this->TsCCAOffset = 1800;
    this->TsCCA = 128;
    this->TsTxOffset = 2120;
    this->TsRxOffset = 1120;
    this->TsRxAckDelay = 800;
    this->TsTxAckDelay = 1000;
    this->TsRxWait = 2200;
    this->TsAckWait = 400;
    this->TsRxTx = 192;
    this->TsMaxAck = 2400;
    this->TsMaxTx = 4256;
    this->TsTimeslotLength = 10000;
}


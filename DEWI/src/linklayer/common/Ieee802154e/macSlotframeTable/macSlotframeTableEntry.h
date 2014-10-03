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
 * @author: 2014    Stefan Reis     (modified)
 */

#ifndef MACSLOTFRAMETABLEENTRY_H_
#define MACSLOTFRAMETABLEENTRY_H_

#include "INETDefs.h"
#include "NotifierConsts.h"
#include "Ieee802154eEnum.h"
#include "Ieee802154eDefSlotframe.h"

class  IMacSlotframeTable;

class INET_API macSlotframeTableEntry : public cObject
{
    public:
        enum State {UP, DOWN, GOING_UP, GOING_DOWN};
    protected:
        IMacSlotframeTable *ownerp;
        cModule *slotframeModule;
        int slotframeId;
        int slotframeSize; //number of timeslots in slotframe
        State state;

    private:
        macSlotframeTableEntry(const macSlotframeTableEntry& obj);
        macSlotframeTableEntry& operator=(const macSlotframeTableEntry& obj);

    protected:
        //change notification
        virtual void configChanged() {changed(NF_INTERFACE_CONFIG_CHANGED);}
        virtual void stateChanged() {changed(NF_INTERFACE_STATE_CHANGED);}
        virtual void changed(int category);

    public:
        macSlotframeTableEntry();
        virtual ~macSlotframeTableEntry();

        virtual void resetSlotframe();
        virtual std::string info() const;
        virtual std::string getFullPath() const;

        //Returns the SlotframeTable this slotframe lives in, or Null
        IMacSlotframeTable *getSlotframeTable() const{return ownerp;}

        //Returns the requested State of this slotframe
        State getState() const{return state;}

        //Returns slotframe Id
        int getSlotframeId() const {return slotframeId;}

        //Returns slotframe Module
        cModule *getSlotframeModule() const{return slotframeModule;}

        //Returns Slotframe size
        int getSlotframeSize() const{return slotframeSize;}

        //Set slotframeTable
        virtual void setSlotframeTable(IMacSlotframeTable* t) {this->ownerp = t;}

        //Set slotframeId
        virtual void setSlotframeId(int id) {slotframeId = id;}

        //Set slotframeSize
        virtual void setSlotframeSize(int size) {slotframeSize = size;}
};
#endif /* MACSLOTFRAMETABLEENTRY_H_ */

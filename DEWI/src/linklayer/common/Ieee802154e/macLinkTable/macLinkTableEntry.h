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
 * @author: 2014    Stefan Reis     (modified for the integration to the STEFAN branch of WPAN)
 */

#ifndef MACLINKTABLEENTRY_H_
#define MACLINKTABLEENTRY_H_

#include "INETDefs.h"
#include "NotifierConsts.h"
#include "Ieee802154eEnum.h"
#include "Ieee802154eDef.h"
#include "Ieee802154Def.h"
#include "Ieee802154eDefSlotframe.h"

class IMacLinkTable;

class INET_API macLinkTableEntry : public cObject
{
    public:
        enum State {UP, DOWN, GOING_UP, GOING_DOWN};
    protected:
        IMacLinkTable *ownerp;
        cModule *linkModule;
        int LinkId;
        MACTSCHLinkOptions linkOption;
        MACTSCHLinkType linkType;
        int slotframeId;
        UINT_16 nodeAddress;
        int timeslot;
        int channelOffset;
        bool prevStage;          ///< requested interface state, similar to Linux ifup/ifdown
        bool nextStage;
        bool sameStage;
        State state;

    private:
        macLinkTableEntry(const macLinkTableEntry& obj);
        macLinkTableEntry& operator=(const macLinkTableEntry& obj);

    protected:
        //change notification
        virtual void configChanged() {changed(NF_LINK_CONFIG_CHANGED);}
        virtual void stateChanged() {changed(NF_LINK_STATE_CHANGED);}
        virtual void changed(int category);
    public:
        macLinkTableEntry();
        virtual ~macLinkTableEntry();

        virtual void resetLink();
        virtual std::string info() const;
        virtual std::string getFullPath() const;

        //Returns the LinkTable this Link is in, or NULL
        IMacLinkTable *getLinkTable() const{return ownerp;}

        //Returns the requested State of this link
        State getState() const {return state;}

        int getLinkId() const {return LinkId;}
        cModule *getLinkModule() const {return linkModule;}
        MACTSCHLinkOptions getLinkOption() const {return linkOption;}
        MACTSCHLinkType getLinkType() const {return linkType;}
        int getSlotframeId() const {return slotframeId;}
        UINT_16 getNodeAddress() const {return nodeAddress;}
        int getTimeslot() const {return timeslot;}
        int getChannelOffset() const {return channelOffset;}
        bool isprevStage(){return prevStage;}
        bool isnextStage(){return nextStage;}
        bool issameStage(){return sameStage;}


        virtual void setMacLinkTable(IMacLinkTable* t) {this->ownerp = t;}
        virtual void setLinkId(int id) {LinkId = id;}
        virtual void setLinkOption(MACTSCHLinkOptions option) {linkOption = option;}
        virtual void setLinkType(MACTSCHLinkType type) {linkType = type;}
        virtual void setSlotframeId(int id) {slotframeId = id;}
        virtual void setNodeAddress(UINT_16 address) {nodeAddress = address;}
        virtual void setTimeslot(int slot) {timeslot = slot;}
        virtual void setChannelOffset(int offset) {channelOffset = offset;}
        virtual void isprevStage(bool is){prevStage = is;}
        virtual void isnextStage(bool is){nextStage = is;}
        virtual void issameStage(bool is){sameStage = is;}
};
#endif /* MACLINKTABLEENTRY_H_ */

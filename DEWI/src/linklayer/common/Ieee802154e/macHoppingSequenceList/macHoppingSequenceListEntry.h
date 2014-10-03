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

#ifndef MACHOPPINGSEQUENCELISTENTRY_H_
#define MACHOPPINGSEQUENCELISTENTRY_H_

#include "INETDefs.h"
#include "NotifierConsts.h"
#include "Ieee802154eDef.h"
#include "Ieee802154eEnum.h"
#include "Ieee802154eDefSlotframe.h"

class IMacHoppingSequenceList;

class INET_API macHoppingSequenceListEntry : public cObject
{
    public:
        enum State {UP, DOWN, GOING_UP, GOING_DOWN};
    protected:
        IMacHoppingSequenceList *ownerp;
        cModule *entryModule;

        /*
         * Hopping Sequence ID - Each hopping sequence has a unique ID
         */
        int hoppingSequenceId;

        /*
         * Corresponds to the 5 MSBs (b27, ..., b31) of a row in phyChannelsSupported.
         * Note this may not correspond to the current channelPage in use.
         */
        UINT_8 channelPage;

        /*
         * Number of channels supported by the PHY on this channelPage.
         */
        UINT_16 numberOfChannels;

        /*
         * For channel pages 0 to 6, the 27 LSBs (b0, b1, ..., b26) indicate the status (1 = to be used, 0 = not to be used) for each
         * of the up to 27 valid channels available to the PHY. For pages 7 and 8, the 27 LSBs indicate the configuration of the PHY,
         * and the channel list is contained in the extendedBitmap.
         */
        UINT_32 phyConfiguration;

        /*
         * For pages 7 and 8, a bitmap of numberOfChannels bits, where bk shall indicate the status of channel k for each of the up
         * to numberOfChannels valid channels supported by that channel page and phyConfiguration. Otherwise field is empty.
         */
        UINT_32 extendedBitmap;

        /*
         * The number of channels in the Hopping Sequence. Does not necessarily equal numberOfChannels.
         */
        UINT_16 hoppingSequenceLength;

        /*
         * A macHoppingSequenceLength-element list of channels to be hopped over.
         */
        std::vector<UINT_16> hoppingSequenceList;

        /*
         * Index of the current position in the hopping sequence list.
         */
        UINT_16 currentHop;

        /*
         * For unslotted channel hopping modes, this field is the channel dwell time, in units of 10 mikro sec. For other modes,
         * the field is empty.
         */
        UINT_16 hopDwellTime;

        State state;

    private:
        macHoppingSequenceListEntry(const macHoppingSequenceListEntry& obj);
        macHoppingSequenceListEntry& operator=(const macHoppingSequenceListEntry& obj);

    protected:
        //change notification
        virtual void configChanged() {changed(NF_TEMPLATE_CONFIG_CHANGED);}
        virtual void stateChanged() {changed(NF_TEMPLATE_STATE_CHANGED);}
        virtual void changed(int category);

    public:
        macHoppingSequenceListEntry();
        virtual ~macHoppingSequenceListEntry();

        virtual void resetEntries();
        virtual std::string info() const;
        virtual std::string getFullPath() const;

        IMacHoppingSequenceList *getHSList() const{return ownerp;}

        //Returns the requested state of this Entry
        State getState() const{return state;}

        //Returns the entry elements
        cModule *getHoppingSequenceListModule() const{return entryModule;}
        UINT_8  getHoppingSequenceId() const{return hoppingSequenceId;}
        UINT_8  getChannelPage() const{return channelPage;}
        UINT_16 getNumberOfChannels() const{return numberOfChannels;}
        UINT_32 getPhyConfiguration() const{return phyConfiguration;}
        UINT_32 getExtendedBitmap() const{return extendedBitmap;}
        UINT_16 getHoppingSequenceLength() const{return hoppingSequenceLength;}
        std::vector<UINT_16> getHoppingSequenceList() const{return hoppingSequenceList;}
        UINT_16 getCurrentHop() const{return currentHop;}
        UINT_16 getHopDwellTime() const{return hopDwellTime;}

        virtual void setHoppingSequenceListModule(IMacHoppingSequenceList* t){this->ownerp = t;}
        virtual void setHoppingSequenceId(int id) {hoppingSequenceId = id;}
        virtual void setChannelPage(UINT_8 d) {channelPage = d;}
        virtual void setNumberOfChannels(UINT_16 d){numberOfChannels = d;}
        virtual void setPhyConfiguration(UINT_32 d){phyConfiguration = d;}
        virtual void setExtendedBitmap(UINT_32 d){extendedBitmap = d;}
        virtual void setHoppingSequenceLength(UINT_16 d){hoppingSequenceLength = d;}
        virtual void setHoppingSequenceList(std::vector<UINT_16> d){hoppingSequenceList = d;}
        virtual void setCurrentHop(UINT_16 d){currentHop = d;}
        virtual void setHopDwellTime(UINT_16 d){hopDwellTime = d;}
};

#endif /* MACHOPPINGSEQUENCELISTENTRY_H_ */

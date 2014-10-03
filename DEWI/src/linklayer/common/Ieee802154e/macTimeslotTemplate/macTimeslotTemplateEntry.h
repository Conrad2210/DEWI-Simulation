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

#ifndef MACTIMESLOTTEMPLATEENTRY_H_
#define MACTIMESLOTTEMPLATEENTRY_H_

/*
 * @author: Conrad Dandelski
 * Copyright(c) 2014: Conrad Dandelski
 * Copyright(c) 2014: Nimbus Centre for Embedded Systems Research, Cork, Ireland
 * Copyright(c) 2014: Cork Institute of Technology, Cork, Ireland
 */

#include "INETDefs.h"
#include "Ieee802154eDef.h"
#include "NotifierConsts.h"
#include "Ieee802154eEnum.h"

class IMacTimeslotTemplate;

class INET_API macTimeslotTemplateEntry : public cNamedObject
{
    public:
        enum State {UP, DOWN, GOING_UP, GOING_DOWN};
    protected:
        IMacTimeslotTemplate *ownerp;
        cModule *templateModule;
        int TemplateId;
        /*
         * the time between the beginning
         * of the timeslot and start of
         * CCA operation
         */
        uint16_t TsCCAOffset;

        /*
         * the duration of CCA
         */
        uint16_t TsCCA;

        /*
         * the time between the beginning of the
         * timeslot and the start of packet transmission
         */
        uint16_t TsTxOffset;

        /*
         * beginning of the timeslot to when
         * the receiver must be listening
         */
        uint16_t TsRxOffset;

        /*
         * end of packet to when the transmitter must
         * listen for ACK
         */

        uint16_t TsRxAckDelay;

        /*
         * end of packet to start ACK
         */
        uint16_t TsTxAckDelay;

        /*
         * the time to wait for start of packet
         */
        uint16_t TsRxWait;

        /*
         * the minimum time to wait for an ACK
         */
        uint16_t TsAckWait;

        /*
         * transmit to Receive turnaround (12 symbols)
         */
        uint16_t TsRxTx;

        /*
         * transmission time to send ACK
         */
        uint16_t TsMaxAck;

        /*
         * transmission time to send the maximum
         * length packet (133 bytes)
         */
        uint16_t TsMaxTx;

        /*
         * maximum timeslot length (included unused time)
         */
        uint16_t TsTimeslotLength;

        State state;

    private:
        macTimeslotTemplateEntry(const macTimeslotTemplateEntry& obj);
        macTimeslotTemplateEntry& operator=(const macTimeslotTemplateEntry& obj);

    protected:
        //change notification
        virtual void configChanged() {changed(NF_TEMPLATE_CONFIG_CHANGED);}
        virtual void stateChanged() {changed(NF_TEMPLATE_STATE_CHANGED);}
        virtual void changed(int category);

    public:
        macTimeslotTemplateEntry();
        virtual ~macTimeslotTemplateEntry();

        virtual void resetTemplate();
        virtual std::string info() const;
        virtual std::string getFullPath() const;

        //Returns the TemplateTable this Template lives in, or NULL
        IMacTimeslotTemplate *getTemplateTable() const{return ownerp;}

        //Returns the requested state of this Template
        State getState() const{return state;}

        //Returns Template Id
        int getTemplateId() const{return TemplateId;}
        cModule *getTemplateModule() const{return templateModule;}
        uint16_t getCCAOffset() const{return TsCCAOffset;}
        uint16_t getCCA() const{return TsCCA;}
        uint16_t getTxOffset() const{return TsTxOffset;}
        uint16_t getRxOffset() const{return TsRxOffset;}
        uint16_t getRxAckDelay() const{return TsRxAckDelay;}
        uint16_t getTxAckDelay() const{return TsTxAckDelay;}
        uint16_t getRxWait() const{return TsRxWait;}
        uint16_t getAckWait() const{return TsAckWait;}
        uint16_t getRxTx() const{return TsRxTx;}
        uint16_t getMaxAck() const{return TsMaxAck;}
        uint16_t getMaxTx() const{return TsMaxTx;}
        uint16_t getTimeslotLength() const{return TsTimeslotLength;}


        double getCCAOffsetDbl() const{return ((double)TsCCAOffset / 1000000);}
        double getCCADbl() const{return ((double)TsCCA / 1000000);}
        double getTxOffsetDbl() const{return ((double)TsTxOffset / 1000000);}
        double getRxOffsetDbl() const{return ((double)TsRxOffset / 1000000);}
        double getRxAckDelayDbl() const{return ((double)TsRxAckDelay / 1000000);}
        double getTxAckDelayDbl() const{return ((double)TsTxAckDelay / 1000000);}
        double getRxWaitDbl() const{return ((double)TsRxWait / 1000000);}
        double getAckWaitDbl() const{return ((double)TsAckWait / 1000000);}
        double getRxTxDbl() const{return ((double)TsRxTx / 1000000);}
        double getMaxAckDbl() const{return ((double)TsMaxAck / 1000000);}
        double getMaxTxDbl() const{return ((double)TsMaxTx / 1000000);}
        double getTimeslotLengthDbl() const{return ((double)TsTimeslotLength / 1000000);}

        virtual void setTemplateTable(IMacTimeslotTemplate* t){ownerp = t;}
        virtual void setTemplateId(int id) {TemplateId = id;}
        virtual void setCCAOffset(uint16_t d) {TsCCAOffset = d;}
        virtual void setCCA(uint16_t d){TsCCA = d;}
        virtual void setTxOffset(uint16_t d){TsTxOffset = d;}
        virtual void setRxOffset(uint16_t d){TsRxOffset = d;}
        virtual void setRxAckDelay(uint16_t d){TsRxAckDelay = d;}
        virtual void setTxAckDelay(uint16_t d){TsTxAckDelay = d;}
        virtual void setRxWait(uint16_t d){TsRxWait = d;}
        virtual void setAckWait(uint16_t d){TsAckWait = d;}
        virtual void setRxTx(uint16_t d){TsRxTx = d;}
        virtual void setMaxAck(uint16_t d){TsMaxAck = d;}
        virtual void setMaxTx(uint16_t d) {TsMaxTx = d;}
        virtual void setTimeslotLength(uint16_t d){TsTimeslotLength = d;}


};

#endif /* MACTIMESLOTTEMPLATEENTRY_H_ */

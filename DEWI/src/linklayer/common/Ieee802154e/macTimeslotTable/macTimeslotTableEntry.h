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
 * @author: 2014    Stefan Reis     (fixed the missing TimeslotLength value and changed the value types)
 */

#ifndef MACTIMESLOTTABLEENTRY_H_
#define MACTIMESLOTTABLEENTRY_H_

#include "INETDefs.h"
#include "Ieee802154eDef.h"
#include "NotifierConsts.h"
#include "Ieee802154eEnum.h"
#include "Ieee802154eDefSlotframe.h"

class IMacTimeslotTable;

class INET_API macTimeslotTableEntry : public cObject
{
    public:
        enum State {UP, DOWN, GOING_UP, GOING_DOWN};
    protected:
        IMacTimeslotTable *ownerp;
        cModule *templateModule;
        int TemplateId;
        /*
         * the time between the beginning
         * of the timeslot and start of
         * CCA operation
         */
        UINT_16 TsCCAOffset;

        /*
         * the duration of CCA
         */
        UINT_16 TsCCA;

        /*
         * the time between the beginning of the
         * timeslot and the start of packet transmission
         */
        UINT_16 TsTxOffset;

        /*
         * beginning of the timeslot to when
         * the receiver must be listening
         */
        UINT_16 TsRxOffset;

        /*
         * end of packet to when the transmitter must
         * listen for ACK
         */
        UINT_16 TsRxAckDelay;

        /*
         * end of packet to start ACK
         */
        UINT_16 TsTxAckDelay;

        /*
         * the time to wait for start of packet
         */
        UINT_16 TsRxWait;

        /*
         * the minimum time to wait for an ACK
         */
        UINT_16 TsAckWait;

        /*
         * transmit to Receive turnaround (12 symbols)
         */
        UINT_16 TsRxTx;

        /*
         * transmission time to send ACK
         */
        UINT_16 TsMaxAck;

        /*
         * transmission time to send the maximum
         * length packet (133 bytes)
         */
        UINT_16 TsMaxTx;

        /*
         * maximum timeslot length (included unused time)
         */
        UINT_16 TsTimeslotLength;

        State state;

    private:
        macTimeslotTableEntry(const macTimeslotTableEntry& obj);
        macTimeslotTableEntry& operator=(const macTimeslotTableEntry& obj);

    protected:
        //change notification
        virtual void configChanged() {changed(NF_TEMPLATE_CONFIG_CHANGED);}
        virtual void stateChanged() {changed(NF_TEMPLATE_STATE_CHANGED);}
        virtual void changed(int category);

    public:
        macTimeslotTableEntry();
        virtual ~macTimeslotTableEntry();

        virtual void resetTemplate();
        virtual std::string info() const;
        virtual std::string getFullPath() const;

        //Returns the TemplateTable this Template lives in, or NULL
        IMacTimeslotTable *getTemplateTable() const{return ownerp;}

        //Returns the requested state of this Template
        State getState() const{return state;}

        //Returns Template Id
        UINT_8 getTemplateId() const{return TemplateId;}
        cModule *getTemplateModule() const{return templateModule;}
        UINT_16 getCCAOffset() const{return TsCCAOffset;}
        UINT_16 getCCA() const{return TsCCA;}
        UINT_16 getTxOffset() const{return TsTxOffset;}
        UINT_16 getRxOffset() const{return TsRxOffset;}
        UINT_16 getRxAckDelay() const{return TsRxAckDelay;}
        UINT_16 getTxAckDelay() const{return TsTxAckDelay;}
        UINT_16 getRxWait() const{return TsRxWait;}
        UINT_16 getAckWait() const{return TsAckWait;}
        UINT_16 getRxTx() const{return TsRxTx;}
        UINT_16 getMaxAck() const{return TsMaxAck;}
        UINT_16 getMaxTx() const{return TsMaxTx;}
        UINT_16 getTimeslotLength() const{return TsTimeslotLength;}

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

        virtual void setTemplateTable(IMacTimeslotTable* t){this->ownerp = t;}
        virtual void setTemplateId(int id) {TemplateId = id;}
        virtual void setCCAOffset(UINT_16 d) {TsCCAOffset = d;}
        virtual void setCCA(UINT_16 d){TsCCA = d;}
        virtual void setTxOffset(UINT_16 d){TsTxOffset = d;}
        virtual void setRxOffset(UINT_16 d){TsRxOffset = d;}
        virtual void setRxAckDelay(UINT_16 d){TsRxAckDelay = d;}
        virtual void setTxAckDelay(UINT_16 d){TsTxAckDelay = d;}
        virtual void setRxWait(UINT_16 d){TsRxWait = d;}
        virtual void setAckWait(UINT_16 d){TsAckWait = d;}
        virtual void setRxTx(UINT_16 d){TsRxTx = d;}
        virtual void setMaxAck(UINT_16 d){TsMaxAck = d;}
        virtual void setMaxTx(UINT_16 d) {TsMaxTx = d;}
        virtual void setTimeslotLength(UINT_16 d) {TsTimeslotLength = d;}
};

#endif /* MACTIMESLOTTABLEENTRY_H_ */

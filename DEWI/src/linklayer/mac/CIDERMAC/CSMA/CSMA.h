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

#ifndef CSMA_H_
#define CSMA_H_

#include <INETDefs.h>
#include <WirelessMacBase.h>
#include "Ieee802154eFrame_m.h"
#include "InterfaceEntry.h"
#include "DataCenter.h"
#include "NotificationBoard.h"
#include "RadioState.h"
#include "CIDERQueue.h"


class INET_API CSMA: public WirelessMacBase
{

    Ieee802154eFrame *sendPacket;
    PHYenum phystatus;
public:
    CSMA();
    virtual ~CSMA();

    virtual int numInitStages() const
    {
        return 4;
    }
    virtual void initialize(int stage);

    int mUpperLayerIn;
    int mUpperLayerOut;
    int mLowerLayerIn;
    int mLowerLayerOut;

protected:
    virtual void handleMessage(cMessage *msg);
    /** @brief Handle self messages such as timers */
    virtual void handleSelfMsg(cMessage *msg);

    /** @brief Handle packets from upper layer */
    virtual void handleUpperMsg(cPacket *msg);

    /** @brief Handle commands from upper layer */
    virtual void handleCommand(cMessage *msg);

    /** @brief Handle packets from lower layer */
    virtual void handleLowerMsg(cPacket *msg);

    virtual void handleCIDERMessage(cMessage *msg);

    virtual void sendDown(cMessage *msg);

    /** @brief Handle control messages from lower layer */
    virtual void handleLowerControl(cMessage *msg);

    virtual InterfaceEntry *createInterfaceEntry();
    /**
     * should clear queue and emit signal "dropPkFromHLIfaceDown" with entire packets
     */
    virtual void flushQueue();

    /**
     * should clear queue silently
     */
    virtual void clearQueue();

protected:
    bool coordinator;
    void commonInitialize();// common initializaton Ieee802154MAc csma802154



    DataCenter *center;

    CIDERQueue* queueModule;
    typedef std::list<Ieee802154eFrame*> MacQueue;

    /** @name Different tracked statistics.*/
    /*@{*/
    int nbRxFramesBroadcast;
    int nbTxFrames;
    int nbRxFrames;
    int nbMissedAcks;
    int nbRecvdAcks;
    int nbDroppedFrames;
    int nbTxAcks;
    int nbDuplicates;
    int nbBackoffs;
    double backoffValues;
    bool useIeee802Ctrl;
    /*@}*/

    /** @brief Records general statistics?*/
    bool stats;
    /** @brief Record out put vectors?*/
    bool trace;

    /** @brief MAC states
     * see states diagram.
     */
    enum t_mac_states
    {
        IDLE_1=1,
        BACKOFF_2,
        CCA_3,
        TRANSMITFRAME_4,
        WAITACK_5,
        WAITSIFS_6,
        TRANSMITACK_7

    };

    /*************************************************************/
    /****************** TYPES ************************************/
    /*************************************************************/

    /** @brief Kinds for timer messages.*/
    enum t_mac_timer
    {
        TIMER_NULL=0,
        TIMER_BACKOFF,
        TIMER_CCA,
        TIMER_SIFS,
        TIMER_RX_ACK,
    };

    /** @name Pointer for timer messages.*/
    /*@{*/
    cMessage * backoffTimer, * ccaTimer, * txTimer, * sifsTimer, * rxAckTimer;
    /*@}*/

    /** @brief MAC state machine events.
     * See state diagram.*/
    enum t_mac_event
    {
        EV_SEND_REQUEST=1,                   // 1, 11, 20, 21, 22
        EV_TIMER_BACKOFF,                    // 2, 7, 14, 15
        EV_FRAME_TRANSMITTED,                // 4, 19
        EV_ACK_RECEIVED,                     // 5
        EV_ACK_TIMEOUT,                      // 12
        EV_FRAME_RECEIVED,                   // 15, 26
        EV_DUPLICATE_RECEIVED,
        EV_TIMER_SIFS,                       // 17
        EV_BROADCAST_RECEIVED,           // 23, 24
        EV_TIMER_CCA
    };

    /** @brief Types for frames sent by the CSMA.*/
    enum t_csma_frame_types
    {
        DATA,
        ACK
    };

    enum t_mac_carrier_sensed
    {
        CHANNEL_BUSY=1,
        CHANNEL_FREE
    } ;

    enum t_mac_status
    {
        STATUS_OK=1,
        STATUS_ERROR,
        STATUS_RX_ERROR,
        STATUS_RX_TIMEOUT,
        STATUS_FRAME_TO_PROCESS,
        STATUS_NO_FRAME_TO_PROCESS,
        STATUS_FRAME_TRANSMITTED
    };

    /** @brief The different back-off methods.*/
    enum backoff_methods
    {
        /** @brief Constant back-off time.*/
        CONSTANT = 0,
        /** @brief Linear increasing back-off time.*/
        LINEAR,
        /** @brief Exponentially increasing back-off time.*/
        EXPONENTIAL,
    };

    /** @brief keep track of MAC state */
    t_mac_states macState;
    t_mac_status status;

    /** @brief Maximum time between a packet and its ACK
     *
     * Usually this is slightly more then the tx-rx turnaround time
     * The channel should stay clear within this period of time.
     */
    simtime_t sifs;
    int headerLength;
    double bitrate;
    /** @brief The amount of time the MAC waits for the ACK of a packet.*/
    simtime_t macAckWaitDuration;

    bool transmissionAttemptInterruptedByRx;
    /** @brief CCA detection time */
    simtime_t ccaDetectionTime;
    /** @brief Time to setup radio from sleep to Rx state */
    simtime_t rxSetupTime;
    /** @brief Time to switch radio from Rx to Tx state */
    simtime_t aTurnaroundTime;
    /** @brief maximum number of backoffs before frame drop */
    int macMaxCSMABackoffs;
    /** @brief maximum number of frame retransmissions without ack */
    unsigned int macMaxFrameRetries;
    /** @brief base time unit for calculating backoff durations */
    simtime_t aUnitBackoffPeriod;
    /** @brief Stores if the MAC expects Acks for Unicast packets.*/
    bool useMACAcks;

    /* @brief Random Number of Backoff retries*/
    bool randomBackoff;

    /** @brief Defines the backoff method to be used.*/
    backoff_methods backoffMethod;

    /**
     * @brief Minimum backoff exponent.
     * Only used for exponential backoff method.
     */
    int macMinBE;
    /**
     * @brief Maximum backoff exponent.
     * Only used for exponential backoff method.
     */
    int macMaxBE;

    /** @brief initial contention window size
     * Only used for linear and constant backoff method.*/
    double initialCW;

    /** @brief The power (in mW) to transmit with.*/
    double txPower;

    /** @brief number of backoff performed until now for current frame */
    int NB;

    /** @brief A queue to store packets from upper layer in case another
		    packet is still waiting for transmission..*/


    /** @brief length of the queue*/
    unsigned int queueLength;

    /** @brief count the number of tx attempts
     *
     * This holds the number of transmission attempts for the current frame.
     */
    unsigned int txAttempts;


    /** @brief plus category from BB */
    int catDroppedPacket;

    /** @brief publish dropped packets nic wide */
    int nicId;

    /** @brief The bit length of the ACK packet.*/
    int ackLength;



    void sendNewPacketInTx(Ieee802154eFrame *p)
    {
        if (sendPacket)
            error("the previous packet is not send yet");
        if (phystatus==phy_TX_ON)
            sendDown(p);
        else
            sendPacket = p;
    }

protected:
    // FSM functions
    void fsmError(t_mac_event event, cMessage *msg);
    void executeMac(t_mac_event event, cMessage *msg);
    void updateStatusIdle(t_mac_event event, cMessage *msg);
    void updateStatusBackoff(t_mac_event event, cMessage *msg);
    void updateStatusCCA(t_mac_event event, cMessage *msg);
    void updateStatusTransmitFrame(t_mac_event event, cMessage *msg);
    void updateStatusWaitAck(t_mac_event event, cMessage *msg);
    void updateStatusSIFS(t_mac_event event, cMessage *msg);
    void updateStatusTransmitAck(t_mac_event event, cMessage *msg);
    void updateStatusNotIdle(cMessage *msg);
    void manageQueue();
    void updateMacState(t_mac_states newMacState);


    //void attachSignal(Ieee802154Frame* mac, simtime_t startTime);
    void manageMissingAck(t_mac_event event, cMessage *msg);
    void startTimer(t_mac_timer timer);

    virtual double scheduleBackoff();
    virtual void sendUp(cMessage *msg);

    virtual cPacket *decapsMsg(Ieee802154eFrame * macPkt);
    Ieee802154eFrame * ackMessage;

    //sequence number for sending, map for the general case with more senders
    //also in initialisation phase multiple potential parents
    //std::map<int, unsigned long> SeqNrParent; //parent -> sequence number
    std::map<MACAddress, uint8_t> SeqNrParent; //parent -> sequence number

    //sequence numbers for receiving
    //std::map<int, unsigned long> SeqNrChild; //child -> sequence number
    std::map<MACAddress, uint8_t> SeqNrChild; //child -> sequence number


    /*
     * IEEE802154 related Functions
     */

protected:
    virtual void    initializeQueueModule();
    virtual double    getRate(char);
    virtual bool    toParent(Ieee802154eFrame*);

    /** @brief calculate byte length of frame of certain type */
    virtual int    calFrmByteLength(Ieee802154eFrame*);

    /** @brief calculate byte length of frame header  */
    virtual int    calMHRByteLength(uint8_t addrModeSrc, uint8_t addrModeDst);

    /** @brief calculate duration of the frame transmitted over wireless channel  */
    virtual simtime_t    calDuration(Ieee802154eFrame*);

    MACAddress configurationMacAddress();
    IE3ADDR getMacAddr() {return aExtendedAddress;}

    UINT_16 getShortAddress(MACAddress v) {return (UINT_16) v.getInt();}
    MACAddress getLongAddress(UINT_16 v);
    int getRadioModuleId() {return radioModule;}



    virtual void    PLME_SET_TRX_STATE_request(PHYenum state);


    /*
     * IEEE802154 related Variables
     */

protected:

    IE3ADDR aExtendedAddress;
    MACAddress macaddress;
    int radioModule;
    InterfaceEntry *iface;
    RadioState::State radioState;

    NotificationBoard* mpNb;

    PHYenum trx_state_req;

    /** @brief MAC PIB attributes */
    MAC_PIB mpib;
    /** @brief PHY PIB attributes copied from PHY via notificationboard */
    PHY_PIB ppib, tmp_ppib;

    /** @brief current bit rate at PHY */
    double phy_bitrate;
};

#endif /* CSMA_H_ */

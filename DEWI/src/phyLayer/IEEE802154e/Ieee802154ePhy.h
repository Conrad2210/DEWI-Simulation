//******************************
//LQI measurement not implemented
//

#ifndef IEEE_802154_E_PHY_H
#define IEEE_802154_E_PHY_H

#include "Radio.h"
#include "RadioState.h"
#include "Ieee802154eConst.h"
#include "Ieee802154eDef.h"
#include "Ieee802154eEnum.h"
#include "Ieee802154eField.h"
#include "Ieee802154eMacPhyPrimitives_m.h"
#include "AirFrame_m.h"
#include "IRadioModel.h"
#include "IReceptionModel.h"
#include "FWMath.h"
#include "NotificationBoard.h"
#include "NotifierConsts.h"
#include "ObstacleControl.h"
#include "ILifecycle.h"

/**
 * IEEE 802.15.4 Physical (PHY) Layer
 * Refer to IEEE Std 802.15.4-2006, IEEE Std 802.15.4-2011 & IEEE Std 802.15.4e-2012
 *
 * @author ?
 * @ingroup phyLayer
 *
 * modified
 * @author Stefan Reis
 * @author Conrad Dandelski
 */

class INET_API Ieee802154ePhy : public ChannelAccess, public ILifecycle
{
    public:
        static uint16_t aMaxPHYPacketSize; //max PSDU size (in bytes) the PHY shall be able to receive
        static uint16_t aTurnaroundTime; //Rx-to-Tx or Tx-to-Rx max turnaround time (in symbol period)
        static uint16_t aMaxBeaconOverhead; //max # of octets added by the MAC sublayer to the payload of its beacon frame
        static uint16_t aMaxBeaconPayloadLength;
        static uint16_t aMaxMACFrameSize;
        static uint16_t aMaxFrameOverhead;
        static uint16_t aMinMPDUOverhead;
        static uint16_t aMaxMACPayloadSize;

    public:
        Ieee802154ePhy();
        virtual ~Ieee802154ePhy();

    protected:
        static simsignal_t changeLevelNoise80215;
        virtual void receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj);
        virtual void initialize(int);
        virtual int numInitStages() const { return 3; }
        virtual void finish();

    // message handle functions
        virtual void handleMessage(cMessage*);
        virtual void handleUpperMsg(AirFrame*);
        virtual void handleSelfMsg(cMessage*);
        virtual void handlePrimitive(int, cMessage*);
        virtual void handleLowerMsgStart(AirFrame*);
        virtual void handleLowerMsgEnd(AirFrame*);
        virtual AirFrame* encapsulatePacket(cMessage*);
        virtual void bufferMsg(AirFrame*);
        virtual AirFrame* unbufferMsg(cMessage*);
        virtual void sendUp(cMessage*);
        virtual void sendDown(AirFrame*);

        virtual IReceptionModel *createReceptionModel() { return (IReceptionModel *) createOne(par("attenuationModel").stringValue()); }
        virtual IRadioModel *createRadioModel() { return (IRadioModel *) createOne(par("radioModel").stringValue()); }

        // primitives processing functions
        virtual void handle_PD_DATA_request(UINT_8 psduLength, cMessage* psdu);
        virtual void PD_DATA_confirm(PHYenum status);
       // virtual void PD_DATA_confirm(PHYenum status, short);
        virtual void PD_DATA_indication(UINT_8 psduLength, cMessage* psdu, UINT_8 ppduLinkQuality);
        virtual void handle_PLME_CCA_request();
        virtual void PLME_CCA_confirm(PHYenum status);
        virtual void handle_PLME_ED_request();
        virtual void PLME_ED_confirm(PHYenum status, UINT_8 energyLevel);
        virtual void handle_PLME_GET_request(uint8_t pibAttribute);
        virtual void PLME_GET_confirm(PHYenum status, PHYPIBenum pibAttribute, PHY_PIB pibAttributeValue);
        virtual void handle_PLME_SET_TRX_STATE_request(PHYenum setState);
        virtual void PLME_SET_TRX_STATE_confirm(PHYenum status);

//FIXME old version [SR]        virtual void handle_PLME_SET_request(Ieee802154eMacPhyPrimitives *primitive);
        virtual void handle_PLME_SET_request(PHYPIBenum pibAttribute, PHY_PIB pibAttributeValue);
        virtual void PLME_SET_confirm(PHYenum status, PHYPIBenum attribute);
        virtual void PLME_bitRate(double bitRate);


        virtual void setRadioState(RadioState::State newState);
        virtual int getChannelNumber() const { return rs.getChannelNumber(); }
        virtual void addNewSnr();
        virtual void changeChannel(int newChannel);
        virtual bool channelSupported(int channel);
        virtual uint16_t calculateEnergyLevel();
        virtual double getRate(char dataOrSymbol);
        virtual bool processAirFrame(AirFrame*);


        virtual void disconnectTransceiver() {transceiverConnect = false;}
        virtual void connectTransceiver() {transceiverConnect = true;}
        virtual void disconnectReceiver();
        virtual void connectReceiver();


  protected:
        INoiseGenerator *noiseGenerator;
        bool transceiverConnect;
        bool receiverConnect;
        bool m_debug;               // debug switch
        IRadioModel* radioModel;
        IReceptionModel* receptionModel;
        ObstacleControl* obstacles;

        int upperLayerOut;
        int upperLayerIn;

        double transmitterPower;    // in mW
        double noiseLevel;
        double carrierFrequency;
        double sensitivity;         // in mW
        double receptionThreshold;
        double thermalNoise;

        struct SnrStruct
        {
            /** @brief pointer to the message this information belongs to */
                AirFrame* ptr;

                /** @brief < received power of the message */
                double rcvdPower;

                /** @brief < stores SNR over time */
                SnrList sList;
        };

        /** @brief stores the snrList and the the recvdPower for the
         *  message currently being received, together with a pointer to the message */
        SnrStruct snrInfo;

        struct Compare {
            bool operator() (AirFrame* const &lhs, AirFrame* const &rhs) const {
                ASSERT(lhs && rhs);
                return lhs->getId() < rhs->getId();
            }
        };
        typedef std::map<AirFrame*, double, Compare> RecvBuff;

        /** @brief A buffer to store a pointer to a message and the related receive power */
        RecvBuff recvBuff;

        /** @brief duplicated outgoing pkt, accessing encapsulated msg only when transmitter is forcibly turned off
         *  set a error flag into the encapsulated msg to inform all nodes rxing this pkt that the transmition is terminated and the pkt is corrupted
         *  use the new feature "Reference counting" of encapsulated messages */
        AirFrame* txPktCopy;

        //AirFrame *rxPkt;

        /** @brief accumulated received power in each channel, for ED measurement purpose */
        double rxPower[27];

        /** @brief peak power in current channle, for ED measurement purpose */
        double rxPeakPower;

        int numCurrRx;

        /** @brief four states model: idle, rxing, txing, sleep */
        RadioState rs;

        /** @brief three states model, according to spec: RX_ON, TX_ON, TRX_OFF */
        PHYenum phyRadioState;

        PHYenum newState;

        PHYenum newState_turnaround;

        /** @brief indicating whether channel is idle at the starting of CCA */
        bool isCCAStartIdle;

        /** @brief default PHY PIB Attributes */
        static PHY_PIB PHYPIB;

        /** @brief MAC PIB attributes */
        PHY_PIB phypib;

        /**
        * @name Timer messages
        */
        //@{
        /** @brief timer for CCA, delay 8 symbols */
        cMessage* CCA_timer;

        /** @brief timer for ED measurement */
        cMessage* ED_timer;

        /** @brief timer for Tx2Rx turnaround */
        cMessage* TRX_timer;

        /** @brief timer for TX over */
        cMessage* TxOver_timer;
        //@}

        virtual void registerBattery();
        virtual void updateDisplayString();
        virtual double calcDistFreeSpace();
        bool drawCoverage;
        cMessage *updateString;
        simtime_t updateStringInterval;

        /**
        * @name Statistical variables
        */
        //@{
        /** @brief number of packets received correctly, counted in <handleLowerMsgEnd(AirFrame * airframe)> */
        unsigned long numReceivedCorrectly;

        /** @brief number of packets given up, counted in <handleLowerMsgEnd(AirFrame * airframe)> */
        unsigned long numGivenUp;

        /** @brief number of loss rate, counted in <handleLowerMsgEnd(AirFrame * airframe)> */
        double lossRate;
        //@}

        static simsignal_t bitrateSignal;
        static simsignal_t radioStateSignal;    //enum
        static simsignal_t channelNumberSignal;
        static simsignal_t lossRateSignal;
        virtual bool handleOperationStage(LifecycleOperation *operation, int stage, IDoneCallback *doneCallback);

};

#endif

#include "Ieee802154ePhy.h"
#include "BasicBattery.h"
#include "PhyControlInfo_m.h"
#include "Radio80211aControlInfo_m.h"
#include "NodeStatus.h"
#include "NodeOperations.h"

#define MIN_DISTANCE 0.001 // minimum distance 1 millimeter
// #undef EV
//#define EV (ev.isDisabled() || !m_debug) ? std::cout : ev  ==> EV is now part of <omnetpp.h>

simsignal_t Ieee802154ePhy::bitrateSignal = SIMSIGNAL_NULL;
simsignal_t Ieee802154ePhy::radioStateSignal = SIMSIGNAL_NULL;
simsignal_t Ieee802154ePhy::channelNumberSignal = SIMSIGNAL_NULL;
simsignal_t Ieee802154ePhy::lossRateSignal = SIMSIGNAL_NULL;

/** Default PHY PIB Attributes */
PHY_PIB Ieee802154ePhy::PHYPIB =
{
        def_phyCurrentChannel,              def_phyChannelsSupported,
        0 /*phyTransmitPower - Std 802.15.4-2006 (table 23) page 46*/,
        0 /*phyTXPowerTolerance - Std 802.15.4-2011 (table 71) page 156 */,
        0 /*phyTXPower - Std 802.15.4-2011 (table 71) page 156 */,
        def_phyCCAMode,                     def_phyCurrentPage,
        0 /*phyMaxFrameDuration*/,          0 /*phySHRDuration*/,
        0 /*phySymbolsPerOctet*/,           0 /*phyPreambleSymbolLength - used for UWB PHY*/,
        0 /*phyUWBDataRatesSupported*/,     def_phyCSSLowDataRateSupported,
        def_phyUWBCoUSupported,             def_phyUWBCSSupported,
        def_phyUWBLCPSupported,             0 /*phyUWBCurrentPulseShape*/,
        0 /*phyUWBCoUpulse*/,               0 /*phyUWBCSpulse*/,
        0 /*phyUWBLCPWeight1*/,             0 /*phyUWBLCPWeight2*/,
        0 /*phyUWBLCPWeight3*/,             0 /*phyUWBLCPWeight4*/,
        0 /*phyUWBLCPDelay2*/,              0 /*phyUWBLCPDelay3*/,
        0 /*phyUWBLCPDelay4*/,              def_phyRanging,
        def_phyRangingCrystalOffset,        def_phyRangingDPS,
        def_phyCurrentCode,                 def_phyNativePRF,
        def_phyUWBScanBinsPerChannel,       def_phyUWBInsertedPreambleInterval,
        0 /*phyTXRMARKEROffset*/,           0 /*phyRXRMARKEROffset*/,
        0 /*phyRFRAMEProcessingTime*/,      0 /*def_phyCCADuration*/
};

Define_Module(Ieee802154ePhy);

uint16_t Ieee802154ePhy::aMaxPHYPacketSize      = 127;      //max PSDU size (in bytes) the PHY shall be able to receive
uint16_t Ieee802154ePhy::aTurnaroundTime        = 12;       //Rx-to-Tx or Tx-to-Rx max turnaround time (in symbol period)
uint16_t Ieee802154ePhy::aMaxBeaconOverhead     = 75;       //max # of octets added by the MAC sublayer to the payload of its beacon frame
uint16_t Ieee802154ePhy::aMaxBeaconPayloadLength = aMaxPHYPacketSize - aMaxBeaconOverhead;       //max size, in octets, of a beacon payload
uint16_t Ieee802154ePhy::aMaxFrameOverhead      = 25;       //max # of octets added by the MAC sublayer to its payload w/o security.//max # of octets that can be transmitted in the MAC frame payload field
uint16_t Ieee802154ePhy::aMaxMACFrameSize       = aMaxPHYPacketSize - aMaxFrameOverhead;
uint16_t Ieee802154ePhy::aMinMPDUOverhead       = 9;
uint16_t Ieee802154ePhy::aMaxMACPayloadSize     = aMaxPHYPacketSize - aMinMPDUOverhead;
simsignal_t Ieee802154ePhy::changeLevelNoise80215 = SIMSIGNAL_NULL;

#define BASE_NOISE_LEVEL (noiseGenerator?noiseLevel+noiseGenerator->noiseLevel():noiseLevel)


Ieee802154ePhy::Ieee802154ePhy() : rs(this->getId())
{
    radioModel              = NULL;
    receptionModel          = NULL;
    CCA_timer               = NULL;
    ED_timer                = NULL;
    TRX_timer               = NULL;
    TxOver_timer            = NULL;
    updateString            = NULL;
    transceiverConnect      = true;
    receiverConnect         = true;
    numReceivedCorrectly    = 0;
    numGivenUp              = 0;
    noiseGenerator          = NULL;
}

void Ieee802154ePhy::registerBattery()
{
    BasicBattery *bat = BatteryAccess().getIfExists();
    if (bat)
    {
        //int id,double mUsageRadioIdle,double mUsageRadioRecv,double mUsageRadioSend,double mUsageRadioSleep)=0;
        // read parameters
        double mUsageRadioIdle      = par("usage_radio_idle");
        double mUsageRadioRecv      = par("usage_radio_recv");
        double mUsageRadioSleep     = par("usage_radio_sleep");
        double mTransmitterPower    = par("transmitterPower");

        double trans_power_dbm = 10*log10(mTransmitterPower);
        // calculation of usage_radio_send
        // based on the values in Olaf Landsiedel's AEON paper
        /*if (trans_power_dbm <= -18)
            mUsageRadioSend = 8.8;
        else if (trans_power_dbm <= -13)
            mUsageRadioSend = 9.8;
        else if (trans_power_dbm <= -10)
            mUsageRadioSend = 10.4;
        else if (trans_power_dbm <= -6)
            mUsageRadioSend = 11.3;
        else if (trans_power_dbm <= -2)
            mUsageRadioSend = 15.6;
        else if (trans_power_dbm <= 0)
            mUsageRadioSend = 17;
        else if (trans_power_dbm <= 3)
            mUsageRadioSend = 20.2;
        else if (trans_power_dbm <= 4)
            mUsageRadioSend = 22.5;
        else if (trans_power_dbm <= 5)
            mUsageRadioSend = 26.9;
        else
            error("[Battery]: transmitter Power too high!");*/

        // based on the values for CC2420 in howitt paper
        double mUsageRadioSend;
        if (trans_power_dbm <= -25)
            mUsageRadioSend = 8.53;
        else if (trans_power_dbm <= -15)
            mUsageRadioSend = 9.64;
        else if (trans_power_dbm <= -10)
            mUsageRadioSend = 10.68;
        else if (trans_power_dbm <= -7)
            mUsageRadioSend = 11.86;
        else if (trans_power_dbm <= -5)
            mUsageRadioSend = 13.11;
        else if (trans_power_dbm <= -3)
            mUsageRadioSend = 14.09;
        else if (trans_power_dbm <= -1)
            mUsageRadioSend = 15.07;
        else if (trans_power_dbm <= 0)
            mUsageRadioSend = 16.24;
        else
            error("[Battery]: transmitter Power too high!");
        bat->registerWirelessDevice(rs.getRadioId(),mUsageRadioIdle,mUsageRadioRecv,mUsageRadioSend,mUsageRadioSleep);
    }
}

Ieee802154ePhy::~Ieee802154ePhy()
{
    cancelAndDelete(CCA_timer);
    cancelAndDelete(ED_timer);
    cancelAndDelete(TRX_timer);
    cancelAndDelete(TxOver_timer);
    delete radioModel;
    delete receptionModel;
    if (txPktCopy != NULL)
        delete txPktCopy;

    // delete messages being received
    for (RecvBuff::iterator it = recvBuff.begin(); it!=recvBuff.end(); ++it)
        delete it->first;
}

void Ieee802154ePhy::initialize(int stage)
{
    ChannelAccess::initialize(stage);

    EV << getParentModule()->getParentModule()->getFullName() << ": initializing Ieee802154ePhy, stage=" << stage << endl;

    if (stage == 0)
    {
        upperLayerIn = findGate("upperLayerIn");
        upperLayerOut = findGate("upperLayerOut");
        gate("radioIn")->setDeliverOnReceptionStart(true);

        // initialize PHY PIB attributes
        phypib = PHYPIB;

        // The following parameters to be specified in omnetpp.ini
        m_debug = par("debug");
        rs.setChannelNumber(par("channelNumber").longValue()); // default: 11, 2.4G
        phypib.phyCurrentChannel = par("channelNumber");
        // carrierFrequency     = cc->par("carrierFrequency");  // taken from ChannelControl
        carrierFrequency = par("carrierFrequency");
        sensitivity             = FWMath::dBm2mW(par("sensitivity").doubleValue()); // -85 dBm for 2450 MHz, -92 dBm for 868/915 MHz
        thermalNoise            = FWMath::dBm2mW(par("thermalNoise").doubleValue());

        phypib.phyTransmitPower = par("transmitterPower");  // in mW - for Std 802.15.4-2006
        phypib.phyTXPower       = phypib.phyTransmitPower;   // in mW - for Std 802.15.4-2011

        if (phypib.phyTransmitPower > (double) getChannelControlPar("pMax"))
            error("[PHY]: transmitterPower cannot be bigger than pMax in ChannelControl!");

        receptionThreshold = sensitivity;

        if (par("setReceptionThreshold").boolValue())
        {
            receptionThreshold = FWMath::dBm2mW(par("receptionThreshold").doubleValue());
            if (par("maxDistantReceptionThreshold").doubleValue() > 0)
            {
                receptionThreshold = receptionModel->calculateReceivedPower(transmitterPower, carrierFrequency, par("maxDistantReceptionThreshold").doubleValue());
            }
        }

        std::string noiseModel =  par("NoiseGenerator").stdstringValue();
        if (noiseModel!="")
        {
            noiseGenerator = (INoiseGenerator *) createOne(noiseModel.c_str());
            noiseGenerator->initializeFrom(this);
            // register to get a notification when position changes
            changeLevelNoise80215 = registerSignal("changeLevelNoise80215");
            subscribe(changeLevelNoise80215, this); // the INoiseGenerator must send a signal to this module
        }

        // initialize noiseLevel
        noiseLevel  = thermalNoise;
        snrInfo.ptr = NULL;
        txPktCopy   = NULL;
        //rxPkt = NULL;
        for (int i = 0; i < 27; i++)
        {
            rxPower[i] = 0;     // remember to clear after channel switching
        }
        rxPeakPower = 0;
        numCurrRx   = 0;

        phyRadioState = phy_TRX_OFF; // will be set in the MAC initialization

//        phyRadioState = phy_RX_ON;
        PLME_SET_TRX_STATE_confirm(phyRadioState);
//        rs.setState(RadioState::IDLE);

        rs.setState(RadioState::OFF); // will be set in the MAC initialization

        if (BASE_NOISE_LEVEL >= sensitivity)
                    rs.setState(RadioState::RECV);

        //rs.setChannelNumber((int)def_phyCurrentChannel); // default: 11, 2.4G
        rs.setBitrate(getRate('b'));
        rs.setRadioId(this->getId());

        newState = phy_TRX_OFF;
        newState_turnaround = phy_TRX_OFF;

//        newState = phy_IDLE;
//        newState_turnaround = phy_IDLE;

        isCCAStartIdle = false;

        // initialize self messages (timer)
        CCA_timer       = new cMessage("CCA_timer",     PHY_CCA_TIMER);
        ED_timer        = new cMessage("ED_timer",      PHY_ED_TIMER);
        TRX_timer       = new cMessage("TRX_timer",     PHY_TRX_TIMER);
        TxOver_timer    = new cMessage("TxOver_timer",  PHY_TX_OVER_TIMER);


        obstacles = ObstacleControlAccess().getIfExists();
        if (obstacles) EV << "Found ObstacleControl" << endl;

        // this is the parameter of the channel controller (global)
        std::string propModel = getChannelControlPar("propagationModel").stdstringValue();
        if (propModel == "")
            propModel = "FreeSpaceModel";

        receptionModel = (IReceptionModel *) createOne(propModel.c_str());
        receptionModel->initializeFrom(this);

        // radio model to handle frame length and reception success calculation (modulation, error correction etc.)
        std::string rModel =  par("radioModel").stdstringValue();
        if (rModel=="")
            rModel = "GenericRadioModel";

        radioModel = (IRadioModel *) createOne(rModel.c_str());
        radioModel->initializeFrom(this);

        radioModel = createRadioModel();
        radioModel->initializeFrom(this);
        bitrateSignal = registerSignal("bitrate");
        radioStateSignal = registerSignal("radioState");
        channelNumberSignal = registerSignal("channelNo");
        lossRateSignal = registerSignal("lossRate");
        bool change = false;

        if (par("aMaxPHYPacketSize").longValue() != aMaxPHYPacketSize)
        {
            change = true;
            aMaxPHYPacketSize = par("aMaxPHYPacketSize").longValue();
        }

        if (aTurnaroundTime != par("aTurnaroundTime").longValue())
        {
            change = true;
            aTurnaroundTime = par("aTurnaroundTime").longValue();
        }

        if (aMaxBeaconOverhead != par("aMaxBeaconOverhead").longValue())
        {
            change = true;
            aMaxBeaconOverhead = par("aMaxBeaconOverhead").longValue();
        }

        if (aMaxFrameOverhead != par("aMaxFrameOverhead").longValue())
        {
            change = true;
            aMaxFrameOverhead = par("aMaxFrameOverhead").longValue();
        }
        if (change)
        {
            aMaxBeaconPayloadLength = aMaxPHYPacketSize - aMaxBeaconOverhead;       //max size, in octets, of a beacon payload
            aMaxMACFrameSize  = aMaxPHYPacketSize - aMaxFrameOverhead;
        }

    }
    else if (stage == 1)
    {
        // tell initial values to MAC; must be done in stage 1, because they
        // subscribe in stage 0
        nb->fireChangeNotification(NF_RADIOSTATE_CHANGED, &rs);
        nb->fireChangeNotification(NF_RADIO_CHANNEL_CHANGED, &rs);
        nb->fireChangeNotification(NF_PHY_PKT_DISCARDED, &rs);
    }
    else if (stage == 2)
    {
        // tell initial channel number to ChannelControl; should be done in
        // stage == 2 or later, because base class initializes myHostRef in that stage

       	cc->setRadioChannel(myRadioRef, rs.getChannelNumber());

        // statistics
        emit(bitrateSignal, rs.getBitrate());
        emit(radioStateSignal, rs.getState());
        emit(channelNumberSignal, rs.getChannelNumber());

        if (this->hasPar("drawCoverage"))
            drawCoverage = par("drawCoverage");
        else
            drawCoverage = false;

        registerBattery();
        this->updateDisplayString();

        if (this->hasPar("refresCoverageInterval"))
        	updateStringInterval = par("refresCoverageInterval");
        else
        	updateStringInterval = 0;

        WATCH(rs);
        WATCH(phyRadioState);
    }
}

void Ieee802154ePhy::finish()
{
}

bool Ieee802154ePhy::processAirFrame(AirFrame *airframe)
{
    int chnum = airframe->getChannelNumber();
    return (chnum == getChannelNumber());
}

void Ieee802154ePhy::handleMessage(cMessage *msg)
{
    // handle primitives
    if (updateString && updateString == msg)
    {
        this->updateDisplayString();
        return;
    }
    if (!msg->isSelfMessage())
    {
        if (msg->getArrivalGateId() == upperLayerIn && (dynamic_cast<cPacket*>(msg) == NULL))
        {
            if (msg->getKind() == 0)
                error("[PHY]: Message '%s' with length == 0 is supposed to be a primitive, but msg kind is also zero", msg->getName());

            EV << "[PHY]: a primitive received from MAC layer, processing ..." << endl;
            handlePrimitive(msg->getKind(), msg);
            return;
        }
    }
    if (msg->getArrivalGateId() == upperLayerIn)
    {
        if (transceiverConnect)
        {   // PD_DATA_request
            EV << "[PHY]: a MAC frame " << msg->getName()  << " received from MAC layer" << endl;
            handle_PD_DATA_request(0, (cMessage*) msg);
        }
        else
        {
            EV << "[PHY]: a MAC frame " << msg->getName()  << " received from MAC layer but transceiver is disconnected, delete packet" << endl;
            delete msg;
        }
    }
    else if (msg->isSelfMessage())
    {
        handleSelfMsg(msg);
    }
    else if (processAirFrame (check_and_cast<AirFrame*>(msg)))
    {
        if (rs.getState() != RadioState::OFF && receiverConnect)
        {
            // must be an AirFrame
            AirFrame *airframe = (AirFrame *) msg;
            handleLowerMsgStart(airframe);
            bufferMsg(airframe);
        }
        else
        {
            EV << "[PHY]: message undetectable while receiver disenabled -- dropping it\n";
            delete msg;
        }
    }
    else
    {
        EV << "[PHY]: listening to a different channel when receiving message -- dropping it\n";
        delete msg;
    }
}

void Ieee802154ePhy::handlePrimitive(int msgkind, cMessage *msg)
{
    Ieee802154eMacPhyPrimitives *primitive = check_and_cast<Ieee802154eMacPhyPrimitives *>(msg);
    switch (msgkind)
    {
    case PD_DATA_REQUEST:   // FIXME: need to check [SR]
        EV <<"[PHY]: this is a PD_DATA_REQUEST" << endl;
        handle_PD_DATA_request(primitive->getPsduLength(), encapsulatePacket(msg));
        delete primitive;
        break;

    case PLME_CCA_REQUEST:
        EV <<"[PHY]: this is a PLME_CCA_REQUEST" << endl;
        handle_PLME_CCA_request();
        delete primitive;
        break;

    case PLME_ED_REQUEST:
        EV <<"[PHY]: this is a PLME_ED_REQUEST" << endl;
        handle_PLME_ED_request();
        delete primitive;
        break;

    case PLME_GET_REQUEST:
        EV <<"[PHY]: this is a PLME_GET_REQUEST" << endl;
        handle_PLME_GET_request(PHYPIBenum(primitive->getPibAttribute()));
        delete primitive;
        break;

    case PLME_SET_TRX_STATE_REQUEST:
        EV <<"[PHY]: this is a PLME_SET_TRX_STATE_REQUEST" << endl;
        handle_PLME_SET_TRX_STATE_request(PHYenum(primitive->getStatus()));
        delete primitive;
        break;

    case PLME_SET_REQUEST:
        EV <<"[PHY]: this is a PLME_SET_REQUEST" << endl;
        handle_PLME_SET_request(PHYPIBenum(primitive->getPibAttribute()), PHY_PIB(primitive->getPibAttributeValue()));
        delete primitive;
        break;

    case PLME_GET_BITRATE:
        PLME_bitRate(rs.getBitrate());
        break;

    default:
        error("[PHY]: unknown primitive received (msgkind=%d)", msgkind);
        break;
    }
}

AirFrame* Ieee802154ePhy::encapsulatePacket(cMessage *frame)
{
    //PhyControlInfo *ctrl = dynamic_cast<PhyControlInfo *>(frame->removeControlInfo());
    //ASSERT(!ctrl || ctrl->getChannelNumber()==-1); // per-packet channel switching not supported

    // Note: we don't set length() of the AirFrame, because duration will be used everywhere instead
    PhyControlInfo *ctrl = dynamic_cast<PhyControlInfo *>(frame->removeControlInfo());
    AirFrame*   airframe = new AirFrame();
    //XXX: [SR] this is a workaround to display the correct name of the packet (to detect data packet)
    const char* name = (strcmp(frame->getName(),"Ieee802154DATA") == 0)?PK(frame)->getEncapsulatedPacket()->getName():frame->getName();
    airframe->setName(name);
    airframe->setPSend(phypib.phyTransmitPower);
    airframe->setChannelNumber(getChannelNumber());
    airframe->setBitrate(rs.getBitrate());
    if (ctrl)
    {
        if (ctrl->getChannelNumber()>=0)
            airframe->setChannelNumber(ctrl->getChannelNumber());
        if (ctrl->getBitrate()>=0)
        {
            airframe->setBitrate(ctrl->getBitrate());
            if (rs.getBitrate()!=ctrl->getBitrate())
                rs.setBitrate(ctrl->getBitrate());
        }
        if (ctrl->getTransmitterPower()>=0)
        {
            if (ctrl->getTransmitterPower() <= (double)getChannelControlPar("pMax"))
               airframe->setPSend(ctrl->getTransmitterPower());
        }
        delete ctrl;
    }
    airframe->encapsulate(PK(frame));
    airframe->setDuration(radioModel->calculateDuration(airframe));
    airframe->setSenderPos(getRadioPosition());
    airframe->setCarrierFrequency(carrierFrequency);

    EV << "[PHY]: encapsulating " << name << " into an airframe" << endl;
    return airframe;
}

// XXX [SR] need to be test
void Ieee802154ePhy::handleUpperMsg(AirFrame *airframe)
{
    if (phyRadioState == phy_TX_ON)
    {
        if (par("forceIdle").boolValue())
           ASSERT(rs.getState() != RadioState::TRANSMIT);

        EV << "[PHY]: transmitter is on, start sending message ..." << endl;
        setRadioState(RadioState::TRANSMIT);
        ASSERT(txPktCopy == NULL);
        txPktCopy = (AirFrame *) airframe->dup();

        if (TxOver_timer->isScheduled())
        {
            error("[PHY]: try to transmit a packet while radio is Txing");
            PD_DATA_confirm(phy_BUSY_TX);
        }

        scheduleAt(simTime() + airframe->getDuration(), TxOver_timer);

        EV << "[PHY]: the transmission needs " << airframe->getDuration() << " s" << endl;
        sendDown(airframe);
    }
    else
    {
        error("[PHY]: transmitter is not ON while trying to send a %s msg to channel, radio is in %d state!", airframe->getName(), phyRadioState);

        if(phyRadioState == phy_RX_ON){
            PD_DATA_confirm(phy_RX_ON);
        }
        else if(phyRadioState == phy_TRX_OFF){
            PD_DATA_confirm(phy_TRX_OFF);
        }
    }
}

void Ieee802154ePhy::sendUp(cMessage *msg)
{
    if (receiverConnect)
    {
        EV << "[PHY]: sending received " << msg->getName() << " frame to MAC layer" << endl;
        send(msg, upperLayerOut);
    }
    else
    {
        EV << "[PHY]: a MAC frame " << msg->getName()  << " received from the radio layer but receiver is disconnected, delete packet" << endl;
        delete msg;
    }
}

void Ieee802154ePhy::sendDown(AirFrame *airframe)
{
    sendToChannel(airframe);
}

void Ieee802154ePhy::bufferMsg(AirFrame *airframe) //FIXME: add explicit simtime_t atTime arg?
{
    // set timer to indicate transmission is complete
    cMessage *endRxTimer = new cMessage("endRx", PHY_RX_OVER_TIMER);
    endRxTimer->setContextPointer(airframe);
    airframe->setContextPointer(endRxTimer);

    // NOTE: use arrivalTime instead of simTime, because we might be calling this
    // function during a channel change, when we're picking up ongoing transmissions
    // on the channel -- and then the message's arrival time is in the past!
    scheduleAt(airframe->getArrivalTime() + airframe->getDuration(), endRxTimer);
}

AirFrame* Ieee802154ePhy::unbufferMsg(cMessage *msg)
{
    AirFrame* airframe = (AirFrame *) msg->getContextPointer();
    //delete the self message
    delete msg;
    return airframe;
}

void Ieee802154ePhy::handleLowerMsgStart(AirFrame * airframe)
{
    // Calculate the receive power of the message

    // calculate distance
    const Coord& framePos = airframe->getSenderPos();
    double distance = getRadioPosition().distance(framePos);
    if (distance<MIN_DISTANCE)
        distance = MIN_DISTANCE;

    // calculate receive power
    AirFrame * airframeExt = dynamic_cast<AirFrame * >(airframe);
    double frequency = carrierFrequency;
    if (airframeExt && (airframeExt->getCarrierFrequency() > 0.0))
            frequency = airframeExt->getCarrierFrequency();

    double rcvdPower = receptionModel->calculateReceivedPower(airframe->getPSend(), frequency, distance);

    if (obstacles && distance > MIN_DISTANCE)
        rcvdPower = obstacles->calculateReceivedPower(rcvdPower, carrierFrequency, framePos, 0, getRadioPosition(), 0);

    airframe->setPowRec(rcvdPower);

    // accumulate receive power for each pkt received in current channel, no matter real pkt or noise
    rxPower[getChannelNumber()] += rcvdPower;

    // store the receive power in the recvBuff
    recvBuff[airframe] = rcvdPower;

    if (ED_timer->isScheduled()) // all packets received during ED measurement will be discarded by MAC layer, not here
        if (rxPeakPower < rxPower[getChannelNumber()])
            rxPeakPower = rxPower[getChannelNumber()];

    // if receive power is bigger than sensitivity and if not sending
    // and currently not receiving another message and the message has
    // arrived in time
    // NOTE: a message may have arrival time in the past here when we are
    // processing ongoing transmissions during a channel change
    if (airframe->getArrivalTime() == simTime() && rcvdPower >= sensitivity && rs.getState() != RadioState::TRANSMIT && snrInfo.ptr == NULL)
    {
        EV << "[PHY]: start receiving " << airframe->getName() << " frame ...\n";

        // Put frame and related SnrList in receive buffer
        SnrList snrList;
        snrInfo.ptr = airframe;
        snrInfo.rcvdPower = rcvdPower;
        snrInfo.sList = snrList;

        // add initial snr value
        addNewSnr();
        if (rs.getState() != RadioState::RECV)
        {
            // publish new RadioState
            EV << "publish new RadioState:RECV\n";
            setRadioState(RadioState::RECV);
        }
    }
    // receive power is too low or another message is being received
    else
    {
        EV << "[PHY]: frame " << airframe->getName() << " is just noise\n";
        //add receive power to the noise level
        noiseLevel += rcvdPower;

        // if a message is being received, add a new snr value
        if (snrInfo.ptr != NULL)
        {
            // update snr info for currently being received message
            EV << "[PHY]: adding new snr value to snr list of message being received\n";
            addNewSnr();
        }

        // update the RadioState if the noiseLevel exceeded the threshold
        // and the radio is currently not in receive or in send mode
        if (BASE_NOISE_LEVEL >= receptionThreshold &&  rs.getState() == RadioState::IDLE)
        {
            EV << "[PHY]: noise level is high, setting radio state to RECV\n";
            setRadioState(RadioState::RECV);
        }
    }
}

void Ieee802154ePhy::handleLowerMsgEnd(AirFrame * airframe)
{
    bool isCorrupt = false;
    bool isCollision = false;
    rxPower[getChannelNumber()] -= recvBuff[airframe];

    if (snrInfo.ptr == airframe)
    {
        // my receiver was turned off during reception
        if (airframe->getKind() == BITERROR_FORCE_TRX_OFF)
        {
            EV << "[PHY]: reception of " << airframe->getName() << " frame failed because MAC layer turned off the receiver forcibly during the reception, drop it \n";
            noiseLevel -= recvBuff[airframe];
            isCorrupt = true;
            // signal the MAC layer the frame discarded [SR]
            nb->fireChangeNotification(NF_PHY_PKT_DISCARDED, &rs);
        }
        // the sender of this pkt turned off the transmitter during transmission
        else if (airframe->getEncapsulatedPacket()->getKind() == BITERROR_FORCE_TRX_OFF)
        {
            EV << "[PHY]: reception of " << airframe->getName() << " frame failed because the sender turned off its transmitter during the transmission, drop it \n";
            isCorrupt = true;
            // signal the MAC layer the frame discarded [SR]
            nb->fireChangeNotification(NF_PHY_PKT_DISCARDED, &rs);
        }
        else
        {
            EV << "[PHY]: reception of " << airframe->getName() << " frame is over, preparing to send it to MAC layer\n";

            // get Packet and list out of the receive buffer:
            SnrList list;
            list = snrInfo.sList;
            double snirMin = list.begin()->snr;

            for (SnrList::const_iterator iter = list.begin(); iter != list.end(); iter++)
                if (iter->snr < snirMin)
                    snirMin = iter->snr;

            airframe->setSnr(10*log10(snirMin));
            PhyIndication frameInfo = radioModel->isReceivedCorrectly(airframe, list);

            if (frameInfo != FRAMEOK)
            {
                isCollision = true;
                // we cannot do this before decapsulation, because we only detect this packet collided, not all, refer to encapsulation msg
                //airframe->getEncapsulatedMsg()->setKind(COLLISION);
            }

            // we decapsulate here to set some flag
            cMessage *frame = airframe->decapsulate();
            frame->setKind(PACKETOK);
            Radio80211aControlInfo * cinfo = new Radio80211aControlInfo;    //FIXME: need to shift in the PHY class, to reduce the depency to other files [SR]
            cinfo->setSnr(airframe->getSnr());
            cinfo->setLossRate(-1);
            cinfo->setRecPow(airframe->getPowRec());
            frame->setControlInfo(cinfo);

            if (isCollision)
                frame->setKind(frameInfo);
            else if (CCA_timer->isScheduled())  // during CCA, tell MAC layer to discard this pkt
                frame->setKind(RX_DURING_CCA);

            if (frame->getKind() == PACKETOK)
                numReceivedCorrectly++;
            else
                numGivenUp++;

            if ( (numReceivedCorrectly+numGivenUp)%50 == 0)
            {
                lossRate = (double)numGivenUp/((double)numReceivedCorrectly+(double)numGivenUp);
                emit(lossRateSignal, lossRate);
                numReceivedCorrectly = 0;
                numGivenUp = 0;
            }

            PD_DATA_indication(0, (cMessage*)frame, 0); //TODO: LQI not implemented [SR]
        }

        // delete the pointer to indicate that no message is currently
        // being received and clear the list
        snrInfo.ptr = NULL;
        snrInfo.sList.clear();

        // delete the frame from the recvBuff
        recvBuff.erase(airframe);
        delete airframe;
    }
    // all other messages are noise
    else
    {
        EV << "[PHY]: reception of noise message " << airframe->getName() <<" is over, removing recvdPower from noiseLevel....\n";
        noiseLevel -= recvBuff[airframe]; // get the rcvdPower and subtract it from the noiseLevel
        recvBuff.erase(airframe);       // delete message from the recvBuff

        if (recvBuff.empty())
            noiseLevel = thermalNoise;

        // update snr info for message currently being received if any
        if (snrInfo.ptr != NULL)
            addNewSnr();

        delete airframe;    // message should be deleted

        // signal the MAC layer the frame discarded[SR]
        nb->fireChangeNotification(NF_PHY_PKT_DISCARDED, &rs);
    }

    // check the RadioState and update if necessary
    // change to idle if noiseLevel smaller than threshold and state was
    // not idle before
    // do not change state if currently sending or receiving a message!!!
    if (BASE_NOISE_LEVEL < receptionThreshold && phyRadioState == phy_RX_ON && rs.getState() == RadioState::RECV && snrInfo.ptr == NULL)
    {
        setRadioState(RadioState::IDLE);
        EV << "[PHY]: radio finishes receiving\n";
        if (newState != phy_IDLE)
        {
            newState_turnaround = newState;
            newState = phy_IDLE;
            if (newState_turnaround == phy_TRX_OFF)
            {
                phyRadioState = phy_TRX_OFF;
                setRadioState(RadioState::SLEEP);
                PLME_SET_TRX_STATE_confirm(phyRadioState);
            }
            else
            {
                // delay <aTurnaroundTime> symbols for Rx2Tx
                phyRadioState = phy_TRX_OFF;
                PLME_SET_TRX_STATE_confirm(phyRadioState);
                setRadioState(RadioState::SLEEP); // radio disabled during TRx turnaround
                if (TRX_timer->isScheduled())    cancelEvent(TRX_timer);
                scheduleAt(simTime() + aTurnaroundTime/getRate('s'), TRX_timer);
            }
        }
    }
}

void Ieee802154ePhy::handleSelfMsg(cMessage *msg)
{
    switch (msg->getKind())
    {
    case PHY_RX_OVER_TIMER:     // Rx over, dynamic timer
    {
        EV << "[PHY]: frame is completely received now\n";
        AirFrame* airframe = unbufferMsg(msg);  // unbuffer the message
        handleLowerMsgEnd(airframe);
        break;
    }
    case PHY_TX_OVER_TIMER:     // Tx over
    {
        EV << "[PHY]: transmitting of " << txPktCopy->getName() << " completes!" << endl;

        if (par("forceIdle").boolValue())
            setRadioState(RadioState::IDLE);

        delete txPktCopy;
        txPktCopy = NULL;

        EV << "[PHY]: send a PD_DATA_confirm with success to MAC layer" << endl;
        PD_DATA_confirm(phy_SUCCESS);
        //PD_DATA_confirm(phy_SUCCESS,TX_OVER); //FIXME: TX_OVER? [SR]

        // process radio and channel state switch
        if (newState != phy_IDLE)
        {
            newState_turnaround = newState;
            newState = phy_IDLE;
            if (newState_turnaround == phy_TRX_OFF)
            {
                phyRadioState = phy_TRX_OFF;

                //PLME_SET_TRX_STATE_confirm(phyRadioState);   // FIXME: this call is wrong (not in this way in the Std)[SR]

                setRadioState(RadioState::SLEEP);
            }
            else
            {
                // delay <aTurnaroundTime> symbols for Rx2Tx
                phyRadioState = phy_TRX_OFF;

                //PLME_SET_TRX_STATE_confirm(phyRadioState);      // FIXME: this call is wrong (not in this way in the Std)[SR]

                setRadioState(RadioState::SLEEP);       // radio disabled during TRx turnaround

                if (TRX_timer->isScheduled())
                    cancelEvent(TRX_timer);

                scheduleAt(simTime() + aTurnaroundTime/getRate('s'), TRX_timer);
            }
        }
        break;
    }
    case PHY_CCA_TIMER:     // perform CCA after delay 8 symbols
    {
        if (rs.getState() ==  RadioState::IDLE && isCCAStartIdle)
        {
            EV <<"[CCA]: CCA completes, channel is IDLE, reporting to MAC layer" << endl;
            PLME_CCA_confirm(phy_IDLE);
        }
        else
        {
            EV <<"[CCA]: CCA completes, channel is BUSY, reporting to MAC layer" << endl;
            PLME_CCA_confirm(phy_BUSY);
        }
        break;
    }
    case PHY_ED_TIMER:
    {
        PLME_ED_confirm(phy_SUCCESS, calculateEnergyLevel());
        break;
    }
    case PHY_TRX_TIMER:     // TRx turnaround over

    {   setRadioState(RadioState::IDLE);
        if(phyRadioState == newState_turnaround)
            PLME_SET_TRX_STATE_confirm(phyRadioState);      // FIXME: need to be test [SR]
        else
            PLME_SET_TRX_STATE_confirm(phy_SUCCESS);        // FIXME: need to be test [SR]

        // update the phyRadioState with the new state
        phyRadioState = newState_turnaround;
        break;
    }
    default:
        error("[PHY]: unknown PHY timer type!");
        break;
    }
}

//*******************************************************************
// PHY primitives processing
//*******************************************************************

void Ieee802154ePhy::handle_PD_DATA_request(UINT_8 psduLength, cMessage* psdu)
{
    // psduLength isn't used, is it only for the Std
    AirFrame *airframe = encapsulatePacket(psdu);
    handleUpperMsg(airframe);
}

void Ieee802154ePhy::PD_DATA_confirm(PHYenum status)
{
    Ieee802154eMacPhyPrimitives *primitive = new Ieee802154eMacPhyPrimitives();
    primitive->setKind(PD_DATA_CONFIRM);
    primitive->setStatus(status);

    primitive->setBitRate(rs.getBitrate());     //FIXME: why we need the bitrate? [SR]

    switch(status)
    {
    case phy_SUCCESS:
        EV << "[PHY]: sending a PD_DATA_confirm with < phy_SUCCESS > to MAC" << endl;
        break;
    case phy_RX_ON:
        EV << "[PHY]: sending a PD_DATA_confirm with < phy_RX_ON > to MAC" << endl;
        break;
    case phy_TRX_OFF:
        EV << "[PHY]: sending a PD_DATA_confirm with < phy_TRX_OFF > to MAC" << endl;
        break;
    case phy_BUSY_TX:
        EV << "[PHY]: sending a PD_DATA_confirm with < phy_BUSY_TX > to MAC" << endl;
        break;
    default:
        EV << "[PHY]: sending a PD_DATA_confirm with " << status << " to MAC" << endl;
        break;
    }

    send(primitive, upperLayerOut);
}

// XXX: LQI is not implemented [Sr]; psduLength are not implemented, it is only for the Std
void Ieee802154ePhy::PD_DATA_indication(UINT_8 psduLength, cMessage* psdu, UINT_8 ppduLinkQuality)
{
    sendUp(psdu);
    EV << "[PHY]: sending a PD_DATA_indication to MAC" << endl;
}

//--- PHY management service ---

void Ieee802154ePhy::PLME_CCA_confirm(PHYenum status)
{
    // called by the Ieee802154ePhy::handleSelfMsg(cMessage *msg)
    Ieee802154eMacPhyPrimitives *primitive = new Ieee802154eMacPhyPrimitives();
    primitive->setKind(PLME_CCA_CONFIRM);
    primitive->setStatus(status);

    primitive->setBitRate(rs.getBitrate());     //FIXME: why we need the bitrate? [SR]

    EV << "[PHY]: sending a PLME_CCA_confirm with " << status << " to MAC" << endl;
    send(primitive, upperLayerOut);
}

void Ieee802154ePhy::PLME_ED_confirm(PHYenum status, UINT_8 energyLevel)
{
    // called by the Ieee802154ePhy::handleSelfMsg(cMessage *msg)
    Ieee802154eMacPhyPrimitives *primitive = new Ieee802154eMacPhyPrimitives();
    primitive->setKind(PLME_ED_CONFIRM);
    primitive->setStatus(status);
    primitive->setEnergyLevel(energyLevel);

    primitive->setBitRate(rs.getBitrate());     //FIXME: why we need the bitrate? [SR]

    EV << "[PHY]: sending a PLME_ED_confirm with " << status << " to MAC" << endl;
    send(primitive, upperLayerOut);
}

void Ieee802154ePhy::PLME_GET_confirm(PHYenum status, PHYPIBenum pibAttribute, PHY_PIB pibAttributeValue){
    // called by the Ieee802154ePhy::handle_PLME_GET_request(uint8_t pibAttribute)
    Ieee802154eMacPhyPrimitives *primitive = new Ieee802154eMacPhyPrimitives();
    primitive->setKind(PLME_GET_CONFIRM);
    primitive->setStatus(status);
    primitive->setPibAttribute(pibAttribute);
    primitive->setPibAttributeValue(pibAttributeValue);

    primitive->setBitRate(rs.getBitrate());     //FIXME: why we need the bitrate? [SR]

    EV << "[PHY]: sending a PLME_GET_confirm with " << status << " to MAC" << endl;
    send(primitive, upperLayerOut);
}

void Ieee802154ePhy::PLME_SET_TRX_STATE_confirm(PHYenum status)
{
    // called by the Ieee802154ePhy::handleSelfMsg(cMessage *msg)
    Ieee802154eMacPhyPrimitives *primitive = new Ieee802154eMacPhyPrimitives();
    primitive->setKind(PLME_SET_TRX_STATE_CONFIRM);
    primitive->setStatus(status);

    primitive->setBitRate(rs.getBitrate());     //FIXME: why we need the bitrate? [SR]

    switch (status)
    {
        case phy_RX_ON:
            EV << "[PHY]: sending a PLME_SET_TRX_STATE_confirm with < phy_RX_ON > to MAC" << endl;
            break;

        case phy_TRX_OFF:
            EV << "[PHY]: sending a PLME_SET_TRX_STATE_confirm with < phy_TRX_OFF > to MAC" << endl;
            break;

        case phy_FORCE_TRX_OFF:
            EV << "[PHY]: sending a PLME_SET_TRX_STATE_confirm with < phy_FORCE_TRX_OFF > to MAC" << endl;
            break;

        case phy_TX_ON:
            EV << "[PHY]: sending a PLME_SET_TRX_STATE_confirm with < phy_TX_ON > to MAC" << endl;

        case phy_SUCCESS:
            EV << "phy_SUCCESS - [PHY]: sending a PLME_SET_TRX_STATE_confirm with < phy_SUCCESS > to MAC" << endl;
            break;

        default:
            EV << "[PHY]: sending a PLME_SET_TRX_STATE_confirm with < UNSUPPORT_ATTRIBUTE > to MAC" << endl;
            break;
    }

    send(primitive, upperLayerOut);
}

void Ieee802154ePhy::PLME_SET_confirm(PHYenum status, PHYPIBenum attribute)
{
    Ieee802154eMacPhyPrimitives *primitive = new Ieee802154eMacPhyPrimitives();
    primitive->setKind(PLME_SET_CONFIRM);
    primitive->setStatus(status);
    primitive->setPibAttribute(attribute);

    primitive->setBitRate(rs.getBitrate());
    EV << "[PHY]: sending a PLME_SET_confirm with " << status << " and attr " <<  attribute << " to MAC" << endl;
    send(primitive, upperLayerOut);
}

/**@author: 2014        Stefan Reis     (modified) */
void Ieee802154ePhy::handle_PLME_CCA_request()
{   //XXX: [SR] the different CCA Mode's are not implemented (ignored the ED threshold) - Std 802.15.4-2011 (8.2.7 Clear channel assessment (CCA)) page 153
    if (phyRadioState == phy_RX_ON)
    {
        // perform CCA, delay 8 symbols
        if (CCA_timer->isScheduled())
            error("[CCA]: received a PLME_CCA_REQUEST from MAC layer while CCA is running");

        // check if it's idle at start
        IChannelControl::TransmissionList tlAux = cc->getOngoingTransmissions(getChannelNumber());
        for (IChannelControl::TransmissionList::const_iterator it = tlAux.begin(); it != tlAux.end(); ++it)
        {
            AirFrame *airframe = check_and_cast<AirFrame *> (*it);
            // time for the message to reach us
            double distance = getRadioPosition().distance(airframe->getSenderPos());
            simtime_t propagationDelay = distance / 3.0E+8;

            // if we hear some part of the message
            if (airframe->getTimestamp() + airframe->getDuration() + propagationDelay > simTime())
            {
                isCCAStartIdle = false;
            }
            else
                isCCAStartIdle = true;
        }

        if (tlAux.size() == 0)
            isCCAStartIdle = true;

        EV <<"[CCA]: performing CCA ..., lasting 8 symbols (" << 8.0/getRate('s') << " s)" << endl;
        scheduleAt(simTime() + 8.0/getRate('s'), CCA_timer);
    }
    else
    {
        EV <<"[CCA]: received a PLME_CCA_REQUEST from MAC layer while receiver is off, reporting to MAC layer" << endl;
        PLME_CCA_confirm(phy_TRX_OFF);
    }
}

void Ieee802154ePhy::handle_PLME_ED_request()
{
    if (phyRadioState == phy_RX_ON)
    {
        rxPeakPower = rxPower[getChannelNumber()];
        ASSERT(!ED_timer->isScheduled());
        scheduleAt(simTime() + 8.0/getRate('s'), ED_timer);
    }
    else
        PLME_ED_confirm(phyRadioState, 0);
}

void Ieee802154ePhy::handle_PLME_GET_request(uint8_t pibAttribute)
{
    if (pibAttribute > 0 && pibAttribute <= PHY_CCA_DURATION)
        PLME_GET_confirm(phy_SUCCESS, (PHYPIBenum)pibAttribute , phypib);
    else
        PLME_GET_confirm(phy_INVALID_PARAMETER, PHY_INVALID_PARAMETER, phypib);
}

// XXX need to be test [SR]
void Ieee802154ePhy::handle_PLME_SET_TRX_STATE_request(PHYenum setState)
{
    bool delay;
    PHYenum tmp_state;
    PHYenum curr_state = phyRadioState;

    switch (setState)
    {
    case phy_RX_ON:
        EV << "request RX ON \n";
        break;

    case phy_TRX_OFF:
        EV << "request TX Off \n";
        break;

    case phy_FORCE_TRX_OFF:
            EV << "request FORCE TX OFF \n";
            break;

    case phy_TX_ON:
    	EV << "request TX ON \n";
    	break;

    default:
    	EV << "request UNSUPPORT_ATTRIBUTE \n";
        break;
    }

    //ignore any pending request
    if (newState != phy_IDLE)
        newState = phy_IDLE;
    else if (TRX_timer->isScheduled())
        cancelEvent(TRX_timer);

    tmp_state = curr_state;
    if (setState != curr_state) // case A: desired state is different from current state
    {
        delay = false;
        // case A1: if the PHY is transmitting
        if (((setState == phy_RX_ON)||(setState == phy_TRX_OFF)) && rs.getState() == RadioState::TRANSMIT)
        {
            tmp_state = phy_BUSY_TX;
            newState = setState;
        }
        // case A2: if the PHY is receiving
        else if (((setState == phy_TX_ON)||(setState == phy_TRX_OFF)) &&  rs.getState() == RadioState::RECV)
        {
            tmp_state = phy_BUSY_RX;
            newState = setState;
        }
        // case A3: if to force TX goes off
        else if (setState == phy_FORCE_TRX_OFF)
        {
            tmp_state = (curr_state == phy_TRX_OFF)? phy_TRX_OFF:phy_SUCCESS;
            phyRadioState = phy_TRX_OFF;                // turn off radio immediately

            PLME_SET_TRX_STATE_confirm(phyRadioState);

            setRadioState(RadioState::SLEEP);

            // a packet is being received, force it terminated
            // We do not clear the Rx buffer here and will let the rx end timer decide what to do
            if (snrInfo.ptr != NULL)
            {
                snrInfo.ptr->setKind(BITERROR_FORCE_TRX_OFF);   //incomplete reception -- force packet discard
                noiseLevel += snrInfo.rcvdPower;    // the rest reception becomes noise
            }

            // a packet is being transmitted, force it terminated
            if ( rs.getState() == RadioState::TRANSMIT && TxOver_timer->isScheduled())
            {
                ASSERT(txPktCopy);
                txPktCopy->getEncapsulatedPacket()->setKind(BITERROR_FORCE_TRX_OFF);
                cancelEvent(TxOver_timer);
                delete txPktCopy;
                txPktCopy = NULL;

                PD_DATA_confirm(phy_TRX_OFF);
            }
            // FIXME: need to test [SR]
            // ****************************** important! *********************************
            // phy_FORCE_TRX_OFF is usually called by MAC followed with a phy_TX_ON or phy_RX_ON,
            // since phy_FORCE_TRX_OFF will always succeed, no PLME_SET_TRX_STATE_confirm is necessary
            // if PLME_SET_TRX_STATE_confirm is sent, error will occur in trx_state_req at MAC
            return;
        }
        // case A4
        else
        {
            tmp_state = phy_SUCCESS;
            if (((setState == phy_RX_ON)&&(curr_state == phy_TX_ON))
                    ||((setState == phy_TX_ON)&&(curr_state == phy_RX_ON)))
            {
                newState_turnaround = setState;
                delay = true;
            }
            else
                // three cases:
                // curr: RX_ON && IDLE, set: TRX_OFF
                // curr: TX_ON && IDLE, set: TRX_OFF    (probability >> 0)
                // curr: TRX_OFF,   set: RX_ON or TX_ON
            {
                phyRadioState = setState;

               // PLME_SET_TRX_STATE_confirm(phyRadioState); //[SR] could be wrong

                if (setState == phy_TRX_OFF)
                    setRadioState(RadioState::SLEEP);
                else
                    setRadioState(RadioState::IDLE);
            }
        }

        //we need to delay <aTurnaroundTime> symbols if Tx2Rx or Rx2Tx
        if (delay)
        {
            phyRadioState = phy_TRX_OFF;

            //PLME_SET_TRX_STATE_confirm(phyRadioState); //[SR] need to be test

            setRadioState(RadioState::SLEEP);   //should be disabled immediately (further transmission/reception will not succeed)
            scheduleAt(simTime() + aTurnaroundTime/getRate('s'), TRX_timer);
            return; // send back a confirm when turnaround finishes
        }
        else
            PLME_SET_TRX_STATE_confirm(tmp_state);
    }
    // case B: desired state already set (setState == curr_state)
    else
        PLME_SET_TRX_STATE_confirm(tmp_state);
}

void Ieee802154ePhy::handle_PLME_SET_request(PHYPIBenum pibAttribute, PHY_PIB pibAttributeValue)
{
    PHYenum status = phy_SUCCESS;

    switch (pibAttribute)
    {
    case PHY_CURRENT_CHANNEL:
        // actually checking the channel number validity has been done by MAC layer before sending this primitive
        if ((pibAttributeValue.phyCurrentChannel < 0) || (pibAttributeValue.phyCurrentChannel > 26))
            error("[PHY]: channel not supported by IEEE802.15.4");

        if (!channelSupported(pibAttributeValue.phyCurrentChannel))
        {
            status = phy_INVALID_PARAMETER;
            break;
        }

        if (pibAttributeValue.phyCurrentChannel != getChannelNumber())
        {
            changeChannel(pibAttributeValue.phyCurrentChannel);
        }
        break;

    case PHY_CHANNELS_SUPPORTED:
        status = phy_READ_ONLY;
        break;

    case PHY_TX_POWER_TOLERANCE:
        if ((pibAttributeValue.phyTXPowerTolerance != 1) || (pibAttributeValue.phyTXPowerTolerance != 3) || (pibAttributeValue.phyTXPowerTolerance != 6))
            status = phy_INVALID_PARAMETER;
        else
            phypib.phyTXPowerTolerance = pibAttributeValue.phyTXPowerTolerance;
        break;

    case PHY_TRANSMIT_POWER:
        status = phy_READ_ONLY;
        break;

    case PHY_TX_POWER:
        // FIXME: phyTransmitPower need to change in the battery [SR] rs.setPhyTransmitPower(pibAttributeValue.phyTXPower);
        phypib.phyTXPower = pibAttributeValue.phyTXPower;
        //nb->fireChangeNotification(NF_TRANSMIT_POWER_CHANGED, &rs);
        break;

    case PHY_CCA_MODE:
        if ((pibAttributeValue.phyCCAMode < 1) || (pibAttributeValue.phyCCAMode  > 6))
            status = phy_INVALID_PARAMETER;
        else
        {
            phypib.phyCCAMode = pibAttributeValue.phyCCAMode;
            //rs.setPhyCCAMode(pibAttributeValue.phyCCAMode);
            //nb->fireChangeNotification(NF_CCA_MODE_CHANGED, &rs);
        }
        break;

    case PHY_CURRENT_PAGE:
        if (pibAttributeValue.phyCurrentPage > 31 && (pibAttributeValue.phyCurrentPage != 0 && 2.5 == carrierFrequency))
            status = phy_INVALID_PARAMETER;
        else
            phypib.phyCurrentPage = pibAttributeValue.phyCurrentPage;
        break;

    case PHY_MAX_FRAME_DURATION:
        status = phy_READ_ONLY;
        break;

    case PHY_SHR_DURATION:
        status = phy_READ_ONLY;
        break;

    case PHY_SYMBOLS_PER_OCTET:
        status = phy_READ_ONLY;
        break;

    case PHY_PREAMBLE_SYMBOL_LENGTH:
        status = phy_READ_ONLY;
        break;

    case PHY_UWB_DATA_RATES_SUPPORTED:
        status = phy_READ_ONLY;
        break;

    case PHY_CSS_LOW_DATA_RATE_SUPPORTED:
        status = phy_READ_ONLY;
        break;

    case PHY_UWB_COU_SUPPORTED:
        status = phy_READ_ONLY;
        break;

    case PHY_UWB_CS_SUPPORTED:
        status = phy_READ_ONLY;
        break;

    case PHY_UWB_LCP_SUPPORTED:
        status = phy_READ_ONLY;
        break;

    case PHY_UWB_CURRENT_PULSE_SHAPE:   //0 == MANDATORY, 1 == COU, 2 == CS, 3 == LCP
        if (pibAttributeValue.phyUWBCurrentPulseShape > 3)
            status = phy_INVALID_PARAMETER;
        else
            phypib.phyUWBCurrentPulseShape = pibAttributeValue.phyUWBCurrentPulseShape;
        break;

    case PHY_UWB_COU_PULSE:  //CCh.1, CCh.2, CCh.3, CCh.4, CCh.5, CCh.6
        if (pibAttributeValue.phyUWBCoUpulse > 6 && pibAttributeValue.phyUWBCoUpulse == 0)
            status = phy_INVALID_PARAMETER;
        else
            phypib.phyUWBCoUpulse = pibAttributeValue.phyUWBCoUpulse;
        break;

    case PHY_UWB_CS_PULSE:  //No.1, No.2, No.3, No.4, No.5, No.6
             if (pibAttributeValue.phyUWBCSpulse > 6 && pibAttributeValue.phyUWBCSpulse == 0)
                 status = phy_INVALID_PARAMETER;
             else
                 phypib.phyUWBCSpulse = pibAttributeValue.phyUWBCSpulse;
        break;

    case PHY_UWB_LCP_WEIGHT_1:
        if (pibAttributeValue.phyUWBLCPWeight1 > 127 && pibAttributeValue.phyUWBLCPWeight1 < -128)
            status = phy_INVALID_PARAMETER;
        else
            phypib.phyUWBLCPWeight1 = pibAttributeValue.phyUWBLCPWeight1;
        break;

    case PHY_UWB_LCP_WEIGHT_2:
        if (pibAttributeValue.phyUWBLCPWeight2 > 127 && pibAttributeValue.phyUWBLCPWeight2 < -128)
            status = phy_INVALID_PARAMETER;
        else
            phypib.phyUWBLCPWeight2 = pibAttributeValue.phyUWBLCPWeight2;
        break;

    case PHY_UWB_LCP_WEIGHT_3:
        if (pibAttributeValue.phyUWBLCPWeight3 > 127 && pibAttributeValue.phyUWBLCPWeight3 < -128)
            status = phy_INVALID_PARAMETER;
        else
            phypib.phyUWBLCPWeight3 = pibAttributeValue.phyUWBLCPWeight3;
        break;

    case PHY_UWB_LCP_WEIGHT_4:
        if (pibAttributeValue.phyUWBLCPWeight4 > 127 && pibAttributeValue.phyUWBLCPWeight4 < -128)
            status = phy_INVALID_PARAMETER;
        else
            phypib.phyUWBLCPWeight4 = pibAttributeValue.phyUWBLCPWeight4;
        break;

    case PHY_UWB_LCP_DELAY_2: // no invalid parameter, because of the UINT_8 from the struct (0-255)
        phypib.phyUWBLCPDelay2 = pibAttributeValue.phyUWBLCPDelay2;
        break;

    case PHY_UWB_LCP_DELAY_3: // no invalid parameter, because of the UINT_8 from the struct (0-255)
        phypib.phyUWBLCPDelay3 = pibAttributeValue.phyUWBLCPDelay3;
        break;

    case PHY_UWB_LCP_DELAY_4: // no invalid parameter, because of the UINT_8 from the struct (0-255)
        phypib.phyUWBLCPDelay4 = pibAttributeValue.phyUWBLCPDelay4;
        break;

    case PHY_RANGING:
        status = phy_READ_ONLY;
        break;

    case PHY_RANGING_CRYSTAL_OFFSET:
        status = phy_READ_ONLY;
        break;

    case PHY_RANGING_DPS:
        status = phy_READ_ONLY;
        break;

    case PHY_CURRENT_CODE: // TODO: check at UWB and CSS mode here
        // This value is zero for PHYs other than UWB or CSS.
        if (pibAttributeValue.phyCurrentCode > 24)
            status = phy_INVALID_PARAMETER;
        else
            phypib.phyCurrentCode = pibAttributeValue.phyCurrentCode;
        break;

    case PHY_NATIVE_PRF: // TODO: check at UWB mode here
        // For UWB PHYs, the native PRF. Zero is for non-UWB PHYs;
        // one is for PRF of 4; two is for a PRF of 16; and three is for PHYs that have no preference.
        if (pibAttributeValue.phyNativePRF > 2)
            status = phy_INVALID_PARAMETER;
        else
            phypib.phyNativePRF = pibAttributeValue.phyNativePRF;
        break;

    case PHY_UWB_SCAN_BINS_PER_CHANNEL: // TODO: check at UWB mode here
        // Set to zero for non-UWB PHYs. Note: no invalid parameter, because of the UINT_8 from the struct (0-255)
        phypib.phyNativePRF = pibAttributeValue.phyNativePRF;
        break;

    case PHY_UWB_INSERTED_PREAMBLE_INTERVAL: // TODO: check at UWB mode here
        // Set to four for UWB PHY in CCA mode 6; otherwise, set to zero
        if (pibAttributeValue.phyUWBInsertedPreambleInterval != 0 || pibAttributeValue.phyUWBInsertedPreambleInterval != 4 )
            status = phy_INVALID_PARAMETER;
        else
            phypib.phyUWBInsertedPreambleInterval = pibAttributeValue.phyUWBInsertedPreambleInterval;
        break;

    case PHY_TX_RMARKER_OFFSET:
        phypib.phyTXRMARKEROffset = pibAttributeValue.phyTXRMARKEROffset;
        break;

    case PHY_RX_RMARKER_OFFSET:
        phypib.phyRXRMARKEROffset = pibAttributeValue.phyRXRMARKEROffset;
        break;

    case PHY_RFRAME_PROCESSING_TIME:
        phypib.phyRFRAMEProcessingTime = pibAttributeValue.phyRFRAMEProcessingTime;
        break;

    case PHY_CCA_DURATION:
        // This attribute shall only be used with PHYs operating in the 950 MHz band.
        if (pibAttributeValue.phyCCADuration > 1000 )
            status = phy_INVALID_PARAMETER;
        else
            phypib.phyCCADuration = pibAttributeValue.phyCCADuration;
        break;

    default:
        status = phy_UNSUPPORT_ATTRIBUTE;
        break;
    }

    PLME_SET_confirm(status, pibAttribute);
}


// XXX: do we need this? [SR]
void Ieee802154ePhy::PLME_bitRate(double bitRate)
{
    Ieee802154eMacPhyPrimitives *primitive = new Ieee802154eMacPhyPrimitives();
    primitive->setKind(PLME_GET_BITRATE);
    primitive->setBitRate(bitRate);
    primitive->setBitRate(rs.getBitrate());
    EV << "[PHY]: sending a PLME_bitRate with " << bitRate << " to MAC" << endl;
    send(primitive, upperLayerOut);
}


void Ieee802154ePhy::changeChannel(int channel)
{
    if (channel == rs.getChannelNumber())
        return;
    if (rs.getState() == RadioState::TRANSMIT)
        error("changing channel while transmitting is not allowed");

   // Clear the recvBuff
   for (RecvBuff::iterator it = recvBuff.begin(); it!=recvBuff.end(); ++it)
   {
        AirFrame *airframe = it->first;
        cMessage *endRxTimer = (cMessage *)airframe->getContextPointer();
        delete airframe;
        delete cancelEvent(endRxTimer);
    }
    recvBuff.clear();

    // clear snr info
    snrInfo.ptr = NULL;
    snrInfo.sList.clear();
    rxPower[getChannelNumber()] = 0; // clear power accumulator on current channel

    //XXX:[SR] we don't need this, because we send after the SET command a TRX_SET command!
    //if (rs.getState()!=RadioState::IDLE)
    //    rs.setState(RadioState::IDLE);// Force radio to Idle

    // do channel switch
    EV << "Changing to channel #" << channel << "\n";

    emit(channelNumberSignal, channel);
    rs.setChannelNumber(channel);
    phypib.phyCurrentChannel = channel;

    rxPower[channel] = 0; // clear power accumulator on new channel
    noiseLevel = thermalNoise; // reset noise level

    // do channel switch
    rs.setBitrate(getRate('b'));  // bitrate also changed
    emit(bitrateSignal, getRate('b'));

    cc->setRadioChannel(myRadioRef, rs.getChannelNumber());

    cModule *myHost = findHost();

    //cGate *radioGate = myHost->gate("radioIn");

    cGate* radioGate = this->gate("radioIn")->getPathStartGate();

    EV << "RadioGate :" << radioGate->getFullPath() << " " << radioGate->getFullName() << endl;

    // pick up ongoing transmissions on the new channel
    EV << "Picking up ongoing transmissions on new channel:\n";
    IChannelControl::TransmissionList tlAux = cc->getOngoingTransmissions(channel);
    for (IChannelControl::TransmissionList::const_iterator it = tlAux.begin(); it != tlAux.end(); ++it)
    {
    	AirFrame *airframe = check_and_cast<AirFrame *> (*it);
    	// time for the message to reach us
    	double distance = getRadioPosition().distance(airframe->getSenderPos());
    	simtime_t propagationDelay = distance / 3.0E+8;

    	// if this transmission is on our new channel and it would reach us in the future, then schedule it
    	if (channel == airframe->getChannelNumber())
    	{
    		EV << " - (" << airframe->getClassName() << ")" << airframe->getName() << ": ";
    	}

    	// if there is a message on the air which will reach us in the future
    	if (airframe->getTimestamp() + propagationDelay >= simTime())
    	{
    		EV << "will arrive in the future, scheduling it\n";

    		// we need to send to each radioIn[] gate of this host
    		//for (int i = 0; i < radioGate->size(); i++)
    		//    sendDirect(airframe->dup(), airframe->getTimestamp() + propagationDelay - simTime(), airframe->getDuration(), myHost, radioGate->getId() + i);

    		// JcM Fix: we need to this radio only. no need to send the packet to each radioIn
    		// since other radios might be not in the same channel
    		sendDirect(airframe->dup(), airframe->getTimestamp() + propagationDelay - simTime(), airframe->getDuration(), myHost, radioGate->getId() );
    	}
    	// if we hear some part of the message
    	else if (airframe->getTimestamp() + airframe->getDuration() + propagationDelay > simTime())
    	{
    		EV << "missed beginning of frame, processing it as noise\n";

    		AirFrame *frameDup = airframe->dup();
    		frameDup->setArrivalTime(airframe->getTimestamp() + propagationDelay);
    		handleLowerMsgStart(frameDup);
    		bufferMsg(frameDup);
    	}
    	else
    	{
    		EV << "in the past\n";
    	}
    }

    // notify other modules about the channel switch; and actually, radio state has changed too
    nb->fireChangeNotification(NF_RADIO_CHANNEL_CHANGED, &rs);
    //nb->fireChangeNotification(NF_RADIOSTATE_CHANGED, &rs); //[SR] we don't change the Radiostate here, this will be happen in the SET_TRX function
}


bool Ieee802154ePhy::channelSupported(int channel)
{
    return ((phypib.phyChannelsSupported & (1 << channel)) != 0);
}

double Ieee802154ePhy::getRate(char bitOrSymbol)
{
    double rate;

    if (getChannelNumber() == 0)
    {
        if (bitOrSymbol == 'b')
            rate = BR_868M;
        else
            rate = SR_868M;
    }
    else if (getChannelNumber() <= 10)
    {
        if (bitOrSymbol == 'b')
            rate = BR_915M;
        else
            rate = SR_915M;
    }
    else if (getChannelNumber() <= 26)
    {
        if (bitOrSymbol == 'b')
            rate = BR_2_4G;
        else
            rate = SR_2_4G;
    }
    else
        error("[PHY]: channel number ", getChannelNumber(), " is not supported");
    return (rate*1000);     // return bit/s
}

uint16_t Ieee802154ePhy::calculateEnergyLevel()
{
    int energy;
    uint16_t t_EnergyLevel;

    //refer to sec 6.7.7 for ED implementation details
    //ED is somewhat simulation/implementation specific; here's our way:

    /* Linux floating number compatibility
    energy = (int)((rxEDPeakPower/RXThresh_)*128);
    */

    double tmpf;
    tmpf = rxPeakPower/sensitivity;
    energy = (int)(tmpf * 128);

    t_EnergyLevel = (energy > 255)?255:energy;
    return t_EnergyLevel;
}

void Ieee802154ePhy::setRadioState(RadioState::State newState)
{
    if (rs.getState() != newState)
    {
        emit(radioStateSignal, newState);
        if (newState == RadioState::SLEEP)
        {
            disconnectTransceiver();
            disconnectReceiver();
        }
        else if (rs.getState() == RadioState::SLEEP)
        {
            connectTransceiver();
            connectReceiver();
            if (rs.getState() == newState)
            {
                rs.setState(newState);
                nb->fireChangeNotification(NF_RADIOSTATE_CHANGED, &rs);
                return;
            }
        }
    }
    rs.setState(newState);
    nb->fireChangeNotification(NF_RADIOSTATE_CHANGED, &rs);
}

void Ieee802154ePhy::addNewSnr()
{
    SnrListEntry listEntry;     // create a new entry
    listEntry.time = simTime();
    listEntry.snr = snrInfo.rcvdPower / noiseLevel;
    snrInfo.sList.push_back(listEntry);
}

// TODO: change the parent to AbstractRadioExtended and remove this methods
void Ieee802154ePhy::updateDisplayString() {
    // draw the interference area and sensitivity area
    // according pathloss propagation only
    // we use the channel controller method to calculate interference distance
    // it should be the methods provided by propagation models, but to
    // avoid a big modification, we reuse those methods.

    if (!ev.isGUI() || !drawCoverage) // nothing to do
        return;
    if (myRadioRef) {
    	cDisplayString& d = hostModule->getDisplayString();

    	// communication area (up to sensitivity)
    	// FIXME this overrides the ranges if more than one radio is present is a host
    	double sensitivity_limit = cc->getInterferenceRange(myRadioRef);
    	d.removeTag("r1");
    	d.insertTag("r1");
    	d.setTagArg("r1",0,(long) sensitivity_limit);
    	d.setTagArg("r1",2,"gray");
    	d.removeTag("r2");
    	d.insertTag("r2");
    	d.setTagArg("r2",0,(long) calcDistFreeSpace());
    	d.setTagArg("r2",2,"blue");
    }
    if (updateString==NULL && updateStringInterval>0)
    	updateString = new cMessage("refress timer");
    if (updateStringInterval>0)
        scheduleAt(simTime()+updateStringInterval,updateString);
}

double Ieee802154ePhy::calcDistFreeSpace()
{
    //the carrier frequency used
    double carrierFrequency = getChannelControlPar("carrierFrequency");
    //signal attenuation threshold
    //path loss coefficient
    double alpha = getChannelControlPar("alpha");

    double waveLength = (SPEED_OF_LIGHT / carrierFrequency);
    //minimum power level to be able to physically receive a signal
    double minReceivePower = sensitivity;

    double interfDistance = pow(waveLength * waveLength * phypib.phyTransmitPower /
                         (16.0 * M_PI * M_PI * minReceivePower), 1.0 / alpha);
    return interfDistance;
}

void Ieee802154ePhy::disconnectReceiver()
{
    receiverConnect = false;
    cc->disableReception(this->myRadioRef);
    // Clear the recvBuff
    for (RecvBuff::iterator it = recvBuff.begin(); it!=recvBuff.end(); ++it)
    {
         AirFrame *airframe = it->first;
         cMessage *endRxTimer = (cMessage *)airframe->getContextPointer();
         delete airframe;
         if (endRxTimer) // strange case that the change is at the same time that the endRxTimer is received, it is possible with batteries
             delete cancelEvent(endRxTimer);
     }
     recvBuff.clear();

     // clear snr info
     snrInfo.ptr = NULL;
     snrInfo.sList.clear();
     rxPower[getChannelNumber()] = 0; // clear power accumulator on current channel
}

void Ieee802154ePhy::connectReceiver()
{
   cc->enableReception(this->myRadioRef);
   receiverConnect = true;
   if (rs.getState()!=RadioState::IDLE)
        rs.setState(RadioState::IDLE);// Force radio to Idle

    rxPower[getChannelNumber()] = 0; // clear power accumulator on new channel
    noiseLevel = thermalNoise; // reset noise level

    // do channel switch
    rs.setBitrate(getRate('b'));  // bitrate also changed
    emit(bitrateSignal, getRate('b'));

    cc->setRadioChannel(myRadioRef, rs.getChannelNumber());

    cModule *myHost = findHost();

    //cGate *radioGate = myHost->gate("radioIn");

    cGate* radioGate = this->gate("radioIn")->getPathStartGate();

    EV << "RadioGate :" << radioGate->getFullPath() << " " << radioGate->getFullName() << endl;

    // pick up ongoing transmissions on the new channel
    EV << "Picking up ongoing transmissions on new channel:\n";
    IChannelControl::TransmissionList tlAux = cc->getOngoingTransmissions(getChannelNumber());
    for (IChannelControl::TransmissionList::const_iterator it = tlAux.begin(); it != tlAux.end(); ++it)
    {
        AirFrame *airframe = check_and_cast<AirFrame *> (*it);
        // time for the message to reach us
        double distance = getRadioPosition().distance(airframe->getSenderPos());
        simtime_t propagationDelay = distance / 3.0E+8;


        // if there is a message on the air which will reach us in the future
        if (airframe->getTimestamp() + propagationDelay >= simTime())
        {
            // if this transmission is on our new channel and it would reach us in the future, then schedule it
            EV << " - (" << airframe->getClassName() << ")" << airframe->getName() << ": ";
            EV << "will arrive in the future, scheduling it\n";

            // we need to send to each radioIn[] gate of this host
            //for (int i = 0; i < radioGate->size(); i++)
            //    sendDirect(airframe->dup(), airframe->getTimestamp() + propagationDelay - simTime(), airframe->getDuration(), myHost, radioGate->getId() + i);

            // JcM Fix: we need to this radio only. no need to send the packet to each radioIn
            // since other radios might be not in the same channel
            sendDirect(airframe->dup(), airframe->getTimestamp() + propagationDelay - simTime(), airframe->getDuration(), myHost, radioGate->getId() );
        }
        // if we hear some part of the message
        else if (airframe->getTimestamp() + airframe->getDuration() + propagationDelay > simTime())
        {
            EV << "missed beginning of frame, processing it as noise\n";

            AirFrame *frameDup = airframe->dup();
            frameDup->setArrivalTime(airframe->getTimestamp() + propagationDelay);
            handleLowerMsgStart(frameDup);
            bufferMsg(frameDup);
        }
    }
    // notify other modules about the channel switch; and actually, radio state has changed too
    nb->fireChangeNotification(NF_RADIOSTATE_CHANGED, &rs);
}

bool Ieee802154ePhy::handleOperationStage(LifecycleOperation *operation, int stage, IDoneCallback *doneCallback)
{
    Enter_Method_Silent();
    if (dynamic_cast<NodeStartOperation *>(operation)) {
        if (stage == NodeStartOperation::STAGE_PHYSICAL_LAYER)
            setRadioState(RadioState::IDLE);  //FIXME only if the interface is up, too; also: connectReceiver(), etc.
    }
    else if (dynamic_cast<NodeShutdownOperation *>(operation)) {
        if (stage == NodeStartOperation::STAGE_PHYSICAL_LAYER)
            setRadioState(RadioState::OFF);
    }
    else if (dynamic_cast<NodeCrashOperation *>(operation)) {
        if (stage == NodeStartOperation::STAGE_LOCAL)  // crash is immediate
            setRadioState(RadioState::OFF);
    }
    else
        throw cRuntimeError("Unsupported operation '%s'", operation->getClassName());
    return true;
}

void Ieee802154ePhy::receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj)
{
    ChannelAccess::receiveSignal(source,signalID, obj);
    if (signalID == changeLevelNoise80215)
    {
        if (BASE_NOISE_LEVEL < receptionThreshold)
        {
            if (rs.getState()==RadioState::RECV && snrInfo.ptr==NULL)
                setRadioState(RadioState::IDLE);
        }
        else
        {
            if (rs.getState()!=RadioState::IDLE)
                setRadioState(RadioState::RECV);
        }
    }
}

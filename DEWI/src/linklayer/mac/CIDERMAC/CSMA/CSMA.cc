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

#include <CSMA.h>
#include "Ieee802154eNetworkCtrlInfo_m.h"
#include "Ieee802Ctrl_m.h"
#include "Ieee802154ePhy.h"
#include "RLLFrame_m.h"
#include "CIDERFrame_m.h"
#include "Radio80211aControlInfo_m.h"

Define_Module(CSMA)
CSMA::CSMA()
{
	sendPacket = NULL;
}

CSMA::~CSMA()
{

}

void CSMA::initialize(int stage)
{
	WirelessMacBase::initialize(stage);
	if (stage == 0)
	{

		mUpperLayerIn = findGate("upperDeciderIn");
		mUpperLayerOut = findGate("upperDeciderOut");
		mLowerLayerIn = findGate("lowerDeciderIn");
		mLowerLayerOut = findGate("lowerDeciderOut");
		commonInitialize();
		if (par("GateWay").longValue() == this->getParentModule()->getParentModule()->getParentModule()->getIndex())
		{
			coordinator = true;
		}
		else
			coordinator = false;
		useMACAcks = true;
		sifs = par("sifs");
		randomBackoff = par("randomBackoff").boolValue();
		transmissionAttemptInterruptedByRx = false;
		nbTxFrames = 0;
		nbRxFrames = 0;
		nbMissedAcks = 0;
		nbRxFramesBroadcast = 0;
		nbTxAcks = 0;
		nbRecvdAcks = 0;
		nbDroppedFrames = 0;
		nbDuplicates = 0;
		nbBackoffs = 0;
		backoffValues = 0;
		stats = par("stats");
		trace = par("trace");
		macMaxCSMABackoffs = par("macMaxCSMABackoffs");

		macMaxFrameRetries = par("macMaxFrameRetries");
		macAckWaitDuration = par("macAckWaitDuration").doubleValue();
		aUnitBackoffPeriod = par("aUnitBackoffPeriod");
		ccaDetectionTime = par("ccaDetectionTime").doubleValue();
		rxSetupTime = par("rxSetupTime").doubleValue();
		aTurnaroundTime = par("aTurnaroundTime").doubleValue();
		bitrate = getRate('b'); //par("bitrate");
		ackLength = par("ackLength");
		ackMessage = NULL;

		//init parameters for backoff method
		std::string backoffMethodStr = par("backoffMethod").stdstringValue();
		if (backoffMethodStr == "exponential")
		{
			backoffMethod = EXPONENTIAL;
			macMinBE = par("macMinBE");
			macMaxBE = par("macMaxBE");
		}
		else
		{
			if (backoffMethodStr == "linear")
			{
				backoffMethod = LINEAR;
			}
			else if (backoffMethodStr == "constant")
			{
				backoffMethod = CONSTANT;
			}
			else
			{
				error("Unknown backoff method \"%s\".\
	                            Use \"constant\", \"linear\" or \"\
	                            \"exponential\".", backoffMethodStr.c_str());
			}
			initialCW = par("contentionWindow");
		}
		NB = 0;

		// txPower = par("txPower").doubleValue();

		nicId = getParentModule()->getId();

		// initialize the timers
		backoffTimer = new cMessage("timer-backoff");
		ccaTimer = new cMessage("timer-cca");
		sifsTimer = new cMessage("timer-sifs");
		rxAckTimer = new cMessage("timer-rxAck");
		macState = IDLE_1;
		txAttempts = 0;
	}
	else if (stage == 2)
	{
		if (iface && (iface->getMTU() != Ieee802154ePhy::aMaxMACFrameSize))
		{
			iface->setMtu(Ieee802154ePhy::aMaxMACFrameSize);
		}
		EV << "queueLength = " << queueLength << " bitrate = " << bitrate << " backoff method = " << par("backoffMethod").stringValue() << endl;

		EV << "finished csma init stage 1." << endl;
		center = check_and_cast<DataCenter *>(center->getModuleByPath("DataCenter"));
	}
	else if (stage == 3)
	{
		// open radio receiver, waiting for first beacon's arrival
		PLME_SET_TRX_STATE_request(phy_RX_ON);
	}
}

void CSMA::handleMessage(cMessage* msg)
{
	if (!isOperational)
	{
		handleMessageWhenDown(msg);
		return;
	}
	if (msg->getArrivalGateId() == mLowerLayerIn && dynamic_cast<cPacket*>(msg) == NULL)
	{
		if (msg->getKind() == 0)
			error("[MAC]: message '%s' with length==0 is supposed to be a primitive, but msg kind is also zero", msg->getName());
		handleLowerControl(msg);
		return;
	}
	if (msg->getArrivalGateId() == mLowerLayerIn)
	{
		mpNb->fireChangeNotification(NF_LINK_FULL_PROMISCUOUS, msg);
		handleLowerMsg(PK(msg));
	}
	else if (msg->isSelfMessage())
	{
		handleSelfMsg(msg);
	}
	else if (msg->getArrivalGateId() == mUpperLayerIn)
	{
		handleUpperMsg(PK(msg));
	}
}

void CSMA::handleSelfMsg(cMessage* msg)
{
	EV << "timer routine." << endl;
	if (msg == backoffTimer)
		executeMac(EV_TIMER_BACKOFF, msg);
	else if (msg == ccaTimer)
		executeMac(EV_TIMER_CCA, msg);
	else if (msg == sifsTimer)
		executeMac(EV_TIMER_SIFS, msg);
	else if (msg == rxAckTimer)
	{
		nbMissedAcks++;
		executeMac(EV_ACK_TIMEOUT, msg);
	}
	else
		EV << "CSMA Error: unknown timer fired:" << msg << endl;
}

void CSMA::handleUpperMsg(cPacket* msg)
{

	if (dynamic_cast<CIDERFrame *>(msg))
	{
		handleCIDERMessage(msg);
	}
}

void CSMA::handleCIDERMessage(cMessage* msg)
{
	Ieee802154eFrame *newFrame = new Ieee802154eFrame("CIDER_MAC_Frame", Ieee802154e_DATA);

	newFrame->encapsulate(PK(msg));
	Ieee802154eNetworkCtrlInfo *cntrl = new Ieee802154eNetworkCtrlInfo("CntrlInfo", Ieee802154e_DATA);

	newFrame->setControlInfo(cntrl);

	uint16_t index;
	IE3ADDR destAddr;
	UINT_16 destPanId;
	UINT_8 srcAddrMode;

	/** brief:  TRUE if a GTS is to be used for transmission, FALSE indicates that the CAP will be used.
	 *          see Std 802.15.4-2011 (6.3.1 MCPS-DATA.request) page 118 */
	bool gtsTX = false;
	destAddr = MACAddress(MACAddress::BROADCAST_ADDRESS);

	destAddr = (IE3ADDR) 0xffff; // broadcast address - Std 802.15.4-2006 (7.3.1.1) page 150
	destPanId = 0xffff; // broadcast PAN ID - Std 802.15.4-2006 (7.3.1.1) page 150
	gtsTX = false;              // send in the CAP

	EV << "[MAC]: an " << msg->getName() << " (#" << ", " << PK(msg)->getByteLength() << " Bytes, destined for MAC address " << destAddr << " received from the upper layer" << endl;

	FrameCtrl frmCtrl;
	frmCtrl.frameType = Ieee802154e_DATA;
	newFrame->setSeqNmbr(mpib.macDSN++);
	frmCtrl.securityEnabled = false;
	frmCtrl.frmPending = false;
	frmCtrl.ackReq = true;
	frmCtrl.seqNbSup = false;   // send Sequence number
	frmCtrl.dstAddrMode = defFrmCtrl_AddrMode16;
	frmCtrl.frameVersion = Ieee802154_compatible;
	if (mpib.macEBAutoSA == 0)  // NONE Address
		frmCtrl.srcAddrMode = defFrmCtrl_AddrModeNone;
	else if (mpib.macEBAutoSA == 1) // SHORT Address
		frmCtrl.srcAddrMode = defFrmCtrl_AddrMode16;
	else if (mpib.macEBAutoSA == 2) // FULL Address
		frmCtrl.srcAddrMode = defFrmCtrl_AddrMode64;

	IE3ADDR tmpDstAddr;
	IE3ADDR tmpSrcAddr;
	UINT_16 tmpDstPanId;
	UINT_16 tmpSrcPanId;

	if (frmCtrl.dstAddrMode == defFrmCtrl_AddrMode16)
	{ // 16 bit address
		tmpDstPanId = mpib.macPANId;
		tmpDstAddr = (IE3ADDR) MACAddress::BROADCAST_ADDRESS;
	}
	else
	{ // 64 bit address
		tmpDstPanId = mpib.macPANId;
		tmpDstAddr = (IE3ADDR) MACAddress::BROADCAST_ADDRESS;
	}

	if (frmCtrl.srcAddrMode == defFrmCtrl_AddrMode16)
	{ // 16 bit address
		tmpSrcPanId = mpib.macPANId;
		tmpSrcAddr = (IE3ADDR) mpib.macShortAddress;
	}
	else
	{ // 64 bit address
		tmpSrcPanId = mpib.macPANId;
		tmpSrcAddr = (IE3ADDR) mpib.macExtendedAddress;
	}

	newFrame->setDstPanId(tmpDstPanId);
	newFrame->setDstAddr(tmpDstAddr);
	newFrame->setSrcPanId(tmpSrcPanId);
	newFrame->setSrcAddr(tmpSrcAddr);
	//frmCtrl.compPanID = getPANIDComp(frmCtrl, newFrame->getSrcPanId(), newFrame->getDstPanId());

	// security is not implemented
	AuxiliarySecurityHeader auxSecHd;
	auxSecHd.secLvl = ASH_SLVL_NONE;
	auxSecHd.keyIdentMode = 0;
	auxSecHd.keySource = 0;
	auxSecHd.keyIndex = 0;

	newFrame->setFrmCtrl(frmCtrl);
	newFrame->setAuxSecHd(auxSecHd);

	executeMac(EV_SEND_REQUEST, newFrame);

}

void CSMA::handleCommand(cMessage* msg)
{
}

void CSMA::handleLowerMsg(cPacket* msg)
{
	Ieee802154eFrame *macPkt = dynamic_cast<Ieee802154eFrame *>(msg);
	MACAddress src = macPkt->getSrcAddr();
	MACAddress dest = macPkt->getDstAddr();
	//long ExpectedNr = 0;
	uint8_t ExpectedNr = 0;

	if (macPkt->getKind() != PACKETOK)
	{
		EV << "Received with errors frame name= " << macPkt->getName() << ", myState=" << macState << " src=" << macPkt->getSrcAddr() << " dst=" << macPkt->getDstAddr() << " myAddr=" << getMacAddr() << endl;
		if (macPkt->getKind() == COLLISION)
		{
			EV << "[MAC]: frame corrupted due to collision, dropped" << endl;

		}
		delete macPkt;
		return;
	}

	EV << "Received frame name= " << macPkt->getName() << ", myState=" << macState << " src=" << macPkt->getSrcAddr() << " dst=" << macPkt->getDstAddr() << " myAddr=" << getMacAddr() << endl;

	if (macPkt->getDstAddr() == getMacAddr())
	{
		if (!useMACAcks)
		{
			EV << "Received a data packet addressed to me." << endl;
			nbRxFrames++;
			executeMac(EV_FRAME_RECEIVED, macPkt);
		}
		else
		{
			//long SeqNr = macPkt->getSequenceId();
			//long SeqNr = macPkt->getBdsn();
			uint8_t SeqNr = macPkt->getBdsn();

			if (strcmp(macPkt->getName(), "CSMA-Ack") != 0)
			{
				// This is a data message addressed to us
				// and we should send an ack.
				// we build the ack packet here because we need to
				// copy data from macPkt (src).
				EV << "Received a data packet addressed to me," << " preparing an ack..." << endl;

				nbRxFrames++;

				if (ackMessage != NULL)
					delete ackMessage;
				ackMessage = new Ieee802154eFrame("CSMA-Ack");
				ackMessage->setSrcAddr(getMacAddr());
				ackMessage->setDstAddr(macPkt->getSrcAddr());
				ackMessage->setBitLength(ackLength);
				//Check for duplicates by checking expected seqNr of sender
				if (SeqNrChild.find(src) == SeqNrChild.end())
				{
					//no record of current child -> add expected next number to map
					SeqNrChild[src] = SeqNr + 1;
					EV << "Adding a new child to the map of Sequence numbers:" << src << endl;
					executeMac(EV_FRAME_RECEIVED, macPkt);
				}
				else
				{
					ExpectedNr = SeqNrChild[src];
					EV << "Expected Sequence number is " << ExpectedNr << " and number of packet is " << SeqNr << endl;
					int8_t sub = ((int8_t) SeqNr) - ((int8_t) ExpectedNr);
					//if (SeqNr < ExpectedNr)
					if (sub < 0)
					{
						//Duplicate Packet, count and do not send to upper layer
						nbDuplicates++;
						executeMac(EV_DUPLICATE_RECEIVED, macPkt);
					}
					else
					{
						SeqNrChild[src] = SeqNr + 1;
						executeMac(EV_FRAME_RECEIVED, macPkt);
					}
				}

			}
			else if (!queueModule->isEmpty())
			{

				// message is an ack, and it is for us.
				// Is it from the right node ?
				Ieee802154eFrame * firstPacket = static_cast<Ieee802154eFrame *>(queueModule->requestSpcPacket());
				if (macPkt->getSrcAddr() == firstPacket->getDstAddr())
				{
					nbRecvdAcks++;
					executeMac(EV_ACK_RECEIVED, macPkt);
				}
				else
				{
					EV << "Error! Received an ack from an unexpected source: src=" << macPkt->getSrcAddr() << ", I was expecting from node addr=" << firstPacket->getDstAddr() << endl;
					delete macPkt;
				}
			}
			else
			{
				EV << "Error! Received an Ack while my send queue was empty. src=" << macPkt->getSrcAddr() << "." << endl;
				delete macPkt;
			}
		}
	}
	else if (dest.isBroadcast())
	{
		nbRxFramesBroadcast++;
		executeMac(EV_BROADCAST_RECEIVED, macPkt);
	}
	else
	{
		EV << "packet not for me, deleting...\n";
		delete macPkt;
	}
}

InterfaceEntry* CSMA::createInterfaceEntry()
{
	InterfaceEntry *e = new InterfaceEntry(this);
	// interface name: NetworkInterface module's name without special characters ([])
	char *interfaceName = new char[strlen(getParentModule()->getFullName()) + 1];
	char *d = interfaceName;
	for (const char *s = getParentModule()->getFullName(); *s; s++)
		if (isalnum(*s))
			*d++ = *s;
	*d = '\0';

	e->setName(interfaceName);
	delete[] interfaceName;

	// address
	e->setMACAddress(macaddress);
	e->setInterfaceToken(macaddress.formInterfaceIdentifier());

	e->setMtu(Ieee802154ePhy::aMaxMACFrameSize);

	// capabilities
	e->setBroadcast(true);
	e->setMulticast(true);
	e->setPointToPoint(false);
	//e->
	iface = e;
	// add
	return e;
}

void CSMA::flushQueue()
{
	queueModule->clearQueue();
}

void CSMA::fsmError(t_mac_event event, cMessage* msg)
{
	EV << "FSM Error ! In state " << macState << ", received unknown event:" << event << "." << endl;
	if (msg != NULL)
		delete msg;
}

void CSMA::executeMac(t_mac_event event, cMessage* msg)
{
	EV << "In executeMac" << endl;
	if (macState != IDLE_1 && event == EV_SEND_REQUEST)
	{
		updateStatusNotIdle(msg);
	}
	else
	{
		switch (macState)
		{
		case IDLE_1:
			updateStatusIdle(event, msg);
			break;
		case BACKOFF_2:
			updateStatusBackoff(event, msg);
			break;
		case CCA_3:
			updateStatusCCA(event, msg);
			break;
		case TRANSMITFRAME_4:
			updateStatusTransmitFrame(event, msg);
			break;
		case WAITACK_5:
			updateStatusWaitAck(event, msg);
			break;
		case WAITSIFS_6:
			updateStatusSIFS(event, msg);
			break;
		case TRANSMITACK_7:
			updateStatusTransmitAck(event, msg);
			break;
		default:
			EV << "Error in CSMA FSM: an unknown state has been reached. macState=" << macState << endl;
		}
	}
}

void CSMA::updateStatusIdle(t_mac_event event, cMessage* msg)
{
	switch (event)
	{
	case EV_SEND_REQUEST:
		if (queueModule->getQueueSize() <= (int) queueModule->getCapacity())
		{
			queueModule->insertInQueue(msg);
			EV << "(1) FSM State IDLE_1, EV_SEND_REQUEST and [TxBuff avail]: startTimerBackOff -> BACKOFF." << endl;
			updateMacState(BACKOFF_2);
			NB = 0;
			//BE = macMinBE;
			startTimer(TIMER_BACKOFF);
		}
		else
		{
			// queue is full, message has to be deleted
			EV << "(12) FSM State IDLE_1, EV_SEND_REQUEST and [TxBuff not avail]: dropping packet -> IDLE." << endl;
			//msg->setName("MAC ERROR");
			//msg->setKind(PACKET_DROPPED);
			//sendControlUp(msg);
			delete msg;
			//droppedPacket.setReason(DroppedPacket::QUEUE);
			//utility->publishBBItem(catDroppedPacket, &droppedPacket, nicId);
			updateMacState(IDLE_1);
		}
		break;
	case EV_DUPLICATE_RECEIVED:
		EV << "(15) FSM State IDLE_1, EV_DUPLICATE_RECEIVED: setting up radio tx -> WAITSIFS." << endl;
		//sendUp(decapsMsg(static_cast<MacSeqPkt *>(msg)));
		delete msg;

		if (useMACAcks)
		{
			PLME_SET_TRX_STATE_request(phy_TX_ON);
			//phy->setRadioState(Radio::TX);
			updateMacState(WAITSIFS_6);
			startTimer(TIMER_SIFS);
		}
		break;

	case EV_FRAME_RECEIVED:
		EV << "(15) FSM State IDLE_1, EV_FRAME_RECEIVED: setting up radio tx -> WAITSIFS." << endl;
		sendUp(decapsMsg(static_cast<Ieee802154eFrame *>(msg)));
		delete msg;

		if (useMACAcks)
		{
			PLME_SET_TRX_STATE_request(phy_TX_ON);
			//phy->setRadioState(Radio::TX);
			updateMacState(WAITSIFS_6);
			startTimer(TIMER_SIFS);
		}
		break;

	case EV_BROADCAST_RECEIVED:
	{
		EV << "(23) FSM State IDLE_1, EV_BROADCAST_RECEIVED: Nothing to do." << endl;
		Radio80211aControlInfo * cinfo;
		if (msg->getControlInfo())
			cinfo = check_and_cast<Radio80211aControlInfo *>(msg->removeControlInfo());

		cPacket *msg1 = decapsMsg(static_cast<Ieee802154eFrame *>(msg));
		if (dynamic_cast<CIDERFrame *>(msg1))
		{
			CIDERFrame *temp = check_and_cast<CIDERFrame*>(msg1);
			temp->setTxPower(1);
			temp->setRxPower(cinfo->getRecPow());
			sendUp(temp);
		}
		else
		{
			sendUp(msg1);
		}
		delete msg;
	}
		break;
	default:
		fsmError(event, msg);
	}
}

void CSMA::updateStatusBackoff(t_mac_event event, cMessage* msg)
{
	switch (event)
	{
	case EV_TIMER_BACKOFF:
		EV << "(2) FSM State BACKOFF, EV_TIMER_BACKOFF:" << " starting CCA timer." << endl;
		startTimer(TIMER_CCA);
		updateMacState(CCA_3);
		PLME_SET_TRX_STATE_request(phy_RX_ON);
		//phy->setRadioState(Radio::RX);
		break;
	case EV_DUPLICATE_RECEIVED:
		// suspend current transmission attempt,
		// transmit ack,
		// and resume transmission when entering manageQueue()
		EV << "(28) FSM State BACKOFF, EV_DUPLICATE_RECEIVED:";
		if (useMACAcks)
		{
			EV << "suspending current transmit tentative and transmitting ack";
			transmissionAttemptInterruptedByRx = true;
			cancelEvent(backoffTimer);
			PLME_SET_TRX_STATE_request(phy_TX_ON);
			//phy->setRadioState(Radio::TX);
			updateMacState(WAITSIFS_6);
			startTimer(TIMER_SIFS);
		}
		else
		{
			EV << "Nothing to do.";
		}
		//sendUp(decapsMsg(static_cast<MacSeqPkt *>(msg)));
		delete msg;

		break;
	case EV_FRAME_RECEIVED:
		// suspend current transmission attempt,
		// transmit ack,
		// and resume transmission when entering manageQueue()
		EV << "(28) FSM State BACKOFF, EV_FRAME_RECEIVED:";
		if (useMACAcks)
		{
			EV << "suspending current transmit tentative and transmitting ack";
			transmissionAttemptInterruptedByRx = true;
			cancelEvent(backoffTimer);
			PLME_SET_TRX_STATE_request(phy_TX_ON);
			//phy->setRadioState(Radio::TX);
			updateMacState(WAITSIFS_6);
			startTimer(TIMER_SIFS);
		}
		else
		{
			EV << "sending frame up and resuming normal operation.";
		}
		sendUp(decapsMsg(static_cast<Ieee802154eFrame *>(msg)));
		delete msg;
		break;
	case EV_BROADCAST_RECEIVED:
		EV << "(29) FSM State BACKOFF, EV_BROADCAST_RECEIVED:" << "sending frame up and resuming normal operation." << endl;
		sendUp(decapsMsg(static_cast<Ieee802154eFrame *>(msg)));
		delete msg;
		break;
	default:
		fsmError(event, msg);
	}
}

void CSMA::updateStatusCCA(t_mac_event event, cMessage* msg)
{
	switch (event)
	{
	case EV_TIMER_CCA:
	{
		EV << "(25) FSM State CCA_3, EV_TIMER_CCA" << endl;
		bool isIdle = radioState == RadioState::IDLE;
		if (isIdle)
		{
			EV << "(3) FSM State CCA_3, EV_TIMER_CCA, [Channel Idle]: -> TRANSMITFRAME_4." << endl;
			updateMacState(TRANSMITFRAME_4);
			PLME_SET_TRX_STATE_request(phy_TX_ON);
			//phy->setRadioState(Radio::TX);
			Ieee802154eFrame * mac = check_and_cast<Ieee802154eFrame *>(queueModule->requestSpcPacket());
			//sendDown(mac);
			// give time for the radio to be in Tx state before transmitting
			//sendDelayed(mac, aTurnaroundTime, mLowerLayerOut);
			sendNewPacketInTx(mac);
			nbTxFrames++;
		}
		else
		{
			// Channel was busy, increment 802.15.4 backoff timers as specified.
			EV << "(7) FSM State CCA_3, EV_TIMER_CCA, [Channel Busy]: " << " increment counters." << endl;
			NB = NB + 1;
			//BE = std::min(BE+1, macMaxBE);

			// decide if we go for another backoff or if we drop the frame.
			if (NB > macMaxCSMABackoffs)
			{
				// drop the frame
				EV << "Tried " << NB << " backoffs, all reported a busy " << "channel. Dropping the packet." << endl;
				queueModule->deleteMsgFromQueu(queueModule->requestSpcPacket());
				;
				txAttempts = 0;
				nbDroppedFrames++;
				//mac->setName("MAC ERROR");
				//mac->setKind(PACKET_DROPPED);
				//sendControlUp(mac);
				manageQueue();
			}
			else
			{
				// redo backoff
				updateMacState(BACKOFF_2);
				startTimer(TIMER_BACKOFF);
			}
		}
		break;
	}
	case EV_DUPLICATE_RECEIVED:
		EV << "(26) FSM State CCA_3, EV_DUPLICATE_RECEIVED:";
		if (useMACAcks)
		{
			EV << " setting up radio tx -> WAITSIFS." << endl;
			// suspend current transmission attempt,
			// transmit ack,
			// and resume transmission when entering manageQueue()
			transmissionAttemptInterruptedByRx = true;
			cancelEvent(ccaTimer);
			PLME_SET_TRX_STATE_request(phy_TX_ON);
			//phy->setRadioState(Radio::TX);
			updateMacState(WAITSIFS_6);
			startTimer(TIMER_SIFS);
		}
		else
		{
			EV << " Nothing to do." << endl;
		}
		//sendUp(decapsMsg(static_cast<MacPkt *>(msg)));
		delete msg;
		break;

	case EV_FRAME_RECEIVED:
		EV << "(26) FSM State CCA_3, EV_FRAME_RECEIVED:";
		if (useMACAcks)
		{
			EV << " setting up radio tx -> WAITSIFS." << endl;
			// suspend current transmission attempt,
			// transmit ack,
			// and resume transmission when entering manageQueue()
			transmissionAttemptInterruptedByRx = true;
			cancelEvent(ccaTimer);
			PLME_SET_TRX_STATE_request(phy_TX_ON);
			//phy->setRadioState(Radio::TX);
			updateMacState(WAITSIFS_6);
			startTimer(TIMER_SIFS);
		}
		else
		{
			EV << " Nothing to do." << endl;
		}
		sendUp(decapsMsg(static_cast<Ieee802154eFrame *>(msg)));
		delete msg;
		break;
	case EV_BROADCAST_RECEIVED:
		EV << "(24) FSM State BACKOFF, EV_BROADCAST_RECEIVED:" << " Nothing to do." << endl;
		sendUp(decapsMsg(static_cast<Ieee802154eFrame *>(msg)));
		delete msg;
		break;
	default:
		fsmError(event, msg);
	}
}

void CSMA::updateStatusTransmitFrame(t_mac_event event, cMessage* msg)
{
	if (event == EV_FRAME_TRANSMITTED)
	{
		Ieee802154eFrame * packet = sendPacket;
		PLME_SET_TRX_STATE_request(phy_RX_ON);
		//phy->setRadioState(Radio::RX);

		bool expectAck = useMACAcks;
		if (!packet->getDstAddr().isBroadcast())
		{
			//unicast
			EV << "(4) FSM State TRANSMITFRAME_4, " << "EV_FRAME_TRANSMITTED [Unicast]: ";
		}
		else
		{
			//broadcast
			EV << "(27) FSM State TRANSMITFRAME_4, EV_FRAME_TRANSMITTED " << " [Broadcast]";
			expectAck = false;
		}

		if (expectAck)
		{
			EV << "RadioSetupRx -> WAITACK." << endl;
			updateMacState(WAITACK_5);
			startTimer(TIMER_RX_ACK);
		}
		else
		{
			EV << ": RadioSetupRx, manageQueue..." << endl;
			sendPacket = NULL;
			packet = NULL;
			manageQueue();
		}
	}
	else
	{
		fsmError(event, msg);
	}
}

void CSMA::updateStatusWaitAck(t_mac_event event, cMessage* msg)
{
	ASSERT(useMACAcks);

	//cMessage * mac;
	switch (event)
	{
	case EV_ACK_RECEIVED:
		EV << "(5) FSM State WAITACK_5, EV_ACK_RECEIVED: " << " ProcessAck, manageQueue..." << endl;
		if (rxAckTimer->isScheduled())
			cancelEvent(rxAckTimer);

		queueModule->deleteMsgFromQueu(queueModule->requestSpcPacket());
		txAttempts = 0;
		//      mac->setName("MAC SUCCESS");
		//      mac->setKind(TX_OVER);
		//      sendControlUp(mac);
		delete msg;
		manageQueue();
		break;
	case EV_ACK_TIMEOUT:
		EV << "(12) FSM State WAITACK_5, EV_ACK_TIMEOUT:" << " incrementCounter/dropPacket, manageQueue..." << endl;
		manageMissingAck(event, msg);
		break;
	case EV_BROADCAST_RECEIVED:
	case EV_FRAME_RECEIVED:
		sendUp(decapsMsg(static_cast<Ieee802154eFrame*>(msg)));
	case EV_DUPLICATE_RECEIVED:
		EV << "Error ! Received a frame during SIFS !" << endl;
		delete msg;
		break;
	default:
		fsmError(event, msg);
	}

}

void CSMA::updateStatusSIFS(t_mac_event event, cMessage* msg)
{

	ASSERT(useMACAcks);

	switch (event)
	{
	case EV_TIMER_SIFS:
		EV << "(17) FSM State WAITSIFS_6, EV_TIMER_SIFS:" << " sendAck -> TRANSMITACK." << endl;
		updateMacState(TRANSMITACK_7);
		//attachSignal(ackMessage, simTime());
		//sendDown(ackMessage);
		sendNewPacketInTx(ackMessage);
		nbTxAcks++;
		//      sendDelayed(ackMessage, aTurnaroundTime, lowerLayerOut);
		ackMessage = NULL;
		break;
	case EV_TIMER_BACKOFF:
		// Backoff timer has expired while receiving a frame. Restart it
		// and stay here.
		EV << "(16) FSM State WAITSIFS_6, EV_TIMER_BACKOFF. " << "Restart backoff timer and don't move." << endl;
		startTimer(TIMER_BACKOFF);
		break;
	case EV_BROADCAST_RECEIVED:
	case EV_FRAME_RECEIVED:
		EV << "Error ! Received a frame during SIFS !" << endl;
		sendUp(decapsMsg(static_cast<Ieee802154eFrame*>(msg)));
		delete msg;
		break;
	default:
		fsmError(event, msg);
	}
}

void CSMA::updateStatusTransmitAck(t_mac_event event, cMessage* msg)
{
	ASSERT(useMACAcks);

	if (event == EV_FRAME_TRANSMITTED)
	{
		EV << "(19) FSM State TRANSMITACK_7, EV_FRAME_TRANSMITTED:" << " ->manageQueue." << endl;
		PLME_SET_TRX_STATE_request(phy_RX_ON);
		//phy->setRadioState(Radio::RX);
		//      delete msg;
		manageQueue();
	}
	else
	{
		fsmError(event, msg);
	}
}

void CSMA::updateStatusNotIdle(cMessage* msg)
{
	EV << "(20) FSM State NOT IDLE, EV_SEND_REQUEST. Is a TxBuffer available ?" << endl;

	if (queueModule->getQueueSize() <= (int) queueLength)
	{
		queueModule->insertInQueue(msg);
		EV << "(21) FSM State NOT IDLE, EV_SEND_REQUEST" << " and [TxBuff avail]: enqueue packet and don't move." << endl;
	}
	else
	{
		// queue is full, message has to be deleted
		EV << "(22) FSM State NOT IDLE, EV_SEND_REQUEST" << " and [TxBuff not avail]: dropping packet and don't move." << endl;
		//msg->setName("MAC ERROR");
		//msg->setKind(PACKET_DROPPED);
		//sendControlUp(msg);
		delete msg;
		//droppedPacket.setReason(DroppedPacket::QUEUE);
		//utility->publishBBItem(catDroppedPacket, &droppedPacket, nicId);
	}
}

void CSMA::manageQueue()
{
	if (queueModule->getQueueSize() != 0)
	{
		EV << "(manageQueue) there are " << queueModule->getQueueSize() << " packets to send, entering backoff wait state." << endl;
		if (!backoffTimer->isScheduled())
			startTimer(TIMER_BACKOFF);
		updateMacState(BACKOFF_2);
		if (transmissionAttemptInterruptedByRx)
		{
			// resume a transmission cycle which was interrupted by
			// a frame reception during CCA check
			transmissionAttemptInterruptedByRx = false;
		}
		else
		{
			// initialize counters if we start a new transmission
			// cycle from zero
			NB = 0;
			//BE = macMinBE;
		}
	}
	else
	{
		EV << "(manageQueue) no packets to send, entering IDLE state." << endl;
		updateMacState(IDLE_1);
	}
}

void CSMA::updateMacState(t_mac_states newMacState)
{
	macState = newMacState;
}

void CSMA::clearQueue()
{
	queueModule->clearQueue();
}

void CSMA::manageMissingAck(t_mac_event event, cMessage* msg)
{
	if (txAttempts < macMaxFrameRetries + 1)
	{
		// increment counter
		txAttempts++;
		EV << "I will retransmit this packet (I already tried " << txAttempts << " times)." << endl;
	}
	else
	{
		// drop packet
		EV << "Packet was transmitted " << txAttempts << " times and I never got an Ack. I drop the packet." << endl;
		queueModule->deleteMsgFromQueu(queueModule->requestSpcPacket());
		txAttempts = 0;
		//mac->setName("MAC ERROR");
		//mac->setKind(PACKET_DROPPED);
		//sendControlUp(mac);
//        mpNb->fireChangeNotification(NF_LINK_BREAK, mac);
	}
	manageQueue();
}

void CSMA::startTimer(t_mac_timer timer)
{
	if (timer == TIMER_BACKOFF)
	{
		scheduleAt(scheduleBackoff(), backoffTimer);
	}
	else if (timer == TIMER_CCA)
	{
		simtime_t ccaTime = rxSetupTime + ccaDetectionTime;
		EV << "(startTimer) ccaTimer value=" << ccaTime << "(rxSetupTime,ccaDetectionTime:" << rxSetupTime << "," << ccaDetectionTime << ")." << endl;
		scheduleAt(simTime() + rxSetupTime + ccaDetectionTime, ccaTimer);
	}
	else if (timer == TIMER_SIFS)
	{
		ASSERT(useMACAcks);
		EV << "(startTimer) sifsTimer value=" << sifs << endl;
		scheduleAt(simTime() + sifs, sifsTimer);
	}
	else if (timer == TIMER_RX_ACK)
	{
		ASSERT(useMACAcks);
		simtime_t ackDuration = ((double) (ackLength + (def_phyHeaderLength * 8))) / bitrate;
		EV << "(startTimer) rxAckTimer value=" << macAckWaitDuration << " + " << ackDuration << endl;
		scheduleAt(simTime() + ackDuration + macAckWaitDuration, rxAckTimer);
	}
	else
	{
		EV << "Unknown timer requested to start:" << timer << endl;
	}
}

double CSMA::scheduleBackoff()
{
	double backoffTime;

	switch (backoffMethod)
	{
	case EXPONENTIAL:
	{
		int BE = std::min(macMinBE + NB, macMaxBE);
		double d = pow((double) 2, (int) BE);
		int v = (int) d - 1;
		int r = intuniform(1, v, 0);
		backoffTime = r * aUnitBackoffPeriod.dbl();

		EV << "(startTimer) backoffTimer value=" << backoffTime << " (BE=" << BE << ", 2^BE-1= " << v << "r=" << r << ")" << endl;
		break;
	}
	case LINEAR:
	{
		int slots = intuniform(1, initialCW + NB, 0);
		backoffTime = slots * aUnitBackoffPeriod.dbl();
		EV << "(startTimer) backoffTimer value=" << backoffTime << endl;
		break;
	}
	case CONSTANT:
	{
		int slots = intuniform(1, initialCW, 0);
		backoffTime = slots * aUnitBackoffPeriod.dbl();
		EV << "(startTimer) backoffTimer value=" << backoffTime << endl;
		break;
	}
	default:
		error("Unknown backoff method!");
	}

	nbBackoffs = nbBackoffs + 1;
	backoffValues = backoffValues + backoffTime;

	return backoffTime + simTime().dbl();
}

void CSMA::sendUp(cMessage* msg)
{
	mpNb->fireChangeNotification(NF_LINK_PROMISCUOUS, msg);
	send(msg, mUpperLayerOut);
}

void CSMA::sendDown(cMessage* msg)
{
	send(msg, mLowerLayerOut);
}

void CSMA::commonInitialize()
{
	macaddress = configurationMacAddress();
	if (!macaddress.getFlagEui64())
	{
		opp_error("802154 address error, address is not EUI64");
	}
	iface = NULL;
	aExtendedAddress = macaddress;
	registerInterface();

	// get a pointer to the NotificationBoard modul
	mpNb = NotificationBoardAccess().get();
	// subscribe for the information of the carrier sens
	mpNb->subscribe(this, NF_RADIOSTATE_CHANGED);
	//mpNb->subscribe(this, NF_BITRATE_CHANGED)
	mpNb->subscribe(this, NF_RADIO_CHANNEL_CHANGED);
	radioState = RadioState::IDLE;
	// obtain pointer to external que
	initializeQueueModule();
}

cPacket* CSMA::decapsMsg(Ieee802154eFrame* macPkt)
{
	cPacket * msg = macPkt->decapsulate();
	if (useIeee802Ctrl)
	{
		Ieee802Ctrl* cinfo = new Ieee802Ctrl();
		MACAddress destination = macPkt->getSrcAddr();
		if (macPkt->getSrcAddr().isBroadcast())
			destination = MACAddress::BROADCAST_ADDRESS;
		else
			destination.convert48();
		cinfo->setSrc(destination);
		msg->setControlInfo(cinfo);
	}
	else
	{
		Ieee802154eNetworkCtrlInfo * cinfo = new Ieee802154eNetworkCtrlInfo();
		cinfo->setNetwAddr(macPkt->getSrcAddr().getInt());
	}
	return msg;
}

double CSMA::getRate(char bitOrSymbol)
{
	double rate;

	if (ppib.phyCurrentChannel == 0)
	{
		if (bitOrSymbol == 'b')
			rate = BR_868M;
		else
			rate = SR_868M;
	}
	else if (ppib.phyCurrentChannel <= 10)
	{
		if (bitOrSymbol == 'b')
			rate = BR_915M;
		else
			rate = SR_915M;
	}
	else
	{
		if (bitOrSymbol == 'b')
			rate = BR_2_4G;
		else
			rate = SR_2_4G;
	}
	return (rate * 1000);
}

bool CSMA::toParent(Ieee802154eFrame* msg)
{
	if (((msg->getFrmCtrl().dstAddrMode == defFrmCtrl_AddrMode16) && (msg->getDstAddr().getInt() == mpib.macCoordShortAddress)) || ((msg->getFrmCtrl().dstAddrMode == defFrmCtrl_AddrMode64) && (msg->getDstAddr() == mpib.macCoordExtendedAddress)))
		return true;
	else
		return false;
}

int CSMA::calFrmByteLength(Ieee802154eFrame* msg)
{
	//EV << "[MAC]: calculating size of " << frame->getName() << endl;

	//Ieee802154eFrame* tmpFrame = check_and_cast<Ieee802154eFrame *>(frame);

	Ieee802154eFrameType frmType = Ieee802154eFrameType(msg->getFrmCtrl().frameType);
	int byteLength = 0; // MHR + MAC payload + MFR
	int MHRLength = calMHRByteLength(msg->getFrmCtrl().srcAddrMode, msg->getFrmCtrl().dstAddrMode);

	if (msg->getDstPanId() == 0) // PAN ID not present
		MHRLength = MHRLength - 2;

	if (msg->getSrcPanId() == 0) // PAN ID not present
		MHRLength = MHRLength - 2;

	if (msg->getFrmCtrl().securityEnabled) // security field - Std 802.15.4-2006 (table 96) page 212
	{
		if (msg->getAuxSecHd().keyIdentMode == 0x00)
		{
			MHRLength = MHRLength + 1 + 4;
		}
		else if (msg->getAuxSecHd().keyIdentMode == 0x01)
		{
			MHRLength = MHRLength + 1 + 4 + 1;
		}
		else if (msg->getAuxSecHd().keyIdentMode == 0x02)
		{
			MHRLength = MHRLength + 1 + 4 + 5;
		}
		else if (msg->getAuxSecHd().keyIdentMode == 0x03)
		{
			MHRLength = MHRLength + 1 + 4 + 9;
		}
	}

	if (frmType == Ieee802154e_BEACON)  // Std 802.15.4-2006 (figure 10) page 21
	{
		if (msg->getFrmCtrl().frameVersion == Ieee802154_compatible)
		{
			ASSERT(msg);
			// Enhanced beacon frame - Std 802.15.4e-2012 (5.2.2.1 Beacon frame format) page 64
			Ieee802154EnhancedBeaconFrame* enhBeaconFrm = check_and_cast<Ieee802154EnhancedBeaconFrame *>(msg);
			if (enhBeaconFrm->getFrmCtrl().presIElist)
			{
				MHRLength += (enhBeaconFrm->getIeHeaderArraySize() > 0) ? 1 : 0;
				for (unsigned int i = 0; i < enhBeaconFrm->getIeHeaderArraySize(); i++)
				{
					MHRLength = MHRLength + enhBeaconFrm->getIeHeader(i).ieLenght;
				}
				MHRLength += (enhBeaconFrm->getIePayloadArraySize() > 0) ? 1 : 0;
				for (unsigned int i = 0; i < enhBeaconFrm->getIePayloadArraySize(); i++)
				{
					MHRLength = MHRLength + enhBeaconFrm->getIePayload(i).ieLenght;
				}
			}
			// beacon payload
			byteLength = MHRLength + (msg->getFrmCtrl().seqNbSup ? 0 : 1) + 2; //SequencNumber + FCS (2 byte)
		}
		else
		{
			ASSERT(msg);
			Ieee802154eBeaconFrame* beaconFrm = check_and_cast<Ieee802154eBeaconFrame *>(msg);
			//byteLength = MHRLength + 2 + (2+ gtsCount * 3) + (1 + beaconFrm->getPaFields().numShortAddr * 2 + beaconFrm->getPaFields().numExtendedAddr * 8) + 2;
			// see Std 802.15.4-2006 (figure 44) page 143
			byteLength = MHRLength + 2 + (beaconFrm->getGtsField().gtsDescrCnt == 0 ? 1 : 2) + beaconFrm->getGtsField().gtsDescrCnt * 3 + 1 + beaconFrm->getPendingAdrrFld().numShortAddr * 2 + beaconFrm->getPendingAdrrFld().numExtendedAddr * 8 + 2;
		}
	}
	else if (frmType == Ieee802154e_DATA) // Std 802.15.4-2006 (figure 11) page 22 , MAC payload not included here, will be added automatically while encapsulation later on
	{ // 2 Frame Control + 1 Sequence Number + 0-20 Addressing fields + 0-14 Auxiliary Security Header + 2 FCS - Std 802.15.4-2011 (figure 46) page 65
		byteLength = MHRLength + (msg->getFrmCtrl().seqNbSup ? 0 : 1) + 2;
	}
	else if (frmType == Ieee802154e_MULTI) // Std 802.15.4e-2012 (figure 48j) page 74, MAC payload not included here, will be added automatically while encapsulation later on
	{ // 1-2 Frame Control + 0-1 Sequence Number + 0-20 Addressing fields + 0-14 Auxiliary Security Header + ? Information Elements + 2 FCS
		Ieee802154eMPFrame* tmpMPFrame = check_and_cast<Ieee802154eMPFrame *>(msg);
		//
		byteLength = MHRLength + (tmpMPFrame->getMpFrmCtrl().lngFrameCtrl ? 1 : 0) + (tmpMPFrame->getFrmCtrl().seqNbSup ? 0 : 1) + (!tmpMPFrame->getMpFrmCtrl().presPANID ? -4 : 0) + 2;
	}
	else if (frmType == Ieee802154e_ACK) // Std 802.15.4-2006 (figure 12) page 22
	{
		if (msg->getFrmCtrl().frameVersion == Ieee802154_compatible)
		{
			// Enhanced acknowledge frame - Std 802.15.4e-2012 (5.2.2.3 Acknowledgment frame format) page 66
			Ieee802154eEnhancedAckFrame* enhAckFrm = check_and_cast<Ieee802154eEnhancedAckFrame *>(msg);
			if (enhAckFrm->getFrmCtrl().presIElist)
			{
				MHRLength += (enhAckFrm->getIeHeaderArraySize() > 0) ? 1 : 0;
				for (unsigned int i = 0; i < enhAckFrm->getIeHeaderArraySize(); i++)
				{
					MHRLength = MHRLength + enhAckFrm->getIeHeader(i).ieLenght;
				}
				MHRLength += (enhAckFrm->getIePayloadArraySize() > 0) ? 1 : 0;
				for (unsigned int i = 0; i < enhAckFrm->getIePayloadArraySize(); i++)
				{
					MHRLength = MHRLength + enhAckFrm->getIePayload(i).ieLenght;
				}
			}
			byteLength = MHRLength + (msg->getFrmCtrl().seqNbSup ? 0 : 1) + 2;
		}
		else
			// Acknowledge frame - Std 802.15.4-2006/2011
			byteLength = SIZE_OF_802154_ACK;
	}
	else if (frmType == Ieee802154e_CMD) // Std 802.15.4-2006 (figure 13) page 23
	{
		ASSERT(msg);
		Ieee802154eCmdFrame* cmdFrm = check_and_cast<Ieee802154eCmdFrame *>(msg);
		switch (cmdFrm->getCmdType())
		{
		case ASSOCIATION_REQUEST: // Fig 48: MHR (17/23) + Payload (2) + FCS (2)
			byteLength = MHRLength + 4;
			break;

		case ASSOCIATION_RESPONSE:
			byteLength = SIZE_OF_802154_ASSOCIATION_RESPONSE;
			break;

		case DISASSOCIATION_NOTIFICATION:
			byteLength = SIZE_OF_802154_DISASSOCIATION_NOTIFICATION;
			break;

		case DATA_REQUEST: // Fig 52: MHR (7/11/13/17) + Payload (1) + FCS (2)
			byteLength = MHRLength + 3;
			break;

		case PAN_ID_CONFLICT_NOTIFICATION:
			byteLength = SIZE_OF_802154_PANID_CONFLICT_NOTIFICATION;
			break;

		case ORPHAN_NOTIFICATION:
			byteLength = SIZE_OF_802154_ORPHAN_NOTIFICATION;
			break;

		case BEACON_REQUEST:
			byteLength = SIZE_OF_802154_BEACON_REQUEST;
			break;

		case COORDINATOR_REALIGNMENT: // Fig 56: MHR (17/23) + Payload (8) + FCS (2)
			byteLength = MHRLength + 10;
			break;

		case GTS_REQUEST:
			byteLength = SIZE_OF_802154_GTS_REQUEST;
			break;

		default:
			error("[MAC]: cannot calculate the size of a MAC command frame with unknown type!");
			break;
		}
	}
	else
		error("[MAC]: cannot calculate the size of a MAC frame with unknown type!");

	//EV << "[MAC]: header size is " << MHRLength << " Bytes" << endl;
	//EV << "[MAC]: MAC frame size is " << byteLength << " Bytes" << endl;
	return byteLength;
}

int CSMA::calMHRByteLength(uint8_t addrModeSrc, uint8_t addrModeDst)
{
	int sum = 2; // frame control - see Std 802.15.4e-2012 (5.2.1.1) page 59
	switch (addrModeSrc)
	{
	case 0:
		sum += 0;
		break;
	case 1:
		sum += 2 + 1;
		break;
	case 2:
		sum += 2 + 2;
		break;
	case 3:
		sum += 2 + 8;
		break;
	default:
		error("[MAC]: impossible address src mode sum!");
		break;
	}
	switch (addrModeDst)
	{
	case 0:
		sum += 0;
		break;
	case 1:
		sum += 2 + 1;
		break;
	case 2:
		sum += 2 + 2;
		break;
	case 3:
		sum += 2 + 8;
		break;
	default:
		error("[MAC]: impossible address dst mode sum!");
		break;
	}

	return sum;
}

void CSMA::initializeQueueModule()
{

	// use of external queue module is optional -- find it if there's one specified
	if (par("queueModule").stringValue()[0])
	{
		cModule *module = getParentModule()->getSubmodule(par("queueModule").stringValue());
		queueModule = check_and_cast<CIDERQueue *>(module);

//        EV << "[IFQUEUE]: requesting first one frame from queue module\n";
//        queueModule->requestPacket();
	}
}

simtime_t CSMA::calDuration(Ieee802154eFrame* msg)
{
	return (def_phyHeaderLength * 8 + msg->getBitLength()) / phy_bitrate;
}

MACAddress CSMA::configurationMacAddress()
{
	MACAddress address;
	const char *addressString = par("address");
	if (!strcmp(addressString, "auto"))
	{
		// assign automatic address
		address = MACAddress::generateAutoAddress();
		address.convert64();
		// change module parameter from "auto" to concrete address
		par("address").setStringValue(address.str().c_str());
	}
	else
		address.setAddress(addressString);

	return address;
}

void CSMA::handleLowerControl(cMessage* msg)
{
	if (msg->getKind() == PD_DATA_CONFIRM)
		executeMac(EV_FRAME_TRANSMITTED, msg);
	else if (msg->getKind() == PLME_SET_TRX_STATE_CONFIRM)
	{
		Ieee802154eMacPhyPrimitives* primitive = check_and_cast<Ieee802154eMacPhyPrimitives *>(msg);
		phystatus = PHYenum(primitive->getStatus());
		if (phystatus == phy_SUCCESS && sendPacket)
		{
			queueModule->deleteMsgFromQueu(sendPacket);
			sendDown(sendPacket);
		}
	}

	/*
	 if (msg->getKind() == MacToPhyInterface::TX_OVER) {
	 executeMac(EV_FRAME_TRANSMITTED, msg);
	 } else if (msg->getKind() == BaseDecider::PACKET_DROPPED) {
	 EV<< "control message: PACKED DROPPED" << endl;
	 } else {
	 EV << "Invalid control message type (type=NOTHING) : name="
	 << msg->getName() << " modulesrc="
	 << msg->getSenderModule()->getFullPath()
	 << "." << endl;
	 }*/
	delete msg;
}

MACAddress CSMA::getLongAddress(UINT_16 val)
{
	MACAddress tmp;

	unsigned char addrbytes[MAC_ADDRESS_SIZE64];
	addrbytes[0] = 0x00;
	addrbytes[1] = 0x00;
	addrbytes[2] = 0x00;
	addrbytes[3] = 0xff;
	addrbytes[4] = 0xfe;
	addrbytes[5] = 0x00;
	addrbytes[6] = (val >> 8) & 0xff;
	addrbytes[7] = (val >> 0) & 0xff;
	tmp.setFlagEui64(true);
	tmp.setAddressBytes(addrbytes);
	return tmp;
}

void CSMA::PLME_SET_TRX_STATE_request(PHYenum state)
{
	EV << "[MAC]: sending PLME_SET_TRX_STATE_request <" << state << "> to PHY layer" << endl;
	// construct PLME_SET_TRX_STATE_request primitive
	Ieee802154eMacPhyPrimitives *primitive = new Ieee802154eMacPhyPrimitives();
	primitive->setKind(PLME_SET_TRX_STATE_REQUEST);
	primitive->setStatus(state);
	send(primitive, mLowerLayerOut);
	trx_state_req = state; // store requested radio state
}

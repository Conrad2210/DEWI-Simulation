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

#include "Ieee802154eMacRLL.h"

Define_Module(Ieee802154eMacRLL);

Ieee802154eMacRLL::Ieee802154eMacRLL()
{

	awaitingBeacon = NULL;
	scanTimer = NULL;

}

Ieee802154eMacRLL::~Ieee802154eMacRLL()
{
    delete awaitingBeacon;
    delete scanTimer;
}

void Ieee802154eMacRLL::initialize(int stage)
{
    Ieee802154eMac::initialize(stage);
    if(stage != 4)
	return;

    scanTimer = new cMessage("scanTimer", MAC_SCAN_TIMER);
    awaitingBeacon = new cMessage("awaitingBeaconTimer", MAC_AWAITING_BEACON);

}

void Ieee802154eMacRLL::handleMessage(cMessage *msg)
{

    if(!isOperational)
    {
	handleMessageWhenDown(msg);
	return;
    }

    if(msg->getArrivalGateId() == mLowerLayerIn && dynamic_cast<cPacket*>(msg) == NULL)
    {
	if(msg->getKind() == 0)
	    error("[MAC]: message '%s' with length == 0 is supposed to be a primitive, but msg kind is also zero", msg->getName());

	handleMacPhyPrimitive(msg->getKind(), msg); // from PHY layer - confirm messages
	return;
    }

    if(msg->getArrivalGateId() == mUpperLayerIn && dynamic_cast<cPacket*>(msg) == NULL)
    {
	if(msg->getKind() == 0)
	    error("[MAC]: message '%s' with length == 0 is supposed to be a primitive, but msg kind is also zero", msg->getName());

	if(!handlePrimitive(msg->getKind(), msg)) // from NET layer - command messages
	{
	    handleSchedulerMsg(msg);
	}
	//XXX: note: not in use, not for the StarNet or the CSMA802154 example, from the NET layer comes only data packets
	return;
    }
    else if(msg->getArrivalGateId() == mLowerLayerIn)
    {
	handleLowerMsg(PK(msg));    // from PHY layer - data messages
    }
    else if(msg->isSelfMessage())
    {
	handleSelfMsg(msg);         // for timer
    }
    else if(msg->getArrivalGateId() == mLowerLayerIn)
    {
	EV << "QUEUE: do something if a packet comes from the Queue" << endl;
    }
    else
    {
	handleUpperMsg(PK(msg)); // from network layer - data and command messages
    }
}

void Ieee802154eMacRLL::handleSchedulerMsg(cMessage *msg)
{
    switch(msg->getKind())
    {
	case TP_MLME_START_REQUEST:
	{
	    handle_MLME_START_request(msg->dup());
	    delete msg;
	    break;
	}
	case TP_MLME_BEACON_REQUEST:
	{
	    handle_MLME_BEACON_request(msg->dup());
	    delete msg;
	    break;
	}
	case TP_MLME_SCAN_REQUEST:
	{
	    handle_MLME_SCAN_request(msg->dup());
	    delete msg;
	    break;
	}
	case TP_MLME_SET_BEACON_REQUEST:
	{
	    handleEB(msg->dup());
	    delete msg;
	    break;
	}
	case TP_MLME_ASSOCIATE_REQUEST:
	{
	    handle_MLME_ASSOCIATE_request(msg->dup());
	    delete msg;
	    break;
	}
	case TP_MLME_DISASSOCIATE_REQUEST:
	{
	    handle_MLME_DISASSOCIATE_request(msg->dup());
	    delete msg;
	    break;
	}
	case TP_MLME_ASSOCIATE_RESPONSE:
	{
	    handle_MLME_ASSOCIATE_response(msg->dup());
	    delete msg;
	    break;
	}
	case TP_MLME_DISASSOCIATE_RESPONSE:
	{
	    handle_MLME_DISASSOCIATE_response(msg->dup());
	    delete msg;
	    break;
	}
	case TP_SCHEDULE_REQUEST:
	{
	    handle_SCHEDULE_request(msg->dup());
	    delete msg;
	    break;
	}
	case TP_SCHEDULE_RESPONSE:
	{
	    handle_SCHEDULE_response(msg->dup());
	    delete msg;
	    break;
	}
	case TP_RESTART_REQUEST:
	{
	    handle_RESTART_request(msg->dup());
	    delete msg;
	    break;
	}
	default:
	{
	    if(ev.isGUI())
	    {
		EV << "Unknown command, delete frame" << endl;
	    }
	    delete msg;
	}
    }
}

void Ieee802154eMacRLL::handleSelfMsg(cMessage *msg)
{
    switch(msg->getKind())
    {
	case START_PAN_COOR_TIMER:
	    startPANCoor();
	    delete msg;     // it's a dynamic timer
	    break;

	case MAC_BACKOFF_TIMER:
	    handleBackoffTimer();
	    break;

	case MAC_DEFER_CCA_TIMER:
	    handleDeferCCATimer();
	    break;

	case MAC_BCN_RX_TIMER:
	    handleBcnRxTimer();
	    break;

	case MAC_BCN_TX_TIMER:
	    handleBcnTxTimer();
	    break;

	case MAC_ACK_TIMEOUT_TIMER:
	    handleAckTimeoutTimer();
	    break;

	case MAC_TX_ACK_BOUND_TIMER:
	    handleTxAckBoundTimer();
	    break;

	case MAC_TX_CMD_DATA_BOUND_TIMER:
	    handleTxCmdDataBoundTimer();
	    break;

	case MAC_IFS_TIMER:
	    handleIfsTimer();
	    break;

	case MAC_TX_SD_TIMER:
	case MAC_RX_SD_TIMER:
	    handleSDTimer();
	    break;

	case MAC_FINAL_CAP_TIMER:
	    handleFinalCapTimer();
	    break;

	case MAC_GTS_TIMER:
	    handleGtsTimer();
	    break;

	case MAC_ASN_TIMER:
	    handleAsnTimer();
	    break;

	case MAC_TS_CCA_OFFSET_TIMER:
	    handleTsCCAOffset();
	    break;

	case MAC_TS_TX_OFFSET_TIMER:
	    handleTsTxOffset();
	    break;

	case MAC_TS_RX_ACK_DELAY_TIMER:
	    handleTsRxAckDelay();
	    break;

	case MAC_TS_ACK_WAIT_TIMER:
	    handleTsAckWait();
	    break;

	case MAC_TS_RX_OFFSET_TIMER:
	    handleTsRxOffset();
	    break;

	case MAC_TS_RX_WAIT_TIMER:
	    handleTsRxWait();
	    break;

	case MAC_TS_TX_ACK_DELAY_TIMER:
	    handleTsTxAckDelay();
	    break;

	case MAC_SCAN_TIMER:
	    MLME_SCAN_confirm(msg);
	    break;

	case MAC_AWAITING_BEACON:
	    handleAwaitingBeaconTimer();
	    break;

	default:
	    error("[MAC]: unknown MAC timer type!");
	    break;
    }
}

void Ieee802154eMacRLL::handleLowerMsg(cPacket *msg)
{
    //[SR] old version
    bool noAck;
    int i = 0;
    Ieee802154eFrame* frame = dynamic_cast<Ieee802154eFrame *>(msg);

    if(!frame)
    {
	EV << "[MAC]: message from physical layer (" << msg->getClassName() << ")" << msg->getName() << " is not a subclass of Ieee802154eFrame, drop it" << endl;
	delete frame;
	return;
    }

    FrameCtrl frmCtrl = frame->getFrmCtrl();
    Ieee802154eFrameType frmType = frmCtrl.frameType;
    if(msg->getControlInfo() && frmType != Ieee802154e_BEACON)
	delete msg->removeControlInfo();

    EV << "[MAC]: an " << frameTypeTxt[frmType] << " frame received from the PHY layer, performing filtering now ..." << endl;

    // perform MAC frame filtering
    if(frameFilter(frame))
    {
	EV << "The received frame is filtered, drop frame" << endl;
	delete frame;
	// reset the TSCH task if the pkt was dropped
	if(mpib.macTSCHenabled)
	{
	    if(taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 10)
	    {
		taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 11;
		EV << "[TSCH CCA]-Transmitter:[11] frame was dropped at the MAC layer" << endl;
		handleTsAckWait();
		// next step in handleTsAckWait()
	    }
	    else if(taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) == 5)
	    {
		taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) = 6;
		EV << "[TSCH CCA]-Receiver:[6] frame was dropped at the MAC layer" << endl;
		handleTsRxWait();
	    }
	}
	return;
    }

    // check if neighbor in the neighborTable

    // check timing for GTS (debug)
    //    if (frmType == Ieee802154e_DATA && frame->getIsGTS())
    //    {
    //        if (isPANCoor)
    //        {
    //            // check if I'm supposed to receive the data from this device in this GTS
    //            if (indexCurrGts == 99 || gtsList[indexCurrGts].isRecvGTS || gtsList[indexCurrGts].devShortAddr != getShortAddress(frame->getSrcAddr()))
    //                error("[GTS]: timing error, PAN coordinator is not supposed to receive this DATA pkt at this time!");
    //        }
    //        else
    //        {
    //            if (index_gtsTimer != 99 || !isRecvGTS || frame->getSrcAddr().getInt() != mpib.macCoordShortAddress)
    //                error("[GTS]: timing error, the device is not supposed to receive this DATA pkt at this time!");
    //        }
    //    }

    EV << "[MAC]: checking if the received frame requires an ACK" << endl;
    //send an acknowledgment if needed (no matter this is a duplicated packet or not)
    if((frmType == Ieee802154e_DATA) || (frmType == Ieee802154e_CMD) || (frmType == Ieee802154e_MULTI) || (frmType == Ieee802154e_ASSOCIATION_REQUEST) || (frmType == Ieee802154e_DISASSOCIATION_REQUEST) || (frmType == Ieee802154e_ASSOCIATION_RESPONSE) || (frmType == Ieee802154e_DISASSOCIATION_RESPONSE) || (frmType == Ieee802154e_SCHEDULER_REQUEST) || (frmType == Ieee802154e_SCHEDULER_RESPONSE))
    {
	if(frmCtrl.ackReq) //Acknowledgment required
	{
	    /*
	     //association request command will be ignored under following cases
	     if (frmType == Ieee802154_CMD && check_and_cast<Ieee802154CmdFrame *>(frame)->getCmdType() == Ieee802154_ASSOCIATION_REQUEST)
	     if ((!capability.FFD) //not an FFD
	     || (mpib.macShortAddress == 0xffff) //not yet joined any PAN
	     || (!macAssociationPermit)) //association not permitted
	     {
	     delete frame;
	     return;
	     }*/

	    noAck = false;
	    // MAC layer can process only one command (rx or tx) at a time
	    if(frmType == Ieee802154e_CMD)
		if((rxCmd) || (txBcnCmd))
		    noAck = true;

	    if(!noAck)
	    {
		EV << "[MAC]: yes, constructing the ACK frame" << endl;
		if(!txAck)
		    constructACK(frame);
		else
		{
		    txAck = NULL;

		}

		// for TSCH Std 802.15.4e-2012 - TSCH CCA Receiver
		if(mpib.macTSCHenabled && taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) == 5)
		{
		    taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) = 7;
		    EV << "[TSCH CCA]-Receiver:[7] the data frame arrived, set the TRX_STATE on TRX_OFF and wait the TsTxAckDelay" << endl;
		    PLME_SET_TRX_STATE_request(phy_TRX_OFF); // state change confirm will be ignored

		    if(tsTxAckDelayTimer->isScheduled())
			cancelEvent(tsTxAckDelayTimer);

		    scheduleAt(simTime() + timeslotTable->getTemplate(useTimeslotID)->getTxAckDelayDbl(), tsTxAckDelayTimer);
		    // next step in handleTsTxAckDelay()
		}
		else
		{    // old version [SR]
		     //stop CSMA-CA if it is pending (it will be restored after the transmission of ACK)
		    if(backoffStatus == 99)
		    {
			EV << "[MAC]: CSMA-CA is pending, stop it, it will resume after sending ACK" << endl;
			backoffStatus = 0;
			csmacaCancel();
		    }
		    EV << "[MAC]: prepare to send the ACK, ask PHY layer to turn on the transmitter first" << endl;
		    PLME_SET_TRX_STATE_request(phy_TX_ON);
		}
	    }
	}
	else // no ACK required
	{
	    //            if (frame->getIsGTS()) // received in GTS
	    //            {
	    //                if (isPANCoor)
	    //                {
	    //                    // the device may transmit more pkts in this GTS, turn on radio
	    //                    PLME_SET_TRX_STATE_request(phy_RX_ON);
	    //                }
	    //                else
	    //                {
	    //                    // PAN coordinator can transmit only one pkt to me in my GTS, turn off radio now
	    //                    PLME_SET_TRX_STATE_request(phy_TRX_OFF);
	    //                }
	    //            }
	    //            else
	    //            {   // if this is a broadcast or a beacon, go back to sleep
	    if(taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) == 5)
	    {
		taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) = false;
		taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) = 0;
	    }
	    resetTRX();
	    //            }
	}
    }

    // drop new received cmd pkt if mac is current processing a cmd
    if(frmType == Ieee802154e_CMD)
    {
	if((rxCmd) || (txBcnCmd))
	{
	    EV << "[MAC]: the received CMD frame is dropped, because MAC is currently processing a MAC CMD" << endl;
	    delete frame;
	    return;
	}
    }

    // drop new received data pkt if mac is current processing last received data pkt
    if(frmType == Ieee802154e_DATA)
    {
	if(rxData)
	{
	    EV << "[MAC]: the received DATA frame is dropped, because MAC is currently processing the last received DATA frame" << endl;
	    delete frame;
	    return;
	}
    }

    //check duplication -- must be performed AFTER all drop's
    if(frmType == Ieee802154e_BEACON)
	i = chkAddUpdHListLink(&hlistBLink1, &hlistBLink2, frame->getSrcAddr(), frame->getSeqNmbr());
    else if(frmType != Ieee802154e_ACK) // data or cmd
	i = chkAddUpdHListLink(&hlistDLink1, &hlistDLink2, frame->getSrcAddr(), frame->getSeqNmbr());
    else // ACK
    {
	// check ACK in <handleAck()>
    }

    if(i == 2) // duplication found in the HListLink
    {
	EV << "[MAC]: duplication detected, drop frame" << endl;
	delete frame;

	if(frmType == Ieee802154e_DATA && mpib.macMetricsEnabled)
	    mpib.macDuplicateFrameCount++;

	return;
    }

    switch(frmType)
    {
	case Ieee802154e_BEACON:
	    EV << "[MAC]: continue to process received BEACON pkt" << endl;
	    handleBeacon(frame);
	    break;

	case Ieee802154e_DATA:
	    EV << "[MAC]: continue to process received DATA pkt" << endl;
	    handleData(frame);
	    break;

	case Ieee802154e_ACK:
	    EV << "[MAC]: continue to process received ACK pkt" << endl;
	    handleAck(frame);
	    break;

	    //	case Ieee802154e_CMD:
	    //	    EV << "[MAC]: continue to process received CMD pkt" << endl;
	    //	    handleCommand80215(frame);
	    //	    break;

	case Ieee802154e_ASSOCIATION_REQUEST:
	    EV << "[MAC} continue to proxess received association request pkt" << endl;
	    MLME_ASSOCIATE_indication(frame);
	    break;
	case Ieee802154e_ASSOCIATION_RESPONSE:
	    EV << "[MAC} continue to process received association response pkt" << endl;
	    MLME_ASSOCIATE_confirm(frame->dup());
	    delete frame;
	    break;
	case Ieee802154e_DISASSOCIATION_REQUEST:
	    EV << "[MAC} continue to process received association response pkt" << endl;
	    MLME_DISASSOCIATE_indication(frame->dup());
	    delete frame;
	    break;

	case Ieee802154e_DISASSOCIATION_RESPONSE:
	    EV << "[MAC} continue to process received association response pkt" << endl;
	    MLME_DISASSOCIATE_confirm(frame->dup());
	    delete frame;
	    break;

	case Ieee802154e_SCHEDULER_REQUEST:
	    EV << "[MAC] continue to process received scheduler request pkt" << endl;
	    SCHEDULE_indication(frame->dup());
	    delete frame;
	    break;
	case Ieee802154e_SCHEDULER_RESPONSE:
	    EV << "[MAC] continue to process received scheduler response pkt" << endl;
	    SCHEDULE_confirm(frame->dup(), false);
	    delete frame;
	    break;
	default:
	    error("[MAC]: undefined MAC frame type: %d", frmType);
	    break;
    }
}

void Ieee802154eMacRLL::handle_MLME_ASSOCIATE_request(cMessage *msg)
{
    /*UINT_8 channelNumber, UINT_8 channelPage, Ieee802154eAddrMode coordAddrMode,
     UINT_16 coordPANId, IE3ADDR coordAddress, UINT_8 capabilityInformation,
     UINT_8 securityLevel, UINT_8 keyIdMode, UINT_64 keySource, UINT_8 keyIndex,
     UINT_64 lowLatencyNetworkInfo, UINT_16 channelOffset, UINT_8 hoppingSequenceID
     */

    Ieee802154eAssociationFrame *dataFrame = new Ieee802154eAssociationFrame(msg->getName(), msg->getKind());
    Ieee802154eNetworkCtrlInfo *AssReq = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    FrameCtrl frmCtrl;

    //Create Request
    AssReq->setCoordAddrMode(MAC_COORD_SHORT_ADDRESS);
    AssReq->setCoordPANId(mpib.macPANId);
    AssReq->setCoordAddress(mpib.macCoordShortAddress);
    AssReq->setChannelOffset(linkTable->getLink(0)->getChannelOffset());

    if(AssReq->getPanCoordinator())
    {
	dataFrame->setCH(AssReq->getPanCoordinator());
	mpib.macUseEnhancedBeacon = true;
	isPANCoor = true;
    }
    else
	dataFrame->setCS(AssReq->getPanCoordinator());

    dataFrame->setCntrlInfo(*AssReq->dup());
    dataFrame->setSeqNmbr(mpib.macDSN++);
    //Create Frame Control
    frmCtrl.frameType = Ieee802154e_ASSOCIATION_REQUEST;
    frmCtrl.securityEnabled = secuData;
    frmCtrl.frmPending = (txBroadCmd == NULL) ? false:true;
    frmCtrl.ackReq = true;
    frmCtrl.seqNbSup = false;   // send Sequence number
    frmCtrl.dstAddrMode = defFrmCtrl_AddrMode16;
    frmCtrl.frameVersion = Ieee802154_compatible;
    if(mpib.macEBAutoSA == 0)  // NONE Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrModeNone;
    else if(mpib.macEBAutoSA == 1) // SHORT Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrMode16;
    else if(mpib.macEBAutoSA == 2) // FULL Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrMode64;

    IE3ADDR tmpDstAddr;
    IE3ADDR tmpSrcAddr;
    UINT_16 tmpDstPanId;
    UINT_16 tmpSrcPanId;

    if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode16)
    { // 16 bit address
	tmpDstPanId = mpib.macPANId;
	tmpDstAddr = (IE3ADDR)mpib.macCoordShortAddress;
    }
    else
    { // 64 bit address
	tmpDstPanId = mpib.macPANId;
	tmpDstAddr = (IE3ADDR)mpib.macCoordExtendedAddress;
    }

    if(frmCtrl.srcAddrMode == defFrmCtrl_AddrMode16)
    { // 16 bit address
	tmpSrcPanId = mpib.macPANId;
	tmpSrcAddr = (IE3ADDR)mpib.macShortAddress;
    }
    else
    { // 64 bit address
	tmpSrcPanId = mpib.macPANId;
	tmpSrcAddr = (IE3ADDR)mpib.macExtendedAddress;
    }

    dataFrame->setDstPanId(tmpDstPanId);
    dataFrame->setDstAddr(tmpDstAddr);
    dataFrame->setSrcPanId(tmpSrcPanId);
    dataFrame->setSrcAddr(tmpSrcAddr);
    frmCtrl.compPanID = getPANIDComp(frmCtrl, dataFrame->getSrcPanId(), dataFrame->getDstPanId());

    // security is not implemented
    AuxiliarySecurityHeader auxSecHd;
    auxSecHd.secLvl = ASH_SLVL_NONE;
    auxSecHd.keyIdentMode = 0;
    auxSecHd.keySource = 0;
    auxSecHd.keyIndex = 0;

    dataFrame->setFrmCtrl(frmCtrl);
    dataFrame->setAuxSecHd(auxSecHd);

    if(!queueModule->existAssReq(tmpDstAddr))
	queueModule->insertInQueue(dataFrame->dup());

    delete dataFrame;
    delete AssReq;
}

void Ieee802154eMacRLL::MLME_ASSOCIATE_indication(cMessage *msg)
{
    Ieee802154eAssociationFrame *tmp = check_and_cast<Ieee802154eAssociationFrame*>(msg);
    Ieee802154eNetworkCtrlInfo *primitive = new Ieee802154eNetworkCtrlInfo("AssociationIndication", TP_MLME_ASSOCIATE_INDICATION);

    primitive->setDeviceAddress(tmp->getSrcAddr().getInt());

    primitive->setChannelOffset(tmp->getCntrlInfo().getChannelOffset());

    primitive->setAssocShortAddress(getShortAddress(tmp->getSrcAddr()));

    primitive->setPanCoordinator(tmp->getCH());

    EV << "[MAC]: sending a MLME-ASSOCIATE.indication to NETWORK" << endl;
    send(primitive->dup(), mUpperLayerOut);
    delete primitive;
    delete tmp;
}

void Ieee802154eMacRLL::handle_MLME_ASSOCIATE_response(cMessage *msg)
{
    Ieee802154eAssociationFrame *dataFrame = new Ieee802154eAssociationFrame();
    Ieee802154eNetworkCtrlInfo *AssReq = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    FrameCtrl frmCtrl;
    //Create Request
    dataFrame->setName("AssociationResponse");
    dataFrame->setCntrlInfo(*AssReq->dup());
    dataFrame->setSeqNmbr(mpib.macDSN++);
    if(AssReq->getPanCoordinator())
    {
	dataFrame->setCH(true);
	dataFrame->setCS(false);
    }
    else
    {
	dataFrame->setCH(false);
	dataFrame->setCS(true);
    }
    //Create Frame Control
    frmCtrl.frameType = Ieee802154e_ASSOCIATION_RESPONSE;
    frmCtrl.securityEnabled = secuData;
    frmCtrl.frmPending = (txBroadCmd == NULL) ? false:true;
    frmCtrl.ackReq = true;
    frmCtrl.seqNbSup = false;   // send Sequence number
    frmCtrl.dstAddrMode = defFrmCtrl_AddrMode16;
    frmCtrl.frameVersion = Ieee802154_compatible;

    if(mpib.macEBAutoSA == 0)  // NONE Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrModeNone;
    else if(mpib.macEBAutoSA == 1) // SHORT Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrMode16;
    else if(mpib.macEBAutoSA == 2) // FULL Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrMode64;

    IE3ADDR tmpDstAddr;
    IE3ADDR tmpSrcAddr;
    UINT_16 tmpDstPanId;
    UINT_16 tmpSrcPanId;

    if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode16)
    { // 16 bit address
	tmpDstPanId = mpib.macPANId;
	tmpDstAddr = (IE3ADDR)AssReq->getDeviceAddress();
    }
    else
    { // 64 bit address
	tmpDstPanId = mpib.macPANId;
	tmpDstAddr = (IE3ADDR)AssReq->getDeviceAddress();
    }

    if(frmCtrl.srcAddrMode == defFrmCtrl_AddrMode16)
    { // 16 bit address
	tmpSrcPanId = mpib.macPANId;
	tmpSrcAddr = (IE3ADDR)mpib.macShortAddress;
    }
    else
    { // 64 bit address
	tmpSrcPanId = mpib.macPANId;
	tmpSrcAddr = (IE3ADDR)mpib.macExtendedAddress;
    }

    dataFrame->setDstPanId(tmpDstPanId);
    dataFrame->setDstAddr(tmpDstAddr);
    dataFrame->setSrcPanId(tmpSrcPanId);
    dataFrame->setSrcAddr(tmpSrcAddr);
    frmCtrl.compPanID = getPANIDComp(frmCtrl, dataFrame->getSrcPanId(), dataFrame->getDstPanId());

    // security is not implemented
    AuxiliarySecurityHeader auxSecHd;
    auxSecHd.secLvl = ASH_SLVL_NONE;
    auxSecHd.keyIdentMode = 0;
    auxSecHd.keySource = 0;
    auxSecHd.keyIndex = 0;

    dataFrame->setFrmCtrl(frmCtrl);
    dataFrame->setAuxSecHd(auxSecHd);

    if(!queueModule->existAssRes(tmpDstAddr))
	queueModule->insertInQueue(dataFrame->dup());

    delete AssReq;
    delete dataFrame;
}

void Ieee802154eMacRLL::MLME_ASSOCIATE_confirm(cMessage *msg)
{
    Ieee802154eAssociationFrame *tmp = check_and_cast<Ieee802154eAssociationFrame *>(msg);
    Ieee802154eNetworkCtrlInfo *primitive = new Ieee802154eNetworkCtrlInfo("AssociationConfirm", TP_MLME_ASSOCIATE_CONFIRM);

    if(tmp->getCntrlInfo().getStatus() == mac_FastA_successful)
    {
	notAssociated = false;
	panCoorName = tmp->getSenderModule()->getFullName();
	mpib.macShortAddress = tmp->getCntrlInfo().getAssocShortAddress();
    }
    else
    {
	notAssociated = true;
    }
    primitive->setStatus(tmp->getCntrlInfo().getStatus());
    primitive->setAssocShortAddress(getShortAddress(tmp->getSrcAddr()));
    primitive->setPanCoordinator(tmp->getCH());
    primitive->setStage(tmp->getCntrlInfo().getStage());
    primitive->setPanId(tmp->getSrcPanId());
    EV << "[MAC]: sending a MLME-ASSOCIATE.confirm to NETWORK" << endl;
    send(primitive->dup(), mUpperLayerOut);

    delete primitive;
    delete tmp;
}

void Ieee802154eMacRLL::handle_MLME_DISASSOCIATE_request(cMessage *msg)
{
    Ieee802154eDisassociationFrame *dataFrame = new Ieee802154eDisassociationFrame(msg->getName(), TP_MLME_DISASSOCIATE_REQUEST);
    Ieee802154eNetworkCtrlInfo *AssReq = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    FrameCtrl frmCtrl;

    //Create Request
    AssReq->setCoordAddrMode(MAC_COORD_SHORT_ADDRESS);
    AssReq->setCoordPANId(mpib.macPANId);
    AssReq->setCoordAddress(mpib.macCoordShortAddress);

    dataFrame->setCntrlInfo(*AssReq->dup());
    dataFrame->setSeqNmbr(mpib.macDSN++);
    //Create Frame Control
    frmCtrl.frameType = Ieee802154e_DISASSOCIATION_REQUEST;
    frmCtrl.securityEnabled = secuData;
    frmCtrl.frmPending = (txBroadCmd == NULL) ? false:true;
    frmCtrl.ackReq = true;
    frmCtrl.seqNbSup = false;   // send Sequence number
    frmCtrl.dstAddrMode = defFrmCtrl_AddrMode16;
    frmCtrl.frameVersion = Ieee802154_compatible;
    if(mpib.macEBAutoSA == 0)  // NONE Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrModeNone;
    else if(mpib.macEBAutoSA == 1) // SHORT Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrMode16;
    else if(mpib.macEBAutoSA == 2) // FULL Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrMode64;

    IE3ADDR tmpDstAddr;
    IE3ADDR tmpSrcAddr;
    UINT_16 tmpDstPanId;
    UINT_16 tmpSrcPanId;

    if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode16)
    { // 16 bit address
	tmpDstPanId = mpib.macPANId;
	tmpDstAddr = (IE3ADDR)mpib.macCoordShortAddress;
    }
    else
    { // 64 bit address
	tmpDstPanId = mpib.macPANId;
	tmpDstAddr = (IE3ADDR)mpib.macCoordExtendedAddress;
    }

    if(frmCtrl.srcAddrMode == defFrmCtrl_AddrMode16)
    { // 16 bit address
	tmpSrcPanId = mpib.macPANId;
	tmpSrcAddr = (IE3ADDR)mpib.macShortAddress;
    }
    else
    { // 64 bit address
	tmpSrcPanId = mpib.macPANId;
	tmpSrcAddr = (IE3ADDR)mpib.macExtendedAddress;
    }

    dataFrame->setDstPanId(tmpDstPanId);
    dataFrame->setDstAddr(tmpDstAddr);
    dataFrame->setSrcPanId(tmpSrcPanId);
    dataFrame->setSrcAddr(tmpSrcAddr);
    frmCtrl.compPanID = getPANIDComp(frmCtrl, dataFrame->getSrcPanId(), dataFrame->getDstPanId());

    // security is not implemented
    AuxiliarySecurityHeader auxSecHd;
    auxSecHd.secLvl = ASH_SLVL_NONE;
    auxSecHd.keyIdentMode = 0;
    auxSecHd.keySource = 0;
    auxSecHd.keyIndex = 0;

    dataFrame->setFrmCtrl(frmCtrl);
    dataFrame->setAuxSecHd(auxSecHd);

    if(!queueModule->existAssReq(tmpDstAddr))
	queueModule->insertInQueue(dataFrame->dup());

    delete dataFrame;
    delete AssReq;
}

void Ieee802154eMacRLL::MLME_DISASSOCIATE_indication(cMessage *msg)
{
    Ieee802154eDisassociationFrame *tmpMsg = check_and_cast<Ieee802154eDisassociationFrame *>(msg);
    tmpMsg->setKind(TP_MLME_DISASSOCIATE_INDICATION);
    EV << "[MAC]: sending a MLME-DISASSOCIATE.indication to NETWORK" << endl;
    send(tmpMsg, mUpperLayerOut);
}

void Ieee802154eMacRLL::handle_MLME_DISASSOCIATE_response(cMessage *msg)
{
    Ieee802154eDisassociationFrame *tmpMsg = check_and_cast<Ieee802154eDisassociationFrame *>(msg);
    Ieee802154eDisassociationFrame *dataFrame = new Ieee802154eDisassociationFrame("DisassociationResponse", TP_MLME_DISASSOCIATE_RESPONSE);
    Ieee802154eNetworkCtrlInfo *AssReq = new Ieee802154eNetworkCtrlInfo();
    FrameCtrl frmCtrl;
    //Create Request
    dataFrame->setName("DisassociationResponse");
    AssReq->setStatus(mac_SUCCESS);
    dataFrame->setCntrlInfo(*AssReq->dup());
    dataFrame->setSeqNmbr(mpib.macDSN++);
    //Create Frame Control
    frmCtrl.frameType = Ieee802154e_DISASSOCIATION_RESPONSE;
    frmCtrl.securityEnabled = secuData;
    frmCtrl.frmPending = (txBroadCmd == NULL) ? false:true;
    frmCtrl.ackReq = true;
    frmCtrl.seqNbSup = false;   // send Sequence number
    frmCtrl.dstAddrMode = defFrmCtrl_AddrMode16;
    frmCtrl.frameVersion = Ieee802154_compatible;

    if(mpib.macEBAutoSA == 0)  // NONE Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrModeNone;
    else if(mpib.macEBAutoSA == 1) // SHORT Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrMode16;
    else if(mpib.macEBAutoSA == 2) // FULL Address
	frmCtrl.srcAddrMode = defFrmCtrl_AddrMode64;

    IE3ADDR tmpDstAddr;
    IE3ADDR tmpSrcAddr;
    UINT_16 tmpDstPanId;
    UINT_16 tmpSrcPanId;

    if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode16)
    { // 16 bit address
	tmpDstPanId = mpib.macPANId;
	tmpDstAddr = (IE3ADDR)tmpMsg->getSrcAddr();
    }
    else
    { // 64 bit address
	tmpDstPanId = mpib.macPANId;
	tmpDstAddr = (IE3ADDR)tmpMsg->getSrcAddr();
    }

    if(frmCtrl.srcAddrMode == defFrmCtrl_AddrMode16)
    { // 16 bit address
	tmpSrcPanId = mpib.macPANId;
	tmpSrcAddr = (IE3ADDR)mpib.macShortAddress;
    }
    else
    { // 64 bit address
	tmpSrcPanId = mpib.macPANId;
	tmpSrcAddr = (IE3ADDR)mpib.macExtendedAddress;
    }

    dataFrame->setDstPanId(tmpDstPanId);
    dataFrame->setDstAddr(tmpDstAddr);
    dataFrame->setSrcPanId(tmpSrcPanId);
    dataFrame->setSrcAddr(tmpSrcAddr);
    frmCtrl.compPanID = getPANIDComp(frmCtrl, dataFrame->getSrcPanId(), dataFrame->getDstPanId());

    // security is not implemented
    AuxiliarySecurityHeader auxSecHd;
    auxSecHd.secLvl = ASH_SLVL_NONE;
    auxSecHd.keyIdentMode = 0;
    auxSecHd.keySource = 0;
    auxSecHd.keyIndex = 0;

    dataFrame->setFrmCtrl(frmCtrl);
    dataFrame->setAuxSecHd(auxSecHd);

    if(!queueModule->existAssRes(tmpDstAddr))
	queueModule->insertInQueue(dataFrame->dup());

    delete AssReq;
    delete dataFrame;
    delete tmpMsg;
}

void Ieee802154eMacRLL::MLME_DISASSOCIATE_confirm(cMessage *msg)
{
    Ieee802154eDisassociationFrame * tmpMsg = check_and_cast<Ieee802154eDisassociationFrame *>(msg);
    neighborTable->deleteNeighbor(neighborTable->getNeighborBySAddr(getShortAddress(tmpMsg->getSrcAddr())));
    tmpMsg->setKind(TP_MLME_DISASSOCIATE_CONFIRM);
    EV << "[MAC]: sending a MLME-DISASSOCIATE.confirm to NETWORK" << endl;
    send(tmpMsg->dup(), mUpperLayerOut);
    delete tmpMsg;
}

void Ieee802154eMacRLL::handle_MLME_SCAN_request(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *scanReq = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    if(scanReq->getScanType() == 0x02)
    {
	PHY_PIB tmpPIB;
	tmpPIB.phyCurrentChannel = scanReq->getChannel();
	PLME_SET_request(PHY_CURRENT_CHANNEL, tmpPIB);
	PLME_SET_TRX_STATE_request(phy_RX_ON);
	isSCAN = true;
    }
    else if(scanReq->getScanType() == 0x00)
    {
	PHY_PIB tmpPIB;
	tmpPIB.phyCurrentChannel = scanReq->getChannel();
	PLME_SET_request(PHY_CURRENT_CHANNEL, tmpPIB);
	PLME_SET_TRX_STATE_request(phy_TRX_OFF);
	resetTRX();
	isSCAN = false;
    }
    delete scanReq;
}

void Ieee802154eMacRLL::MLME_SCAN_confirm(cMessage *msg)
{
    Ieee802154EnhancedBeaconFrame * frame = check_and_cast<Ieee802154EnhancedBeaconFrame *>(msg);
    frame->setName("ScanConfirm");
    frame->setKind(TP_MLME_SCAN_CONFIRM);
    send(frame, mUpperLayerOut);
}

void Ieee802154eMacRLL::handle_MLME_START_request(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *startRe = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
    if(startRe->getPanCoordinator())
    {
	mpib.macBeaconOrder = par("BO");
	mpib.macSuperframeOrder = par("SO");
	PLME_SET_TRX_STATE_request(phy_TRX_OFF);
	// transmit one enhanced beacon, that all the devices have the time and the ASN
	//constructBCN(); // construct beacon, save it in the frame queue
	// start ASN
	//mpib.macPANId;
	BE = mpib.macMinBE; // for shared links (TSCH CSMA-CA)
	if(asnTimer->isScheduled())
	    cancelEvent(asnTimer);
	scheduleAt(simTime() + (double)startRe->getStartTime(), asnTimer);
	//startDevice();
    }
    else
    {
	startDevice();
    }

    MLME_START_confirm(mac_SUCCESS);
    delete startRe;
}

void Ieee802154eMacRLL::MLME_START_confirm(MACenum status)
{
    Ieee802154eNetworkCtrlInfo *startCo = new Ieee802154eNetworkCtrlInfo("StartConfirm", TP_MLME_START_CONFIRM);
    startCo->setStatus(status);
    if(par("isPANCoor").boolValue())
    {
	startCo->setBeaconOrder(mpib.macBeaconOrder);
	startCo->setSuperframeOrder(mpib.macSuperframeOrder);
    }
    startCo->setPanCoordinator(isPANCoor);
    send(startCo->dup(), mUpperLayerOut);

    delete startCo;
}

void Ieee802154eMacRLL::handle_MLME_BEACON_request(cMessage *msg)
{
    txBeaconNow = true;

    delete msg;
}

void Ieee802154eMacRLL::MLME_BEACON_confirm(MACenum status)
{
    Ieee802154eNetworkCtrlInfo *primitive = new Ieee802154eNetworkCtrlInfo();
    primitive->setKind(TP_MLME_BEACON_CONFIRM);
    primitive->setStatus(status);
    primitive->setBeaconOrder(mpib.macBeaconOrder);
    primitive->setSuperframeOrder(mpib.macSuperframeOrder);

    EV << "[MAC]: sending a MLME_BEACON.confirm to NETWORK" << endl;
    send(primitive->dup(), mUpperLayerOut);
    delete primitive;
}

void Ieee802154eMacRLL::MLME_BEACON_NOTIFY_indication(cMessage *msg)
{
}

void Ieee802154eMacRLL::handle_MLME_SET_SLOTFRAME_request(cMessage *msg)
{
}

//SCHEDULE

void Ieee802154eMacRLL::handle_SCHEDULE_request(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *tmpNetCn;
    if(dynamic_cast<Ieee802154eMulHoCluFrame *>(msg))
    {
	Ieee802154eMulHoCluFrame *tmpSchedul = check_and_cast<Ieee802154eMulHoCluFrame *>(msg);
	tmpNetCn = new Ieee802154eNetworkCtrlInfo();
	FrameCtrl frmCtrl;
	frmCtrl.frameType = Ieee802154e_SCHEDULER_REQUEST;
	frmCtrl.securityEnabled = secuData;
	frmCtrl.frmPending = (txData == NULL) ? false:true;
	frmCtrl.ackReq = true;
	frmCtrl.seqNbSup = false;
	frmCtrl.dstAddrMode = defFrmCtrl_AddrMode64;
	frmCtrl.frameVersion = Ieee802154_compatible;
	if(mpib.macEBAutoSA == 0)  // NONE Address
	    frmCtrl.srcAddrMode = defFrmCtrl_AddrModeNone;
	else if(mpib.macEBAutoSA == 1)  // SHORT Address
	    frmCtrl.srcAddrMode = defFrmCtrl_AddrMode16;
	else if(mpib.macEBAutoSA == 2)  // FULL Address
	    frmCtrl.srcAddrMode = defFrmCtrl_AddrMode64;

	IE3ADDR tmpDstAddr;
	IE3ADDR tmpSrcAddr;
	//UINT_16 tmpDstPanId;
	UINT_16 tmpSrcPanId;

	if(frmCtrl.srcAddrMode == defFrmCtrl_AddrMode16)
	{ // 16 bit address
	    tmpSrcPanId = mpib.macPANId;
	    tmpSrcAddr = (IE3ADDR)mpib.macShortAddress;
	}
	else
	{ // 64 bit address
	    tmpSrcPanId = mpib.macPANId;
	    tmpSrcAddr = (IE3ADDR)mpib.macExtendedAddress;
	}

	tmpSchedul->setSrcPanId(tmpSrcPanId);
	tmpSchedul->setSrcAddr(tmpSrcAddr);
	frmCtrl.compPanID = getPANIDComp(frmCtrl, tmpSchedul->getSrcPanId(), tmpSchedul->getDstPanId());

	AuxiliarySecurityHeader auxSecHd;
	auxSecHd.secLvl = ASH_SLVL_NONE;
	auxSecHd.keyIdentMode = 0;
	auxSecHd.keySource = 0;
	auxSecHd.keyIndex = 0;

	tmpSchedul->setCntrlInfo(*tmpNetCn->dup());
	tmpSchedul->setFrmCtrl(frmCtrl);
	tmpSchedul->setAuxSecHd(auxSecHd);
	tmpSchedul->setSeqNmbr(mpib.macDSN++);

	if(!queueModule->existSchedReq(tmpDstAddr))
	    queueModule->insertInQueue(tmpSchedul->dup());
	delete tmpNetCn;
	delete tmpSchedul;
    }
    else
    {
	Ieee802154eScheduleFrame *tmpSchedul = new Ieee802154eScheduleFrame("SchedulerRequest", TP_SCHEDULE_REQUEST);
	tmpNetCn = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
	FrameCtrl frmCtrl;
	frmCtrl.frameType = Ieee802154e_SCHEDULER_REQUEST;
	frmCtrl.securityEnabled = secuData;
	frmCtrl.frmPending = (txData == NULL) ? false:true;
	frmCtrl.ackReq = true;
	frmCtrl.seqNbSup = false;
	frmCtrl.dstAddrMode = tmpNetCn->getDeviceAddrMode();
	frmCtrl.frameVersion = Ieee802154_compatible;
	if(mpib.macEBAutoSA == 0)  // NONE Address
	    frmCtrl.srcAddrMode = defFrmCtrl_AddrModeNone;
	else if(mpib.macEBAutoSA == 1)  // SHORT Address
	    frmCtrl.srcAddrMode = defFrmCtrl_AddrMode16;
	else if(mpib.macEBAutoSA == 2)  // FULL Address
	    frmCtrl.srcAddrMode = defFrmCtrl_AddrMode64;

	IE3ADDR tmpDstAddr;
	IE3ADDR tmpSrcAddr;
	UINT_16 tmpDstPanId;
	UINT_16 tmpSrcPanId;

	if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode16)
	{ // 16 bit address
	    tmpDstPanId = mpib.macPANId;

	    tmpDstAddr = (IE3ADDR)getShortAddress(MACAddress(tmpNetCn->getDeviceAddress()));
	}
	else if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode64)
	{ // 64 bit address
	    tmpDstPanId = mpib.macPANId;
	    tmpDstAddr = (IE3ADDR)tmpNetCn->getDeviceAddress();
	}

	if(frmCtrl.srcAddrMode == defFrmCtrl_AddrMode16)
	{ // 16 bit address
	    tmpSrcPanId = mpib.macPANId;
	    tmpSrcAddr = (IE3ADDR)mpib.macShortAddress;
	}
	else
	{ // 64 bit address
	    tmpSrcPanId = mpib.macPANId;
	    tmpSrcAddr = (IE3ADDR)mpib.macExtendedAddress;
	}

	tmpSchedul->setDstPanId(tmpDstPanId);
	tmpSchedul->setDstAddr(tmpDstAddr);
	tmpSchedul->setSrcPanId(tmpSrcPanId);
	tmpSchedul->setSrcAddr(tmpSrcAddr);
	frmCtrl.compPanID = getPANIDComp(frmCtrl, tmpSchedul->getSrcPanId(), tmpSchedul->getDstPanId());

	AuxiliarySecurityHeader auxSecHd;
	auxSecHd.secLvl = ASH_SLVL_NONE;
	auxSecHd.keyIdentMode = 0;
	auxSecHd.keySource = 0;
	auxSecHd.keyIndex = 0;

	tmpSchedul->setCntrlInfo(*tmpNetCn->dup());
	tmpSchedul->setFrmCtrl(frmCtrl);
	tmpSchedul->setAuxSecHd(auxSecHd);
	tmpSchedul->setSeqNmbr(mpib.macDSN++);

	if(!queueModule->existSchedReq(tmpDstAddr))
	    queueModule->insertInQueue(tmpSchedul->dup());
	delete tmpNetCn;
	delete tmpSchedul;
    }
}

void Ieee802154eMacRLL::SCHEDULE_indication(cMessage *msg)
{
    if(dynamic_cast<Ieee802154eMulHoCluFrame *>(msg))
    {
	//if(!queueModule->existSchedRes(msg->getSrcAddr()))
	msg->setKind(TP_SCHEDULE_INDICATION);
	send(msg->dup(), mUpperLayerOut);
	delete msg;
    }
    else
    {
	Ieee802154eScheduleFrame *scheduler = check_and_cast<Ieee802154eScheduleFrame *>(msg);
	Ieee802154eNetworkCtrlInfo *Ctrl = new Ieee802154eNetworkCtrlInfo("SchedulerRequest", TP_SCHEDULE_INDICATION);
	Ctrl->setTimeslot(scheduler->getCntrlInfo().getTimeslot());
	Ctrl->setChannelOffset(scheduler->getCntrlInfo().getChannelOffset());

	if(scheduler->getCntrlInfo().getDeviceAddrMode() == defFrmCtrl_AddrMode16)
	    Ctrl->setDeviceAddress(getShortAddress(scheduler->getSrcAddr()));
	else if(scheduler->getCntrlInfo().getDeviceAddrMode() == defFrmCtrl_AddrMode64)
	{
	    Ctrl->setDeviceAddress(scheduler->getSrcAddr().getInt());

	}

	if(!queueModule->existSchedRes(scheduler->getSrcAddr()))
	    send(Ctrl->dup(), mUpperLayerOut);

	delete Ctrl;
	delete scheduler;
    }
}

void Ieee802154eMacRLL::handle_SCHEDULE_response(cMessage *msg)
{
    Ieee802154eNetworkCtrlInfo *tmpNetCn;
    if(dynamic_cast<Ieee802154eMulHoCluFrame *>(msg))
    {
	Ieee802154eMulHoCluFrame *tmpSchedul = check_and_cast<Ieee802154eMulHoCluFrame *>(msg);
	tmpNetCn = new Ieee802154eNetworkCtrlInfo();
	FrameCtrl frmCtrl;
	frmCtrl.frameType = Ieee802154e_SCHEDULER_RESPONSE;
	frmCtrl.securityEnabled = secuData;
	frmCtrl.frmPending = (txData == NULL) ? false:true;
	frmCtrl.ackReq = true;
	frmCtrl.seqNbSup = false;
	frmCtrl.dstAddrMode = defFrmCtrl_AddrMode64;
	frmCtrl.frameVersion = Ieee802154_compatible;
	if(mpib.macEBAutoSA == 0)  // NONE Address
	    frmCtrl.srcAddrMode = defFrmCtrl_AddrModeNone;
	else if(mpib.macEBAutoSA == 1)  // SHORT Address
	    frmCtrl.srcAddrMode = defFrmCtrl_AddrMode16;
	else if(mpib.macEBAutoSA == 2)  // FULL Address
	    frmCtrl.srcAddrMode = defFrmCtrl_AddrMode64;

	IE3ADDR tmpDstAddr;
	IE3ADDR tmpSrcAddr;
	//UINT_16 tmpDstPanId;
	UINT_16 tmpSrcPanId;

	if(frmCtrl.srcAddrMode == defFrmCtrl_AddrMode16)
	{ // 16 bit address
	    tmpSrcPanId = mpib.macPANId;
	    tmpSrcAddr = (IE3ADDR)mpib.macShortAddress;
	}
	else
	{ // 64 bit address
	    tmpSrcPanId = mpib.macPANId;
	    tmpSrcAddr = (IE3ADDR)mpib.macExtendedAddress;
	}

	tmpSchedul->setSrcPanId(tmpSrcPanId);
	tmpSchedul->setSrcAddr(tmpSrcAddr);
	frmCtrl.compPanID = getPANIDComp(frmCtrl, tmpSchedul->getSrcPanId(), tmpSchedul->getDstPanId());

	AuxiliarySecurityHeader auxSecHd;
	auxSecHd.secLvl = ASH_SLVL_NONE;
	auxSecHd.keyIdentMode = 0;
	auxSecHd.keySource = 0;
	auxSecHd.keyIndex = 0;

	tmpSchedul->setCntrlInfo(*tmpNetCn->dup());
	tmpSchedul->setFrmCtrl(frmCtrl);
	tmpSchedul->setAuxSecHd(auxSecHd);
	tmpSchedul->setSeqNmbr(mpib.macDSN++);

	if(!queueModule->existSchedReq(tmpDstAddr))
	    queueModule->insertInQueue(tmpSchedul->dup());
	delete tmpNetCn;
	delete tmpSchedul;
    }
    else
    {
	Ieee802154eScheduleFrame *tmpSchedul = new Ieee802154eScheduleFrame("SchedulerResponse", TP_SCHEDULE_RESPONSE);
	tmpNetCn = check_and_cast<Ieee802154eNetworkCtrlInfo *>(msg);
	FrameCtrl frmCtrl;
	frmCtrl.frameType = Ieee802154e_SCHEDULER_REQUEST;
	frmCtrl.securityEnabled = secuData;
	frmCtrl.frmPending = (txData == NULL) ? false:true;
	frmCtrl.ackReq = true;
	frmCtrl.seqNbSup = false;
	frmCtrl.dstAddrMode = tmpNetCn->getDeviceAddrMode();
	frmCtrl.frameVersion = Ieee802154_compatible;
	if(mpib.macEBAutoSA == 0)  // NONE Address
	    frmCtrl.srcAddrMode = defFrmCtrl_AddrModeNone;
	else if(mpib.macEBAutoSA == 1)  // SHORT Address
	    frmCtrl.srcAddrMode = defFrmCtrl_AddrMode16;
	else if(mpib.macEBAutoSA == 2)  // FULL Address
	    frmCtrl.srcAddrMode = defFrmCtrl_AddrMode64;

	IE3ADDR tmpDstAddr;
	IE3ADDR tmpSrcAddr;
	UINT_16 tmpDstPanId;
	UINT_16 tmpSrcPanId;

	if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode16)
	{ // 16 bit address
	    tmpDstPanId = mpib.macPANId;

	    tmpDstAddr = (IE3ADDR)getShortAddress(MACAddress(tmpNetCn->getDeviceAddress()));
	}
	else if(frmCtrl.dstAddrMode == defFrmCtrl_AddrMode64)
	{ // 64 bit address
	    tmpDstPanId = mpib.macPANId;
	    tmpDstAddr = (IE3ADDR)tmpNetCn->getDeviceAddress();
	}

	if(frmCtrl.srcAddrMode == defFrmCtrl_AddrMode16)
	{ // 16 bit address
	    tmpSrcPanId = mpib.macPANId;
	    tmpSrcAddr = (IE3ADDR)mpib.macShortAddress;
	}
	else
	{ // 64 bit address
	    tmpSrcPanId = mpib.macPANId;
	    tmpSrcAddr = (IE3ADDR)mpib.macExtendedAddress;
	}

	tmpSchedul->setDstPanId(tmpDstPanId);
	tmpSchedul->setDstAddr(tmpDstAddr);
	tmpSchedul->setSrcPanId(tmpSrcPanId);
	tmpSchedul->setSrcAddr(tmpSrcAddr);
	frmCtrl.compPanID = getPANIDComp(frmCtrl, tmpSchedul->getSrcPanId(), tmpSchedul->getDstPanId());

	AuxiliarySecurityHeader auxSecHd;
	auxSecHd.secLvl = ASH_SLVL_NONE;
	auxSecHd.keyIdentMode = 0;
	auxSecHd.keySource = 0;
	auxSecHd.keyIndex = 0;

	tmpSchedul->setCntrlInfo(*tmpNetCn->dup());
	tmpSchedul->setFrmCtrl(frmCtrl);
	tmpSchedul->setAuxSecHd(auxSecHd);
	tmpSchedul->setSeqNmbr(mpib.macDSN++);

	if(!queueModule->existSchedReq(tmpDstAddr))
	    queueModule->insertInQueue(tmpSchedul->dup());
	delete tmpNetCn;
	delete tmpSchedul;
    }
}

void Ieee802154eMacRLL::SCHEDULE_confirm(cMessage *msg, bool ack)
{
    if(dynamic_cast<Ieee802154eMulHoCluFrame *>(msg))
    {
	msg->setKind(TP_SCHEDULE_CONFIRM);
	send(msg->dup(), mUpperLayerOut);

	delete msg;
    }
    else
    {

	Ieee802154eScheduleFrame *scheduler = check_and_cast<Ieee802154eScheduleFrame *>(msg);
	Ieee802154eNetworkCtrlInfo *Ctrl = check_and_cast<Ieee802154eNetworkCtrlInfo *>(&scheduler->getCntrlInfo());
	if(scheduler->getFrmCtrl().frameType == Ieee802154e_SCHEDULER_RESPONSE)
	{

	    Ctrl->setName("SchedulerRequest");
	    Ctrl->setKind(TP_SCHEDULE_CONFIRM);
	    Ctrl->setReceivedByACK(ack);
	    send(Ctrl->dup(), mUpperLayerOut);

	}

	if(scheduler->getFrmCtrl().frameType == Ieee802154e_SCHEDULER_REQUEST)
	{
	    Ctrl->setStatus(mac_DENIED);
	    Ctrl->setName("SchedulerRequest");
	    Ctrl->setKind(TP_SCHEDULE_CONFIRM);
	    Ctrl->setReceivedByACK(ack);
	    send(Ctrl->dup(), mUpperLayerOut);

	}

	//delete Ctrl;
	delete scheduler;
    }

}

//Multihop CLustering
void Ieee802154eMacRLL::handle_RESTART_request(cMessage *msg)
{
    if(backoffTimer->isScheduled())
	cancelEvent(backoffTimer);

    if(deferCCATimer->isScheduled())
	cancelEvent(deferCCATimer);
    if(bcnRxTimer->isScheduled())
	cancelEvent(bcnRxTimer);
    if(bcnTxTimer->isScheduled())
	cancelEvent(bcnTxTimer);
    if(ackTimeoutTimer->isScheduled())
	cancelEvent(ackTimeoutTimer);
    if(txAckBoundTimer->isScheduled())
	cancelEvent(txAckBoundTimer);
    if(txCmdDataBoundTimer->isScheduled())
	cancelEvent(txCmdDataBoundTimer);
    if(ifsTimer->isScheduled())
	cancelEvent(ifsTimer);
    if(txSDTimer->isScheduled())
	cancelEvent(txSDTimer);
    if(rxSDTimer->isScheduled())
	cancelEvent(rxSDTimer);
    if(finalCAPTimer->isScheduled())
	cancelEvent(finalCAPTimer);
    if(gtsTimer->isScheduled())
	cancelEvent(gtsTimer);

    // TSCH timer
    if(asnTimer->isScheduled())
	cancelEvent(asnTimer);
    if(tsCCAOffsetTimer->isScheduled())
	cancelEvent(tsCCAOffsetTimer);
    if(tsTxOffsetTimer->isScheduled())
	cancelEvent(tsTxOffsetTimer);
    if(tsRxAckDelayTimer->isScheduled())
	cancelEvent(tsRxAckDelayTimer);
    if(tsAckWaitTimer->isScheduled())
	cancelEvent(tsAckWaitTimer);
    if(tsRxOffsetTimer->isScheduled())
	cancelEvent(tsRxOffsetTimer);
    if(tsRxWaitTimer->isScheduled())
	cancelEvent(tsRxWaitTimer);
    if(tsTxAckDelayTimer->isScheduled())
	cancelEvent(tsTxAckDelayTimer);
    if(scanTimer->isScheduled())
	cancelEvent(scanTimer);
    if(awaitingBeacon->isScheduled())
	cancelEvent(awaitingBeacon);

    emptyHListLink(&hlistBLink1, &hlistBLink2);
    emptyHListLink(&hlistDLink1, &hlistDLink2);

    MPIBcopyStandard();
    mpib.macExtendedAddress = aExtendedAddress;
    mpib.macBSN = intrand(256);     // Beacon Sequence Number
    mpib.macDSN = intrand(256);     // Data Sequence Number
    mpib.macEBSN = intrand(256);     // Enhanced Beacon Sequence Number

    m_debug = par("debug");
    isPANCoor = par("isPANCoor");
    coordRealignment = false;
    mpib.macBeaconOrder = par("BO");
    mpib.macSuperframeOrder = par("SO");
    dataTransMode = par("dataTransMode");
    panCoorName = par("panCoorName");

    ack4Data = par("ack4Data");
    mpib.macRxOnWhenIdle = par("setRxOnWhenIdle");

    trx_state_req_activ = false;
    trx_state_req = phy_INVALID_PARAMETER;
    useIeee802Ctrl = par("useIeee802Ctrl");

    // for test [SR]
    mpib.macMaxFrameRetries = par("maxRetries");

    // set TSCHCapable
    mpib.macTSCHcapable = par("TSCHcapable").boolValue();
    // for the IEEE Std 802.15.4e-2012 TSCH mode
    useTSCH = par("useTSCH");
    timeSource = false;    // It will be only set in the TSCH mode
    useCCA = par("useCCA");     // It will be only set in the TSCH mode
    txBeaconNow = false;
    if(useTSCH)
    {
	mpib.macTSCHcapable = true;
	/* Note: It will be set true for a PAN coordinator or if the first enhanced beacon with ASN arrived.
	 * useTSCHdirect - activate the TSCh mode without a Beacon from the PAN coordinator (note: the device will be not associated with the coordinator.) */
	mpib.macTSCHenabled = isPANCoor ? true:par("useTSCHdirect");
	mpib.macTimestampSupported = true; // see Std 802.15.4e-2012 (5.1.4.2a.2 Node synchronization) page 29
	mpib.macUseEnhancedBeacon = true;  // Use Enhanced Beacon
	timeSource = par("timeSource");
	useCCA = par("useCCA");
	useTimeslotID = par("useTimeslotID"); //default timeslot template=0 - Std 802.15.4e-2012 (Table 52e�TSCH-MAC PIB attributes for macTimeslotTemplate) page 176
	useHoppingSequenceID = par("useHoppingSequenceID"); // default = 0; all channels are supported
    }
    timeError = 0x0000;
    timeCorrection = 0.0;
    startTimeslot = 0.0;

    if(par("useMetrics"))
    {
	mpib.macMetricsCapable = true;
	mpib.macMetricsEnabled = true;
    }

    if(mpib.macBeaconOrder > 15 || mpib.macSuperframeOrder > mpib.macBeaconOrder)
	error("[MAC]: wrong values for MAC parameter BO or SO");
    else if(mpib.macBeaconOrder == 15)
	error("[MAC]: non-beacon mode (BO = 15) has not been tested!");
    else if(mpib.macSuperframeOrder == mpib.macBeaconOrder)
	error("[MAC]: warning!! the case BO == SO has not been tested!");

    if(dataTransMode != 1 && dataTransMode != 2 && dataTransMode != 3) // Std IEEE 802.15.4-2011 - 4.5.2 Data transfer model; page 13
	error("[MAC]: data transfer mode = %d is not supported in the model!", dataTransMode);

    // Initialization parameters for backoff method
    std::string backoffMethodStr = par("backoffMethod").stdstringValue();

    // for TSCH - The device shall use an exponential backoff mechanism - Std 802.15.4e-2012 (5.1.1.4.3 TSCH CSMA-CA algorithm) page 14
    if(mpib.macTSCHenabled)
    {
	backoffMethod = EXPONENTIAL;
	// TSCH-specific MAC PIB attributes - Std 802.15.4e-2012 (table 52b) page 174
	mpib.macMinBE = par("macMinBE");    // default: for CSMA-CA = 3; for TSCH-CA = 1
	mpib.macMaxBE = par("macMaxBE");    // default: for CSMA-CA = 5; for TSCH-CA = 7
    }
    else if(backoffMethodStr == "exponential")
    {
	backoffMethod = EXPONENTIAL;
	mpib.macMinBE = par("macMinBE");
	mpib.macMaxBE = par("macMaxBE");
    }
    else
    {
	if(backoffMethodStr == "linear")
	    backoffMethod = LINEAR;
	else if(backoffMethodStr == "constant")
	    backoffMethod = CONSTANT;
	else
	{
	    error("Unknown backoff method \"%s\".\
    	                       Use \"constant\", \"linear\" or \"\
    	                       \"exponential\".", backoffMethodStr.c_str());
	}
	initialCW = par("contentionWindow"); // need to be check [SR]
    }
    NB = 0;
    BE = 0;

    panStartTime = 5.0;

    secuData = false;    // security not implemented
    secuBeacon = false;    // security not implemented

    taskP.init();
    taskP.TS_RX_CCA_tschcca = false;
    taskP.TS_TX_CCA_tschcca = false;
    taskP.TS_RX_CCA_tschcca_STEP = 0;
    taskP.TS_TX_CCA_tschcca_STEP = 0;

    // for beacon
    rxBO = 15;       // set through the beacon
    rxSO = 15;       // set through the beacon
    beaconWaitingTx = false;
    notAssociated = true;
    bcnLossCounter = 0;

    // for transmission
    inTransmission = false;
    waitBcnCmdAck = false;
    waitBcnCmdUpperAck = false;
    waitDataAck = false;
    waitGTSAck = false;
    numBcnCmdRetry = 0;
    numBcnCmdUpperRetry = 0;
    numDataRetry = 0;
    numGTSRetry = 0;

    // for CSMA-CA
    csmacaWaitNextBeacon = false;
    backoffStatus = 0;

    // for timer
    lastTime_bcnRxTimer = 0;
    inTxSD_txSDTimer = false;
    inRxSD_rxSDTimer = false;
    index_gtsTimer = 0;

    // device capability
    capability.alterPANCoor = false;
    capability.FFD = true;
    capability.powerSource = false;
    capability.recvOnWhenIdle = mpib.macRxOnWhenIdle; // default: true
    capability.secuCapable = false;
    capability.alloShortAddr = true;
    capability.hostName = getParentModule()->getParentModule()->getFullName();

    // GTS variables for PAN coordinator
    gtsCount = 0;
    for(int i = 0; i < 7; i++)
    {
	gtsList[i].devShortAddr = def_macShortAddress;  // 0xffff
	gtsList[i].startSlot = 0;
	gtsList[i].length = 0;
	gtsList[i].isRecvGTS = false;
	gtsList[i].isTxPending = false;
    }
    tmp_finalCap = aNumSuperframeSlots - 1; // 15 if no GTS
    indexCurrGts = 99;
    isTxPending = false;

    // GTS variables for devices
    gtsLength = 0;
    gtsStartSlot = 0;
    gtsTransDuration = 0;
    txPkt = NULL;
    txBeacon = NULL;
    txBcnCmdUpper = NULL;
    txBcnCmd = NULL;
    txData = NULL;
    txGTS = NULL;
    txAck = NULL;
    txCsmaca = NULL;
    tmpCsmaca = NULL;
    rxBeacon = NULL;
    rxData = NULL;
    rxCmd = NULL;

    resetTRX();

    msg->setKind(TP_RESTART_CONFIRM);
    msg->setName("Restart Confirm");
    RESTART_confirm(msg);
}

void Ieee802154eMacRLL::RESTART_confirm(cMessage *msg)
{
    send(msg->dup(), mUpperLayerOut);
    delete msg;
}

void Ieee802154eMacRLL::handleAwaitingBeaconTimer()
{
    awaitingNextBeacon = false;
}

void Ieee802154eMacRLL::handleEB(cMessage *msg)
{
    simtime_t now, tmpf, duration;
    uint16_t ifs;
    bool firstBe = false;
    double timeCorr = 0.0;
    now = simTime();

    //calculate the time when the first bit of the beacon was received

    awaitingNextBeacon = false;

    if(dynamic_cast<Ieee802154EnhancedBeaconFrame*>(msg))
    {
	rxBeacon = check_and_cast<Ieee802154EnhancedBeaconFrame *>(msg);
	duration = calDuration(rxBeacon);
	bcnRxTime = rxBeacon->getTimestamp() - duration;
	firstBe = true;
    }
    else
    {
	delete msg;
	duration = calDuration(rxBeacon);
	bcnRxTime = now - duration;
    }

    /**
     * important: this value is calculated in <csmacaStart()>, if later on a CSMA-CA is pending
     *        for this bcn and backoff will resume without calling <csmacaStart()> (see <csmacaTrxBeacon()>) ,
     *        therefore this value will not be updated, but <csmacaCanProceed()> and other functions will
     *        use it and needs to be updated here
     */
    schedBcnRxTime = bcnRxTime;
    EV << "The first bit of this beacon was received by PHY layer at " << bcnRxTime << endl;

    //calculate <rxBcnDuration>
    if(rxBeacon->getByteLength() <= aMaxSIFSFrameSize)
	ifs = mpib.macSIFSPeriod;
    else
	ifs = mpib.macLIFSPeriod;

    tmpf = duration * phy_symbolrate;
    tmpf += ifs;
    rxBcnDuration = (uint16_t)(SIMTIME_DBL(tmpf) / aUnitBackoffPeriod);

    if(fmod(tmpf, aUnitBackoffPeriod) > 0.0)
	rxBcnDuration++;

    //update PAN descriptor
    rxPanDescriptor.coordAddrMode = (Ieee802154eAddrMode)rxBeacon->getFrmCtrl().srcAddrMode;
    rxPanDescriptor.coordPANId = rxBeacon->getSrcPanId();
    rxPanDescriptor.coordAddress_16_or_64 = rxBeacon->getSrcAddr();
    rxPanDescriptor.logicalChannel = ppib.phyCurrentChannel;

    // handle the IE's
    handleIEfield(rxBeacon);
    if(firstBe)
    {
	timeCorr = simTime().dbl() - rxBeacon->getCreationTime().dbl();

	int asnSlots = (timeCorr / timeslotTable->getTemplate(useTimeslotID)->getTimeslotLengthDbl()) + 1;

	mpib.macASN += asnSlots;
    }

    // reset lost beacon counter
    bcnLossCounter = 0;
    mpib.macTSCHenabled = mpib.macTSCHcapable ? true:false; //activate TSCH mode only if the device is TSCH capable
    backoffMethod = EXPONENTIAL;
    // TSCH-specific MAC PIB attributes - Std 802.15.4e-2012 (table 52b) page 174
    mpib.macMinBE = par("macMinBE");    // default: for CSMA-CA = 3; for TSCH-CA = 1
    mpib.macMaxBE = par("macMaxBE");    // default: for CSMA-CA = 5; for TSCH-CA = 7

    if(firstBe)
    {
	double temp = bcnRxTime.dbl() - (timeslotTable->getTemplate(useTimeslotID)->getRxOffsetDbl() + (timeslotTable->getTemplate(useTimeslotID)->getRxWaitDbl() / 2)) + timeCorr;
	int tempi = temp / timeslotTable->getTemplate(useTimeslotID)->getTimeslotLengthDbl();
	temp = tempi * timeslotTable->getTemplate(useTimeslotID)->getTimeslotLengthDbl() + timeslotTable->getTemplate(useTimeslotID)->getTimeslotLengthDbl();
	// start ASN timer
	startAsnTimer(temp);
    }
    else
    {
	startAsnTimer(bcnRxTime.dbl() - (timeslotTable->getTemplate(useTimeslotID)->getRxOffsetDbl() + (timeslotTable->getTemplate(useTimeslotID)->getRxWaitDbl() / 2)));
    }

    //    ASSERT(mpib.macCoordShortAddress == def_macCoordShortAddress);
    //    ASSERT(mpib.macPANId == def_macPANId);
    notAssociated = notAssociated;

    mpib.macShortAddress = (UINT_16)mpib.macExtendedAddress.getInt();
    mpib.macSimpleAddress = (UINT_8)mpib.macExtendedAddress.getInt();

    mpib.macPANId = rxBeacon->getSrcPanId();           // store PAN id
    mpib.macCoordShortAddress = getShortAddress(rxBeacon->getSrcAddr()); // store coordinator address, always use short address
    mpib.macCoordExtendedAddress = rxBeacon->getSrcAddr(); // PAN coordinator uses the same address for both its own 16 and 64 bit address

    mpib.macSuperframeOrder = (UINT8)rxBeacon->getSO();
    mpib.macBeaconOrder = (UINT8)rxBeacon->getBO();
    if(awaitingBeacon->isScheduled())
	cancelEvent(awaitingBeacon);
    scheduleAt(simTime() + pow(2, mpib.macBeaconOrder) * mpib.macSuperframeOrder * timeslotTable->getTemplate(0)->getTimeslotLengthDbl() - timeslotTable->getTemplate(0)->getTimeslotLengthDbl(), awaitingBeacon);
    // time correction
    calcTimeCorr(rxBeacon);

    taskP.TS_RX_CCA_tschcca = false;
    taskP.TS_TX_CCA_tschcca = false;
    taskP.TS_RX_CCA_tschcca_STEP = 0;
    taskP.TS_TX_CCA_tschcca_STEP = 0;

    dispatch(phy_SUCCESS, __FUNCTION__);

    resetTRX();
    Ieee802154eNetworkCtrlInfo *tmp = new Ieee802154eNetworkCtrlInfo("BeaconConfirm", TP_MLME_SET_BEACON_CONFIRM);
    send(tmp->dup(), mUpperLayerOut);
    delete tmp;
    delete rxBeacon;
    rxBeacon = NULL;
    //delete bcnFrame;
    numRxBcnPkt++;
}

void Ieee802154eMacRLL::handleAck(Ieee802154eFrame *frame)
{
    if(frame->getFrmCtrl().frameType == Ieee802154e_ACK)
    {
	if(frame->getFrmCtrl().frameVersion == Ieee802154_compatible) // 802.15.4e Enhanced ACK frame
	{
	    // TODO:[SR] handle timekeeping and keepalive

	    // TSCH CCA - Transmitter
	    if(mpib.macTSCHenabled && taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) && taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 10)
	    {
		//check the sequence number in the ACK to see if it matches that in the <txPkt>
		ASSERT(txData);

		// stop the TsAckWaitTimer
		if(tsAckWaitTimer->isScheduled())
		    cancelEvent(tsAckWaitTimer);

		if(frame->getSeqNmbr() != txData->getSeqNmbr())
		{
		    delete frame;
		    EV << "[TSCH CCA]-Transmitter:[11] frame was dropped at the MAC layer, the SeqNumber in the ACK does not match" << endl;

		    taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 11;
		    handleTsAckWait();
		    // next step in handleTsAckWait()
		    return;
		}
		else // successful transmission, with ACK
		{
		    // handle the Header and Payload IE's and check if there was a NACK inside
		    if(handleIEfield(frame))
			mpib.macNACKcount++; // count NACK

		    // count successful transmission
		    int retries = txData->getRetries();
		    if(mpib.macMetricsEnabled)
		    {
			if(retries == 0)
			    mpib.macTXSuccessCount++;
			else if(retries == 1)
			    mpib.macRetryCount++;
			else
			{
			    mpib.macMultipleRetryCount++;
			    if(retries == 2)
				numRetryTwo++;
			    else if(retries == 3)
				numRetryThree++;
			    else if(retries == 4)
				numRetryFour++;
			}
		    }

		    //		    if(retries == 4)
		    //			int test = 0;

		    // record for the statistics (with slotframe size 100 and sim end time=10000, we can use the ASN number direct)
		    int timeslot = FWMath::modulo(curASN, slotframeTable->getSlotframe(activeLinkEntry->getSlotframeId())->getSlotframeSize());
		    timeslot = retries * 100 + timeslot;
		    emit(sucTimeslot, timeslot);

		    // count the transmission in the neighborTable
		    UINT_16 destAddr = getShortAddress(txData->getDstAddr());

		    taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) = false;
		    taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 0;

		    string tmpTxt = (txData->getFrmCtrl().frameType == Ieee802154e_DATA) ? txData->getEncapsulatedPacket()->getName():frameTypeTxt[txData->getFrmCtrl().frameType];
		    EV << "[TSCH CCA]-Transmitter:[10] the correct ACK arrives, transmission of the " << txPkt->getName() << "(" << tmpTxt << ") frame was successful, delete frame from queue" << endl;

		    MCPS_DATA_confirm(0, txData->getArrivalTime().getScale(), false, 0, 0, 0, 0, 0, mac_SUCCESS, txData->getRetries(), 0, txData);

		    // delete the msg from the queue
		    queueModule->deleteMsgQueue(txData->getDstAddr(), false);

		    /**note:
		     * A successful transmission in a shared link resets the backoff window to the minimum value.
		     * The backoff window is reset to the minimum value if the transmission in a dedicated link
		     * is successful and the transmit queue is then empty.
		     * Std 802.15.4e-2012 (5.1.1.4.3 TSCH CSMA-CA algorithm) page 14 */
		    if(destAddr != 0xffff) // if is not the broadcast address
		    {
			activeNeighborEntry = neighborTable->getNeighborBySAddr(destAddr);

			if(tschSharedLink || queueModule->isEmpty())
			{
			    BE = mpib.macMinBE;
			    if(activeNeighborEntry != NULL)
				activeNeighborEntry->setTransDelay(-1); // reset the transmission delay
			}

			if(activeNeighborEntry != NULL) // count the successful transmission
			    activeNeighborEntry->incrNumTxDataAck();
		    }

		    delete frame;   // delete the ACK
		    if(txPkt == txData)
		    {
			if(txData->getFrmCtrl().frameType == Ieee802154e_SCHEDULER_RESPONSE)
			{
			    SCHEDULE_confirm(txData->dup(), true);
			}
			if(txData->getFrmCtrl().frameType == Ieee802154e_DISASSOCIATION_RESPONSE)
			{
			    neighborTable->deleteNeighbor(neighborTable->getNeighborBySAddr(getShortAddress(txData->getDstAddr())));
			}
			delete txData;
			txData = NULL;
		    }
		    numRxAckPkt++;

		    resetTRX();
		    // finish
		}
	    }
	}
    }
}

void Ieee802154eMacRLL::handleAsnTimer()
{
    if(mpib.macTSCHenabled && !taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) && !taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA))
    {
	startTimeslot = simTime(); // start time of this timeslot

	// set the timer for the next timeslot, depending at the ASN
	if(asnTimer->isScheduled())
	    cancelEvent(asnTimer);

	// get the active LinkEntry
	int tmpASN = mpib.macASN;
	activeLinkEntry = getLinkEntry(mpib.macASN);
	curASN = mpib.macASN;

	// wake up to the next active timeslot
	double nextTime = getNextActiveTimeslot();
	scheduleAt(simTime() + nextTime, asnTimer);

	EV << "[TSCH CCA]-Time: ASN=" << tmpASN << "; timeCorrection=" << timeCorrection << "s; next ASN count=" << (int)mpib.macASN << " next active time=" << simTime() + nextTime << "s" << endl;

	if(activeLinkEntry == NULL)
	    return;

	EV << activeLinkEntry->info() << endl;

	//check if beacon is ready
	if(activeLinkEntry->getLinkType() == LNK_TP_ADVERTISING && txBeaconNow && !activeLinkEntry->isprevStage())
	{
	    constructBCN();
	    MLME_BEACON_confirm(mac_SUCCESS);
	    txBeaconNow = false;
	}

	// request the frame for this dest address
	cMessage *tmpMsg = NULL;

	switch(activeLinkEntry->getLinkType())
	{
	    case LNK_TP_NORMAL:
		tmpMsg = queueModule->requestSpcPacket((IE3ADDR)activeLinkEntry->getNodeAddress());
		break;
	    case LNK_TP_ADVERTISING:
		if(!awaitingNextBeacon || isPANCoor)
		{
		    tmpMsg = queueModule->requestBeaconPacket();

		    if(tmpMsg == NULL)
		    {
			tmpMsg = queueModule->requestAdvPacket();
		    }
		    if(tmpMsg == NULL && activeLinkEntry->isnextStage())
		    {
			tmpMsg = queueModule->requestDisAssPacket(true);
		    }

		    if(tmpMsg == NULL)
		    {
			tmpMsg = queueModule->requestSchdulePacket();
		    }

		    if(tmpMsg == NULL)
			tmpMsg = NULL;
		}
		else
		    tmpMsg = NULL;
		break;

	    case LNK_TP_JOIN:
		if(!awaitingNextBeacon)
		{
		    if(activeLinkEntry->isprevStage())
		    {
			tmpMsg = queueModule->requestAdvPacket();

			if(tmpMsg == NULL)
			    tmpMsg = queueModule->requestDisAssPacket(false);
		    }
		}
		break;
	    default:
		tmpMsg = NULL;
		break;

	}

	//	if(activeLinkEntry->getLinkType() == LNK_TP_NORMAL)
	//	    tmpMsg = queueModule->requestSpcPacket((IE3ADDR)activeLinkEntry->getNodeAddress());
	//
	//	if(activeLinkEntry)
	//
	//	if(tmpMsg == NULL && activeLinkEntry->getLinkType() == LNK_TP_ADVERTISING)
	//	{
	//	    //check if packet is available for advertisment
	//	    tmpMsg = queueModule->requestAdvPacket();
	//	}
	//	if(tmpMsg == NULL && activeLinkEntry->getLinkType() == LNK_TP_ADVERTISING)
	//	{
	//	    tmpMsg = queueModule->requestSchdulePacket();
	//	}

	if(tmpMsg != NULL)
	{
	    txData = check_and_cast<Ieee802154eFrame *>(tmpMsg);
	    //	    if(txData->getFrmCtrl().frameType == Ieee802154e_BEACON)
	    //	    {
	    //		//check if asn is still up to date
	    //		checkBeaconASN(txData);
	    //
	    //	    }

	}

	else
	    txData = NULL;

	MACTSCHLinkOptions linkOption = activeLinkEntry->getLinkOption();

	// if we have nothing to send in a shared_receive timeslot, set the LinkOption to receive
	if(linkOption == LNK_OP_SHARED_RECEIVE && txData == NULL)
	    linkOption = LNK_OP_RECEIVE;	    // set the LinkOption to receive, if no frame is to send
	else if(linkOption == LNK_OP_SHARED_RECEIVE && txData != NULL)
	    linkOption = LNK_OP_SHARED;	    // set the LinkOption to shared, if one frame is to send

	/** check the linkOption of the link for this timeslot
	 *  see Std 802.15.4e-2012 (Figure 22b - Timeslot diagram of acknowledged transmission) page 28 */
	if(linkOption == LNK_OP_TRANSMIT || linkOption == LNK_OP_SHARED) // TSCH CCA algorithm OR TSCH CSMA-CA algorithm for shared links
	{
	    tschSharedLink = (linkOption == LNK_OP_SHARED) ? true:false; // for shared links

	    if(txData != NULL)
	    {
		int retries = txData->getRetries();
		activeNeighborEntry = NULL;
		UINT_16 destAddr = getShortAddress(txData->getDstAddr());
		if(destAddr != 0xffff) // if is not the broadcast address
		{
		    activeNeighborEntry = neighborTable->getNeighborBySAddr(destAddr);

		    if(activeNeighborEntry == NULL)
			return; // Cancel the transmission if we have no neighbor for this data pkt
		}

		string tmpTxt = (txData->getFrmCtrl().frameType == Ieee802154e_DATA) ? txData->getEncapsulatedPacket()->getName():frameTypeTxt[txData->getFrmCtrl().frameType];
		EV << "[TSCH CCA]-Transmitter: " << retries << " retries for this " << txData->getName() << "(" << tmpTxt << ") frame" << endl;

		if(tschSharedLink && backoffMethod == EXPONENTIAL && retries <= mpib.macMaxFrameRetries && retries > 0 && activeNeighborEntry != NULL)
		{ // in TSCH mode the device shall use an exponential backoff mechanism - Std 802.15.4e-2012 (5.1.1.4.3 TSCH CSMA-CA algorithm) page 14
		    int td = activeNeighborEntry->getTransDelay(); // transmission delayed

		    if(td < 0)
		    { // calculation from the cmsa802154.cc file
			double d = pow((double)2, (int)BE);
			int v = (int)d - 1;
			int r = intuniform(0, v, 0);
			td = r;
			activeNeighborEntry->setTransDelay(r);
			EV << "[TSCH CCA]-Transmitter (shared links): retransmission delayed for " << r << " for shared links" << endl;
		    }

		    // check if we need to delay the transmission
		    if(td > 0)
		    {
			EV << "[TSCH CCA]-Transmitter (shared links): transmission delay for " << td << " shared links" << endl;
			activeNeighborEntry->decrTransDelay(); // reduce the transmission delay for the next shared link
			//			resetTRX();
			//			return;
			//We have to wait for transmit the packet, so turn on the receiver to listen;
			taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) = true;
			taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) = 1;
			// start the macTsRxOffset timer
			if(tsRxOffsetTimer->isScheduled())
			    cancelEvent(tsRxOffsetTimer);

			EV << "[TSCH CCA]-Receiver:[1] wait the TsRxOffset" << endl;
			scheduleAt(simTime() + timeslotTable->getTemplate(useTimeslotID)->getRxOffsetDbl(), tsRxOffsetTimer);
			return;
		    }
		    else
			// for the case td == 0, to have the td == -1 for the recalculation of the transmission delay
			activeNeighborEntry->decrTransDelay();
		}
		else
		{
		    if(retries > mpib.macMaxFrameRetries)
			EV << "Whats going on???";
		}

		taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) = true;
		taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 1;

		if(useCCA)
		{
		    taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 1;

		    // start the macTsCCAOffset timer
		    if(tsCCAOffsetTimer->isScheduled())
			cancelEvent(tsCCAOffsetTimer);

		    EV << "[TSCH CCA]-Transmitter:[1] use CCA, wait the TsCCAOffsetTime" << endl;
		    scheduleAt(simTime() + timeslotTable->getTemplate(useTimeslotID)->getCCAOffsetDbl(), tsCCAOffsetTimer);
		    // next step in handleTsCCAOffset()
		}
		else
		{
		    taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 100;

		    // start the macTsCCAOffset timer
		    if(tsTxOffsetTimer->isScheduled())
			cancelEvent(tsTxOffsetTimer);

		    EV << "[TSCH CCA]-Transmitter:[100] no CCA, wait the TsTxOffsetTime" << endl;
		    scheduleAt(simTime() + timeslotTable->getTemplate(useTimeslotID)->getTxOffsetDbl(), tsTxOffsetTimer);
		    // next step in handleTsTxOffset()
		}
	    }
	    else
	    {
		EV << "[TSCH CCA]-Transmitter: no pkt to send, go back to sleep" << endl;
		taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) = false;
		taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 0;

		resetTRX();
	    }
	}
	else if(linkOption == LNK_OP_RECEIVE || linkOption == LNK_OP_RECEIVE_TIMEKEEPING) // Receiving Link
	{
	    taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) = true;
	    taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) = 1;
	    // start the macTsRxOffset timer
	    if(tsRxOffsetTimer->isScheduled())
		cancelEvent(tsRxOffsetTimer);

	    EV << "[TSCH CCA]-Receiver:[1] wait the TsRxOffset" << endl;
	    scheduleAt(simTime() + timeslotTable->getTemplate(useTimeslotID)->getRxOffsetDbl(), tsRxOffsetTimer);
	    // next step in handleTsRxOffset()
	}
	else    // Reserved
	{
	    EV << "[TSCH CCA]-Reserved: nothing to do, go back to sleep" << endl;
	    resetTRX();
	}
    }
    else
    {
	taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) = false;
	taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 0;
	taskP.taskStatus(TP_TS_RX_CCA_TSCHCCA) = false;
	taskP.taskStep(TP_TS_RX_CCA_TSCHCCA) = 0;
	resetTRX();
	queueModule->deleteBCNQueue();


	scheduleAt(simTime(), asnTimer);
    }
}

void Ieee802154eMacRLL::handleTsAckWait()
{
    if(taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) && (taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 5 || taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 9 || taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 11 || taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 99))
    {
	ASSERT(txData);

	taskP.taskStatus(TP_TS_TX_CCA_TSCHCCA) = false;
	taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) = 0;

	// see Std 802.15.4e-2012 (5.1.1.4.3 TSCH CSMA-CA algorithm) page 14 or (figure 11b11b�TSCH CSMA-CA backoff) page 16
	int retries = txData->getRetries();

	// increment retry counter
	txData->setRetries(++retries);

	/** for shared links
	 *   calculate the backoff after a first transmission failure */
	if(tschSharedLink && retries > 1)
	{

	    BE++;
	    BE = (BE <= mpib.macMaxBE) ? BE:mpib.macMaxBE;
	}

	if(retries > mpib.macMaxFrameRetries) // failed retransmission; max retransmission for this packet
	{
	    EV << "[TSCH CCA]-Transmitter:[12] no Ack for this transmission arrived in the TsAckWaitTime, maximum retries arrived, drop this frame" << endl;

	    // record for the statistics (with slotframe size 100 and sim end time=10000, we can use the ASN number direct)
	    int timeslot = FWMath::modulo(curASN, slotframeTable->getSlotframe(activeLinkEntry->getSlotframeId())->getSlotframeSize());
	    emit(sucTimeslot, -timeslot);

	    // to many retries, send MCPS-DATA.confirm with NO_ACK and delete the pkt from the queue
	    MCPS_DATA_confirm(txData->getSeqNmbr(), 0, false, 0, 0, 0, 0, 0, mac_NO_ACK, txData->getRetries(), 0, NULL);
	    queueModule->deleteMsgQueue(txData->getDstAddr(), false);

	    if(txData->getFrmCtrl().frameType == Ieee802154e_SCHEDULER_REQUEST)
	    {

		SCHEDULE_confirm(txData->dup(), false);
	    }

	    //	    if(txData->getFrmCtrl().frameType == Ieee802154e_SCHEDULER_RESPONCE)
	    //	    {
	    //		macLinkTableEntry *entry = new macLinkTableEntry();
	    //		Ieee802154eNetworkCtrlInfo *cntrl = check_and_cast<Ieee802154eNetworkCtrlInfo *>(&check_and_cast<Ieee802154eScheduleFrame *>(txData)->getCntrlInfo());
	    //		//fixme:
	    //		entry = linkTable->getLinkByTimeslotOffsetAddress(cntrl->getTimeslot(), cntrl->getChannelOffset(), cntrl->getDeviceAddress());
	    //		if(entry != NULL)
	    //		    linkTable->deleteLink(entry);
	    //	    }
	    delete txData;
	    txData = NULL;

	    // count the failed retransmission
	    if(mpib.macMetricsEnabled)
		mpib.macTXFailCount++;
	}
	else
	{ // failed retransmission: retry the next time
	    if(taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 5)
		EV << "[TSCH CCA]-Transmitter:[12] data transmission failed, go back to sleep, try the next timeslot to the dest address" << endl;
	    else if(taskP.taskStep(TP_TS_TX_CCA_TSCHCCA) == 99)
		EV << "[TSCH CCA]-Transmitter:[12] CCA failed, go back to sleep, try the next timeslot to the dest address" << endl;
	    else
		EV << "[TSCH CCA]-Transmitter:[12] no Ack for this transmission arrived in the TsAckWaitTime, try the next timeslot to the dest address" << endl;
	}

	// delete the pkt
	if(txPkt == txBeacon)
	{
	    beaconWaitingTx = false;
	    delete txBeacon;
	    txBeacon = NULL;
	}
	else if(txPkt == txAck)
	{
	    delete txAck;
	    txAck = NULL;
	}
	else if(txPkt == txData)
	{
	    txData = NULL;
	}

	resetTRX();
	// finish
    }
}

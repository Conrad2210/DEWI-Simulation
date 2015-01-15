/*
 * ccsma.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: user
 */

#include <CCSMA/ccsma.h>
#include "CCSMAFrame_m.h"
#include "BurstMSG_m.h"
Define_Module(ccsma);
ccsma::ccsma()
{
	// TODO Auto-generated constructor stub

}

ccsma::~ccsma()
{
	// TODO Auto-generated destructor stub
}

void ccsma::handleLowerMsg(cPacket *msg)
{

	Ieee802154Frame *macPkt = dynamic_cast<Ieee802154Frame *>(msg);
	Ieee802154Frame *macPktRe = dynamic_cast<Ieee802154Frame *>(msg)->dup();
	MACAddress src = macPkt->getSrcAddr();
	MACAddress dest = macPkt->getDstAddr();
	//long ExpectedNr = 0;
	uint8_t ExpectedNr = 0;
	if (msg->getControlInfo())
		delete msg->removeControlInfo();

	if (macPkt->getKind() != PACKETOK)
	{
		EV << "Received with errors frame name= " << macPkt->getName()
					<< ", myState=" << macState << " src="
					<< macPkt->getSrcAddr() << " dst=" << macPkt->getDstAddr()
					<< " myAddr=" << getMacAddr() << endl;
		if (macPkt->getKind() == COLLISION)
		{
			EV << "[MAC]: frame corrupted due to collision, dropped" << endl;
			numCollision++;
		}
		delete macPkt;
		return;
	}

	EV << "Received frame name= " << macPkt->getName() << ", myState="
				<< macState << " src=" << macPkt->getSrcAddr() << " dst="
				<< macPkt->getDstAddr() << " myAddr=" << getMacAddr() << endl;

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
				EV << "Received a data packet addressed to me,"
							<< " preparing an ack..." << endl;

				nbRxFrames++;

				if (ackMessage != NULL)
					delete ackMessage;
				ackMessage = new Ieee802154Frame("CSMA-Ack");
				ackMessage->setSrcAddr(getMacAddr());
				ackMessage->setDstAddr(macPkt->getSrcAddr());
				ackMessage->setBitLength(ackLength);
				//Check for duplicates by checking expected seqNr of sender
				if (SeqNrChild.find(src) == SeqNrChild.end())
				{
					//no record of current child -> add expected next number to map
					SeqNrChild[src] = SeqNr + 1;
					EV << "Adding a new child to the map of Sequence numbers:"
								<< src << endl;
					executeMac(EV_FRAME_RECEIVED, macPkt);
				}
				else
				{
					ExpectedNr = SeqNrChild[src];
					EV << "Expected Sequence number is " << ExpectedNr
								<< " and number of packet is " << SeqNr << endl;
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
			else if (macQueue.size() != 0)
			{

				// message is an ack, and it is for us.
				// Is it from the right node ?
				Ieee802154Frame * firstPacket =
						static_cast<Ieee802154Frame *>(macQueue.front());
				if (macPkt->getSrcAddr() == firstPacket->getDstAddr())
				{
					nbRecvdAcks++;
					executeMac(EV_ACK_RECEIVED, macPkt);
				}
				else
				{
					EV
								<< "Error! Received an ack from an unexpected source: src="
								<< macPkt->getSrcAddr()
								<< ", I was expecting from node addr="
								<< firstPacket->getDstAddr() << endl;
					delete macPkt;
				}
			}
			else
			{
				EV
							<< "Error! Received an Ack while my send queue was empty. src="
							<< macPkt->getSrcAddr() << "." << endl;
				delete macPkt;
				delete macPktRe;
			}
		}
	}
	else if (dest.isBroadcast())
	{
		if (macPktRe->getEncapsulatedPacket()->getEncapsulatedPacket()->getEncapsulatedPacket()->hasPar(
				"sourceId")
				&& macPktRe->getEncapsulatedPacket()->getEncapsulatedPacket()->getEncapsulatedPacket()->hasPar(
						"msgId"))
		{
			// duplicate control
			int moduleId =
					(int) macPktRe->getEncapsulatedPacket()->getEncapsulatedPacket()->getEncapsulatedPacket()->par(
							"sourceId");
			int msgId =
					(int) macPktRe->getEncapsulatedPacket()->getEncapsulatedPacket()->getEncapsulatedPacket()->par(
							"msgId");
			SourceSequence::iterator it = ssSourceSequence.find(moduleId);
			if (it != ssSourceSequence.end())
			{
				if (it->second >= msgId)
				{
					if (ev.isGUI())
						EV << "Packet double received: "
									<< (int) macPktRe->getEncapsulatedPacket()->getEncapsulatedPacket()->getEncapsulatedPacket()->par(
											"msgId") << endl;
					delete macPkt;
					delete macPktRe;
					return;
				}
				else
					it->second = msgId;
			}
			else
				ssSourceSequence[moduleId] = msgId;
		}

		if (macPktRe->getEncapsulatedPacket()->getEncapsulatedPacket()->getEncapsulatedPacket()->par(
				"BroadcastPk"))
		{

			EV << "Broadcast Packet received and resend packet\n";

			ipDestAddr = IPvXAddress("255.255.255.255");

			dynamic_cast<BurstMSG *>(macPktRe->getEncapsulatedPacket()->getEncapsulatedPacket()->getEncapsulatedPacket())->setHopCount(
					dynamic_cast<BurstMSG *>(macPktRe->getEncapsulatedPacket()->getEncapsulatedPacket()->getEncapsulatedPacket())->getHopCount()
							+ 1);
			if (par("isLightSwitch").boolValue())
			{
				delete macPktRe;
			}
			else
			{
				scheduleAt(simTime() + par("reBroadcastDelay").doubleValue(),
						macPktRe);
			}

			nbRxFramesBroadcast++;
			executeMac(EV_BROADCAST_RECEIVED, macPkt);

		}

	}
	else
	{
		EV << "packet not for me, deleting...\n";
		delete macPkt;
	}
}

void ccsma::handleSelfMsg(cMessage *msg)
{
	std::string temp = msg->getClassName();
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
	else if (temp == "Ieee802154Frame")
	{
		executeMac(EV_SEND_REQUEST, msg);
	}
	else
		EV << "CSMA Error: unknown timer fired:" << msg << endl;
}

void ccsma::sendDown(Ieee802154Frame* frame)
{
    /*if (updateNFailLink(fl_oper_est,index_) == 0)
     {
     if (txBeacon)
     {
     beaconWaiting = false;
     Packet::free(txBeacon);
     txBeacon = 0;
     }
     return;
     }
     else if (updateLFailLink(fl_oper_est,index_,p802_15_4macDA(p)) == 0)
     {
     dispatch(p_UNDEFINED,"PD_DATA_confirm");
     return;
     }*/

// TBD: energy model

    inTransmission = true;     // cleared by PD_DATA_confirm
    send(frame, mLowerLayerOut);     // send a duplication
    EV << "[MAC]: sending frame " << frame->getName() << " (" << frame->getByteLength() << " Bytes) to PHY layer"
            << endl;
    EV << "[MAC]: the estimated transmission time is " << calDuration(frame) << " s" << endl;

}

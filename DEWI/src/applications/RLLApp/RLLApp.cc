//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY{} without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <RLLApp.h>
#include "RLLAppMsg_m.h"
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
Define_Module(RLLApp);
RLLApp::RLLApp()
{
	BurstTimer = NULL;
	BurstMessageTimer = NULL;
	StopTimer = NULL;
	dataCenter = NULL;
	AssTimer = NULL;
}

RLLApp::~RLLApp()
{
	cancelAndDelete(BurstTimer);
	cancelAndDelete(BurstMessageTimer);
	cancelAndDelete(StopTimer);
	cancelAndDelete(AssTimer);
}

void RLLApp::initialize(int stage)
{
	TrafGenPar::initialize(stage);
	EV << getParentModule()->getFullName() << ": initializing RLL Test App, stage=" << stage << endl;
	if (0 == stage)
	{
		m_debug = par("debug");
		mLowerLayerIn = findGate("lowerLayerIn");
		mLowerLayerOut = findGate("lowerLayerOut");
		m_moduleName = getParentModule()->getFullName();

		m_isLightSwitch = par("LightSwitch");
		m_numberMessageSend = 0;
		m_burstCounter = 0;
		m_messageCounter = 0;
		m_burstDuration = par("burstDuration").doubleValue();
		m_interArrivalTime = par("interArrivalTime").doubleValue();
		m_BurstPause = par("BurstPause").doubleValue();
		m_AppStartTime = par("StartTime").doubleValue();
		m_totalBurstToSend = par("numberOfBursts");
		AssTimer = new cMessage("AssTimer");
		counterRxMsg = 0;
		counterTxMsg = 0;
		m_numberMessageToSend = (int) (m_burstDuration / m_interArrivalTime);

		BurstTimer = new cMessage("BurstTimer");
		BurstMessageTimer = new cMessage("BurstMessageTimer");
		StopTimer = new cMessage("AppStopTimer");

		std::stringstream a;
		a << getParentModule()->getName() << " " << getParentModule()->getIndex();
		//EndToEndDelay = new cOutVector(a.str().c_str());
		E2E = new DataVector(a.str(), "latency");
		receivedMSG = new DataVector(a.str(),"rxMSG");
		if (m_isLightSwitch)
		{
			scheduleAt(simTime() + 5, AssTimer);
		}

	}
	if (1 == stage)
	{

			E2E->registerVector();
			receivedMSG->registerVector();

		dataCenter = check_and_cast<DataCenter *>(dataCenter->getModuleByPath("DataCenter"));
		if (strcmp("gateWay", getParentModule()->getName()))
			dataCenter->registerAssociatedVector(getParentModule()->getIndex(), getParentModule()->getName(), false, -1, -1, "");
	}
}

void RLLApp::finish()
{
	cancelEvent(BurstTimer);
	cancelEvent(BurstMessageTimer);
	cancelEvent(StopTimer);
	cancelEvent(AssTimer);
	std::stringstream sss;
	sss << getParentModule()->getIndex();
	std::stringstream ss;
	ss << counterRxMsg;
	dataCenter->recordScalar(ss.str(),"scaRxMsg", getParentModule()->getName(), sss.str());
	ss.str("");
	ss << counterTxMsg;
	dataCenter->recordScalar(ss.str(),"scaTxMsg", getParentModule()->getName(), sss.str());
}

// OPERATIONS
void RLLApp::handleSelfMsg(cMessage* msg)
{
	if (msg == BurstTimer)
	{
		startBurst();
	}
	else if (msg == BurstMessageTimer)
	{
		sendNextBurstMessage();
	}
	else if (msg == StopTimer)
	{
		endSim();
	}
	else if (msg == AssTimer)
	{
		checkAssociation();
	}

	else
	{
		if (ev.isGUI())
		{
			EV << "Unrecognized self message: Do nothing" << endl;
		}
	}

}
void RLLApp::handleLowerMsg(cMessage* msg)
{
	double now = simTime().dbl();

	RLLAppMsg *tmp = check_and_cast<RLLAppMsg *>(msg);
	msg = NULL;

	std::stringstream ss;
	ss << "Received: " << tmp->getName();
	getParentModule()->bubble(ss.str().c_str());
	E2E->record((now - tmp->getCreationTime().dbl()));
	counterRxMsg++;
	receivedMSG->record(tmp->getName());
	delete tmp;
}

void RLLApp::checkAssociation()
{
	if (m_isLightSwitch)
	{
		if (dataCenter->allAssociated())
		{

			scheduleAt(simTime() + 10, BurstTimer);

		}

		else
		{
			if (AssTimer->isScheduled())
				cancelEvent(AssTimer);

			scheduleAt(simTime() + 5, AssTimer);
		}
	}
}

void RLLApp::SendTraf(cPacket *msg, const char* dest)
{
	delete msg;

	RLLAppMsg* appPkt = new RLLAppMsg("RLLAppMsg");
	appPkt->setBitLength(PacketSize() * 8);
	appPkt->setSourceName(m_moduleName);
	appPkt->setDestName("Broadcast");
	appPkt->setCreationTime(simTime());

	send(appPkt->dup(), mLowerLayerOut);
	delete appPkt;
}

void RLLApp::startBurst()
{
	if (m_burstCounter <= m_totalBurstToSend)
	{
		char msgName[32];
		sprintf(msgName, "RLLAppMsg-%d-%d", m_burstCounter, m_messageCounter++);
		RLLAppMsg* appPkt = new RLLAppMsg(msgName);
		appPkt->setBitLength(PacketSize() * 8);
		appPkt->setSourceName(m_moduleName);
		appPkt->setDestName("Broadcast");
		appPkt->setCreationTime(simTime());
		appPkt->setBurstId(m_burstCounter);
		appPkt->setMessageId(m_messageCounter);

		send(appPkt->dup(), mLowerLayerOut);
		m_numberMessageSend++;
		counterTxMsg++;
		scheduleAt(simTime() + m_interArrivalTime, BurstMessageTimer);

		delete appPkt;
	}
	else
	{
		scheduleAt(simTime() + 10, StopTimer);
	}
}

void RLLApp::sendNextBurstMessage()
{
	if (m_numberMessageSend < m_numberMessageToSend)
	{
		char msgName[32];
		sprintf(msgName, "RLLAppMsg-%d-%d", m_burstCounter, m_messageCounter++);
		RLLAppMsg* appPkt = new RLLAppMsg(msgName);
		appPkt->setBitLength(PacketSize() * 8);
		appPkt->setSourceName(m_moduleName);
		appPkt->setDestName("Broadcast");
		appPkt->setCreationTime(simTime());
		appPkt->setBurstId(m_burstCounter);
		appPkt->setMessageId(m_messageCounter);
		send(appPkt->dup(), mLowerLayerOut);
		m_numberMessageSend++;
		counterTxMsg++;
		scheduleAt(simTime() + m_interArrivalTime, BurstMessageTimer);
		delete appPkt;
		appPkt = NULL;
	}
	else
	{
		m_numberMessageSend = 0;
		m_burstCounter++;
		m_messageCounter = 0;
		double wait = uniform(m_BurstPause, m_BurstPause*2);
		scheduleAt(simTime() + wait, BurstTimer);
	}
}

void RLLApp::endSim()
{
    simulation.callFinish();
    simulation.endRun();
}

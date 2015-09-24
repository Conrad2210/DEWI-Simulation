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

#ifndef RLLAPP_H_
#define RLLAPP_H_

#include <TrafGenPar.h>
#include "DataCenter.h"
#include "DataVector.h"

class RLLApp : public TrafGenPar
{
    public:
	RLLApp();
	virtual ~RLLApp();

	virtual void initialize(int);
	virtual void finish();

	virtual int numInitStages() const
	{
	    return 5;
	}

    protected:

	// OPERATIONS
	virtual void handleSelfMsg(cMessage*);
	virtual void handleLowerMsg(cMessage*);

	virtual void SendTraf(cPacket *msg, const char*);

	virtual void startBurst();
	virtual void sendNextBurstMessage();
	virtual void endSim();
	virtual void checkAssociation();

    protected:
	bool m_debug;        // debug switch
	int mLowerLayerIn;
	int mLowerLayerOut;
	DataCenter *dataCenter;
	DataVector *E2E;
	DataVector *receivedMSG;

	bool m_isLightSwitch;

	const char* m_moduleName;
	int m_numberMessageToSend;
	int m_numberMessageSend;
	int m_burstCounter;
	int m_messageCounter;
	int m_totalBurstToSend;

	double m_AppStartTime;
	double m_AppStopTime;
	double m_burstDuration;
	double m_interArrivalTime;
	double m_BurstPause;

	int counterRxMsg;
	int counterTxMsg;

	cMessage *BurstTimer;
	cMessage *BurstMessageTimer;
	cMessage *StopTimer;
	cMessage *AssTimer;
};

#endif /* RLLAPP_H_ */

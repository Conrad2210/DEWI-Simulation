/*
 * UDPBroadcastControl.h
 *
 *  Created on: 16 Apr 2014
 *      Author: R00118979
 */

#ifndef UDPBROADCASTCONTROL_H_
#define UDPBROADCASTCONTROL_H_

#include <vector>
#include <map>

#include "INETDefs.h"

#include "AppBase.h"

#include "UDPSocket.h"
#include "../akaroa/include/akaroa.H"


//UDP Application: UDPBroadcstControl; see NED for more info


class INET_API UDPBroadcastControl : public AppBase
{

    protected:
        enum SelfMsgKinds {START = 1, SEND, STOP};

        UDPSocket socket;
        int nLocalPort, nDestPort;
        std::vector<IPvXAddress> ipDestAddresses;
        IPvXAddress ipDestAddr;


        cOutVector *EndToEndDelay;
        cOutVector *hopCount;
        simtime_t timeForCount;
//        cOutVector *ReceivedMessages;
//        cOutVector *ResendMessages;
//        cOutVector *DoubleReceivedMessages;


        typedef std::map<int,int> SourceSequence;
        SourceSequence ssSourceSequence;
        simtime_t stDelayLimit;
        cMessage *mTimerNext;
        simtime_t stStartTime;
        simtime_t stStopTime;
        simtime_t stNextPkt;
        simtime_t stNextBurst;
        simtime_t stNextSleep;
        simtime_t endtoendDelay;

        bool bActiveBurst;
        bool bIsSource;
        bool bIsSink;
        bool bHaveSleepDuration;
        int nOutputInterface;
        std::vector<int> nOutputInterfaceMulticastBroadcast;

        static int nCounter; //counter for generating a global number for each packet

        int nNumSent;
        int nNumReceived;
        int nNumDoubleReceived;
        int nNumDeleted;
        int nNumDuplicated;
        int nNumResend;

        // volatile parameters:
        cPar *pMessageLengthPar;
        cPar *pBurstDurationPar;
        cPar *pSleepDurationPar;
        cPar *pSendIntervalPar;
        cPar *pReBroadcastDelayPar;

        //statistics
        static simsignal_t sstSentPkSignal;
        static simsignal_t sstRcvdPkSignal;
        static simsignal_t sstOutOfOrderPkSignal;
        static simsignal_t sstDublicatedPkSignal;
        static simsignal_t sstDropPkSignal;
        static simsignal_t sstReBroadcastPkSignal;

        IPvXAddress chooseDestAddr();
        virtual cPacket *createPacket();
        virtual cPacket *createBroadcastPacket();
        virtual cPacket *createRePacket(cPacket *pk);
        virtual void processPacket(cPacket *msg);
        virtual void generateBurst();

    protected:
        virtual int numInitStages() const {return 4;}
        virtual void initialize(int stage);
        virtual void handleMessageWhenUp(cMessage *msg);
        virtual void finish();
        virtual bool sendBroadcast(const IPvXAddress &dest, cPacket *pkt);
        virtual void sendPacket();

        virtual void processStart();
        virtual void processSend();
        virtual void processStop();


        //AppBase:
        virtual bool startApp(IDoneCallback *doneCallback);
        virtual bool stopApp(IDoneCallback *doneCallback);
        virtual bool crashApp(IDoneCallback *doneCallback);

    public:
        UDPBroadcastControl();
        ~UDPBroadcastControl();
};



#endif /* UDPBROADCASTCONTROL_H_ */

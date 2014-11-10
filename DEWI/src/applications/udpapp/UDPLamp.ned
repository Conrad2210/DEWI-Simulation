//
// Copyright (c) 2014 Conrad Dandelski
// Copyright (c) 2014 Nimbus Centre for Embedded Systems Research, Cork Institute of Technology, Ireland
//
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
//@author Conrad Dandelski

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
///////////////////////This App send broadcast messages/////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


//
//Sends UDP packets to Broadcast address, or acts as a sink which resend the Broadcast message
//Compatible with IPv4...


package dewi.applications.udpapp;
import dewi.applications.IUDPApp;

simple UDPLamp like IUDPApp
{
    parameters:
        int localPort = default(-1); //local UDP port number (if set to -1: use ephemeral port)
        int destPort = default(-1); //remote UDP port number
        string destAddresses = default("");
        
        double startTime @unit(s) = default(1s); //application start time
        double stopTime @unit(s) = default(7100s); //application stop time; a negative time means no limit
        volatile double sendInterval @unit(s); //time between to messages; usually random number e.g. 1s+uniform(-0.001s,0.001s);
        volatile double burstDuration @unit(s);
        volatile double reBroadcastDelay @unit(s) = default(0s);
        double delayLimit @unit(s) = default(0); //maximum accepted delay for a packet; packets with a bigger delay are dropped, zero value means no limit
        bool isSource; //Source can broadcast new Messages
        bool isSink; //Sink only rebroadcast the messages
        volatile int messageLength @unit(B); //length of messages to generate, in bytes
        
        string outputInterface = default(""); //interface in whoch the packets will be sent
        string outputInterfaceMulticastBroadcast = default(""); //interface in whoch the broadcast packets will be sent
        
        bool setBroadcast = default(true); //configure the socket for receive broadcast packets
        bool resendBroadcast = default(true);
        
        
        
        // Variables for mitigate Broadcast storms
        string Method @enum("NONE","PROBABILISTIC") = default("NONE"); 
        double PThreshold = default(0.1);
        
        @signal[sentPk](type=cPacket);
        @signal[rcvdPk](type=cPacket);
        @signal[dropPk](type=cPacket);
        @signal[outOfOrderPk](type=cPacket);
        @signal[ReBroadcastPk](type=cPacket);
        @signal[DublicatedPk](type=cPacket);
        @statistic[sentPk](title="packets sent"; source=sentPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[rcvdPk](title="packets received"; source=rcvdPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[ReBroadcastPk](title="packets resend"; source=ReBroadcastPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[DublicatedPk](title="packets received double"; source=DublicatedPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[dropPk](title="packets dropped"; source=dropPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[outOfOrderPk](title="packets received out of order"; source=outOfOrderPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[endToEndDelay](title="delay"; source="messageAge(rcvdPk)"; unit=s; record=histogram,vector; interpolationmode=none);
        @display("i=block/app");
        
        gates:
        input udpIn @labels(UDPControlInfo/up);
        output udpOut @labels(UDPControlInfo/down);
        
}

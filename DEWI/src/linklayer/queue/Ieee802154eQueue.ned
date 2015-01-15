//
// Copyright (C) 2005 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//


package dewi.linklayer.queue;

import dewi.linklayer.base.IOutputQueue;


//@author: Stefan Reis		2014	(based on src/linklayer/queue/DropTailQueue)
//@brief:  The mac queue based at the Data Convey Model (Chapter 2.3.) from the 6TiSCH Operation Sublayer (6top)
//         (draft-wang-6tisch-6top-00) from the IETF working group 6TSCH. https://datatracker.ietf.org/wg/6tisch/
// note:   The frame can requested at only the DestAddr, you get the oldest frame from the queue or Null back.
//         Or you can request a frame with the DestAddr and the msduHandle number, you get the specific frame back
//         or NULL. The priority can be handle over the unique msduHandle number of each frame. */
//
simple Ieee802154eQueue like IOutputQueue
{
    parameters:
        int frameCapacity = default(100);
        string queueName = default("l2queue"); // name of the inner cQueue object, used in the 'q' tag of the display string
        @display("i=block/queue");
        @signal[rcvdPk](type=cPacket);
        @signal[enqueuePk](type=cPacket);
        @signal[dequeuePk](type=cPacket);
        @signal[dropPkByQueue](type=cPacket);
        @signal[queueingTime](type=simtime_t; unit=s);
        @signal[queueLength](type=long);
        @statistic[rcvdPk](title="received packets"; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[dropPk](title="dropped packets"; source=dropPkByQueue; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[queueingTime](title="queueing time"; record=histogram,vector; interpolationmode=none);
        @statistic[queueLength](title="queue length"; record=max,timeavg,vector; interpolationmode=sample-hold);
    gates:
        input in;
        output out;
}


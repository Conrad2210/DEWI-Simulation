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

package dewi.linklayer.mac.CIDERMAC.CSMA;

simple CSMA
{
    parameters:
        @class(csma802154);
        bool debug = default(false);
        bool stats = default(true);
        bool trace = default(false);
        bool randomBackoff = default(false);

        string address = default("auto");
        string queueModule;				// name of optional external queue module
        bool useMACAcks = default(true);
        int queueLength = default(100);
        // simple interframe space. Time to wait between receiving a frame and acknowledging it.
        // Should be bigger than the maximum time for switching between Tx and Rx at the receiver.      
        double sifs @unit(s) = default(0.000192 s); // 12 symbols


        double macMaxCSMABackoffs = default(5);
        // maximum number of frame retransmission
        double macMaxFrameRetries = default(3);
        // time to wait for an acknowledgement after transmitting a unicast frame.
        double macAckWaitDuration @unit(s) = default(0.00056 s);  // 1+12+10+12 symbols
        // Clear Channel Assessment detection time		
        double ccaDetectionTime @unit(s) = default(0.000128 s); // 8 symbols
        // Time to setup radio to reception state
        double rxSetupTime @unit(s);
        // Time to switch radio from Rx to Tx state
        double aTurnaroundTime @unit(s) = default(0.000192 s);    // 12 symbols
        double aUnitBackoffPeriod @unit(s) = default(0.00032 s);
        // complete MAC ack message length (in bits) 
        // (! headerLength is not added to this)
        double ackLength @unit(bit) = default(40 bit);
        double headerLength @unit(bit) = default(216 bit);

        //backoff method to use. constant, linear or exponential
        string backoffMethod = default("exponential");
        // base unit for all backoff calculations
        // # of backoff periods of the initial contention window
        // (for linear and constant backoff method only)
        int contentionWindow = default(2);
        // minimum backoff exponent (for exponential backoff method only)
        double macMinBE = default(3);
        // maximum backoff exponent (for exponential backoff method only)
        double macMaxBE = default(8);
		// debug switch
    // whether is PAN coordinator


    gates:
        input upperDeciderIn; 			// from the upper layer
        output upperDeciderOut; 		// to the upper layer
        input lowerDeciderIn; 			// from the physical layer
        output lowerDeciderOut; 			// to the physical layer

        output queueOut;
        input queueIn;

}

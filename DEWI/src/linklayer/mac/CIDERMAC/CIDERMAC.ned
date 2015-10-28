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

package dewi.linklayer.mac.CIDERMAC;

simple CIDERMAC
{
    parameters:
        @class(CIDERMAC);
        bool debug = default(false);					// debug switch
        // whether is PAN coordinator
        bool isPANCoor = default(false);				// whether is PAN coordinator
        // beacon order [0, 15] - default 15 (no beacon) - Std 802.15.4-2011 (table 52) page 127 - see 5.1.1.1
        double BO = default(8);					// beacon order [0, 15]
        // superframe order [0, BO) - default 15 (no beacon) - Std 802.15.4-2011 (table 52) page 128 - see 5.1.1
        double SO = default(4);					// superframe order [0, BO)
        double dataTransMode = default(1);		// 1: direct; 2: indirect; 3: GTS
        // name of optional external queue module
        string queueModule;			// name of optional external queue module
        string panCoorName = default("host[0]");			// name of the PAN coordinator

        bool ack4Data = default(true);
        bool setRxOnWhenIdle = default(true);



        //TSCH VARIABLES
        bool TSCHcapable = default(true);

        bool useTSCH = default(false);

        // for the Std 802.15.4e-2014 TSCH mechanism - to set the Node as a timeScource
        bool timeSource = default(false);

        // for the Std 802.15.4e-2014 TSCH mechanism - to use or not the CCA during the TSCH CCA transmission
        bool useCCA = default(true);
        // for the Std 802.15.4e-2014 TSCH mechanism - to set the default timeslotID before the first beacon received
        int useTimeslotID = default(0);

        // for the Std 802.15.4e-2014 TSCH mechanism - to set the default hopping sequence ID before the first beacon received
        int useHoppingSequenceID = default(0);
        //
        int maxRetries = default(3); // default = 3; range = 0-7

        @display("i=block/layer");
    gates:
        input upperLayerIn; 		// from the upper layer
        output upperLayerOut; 		// to the upper layer
        input lowerLayerIn; 		// from the physical layer
        output lowerLayerOut; 		// to the physical layer
        input queueIn;				// from the MAC queue
        output queueOut;			// to the MAC queue

}

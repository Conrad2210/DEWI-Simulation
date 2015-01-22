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

package dewi.linklayer.mac.IEEE802154e;



simple Ieee802154eMac
{
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

    // switch for the network messages
    bool useIeee802Ctrl = default(false);

    // switch for the MAC Metrics - Std 802.15.4e-2012 (6.4.3.9 MAC-performance-metrics-specific MAC PIB attributes) page 182
    bool useMetrics = default(true);

    // switch for the Std 802.15.4e-2014 TSCH mechanism
    bool useTSCH = default(false);
    // use the TSCH without a beacon from the PAN coordinator (Note: devices will be not associated with coordinator)
        bool useTSCHdirect = default(true);
    // for the Std 802.15.4e-2014 TSCH mechanism - to set the Node as a timeScource
    bool timeSource = default(false);
    // for the Std 802.15.4e-2014 TSCH mechanism - to use or not the CCA during the TSCH CCA transmission
    bool useCCA = default(true);
    // for the Std 802.15.4e-2014 TSCH mechanism - to set the default timeslotID before the first beacon received
    int useTimeslotID = default(0);

    // for the Std 802.15.4e-2014 TSCH mechanism - to set the default hopping sequence ID before the first beacon received
    int useHoppingSequenceID = default(0);

    int maxRetries = default(3); // default = 3; range = 0-7

    //backoff method to use. constant, linear or exponential
    string backoffMethod = default("exponential");
    // base unit for all backoff calculations
    // # of backoff periods of the initial contention window
    // (for linear and constant backoff method only)
    int contentionWindow = default(2);
    // minimum backoff exponent (for exponential backoff method only) - [0, macMaxBE) - default 3 - Std 802.15.4-2011 (table 52) page 128
    double macMinBE = default(3);	// [0, macMaxBE) - default 3
    // maximum backoff exponent (for exponential backoff method only) - [3, 8] - default 5 - Std 802.15.4-2011 (table 52) page 127
    double macMaxBE = default(5);   // [3, 8] - default 5 

    string address = default("auto");

    bool readLinksFromFile = default(false);
    string LinkTableFileName = default("linkTable");
    string LinkTableFilesFolder = default("");
    bool readSlotframesFromFile = default(false);
    string SlotframeTableFileName = default("slotframeTable");
    string SlotframeTableFilesFolder = default("");
    bool readTemplatesFromFile = default(false);
    string TemplateTableFileName = default("templateTable");
    string TemplateTableFilesFolder = default("");
    string macSlotframeTableModule = default("^.^.slotframeTable");
    string macLinkTableModule = default("^.^.linkTable");
    string macTimeslotTableModule = default("^.^.timeslotTemplate");
    string macHoppingSequenceListModule = default("^.^.hoppingSequenceList");
    string macNeighborTableModule = default("^.^.neighborTable");
    int numHosts;


    @display("i=block/layer");
    gates:
        input upperLayerIn; 		// from the upper layer
        output upperLayerOut; 		// to the upper layer
        input lowerLayerIn; 		// from the physical layer
        output lowerLayerOut; 		// to the physical layer
        input queueIn;				// from the MAC queue
        output queueOut;			// to the MAC queue



}

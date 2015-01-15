
package dewi.linklayer.mac.IEEE802154;

simple Ieee802154Mac
{
    parameters:
        // debug switch
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
        // transmit or receive in GTS
        bool isRecvGTS = default(true);				// transmit or receive in GTS
        // payload of data frames transmitted in GTS, copied from traffic module
        double gtsPayload = default(10);			// payload of data frames transmitted in GTS, copied from traffic module
        // whether acking for Data pkts received in GTS
        bool ack4Gts = default(false);				// whether acking for Data pkts received in GTS
        bool ack4Data = default(false);
        
        //activate or deactivate beacon forwarding
        bool beaconForwarding = default(false);
        // switch for the network messages
        bool useIeee802Ctrl = default(false);				// whether acking for Data pkts received in GTS

        string address = default("auto");
        @display("i=block/layer");
    gates:
        input upperLayerIn; 			// from the upper layer
        output upperLayerOut; 		// to the upper layer
        input lowerLayerIn; 			// from the physical layer
        output lowerLayerOut; 			// to the physical layer
}

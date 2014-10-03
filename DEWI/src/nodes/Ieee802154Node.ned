package dewi.nodes;

import dewi.linklayer.Ieee802154Nic;
import dewi.networklayer.StarRouting.Ieee802154StarRouting;

//import inet.underTest.wpan.battery.Ieee802154Battery;
import dewi.battery.models.InetSimpleBattery;
import dewi.applications.Ieee802154TestApp.Ieee802154TestApp;
import dewi.mobility.IMobility;
import dewi.base.NotificationBoard;
import dewi.base.Display;

//
// This module implements simple ZigBee nodes with IEEE 802.15.4 MAC and PHY layer, plus a simple test app layer
//

module Ieee802154Node
{
    parameters:
        @node();
        string mobilityType;
        volatile double numHosts;
    gates:
        input radioIn;
    submodules:
        notificationBoard: NotificationBoard {
            parameters:
                @display("p=65,70;i=block/control");

        }
        mobility: <mobilityType> like IMobility {
            parameters:
                @display("p=65,150;i=block/cogwheel");

        }
        disp: Display {
            parameters:
                @display("p=65,230;i=block/app2");

        }
        // battery module
        battery: InetSimpleBattery{
            parameters:
                @display("p=65,310;i=block/plug");

        }
        // application-layer
        app: Ieee802154TestApp {
            parameters:
                @display("p=250,100;i=block/source");

        }
        // network layer
        net: Ieee802154StarRouting {
            parameters:
                @display("p=250,200;i=block/network2");

        }
        // network interface
        wlan: Ieee802154Nic {
            parameters:
                @display("p=250,300;q=queue;i=block/ifcard");

        }
    connections allowunconnected:
        radioIn --> wlan.radioIn;
        wlan.upperLayerOut --> net.lowerLayerIn;
        wlan.upperLayerIn <-- net.lowerLayerOut;
        net.upperLayerOut --> app.lowerLayerIn;
        net.upperLayerIn <-- app.lowerLayerOut;
}

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

package dewi.nodes.CIDERNode.FixedPos;

//Includes
//Mobility
import dewi.mobility.models.StationaryMobility;
import dewi.mobility.models.StationaryGrid;


//NIC
import dewi.nic.CIDERNic;

//Applications
//UDPApp
import dewi.applications.RLLApp.RLLApp;


//RoutingTable
import dewi.networklayer.CIDERNet.CIDER.CIDER;
import dewi.networklayer.CIDERNet.RLL.RLL;
import dewi.networklayer.CIDERNet.Net.SimpleNet;

//TransportLayer
//UDP
import dewi.networklayer.common.InterfaceTable;
import dewi.base.NotificationBoard;
//Routing
//MANETRouting
import dewi.networklayer.IManetRouting;

import dewi.linklayer.common.Ieee802154e.macSlotframeTable.macSlotframeTable;
import dewi.linklayer.common.Ieee802154e.macLinkTable.macLinkTable;
import dewi.linklayer.common.Ieee802154e.macTimeslotTable.macTimeslotTable;
import dewi.linklayer.common.Ieee802154e.macHoppingSequenceList.macHoppingSequenceList;
import dewi.linklayer.common.Ieee802154e.macNeighborTable.macNeighborTable;




module LightSwitch
{
    parameters:
        @node();
        //Variable X and Y for PlayGround Area

        int numHosts;
        //Number of Hosts, to calculate the position of the nodes
        //int Hosts;


        @display("bgb=473,513");
    gates:
        input radioIn @directIn;

    submodules:


        //Node Location
        mobility: StationaryMobility {
            parameters:
                @display("p=354,385;i=block/cogwheel");
        }



        //Notificationboard
        notificationBoard: NotificationBoard {
            parameters:
                @display("p=354,448;i=block/control");
        }

                //InterfaceTable
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=354,329;i=block/table");
        }

        //Routing
        app: RLLApp {
            @display("p=156,68;i=abstract/penguin");
        }

        RLL: RLL {

            @display("p=56,146;i=block/blackboard");
        }
        networklayer: SimpleNet {
            @display("p=157,201;i=block/routing");
        }
        CIDER: CIDER {
            @display("p=56,240;i=block/blackboard");
        }

        //InterfaceTable


        //NIC
        wlan: CIDERNic {
            parameters:
                numHosts = numHosts;
                @display("p=157,413;q=queue;i=device/accesspoint");
        }


        //Networklayer


        //Application Layer


        //Transportlayer UDP


        slotframeTable: macSlotframeTable {
            parameters:
                @display("p=354,156;i=block/table");
        }

        linkTable: macLinkTable {
            parameters:
                @display("p=354,100;i=block/table");
        }
        timeslotTemplate: macTimeslotTable {
            parameters:
                @display("p=354,43;i=block/table");
        }
        hoppingSequenceList: macHoppingSequenceList {
            parameters:
                @display("p=354,214;i=block/table");
        }
        neighborTable: macNeighborTable {
            parameters:
                @display("p=354,271;i=block/table");
        }




    connections allowunconnected:

        //Connections ApplicationLayer <--> TransportLayer
        app.lowerLayerIn <-- networklayer.upperLayerOut;
        app.lowerLayerOut --> networklayer.upperLayerIn;

        RLL.networkLayerIn <-- networklayer.RLLOut;
        RLL.networkLayerOut --> networklayer.RLLIn;

        CIDER.networkLayerIn <-- networklayer.CIDEROut;
        CIDER.networkLayerOut --> networklayer.CIDERIn;

        //Connections NetworkLayer <--> World (MAC/PHY)
        radioIn --> wlan.radioIn;
        wlan.upperLayerOut --> networklayer.lowerLayerIn;
        wlan.upperLayerIn <-- networklayer.lowerLayerOut;


}

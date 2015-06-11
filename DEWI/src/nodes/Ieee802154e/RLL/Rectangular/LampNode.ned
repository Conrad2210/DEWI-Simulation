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

package dewi.nodes.Ieee802154e.RLL.Rectangular;

//Includes
//Mobility
import dewi.mobility.models.StationaryMobility;
import dewi.mobility.models.StationaryGrid;

//NIC
import dewi.nic.RLLNic;

//Applications
//UDPApp
import dewi.applications.RLLApp.RLLApp;


//RoutingTable
import dewi.networklayer.RLL.RLL;
import dewi.networklayer.RLL.common.clusterTable.RLLClusterTable;
import dewi.networklayer.RLL.common.beaconTable.cBeaconTable;

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



module LampNode
{
    parameters:
        @node();
        //Variable X and Y for PlayGround Area
        double X @unit(m);
        double Y @unit(m);
        double Z @unit(m);
        int numHosts;
        int maxRow;
        int maxCol;
        //Number of Hosts, to calculate the position of the nodes
        //int Hosts;

        //No. of TCP Apps
        int numTcpApps = default(0);

        //No. of UDP Apps
        int numUdpApps = default(0);

        //UDP App Type
        string udpAppType = default("");

        //routingFile
        string routingFile = default("");

        //Routing Protocol
        string routingProtocol @enum("AODVUU","DYMOUM","DYMO","DSRUU","OLSR","OLSR_ETX","DSDV_2","Batman") = default("");  // used mobile routing protocol. see: inet.networklayer.manetrouting

    gates:
        input radioIn @directIn;

    submodules:


        //Node Location
        mobility: StationaryGrid {
            parameters:
                maxCol = maxCol;
                maxRow = maxRow;
                marginX = X;
                marginY = Y;
                marginZ = Z;
                @display("p=60,158;i=block/cogwheel");
        }

        //Notificationboard
        notificationBoard: NotificationBoard {
            parameters:
                @display("p=60,70;i=block/control");
        }

                //InterfaceTable
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=60,224;i=block/table");
        }

        clusterTable: RLLClusterTable {
            parameters:
                @display("p=60,325;i=block/table");
        }

        beaconTable: cBeaconTable {
            parameters:
                @display("p=360,325");
        }


        //Routing
        app: RLLApp {
            @display("p=156,68;i=abstract/penguin");
        }

        networklayer: RLL {

            @display("p=156,162;i=block/routing");
        }

        //InterfaceTable


        //NIC
        wlan: RLLNic {
            parameters:
                numHosts = numHosts;
                @display("p=156,266;q=queue;i=device/accesspoint");
        }



        //Networklayer


        //Application Layer


        //Transportlayer UDP


        slotframeTable: macSlotframeTable {
            parameters:
                @display("p=260,185;i=block/table");
        }

        linkTable: macLinkTable {
            parameters:
                @display("p=260,115;i=block/table");
        }
        timeslotTemplate: macTimeslotTable {
            parameters:
                @display("p=260,45;i=block/table");
        }
        hoppingSequenceList: macHoppingSequenceList {
            parameters:
                @display("p=260,255;i=block/table");
        }
        neighborTable: macNeighborTable {
            parameters:
                @display("p=260,325;i=block/table");
        }




    connections allowunconnected:

        //Connections ApplicationLayer <--> TransportLayer
        app.lowerLayerIn <-- networklayer.upperLayerOut;
        app.lowerLayerOut --> networklayer.upperLayerIn;

        //Connections NetworkLayer <--> World (MAC/PHY)
        radioIn --> wlan.radioIn;
        wlan.upperLayerOut --> networklayer.lowerLayerIn;
        wlan.upperLayerIn <-- networklayer.lowerLayerOut;


}

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

package dewi.nodes.Ieee802154e.Rectangular.RLL;

//Includes
//Mobility
import dewi.mobility.models.StationaryMobility;
import dewi.mobility.models.StationaryGrid;

//NetworkLayer
import dewi.nodes.inet.NetworkLayer;

//NIC
import dewi.nic.RLLNic;

//Applications
//UDPApp
import dewi.applications.IUDPApp;

//NotificationBoard
import dewi.base.NotificationBoard;

//InterfaceTable
import dewi.networklayer.common.InterfaceTable;

//RoutingTable
import dewi.networklayer.ipv4.RoutingTable;

//TransportLayer
//UDP
import dewi.transport.udp.UDP;

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
        mobility: StationaryMobility {
            parameters:
                @display("p=152,368;i=block/cogwheel");
        }


        //Routing
        manetrouting: <routingProtocol> like IManetRouting if routingProtocol != "" {
            @display("p=159,287");
        }

        //InterfaceTable
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=60,150;i=block/table");
        }

        //NIC
        wlan: RLLNic {
            parameters:
                numHosts = numHosts;
                @display("p=248,349;q=queue");
        }

        //Notificationboard
        notificationBoard: NotificationBoard {
            parameters:
                @display("p=60,70;i=block/control");
        }


        //RoutingTable
        routingTable: RoutingTable {
            parameters:
                IPForward = true;
                routerId = "";
                routingFile = routingFile;
                @display("p=60,230;i=block/table");
        }

        //Networklayer
        networkLayer: NetworkLayer {
            parameters:
                proxyARP = false;
                arp.globalARP = true;
                @display("p=248,247;i=block/fork;q=queue");
            gates:
                ifIn[1];
                ifOut[1];
        }

        //Application Layer
        udpApp[numUdpApps]: <udpAppType> like IUDPApp {
            parameters:
                @display("p=272,67;i=block/app");
        }

        //Transportlayer UDP
        udp: UDP {
            parameters:
                @display("p=272,154;i=block/transport");
        }
        
        slotframeTable: macSlotframeTable {
            parameters:
                @display("p=523,190;i=block/table");
        }
        
        linkTable: macLinkTable {
            parameters:
                @display("p=523,120;i=block/table");
        }
        timeslotTemplate: macTimeslotTable {
            parameters:
                @display("p=523,50;i=block/table");
        }
        hoppingSequenceList: macHoppingSequenceList {
            parameters:
                @display("p=523,260;i=block/table");
        }
        neighborTable: macNeighborTable {
            parameters:
                @display("p=523,330;i=block/table");
        }




    connections allowunconnected:

        //Connections ApplicationLayer <--> TransportLayer
        for i=0..numUdpApps-1 {
            udpApp[i].udpOut --> udp.appIn++;
            udpApp[i].udpIn <-- udp.appOut++;
        }

        //Connections TransportLayer <--> NetworkLayer
        udp.ipOut --> networkLayer.udpIn;
        udp.ipIn <-- networkLayer.udpOut;

        //Connections Routing <--> NetworkLayer
        networkLayer.manetOut --> manetrouting.from_ip if routingProtocol != "";
        networkLayer.manetIn <-- manetrouting.to_ip if routingProtocol != "";


        //Connections NetworkLayer <--> World (MAC/PHY)
        radioIn --> wlan.radioIn;
        wlan.upperLayerOut --> networkLayer.ifIn[0];
        wlan.upperLayerIn <-- networkLayer.ifOut[0];


}

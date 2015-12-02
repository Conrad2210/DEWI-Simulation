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

package dewi.simulations.UseCase1_Lighting.Dimming.nonBeaconCSMA;

import dewi.applications.IUDPApp;
import dewi.base.NotificationBoard;
import dewi.mobility.models.StationaryGrid;
import dewi.networklayer.IManetRouting;
import dewi.networklayer.common.InterfaceTable;
import dewi.networklayer.ipv4.RoutingTable;
import dewi.nodes.inet.NetworkLayer;
import dewi.transport.udp.UDP;


module Node
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
                @display("p=152,368;i=block/cogwheel");
        }
                //Notificationboard
        notificationBoard: NotificationBoard {
            parameters:
                @display("p=60,70;i=block/control");
        }

                //InterfaceTable
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=60,150;i=block/table");
        }
        //Routing
        manetrouting: <routingProtocol> like IManetRouting if routingProtocol != "" {
            @display("p=159,287");
        }



        //NIC
//        wlan: CIDERNic {
//            parameters:
//                @display("p=248,349;q=queue");
//        }




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

        //Disp
//        disp: Display {
//            parameters:
//                @display("p=60,326;i=block/app2");
//        }
//
//                // battery module
//        battery: InetSimpleBattery {
//            parameters:
//                @display("p=60,407;i=block/plug");
//
//        }


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
//        radioIn --> wlan.radioIn;
//        wlan.upperLayerOut --> networkLayer.ifIn[0];
//        wlan.upperLayerIn <-- networkLayer.ifOut[0];


}

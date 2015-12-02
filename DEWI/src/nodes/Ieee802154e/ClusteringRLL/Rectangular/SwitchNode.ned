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

package dewi.nodes.Ieee802154e.ClusteringRLL.Rectangular;

//Includes
//Mobility
import dewi.mobility.models.StationaryMobility;
import dewi.mobility.models.StationaryGrid;


//NIC
import dewi.nic.RLLNic;

//Applications
//UDPApp
import dewi.applications.RLLApp.ClusteringRLL;

import dewi.networklayer.RLLComplete.RLLComplete;
import dewi.networklayer.RLLComplete.common.clusterTable.RLLClusterTable;
import dewi.networklayer.RLLComplete.common.beaconTable.cBeaconTable;


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



    gates:
        input radioIn @directIn;

    submodules:


        //Node Location
        mobility: StationaryMobility {
            parameters:
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
                @display("p=60,240;i=block/table");
        }

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
        app: ClusteringRLL {
            @display("p=156,68;i=abstract/penguin");
        }

        networklayer: RLLComplete {

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

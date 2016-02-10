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

package dewi.networklayer.CIDERNet.CIDER;



simple CIDER
{
    string macSlotframeTableModule = default("^.slotframeTable");
    string macLinkTableModule = default("^.linkTable");
    string macTimeslotTableModule = default("^.timeslotTemplate");
    string macHoppingSequenceListModule = default("^.hoppingSequenceList");
    string macNeighborTableModule = default("^.neighborTable");
    string RLLClusterTableModule = default("^.clusterTable");
    string cBeaconTableModule = default("^.beaconTable");
    double w1 = default(0.2); //Node degree
    double w2 = default(0.3); //Cluster degree
    double w3 = default(0.4); //LP degree
    double w4 = default(0.1); //RSSI
    double w5 = default(0.8);
    double w6 = default(0.2);
    bool LPDevice = default(false);
    double transmitterPower @unit(mW);
    gates:

        input networkLayerIn;
        output networkLayerOut;


}

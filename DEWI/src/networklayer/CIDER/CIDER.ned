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

package dewi.networklayer.CIDER;



simple CIDER
{
    string macSlotframeTableModule = default("^.slotframeTable");
    string macLinkTableModule = default("^.linkTable");
    string macTimeslotTableModule = default("^.timeslotTemplate");
    string macHoppingSequenceListModule = default("^.hoppingSequenceList");
    string macNeighborTableModule = default("^.neighborTable");
    string RLLClusterTableModule = default("^.clusterTable");
    string cBeaconTableModule = default("^.beaconTable");
      gates:

        input lowerLayerIn;
        input upperLayerIn;
        output lowerLayerOut;
        output upperLayerOut;


}
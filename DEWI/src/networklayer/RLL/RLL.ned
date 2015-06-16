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

package dewi.networklayer.RLL;



simple RLL
{
    string macSlotframeTableModule = default("^.slotframeTable");
    string macLinkTableModule = default("^.linkTable");
    string macTimeslotTableModule = default("^.timeslotTemplate");
    string macHoppingSequenceListModule = default("^.hoppingSequenceList");
    string macNeighborTableModule = default("^.neighborTable");
    string RLLClusterTableModule = default("^.clusterTable");
    string cBeaconTableModule = default("^.beaconTable");
    
	bool isPANCoor = default(false);
	bool capablePanCoor = default(true);
	double StartTime @unit(s) = default(0s);
	double AssociateWaitTime @unit(s) = default(0.5s);
	
	int Distance = default(20);
    
    gates:

        input lowerLayerIn;
        input upperLayerIn;
        output lowerLayerOut;
        output upperLayerOut;


}

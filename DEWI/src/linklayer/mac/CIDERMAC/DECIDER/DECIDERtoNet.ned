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

package dewi.linklayer.mac.CIDERMAC.DECIDER;

simple DECIDERtoNet
{
    
        gates:
        input upperLayerIn; 			// from the upper layer
        output upperLayerOut; 		// to the upper layer
        input CSMAIn; 			// from the physical layer
        output CSMAOut; 			// to the physical layer
        input CSMAQueueIn;
        output CSMAQueueOut;
        input TSCHIn; 			// from the physical layer
        output TSCHOut; 			// to the physical layer
        input TSCHQueueIn;
        output TSCHQueueOut;
        
        output queueOut;
        input queueIn;
}
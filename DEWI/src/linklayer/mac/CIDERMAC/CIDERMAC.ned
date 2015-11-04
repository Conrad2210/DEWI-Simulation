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

package dewi.linklayer.mac.CIDERMAC;

import dewi.linklayer.mac.CIDERMAC.CSMA.CSMA;
import dewi.linklayer.mac.CIDERMAC.DECIDER.DECIDERtoNet;
import dewi.linklayer.mac.CIDERMAC.DECIDER.DECIDERtoPHY;
import dewi.linklayer.mac.CIDERMAC.TSCH.TSCH;


module CIDERMAC
{
    @display("bgb=681,462");
    gates:
        input upperLayerIn;
        output upperLayerOut;
        input lowerLayerIn;
        output lowerLayerOut;
        input queueIn;
        output queueOut;

    submodules:
        TSCH: TSCH {
            parameters:
                @display("p=277,195;i=block/cogwheel;q=IFQueue");
        }

        CSMA: CSMA {
            parameters:
                @display("p=355,195;i=block/cogwheel;q=IFQueue");
        }

        DECIDERtoNet: DECIDERtoNet {
            parameters:
                @display("p=316,111;i=block/fork;q=IFQueue");
        }

        DECIDERtoPHY: DECIDERtoPHY {
            parameters:
                @display("p=316,288;i=block/dispatch;q=IFQueue");
        }

    connections:
        TSCH.lowerDeciderIn <-- DECIDERtoPHY.TSCHOut;
        TSCH.lowerDeciderOut --> DECIDERtoPHY.TSCHIn;
        TSCH.upperDeciderIn <-- DECIDERtoNet.TSCHOut;
        TSCH.upperDeciderOut --> DECIDERtoNet.TSCHIn;

        upperLayerIn --> DECIDERtoNet.upperLayerIn;
        upperLayerOut <-- DECIDERtoNet.upperLayerOut;
        lowerLayerIn --> DECIDERtoPHY.lowerLayerIn;
        lowerLayerOut <-- DECIDERtoPHY.lowerLayerOut;

        queueIn --> DECIDERtoNet.queueIn;
        queueOut <-- DECIDERtoNet.queueOut;
        
        TSCH.queueIn <-- DECIDERtoNet.TSCHQueueOut;
        TSCH.queueOut --> DECIDERtoNet.TSCHQueueIn;
        CSMA.queueIn <-- DECIDERtoNet.CSMAQueueOut;
        CSMA.queueOut --> DECIDERtoNet.CSMAQueueIn;


        CSMA.lowerDeciderIn <-- DECIDERtoPHY.CSMAOut;
        CSMA.lowerDeciderOut --> DECIDERtoPHY.CSMAIn;
        CSMA.upperDeciderIn <-- DECIDERtoNet.CSMAOut;
        CSMA.upperDeciderOut --> DECIDERtoNet.CSMAIn;

}


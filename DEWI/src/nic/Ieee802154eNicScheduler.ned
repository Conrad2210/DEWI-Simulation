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

package dewi.nic;
import dewi.linklayer.base.IOutputQueue;
import dewi.linklayer.mac.IEEE802154e.Scheduler.Ieee802154eMacScheduler;
import dewi.phyLayer.IEEE802154e.Ieee802154ePhy;
import dewi.linklayer.common.Ieee802154e.macLinkTable.macLinkTable;
import dewi.linklayer.common.Ieee802154e.macSlotframeTable.macSlotframeTable;
import dewi.linklayer.common.Ieee802154e.macTimeslotTable.macTimeslotTable;
import dewi.scheduler.IScheduler;



module Ieee802154eNicScheduler
{
parameters:
        // name of the interface queue type
        string ifqType; // name of the interface queue type
        string scheduleTyp;
        int numHosts;
    gates:
        input upperLayerIn; // to upper layers
        output upperLayerOut; // from upper layers
        input radioIn; // to receive AirFrames

    submodules:
        ifq: <ifqType> like IOutputQueue {
            parameters:
                @display("p=70,80;i=block/passiveq;q=IFQueue");
        }
        mac: Ieee802154eMacScheduler {
            parameters:
                numHosts = numHosts;
                queueModule = "ifq";
                @display("p=70,180;i=block/layer");
        }
        phy: Ieee802154ePhy {
            parameters:
                @display("p=70,280;i=block/wrxtx");
        }

        scheduler: <scheduleTyp> like IScheduler {
            parameters:
                @display("p=180,180;i=block/segm;q=IFQueue");
        }

    connections:
        radioIn --> phy.radioIn;
        phy.upperLayerOut --> { @display("m=m,50,50,75,0"); } --> mac.lowerLayerIn;
        phy.upperLayerIn <-- { @display("m=m,25,0,25,0"); } <-- mac.lowerLayerOut;
        mac.upperLayerOut --> upperLayerOut;
        mac.upperLayerIn <-- upperLayerIn;
        ifq.in <-- mac.queueOut;	// to handle the IEEE 802.15.4e-2012 mgs's for the links
        ifq.out --> mac.queueIn; 	// to handle the IEEE 802.15.4e-2012 mgs's for the links        
        scheduler.from_mac <-- mac.schedulerOut;
        scheduler.to_mac --> mac.schedulerIn;
}
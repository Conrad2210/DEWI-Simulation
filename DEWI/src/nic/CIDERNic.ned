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
import dewi.linklayer.mac.CIDERMAC.CIDERMAC;
import dewi.phyLayer.IEEE802154e.Ieee802154ePhy;

module CIDERNic
{
    parameters:
        // name of the interface queue type
        string ifqType; // name of the interface queue type
        int numHosts;
        int GateWay = intuniform(0, numHosts);
    gates:
        input upperLayerIn; // to upper layers
        output upperLayerOut; // from upper layers
        input radioIn; // to receive AirFrames

    submodules:
        mac: CIDERMAC {
            parameters:
                GateWay = GateWay;
                @display("p=70,180;i=block/layer");
        }
        phy: Ieee802154ePhy {
            parameters:
                @display("p=70,280;i=block/wrxtx");
        }

    connections:

        radioIn --> phy.radioIn;
        phy.upperLayerOut --> { @display("m=m,50,50,75,0"); } --> mac.lowerLayerIn;
        phy.upperLayerIn <-- { @display("m=m,25,0,25,0"); } <-- mac.lowerLayerOut;
        mac.upperLayerOut --> upperLayerOut;
        mac.upperLayerIn <-- upperLayerIn;
}

//
// Copyright (C) 2014 Conrad Dandelski

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
// 

package dewi.simulations.Testing;

import dewi.networklayer.autorouting.ipv4.IPv4NetworkConfigurator;
import dewi.world.radio.ChannelControl;
import dewi.nodes.CIDERNode.Rectangular.LampNode;
import dewi.nodes.CIDERNode.Rectangular.LightSwitch;
import dewi.world.datacenter.DataCenter;


network Network
{
    parameters:
        int numHosts;
        double AreaX @unit(m) = default(80m);
        double AreaY @unit(m) = default(40m);

		int GateWay = intuniform(0, numHosts);
        double nodeSeparationX = default(0);
        double nodeSeparationY = default(0);
        int maxRow = CalcMaxRow(numHosts);
        int maxCol = CalcMaxCol(numHosts);
        //@display("i=background/green;is=vl;bgb=80,40;bgs=25,m;bgi=background/green,s;bgg=1,,black");
        @display("bgb=80,40;bgs=25,m");
    submodules:
        lightSwitch: LightSwitch {
            parameters:
                @display("i=lighting/lightswitch;p=15.28,12.16");
        }
        lamp[numHosts]: LampNode {
            parameters:
                numHosts = numHosts;
                GateWay = GateWay;
                maxRow = maxRow;
                maxCol = maxCol;
                //Hosts = numFixHosts;

                @display("b=1.5,1.5,oval,red;;i=status/bulb;p=3.68,10.84");
        }

        channelControl: ChannelControl {
            parameters:
                @display("p=0.68,0.56;i=misc/sun");
        }
        DataCenter: DataCenter {

            @display("p=5.52,0.6");
        }
        configurator: IPv4NetworkConfigurator {
            config = xml("<config><interface hosts='*' address='10.x.x.x' netmask='255.x.x.x'/></config>");
            //            netmask="255.255.0.0";
            //            networkAddress="145.236.0.0";
            @display("p=3.16,0.6");
        }
  
}


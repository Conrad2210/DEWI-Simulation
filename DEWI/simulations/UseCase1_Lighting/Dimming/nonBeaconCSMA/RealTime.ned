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

package dewi.simulations.UseCase1_Lighting.Dimming.nonBeaconCSMA;

import dewi.networklayer.autorouting.ipv4.IPv4NetworkConfigurator;
import dewi.world.radio.ChannelControl;
import dewi.simulations.UseCase1_Lighting.Dimming.nonBeaconCSMA.Node;
import dewi.simulations.UseCase1_Lighting.Dimming.nonBeaconCSMA.LightSwitch;


network RealTimeNetworknonBeaconCSMA
{
    parameters:
        int numHosts;
        double AreaX @unit(m) = default(80m);
        double AreaY @unit(m) = default(40m);
        double X @unit(m);
        double Y @unit(m);
        double Z @unit(m);

        double nodeSeparationX = default(0);
        double nodeSeparationY = default(0);
        int maxRow = CalcMaxRow(numHosts);
        int maxCol = CalcMaxCol(numHosts);
        //@display("i=background/green;is=vl;bgb=80,40;bgs=25,m;bgi=background/green,s;bgg=1,,black");
        @display("bgb=80,40;bgs=25,m");
    submodules:
        lightSwitch: LightSwitch {
            parameters:
                X = X;
                Y = Y;
                Z = Z;
                @display("b=0.1,0.1,rect;i=lighting/lightswitch;p=10,10");
        }
        lamps[numHosts]: Node {
            parameters:
                numHosts = numHosts;
                maxRow = maxRow;
                maxCol = maxCol;
//                X = AreaX;
//                Y = AreaY;
//				
//                Hosts = numFixHosts;

                @display("b=1,1,oval;i=status/bulb;p=8.44,0.92");
        }

        channelControl: ChannelControl {
            parameters:
                @display("p=0.68,0.56;i=misc/sun");
        }

        configurator: IPv4NetworkConfigurator {
            config = xml("<config><interface hosts='*' address='10.x.x.x' netmask='255.x.x.x'/></config>");
            //            netmask="255.255.0.0";
            //            networkAddress="145.236.0.0";
            @display("p=2.04,0.4");
        }
}


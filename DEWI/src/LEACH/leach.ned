
package dewi.LEACH;

import dewi.LEACH.Node.Node;
import dewi.LEACH.Node.BS;

module leach
{
    parameters:
        volatile double numNodes;
        volatile double trRange;
        volatile double rounds;
        volatile double frames;
        volatile double sRand;
        volatile double xMax;
        volatile double yMax;
    submodules:
        bs: BS {
            parameters:
                id = 1;
                randInit = sRand;
                xpos = xMax/2;
                numNodes = numNodes;
                ypos = 1750;
        }
        node[numNodes]: Node {
            parameters:
                energy = 10000;
                id = index+2;
                xpos = intuniform(0, xMax);
                ypos = intuniform(0, yMax);
        }
    connections allowunconnected:
}

//
// Instantiates the network
//
network leach1 extends leach
{
    parameters:
}

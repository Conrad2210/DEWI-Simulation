/*
 * densesensornetwork.cc
 *
 *  Created on: Mar 20, 2014
 *      Author: user
 */

#ifndef NEDFUNCTIONS_CC_
#define NEDFUNCTIONS_CC_

#include "NEDFunctions.h"
static cNEDValue calcPositionY(cComponent *context, cNEDValue argv[], int argc)
{

   int nHost = argv[0];
   double fAreaX = argv[1];
   double fAreaY = argv[2];
   fAreaX = fAreaX - 1.0;

   cNEDValue Value(nHost * fAreaX / fAreaY);
    return Value;
}

Define_NED_Function(calcPositionY,"double PositionY(int nIndexHost, quantity fAreaX, quantity fAreaY)");

cNEDValue nedf_moduleListByPath(cComponent *context, cNEDValue argv[], int argc)
{
    std::string modulenames;
    cTopology topo;
    std::vector<std::string> paths;
    for (int i = 0; i < argc; i++)
        paths.push_back(argv[i].stdstringValue());

    topo.extractByModulePath(paths);

    for (int i = 0; i < topo.getNumNodes(); i++)
    {
        std::string path = topo.getNode(i)->getModule()->getFullPath();
        if (modulenames.length() > 0)
            modulenames = modulenames + " " + path;
        else
            modulenames = path;
    }
    return modulenames;
}

Define_NED_Function2(nedf_moduleListByPath,
        "string moduleListByPath(string modulePath,...)",
        "string",
        "Returns a space-separated list of the modules at the given path(s). "
        "See cTopology::extractByModulePath()."
);

cNEDValue nedf_CalcMaxRow(cComponent *context, cNEDValue argv[], int argc)
{//Parameter[0] = numHosts

    int nHost = argv[0];
    int size = (int)ceil(sqrt((double)nHost));
    int maxRow = 0;

    for(int i = 0; i < nHost; i++)
        {
            if(((int)floor((double)i/(double)size))>maxRow)
                maxRow = ((int)floor((double)i/(double)size));
        }
    cNEDValue Value((long)maxRow);
        return Value;
}

Define_NED_Function(nedf_CalcMaxRow,"int CalcMaxRow(int numHosts)");

cNEDValue nedf_CalcMaxCol(cComponent *context, cNEDValue argv[], int argc)
{//Parameter[0] = numHosts

    int nHost = argv[0];
    int size = (int)ceil(sqrt((double)nHost));
    int maxCol = 0;

    for(int i = 0; i < nHost; i++)
        {
        if((i % size)>maxCol)
                    maxCol = i % size;
        }
    cNEDValue Value((long)maxCol);
        return Value;
}
Define_NED_Function(nedf_CalcMaxCol,"int CalcMaxCol(int numHosts)");

#endif /* NEDFUNCTIONS_CC_ */

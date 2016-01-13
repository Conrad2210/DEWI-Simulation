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

#ifndef CIDER_H_
#define CIDER_H_

#include <csimplemodule.h>
#include "InterfaceEntry.h"
#include "macNeighborTable.h"
#include "macNeighborTableAccess.h"
#include "vector"

typedef std::vector<MACAddress> macVector;


class CIDER: public cSimpleModule {
public:
    CIDER();
    ~CIDER();
	virtual int numInitStages() const
	{
	    return 3;
	}
	virtual void initialize(int);
	virtual void finish();
protected:

	virtual void handleMessage(cMessage *msg);
	virtual void handleSelfMessage(cMessage *msg);
	virtual void handleCIDERMessage(cMessage *msg);
	virtual void calcWeight();
    int networkLayerIn;
    int networkLayerOut;
    int counterPing;
    double dOwnWeight;
    int nNodeDegree;
    int nClusterDegree;
    double dMeanRSSI;
    double w1;
    double w2;
    double w3;
protected:
    InterfaceEntry *myInterface;
    IMacNeighborTable *neighbourTable;

    cMessage *timerInitialPing;
    cMessage *timerNeighUpdate;
    cMessage *timerWeight;
    cMessage *timerFistCH;
    macVector myMACList;
    enum CIDERFrames{
    	CIDERPingTimer = 500,
    	CIDERNeighUpdateTimer = 501,
    	CIDERPing = 502,
    	CIDERNeighUpdate = 503,
    	CIDERCntrlInfo = 504,
    	CIDERWeightMessage = 505,
    	CIDERFirstCH = 506,
    	CIDERFirstCHTimer = 507,
    	CIDERWeightTimer = 508
    };

private:
	double mWTodBm(double mW)
	{
	    return 10 * log10(mW);
	}

	double dBmTomW(double dBm)
	{
	    return pow(10, dBm / 10);
	}
	int compareMACList(macVector vec1, macVector vec2, MACAddress sender);
};

#endif /* CIDER_H_ */

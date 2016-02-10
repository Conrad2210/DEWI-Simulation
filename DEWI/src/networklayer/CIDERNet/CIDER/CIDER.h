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

typedef std::vector<int> macVector;

class CIDER : public cSimpleModule
{
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
        virtual void updatedisplay();

        virtual void CIDERPingMessage(cMessage *msg);
        virtual void handle_CIDERPingMessage(cMessage *msg);

        virtual void CIDERNeighbourUpdate(cMessage *msg);
        virtual void handle_CIDERNeighbourUpdate(cMessage *msg);

        virtual void CIDERWeightUpdate(cMessage *msg);
        virtual void handle_CIDERWeightUpdate(cMessage *msg);

        virtual void CIDERCompWeight(cMessage *msg);
        virtual void handle_CIDERCompWeight(cMessage *msg);

        virtual void CIDERCHCompetition(cMessage *msg);
        virtual void handle_CIDERCHCompetition(cMessage *msg);

        virtual void CIDERClusterAdvert(cMessage *msg);
        virtual void handle_CIDERClusterAdvert(cMessage *msg);

        virtual void CIDERCHElection(cMessage *msg);
        virtual void handle_CIDERCHElection(cMessage *msg);

        virtual void CIDERParentUpdater(cMessage *msg);
        virtual void handle_CIDERParentUpdater(cMessage *msg);

        virtual void CIDERCoverageUpdater(cMessage *msg);
        virtual void handle_CIDERCoverageUpdater(cMessage *msg);

        virtual void CIDERCHDelection(cMessage *msg);
        virtual void handle_CIDERCHDelection(cMessage *msg);

        virtual void CIDERCSDelection(cMessage *msg);
        virtual void handle_CIDERCSDelection(cMessage *msg);

        virtual void CIDERDelectionUpdate(cMessage *msg);
        virtual void handle_CIDERDelectionUpdate(cMessage *msg);

        bool LPDevice;
        int networkLayerIn;
        int networkLayerOut;
        int counterPing;
        double dOwnWeight;
        int nNodeDegree;
        int nClusterDegree;
        int nLPDegree;
        double dNormRSSI;
        double w1; //weight node degree
        double w2; //weight cluster degree
        double w3; //weight mean rssi
        double w4; //weight new coverage
        double w5; //weight rssi


    protected:
        enum CIDERMode
        {
            CH, CS, LP, undefined
        };

        CIDERMode NodeOperationMode;

        InterfaceEntry *myInterface;
        IMacNeighborTable *neighbourTable;

        cMessage *timerInitialPing;
        cMessage *timerNeighUpdate;
        cMessage *timerWeight;
        cMessage *timerCompWeight;
        cMessage *timerCompetition;
        cMessage *timerAdvert;
        cMessage *timerElectChildCH;
        cMessage *timerUpdateParent;
        cMessage *timerCoverageUpdate;
        cMessage *timerDelectCH;
        cMessage *timerDelectCHRep;
        cMessage *timerDelectCS;

        macNeighborTableEntry *parent;
        macVector myMACList;
        macVector assignedCS;
        MACAddress delectAddr;

        enum CIDERFrames
        {
            CIDERPingTimer = 500,
            CIDERNeighUpdateTimer = 501,
            CIDERPing = 502,
            CIDERNeighUpdate = 503,
            CIDERCntrlInfo = 504,
            CIDERWeightTimer = 505,
            CIDERWeightMessage = 506,
            CIDERCompareWeightTimer = 507,
            CIDERCHAdvertTimer = 508,
            CIDERFirstCHCompetitionTimer = 509,
            CIDERFirstCHCompetition = 509,
            CIDERCHAdvert = 510,
            CIDERElectChildCHTimer = 511,
            CIDERElectChildCH = 512,
            CIDERParentUpdateTimer = 513,
            CIDERParentUpdate = 514,
            CIDERCoverageUpdateTimer = 515,
            CIDERCoverageUpdate = 516,
            CIDERDelectCHTimer = 517,
            CIDERDelectCH = 518,
            CIDERDelectCHRepTimer = 519,
            CIDERDelectCHRep = 520,
            CIDERDelectCSTimer = 521,
            CIDERDelectCS = 522,
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

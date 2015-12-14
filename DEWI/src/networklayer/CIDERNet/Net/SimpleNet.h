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

#ifndef SIMPLENET_H_
#define SIMPLENET_H_
#include "INETDefs.h"

#include <csimplemodule.h>

class INET_API SimpleNet: public cSimpleModule {
public:
    SimpleNet();
    virtual ~SimpleNet();
    virtual int numInitStages() const
        {
            return 2;
        }


    virtual void initialize(int stage);
    virtual void finish();

    virtual void handleMessage(cMessage *msg);
    virtual void handleUpperLayer(cMessage *msg);
    virtual void handleLowerLayer(cMessage *msg);
    virtual void handleDataMessage(cMessage *msg);
    virtual void handleRLLMessage(cMessage *msg);
    virtual void handleCIDERMessge(cMessage *msg);


protected:
    int mRLLIn;
    int mRLLOut;
    int mCIDERIn;
    int mCIDEROut;
    int mLowerLayerIn;
    int mLowerLayerOut;
    int mUpperLayerIn;
    int mUpperLayerOut;

};

#endif /* SIMPLENET_H_ */

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

#ifndef DECIDERTONET_H_
#define DECIDERTONET_H_

#include <csimplemodule.h>
#include "INETDefs.h"

class INET_API DECIDERtoNet: public cSimpleModule
{
	public:
		DECIDERtoNet();
		virtual ~DECIDERtoNet();

		virtual int numInitStages ()const{return 2;}

		virtual void initialize(int stage);

		virtual void finish();

		virtual void handleMessage(cMessage *msg);

		virtual void handleLowerMsg(cMessage *msg);

		virtual void handleUpperMsg(cMessage *msg);

		bool isTschActive() const
		{
			return mTSCHActive;
		}

		void setTschActive(bool tschActive)
		{
			mTSCHActive = tschActive;
		}

	protected:

	    int mUpperLayerIn; 			// from the upper layer
	    int mUpperLayerOut; 		// to the upper layer
	    int mCSMAIn; 			// from the physical layer
	    int mCSMAOut; 			// to the physical layer
	    int mCSMAQueueIn;
	    int mCSMAQueueOut;
	    int mTSCHIn; 			// from the physical layer
	    int mTSCHOut; 			// to the physical layer
	    int mTSCHQueueIn;
	    int mTSCHQueueOut;

	    int mQueueOut;
	    int mQueueIn;


	    bool mTSCHActive;
};

#endif /* DECIDERTONET_H_ */

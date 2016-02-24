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

#ifndef DECIDERTOPHY_H_
#define DECIDERTOPHY_H_

#include <csimplemodule.h>
#include "INETDefs.h"

class INET_API DECIDERtoPHY: public cSimpleModule
{
	public:
		DECIDERtoPHY();
		virtual ~DECIDERtoPHY();
		virtual int numInitStages() const
		{
			return 2;
		}

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
		int mLowerLayerIn; 			// from the upper layer
		int mLowerLayerOut; 		// to the upper layer
		int mCSMAIn; 			// from the physical layer
		int mCSMAOut; 			// to the physical layer
		int mTSCHIn; 			// from the physical layer
		int mTSCHOut; 			// to the physical layer

		bool mTSCHActive;
};

#endif /* DECIDERTOPHY_H_ */

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

#ifndef CSMA_H_
#define CSMA_H_

#include <INETDefs.h>
#include <WirelessMacBase.h>
#include "InterfaceEntry.h"
class INET_API CSMA: public WirelessMacBase
{
	public:
		CSMA();
		virtual ~CSMA();

		virtual int numInitStages() const
		{
			return 3;
		}
		virtual void initialize(int stage);

		int mUpperLayerIn;
		int mUpperLayerOut;
		int mLowerLayerIn;
		int mLowerLayerOut;

	protected:
		void handleMessage(cPacket *msg);
		/** @brief Handle self messages such as timers */
		virtual void handleSelfMsg(cMessage *msg);

		/** @brief Handle packets from upper layer */
		virtual void handleUpperMsg(cPacket *msg);

		/** @brief Handle commands from upper layer */
		virtual void handleCommand(cMessage *msg);

		/** @brief Handle packets from lower layer */
		virtual void handleLowerMsg(cPacket *msg);
		virtual InterfaceEntry *createInterfaceEntry();
		/**
		 * should clear queue and emit signal "dropPkFromHLIfaceDown" with entire packets
		 */
		virtual void flushQueue();

		/**
		 * should clear queue silently
		 */
		virtual void clearQueue();
};

#endif /* CSMA_H_ */

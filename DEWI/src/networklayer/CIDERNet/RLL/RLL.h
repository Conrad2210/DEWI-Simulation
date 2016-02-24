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

#ifndef RLL_H_
#define RLL_H_
#include "IRLLClusterTable.h"
#include "IMacLinkTable.h"
#include "IMacSlotframeTable.h"
#include "IMacTimeslotTable.h"
#include "IMacHoppingSequenceList.h"
#include "IMacNeighborTable.h"
#include <csimplemodule.h>
#include "INETDefs.h"
#include <omnetpp.h>

class INET_API RLL: public cSimpleModule {
public:
    RLL();
    virtual ~RLL();


    virtual int numInitStages() const
        {
            return 2;
        }


    virtual void initialize(int stage);
    virtual void finish();

protected:
virtual void handleMessage(cMessage *msg);

virtual void handleDataMessage(cPacket *msg);

//retrieve schedule
virtual void SCHEDULE_request(cMessage *msg);

virtual void handle_SCHEDULE_indication(cMessage *msg);

virtual void SCHEDULE_response(cMessage *msg);

virtual void handle_SCHEDULE_confirm(cMessage *msg);

private:
////////////////////////////////////////////////////////////////////////
//          Helper Functions
////////////////////////////////////////////////////////////////////////
void createInitialEntries();

void setSchedule();
void setScheduleChStUn();
void setScheduleChStZe();
void setScheduleChInit();
void setScheduleCs();

protected:
    int mNetworkLayerIn;
    int mNetworkLayerOut;

    channelList myChannel;
    channelList chChannel;

    /** @brief Link table */
    IMacLinkTable* linkTable;
    /** @brief slotframe table */
    IMacSlotframeTable* slotframeTable;

    /** @brief timeslot table */
    IMacTimeslotTable* timeslotTable;

    /** @brief buffer for one timeslot entry */
    macTimeslotTableEntry* timeslotTemplate;

    /** @brief Hopping Sequence List */
    IMacHoppingSequenceList* hoppingSequenceList;

    /** @brief neighbor information table */
    IMacNeighborTable* neighborTable;

    // @brief cluster information table
    IRLLClusterTable *clusterTable;
};

#endif /* RLL_H_ */

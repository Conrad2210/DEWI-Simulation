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

#ifndef SLOTTEDALOHASCHEDULER_H_
#define SLOTTEDALOHASCHEDULER_H_

#include <IbaseScheduler.h>
#include <csimplemodule.h>

class INET_API SlottedAlohaScheduler: public cSimpleModule,public IbaseScheduler {
public:
    SlottedAlohaScheduler();
    virtual ~SlottedAlohaScheduler();

protected:
    virtual int numInitStages() {return 2;}

    virtual void initialize(int);

    virtual void handleMessage(cMessage *msg);


    //Association Process
    virtual void MLME_ASSOCIATE_request(cMessage *msg );

    virtual void handle_MLME_ASSOCIATE_indication(cMessage *msg);

    virtual void MLME_ASSOCIATE_responce(cMessage *msg);

    virtual void handle_MLME_ASSOCIATE_confirm(cMessage *msg);


    //Dissassociation Process
    virtual void MLME_DISASSOCIATE_request(cMessage *msg);

    virtual void handle_MLME_DIASSOCIATE_indication(cMessage *msg);

    virtual void handle_MLME_DIASSOCIATE_confirm(cMessage *msg);

    //Advertisment

    virtual void MLME_BEACON_request(cMessage *msg);

    virtual void handle_MLME_BEACON_confirm(cMessage *msg);

    virtual void handle_MLME_BEACON_indication(cMessage *msg);

    virtual void MLME_SETSLOTFRAME_request(cMessage *msg);

    virtual void handle_MLME_SETSLOTFRAME_confirm(cMessage *msg);

    virtual void MLME_SETLINK_request(cMessage *msg);

    virtual void handle_MLME_SETLINK_confirm(cMessage *msg);

    virtual void MLME_TSCHMODE_request(cMessage *msg);

    virtual void handle_MLME_TSCHMODE_confirm(cMessage *msg);
};

#endif /* SLOTTEDALOHASCHEDULER_H_ */

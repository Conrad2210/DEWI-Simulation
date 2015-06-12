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

#ifndef RLLCLUSTERTABLEENTRY_H_
#define RLLCLUSTERTABLEENTRY_H_

#include "INETDefs.h"
#include "Ieee802154Def.h"

class IRLLClusterTable;


class INET_API RLLClusterTableEntry : public cObject
{
    public:
	RLLClusterTableEntry();
	RLLClusterTableEntry(const RLLClusterTableEntry &entry);

	virtual ~RLLClusterTableEntry();

    protected:
	IRLLClusterTable *ownerp;
	cModule *clusterModule;
	int id;
	int stage;
	UINT_16 address;
	char* nodeName;
	bool isCH;
	UINT_16 panId;

    public:

	virtual void resetEntry();
	virtual std::string info() const;
	virtual std::string getFullPath() const;

	IRLLClusterTable *getOwnerModule() const{return ownerp;}

	int getEntryId() const {return id;}
	void setEntryId(int i = -1) {id = i;}

	cModule *getClusterModule(){return clusterModule;}
	void setClusterModule(cModule *module = NULL){clusterModule = module;}

	int getStage() {return stage;}
	void setStage(int i = -1){stage = i;}

	UINT_16 getAddress() const{return address;}
	void setAddress(UINT_16 i = 0x00) {address = i;}

	char *getNodeName(){return nodeName;}
	void setNodeName(char *c = (char*)""){nodeName = c;}

	bool getIsCH() {return isCH;}
	void setIsCH(bool b = false){isCH = b;}

	void setPanId(UINT16 ui){panId = ui;}
	UINT16 getPanId(){return panId;}


};

#endif /* RLLCLUSTERTABLEENTRY_H_ */

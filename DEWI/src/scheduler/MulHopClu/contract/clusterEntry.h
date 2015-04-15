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

#ifndef CLUSTERENTRY_H_
#define CLUSTERENTRY_H_
#include "Ieee802154eFrame_m.h"
class clusterEntry
{
    public:
	clusterEntry();
	clusterEntry(bool ch, int Stage, UINT16 addr,UINT16 PAN);
	virtual ~clusterEntry();

    protected:
	bool CH;
	int stage;
	UINT16 shrtAddr;
	UINT16 panId;

    public:
	void setCH(bool b){CH=b;}
	bool getCH(){return CH;}

	void setStage(int i){stage = i;}
	int getStage(){return stage;}

	void setShrtAddr(UINT16 ui){shrtAddr = ui;}
	UINT16 getShrtAddr(){return shrtAddr;}

	void setPanId(UINT16 ui){panId = ui;}
	UINT16 getPanId(){return panId;}

	virtual std::string info() const;

};

#endif /* CLUSTERENTRY_H_ */

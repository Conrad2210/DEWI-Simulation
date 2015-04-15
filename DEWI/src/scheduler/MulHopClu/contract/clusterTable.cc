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

#include <clusterTable.h>
Register_Class(clusterTable)
std::ostream& operator<<(std::ostream& os, const clusterEntry& e)
{
    os << e.info();
    return os;
}


clusterTable::clusterTable()
{
    // TODO Auto-generated constructor stub

}

clusterTable::~clusterTable()
{
    // TODO Auto-generated destructor stub
}

void clusterTable::initialize(int stage)
{
    if(stage == 1)
    {
	WATCH_PTRVECTOR(entryTable);
    }
}

void clusterTable::addEntry(bool ch, int stage, UINT16 shrtAddr, UINT16 PAN)
{
    if(getEntryByShrtAddr(shrtAddr) == NULL)
	entryTable.push_back(new clusterEntry(ch,stage,shrtAddr,PAN));
}

clusterEntry *clusterTable::getEntryByShrtAddr(UINT16 shrtAddr)
{
    for(int i = 0; i < (int)entryTable.size(); i++)
    {
	if(entryTable.at(i)->getShrtAddr() == shrtAddr)
	    return entryTable.at(i);
    }

    return NULL;
}

void clusterTable::deleteEntry(clusterEntry *entry)
{
    for(int i = 0; i < (int)entryTable.size(); i++)
    {
	if(entryTable.at(i)->getShrtAddr() == entry->getShrtAddr())
	{
	    entryTable.erase(entryTable.begin()+i);
	    i = -1;
	}
    }
}

void clusterTable::clearTable()
{
    while(entryTable.size() != 0)
	entryTable.erase(entryTable.begin());
}

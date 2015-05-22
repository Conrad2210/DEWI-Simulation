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

#include "RLLClusterTableEntry.h"
#include "RLLClusterTable.h"

RLLClusterTableEntry::RLLClusterTableEntry()
{
    // TODO Auto-generated constructor stub

}

RLLClusterTableEntry::~RLLClusterTableEntry()
{
    // TODO Auto-generated destructor stub
}

std::string RLLClusterTableEntry::info()const
{
    std::stringstream out;
    out << "Entry Id: " << id;
    out << ", Name: " << nodeName;
    out << ", Stage: " << stage;
    out << ", Address: " << address;
    out << ", isCH: " << isCH;
    return out.str();
}

std::string RLLClusterTableEntry::getFullPath() const
{
    return ownerp == NULL ? getFullName() : ownerp -> getHostModule() -> getFullPath() + "." + getFullName();

}

void RLLClusterTableEntry::resetEntry()
{
    id = -1;
    clusterModule = NULL;
    ownerp = NULL;
    stage = -1;
    address = 0x00;
    nodeName  = "";
    isCH = false;

}

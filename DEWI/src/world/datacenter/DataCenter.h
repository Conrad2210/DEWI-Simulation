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

#ifndef DATACENTER_H_
#define DATACENTER_H_
#include "INETDefs.h"
#include <string>
#include <vector>
#include "DataVector.h"
#include "cAssociated.h"
class DataVector;

class INET_API DataCenter : public cSimpleModule
{
public:
    DataCenter();
    virtual ~DataCenter();
    void registerVector(DataVector* Vec);
    void recordScalar(std::string Data,std::string Type, std::string Index, std::string Name);
    void registerNode(int index, char* name, bool associated);

    void registerAssociatedVector(int ix, const char* na, bool as, int st, int parix, const char* parna);

    void updateAssociatedVector(int ix, const char* na, bool as, int st, int parix, const char* parna);

    bool allAssociated();

    int getNumAssNodes(){return numAssoNodes;}
    int getNumRegisteredAssVectors(){return numRegisteredAssVectors;}



private:
    //initialize datacenter
    void initialize(int stage);

    //finish DataCenter
    //writes values to csv files
    void finish();
    double lastAssociated;
    void updateLastAssociated(double d){lastAssociated = d;}




    /////////////////////////////////////////////////////////////////////////////
    //////////////      Variables       /////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////

private:
    //Path where results get stored when call finish;
    std::string ResultPath;
    bool recordValues;
    int numRegisteredAssVectors;
    int numAssoNodes;

    std::vector<DataVector*> ResultVectors;

    std::vector<cAssociated *> AssociatedVector;
};

#endif /* DATACENTER_H_ */

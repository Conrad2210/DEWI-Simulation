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

#ifndef DATAVECTOR_H_
#define DATAVECTOR_H_

#include <string>
#include <vector>
#include "DataCenter.h"
#include "INETDefs.h"

class DataCenter;

class INET_API DataVector : public cObject {
public:
    //Constructer
    DataVector(std::string name, std::string type);

    //Destructer
    virtual ~DataVector();

    //Save all Data to output file
    void saveData(std::string path);

    //record double value
    void record(double value);

    //record value with name
    void record(double value, std::string name);

    void record(double value, const char* name);

    //record value with Id
    void record(double value, int id);

    //record int value
    void record(int value);

    //record string value
    void record(std::string value);

    //register DataVector to DataCenter
    void registerVector();

    void printSize();


    /////////////////////////////////////////////////////////////////////////////
    //////////////      Variables       /////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////

private:

    //Result Vector
    std::vector<std::string> Data;


    //Vector Name
    std::string Name;

    //Vector Type
    std::string Type;

    //Pointer to DataCenter
    DataCenter* center;




};

#endif /* DATAVECTOR_H_ */

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

#include <datacenter/DataCenter.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "cmodule.h"
#include <dirent.h>
#include <ios>
#include "DataFunctions.h"

Define_Module(DataCenter);

DataCenter::DataCenter() {
    // TODO Auto-generated constructor stub
    ResultPath = "";

}

DataCenter::~DataCenter() {
    // TODO Auto-generated destructor stub
}

void DataCenter::initialize(int stage)
{
    if(stage == 0)
    {
        ResultPath = ev.getConfigEx()->getConfigValue("result-dir");
        recordValues = par("recordValues").boolValue();
    }
}


void DataCenter::finish()
{
    if(recordValues)
    {
        for(int i = 0; i < (int)ResultVectors.size(); i++)
        {
            ResultVectors.at(i)->saveData(ResultPath);
        }
    }

}

void DataCenter::registerVector(DataVector *Vec)
{
    ResultVectors.push_back(Vec);
}

void DataCenter::recordScalar(std::string Data,std::string Type, std::string Index, std::string Name)
{
    //declare Path variable
    std::stringstream path;

    //create absolute path
#ifdef WIN32
    path << ResultPath << "\\" << Type << "\\" << Index;
#elif linux
    path << ResultPath << "/" << Type << "/" << Index;
#endif
    //Create Directories
    if(createDirectories(path.str()))
    {
        //Set path including file name
#ifdef WIN32
        path << "\\" << Name << ".csv";
#elif linux
        path << "/" << Name << ".csv";
#endif

        //open output file
        std::ofstream fout(path.str().c_str());

        //check if outputfile is open
        if(fout.is_open())
        {
            //write data to output file
            fout << Data << ",\n";
        }

        //close file
        fout.close();
    }
}


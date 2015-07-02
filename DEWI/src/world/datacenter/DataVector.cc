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

#include <DataVector.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "cmodule.h"
#include <dirent.h>
#include <ios>
#include "DataFunctions.h"

using namespace std;

#ifndef WITH_LINUX
#define WITH_LINUX
#endif

#ifdef WIN32
#ifndef WITH_WIN
#define WITH_WIN
#endif
#endif

DataVector::DataVector(std::string name, std::string type) {

    //set Name (e.g. NodeIndex)
    Name = name;

    //set result type (e.g. latency)
    Type = type;

    //get pointer to DataCenter
    center = check_and_cast<DataCenter *>(
            center->getModuleByPath("DataCenter"));

}

DataVector::~DataVector() {

}

void DataVector::record(double value) {
    //declare result variable
    std::stringstream a;

    //convert double to string
    a << value;

    //add data entry
    Data.push_back(a.str());
}

void DataVector::record(double value, std::string name) {
    //declare result variable
    std::stringstream a;

    //convert double to string
    a << value << "," << name;

    //add data entry
    Data.push_back(a.str());
}
void DataVector::record(double value, const char* name) {
    //declare result variable
    std::stringstream a;

    //convert double to string
    a << value << "," << name;

    //add data entry
    Data.push_back(a.str());
}

void DataVector::record(double value, int id) {
    //declare result variable
    std::stringstream a;

    //convert double to string
    a << value << "," << id;

    //add data entry
    Data.push_back(a.str());
}

void DataVector::record(int value) {
    //declare result variable
    std::stringstream a;

    //convert int to string
    a << value;

    //add data entry
    Data.push_back(a.str());
}

void DataVector::record(std::string value) {
    //add data entry
    Data.push_back(value);
}

void DataVector::saveData(std::string Path) {

    //declare path variable
    stringstream ss;
    ss.str("");

    //set absolute path
#ifdef WITH_WIN
    ss << Path << "\\" << Type << "\\" << Name;

#endif
#ifdef WITH_LINUX
    ss << Path << "/" << Type << "/" << Name;
#endif

    //make directories

    if (createDirectories(ss.str())) {
        //Set path including fileName
#ifdef WITH_WIN
        ss << "\\" << ev.getConfigEx()->getActiveConfigName() << "_" << ev.getConfigEx()->getActiveRunNumber() << ".csv";
#endif
#ifdef WITH_LINUX
        ss << "/" << ev.getConfigEx()->getActiveConfigName() << "_"
                << ev.getConfigEx()->getActiveRunNumber() << ".csv";
#endif

        //Open output file
        std::ofstream fout(ss.str().c_str());

        //check if output file is open
        if (fout.is_open()) {
            //write data to file
            fout << Type << ",\n";
            for (int i = 0; i < (int) Data.size(); i++) {
                //each data value in an new row
                fout << Data.at(i) << ",\n";
            }
            //close output file
            fout.close();
        }
    }
}

void DataVector::registerVector() {
    //register vector to datacenter
    center->registerVector(this);
}

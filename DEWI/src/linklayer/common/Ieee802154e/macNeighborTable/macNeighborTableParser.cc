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

/*
 * @author: Stefan Reis
 * Copyright(c) 2014: Stefan Reis
 * Copyright(c) 2014: Nimbus Centre for Embedded Systems Research, Cork, Ireland
 * Copyright(c) 2014: Cork Institute of Technology, Cork, Ireland
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "macNeighborTableParser.h"

#include "IInterfaceTable.h"
#include "macNeighborTableEntry.h"
#include "Ieee802154eEnum.h"
#include "Ieee802154eDefSlotframe.h"

//
// Constants
//
const int MAX_FILESIZE = 10000; // TBD lift hardcoded limit
const int MAX_ENTRY_STRING_SIZE = 500;

const char  *NEIGHBOR_START_TOKEN = "neighbor:",
            *NEIGHBOR_END_TOKEN = "neighborend.";

macNeighborTableParser::macNeighborTableParser(IInterfaceTable *i, IMacNeighborTable *st)
{
    ift = i;
    neighborTable = st;
}

int macNeighborTableParser::readNeighborTableFromFile(const char *filename)
{
    FILE *fp;
    int charpointer;
    char *file = new char[MAX_FILESIZE];
    char *neighborFile = NULL;

    std::string path = (std::string) filename + ".txt";

    fp = fopen(path.c_str(), "r");
    if (fp == NULL)
    {
        if (ev.isGUI())
        {
            EV << "Error opening neighbor table file " << filename << endl;
            return 0;
        }
    }

    //read everything into file[] char array
    for (charpointer = 0; (file[charpointer] = getc(fp)) != EOF; charpointer++)
        ;

    charpointer++;
    for (; charpointer < MAX_FILESIZE; charpointer++)
        file[charpointer] = '\0';

    fclose(fp);

    // copy file into specialized, filtered char arrays
    for (charpointer = 0; (charpointer < MAX_FILESIZE) && (file[charpointer] != EOF); charpointer++)
    {
        // check for tokens at beginning of file or line
        if (charpointer == 0 || file[charpointer - 1] == '\n')
        {
            if (streq(file + charpointer, NEIGHBOR_START_TOKEN))
            {
                neighborFile = createFilteredFile(file, charpointer, NEIGHBOR_END_TOKEN);
            }
        }
    }
    delete[] file;

    if (neighborFile)
        parseNeighborTable(neighborFile);

    if (neighborFile)
        delete[] neighborFile;

    return 0;
}

char *macNeighborTableParser::createFilteredFile(char *file, int &charpointer, const char *endtoken)
{
    int i = 0;
    char *filterFile = new char[MAX_FILESIZE];
    filterFile[0] = '\0';

    while (true)
    {
        // skip blank lines and comments
        while (!isalnum(file[charpointer]) && !isspace(file[charpointer]))
        {
            while (file[charpointer++] != '\n')
                ;
        }

        // check for endtoken:
        if (streq(file + charpointer, endtoken))
        {
            filterFile[i] = '\0';
            break;
        }

        // copy whole line to filterFile
        while ((filterFile[i++] = file[charpointer++]) != '\n')
            ;
    }
    return filterFile;
}

int macNeighborTableParser::streq(const char *str1, const char *str2)
{
    return (strncmp(str1, str2, strlen(str2)) == 0);
}

int macNeighborTableParser::strcpyword(char *dest, const char *src)
{
    int i;
    for (i = 0; !isspace(dest[i] = src[i]); i++)
        ;

    dest[i] = '\0';
    return i;
}

void macNeighborTableParser::skipBlanks(char *str, int &charptr)
{
    for (; isspace(str[charptr]); charptr++)
        ;
}

void macNeighborTableParser::parseNeighborTable(char *neighborFile)
{
    char buf[MAX_ENTRY_STRING_SIZE];
    int charpointer = strlen(NEIGHBOR_START_TOKEN);
    macNeighborTableEntry *entry = new macNeighborTableEntry();

    while (neighborFile[charpointer] != '\0')
    {
        //NeighborSize entry
        if (streq(neighborFile + charpointer, "NeighborSize:"))
        {
            char* tmp = parseEntry(neighborFile, "NeighborSize:", charpointer, buf);
            entry->setNeighborSize(atoi(tmp));
        }

        if (streq(neighborFile + charpointer, "\n"))
        {
            // add entry
            entry->setNeighborTable(neighborTable);
            neighborTable->addNeighbor(entry);
            entry = new macNeighborTableEntry();
        }

        // no entry discovered: move charpointer on
        charpointer++;
    }
    delete entry;
    entry = NULL;
}

char *macNeighborTableParser::parseEntry(char *neighborFile, const char *tokenStr, int &charpointer, char* destStr)
{
    int temp = 0;

    charpointer += strlen(tokenStr);
    skipBlanks(neighborFile, charpointer);
    temp = strcpyword(destStr, neighborFile + charpointer);
    charpointer += temp;

    skipBlanks(neighborFile, charpointer);

    return destStr;
}

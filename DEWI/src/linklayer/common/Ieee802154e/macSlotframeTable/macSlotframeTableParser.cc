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
 * @author: Conrad Dandelski
 * Copyright(c) 2014: Conrad Dandelski
 * Copyright(c) 2014: Nimbus Centre for Embedded Systems Research, Cork, Ireland
 * Copyright(c) 2014: Cork Institute of Technology, Cork, Ireland
 *
 * @author: 2014    Stefan Reis     (modified - shifted the read from file in the parse class)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "macSlotframeTableParser.h"

#include "IInterfaceTable.h"
#include "macSlotframeTableEntry.h"
#include "Ieee802154eEnum.h"
#include "Ieee802154eDefSlotframe.h"

//
// Constants
//
const int MAX_FILESIZE = 10000; // TBD lift hardcoded limit
const int MAX_ENTRY_STRING_SIZE = 500;

const char  *SLOTFRAME_START_TOKEN = "slotframe:",
            *SLOTFRAME_END_TOKEN = "slotframeend.";

macSlotframeTableParser::macSlotframeTableParser(IInterfaceTable *i, IMacSlotframeTable *st)
{
    //@ author: Conrad Dandelski
    ift = i;
    slotframeTable = st;
}

int macSlotframeTableParser::readSlotframeTableFromFile(const char *filename)
{
    FILE *fp;
    int charpointer;
    char *file = new char[MAX_FILESIZE];
    char *slotframeFile = NULL;

    std::string path = (std::string) filename + ".txt";

    fp = fopen(path.c_str(), "r");
    if (fp == NULL)
    {
        if (ev.isGUI())
        {
            EV << "Error opening slotframe table file " << filename << endl;
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
            if (streq(file + charpointer, SLOTFRAME_START_TOKEN))
            {
                slotframeFile = createFilteredFile(file, charpointer, SLOTFRAME_END_TOKEN);
            }
        }
    }
    delete[] file;

    if (slotframeFile)
        parseSlotframeTable(slotframeFile);

    if (slotframeFile)
        delete[] slotframeFile;

    return 0;
}

char *macSlotframeTableParser::createFilteredFile(char *file, int &charpointer, const char *endtoken)
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

int macSlotframeTableParser::streq(const char *str1, const char *str2)
{
    return (strncmp(str1, str2, strlen(str2)) == 0);
}

int macSlotframeTableParser::strcpyword(char *dest, const char *src)
{
    int i;
    for (i = 0; !isspace(dest[i] = src[i]); i++)
        ;

    dest[i] = '\0';
    return i;
}

void macSlotframeTableParser::skipBlanks(char *str, int &charptr)
{
    for (; isspace(str[charptr]); charptr++)
        ;
}

void macSlotframeTableParser::parseSlotframeTable(char *slotframeFile)
{
    char buf[MAX_ENTRY_STRING_SIZE];
    int charpointer = strlen(SLOTFRAME_START_TOKEN);
    macSlotframeTableEntry *entry = new macSlotframeTableEntry();

    while (slotframeFile[charpointer] != '\0')
    {
        //slotframeSize entry
        if (streq(slotframeFile + charpointer, "SlotframeSize:"))
        {
            char* tmp = parseEntry(slotframeFile, "SlotframeSize:", charpointer, buf);
            entry->setSlotframeSize(atoi(tmp));
        }

        if (streq(slotframeFile + charpointer, "\n"))
        {
            // add entry
            entry->setSlotframeTable(slotframeTable);
            slotframeTable->addSlotframe(entry);
            entry = new macSlotframeTableEntry();
        }

        // no entry discovered: move charpointer on
        charpointer++;
    }

    delete entry;
    entry = NULL;
}

char *macSlotframeTableParser::parseEntry(char *slotframeFile, const char *tokenStr, int &charpointer, char* destStr)
{
    int temp = 0;

    charpointer += strlen(tokenStr);
    skipBlanks(slotframeFile, charpointer);
    temp = strcpyword(destStr, slotframeFile + charpointer);
    charpointer += temp;

    skipBlanks(slotframeFile, charpointer);

    return destStr;
}

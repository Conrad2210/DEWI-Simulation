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
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "macTimeslotTableParser.h"
#include "macTimeslotTableEntry.h"
#include "Ieee802154eEnum.h"
#include "Ieee802154eDef.h"
#include "Ieee802154eDefSlotframe.h"

//
// Constants
//
const int MAX_FILESIZE = 10000; // TBD lift hardcoded limit
const int MAX_ENTRY_STRING_SIZE = 500;

const char *TIMESLOT_START_TOKEN = "timeslot:", *TIMESLOT_END_TOKEN = "timeslotend.";

macTimeslotTableParser::macTimeslotTableParser(IInterfaceTable *i, IMacTimeslotTable *tt)
{
    ift = i;
    timeslotTable = tt;
}

int macTimeslotTableParser::readTimeslotTableFromFile(const char *filename)
{
    FILE *fp;
    int charpointer;
    char *file = new char[MAX_FILESIZE];
    char *templateFile = NULL;

    std::string path = (std::string) filename + ".txt";

    fp = fopen(path.c_str(), "r");
    if (fp == NULL)
    {
        if (ev.isGUI())
        {
            EV << "Error opening timeslot table file " << filename << endl;
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
            if (streq(file + charpointer, TIMESLOT_START_TOKEN))
            {
                templateFile = createFilteredFile(file, charpointer, TIMESLOT_END_TOKEN);
            }
        }
    }
    delete[] file;

    if (templateFile)
        parseTimeslotTable(templateFile);

    if (templateFile)
        delete[] templateFile;

    return 0;
}

char *macTimeslotTableParser::createFilteredFile(char *file, int &charpointer, const char *endtoken)
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

int macTimeslotTableParser::streq(const char *str1, const char *str2)
{
    return (strncmp(str1, str2, strlen(str2)) == 0);
}

int macTimeslotTableParser::strcpyword(char *dest, const char *src)
{
    int i;
    for (i = 0; !isspace(dest[i] = src[i]); i++)
        ;

    dest[i] = '\0';
    return i;
}

void macTimeslotTableParser::skipBlanks(char *str, int &charptr)
{
    for (; isspace(str[charptr]); charptr++)
        ;
}

void macTimeslotTableParser::parseTimeslotTable(char *templateFile)
{
    char buf[MAX_ENTRY_STRING_SIZE];
    int charpointer = strlen(TIMESLOT_START_TOKEN);;
    macTimeslotTableEntry *entry = new macTimeslotTableEntry();

    while (templateFile[charpointer] != '\0')
    {
        if (streq(templateFile + charpointer, "TsCCAOffset:"))
        {
            char* tmp = parseEntry(templateFile, "TsCCAOffset:", charpointer, buf);
            entry->setCCAOffset(atof(tmp));
        }

        if (streq(templateFile + charpointer, "TsCCA:"))
        {
            char* tmp = parseEntry(templateFile, "TsCCA:", charpointer, buf);
            entry->setCCA(atof(tmp));
        }

        if (streq(templateFile + charpointer, "TsTxOffset:"))
        {
            char* tmp = parseEntry(templateFile, "TsTxOffset:", charpointer, buf);
            entry->setTxOffset(atof(tmp));
        }

        if (streq(templateFile + charpointer, "TsRxOffset:"))
        {
            char* tmp = parseEntry(templateFile, "TsRxOffset:", charpointer, buf);
            entry->setRxOffset(atof(tmp));
        }

        if (streq(templateFile + charpointer, "TsRxAckDelay:"))
        {
            char* tmp = parseEntry(templateFile, "TsRxAckDelay:", charpointer, buf);
            entry->setRxAckDelay(atof(tmp));
        }

        if (streq(templateFile + charpointer, "TsTxAckDelay:"))
        {
            char* tmp = parseEntry(templateFile, "TsTxAckDelay:", charpointer, buf);
            entry->setTxAckDelay(atof(tmp));
        }

        if (streq(templateFile + charpointer, "TsRxWait:"))
        {
            char* tmp = parseEntry(templateFile, "TsRxWait:", charpointer, buf);
            entry->setRxWait(atof(tmp));
        }

        if (streq(templateFile + charpointer, "TsAckWait:"))
        {
            char* tmp = parseEntry(templateFile, "TsAckWait:", charpointer, buf);
            entry->setAckWait(atof(tmp));
        }

        if (streq(templateFile + charpointer, "TsRxTx:"))
        {
            char* tmp = parseEntry(templateFile, "TsRxTx:", charpointer, buf);
            entry->setRxTx(atof(tmp));
        }

        if (streq(templateFile + charpointer, "TsMaxAck:"))
        {
            char* tmp = parseEntry(templateFile, "TsMaxAck:", charpointer, buf);
            entry->setMaxAck(atof(tmp));
        }

        if (streq(templateFile + charpointer, "TsMaxTx:"))
        {
            char* tmp = parseEntry(templateFile, "TsMaxTx:", charpointer, buf);
            entry->setMaxTx(atof(tmp));
        }

        if (streq(templateFile + charpointer, "TsTimeslotLength:"))
        {
            char* tmp = parseEntry(templateFile, "TsTimeslotLength:", charpointer, buf);
            entry->setTimeslotLength(atof(tmp));
        }

        if (streq(templateFile + charpointer, "\n"))
        {
            entry->setTemplateTable(timeslotTable);
            timeslotTable->addTemplate(entry);
            entry = new macTimeslotTableEntry();
        }
        charpointer++;
    }

    delete entry;
    entry = NULL;
}

char *macTimeslotTableParser::parseEntry(char *templateFile, const char *tokenStr, int &charpointer, char* destStr)
{
    int temp = 0;

    charpointer += strlen(tokenStr);
    skipBlanks(templateFile, charpointer);
    temp = strcpyword(destStr, templateFile + charpointer);
    charpointer += temp;

    skipBlanks(templateFile, charpointer);

    return destStr;
}

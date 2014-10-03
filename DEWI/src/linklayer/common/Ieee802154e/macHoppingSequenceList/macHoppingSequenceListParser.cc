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

#include "macHoppingSequenceListParser.h"

#include "macHoppingSequenceListEntry.h"
#include "Ieee802154eEnum.h"
#include "Ieee802154eDefSlotframe.h"

//
// Constants
//
const int MAX_FILESIZE = 10000; // TBD lift hardcoded limit
const int MAX_ENTRY_STRING_SIZE = 500;

const char  *HOPPINGSEQUENCE_START_TOKEN = "hopping:",
            *HOPPINGSEQUENCE_END_TOKEN = "hoppingend.";

macHoppingSequenceListParser::macHoppingSequenceListParser(IInterfaceTable *i, IMacHoppingSequenceList *hs)
{
    ift = i;
    hoppingSequence = hs;
}

int macHoppingSequenceListParser::readHoppingSequenceListFromFile(const char *filename)
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
            EV << "Error opening hopping sequence list file " << filename << endl;
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
            if (streq(file + charpointer, HOPPINGSEQUENCE_START_TOKEN))
            {
                templateFile = createFilteredFile(file, charpointer, HOPPINGSEQUENCE_END_TOKEN);
            }
        }
    }
    delete[] file;

    if (templateFile)
        parseHoppingSequenceList(templateFile);

    if (templateFile)
        delete[] templateFile;

    return 0;
}

char *macHoppingSequenceListParser::createFilteredFile(char *file, int &charpointer, const char *endtoken)
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

int macHoppingSequenceListParser::streq(const char *str1, const char *str2)
{
    return (strncmp(str1, str2, strlen(str2)) == 0);
}

int macHoppingSequenceListParser::strcpyword(char *dest, const char *src)
{
    int i;
    for (i = 0; !isspace(dest[i] = src[i]); i++)
        ;

    dest[i] = '\0';
    return i;
}

void macHoppingSequenceListParser::skipBlanks(char *str, int &charptr)
{
    for (; isspace(str[charptr]); charptr++)
        ;
}

void macHoppingSequenceListParser::parseHoppingSequenceList(char *templateFile)
{
    char buf[MAX_ENTRY_STRING_SIZE];
    int charpointer = strlen(HOPPINGSEQUENCE_START_TOKEN);;
    macHoppingSequenceListEntry *entry = new macHoppingSequenceListEntry();

    while (templateFile[charpointer] != '\0')
    {
        if (streq(templateFile + charpointer, "ChannelPage:"))
        {
            char* tmp = parseEntry(templateFile, "ChannelPage:", charpointer, buf);
            entry->setChannelPage(atof(tmp));
        }

        if (streq(templateFile + charpointer, "NumberOfChannels:"))
        {
            char* tmp = parseEntry(templateFile, "NumberOfChannels:", charpointer, buf);
            entry->setNumberOfChannels(atof(tmp));
        }

        if (streq(templateFile + charpointer, "PhyConfiguration:"))
        {
            char* tmp = parseEntry(templateFile, "PhyConfiguration:", charpointer, buf);
            entry->setPhyConfiguration(atof(tmp));
        }

        if (streq(templateFile + charpointer, "ExtendedBitmap:"))
        {
            char* tmp = parseEntry(templateFile, "ExtendedBitmap:", charpointer, buf);
            entry->setExtendedBitmap(atof(tmp));
        }

        if (streq(templateFile + charpointer, "HoppingSequenceLength:"))
        {
            char* tmp = parseEntry(templateFile, "HoppingSequenceLength:", charpointer, buf);
            entry->setHoppingSequenceLength(atof(tmp));
        }

        // hoppingSequenceList
        if (streq(templateFile + charpointer, "HoppingSequenceList:"))
        {
            entry->setHoppingSequenceList( *parseList(templateFile, "HoppingSequenceList:", charpointer, buf));
        }

        if (streq(templateFile + charpointer, "CurrentHop:"))
        {
            char* tmp = parseEntry(templateFile, "CurrentHop:", charpointer, buf);
            entry->setCurrentHop(atof(tmp));
        }

        if (streq(templateFile + charpointer, "HopDwellTime:"))
        {
            char* tmp = parseEntry(templateFile, "HopDwellTime:", charpointer, buf);
            entry->setHopDwellTime(atof(tmp));
        }

        if (streq(templateFile + charpointer, "\n"))
        {
            entry->setHoppingSequenceListModule(hoppingSequence);
            hoppingSequence->addEntry(entry);
            entry = new macHoppingSequenceListEntry();
        }
        charpointer++;
    }
    delete entry;
    entry = NULL;
}

char *macHoppingSequenceListParser::parseEntry(char *templateFile, const char *tokenStr, int &charpointer, char* destStr)
{
    int temp = 0;

    charpointer += strlen(tokenStr);
    skipBlanks(templateFile, charpointer);
    temp = strcpyword(destStr, templateFile + charpointer);
    charpointer += temp;

    skipBlanks(templateFile, charpointer);

    return destStr;
}

std::vector<UINT_16> *macHoppingSequenceListParser::parseList(char *templateFile, const char *tokenStr, int &charpointer, char* destStr)
{
    std::vector<UINT_16>* tmpList = new std::vector<UINT_16>();

    char* tmp = parseEntry(templateFile, tokenStr, charpointer, destStr);
    tmpList->push_back(atof(tmp));

    tmp = parseEntry(templateFile, ";", charpointer, destStr);

    while (!streq(tmp, "HoppingSequenceListEnd"))
    {
        tmpList->push_back(atof(tmp));
        tmp = parseEntry(templateFile, ";", charpointer, destStr);
    }

    return tmpList;
}

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

#include "macLinkTableParser.h"

#include "IInterfaceTable.h"
#include "macLinkTableEntry.h"
#include "Ieee802154eEnum.h"
#include "Ieee802154eDefSlotframe.h"

//
// Constants
//
const int MAX_FILESIZE = 10000; // TBD lift hardcoded limit
const int MAX_ENTRY_STRING_SIZE = 500;

const char  *LINK_START_TOKEN = "link:",
            *LINK_END_TOKEN = "linkend.";

macLinkTableParser::macLinkTableParser(IInterfaceTable *i, IMacLinkTable *lt)
{
    ift = i;
    linkTable = lt;
}

int macLinkTableParser::readLinkTableFromFile(const char *filename)
{
    FILE *fp;
    int charpointer;
    char *file = new char[MAX_FILESIZE];
    char *linkFile = NULL;

    std::string path = (std::string) filename + ".txt";

    fp = fopen(path.c_str(), "r");
    if (fp == NULL)
    {
        if(ev.isGUI())
        {
            EV<< "Error opening link table file " << filename << endl;
            return 0;
        }
    }

    //read everthing into file[] char array
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
            if (streq(file + charpointer, LINK_START_TOKEN))
            {
                linkFile = createFilteredFile(file, charpointer, LINK_END_TOKEN);
            }
        }
    }
    delete[] file;

    if (linkFile)
        parseLinkTable(linkFile);

    if (linkFile)
        delete[] linkFile;

    return 0;
}

char *macLinkTableParser::createFilteredFile(char *file, int &charpointer, const char *endtoken)
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

int macLinkTableParser::streq(const char *str1, const char *str2)
{
    return (strncmp(str1, str2, strlen(str2)) == 0);
}

int macLinkTableParser::strcpyword(char *dest, const char *src)
{
    int i;
    for (i = 0; !isspace(dest[i] = src[i]); i++)
        ;

    dest[i] = '\0';
    return i;
}

void macLinkTableParser::skipBlanks(char *str, int &charptr)
{
    for (; isspace(str[charptr]); charptr++)
        ;
}

void macLinkTableParser::parseLinkTable(char *linkFile)
{
    char buf[MAX_ENTRY_STRING_SIZE];
    int charpointer = strlen(LINK_START_TOKEN);
    macLinkTableEntry *entry = new macLinkTableEntry();

    while (linkFile[charpointer] != '\0')
    {
        if(streq(linkFile + charpointer, "LinkID:"))
        {
            char* tmp = parseEntry(linkFile, "LinkId:", charpointer, buf);
            entry->setLinkId(atoi(tmp));
        }
        //LinkOption Entry
        if (streq(linkFile + charpointer, "LinkOption:"))
        {
            char* tmpOption = parseEntry(linkFile, "LinkOption:", charpointer, buf);
            if (streq(tmpOption, "0") == 1)
            {
                entry->setLinkOption(LNK_OP_TRANSMIT);
            }
            else if (streq(tmpOption, "1") == 1)
            {
                entry->setLinkOption(LNK_OP_RECEIVE);
            }
            else if (streq(tmpOption, "2") == 1)
            {
                entry->setLinkOption(LNK_OP_SHARED);
            }
            else if (streq(tmpOption, "5") == 1)
            {
                entry->setLinkOption(LNK_OP_RESERVED);
            }
            else if (streq(tmpOption, "3") == 1)
            {
                entry->setLinkOption(LNK_OP_RECEIVE_TIMEKEEPING);
            }
            else if (streq(tmpOption, "4") == 1)
            {
                entry->setLinkOption(LNK_OP_SHARED_RECEIVE);
            }
            else
            {
                entry->setLinkOption(LNK_OP_undefinedOption);
            }
            continue;
        }
        if (streq(linkFile + charpointer, "LinkType:"))
        {
            char* tmpOption = parseEntry(linkFile, "LinkType:", charpointer, buf);
            if (streq(tmpOption, "0") == 1)
            {
                entry->setLinkType(LNK_TP_NORMAL);
            }
            else if (streq(tmpOption, "2") == 1)
            {
                entry->setLinkType(LNK_TP_JOIN);
            }
            else if (streq(tmpOption, "1") == 1)
            {
                entry->setLinkType(LNK_TP_ADVERTISING);
            }
            else
            {
                entry->setLinkType(LNK_TP_undefinedType);
            }
            continue;
        }

        if (streq(linkFile + charpointer, "SlotFrameId:"))
        {
            char* tmpOption = parseEntry(linkFile, "SlotFrameId:", charpointer, buf);
            entry->setSlotframeId(atoi(tmpOption));
            continue;
        }

        if (streq(linkFile + charpointer, "NodeAddress:"))
        {
            char* tmpOption = parseEntry(linkFile, "NodeAddress:", charpointer, buf);
            entry->setNodeAddress(strtol(tmpOption, NULL, 16));
            continue;
        }

        if (streq(linkFile + charpointer, "TimeSlot:"))
        {
            char* tmpOption = parseEntry(linkFile, "TimeSlot:", charpointer, buf);
            entry->setTimeslot(atoi(tmpOption));
            continue;
        }

        if (streq(linkFile + charpointer, "ChannelOffset:"))
        {
            char* tmpOption = parseEntry(linkFile, "ChannelOffset:", charpointer, buf);
            entry->setChannelOffset(atoi(tmpOption));
            continue;
        }

        if(streq(linkFile + charpointer, "\n"))
        {
            entry->setMacLinkTable(linkTable);
            linkTable->addLink(entry);
            entry = new macLinkTableEntry();
        }

        // no entry discovered: move charpointer on
        charpointer++;
    }

    delete entry;
    entry = NULL;
}

char *macLinkTableParser::parseEntry(char *linkFile, const char *tokenStr, int &charpointer, char* destStr)
{
    int temp = 0;

    charpointer += strlen(tokenStr);
    skipBlanks(linkFile, charpointer);
    temp = strcpyword(destStr, linkFile + charpointer);
    charpointer += temp;

    skipBlanks(linkFile, charpointer);

    return destStr;
}

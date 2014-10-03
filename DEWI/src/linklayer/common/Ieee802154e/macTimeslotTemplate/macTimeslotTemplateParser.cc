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

#include "macTimeslotTemplateParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "macTimeslotTemplateEntry.h"
#include "Ieee802154eEnum.h"
#include "Ieee802154eDef.h"

#include "IMacTimeslotTemplate.h"

//
// Constants
//
const int MAX_FILESIZE = 10000; // TBD lift hardcoded limit
const int MAX_ENTRY_STRING_SIZE = 500;

const char *TEMPLATE_START_TOKEN = "template:", *TEMPLATE_END_TOKEN = "templateend.";

macTimeslotTemplateParser::macTimeslotTemplateParser(IMacTimeslotTemplate *tt)
{
    templateTable = tt;

}

macTimeslotTemplateParser::~macTimeslotTemplateParser()
{

}

int macTimeslotTemplateParser::readTemplateTableFromFile(const char *filename)
{
    FILE *fp;
    int charpointer;
    char *file = new char[MAX_FILESIZE];
    char *templateFile = NULL;

    std::string path = (std::string) filename + ".txt";

    fp = fopen(path.c_str(), "r");
    if (fp == NULL)
        if (ev.isGUI())
        {
            EV << "Error opening template file " << filename << endl;
            return 0;
        }
    //throw cRuntimeError("Error opening link table file '%s'", filename);

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
            if (streq(file + charpointer, TEMPLATE_START_TOKEN))
            {
                templateFile = createFilteredFile(file, charpointer, TEMPLATE_END_TOKEN);
            }
        }
    }
    delete[] file;

    if (templateFile)
        parseTemplateTable(templateFile);

    if (templateFile)
        delete[] templateFile;

    return 0;
}

char *macTimeslotTemplateParser::createFilteredFile(char *file, int &charpointer, const char *endtoken)
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

int macTimeslotTemplateParser::streq(const char *str1, const char *str2)
{
    return (strncmp(str1, str2, strlen(str2)) == 0);
}

int macTimeslotTemplateParser::strcpyword(char *dest, const char *src)
{
    int i;
    for (i = 0; !isspace(dest[i] = src[i]); i++)
        ;
    dest[i] = '\0';
    return i;
}

void macTimeslotTemplateParser::skipBlanks(char *str, int &charptr)
{
    for (; isspace(str[charptr]); charptr++)
        ;

}

void macTimeslotTemplateParser::parseTemplateTable(char *templateFile)
{
    char buf[MAX_ENTRY_STRING_SIZE];
    int charpointer = 0;
    macTimeslotTemplateEntry *entry = new macTimeslotTemplateEntry();

    while (templateFile[charpointer] != '\0')
    {

        if (streq(templateFile + charpointer, "CCAOffset:"))
        {
            char* tmp = parseEntry(templateFile, "CCAOffset:", charpointer, buf);
            entry->setCCAOffset(atoi(tmp));
        }

        if (streq(templateFile + charpointer, "CCA:"))
        {
            char* tmp = parseEntry(templateFile, "CCA:", charpointer, buf);
            entry->setCCA(atoi(tmp));
        }

        if (streq(templateFile + charpointer, "TxOffset:"))
        {
            char* tmp = parseEntry(templateFile, "TxOffset:", charpointer, buf);
            entry->setTxOffset(atoi(tmp));
        }

        if (streq(templateFile + charpointer, "RxOffset:"))
        {
            char* tmp = parseEntry(templateFile, "RxOffset:", charpointer, buf);
            entry->setRxOffset(atoi(tmp));
        }

        if (streq(templateFile + charpointer, "RxAckDelay:"))
        {
            char* tmp = parseEntry(templateFile, "RxAckDelay:", charpointer, buf);
            entry->setRxAckDelay(atoi(tmp));
        }

        if (streq(templateFile + charpointer, "TxAckDelay:"))
        {
            char* tmp = parseEntry(templateFile, "TxAckDelay:", charpointer, buf);
            entry->setTxAckDelay(atoi(tmp));
        }

        if (streq(templateFile + charpointer, "RxWait:"))
        {
            char* tmp = parseEntry(templateFile, "RxWait:", charpointer, buf);
            entry->setRxWait(atoi(tmp));
        }

        if (streq(templateFile + charpointer, "AckWait:"))
        {
            char* tmp = parseEntry(templateFile, "AckWait:", charpointer, buf);
            entry->setAckWait(atoi(tmp));
        }

        if (streq(templateFile + charpointer, "RxTx:"))
        {
            char* tmp = parseEntry(templateFile, "RxTx:", charpointer, buf);
            entry->setRxTx(atoi(tmp));
        }

        if (streq(templateFile + charpointer, "MaxAck:"))
        {
            char* tmp = parseEntry(templateFile, "MaxAck:", charpointer, buf);
            entry->setMaxAck(atoi(tmp));
        }

        if (streq(templateFile + charpointer, "MaxTx:"))
        {
            char* tmp = parseEntry(templateFile, "MaxTx:", charpointer, buf);
            entry->setMaxTx(atoi(tmp));
        }

        if(streq(templateFile + charpointer, "TimeslotLength:"))
        {
            char* tmp = parseEntry(templateFile, "TimeslotLength:",charpointer,buf);
            entry->setTimeslotLength(atoi(tmp));
        }

        if (streq(templateFile + charpointer, "\n"))
        {
            entry->setTemplateTable(templateTable);
            templateTable->addTemplate(entry);
            entry = new macTimeslotTemplateEntry();
        }

        charpointer++;
    }
}

char *macTimeslotTemplateParser::parseEntry(char *templateFile, const char *tokenStr, int &charpointer, char* destStr)
{
    int temp = 0;

    charpointer += strlen(tokenStr);
    skipBlanks(templateFile, charpointer);
    temp = strcpyword(destStr, templateFile + charpointer);
    charpointer += temp;

    skipBlanks(templateFile, charpointer);

    return destStr;
}

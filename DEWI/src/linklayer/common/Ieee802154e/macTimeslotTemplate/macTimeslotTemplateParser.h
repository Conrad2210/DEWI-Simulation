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

#ifndef MACTIMESLOTTEMPLATEPARSER_H_
#define MACTIMESLOTTEMPLATEPARSER_H_

#include "INETDefs.h"
#include "macTimeslotTemplate.h"

class INET_API macTimeslotTemplateParser
{
    protected:
        IMacTimeslotTemplate *templateTable;
    public:
        macTimeslotTemplateParser(IMacTimeslotTemplate *tt);
        virtual ~macTimeslotTemplateParser();

        virtual int readTemplateTableFromFile(const char *filename);

    protected:

        // Return 1 if beginning of str1 and str2 is equal up to str2-len,
        // otherwise 0.
        static int streq(const char *str1, const char *str2);

        // Skip blanks in string
        static void skipBlanks(char *str, int &charptr);

        // Copies the first word of src up to a space-char into dest
        // and appends \0, returns position of next space-char in src
        static int strcpyword(char *dest, const char *src);

        //Parsing Functions
        virtual char *createFilteredFile(char *file, int &charpointer, const char *endtoken);

        virtual void parseTemplateTable(char *templateFile);

        virtual char *parseEntry(char *templateFile, const char *tokenStr, int &charpointer, char* destStr);
};

#endif /* MACTIMESLOTTEMPLATEPARSER_H_ */

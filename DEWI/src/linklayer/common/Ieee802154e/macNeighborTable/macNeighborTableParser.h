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

#ifndef MACNEIGHBORTABLEPARSER_H_
#define MACNEIGHBORTABLEPARSER_H_

#include "INETDefs.h"

#include "macNeighborTable.h"

/**
 * Parses a neighbor table file into a neighbor table.
 */
class INET_API macNeighborTableParser
{
    protected:
        IInterfaceTable *ift;
        IMacNeighborTable *neighborTable;

    public:
        /**
         * Constructor
         */
        macNeighborTableParser(IInterfaceTable *ift, IMacNeighborTable *st);

        /**
         * Destructor
         */
        virtual ~macNeighborTableParser(){};

        /**
         * Read Neighbor Table file; return 0 on success, -1 on error
         */
        virtual int readNeighborTableFromFile(const char *filename);

    protected:
        //Parsing Functions
        virtual char *createFilteredFile(char *file, int &charpointer, const char *endtoken);

        virtual void parseNeighborTable(char *neighborFile);

        virtual char *parseEntry(char *neighborFile, const char *tokenStr, int &charpointer, char* destStr);

        // Return 1 if beginning of str1 and str2 is equal up to str2-len,
        // otherwise 0.
        static int streq(const char *str1, const char *str2);

        // Skip blanks in string
        static void skipBlanks(char *str, int &charptr);

        // Copies the first word of src up to a space-char into dest
        // and appends \0, returns position of next space-char in src
        static int strcpyword(char *dest, const char *src);
};
#endif /* MACNEIGHBORTABLEPARSER_H_ */

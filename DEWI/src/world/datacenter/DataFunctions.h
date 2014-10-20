/*
 * DataFunctions.h
 *
 *  Created on: 20 Oct 2014
 *      Author: R00118979
 */

#ifndef DATAFUNCTIONS_H_
#define DATAFUNCTIONS_H_

#include <errno.h>
#include <string.h>
#include "sys/stat.h"
#include <iostream>
#include <algorithm>
#include <unistd.h>

bool CreateDirectories(std::string path);

#ifdef linux

bool CreateDirectories(std::string path)
{
    struct stat st;

    for( std::string::iterator iter = path.begin() ; iter != path.end(); )
    {
         std::string::iterator newIter = std::find( iter, path.end(), '/' );
         std::string newPath = std::string( path.begin(), newIter);

         if( stat( newPath.c_str(), &st) != 0)
         {
             if( mkdir( newPath.c_str(), 0x777) != 0 && errno != EEXIST )
             {
                return false;
             }
         }
         else
            if( !S_ISDIR(st.st_mode) )
             {
                 errno = ENOTDIR;
                 return false;
             }



         iter = newIter;
         if( newIter != path.end() )
             ++ iter;
    }
    return true;
}

#elif WIN32
bool CreateDirectories(std::string path)
{
    struct stat st;

    for( std::string::iterator iter = path.begin() ; iter != path.end(); )
    {
         std::string::iterator newIter = std::find(iter,path.end(),'\\');
         std::string newPath = std::string( path.begin(), newIter);

         if( stat( newPath.c_str(), &st) != 0)
         {
             if( mkdir( newPath.c_str(), 0x777) != 0 && errno != EEXIST )
             {
                return false;
             }
         }
         else
            if( !S_ISDIR(st.st_mode) )
             {
                 errno = ENOTDIR;
                 return false;
             }


         iter = newIter;
         if( newIter != path.end() )
             ++ iter;
    }
    return true;
}

#endif



#endif /* DATAFUNCTIONS_H_ */

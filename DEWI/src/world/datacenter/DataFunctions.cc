/*
 * DataFunctions.cc
 *
 *  Created on: 20 Oct 2014
 *      Author: R00118979
 */

#include "DataFunctions.h"

bool createDirectories(std::string path)
{
#ifdef linux
    struct stat st;

    for( std::string::iterator iter = path.begin() ; iter != path.end(); )
    {
         std::string::iterator newIter = std::find( iter, path.end(), '/' );
         std::string newPath = std::string( path.begin(), newIter);

         if( stat( newPath.c_str(), &st) != 0)
         {
             if( mkdir( newPath.c_str()) != 0 && errno != EEXIST )
             {
                return false;
             }
         }
         else
         {
            if( !S_ISDIR(st.st_mode) )
             {
                 errno = ENOTDIR;
                 return false;
             }
         }


         iter = newIter;
         if( newIter != path.end() )
         {
             ++ iter;
         }
    }
    return true;
#elif WIN32
    struct stat st;

    for( std::string::iterator iter = path.begin() ; iter != path.end(); )
    {
         std::string::iterator newIter = std::find(iter,path.end(),'\\');
         std::string newPath = std::string( path.begin(), newIter);

         if( stat( newPath.c_str(), &st) != 0)
         {
             if( mkdir( newPath.c_str()) != 0 && errno != EEXIST )
             {
                //return false;
             }
         }
         else
         {
            if( !S_ISDIR(st.st_mode) )
             {
                 errno = ENOTDIR;
                 return false;
             }
         }


         iter = newIter;
         if( newIter != path.end() )
         {
             ++ iter;
         }
    }
    return true;
#endif
}





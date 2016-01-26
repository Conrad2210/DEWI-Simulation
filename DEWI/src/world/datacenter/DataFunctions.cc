/*
 * DataFunctions.cc
 *
 *  Created on: 20 Oct 2014
 *      Author: R00118979
 */

#include "DataFunctions.h"

//    #ifndef WITH_LINUX
//        #define WITH_LINUX
//    #endif


#ifdef WIN32
    #ifndef WITH_WIN
        #define WITH_WIN
    #endif
#endif

bool createDirectories(std::string path)
{
#ifdef WITH_LINUX
    struct stat st;

    for( std::string::iterator iter = path.begin() ; iter != path.end(); )
    {
         std::string::iterator newIter = std::find( iter, path.end(), '/' );
         std::string newPath = std::string( path.begin(), newIter);

         if( stat( newPath.c_str(), &st) != 0)
         {
             if( mkdir( newPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0 && errno != EEXIST )
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
#ifdef WITH_WIN
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
return false;
}





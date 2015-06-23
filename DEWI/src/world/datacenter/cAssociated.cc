/*
 * cAssociated.cpp
 *
 *  Created on: 22 Jun 2015
 *      Author: R00118979
 */

#include <cAssociated.h>

cAssociated::cAssociated()
{
    index = -1;
    name = "";
    associated = false;

}

cAssociated::cAssociated(int ix, const char* na, bool as, int st, int parix, const char* parna)
{
    index = ix;
    name = na;
    associated = as;
    stage = st;
    parentIndex = parix;
    parentName = parna;

}

cAssociated::~cAssociated()
{

}


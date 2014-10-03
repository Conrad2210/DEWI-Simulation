/*
 * macTimeslotTemplateAccess.h
 *
 *  Created on: 2 May 2014
 *      Author: R00118979
 */

#ifndef MACTIMESLOTTABLEACCESS_H_
#define MACTIMESLOTTABLEACCESS_H_

/*
 * @author: Conrad Dandelski
 * Copyright(c) 2014: Conrad Dandelski
 * Copyright(c) 2014: Nimbus Centre for Embedded Systems Research, Cork, Ireland
 * Copyright(c) 2014: Cork Institute of Technology, Cork, Ireland
 */

#include "INETDefs.h"

#include "ModuleAccess.h"
#include "IMacTimeslotTable.h"

/*
 * Gives access to the IMacTimeslotTable
 */

class INET_API MacTimeslotTableAccess : public ModuleAccess<IMacTimeslotTable>
{
    public:
        MacTimeslotTableAccess() : ModuleAccess<IMacTimeslotTable>("timeslotTable") {}
};



#endif /* MACTIMESLOTTEABLEACCESS_H_ */

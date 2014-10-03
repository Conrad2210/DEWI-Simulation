/*
 * macTimeslotTemplateAccess.h
 *
 *  Created on: 2 May 2014
 *      Author: R00118979
 */

#ifndef MACTIMESLOTTEMPLATEACCESS_H_
#define MACTIMESLOTTEMPLATEACCESS_H_

/*
 * @author: Conrad Dandelski
 * Copyright(c) 2014: Conrad Dandelski
 * Copyright(c) 2014: Nimbus Centre for Embedded Systems Research, Cork, Ireland
 * Copyright(c) 2014: Cork Institute of Technology, Cork, Ireland
 */

#include "INETDefs.h"

#include "ModuleAccess.h"
#include "IMacTimeslotTemplate.h"

/*
 * Gives access to the IMacTimeslotTemplate
 */

class INET_API MacTimeslotTemplateAccess : public ModuleAccess<IMacTimeslotTemplate>
{
    public:
        MacTimeslotTemplateAccess() : ModuleAccess<IMacTimeslotTemplate>("timeslotTemplate") {}
};



#endif /* MACTIMESLOTTEMPLATEACCESS_H_ */

/*
 * macHoppingSequenceListAccess.h
 *
 *  Created on: 02. June 2014
 *      Author: R00118963
 */

/*
 * @author: Stefan Reis
 * Copyright(c) 2014: Stefan Reis
 * Copyright(c) 2014: Nimbus Centre for Embedded Systems Research, Cork, Ireland
 * Copyright(c) 2014: Cork Institute of Technology, Cork, Ireland
 */

#ifndef MACHOPPINGSEQUENCELISTACCESS_H_
#define MACHOPPINGSEQUENCELISTACCESS_H_


#include "INETDefs.h"

#include "ModuleAccess.h"
#include "IMacHoppingSequenceList.h"

/*
 * Gives access to the IMacHoppingSequenceList
 */
class INET_API MacHoppingSequenceListAccess : public ModuleAccess<IMacHoppingSequenceList>
{
    public:
        MacHoppingSequenceListAccess() : ModuleAccess<IMacHoppingSequenceList>("hoppingSequenceList") {}
};

#endif /* MACHOPPINGSEQUENCELISTACCESS_H_ */

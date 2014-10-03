/*
 * macSlotframeTableAccess.h
 *
 *  Created on: 2 May 2014
 *      Author: R00118979
 */

#ifndef MACSLOTFRAMETABLEACCESS_H_
#define MACSLOTFRAMETABLEACCESS_H_

/*
 * @author: Conrad Dandelski
 * Copyright(c) 2014: Conrad Dandelski
 * Copyright(c) 2014: Nimbus Centre for Embedded Systems Research, Cork, Ireland
 * Copyright(c) 2014: Cork Institute of Technology, Cork, Ireland
 */

#include "INETDefs.h"
#include "ModuleAccess.h"
#include "macSlotframeTable.h"


/*
 * Gives access to IMacSlotframeTable
 */

class INET_API MacSlotframeTableAccess : public ModuleAccess<IMacSlotframeTable>
{
    public:
        MacSlotframeTableAccess(): ModuleAccess<IMacSlotframeTable>("slotframeTable"){}
};
#endif /* MACSLOTFRAMETABLEACCESS_H_ */

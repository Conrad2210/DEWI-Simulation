/*
 * macNeighborTableAccess.h
 *
 *  Created on: 11 August 2014
 *      Author: R00118963
 */

#ifndef MACNEIGHBORTABLEACCESS_H_
#define MACNEIGHBORTABLEACCESS_H_

/*
 * @author: Stefan Reis
 * Copyright(c) 2014: Stefan Reis
 * Copyright(c) 2014: Nimbus Centre for Embedded Systems Research, Cork, Ireland
 * Copyright(c) 2014: Cork Institute of Technology, Cork, Ireland
 */

#include "INETDefs.h"
#include "ModuleAccess.h"
#include "macNeighborTable.h"


/*
 * Gives access to IMacNeighborTable
 */

class INET_API MacNeighborTableAccess : public ModuleAccess<IMacNeighborTable>
{
    public:
        MacNeighborTableAccess(): ModuleAccess<IMacNeighborTable>("neighborTable"){}
};
#endif /* MACNEIGHBORTABLEACCESS_H_ */

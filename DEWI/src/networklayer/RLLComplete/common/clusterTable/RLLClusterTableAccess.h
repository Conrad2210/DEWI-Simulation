/*
 * RLLClusterTableAccess.h
 *
 *  Created on: 10 Jun 2015
 *      Author: R00118979
 */

#ifndef RLLCLUSTERTABLEACCESS_H_
#define RLLCLUSTERTABLEACCESS_H_



#include "INETDefs.h"
#include "ModuleAccess.h"
#include "IRLLClusterTable.h"


/*
 * Gives access to IRLLClusterTable
 */

class INET_API RLLClusterTableAccess : public ModuleAccess<IRLLClusterTable>
{
    public:
	RLLClusterTableAccess(): ModuleAccess<IRLLClusterTable>("clusterTable"){}
};

#endif /* RLLCLUSTERTABLEACCESS_H_ */

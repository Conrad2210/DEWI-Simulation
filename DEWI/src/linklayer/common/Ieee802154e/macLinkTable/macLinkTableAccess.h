/*
 * macLinkTableAccess.h
 *
 *  Created on: 2 May 2014
 *      Author: R00118979
 */

#ifndef MACLINKTABLEACCESS_H_
#define MACLINKTABLEACCESS_H_


/*
 * @author: Conrad Dandelski
 * Copyright(c) 2014: Conrad Dandelski
 * Copyright(c) 2014: Nimbus Centre for Embedded Systems Research, Cork, Ireland
 * Copyright(c) 2014: Cork Institute of Technology, Cork, Ireland
 */
#include "INETDefs.h"

#include "ModuleAccess.h"
#include "ImacLinkTable.h"

/*
 * Gives  access to the ImacLinkTable.
 */
class INET_API MacLinkTableAccess : public ModuleAccess<IMacLinkTable>
{
    public:
        MacLinkTableAccess() : ModuleAccess<IMacLinkTable>("linkTable"){}
};



#endif /* MACLINKTABLEACCESS_H_ */

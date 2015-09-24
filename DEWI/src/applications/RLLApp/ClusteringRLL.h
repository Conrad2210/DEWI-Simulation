//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef CLUSTERINGRLL_H_
#define CLUSTERINGRLL_H_

#include "RLLApp.h"

class INET_API ClusteringRLL: public RLLApp
{
public:
	ClusteringRLL();
	virtual ~ClusteringRLL();
	virtual void initialize(int);
	virtual void finish();

	virtual int numInitStages() const
	{
		return 5;
	}

protected:
	virtual void checkAssociation();
};

#endif /* CLUSTERINGRLL_H_ */

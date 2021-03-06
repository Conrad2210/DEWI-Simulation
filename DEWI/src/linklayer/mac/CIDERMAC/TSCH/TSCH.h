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

#ifndef TSCH_H_
#define TSCH_H_

#include <Ieee802154eMac.h>

class TSCH: public Ieee802154eMac
{
	public:
		TSCH();
		virtual ~TSCH();

		virtual void initialize(int stage);
};

#endif /* TSCH_H_ */

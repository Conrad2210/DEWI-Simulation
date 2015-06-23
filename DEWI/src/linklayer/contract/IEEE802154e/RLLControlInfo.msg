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

cplusplus {{
    using namespace std;
    #include "Ieee802154eField.h"
    #include "Ieee802154eEnum.h"
}}
enum MACenum;
message RLLControlInfo
{
    bool 	 toParent;
    string   destName;
    
   	uint8_t 	dstAddrMode; 
   	int64_t 	dstAddr;
   	uint8_t 	srcAddrMode; 
   	int64_t 	srcAddr;
   	
   	bool panCoordinator;
   	
   	uint64_t deviceAddress;
   	
    short status  @enum(MACenum);
    uint16_t channelOffset;
    uint16_t assocShortAddress;
    
    uint8_t beaconType;
    uint8_t channel;
    uint8_t beaconOrder;
    uint8_t superframeOrder;
    
    uint32_t startTime;
    uint8_t scanType;
    
    
    
    
}
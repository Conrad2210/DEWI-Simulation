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
    #include "Ieee802154eConst.h"
    #include "Ieee802154eDef.h"
    #include "Ieee802154eEnum.h"
    #include "Ieee802154eField.h"
}}

class noncobject MACAddress;

struct AuxiliarySecurityHeader;

struct FrameCtrl;

struct IE_Header;

struct IE_Payload;

struct MPFrameCtrl;

enum Ieee802154eMacCmdType;

enum MACenum;

struct GTSFields;

struct SuperframeSpec;

struct DevCapability;
struct PendingAddrFields;

packet Ieee802154eBasicFrame
{
    unsigned char bdsn;
    
    AuxiliarySecurityHeader auxSecHd;
}

message Ieee802154eFrame extends Ieee802154eBasicFrame
{
    //	MHR
    //	Frame control field (FCF)(16 bits) - Fig 35
    FrameCtrl frmCtrl;
	    //	Sequence number field (8 bits)
    unsigned char seqNmbr;
    //	Addressing fields
    uint16_t dstPanId;
    MACAddress dstAddr;
    uint16_t srcPanId;
    MACAddress srcAddr;
    
    uint8_t retries;
    IE_Header ieHeader[];
    IE_Payload iePayload[];
}

message Ieee802154eBeaconFrame extends Ieee802154eFrame
{
    // MAC payload in beacon frames
    SuperframeSpec sfSpec;		// Superframe Specification
    
    // GTS information fields
	GTSFields gtsField;

	// Pending Address Information field
	PendingAddrFields pendingAdrrFld;
}

// *****************************************
// Enhanced Beacon frame format - Std 802.15.4e-2012 (figure 40a) page 64
// *****************************************
message Ieee802154EnhancedBeaconFrame extends Ieee802154eFrame
{
}

// ********************************
// Data frame format - Std 802.15.4e-2012 (figure 46) page 65
// ********************************
message Ieee802154eDataFrame extends Ieee802154eFrame
{  
}

// ******************************		
// Acknowledgment frame format - Std 802.15.4-2011 (figure 47) page 65
// ******************************
message Ieee802154eAckFrame extends Ieee802154eFrame
{
}

// ******************************		
// Enhanced Acknowledgement frame format - Std 802.15.4e-2012 (figure 47a) page 66
// ******************************
message Ieee802154eEnhancedAckFrame extends Ieee802154eFrame
{   
}

// ********************************				
// MAC command frame format - Std 802.15.4e-2012 (figure 48) page 67
// ********************************
message Ieee802154eCmdFrame extends Ieee802154eFrame
{
    // MAC Payload
    int cmdType @enum(Ieee802154eMacCmdType);		// Commend Frame Identifier
    
    // Command Payload (variable) 
}

packet Ieee802154eMPFrame extends Ieee802154eFrame
{
    //	MHR
    //	Frame control field (FCF)(16 bits) (figure 48k)
    MPFrameCtrl mpFrmCtrl;

	//	MFR
	//	FCS is not modeled and only included in the length (2 bytes)
}
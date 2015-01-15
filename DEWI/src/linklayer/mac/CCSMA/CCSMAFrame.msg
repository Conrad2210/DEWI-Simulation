cplusplus {{
#include "Ieee802154Frame_m.h";
}}

// The common part of 802.15.4 frames
// MHR (header) and MFR (FCS)
class Ieee802154Frame;
packet CCSMAFrame extends Ieee802154Frame
{
    //	MHR
    //	Frame control fieled (FCF)(16 bits) - Fig 35
	int HopCount;

		//	MFR
		//	FCS is not modeled and only included in the length (2 bytes)
}


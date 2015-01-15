//***************************************************************************
//* file:        CSMA802154.ned
//*
//* author:      Jerome Rousselot, Marc Loebbers
//* 
//* copyright:   (C) 2007-2009 CSEM SA, Neuchatel, Switzerland.
//*		 		 (C) 2004 Telecommunication Networks Group (TKN) at
//*              Technische Universitaet Berlin, Germany.
//*
//*              This program is free software; you can redistribute it
//*              and/or modify it under the terms of the GNU General Public
//*              License as published by the Free Software Foundation; either
//*              version 2 of the License, or (at your option) any later
//*              version.
//*              For further information see file COPYING
//*              in the top level directory
//*
//* Funding: This work was partially financed by the European Commission under the  
//* Framework 6 IST Project "Wirelessly Accessible Sensor Populations"  
//* (WASP) under contract IST-034963.
//***************************************************************************
//* part of:    Modifications to the MF-2 framework by CSEM
//**************************************************************************/
package dewi.linklayer.mac.CCSMA;
import dewi.linklayer.mac.CSMA.CSMA802154;
//
// IEEE 802.15.4-2006 non-beacon enabled CSMA protocol
//
simple CCSMA extends CSMA802154
{
    parameters:
    @class(ccsma);
    bool isLightSwitch = default(false);
    volatile double reBroadcastDelay @unit(s) = default(0s);
        
}

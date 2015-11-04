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
package dewi.linklayer.mac.CSMA;

//
// IEEE 802.15.4-2006 non-beacon enabled CSMA protocol
//
simple CSMA802154
{
    parameters:
        @class(csma802154);
        bool debug = default(false);
        bool stats = default(true);
        bool trace = default(false);
        bool randomBackoff = default(false);
        
        string address=default ("auto");
        string queueModule;				// name of optional external queue module
        bool useMACAcks = default(true);
        int queueLength = default(100);
        // simple interframe space. Time to wait between receiving a frame and acknowledging it.
        // Should be bigger than the maximum time for switching between Tx and Rx at the receiver.      
        double sifs @unit(s) = default(0.000192 s); // 12 symbols


        double macMaxCSMABackoffs = default(5);
        // maximum number of frame retransmission
        double macMaxFrameRetries = default(3);        
        // time to wait for an acknowledgement after transmitting a unicast frame.
        double macAckWaitDuration @unit(s) = default(0.00056 s);  // 1+12+10+12 symbols
        // Clear Channel Assessment detection time		
        double ccaDetectionTime @unit(s) = default(0.000128 s); // 8 symbols
        // Time to setup radio to reception state
        double rxSetupTime @unit(s);
        // Time to switch radio from Rx to Tx state
        double aTurnaroundTime @unit(s) = default(0.000192 s);    // 12 symbols
        double aUnitBackoffPeriod @unit(s) = default(0.00032 s);
        // complete MAC ack message length (in bits) 
        // (! headerLength is not added to this)
        double ackLength @unit(bit) = default(40 bit);
        double headerLength @unit(bit) = default(216 bit);   

        //backoff method to use. constant, linear or exponential
        string backoffMethod = default("exponential");
        // base unit for all backoff calculations
        // # of backoff periods of the initial contention window
        // (for linear and constant backoff method only)
        int contentionWindow = default(2);
        // minimum backoff exponent (for exponential backoff method only)
        double macMinBE = default(3);
        // maximum backoff exponent (for exponential backoff method only)
        double macMaxBE = default(8);
		// debug switch
    // whether is PAN coordinator
    bool isPANCoor = default(false);				// whether is PAN coordinator
    // beacon order [0, 15] - default 15 (no beacon) - Std 802.15.4-2011 (table 52) page 127 - see 5.1.1.1
    double BO = default(8);					// beacon order [0, 15]
    // superframe order [0, BO) - default 15 (no beacon) - Std 802.15.4-2011 (table 52) page 128 - see 5.1.1
    double SO = default(4);					// superframe order [0, BO)
    double dataTransMode = default(1);		// 1: direct; 2: indirect; 3: GTS
    // name of optional external queue module
// name of optional external queue module
    string panCoorName = default("host[0]");			// name of the PAN coordinator

    bool ack4Data = default(true);
    bool setRxOnWhenIdle = default(true);

    // switch for the network messages
    bool useIeee802Ctrl = default(false);

    // switch for the MAC Metrics - Std 802.15.4e-2012 (6.4.3.9 MAC-performance-metrics-specific MAC PIB attributes) page 182
    bool useMetrics = default(true);

    // switch for the Std 802.15.4e-2014 TSCH mechanism
    bool TSCHcapable = default(true);
    bool useTSCH = default(false);
    // use the TSCH without a beacon from the PAN coordinator (Note: devices will be not associated with coordinator)
        bool useTSCHdirect = default(false);
    // for the Std 802.15.4e-2014 TSCH mechanism - to set the Node as a timeScource
    bool timeSource = default(false);
    // for the Std 802.15.4e-2014 TSCH mechanism - to use or not the CCA during the TSCH CCA transmission
    bool useCCA = default(true);
    // for the Std 802.15.4e-2014 TSCH mechanism - to set the default timeslotID before the first beacon received
    int useTimeslotID = default(0);

    // for the Std 802.15.4e-2014 TSCH mechanism - to set the default hopping sequence ID before the first beacon received
    int useHoppingSequenceID = default(0);

    int maxRetries = default(3); // default = 3; range = 0-7


    bool readLinksFromFile = default(false);
    string LinkTableFileName = default("linkTable");
    string LinkTableFilesFolder = default("");
    bool readSlotframesFromFile = default(false);
    string SlotframeTableFileName = default("slotframeTable");
    string SlotframeTableFilesFolder = default("");
    bool readTemplatesFromFile = default(false);
    string TemplateTableFileName = default("templateTable");
    string TemplateTableFilesFolder = default("");
    string macSlotframeTableModule = default("^.^.slotframeTable");
    string macLinkTableModule = default("^.^.linkTable");
    string macTimeslotTableModule = default("^.^.timeslotTemplate");
    string macHoppingSequenceListModule = default("^.^.hoppingSequenceList");
    string macNeighborTableModule = default("^.^.neighborTable");
    int numHosts;
        
     gates:
        input upperLayerIn; 			// from the upper layer
        output upperLayerOut; 		// to the upper layer
        input lowerLayerIn; 			// from the physical layer
        output lowerLayerOut; 			// to the physical layer
        
}

cplusplus {{
#include "Ieee802154eConst.h"
#include "Ieee802154eEnum.h"
#include "Ieee802154eField.h"
}}


enum PHYenum;

struct PHY_PIB;

message Ieee802154eMacPhyPrimitives
{
    short status @enum(PHYenum);
    uint8_t pibAttribute;
    PHY_PIB pibAttributeValue;
    
    // for PD-DATA.request
    uint8_t psduLength;   
          
    // FIXME: do we need this? [SR]
    double bitRate;
    short additional=-1; 

    unsigned char energyLevel;

    int channelNumber; 		// the current radio channel
		//unsigned int	phyChannelsSupported; 
		//double		phyTransmitPower;
		//unsigned char	phyCCAMode;
		
}

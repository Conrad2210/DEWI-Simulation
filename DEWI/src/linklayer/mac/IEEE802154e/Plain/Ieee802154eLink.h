
#ifndef IEEE_802154_LINK_H
#define IEEE_802154_LINK_H

#include "Ieee802154eDef.h"

#define hl_oper_del 1
#define hl_oper_est 2
#define hl_oper_rpl 3


// for pkt duplication detection
class HLISTLINK
{
  public:
	IE3ADDR hostID;     // source address
    UINT_8 SN;              //SN of packet last received
    HLISTLINK *last;
    HLISTLINK *next;
    HLISTLINK(MACAddress hostid, uint16_t sn)
    {
        hostID = hostid;
        SN = sn;
        last = 0;
        next = 0;
    }
};

int     addHListLinke        (HLISTLINK **hlistLink1, HLISTLINK **hlistLink2, IE3ADDR hostid, UINT_8 sn);
int     updateHListLinke     (int oper, HLISTLINK **hlistLink1, HLISTLINK **hlistLink2, IE3ADDR hostid, UINT_8 sn = 0);
int     chkAddUpdHListLinke  (HLISTLINK **hlistLink1, HLISTLINK **hlistLink2, IE3ADDR hostid, UINT_8 sn);
void    emptyHListLinke      (HLISTLINK **hlistLink1, HLISTLINK **hlistLink2);
void    dumpHListLinke       (HLISTLINK *hlistLink1, IE3ADDR hostid);

// for device association

// for transaction (indirect transmission)
#endif

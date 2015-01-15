//
// Generated file, do not edit! Created by nedtool 4.6 from src/networklayer/contract/IPProtocolId.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "IPProtocolId_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("IPProtocolId");
    if (!e) enums.getInstance()->add(e = new cEnum("IPProtocolId"));
    e->insert(IP_PROT_ICMP, "IP_PROT_ICMP");
    e->insert(IP_PROT_IGMP, "IP_PROT_IGMP");
    e->insert(IP_PROT_IP, "IP_PROT_IP");
    e->insert(IP_PROT_TCP, "IP_PROT_TCP");
    e->insert(IP_PROT_EGP, "IP_PROT_EGP");
    e->insert(IP_PROT_IGP, "IP_PROT_IGP");
    e->insert(IP_PROT_UDP, "IP_PROT_UDP");
    e->insert(IP_PROT_XTP, "IP_PROT_XTP");
    e->insert(IP_PROT_IPv6, "IP_PROT_IPv6");
    e->insert(IP_PROT_RSVP, "IP_PROT_RSVP");
    e->insert(IP_PROT_IPv6_ICMP, "IP_PROT_IPv6_ICMP");
    e->insert(IP_PROT_NONE, "IP_PROT_NONE");
    e->insert(IP_PROT_OSPF, "IP_PROT_OSPF");
    e->insert(IP_PROT_PIM, "IP_PROT_PIM");
    e->insert(IP_PROT_SCTP, "IP_PROT_SCTP");
    e->insert(IP_PROT_DSR, "IP_PROT_DSR");
    e->insert(IP_PROT_MANET, "IP_PROT_MANET");
    e->insert(IP_PROT_IPv6EXT_HOP, "IP_PROT_IPv6EXT_HOP");
    e->insert(IP_PROT_IPv6EXT_DEST, "IP_PROT_IPv6EXT_DEST");
    e->insert(IP_PROT_IPv6EXT_ROUTING, "IP_PROT_IPv6EXT_ROUTING");
    e->insert(IP_PROT_IPv6EXT_FRAGMENT, "IP_PROT_IPv6EXT_FRAGMENT");
    e->insert(IP_PROT_IPv6EXT_AUTH, "IP_PROT_IPv6EXT_AUTH");
    e->insert(IP_PROT_IPv6EXT_ESP, "IP_PROT_IPv6EXT_ESP");
    e->insert(IP_PROT_IPv6EXT_MOB, "IP_PROT_IPv6EXT_MOB");
);



#ifndef _CS_TICKET_
#define _CS_TICKET_

#include "..\_network_module\Generals.h"
#include "..\_network_module\CSPacket.h"
#include "..\_network_module\CSServer.h"
#include <deque>
#include <mutex>

struct ticket {

    CSPacket pct;
    SOCKET client;

};

class CSTickets {

    std::mutex TP;
    std::deque<ticket> tick;

public:

    void add(SOCKET *client, CSPacket* pct) {
    
        std::lock_guard<std::mutex> lg(TP);
        tick.push_back( { *pct, *client } );
    
    }

    ticket getTicket() {
    
        std::lock_guard<std::mutex> lg(TP);
        ticket tmp = tick.front();
        tick.pop_front();

        return tmp;
    }

    int size() { return tick.size(); }

};


#endif //_CS_TICKET_
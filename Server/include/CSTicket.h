#ifndef _CS_TICKET_
#define _CS_TICKET_

#include "..\..\include\Generals.h"
#include "..\..\include\CSPacket.h"
#include "..\..\include\CSServer.h"
#include <deque>
#include <mutex>

struct ticket {

    CSPacket pct;
    SOCKET client = INVALID_SOCKET;

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
        
        if(tick.empty()){           
        
            ticket tmp;
            return tmp;
            
        }
        
        ticket tmp = tick.front();
        tick.pop_front();

        return tmp;
    }

    int size() { 
    
        std::lock_guard<std::mutex> lg(TP);
        return tick.size(); 
    
    }

};


#endif //_CS_TICKET_
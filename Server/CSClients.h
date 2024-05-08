#ifndef _CS_CLIENT_
#define _CS_CLIENT_

#include "..\_network_module\Generals.h"
#include "..\_network_module\CSServer.h"
#include <vector>
#include <mutex>

struct Client {

    SOCKET client;
    char name[3] = "";

};

class CSClients {

    std::mutex CP;
    std::vector<Client> clients;
    std::vector<Client> tmp;

    std::atomic_bool change = false;

    int largest_sock = 0;

    void coppy() {
    
        std::lock_guard<std::mutex> lg(CP);
        tmp = clients;

        change = false;
    
    }

public:

    void add(SOCKET client, char name[3]) {
    
        Client tmp;
        tmp.client = client;
        toCSTR(name, tmp.name, 3);


        std::lock_guard<std::mutex> lg(CP);
        clients.push_back( std::move(tmp) );

        change = true;
    
    }

    void forSelect(fd_set* set) {

        if(change) {
            coppy();
        }

        for (auto it = tmp.begin(); it != tmp.end(); ++it) {
        
            FD_SET(it->client, set);

            largest_sock = it->client > largest_sock ? it->client : largest_sock;
        
        }    
    }

    int getlargestSocket() { return largest_sock; }

    Client find(const char name[3] = "") {

        if (change) {
            coppy();
        }

        for (auto it = tmp.begin(); it != tmp.end(); ++it) {  

            if (!CStrCmp(it->name, name))
                return *it;

        }

        return { INVALID_SOCKET, ""};
    }

    Client find(SOCKET client) {
    
        if (change) {
            coppy();
        }

        for (auto it = tmp.begin(); it != tmp.end(); ++it) {

            if (it->client == client)
                return *it;

        }
        
        return { INVALID_SOCKET, "" };    
    }

    void del(SOCKET connectClient) {
    
        std::lock_guard<std::mutex> lg(CP);
        for (auto it = clients.begin(); it != clients.end(); ++it) {

            if (it->client == connectClient) {
            
                clients.erase(it);
                change = true;
                return;

            } 
        }
    }

    int getSize() {
    
        std::lock_guard<std::mutex> lg(CP);
        return clients.size();
    
    }

};

#endif //_CS_CLIENT_
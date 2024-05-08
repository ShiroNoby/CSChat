#pragma


#include <thread>

#include "../_network_module/CSServer.h"
#include "../Server/CSClients.h"
#include "../Server/CSTicket.h"
#include "../_network_module/CSReciver.h"

//"A-Z" & "0-9"
char tmpName[3] = "A0";
void init(char* name) {

    ++tmpName[1];
    if (tmpName[1] > '9'){
        ++tmpName[0];
        tmpName[1] = '0';
    }
    if (tmpName[0] >= 'Z') {
        tmpName[0] = 'A';
        tmpName[1] = '0';
    }

    toCSTR(tmpName, name, 3);

}

int main(int argc, char **argv) { //argg

    if (argc < 3) { logExit("invalid(not enough) argument"); } //log

    CSServer server;
    if(server.Create(argv[1], argv[2])) logExit("bad server create");

    CSClients clientsPool;
    CSTickets ticketsPool;

    auto listen = [&]() {

        std::cout << "CSListen start" << std::endl;

        CSReciver ResList;

        while (1) {

            server.Listen();

            CSPacket pct;
            ResList.recv(server.client, &pct);

            if (!(pct.type == Type::cmd && ((*(Packet::command*)&pct.data).cmd == Packet::Cmd::initDevice))) {

                server.Disconect(server.client);
                continue;
            }

            init(pct.data);
            pct.type = Type::initDevice;
            ResList.send(server.client, &pct);

            clientsPool.add(server.client, pct.data);

            server.client = INVALID_SOCKET;

        }

        std::cout << "CSListen stop" << std::endl;

    };
    auto recv   = [&]() {

        std::cout << "CSRecv start" << std::endl;

        CSReciver ResRecv;
        fd_set checkList;

        struct timeval  tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        while (1) {

            if (!clientsPool.getSize()) continue;
            
            FD_ZERO(&checkList);
            clientsPool.forSelect(&checkList);
            int result = select(clientsPool.getlargestSocket(), &checkList, NULL, NULL, &tv);

            if (result == -1) { 
                std::cout << "Select error: " << WSAGetLastError() << std::endl;
                logExit("bad select");
                continue; 
            }
            if (!result) continue;

            for (int index = 0; index < checkList.fd_count; ++index) {

                //check client valid?
                if(clientsPool.find(checkList.fd_array[index]).client == INVALID_SOCKET) continue;

                CSPacket pct;
                ResRecv.recv(checkList.fd_array[index], &pct);
                ticketsPool.add(&checkList.fd_array[index], &pct);

            }
        }

        std::cout << "CSRecv stop" << std::endl;

    };

    //tread1(func1()) -> listen()
    //tread2(func2()) -> recv()
    std::thread CSListen(listen);
    std::thread CSRecv(recv);

    CSListen.detach();
    CSRecv.detach();

    CSReciver ResMain;

    std::cout << "main start" << std::endl;

    //main - parser
    while (1) {
    
        if (!ticketsPool.size()) continue;

        //check client valid?
        ticket tckt = ticketsPool.getTicket();
        if (clientsPool.find(tckt.client).client == INVALID_SOCKET) continue;

        // Msg = 0,
        // cmd,
        // HardPacket,
        switch (tckt.pct.type) {
        
            case Type::Msg: std::cout << "type::msg" << std::endl;
            case Type::HardPacket: {

                std::cout << "type::HardPacket" << std::endl;

                Client cl = clientsPool.find(tckt.pct.header.otherName);
                if (cl.client == INVALID_SOCKET) {
                
                    std::cout << "client not found: " << tckt.pct.header.otherName << std::endl;

                    CSPacket pct;
                    toCSTR("client not found", pct.data, 17);

                    Header header;
                    CStr("SE", header.myName, 2);
                    CStr(tckt.pct.header.myName, header.otherName, 2);

                    pct.header = header;
                    ResMain.send(tckt.client, &pct);
                
                    break;
                }

                if (!CStrCmp(tckt.pct.data, " ")) {
                    
                    std::cout << "pct is empty" << std::endl;
                    continue;
                
                }

                ResMain.send(cl.client, &tckt.pct);

                break;
            }
            case Type::disconnect: {
            
                std::cout << "type::disconnect" << std::endl;

                clientsPool.del(tckt.client);
                server.Disconect(tckt.client);
                //tmpName--

                break;            
            }
        
            default: { continue; }
        }
    }

    std::cout << "main stop" << std::endl;

}
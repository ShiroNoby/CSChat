#include <string>
#include <thread>

#include "CSServer.h"
#include "CSReciver.h"
#include "CSClients.h"
#include "CSTicket.h"

#include "CSLog.h"

//"A-Z" & "0-9"
char tmpName[3] = "A0";
void init(char* name) {

    ++tmpName[1];
    if (tmpName[1] > '9') {
        ++tmpName[0];
        tmpName[1] = '0';
    }
    if (tmpName[0] >= 'Z') {
        tmpName[0] = 'A';
        tmpName[1] = '0';
    }

    toCSTR(tmpName, name, 3);

}

int main(int argc, char** argv) { //argg

    CSLog logMain("log.txt");
    CSLog loglisten("log.txt");
    CSLog logrecv("log.txt");
        
    logMain.clearfile();
    loglisten.clearfile();
    logrecv.clearfile();          
    
    logMain.SystemLog(tier0, __FILE__, __LINE__, "server start");
    if (argc < 3) logMain.SystemLog(tier3, __FILE__, __LINE__, "ERROR: invalid(not enough) argument");

    logMain.SystemLog(tier0, __FILE__, __LINE__, "create server");
    CSServer server;
    if (server.Create(argv[1], argv[2])) logMain.SystemLog(tier3, __FILE__, __LINE__, "ERROR: bad server create");

    logMain.SystemLog(tier0, __FILE__, __LINE__, "create pools");
    CSClients clientsPool;
    CSTickets ticketsPool;

    auto listen = [&]() {

        loglisten.SystemLog(tier1, __FILE__, __LINE__, "server listen now");
        CSReciver ResList;

        while (1) {

            server.Listen();

            loglisten.SystemLog(tier1, __FILE__, __LINE__, "new client");
            CSPacket pct;
            ResList.recv(server.client, &pct);

            if (!(pct.type == Type::cmd && ((*(Packet::command*)&pct.data).cmd == Packet::Cmd::initDevice))) {

                loglisten.SystemLog(tier1, __FILE__, __LINE__, "new client not init-> disconnect");
                server.Disconect(server.client);
                continue;
            }

            loglisten.SystemLog(tier1, __FILE__, __LINE__, "init client");
            init(pct.data);

            loglisten.SystemLog(tier1, __FILE__, __LINE__, "send client name");
            pct.type = Type::initDevice;
            ResList.send(server.client, &pct);

            loglisten.SystemLog(tier1, __FILE__, __LINE__, "add client in pool");
            clientsPool.add(server.client, pct.data);

            server.client = INVALID_SOCKET;

        }

        loglisten.SystemLog(tier1, __FILE__, __LINE__, "server listen stop");

        };
    auto recv = [&]() {
        
        logrecv.SystemLog(tier2, __FILE__, __LINE__, "server recv start now");

        CSReciver ResRecv;
        fd_set checkList;

        struct timeval  tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        while (1) {

            if (!clientsPool.getSize()) continue;

            logrecv.SystemLog(tier2, __FILE__, __LINE__, "server select for recv");
            FD_ZERO(&checkList);
            clientsPool.forSelect(&checkList);
            int result = select(clientsPool.getlargestSocket(), &checkList, NULL, NULL, &tv);

            if (result == -1) {
                logrecv.SystemLog(tier3, __FILE__, __LINE__, "ERROR: Select error");
                //logExit(WSAGetLastError());
                logExit("ERROR: Select error");
                continue;
            }
            if (!result) continue;

            for (int index = 0; index < checkList.fd_count; ++index) {

                logrecv.SystemLog(tier2, __FILE__, __LINE__, "check client");
                if (clientsPool.find(checkList.fd_array[index]).client == INVALID_SOCKET) continue;

                CSPacket pct;

                logrecv.SystemLog(tier2, __FILE__, __LINE__, "recv client");
                ResRecv.recv(checkList.fd_array[index], &pct);

                logrecv.SystemLog(tier2, __FILE__, __LINE__, "add ticket in pool");
                ticketsPool.add(&checkList.fd_array[index], &pct);

            }
        }

        logrecv.SystemLog(tier2, __FILE__, __LINE__, "server recv stop");

        };

    logMain.SystemLog(tier0, __FILE__, __LINE__, "treads start");
    //tread1(func1()) -> listen()
    //tread2(func2()) -> recv()
    std::thread CSListen(listen);
    std::thread CSRecv(recv);

    CSListen.detach();
    CSRecv.detach();

    logMain.SystemLog(tier0, __FILE__, __LINE__, "create reciver");
    CSReciver ResMain;

    logMain.SystemLog(tier0, __FILE__, __LINE__, "start main loop");
    //main - parser
    while (1) {
    
        if (!ticketsPool.size()) continue;
        
        //check client valid?
        ticket tckt = ticketsPool.getTicket();
        if (clientsPool.find(tckt.client).client == INVALID_SOCKET) continue;
        logMain.SystemLog(tier0, __FILE__, __LINE__, "take valid ticket");

        // Msg = 0,
        // cmd,
        // HardPacket,
        switch (tckt.pct.type) {

        case Type::Msg:
        case Type::HardPacket: {

            logMain.SystemLog(tier0, __FILE__, __LINE__, "Type::HardPacket");
            logMain.SystemLog(tier0, __FILE__, __LINE__, "search name");
            Client cl = clientsPool.find(tckt.pct.header.otherName);
            if (cl.client == INVALID_SOCKET) {

                logMain.SystemLog(tier0, __FILE__, __LINE__, "client not found");
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

                logMain.SystemLog(tier0, __FILE__, __LINE__, "msg is empty");
                continue;

            }

            logMain.SystemLog(tier0, __FILE__, __LINE__, "retranslate msg");
            ResMain.send(cl.client, &tckt.pct);

            break;
        }
        case Type::disconnect: {

            logMain.SystemLog(tier0, __FILE__, __LINE__, "Type::disconnect");
            clientsPool.del(tckt.client);
            server.Disconect(tckt.client);

            break;
        }
        case Type::updateNameList: {

            logMain.SystemLog(tier0, __FILE__, __LINE__, "Type::updateNameList");
            std::string str;

            logMain.SystemLog(tier0, __FILE__, __LINE__, "get nameList");
            const std::vector<Client>& tmp = clientsPool.getNameList();

            for (auto a : tmp) {

                str += a.name;
                str += ';';

            }

            if (str.empty()) continue;
            
            int size = str.size();
            
            int pctCount = size / STRSIZE;
            
            if (size % STRSIZE)
                ++pctCount;

            ////////******************************************************send HRDPCT

            logMain.SystemLog(tier0, __FILE__, __LINE__, "send nameList");
            logMain.SystemLog(tier0, __FILE__, __LINE__, str.c_str());
            CSPacket                pct;

            Header                  header("SE", tckt.pct.header.myName);
            pct.header = header;

            Packet::hardPacket      HrdPct;
            std::vector<CSPacket>   pctBuf;

            HrdPct.countPacket = pctCount;
            pct.type = Type::updateNameList;
            toCSTR((const char*)&HrdPct, pct.data, sizeof(Packet::hardPacket));
            pctBuf.push_back(pct);

            int index = 0;

            for (; index < pctCount - 1; ++index) {

                CSPacket    pct;

                Header      header("SE", tckt.pct.header.myName);
                pct.header = header;

                toCSTR(str.data() + (index * STRSIZE), pct.data, STRSIZE);
                pctBuf.push_back(pct);

            }

            pct.type = Type::Msg;
            toCSTR(str.data() + (index * STRSIZE), pct.data, (size % STRSIZE));
            pctBuf.push_back(pct);

            for (index = 0; index <= pctCount; ++index)
                ResMain.send(tckt.client, &pctBuf[index]);

            break;
        }

        default: { continue; }
        }
    }

    logMain.SystemLog(tier0, __FILE__, __LINE__, "main loop stop");
    system("pause");
}
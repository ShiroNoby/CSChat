#ifndef _CS_CHAT_
#define _CS_CHAT_

#define PROCSTART while (1) { while (chat) {
#define PROCEND }}

#include "CSSuportCmd.h"
#include <string>
#include <vector>

char name[3] = "";
char otherName[3] = "";
char end[] = "end";
std::atomic_bool chat = false;
CSReciver rcv;

CSClient* m_client;

class CSChat { 

public:

    //static CSClient* m_client;

    
    CSChat(CSClient* client) {

        m_client = client;

        //CSPacket packet = ClientCMDZ(createLobby, m_client, name);

        //if (packet.type != Type::success) logExit("failed create chat lobby");

        chat = true;
    }

    static void recv() {
        
        //if(chating())
        //
        // while(1)
            // string
            // recv(string)
            // cout << otherName << " " << string
        //

        PROCSTART
        
        //////////*******************************\\\\\\\\\\recv

        CSPacket pct;
        int pctSize = sizeof(pct);

        int res = rcv.recv(m_client->ConnectSocket, &pct);
        std::cout << "recv: " << res << std::endl;

        //if (pct.type != Type::HardPacket || pct.type != Type::Msg) continue;

        if (!CStrCmp(pct.data, " ")) continue;

        if (!CStrCmp(pct.data, "client not found")) {

            std::cout << pct.data << std::endl;
            chat = false;
            break;
        
        }

        std::cout << pct.header.myName << ": ";

        if (pct.type == Type::HardPacket) {

            Packet::hardPacket hrdpct;
            CStr(pct.data, (char*)&hrdpct, sizeof(Packet::hardPacket));

            for (int index = 0; index < hrdpct.countPacket; ++index) {

                int res = rcv.recv(m_client->ConnectSocket, &pct);
                std::cout << pct.data;

            }

        }
        else std::cout << pct.data << std::endl;
                    
        PROCEND
    
    }

    static void imput() {
    
        //
        //if(chating())
        // string
        // geltine(string)
        // send(string)
        //

        PROCSTART

        //////////*******************************\\\\\\\\\\send

        std::string massage;

        bool valid = true;

        while (1) {

            std::string tmp;

            std::getline(std::cin, tmp);

            if (tmp.empty())
                break;

            if (!CStrCmp(tmp.c_str(), end)) {            
                valid = false;
                break;
            }

            tmp += '\n'; 
            massage += tmp;

        }

        if (!valid) { 
            chat = false; 
            break;
        }

        if (!massage.size()) continue;

        int massageSize = massage.size();
        int pctCount = massageSize / STRSIZE;

        if (pctCount)
            if (massageSize % STRSIZE)
                ++pctCount;

        if (!pctCount) { //refact

            CSPacket pct;

            Header header(name, otherName);
            pct.header = header;

            CStr(massage.data(), pct.data, (massageSize % STRSIZE));

            rcv.send(m_client->ConnectSocket, &pct);

        }
        else {

            CSPacket                pct;

            Header                  header(name, otherName);
            pct.header              = header;

            Packet::hardPacket      HrdPct;
            std::vector<CSPacket>   pctBuf;

            HrdPct.countPacket      = pctCount;
            pct.type                = Type::HardPacket;
            CStr((const char*)&HrdPct, pct.data, sizeof(Packet::hardPacket));
            pctBuf.push_back(pct);

            int index = 0;

            for (; index < pctCount - 1; ++index) {

                CSPacket    pct;

                Header      header(name, otherName);
                pct.header  = header;

                CStr(massage.data() + (index * STRSIZE), pct.data, STRSIZE);
                pctBuf.push_back(pct);

            }

            pct.type = Type::Msg;
            CStr(massage.data() + (index * STRSIZE), pct.data, (massageSize % STRSIZE));
            pctBuf.push_back(pct);

            ++pctCount;

            for (index = 0; index < pctCount; ++index)
                rcv.send(m_client->ConnectSocket, &pctBuf[index]);

        }

        std::cout << name << ": " << massage << std::endl;

        PROCEND
    
    }
    
    /*~CSChat() {

        chat = false;

        ClientCMDY(closeChat, m_client);

    }*/

};

#endif //_CS_CHAT_
 

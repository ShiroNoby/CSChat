#pragma

#include "..\_network_module\CSClient.h"
#include "..\_network_module\CSReciver.h"

#include <string>
#include <vector>

/////////***********************************\\\\\\\\\\Generals

inline int CStrlen(const char* data) {
    int sum = 0;
    int i = 0;
    while (data[i]) { ++sum; ++i; }

    return sum;
}

int CStr(const char* from, char* to, int n) {

    if (!from)   return -1;
    if (!to)     return -2;
    if (!n)      return -3;

    for (int index = 0; index < n; ++index) {
        to[index] = from[index];
    }

    return 0;
}

int toCSTR(char* from, char* to, int n) {

    CStr(from, to, n - 1);

    to[n - 1] = '\0';

    return 0;
}

//////////////////////////////////////////
// функция сравнения левой строки с правой
// LStr > RStr     return -1
// LStr < RStr     return 1
// LStr == RStr    return 0
//
// /param LStr - left string
//        RStr - right string
//          
// /return -1 0 1
//////////////////////////////////////////
int CStrCmp(const char* LStr, const char* RStr)
{
    if (LStr == RStr) return 0;

    size_t Llength = CStrlen(LStr);
    size_t Rlength = CStrlen(RStr);
    size_t low = (Llength <= Rlength) ? Llength : Rlength;

    for (size_t index = 0; index < low; ++index)
    {
        if (LStr[index] > RStr[index]) return -1;      // LStr > RStr
        if (LStr[index] < RStr[index]) return 1;       // LStr < RStr
    }

    if (Llength > Rlength) return -1;                  // LStr > RStr
    if (Llength < Rlength) return 1;                   // LStr < RStr

    return 0;                                           // LStr == RStr
}

//////////********************************************\\\\\\\\\\main

int main() {

    //////////****************************************\\\\\\\\\\Client init

    CSClient client;

    try {
        
        if(client.Create("localhost"))
            throw;

        if (client.Connect())
            throw;

    }catch (...) {}


    //////////****************************************\\\\\\\\\\reciver init

    CSReciver reciver;

    {
        char start[] = "start";

        //////////*********************************\\\\\\\\\\recv start

        try {

            CSPacket buff;

            if (reciver.recv(client.ConnectSocket, &buff) == -1)
                throw;

            if (buff.type != Type::Msg)
                throw;

            if (CStrCmp(buff.data, start))
                throw;

        }
        catch (...) {}

        //////////*********************************\\\\\\\\\\send start

        try {

            CSPacket init;

            Packet::MSG massage;

            if (CStr(start, massage.data, CStrlen(start)))
                throw;

            init.type = Type::Msg;
            if (CStr((char*)&massage, init.data, sizeof(massage)))
                throw;

            if (reciver.send(client.ConnectSocket, &init) == -1)
                throw;
        }
        catch (...) {}       

    }


    //////////****************************************\\\\\\\\\\chat

    {
        char end[] = "end";
        bool chating = true;

        while (chating) {            

            //////////*******************************\\\\\\\\\\send

            std::string massage;
            
            while (1) {

                std::string tmp;

                std::getline(std::cin, tmp);

                if (tmp.empty())
                    break;

                tmp += '\n';

                massage += tmp;

            }

            int massageSize = massage.size();
            int pctCount = massageSize / STRSIZE;   

            if (pctCount) 
                if (massageSize % STRSIZE)
                    ++pctCount;

            if (!pctCount) { //refact
            
                CSPacket pct;
                CStr(massage.data(), pct.data, (massageSize % STRSIZE));

                reciver.send(client.ConnectSocket, &pct);

            }
            else {                
                
                CSPacket                pct;
                Packet::hardPacket      HrdPct;
                std::vector<CSPacket>   pctBuf;

                HrdPct.countPacket      = pctCount;
                pct.type                = Type::HardPacket;
                CStr((const char*)&HrdPct, pct.data, sizeof(Packet::hardPacket));
                pctBuf.push_back(pct);

                int index = 0;
                
                for ( ; index < pctCount - 1; ++index) {

                    CSPacket pct;
                    CStr(massage.data() + (index * STRSIZE), pct.data, STRSIZE);
                    pctBuf.push_back(pct);

                }

                pct.type = Type::Msg;
                CStr(massage.data() + (index * STRSIZE), pct.data, (massageSize % STRSIZE));
                pctBuf.push_back(pct);

                ++pctCount;

                for (index = 0; index < pctCount; ++index)
                    reciver.send(client.ConnectSocket, &pctBuf[index]);

            }

            //////////*******************************\\\\\\\\\\recv

            CSPacket pct;
            int pctSize = sizeof(pct);

            int res = reciver.recv(client.ConnectSocket, &pct);
            if (!CStrCmp(pct.data, end)) {

                reciver.send(client.ConnectSocket, &pct);

                chating = false;
                break;

            }

            if (pct.type == Type::HardPacket) {
            
                Packet::hardPacket hrdpct;
                CStr(pct.data, (char*)&hrdpct, sizeof(Packet::hardPacket));
                
                for (int index = 0; index < hrdpct.countPacket; ++index) {
                
                    int res = reciver.recv(client.ConnectSocket, &pct);
                    std::cout << pct.data;
                
                }

            }
            else std::cout << pct.data << std::endl;

        }
    }

    //////////****************************************\\\\\\\\\\client close

    try {

        if (client.shutdown())
            throw;

    }
    catch (...) {}

    return 0;
}
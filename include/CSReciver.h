#ifndef _CS_RECIVER_
#define _CS_RECIVER_

#include "Generals.h"
#include "CSPacket.h"
#include "pch_network_modul.h"

class CSReciver {

public:

    int send(SOCKET client, CSPacket* packet);
    int recv(SOCKET client, CSPacket* packet);

};

int CSReciver::send(SOCKET client, CSPacket* packet) {

    if (client == INVALID_SOCKET || packet == nullptr)
        return -1;

    int IResult = ::send(client, (const char*)packet, (int)sizeof(CSPacket), 0);
    if (IResult == SOCKET_ERROR) {
        std::cout << "send error: " << WSAGetLastError() << std::endl;
        logExit("send failed");
    }           

    return IResult;
}

int CSReciver::recv(SOCKET client, CSPacket* packet) {

    if (client == INVALID_SOCKET)
        return -1;

    int IResult = 0;
    int result = 0;
    int pctSize = sizeof(CSPacket);

    do {
    
        result = ::recv(client, (char*)packet + IResult, pctSize - IResult, 0);
        if (result == SOCKET_ERROR) {
            std::cout << "recv error: " << WSAGetLastError() << std::endl;
            logExit("recv failed");
            return result;
        }

        if (!result) //emty
            return IResult;

        IResult += result;

    } while (IResult != pctSize);

    return IResult;
}

#endif //_CS_RECIVER_
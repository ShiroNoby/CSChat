#ifndef _CS_CLIENT_
#define _CS_CLIENT_

#include "pch_network_modul.h"

class CSClient {

    WSADATA wsaData;
    int iResult;

    struct addrinfo* result = NULL;

public:

    SOCKET ConnectSocket = INVALID_SOCKET;

    CSClient();
    ~CSClient();

    int Create(const char* ip, const char* port);
    int Connect();
    int shutdown();
    
};

CSClient::CSClient() {

    /////////*********************************************\\\\\\\\\\Initialize Winsock

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult) {
        std::cout << "WSAStartup failed: " << iResult << std::endl; //log
        //return 1;
    }

}

CSClient::~CSClient() {

    freeaddrinfo(result);
    shutdown();
    closesocket(ConnectSocket);
    WSACleanup();

}

int CSClient::Create(const char* ip, const char* port = DEFAULT_PORT) {

    /////////*********************************************\\\\\\\\\\create socket

    struct addrinfo  hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    //IP = ip adress server
    iResult = getaddrinfo(ip, port, &hints, &result);
    if (iResult) {
        std::cout << "getaddrinfo failed: " << iResult << std::endl; //log
        WSACleanup();
        return 1;
    }

    ConnectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl; //log
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    return 0;
}

int CSClient::Connect() {

    /////////*********************************************\\\\\\\\\\Connect

    iResult = connect(ConnectSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;        
    }

    if (ConnectSocket == INVALID_SOCKET) {
        std::cout << "Unable to connect to server!" << std::endl; //log
        WSACleanup();
        return 1;
    }

    return 0;
}

int CSClient::shutdown(){

    /////////*********************************************\\\\\\\\\\shutdown

    // shutdown the send half of the connection since no more data will be sent
    iResult = ::shutdown(ConnectSocket, SD_BOTH);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError()); //log
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    return 0;
}


#endif //_CS_CLIENT_
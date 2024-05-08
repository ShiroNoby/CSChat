#ifndef _CS_SERVER_
#define _CS_SERVER_

#include "pch_network_modul.h"

class CSServer {

    WSADATA wsaData;
    int iResult;

    struct addrinfo* result = NULL;

public:

    SOCKET client = INVALID_SOCKET;
    SOCKET ListenSocket = INVALID_SOCKET;

    CSServer(); //first
    ~CSServer();

    int Create(const char* port); //check 1
    int Create(const char* ip, const char* port);

    int Listen(); //check 2
    int Disconect(SOCKET client); //check 3

};

CSServer::CSServer() {
    
    /////////*********************************************\\\\\\\\\\Initialize Winsock

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult) {
        std::cout << "WSAStartup failed: " << iResult << std::endl; //log
        //return 1;
    }

}

CSServer::~CSServer() {
    
    freeaddrinfo(result);
    closesocket(ListenSocket);
    Disconect(client);
    WSACleanup();

}

int CSServer::Create(const char* port = DEFAULT_PORT) {

    /////////*********************************************\\\\\\\\\\create socket

    struct addrinfo  hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(NULL, port, &hints, &result);
    if (iResult) {
        std::cout << "getaddrinfo failed: " << iResult << std::endl; //log
        WSACleanup();
        return 1;
    }

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl; //log
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    /////////*********************************************\\\\\\\\\\Setup the TCP listening socket

    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        std::cout << "bind failed with error: " << WSAGetLastError(); //log
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    return 0;

}

int CSServer::Create(const char* ip, const char* port = DEFAULT_PORT) {

    /////////*********************************************\\\\\\\\\\create socket

    struct addrinfo  hints;

    ZeroMemory(&hints, sizeof(hints));
    ZeroMemory(&result, sizeof(result));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(ip, port, &hints, &result);
    if (iResult) {
        std::cout << "getaddrinfo failed: " << iResult << std::endl; //log
        WSACleanup();
        return 1;
    }

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl; //log
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    /////////*********************************************\\\\\\\\\\Setup the TCP listening socket

    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        std::cout << "bind failed with error: " << WSAGetLastError(); //log
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    return 0;

}

//1         -> Finding client
//0         -> client founded
//other     -> error
int CSServer::Listen(){
    
    Disconect(client);       

    /////////*********************************************\\\\\\\\\\Listen

    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) { //wtf listen() return
        std::cout << "Listen error with code: " << WSAGetLastError() << std::endl; //log
        closesocket(ListenSocket);
        WSACleanup();
        return -1;
    }

    /////////*********************************************\\\\\\\\\\Accept a client socket

    client = accept(ListenSocket, NULL, NULL);
    if (client == INVALID_SOCKET) {
        std::cout << "accept failed: " << WSAGetLastError() << std::endl; //log
        closesocket(ListenSocket);
        WSACleanup();
        return -1;
    }

    return 1;
}

int CSServer::Disconect(SOCKET client){

    if (client == INVALID_SOCKET)
        return 0;

    iResult = shutdown(client, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        std::cout << "shutdown failed: " << WSAGetLastError() << std::endl; //log
        closesocket(client);
        WSACleanup();
    }

    closesocket(client);

    return 0;
}

#endif //_CS_SERVER_
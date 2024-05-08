#pragma

#include "..\Client\CSChat.h"
#include <thread>

//////////*******************************************************\\\\\\\\\\Generals

//serv return name 
//ClientCMD(init)
void  init(CSClient* client, char* name = nullptr) {
    
    CSPacket packet = ClientCMDY(init, client);

    if(packet.type != Type::initDevice)     { logExit("incorrect init"); } //log

    toCSTR(packet.data, name, 3);

}

//////////*******************************************************\\\\\\\\\\main

//argv[0] = client
//argv[1] = ip 
//argv[2] = port
int main(int argc, char** argv) {

    //////////***************************************************\\\\\\\\\\check argv

    if (argc < 3)                           { logExit("invalid(not enough) argument"); } //log

    //////////***************************************************\\\\\\\\\\create\connect client

    CSClient client;                        //for chating
    if (client.Create(argv[1], argv[2]))    { logExit("failed client create");  } //log//exit
    if (client.Connect())                   { logExit("failed client connect"); } //log//exit

    //////////***************************************************\\\\\\\\\\init

    
    init(&client, name);
    std::cout << "you name: " << name << std::endl;

    //////////***************************************************\\\\\\\\\\chat
        
    std::thread TReciver(CSChat::recv);
    std::thread TImput(CSChat::imput);

    TReciver.detach();
    TImput.detach();

    while (1) {

        std::cout << "start chat with: " << std::endl;
                
        std::cin.getline(otherName, 3);
        if (!CStrCmp(otherName, "ex")) break;

        CSChat lobby(&client);

        while (chat);
    
    }

    //ClientCMDZ(exit, &client, name);

    ClientCMDY(disconnect, &client);

    //
    // Client client
    // client.connect(ip,port)
    // name = recv()                                                //init()
    // cout << "you name " << name << std::endl;                    //check()
        // cout << "create chat with" << std::endl;
        // cin >> otherName;
        // if(otherName == "exit") exit()
        // 
        // cout << "create chat with" << otherName << std::endl
        // 
        // send(update) ...
        // send(create chat,otherName) ...
        // 
        // openchat()
        // 
            // while( chating )                                     //chating()
        // 
        //
    //

}

//chating() {
    // 
    //  recv(massage)
    //  std::cout <<otherMame << ": " <<  massage << std::endl;
    // 
    //  cin << massage
    //  if(massage(exit)){
    //      send(update) ...
    //      send(exit) ...
    //      closeChat()
    //  }
    //  send(massage)
    //}



//
// main -> graphics
// tread1 -> recv()
// tread2 -> cin()
//
#pragma

#include "client_pch.h"
 
#define ClientCMDX(x) command_##x()

#define ClientCMDY(x,y) command_##x(y)

#define ClientCMDZ(x,y,z) command_##x( y, z )
 
CSReciver res;

CSPacket command_init( CSClient* client) {
      
    Packet::command command;
    command.cmd = Packet::Cmd::initDevice;

    CSPacket packet;
    packet.type = Type::cmd;
    toCSTR((char*)&command, packet.data, 2);

    res.send(client->ConnectSocket, &packet);
    res.recv(client->ConnectSocket, &packet);

    return packet;

}

void command_disconnect(CSClient* client) {

    CSPacket packet;
    packet.type = Type::disconnect;

    res.send(client->ConnectSocket, &packet);

}

//CSPacket command_createLobby(CSClient* client, const char* name) {
//
//    Packet::command command;
//    command.cmd = Packet::Cmd::createLobby;
//
//    CSPacket packet;
//    packet.type = Type::cmd;
//    toCSTR((char*)&command, packet.data, 2);
//    res.send(client->ConnectSocket, &packet);
//
//    packet.type = Type::Msg;
//    toCSTR(name, packet.data, 3);
//    res.send(client->ConnectSocket, &packet);
//
//    res.recv(client->ConnectSocket, &packet);
//
//    return packet;
//}

//CSPacket command_closeChat(CSClient* client) {
//
//    Packet::command command;
//    command.cmd = Packet::Cmd::closeLobby;
//
//    CSPacket packet;
//    packet.type = Type::cmd;
//    res.send(client->ConnectSocket, &packet);
//
//    return packet;
//
//}

//void command_exit(CSClient* client, char* name) {
//
//    CSPacket packet;
//    packet.type = Type::exit;
//
//    Header header(name);
//    packet.header = header;
//
//    res.send(client->ConnectSocket, &packet);
//}

//command_createLobby(){}
//command_update(){}
//command_closeChat(){
//
// send()
// recv()
// check()
//
//}

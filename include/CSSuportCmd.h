#ifndef _CS_SUPORT_CMD_
#define _CS_SUPORT_CMD_

#include "Generals.h"
#include "CSReciver.h"

#include "CSClient.h"
//#include "CSServer.h"
 
#define ClientCMDA(A) command_##A()
#define ClientCMDB(A,B) command_##A(B)
#define ClientCMDC(A,B,C) command_##A( B, C )
#define ClientCMDE(A,B,C,D,E) command_##A( B, C, D, E )
 
CSReciver res;

//**********************************************Client

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

//send to server cmd 
//recv from server HRDPacket whith name list
void command_update(CSClient* client){
	
	Packet::command command;
    command.cmd = Packet::Cmd::updateNameList;
	
	CSPacket packet;
    packet.type = Type::cmd;
    toCSTR((char*)&command, packet.data, 2);
	
	res.send(client->ConnectSocket, &packet);	
	
}

//type = availableName
void command_isActive(CSClient* client, const char* name){	
		
	Header header;
	CStr(name, header.otherName, 2);
	
	CSPacket packet;
	packet.header = header;
	
    toCSTR(" ", packet.data, 2);
	
	res.send(client->ConnectSocket, &packet);	
	
}

void command_disconnect(CSClient* client) {

    CSPacket packet;
    packet.type = Type::disconnect;

    res.send(client->ConnectSocket, &packet);

}

void command_send(CSClient* client, const char* msg, const char* MyName, const char* otherName){

	int massageSize = CStrlen(msg); 
	int pctCount = massageSize / STRSIZE;	
	
	if (!massageSize) return;

	if (pctCount)
		if (massageSize % STRSIZE)
			++pctCount;

	if (!pctCount) { //refact

		CSPacket pct;

		Header header(MyName, otherName);
		pct.header = header;

		CStr(msg, pct.data, (massageSize % STRSIZE));

		res.send(client->ConnectSocket, &pct);

	}
	else {

		CSPacket                pct;

		Header                  header(MyName, otherName);
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

			Header      header(MyName, otherName);
			pct.header  = header;

			CStr(msg + (index * STRSIZE), pct.data, STRSIZE);
			pctBuf.push_back(pct);

		}

		pct.type = Type::Msg;
		CStr(msg + (index * STRSIZE), pct.data, (massageSize % STRSIZE));
		pctBuf.push_back(pct);

		++pctCount;

		for (index = 0; index < pctCount; ++index)
			res.send(client->ConnectSocket, &pctBuf[index]);

	}
	
}


//**********************************************Server



#endif //_CS_SUPORT_CMD_

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

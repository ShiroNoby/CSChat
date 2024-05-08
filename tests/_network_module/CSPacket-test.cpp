#include "../../_network_module/CSPacket.h"

namespace packetCode {

    enum CodeType {

        succes = 0,
        CSPacket,
        Type,
        MSG,
        command,
        hardPacket,
        packet,
        HP,
        initDevice
    };

};

///////////**********************************************\\\\\\\\\\\\CSPacket



///////////**********************************************\\\\\\\\\\\\Type



///////////**********************************************\\\\\\\\\\\\MSG

int testMSG() {

    Packet::MSG massage;

    if(sizeof(massage) != 128)

    return packetCode::succes;
}

///////////**********************************************\\\\\\\\\\\\command

//int testCommand() {
//
//    Packet::command cmd;
//
//    if (cmd.cmd != " ")
//        return packetCode::command;
//
//    return packetCode::succes;
//}

///////////**********************************************\\\\\\\\\\\\hardPacket

int testHp() {

    Packet::hardPacket pct;

    if (pct.cmd != "start")
        return packetCode::hardPacket;

    if (sizeof(pct) != 7) 
        return packetCode::hardPacket;
    
    return packetCode::succes;
}

///////////**********************************************\\\\\\\\\\\\packet

//
// 4b uint
// 4b 2b short  2b null
// 4b int
// 4b 1b char   3b null;
//
class Tmp {

    unsigned int    uin = -120;    
    short           sh = 200;
    int             in = 5000;
    char            ch = 'H';   

public:

    bool operator==(Tmp &tmp) {
    
        if ((uin != tmp.uin) && (sh != tmp.sh) && (in != tmp.in) && (ch != tmp.ch))
            return false;

        return true;
    }

    bool operator!=(Tmp& tmp) {
        return !(*this == tmp);
    }

};

//
// ~668b
//
struct bigTmp {

    char    data1[50]   = "";       //52
    int     size1       = 500;      //56
    char    data2[100]  = "";       //156
    int     size2       = 500;      //160
    char    data3[200]  = "";       //360
    int     size3       = 11111;    //364
    char    data4[300]  = "";       //664
    int     size4       = 23152356; //668

    bool operator==(bigTmp& tmp) {
    
        if ((size1 != tmp.size1) || (size2 != tmp.size2) || (size3 != tmp.size3) || (size4 != tmp.size4))
            return false;

        return true;
    
    }

};

int testpacket() {

    ///////////******************************************\\\\\\\\\\\\init

    Packet::packet pct;

    if (pct.data != "")
        return packetCode::packet;

    ///////////******************************************\\\\\\\\\\\\size

    if (sizeof(pct) != 128)
        return packetCode::packet;

    ///////////******************************************\\\\\\\\\\\\solo file coppy

    {
        Tmp tmp;
        pct = *(Packet::packet*)&tmp;
    }

    {
        Tmp tmp;
        if (tmp != *((Tmp*)&pct))
            return packetCode::packet;
    }
    
    ///////////******************************************\\\\\\\\\\\\big file coppy

    bigTmp tmp;

    Packet::packet buff[6];

    *buff = *(Packet::packet*)&tmp;

    Packet::packet buff1[6];
    for (int index = 0; index < 6; ++index)
        buff1[index] = buff[index];

    if(!(tmp == *(bigTmp*)buff1))
        return packetCode::packet;

    return packetCode::succes;
}

///////////**********************************************\\\\\\\\\\\\HP

int testHP() {

    ///////////******************************************\\\\\\\\\\\\init

    Packet::HP hp;

    if (hp.cmd != "end")
        return packetCode::HP;

    return packetCode::succes;
}

///////////**********************************************\\\\\\\\\\\\initDevice

int testInitDevice() {

    ///////////******************************************\\\\\\\\\\\\size

    if (sizeof(Packet::initDevice) != 11)
        return 1;

    ///////////******************************************\\\\\\\\\\\\name

    {
        Packet::initDevice id;

        if (id.name_device != "" || id.name_acaunt != "")
            return 1;

        char name_device[3] = "A0";
        char name_acaunt[8] = "TEST000";

        char pull[11] = "A0TEST000";

        Packet::initDevice* tmp = &id;
        tmp = (Packet::initDevice*)pull;

        if (id.name_device != name_device || id.name_acaunt != name_acaunt)
            return packetCode::initDevice;
    }

    return packetCode::succes;
}
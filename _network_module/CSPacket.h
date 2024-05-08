#ifndef _CS_PACKET_
#define _CS_PACKET_

#define STRSIZE 128

namespace Packet {

    ///////////**********************************************\\\\\\\\\\\\MSG

    //~128b
    struct MSG {

        char data[STRSIZE] = " ";

    };

    ///////////**********************************************\\\\\\\\\\\\command

    enum Cmd {

        Start = 1,
        Stop,
        initDevice,
        createLobby,
        closeLobby,


        max_Cmd = 256
    };

    //0~127
    struct command {

        char cmd;

    };

    ///////////**********************************************\\\\\\\\\\\\hardPacket

    struct hardPacket {// ~7b + ~120b( type packet [image,video,sound...])

        char countPacket = 0;
        const char cmd[6] = "start";
        //data type...

    };

    //~128b
    struct packet {

        char data[STRSIZE] = "";

    };

    // 
    // packet1;
    // packet2;
    // ...
    // packetCount;
    // 
    // ~4b
    struct HP {

        const char cmd[4] = "end";

    };

    ///////////**********************************************\\\\\\\\\\\\init

    // name_device[2] = "" or "A1";
    // name_acaunt[8] = "aaaaaaaa";?
    struct initDevice {

        char name_device[3] = "";
        char name_acaunt[8] = "";

    };

    ///////////**********************************************\\\\\\\\\\\\?
}

enum Type {

    Msg = 0,
    cmd,
    HardPacket,
    initDevice,
    disconnect,

    success

};

struct Header {

    char myName[3]      = "";
    char otherName[3]   = "";

    Header() {

        this->otherName[0] = 'S';
        this->otherName[1] = 'E';

    }

    Header(char name[2]) {
    
        this->myName[0] = name[0];
        this->myName[1] = name[1];
        this->otherName[0] = 'S';
        this->otherName[1] = 'E';
    
    }

    Header(char name[2], char otherName[2]) {

        this->myName[0] = name[0];
        this->myName[1] = name[1];
        this->otherName[0] = otherName[0];
        this->otherName[1] = otherName[1];

    }

};

struct CSPacket { //header{name,othername}
    
    Type        type        = Type::Msg;
    Header      header;
    char        data[STRSIZE]   = " ";

};

//create make...

#endif //_CS_PACKET_
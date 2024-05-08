#ifndef _CS_CARD_
#define _CS_CARD_

//
// struct file
// 
// "A1"
// 0 ------------------------------------------
// 2  |    name_device
// 10 |    name_acaunt
// 14 |    unread_msg_count
//    |    
//    |    "MSG"
//    |    0  ------------------------------
//    |    2  |     name_device
//    |    6  |     size_msg
//    |       |     msg
//    |    
//    |    "MSG"
//    |    0  ------------------------------
//    |    2  |     name_device
//    |    6  |     size_msg
//    |       |     msg
//    |       
//     
//


#include "server_pch.h"

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

/////////***********************************\\\\\\\\\\MSG

struct MSG {  //struct for massage
    char            name_device[2];
    int             size = 0;
    std::string     massage;
};//MSG()

/////////***********************************\\\\\\\\\\BuffMsg

struct BuffMsg { //collect massage
    int                 size = 0;
    std::vector<MSG>    buffer;
};//BuffMsg()

/////////***********************************\\\\\\\\\\CSCard

struct CSCard {

    CSCard(const char* firstName,const char* secondName);

    int addMassage(MSG);

    int LoadFromFile(const char*);
    ~CSCard();

private:

    char        name_device[2]; //static
    char        name_acaunt[8];
    BuffMsg     buffer;
};

CSCard::CSCard(const char* firstName, const char* secondName) {

    //check !firstName
    CStr(firstName, name_device, 2);

    //check !secondName
    CStr(secondName, name_acaunt, 8);
}

int CSCard::addMassage(MSG massage) {

    if (!massage.size)
        return -1;

    buffer.size += 1;
    buffer.buffer.push_back(massage);


    return 0;
}

int CSCard::LoadFromFile(const char* file) {//

    if (file == nullptr)
        file = name_device;

    std::ifstream File(file, std::fstream::binary); //???

    if (!File.is_open())
        return -1;	

    File.read(name_device, sizeof(char) * 2);		// reed name_device        "A1"
    File.read(name_acaunt, sizeof(char) * 8);		// read name_acaunt        "AAAAAAAA"
    File.read((char*)&buffer.size, sizeof(int) );	// write unread_msg_count   "2"

    buffer.buffer.resize(buffer.size);

    for (int index = 0; index < buffer.size; ++index) {
        File.ignore(3); //skip MSG
        File.read(buffer.buffer[index].name_device, sizeof(char) * 2);		// read out name_device    "A2"
        File.read((char*)&buffer.buffer[index].size, sizeof(int));			// write size_msg          "10" (symbol)(10*1 byte)
		
        buffer.buffer[index].massage.resize(buffer.buffer[index].size);
        File.read(buffer.buffer[index].massage.data(), buffer.buffer[index].size);	// read massage								
    }

    return 0;
} 

//
	// write name_device        "A1"
	// write name_acaunt        "AAAAAAAA"
	// write unread_msg_count   "2"
	// Write                    "MSG"
	// write out name_device    "A2"
	// write size_msg           "10" (symbol)(10*1 byte)
	// write massage
	// Write                    "MSG"
	// write out name_device    "A3"
	// write size_msg           "111" (symbol)(111*1 byte)
	// write massage
//
CSCard::~CSCard() {

    char filename[3];

    toCSTR(name_device, filename, 3);

    std::fstream file( filename, std::fstream::binary ); //binary

    if (!file.is_open())
        file.open(filename, std::fstream::binary | std::fstream::trunc | std::fstream::out);
	
    file.write(name_device, 2);															// write name_device        "A1"
    file.write(name_acaunt, 8);															// write name_acaunt        "AAAAAAAA"
    file.write(reinterpret_cast<char*>(&buffer.size), sizeof(int));						// write unread_msg_count   "2"

    for (int index = 0; index < buffer.size; ++index) {
        file.write( "MSG", 3 );															// Write                    "MSG"
        file.write( buffer.buffer[index].name_device, 2 );								// write out name_device    "A2"
        file.write( reinterpret_cast<char*>(&buffer.buffer[index].size), sizeof(int) );	// write size_msg           "10" (symbol)(10*1 byte)
        file.write( buffer.buffer[index].massage.data(), buffer.buffer[index].size );	// write massage
    }

    file.close();
}

#endif //_CS_CARD_

///////////**********************************************\\\\\\\\\\\\CSCard

#include "..\..\Server\CSCard.h"


int main() {
    {
        CSCard card1("A0", "SERVER00");

        MSG massage;
        CStr("A1", massage.name_device, 2);
        massage.massage = "hello, World!";
        massage.size = massage.massage.size();

        card1.addMassage(massage);

        CStr("A2", massage.name_device, 2);
        massage.massage = "happy new years!!!";
        massage.size = massage.massage.size();

        card1.addMassage(massage);
    }

    {   
        CSCard card1("A0", "SERVER00");

        card1.LoadFromFile("A0");

        MSG massage;

        CStr("A1", massage.name_device, 2);
        massage.massage = "trird massage testing";
        massage.size = massage.massage.size();

        card1.addMassage(massage);
    }   
}

///////////**********************************************\\\\\\\\\\\\CStrlen




///////////**********************************************\\\\\\\\\\\\CStr



///////////**********************************************\\\\\\\\\\\\toCSTR



///////////**********************************************\\\\\\\\\\\\MSG



///////////**********************************************\\\\\\\\\\\\BuffMsg



///////////**********************************************\\\\\\\\\\\\CSCard



///////////**********************************************\\\\\\\\\\\\CSCard(const char* firstName,const char* secondName);



///////////**********************************************\\\\\\\\\\\\addMassage(MSG);



///////////**********************************************\\\\\\\\\\\\int LoadFromFile(const char*);



///////////**********************************************\\\\\\\\\\\\~CSCard();
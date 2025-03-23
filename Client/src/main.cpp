#include "CSHead.h"
#include "CSNameList.h"
#include "CSChat.h"

#include "CSSuportCmd.h"
#include <thread>
#include <sstream> //?

#include "CSLog.h"

//////////*******************************************************\\\\\\\\\\main

//argv[0] = client
//argv[1] = ip 
//argv[2] = port
int main(int argc, char** argv) {
    
    CSLog logMain("log.txt");
    CSLog logrecv("log.txt");
    
    logMain.clearfile();
    logrecv.clearfile();
    
    logMain.SystemLog(tier0, __FILE__, __LINE__, "client start");
    
    //////////***************************************************\\\\\\\\\\check argv
    
    if (argc < 3)                           { logMain.SystemLog(tier3, __FILE__, __LINE__, "ERROR: invalid(not enough) argument"); }

    //////////***************************************************\\\\\\\\\\create\connect client
    
    logMain.SystemLog(tier0, __FILE__, __LINE__, "connect");
    CSClient client;                        //for chating
    if (client.Create(argv[1], argv[2]))    { logMain.SystemLog(tier3, __FILE__, __LINE__, "ERROR: failed client create"); }
    if (client.Connect())                   { logMain.SystemLog(tier3, __FILE__, __LINE__, "ERROR: failed client connect"); }

    //////////***************************************************\\\\\\\\\\init
    
    logMain.SystemLog(tier0, __FILE__, __LINE__, "myName init");        
    char myName[3] = "";
    CSPacket packet = ClientCMDB(init, &client);

    if(packet.type != Type::initDevice)     { logMain.SystemLog(tier3, __FILE__, __LINE__, "ERROR: incorrect init"); }
    
    toCSTR(packet.data, myName, 3);    
    
    //////////***************************************************\\\\\\\\\\UI
    
    logMain.SystemLog(tier0, __FILE__, __LINE__, "create window");
    sf::RenderWindow window(sf::VideoMode(1000, 800), "SFML works!");

    float X = 0;
    float Y = 0;
    float SizeX = (float)window.getSize().x;
    float SizeY = (float)window.getSize().y;
    float HeadSizeX = SizeX / 4;
    float HeadSizeY = SizeY / 10;
    float nameListSizeY = SizeY - HeadSizeY;
    float ChatSizeX = SizeX - HeadSizeX;

    //////////***************************************************\\\\\\\\\\font
    
    logMain.SystemLog(tier0, __FILE__, __LINE__, "load atlas");
    sf::Font font;
    font.loadFromFile(_atlas_);

    //////////***************************************************\\\\\\\\\\elem

    CSElem tmp1;
    tmp1.type = CSElem::ZeroMsg;

    //////////***************************************************\\\\\\\\\\headText
    
    logMain.SystemLog(tier0, __FILE__, __LINE__, "create HeadName");
    HeadText headText({ X, Y }, { HeadSizeX, HeadSizeY });
    headText.add(myName);

    //////////***************************************************\\\\\\\\\\nameList
    
    logMain.SystemLog(tier0, __FILE__, __LINE__, "create NameList");    
    NameList nameList({ X, HeadSizeY }, { HeadSizeX, nameListSizeY });

    //////////***************************************************\\\\\\\\\\chat
    
    logMain.SystemLog(tier0, __FILE__, __LINE__, "create CSChat");    
    Chat chat({ HeadSizeX, Y }, { ChatSizeX, SizeY });

    //////////***************************************************\\\\\\\\\\activeName

    std::string activeName;
    bool active = false;
    
    //////////***************************************************\\\\\\\\\\RECV
    
    auto recv = [&]() {        
        
        logrecv.SystemLog(tier1, __FILE__, __LINE__, "tread start");        
        while(1){
            
            CSPacket pct;
            res.recv(client.ConnectSocket, &pct);
            
            switch(pct.type){
                
                case Type::Msg:{                    
                    
                    logrecv.SystemLog(tier1, __FILE__, __LINE__, "recv msg");                    
                    if (!CStrCmp(pct.data, " ")) continue;

                    if (!CStrCmp(pct.data, "client not found")){
                        
                        logrecv.SystemLog(tier1, __FILE__, __LINE__, "erase name from nameList");
                        
                        std::string temp = "...stop chat...";
                        chat.elem1.loadToFile(&temp, pct.header.otherName);
                        
                        nameList.erase( pct.header.otherName );
                        nameList.display();
                        break;
                        
                    }
                    
                    logrecv.SystemLog(tier1, __FILE__, __LINE__, "write msg to file"); 
                    std::string str(pct.data); 
                    chat.elem1.loadToFile(&str, pct.header.myName);
                        
                    logrecv.SystemLog(tier1, __FILE__, __LINE__, "change status name in nameList"); 
                    CSElem tmp;
                    tmp.type = CSElem::SomeMsg;
                    nameList[pct.header.myName] = std::move(tmp);
                    nameList.display();
                    
                    break;
                }
                case Type::HardPacket:{
                    
                    logrecv.SystemLog(tier1, __FILE__, __LINE__, "recv HardPacket");                    
                    Packet::hardPacket hrdpct;
                    CStr(pct.data, (char*)&hrdpct, sizeof(Packet::hardPacket));
                    
                    std::string str;
                    
                    for (int index = 0; index < hrdpct.countPacket; ++index) {
                        
                        res.recv(client.ConnectSocket, &pct);
                        str += pct.data;

                    }
                    
                    logrecv.SystemLog(tier1, __FILE__, __LINE__, "write msg to file");
                    chat.elem1.loadToFile(&str, pct.header.myName);
                    
                    logrecv.SystemLog(tier1, __FILE__, __LINE__, "change status name in nameList");
                    CSElem tmp;
                    tmp.type = CSElem::SomeMsg;
                    nameList[pct.header.myName] = std::move(tmp);
                    nameList.display();
                    
                    break;
                }
                case Type::updateNameList:{
                                        
                    logrecv.SystemLog(tier1, __FILE__, __LINE__, "recv updateNameList");                    
                    Packet::hardPacket hrdpct;
                    CStr(pct.data, (char*)&hrdpct, sizeof(Packet::hardPacket));
                                        
                    std::string str;
                    
                    for (int index = 0; index < (int)hrdpct.countPacket; ++index) {

                        res.recv(client.ConnectSocket, &pct);
                        str += pct.data;

                    }
                    
                    logrecv.SystemLog(tier1, __FILE__, __LINE__, "add names in nameList");
                    std::istringstream stream;
                    stream.str(str);
                    std::string tmp;
                    
                    //add new name in nameList
                    //not activ is ignore
                    for(; std::getline(stream,tmp, ';');){                        
                        
                        logrecv.SystemLog(tier1, __FILE__, __LINE__, "!!!check!!!");
                        logrecv.SystemLog(tier1, __FILE__, __LINE__, tmp.c_str());                        
                        if(CStrCmp(tmp.c_str(), myName) == 0)
                            continue;
                        
                        nameList.add(tmp.c_str(), tmp1);
                        
                    }
                    
                    nameList.display(); 
                    
                    break;
                }                
                default: break;                
                
            }            
        }        
    };
    
    logMain.SystemLog(tier0, __FILE__, __LINE__, "create tread");    
    std::thread TReciver(recv);
    TReciver.detach();    

    //////////***************************************************\\\\\\\\\\     

    logMain.SystemLog(tier0, __FILE__, __LINE__, "main loop");    
    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            
            switch (event.type) {

                case sf::Event::Closed: {                    
                    
                    logMain.UserLog(tier0, __FILE__, __LINE__, "sf::Event::Closed");                        
                    logMain.SystemLog(tier0, __FILE__, __LINE__, "window close");    
                    window.close();
                    
                    logMain.SystemLog(tier0, __FILE__, __LINE__, "disconnect");    
                    ClientCMDB(disconnect, &client);
                    
                    break;

                }
                case sf::Event::MouseButtonPressed: {
                    
                    logMain.UserLog(tier0, __FILE__, __LINE__, "sf::Event::MouseButtonPressed");
                    float x = event.mouseButton.x;
                    float y = event.mouseButton.y;

                    //HeadName
                    if (headText._shape.getGlobalBounds().contains(x, y)){
                        
                        logMain.UserLog(tier0, __FILE__, __LINE__, "click to headText");
                        
                        Header header(myName);
                        
                        CSPacket packet;
                        packet.header = header;
                        packet.type = Type::updateNameList;
                        
                        logMain.UserLog(tier0, __FILE__, __LINE__, "send updateNameList");
                        
                        res.send(client.ConnectSocket, &packet);	
                        
                        break;
                        
                    }

                    //Namelist
                    if (nameList._shape.getGlobalBounds().contains(x, y)) {                        
                        
                        logMain.UserLog(tier0, __FILE__, __LINE__, "click to Namelist");
                        unsigned int tmpPos = (unsigned int)y - HeadSizeY;

                        if(nameList.empty()){                            
                            
                            logMain.UserLog(tier0, __FILE__, __LINE__, "namelist is empty");
                            
                            break;
                        }

                        logMain.SystemLog(tier0, __FILE__, __LINE__, "search name");    
                        const char* name = nameList.setActiveElem(tmpPos);
                        
                        if(name == nullptr)
                            break;
                        
                        logMain.SystemLog(tier0, __FILE__, __LINE__, "check active name"); 
                        logMain.SystemLog(tier0, __FILE__, __LINE__, name);    
                        ClientCMDC(isActive, &client, name);

                        //old activeName
                        if (!activeName.empty()) {

                            logMain.SystemLog(tier0, __FILE__, __LINE__, "clear prev chat");    
                            active = false;

                            chat.elem2.msg.clear();
                            chat.elem2.display();

                        }

                        logMain.SystemLog(tier0, __FILE__, __LINE__, "begin chat whit name");    
                        activeName = name;
                        chat.elem1.chat(name);

                        break;

                    }

                    //chat
                    if (chat._shape.getGlobalBounds().contains(x, y)) {
                        
                        logMain.UserLog(tier0, __FILE__, __LINE__, "click to chat");
                        float offsetX = x - HeadSizeX;
                        float offsetY = y;

                        //typingText
                        if (chat.elem2._shape.getGlobalBounds().contains(offsetX, offsetY)) {
                            
                            logMain.UserLog(tier0, __FILE__, __LINE__, "click to chat.typing wind");
                            if (!chat.elem1.pathFile.empty()){                                
                                
                                logMain.SystemLog(tier0, __FILE__, __LINE__, "begin typing msg in chat");                                
                                active = true;
                            }
                        }
                        
                        break;
                    }

                    break;
                }
                case sf::Event::MouseWheelMoved: {
                    
                    logMain.UserLog(tier0, __FILE__, __LINE__, "sf::Event::MouseWheelMoved");
                    float x = event.mouseButton.x;
                    float y = event.mouseButton.y;

                    if (nameList._shape.getGlobalBounds().contains(x, y)) {
                        
                        logMain.UserLog(tier0, __FILE__, __LINE__, "wheelMoved int nameList");
                        if (event.mouseWheel.delta > 0) {

                            logMain.SystemLog(tier0, __FILE__, __LINE__, "nameList up");
                            nameList.up();
                        }

                        if (event.mouseWheel.delta < 0) {

                            logMain.SystemLog(tier0, __FILE__, __LINE__, "nameList down");
                            nameList.down();
                        }
                    }

                    //chat
                    if (chat._shape.getGlobalBounds().contains(x, y)) {
                        
                        logMain.UserLog(tier0, __FILE__, __LINE__, "wheelMoved int chat");
                        float offsetX = x - HeadSizeX;
                        float offsetY = y;

                        //RenderText
                        if (chat.elem1._shape.getGlobalBounds().contains(offsetX, offsetY)) {
                            
                            logMain.UserLog(tier0, __FILE__, __LINE__, "wheelMoved int chat->elem1");
                            if (!chat.elem1.pathFile.empty()) {

                                if (event.mouseWheel.delta > 0) {

                                    logMain.SystemLog(tier0, __FILE__, __LINE__, "chat->elem1 up");
                                    chat.elem1.up();
                                }

                                if (event.mouseWheel.delta < 0) {

                                    logMain.SystemLog(tier0, __FILE__, __LINE__, "chat->elem1 up");
                                    chat.elem1.down();
                                }
                            }
                        }

                        //typingText
                        if (chat.elem2._shape.getGlobalBounds().contains(offsetX, offsetY)) {
                            
                            logMain.UserLog(tier0, __FILE__, __LINE__, "wheelMoved int chat->elem2");
                            if (!chat.elem1.pathFile.empty()) {

                                if (event.mouseWheel.delta > 0) {

                                    logMain.SystemLog(tier0, __FILE__, __LINE__, "chat->elem2 up");
                                    chat.elem2.up();
                                }

                                if (event.mouseWheel.delta < 0) {

                                    logMain.SystemLog(tier0, __FILE__, __LINE__, "chat->elem2 up");
                                    chat.elem2.down();
                                }
                            }
                        }
                    }

                    break;

                }
                case sf::Event::KeyPressed: {

                    if (!chat.elem1.pathFile.empty()) {

                        if (active || !activeName.empty()) {                            
                            
                            logMain.UserLog(tier0, __FILE__, __LINE__, "sf::Event::KeyPressed");
                            int key = event.key.code;
                            if (key == sf::Keyboard::Key::Enter) {

                                logMain.SystemLog(tier0, __FILE__, __LINE__, "pressed enter");
                                chat.elem1.loadToFile(&chat.elem2.msg, activeName.c_str(), "R.");
                                chat.elem1.chat(activeName.c_str());                                
                                                                
                                ClientCMDE(send, &client, chat.elem2.msg.c_str(), myName, activeName.c_str());
                                chat.elem2.msg.clear();

                                chat.elem1.display();
                                chat.elem2.display();

                            }
                            else{
                                
                                logMain.SystemLog(tier0, __FILE__, __LINE__, "other key pressed");
                                chat.elem2.add(key);
                                
                            }
                        }
                    }
                    
                    break;                    
                }
                default:
                    break;

            }
        }

        if (headText.update() || nameList.update() || chat.update()) {
            
            logMain.SystemLog(tier0, __FILE__, __LINE__, "draw window");
            window.clear();

            //draw(karkas)
            window.draw(headText.getShape());
            window.draw(nameList.getShape());
            window.draw(chat.getShape());

            window.display();

        }
    }

    return 0;
};
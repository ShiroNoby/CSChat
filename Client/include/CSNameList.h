#ifndef _CS_NAME_LIST_
#define _CS_NAME_LIST_

#include "CSSlider.h"

#include <map>

struct CSElem {

    enum elemType {

        ZeroMsg = 0,
        SomeMsg,
        Typing,
        Archive

    }type = ZeroMsg;

};

class elemBuilder : public CSRenderTexture {

public:

    explicit elemBuilder(sf::Vector2f center = { 0.f, 0.f }, sf::Vector2f size = { 250.f, 80.f }) 
        : CSRenderTexture(center, size) { _elem.loadFromFile(_build_); }

    //name whith no msg
    sf::Texture buildZeroMsg(const char* name, sf::Font& font) {

        sf::RectangleShape zeroBound;
        zeroBound.setSize({ 250.f, 80.f });
        zeroBound.setTexture(&_elem);
        zeroBound.setTextureRect(sf::IntRect({ 231, 1 }, { 248, 78 }));

        sf::RectangleShape nameBound;
        nameBound.setSize({ 230.f, 61.f });
        nameBound.setTexture(&_elem);
        nameBound.setTextureRect(sf::IntRect({ 0, 0 }, { 231, 61 }));

        sf::RectangleShape zeroMSG;
        zeroMSG.setSize({ 70.f, 59.f });
        zeroMSG.setTexture(&_elem);
        zeroMSG.setTextureRect(sf::IntRect({ 71, 121 }, { 69, 60 }));

        sf::Text nameText;

        //*******************************template 1 name whith no msg

        _renderTexture.create(250.f, 80.f);
        _renderTexture.clear(sf::Color::White);

        zeroBound.setPosition({ 0.f, 0.f });
        _renderTexture.draw(zeroBound);

        nameBound.setPosition({ 10.f, 10.f });
        _renderTexture.draw(nameBound);

        nameText.setPosition({ nameBound.getPosition().x + 20, nameBound.getPosition().y + 10 });
        nameText.setString(name);
        nameText.setFont(font);
        nameText.setFillColor(sf::Color::Black);
        _renderTexture.draw(nameText);

        zeroMSG.setPosition({ (nameBound.getPosition().x + nameBound.getSize().x - zeroMSG.getSize().x - 1), nameBound.getPosition().y + 1 });
        _renderTexture.draw(zeroMSG);
        _renderTexture.display();

        sf::Texture tmp;
        tmp = _renderTexture.getTexture();

        return tmp;

    }

    //name whith  msg
    sf::Texture buildSomeMsg(const char* name, sf::Font& font) {

        sf::RectangleShape zeroBound;
        zeroBound.setPosition({ 300.f, 1.f });
        zeroBound.setSize({ 250.f, 80.f });
        zeroBound.setTexture(&_elem);
        zeroBound.setTextureRect(sf::IntRect({ 231, 1 }, { 248, 78 }));

        sf::RectangleShape nameBound;
        nameBound.setPosition({ 1.f, 100.f });
        nameBound.setSize({ 230.f, 61.f });
        nameBound.setTexture(&_elem);
        nameBound.setTextureRect(sf::IntRect({ 0, 0 }, { 231, 61 }));

        sf::RectangleShape someMSG;
        someMSG.setPosition({ 200.f, 1.f });
        someMSG.setSize({ 70.f, 59.f });
        someMSG.setTexture(&_elem);
        someMSG.setTextureRect(sf::IntRect({ 141, 121 }, { 69, 60 }));

        sf::Text nameText;

        //*******************************template 2 name whith  msg

        _renderTexture.create(250.f, 80.f);
        _renderTexture.clear(sf::Color::White);

        zeroBound.setPosition({ 0.f, 0.f });
        _renderTexture.draw(zeroBound);

        nameBound.setPosition({ 10.f, 10.f });
        _renderTexture.draw(nameBound);

        nameText.setPosition({ nameBound.getPosition().x + 20, nameBound.getPosition().y + 10 });
        nameText.setString(name);
        nameText.setFont(font);
        nameText.setFillColor(sf::Color::Black);
        _renderTexture.draw(nameText);

        someMSG.setPosition({ (nameBound.getPosition().x + nameBound.getSize().x - someMSG.getSize().x - 1), nameBound.getPosition().y + 1 });
        _renderTexture.draw(someMSG);
        _renderTexture.display();

        sf::Texture tmp;
        tmp = _renderTexture.getTexture();

        return tmp;

    }

    //name typing
    sf::Texture buildTyping(const char* name, sf::Font& font) {

        sf::RectangleShape someBound;
        someBound.setPosition({ 300.f, 100.f });
        someBound.setSize({ 250.f, 80.f });
        someBound.setTexture(&_elem);
        someBound.setTextureRect(sf::IntRect({ 231, 81 }, { 248, 78 }));

        sf::RectangleShape nameBound;
        nameBound.setPosition({ 1.f, 100.f });
        nameBound.setSize({ 230.f, 61.f });
        nameBound.setTexture(&_elem);
        nameBound.setTextureRect(sf::IntRect({ 0, 0 }, { 231, 61 }));

        sf::RectangleShape typing;
        typing.setPosition({ 1.f, 1.f });
        typing.setSize({ 70.f, 59.f });
        typing.setTexture(&_elem);
        typing.setTextureRect(sf::IntRect({ 1, 121 }, { 69, 60 }));

        sf::Text nameText;

        //*******************************template 3 name typing

        _renderTexture.create(250.f, 80.f);
        _renderTexture.clear(sf::Color::White);

        someBound.setPosition({ 0.f, 0.f });
        _renderTexture.draw(someBound);

        nameBound.setPosition({ 10.f, 10.f });
        _renderTexture.draw(nameBound);

        nameText.setPosition({ nameBound.getPosition().x + 20, nameBound.getPosition().y + 10 });
        nameText.setString(name);
        nameText.setFont(font);
        nameText.setFillColor(sf::Color::Black);
        _renderTexture.draw(nameText);

        typing.setPosition({ (nameBound.getPosition().x + nameBound.getSize().x - typing.getSize().x - 1), nameBound.getPosition().y + 1 });
        _renderTexture.draw(typing);
        _renderTexture.display();

        sf::Texture tmp;
        tmp = _renderTexture.getTexture();

        return tmp;

    }

    //Aname
    sf::Texture buildArchive(const char* name, sf::Font& font) {

        sf::RectangleShape zeroBound;
        zeroBound.setPosition({ 300.f, 1.f });
        zeroBound.setSize({ 250.f, 80.f });
        zeroBound.setTexture(&_elem);
        zeroBound.setTextureRect(sf::IntRect({ 231, 1 }, { 248, 78 }));

        sf::RectangleShape ArchiveBound;
        ArchiveBound.setPosition({ 1.f, 200.f });
        ArchiveBound.setSize({ 230.f, 61.f });
        ArchiveBound.setTexture(&_elem);
        ArchiveBound.setTextureRect(sf::IntRect({ 0, 60 }, { 231, 61 }));

        sf::RectangleShape typing;
        typing.setPosition({ 1.f, 1.f });
        typing.setSize({ 70.f, 59.f });
        typing.setTexture(&_elem);
        typing.setTextureRect(sf::IntRect({ 1, 121 }, { 69, 60 }));

        sf::Text nameText;

        //*******************************template 4 Aname 

        _renderTexture.create(250.f, 80.f);
        _renderTexture.clear(sf::Color::White);

        zeroBound.setPosition({ 0.f, 0.f });
        _renderTexture.draw(zeroBound);

        ArchiveBound.setPosition({ 10.f, 10.f });
        _renderTexture.draw(ArchiveBound);

        nameText.setPosition({ ArchiveBound.getPosition().x + 20, ArchiveBound.getPosition().y + 10 });
        nameText.setString(name);
        nameText.setFont(font);
        nameText.setFillColor(sf::Color::Black);
        _renderTexture.draw(nameText);

        typing.setPosition({ (ArchiveBound.getPosition().x + ArchiveBound.getSize().x - typing.getSize().x - 1), ArchiveBound.getPosition().y + 1 });
        _renderTexture.draw(typing);
        _renderTexture.display();

        sf::Texture tmp;
        tmp = _renderTexture.getTexture();

        return tmp;
    }

private:

    sf::Texture _elem;

};

class NameList : public CSSlider {

public:

    //start{x,y}, size{}
    NameList(sf::Vector2f center, sf::Vector2f size)
        : CSSlider(center, size) {
            this->display();
    }

    //display
    void display() {        

        //*******************************font

        sf::Font font;
        font.loadFromFile(_atlas_);

        //*******************************elem

        elemBuilder build;

        //*******************************display
    
        size_t size = list.size();


        if(size < 8)
            _renderTexture.create(sizeX, sizeY );
        else
            _renderTexture.create(sizeX, sizeY + (size_elem_y * (size - 8)));

        _renderTexture.clear(sf::Color::White);

        float y = 0.f;

        for (auto elem : list) {
        
            sf::RectangleShape tmp1;
            tmp1.setPosition({ startX, y });
            tmp1.setSize({ size_elem_x, size_elem_y });

            sf::Texture tmp;

            switch(elem.second.type) {
            
                case CSElem::ZeroMsg: {
            
                    tmp = build.buildZeroMsg(elem.first.c_str(), font);
                    break;

                }
                case CSElem::SomeMsg: {

                    tmp = build.buildSomeMsg(elem.first.c_str(), font);
                    break;

                }
                case CSElem::Typing: {

                    tmp = build.buildTyping(elem.first.c_str(), font);
                    break;

                }
                case CSElem::Archive: {

                    tmp = build.buildArchive(elem.first.c_str(), font);
                    break;

                }
            
            };

            tmp1.setTexture(&tmp);

            _renderTexture.draw(tmp1);

            y += size_elem_y;

        }

        _renderTexture.display();
        _update = true;
    }

    //add
    void add(const char* name, CSElem elem) {//this tmpAdd

        if (list.find(name) != list.end()) {
        
            return;
        
        }

        list[name] = elem;
        display();
    }

    //operator[]
    CSElem& operator[](const char* name){        

        return list[name];
    
    }
	
	void erase(const char* name){
		
		list.erase(name);
		
	}
    
    bool empty(){
        
        return list.empty();
        
    }
    
    const char* setActiveElem(float y) {
        
        size_t size =list.size();
        
        if(!size)
            return nullptr;    
        
        if( (Y + y) > (size * size_elem_y))
            return nullptr;   
    
        unsigned int indexElem = (Y + y) / size_elem_y;      

        auto it = list.begin();
        std::advance(it, indexElem);

        if (last) 
            last->type = CSElem::ZeroMsg;
        
        last = &it->second;
        last->type = CSElem::Typing;

        display();

        return it->first.c_str();
    }
     
    //resize    

private:    
        
    std::map<std::string, CSElem> list;

    float size_elem_x = 250.f;
    float size_elem_y = 80.f;

    CSElem* last = nullptr;

};

#endif //_CS_NAME_LIST_
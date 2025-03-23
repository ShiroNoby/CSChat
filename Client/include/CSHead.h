#ifndef _CS_HEAD_
#define _CS_HEAD_

#include "CSRenderTexture.h"

class HeadText : public CSRenderTexture {

public:

    HeadText(sf::Vector2f center, sf::Vector2f size)
        : CSRenderTexture(center, size) {
            _renderTexture.create(sizeX, sizeY);
            _renderTexture.clear(sf::Color::White);
    }

    void add(const char* str) {    

        //*******************************font

        sf::Font font;
        font.loadFromFile(_atlas_);

        //*******************************add
        
        sf::Text Headtext;
        Headtext.setString(str);
        Headtext.setFont(font);
        Headtext.setFillColor(sf::Color::Black);
    
        Headtext.setPosition(sizeX / 5, sizeY / 4);
        _renderTexture.draw(Headtext);
        _renderTexture.display();

        _update = true;
    }

    //resize

};

#endif _CS_HEAD_
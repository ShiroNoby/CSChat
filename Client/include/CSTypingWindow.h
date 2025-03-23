#ifndef _CS_TYPing_WIND_
#define _CS_TYPing_WIND_

#include "CSSlider.h"

#include <string>

class TypingWindow : public CSSlider {

public:

    TypingWindow(sf::Vector2f center, sf::Vector2f size)
        : CSSlider(center, size) {
            this->display();
    }

    void display() {

        sf::Font font;
        font.loadFromFile(_atlas_);
        
        unsigned int countLineCharacter = msg.size() / countCharacter; //30 is count character in line
    
        if (countLineCharacter < sizeY / sizeCharacter)
            _renderTexture.create(sizeX, sizeY);
        else
            _renderTexture.create(sizeX, sizeY + (sizeCharacter * countLineCharacter));

        _renderTexture.clear(sf::Color::White);

        sf::Text text;
        text.setFont(font);
        text.setFillColor(sf::Color::Black);
        text.setLetterSpacing(2.5f);
        
        if(msg.empty())
            _renderTexture.clear(sf::Color::Cyan);
        

        for (unsigned int index = 0; index <= countLineCharacter; ++index) {

            text.setCharacterSize(sizeCharacter);
            text.setPosition(0.f, (index * sizeCharacter));
            text.setString(msg.substr(index * countCharacter, countCharacter)); //substr

            _renderTexture.draw(text);

        }

        _renderTexture.display();
        _update = true;
            
    }

    void add(int keyVal) { //rework

        if (keyVal >= 0 && keyVal <= 25) 
            msg += (char)(keyVal + 97);
        else if (keyVal >= 26 && keyVal <= 35) 
            msg += std::to_string((keyVal - 26));
        else {
        
            switch (keyVal) {

                case sf::Keyboard::Key::Comma: {

                    msg += ',';
                    break;

                }
                case sf::Keyboard::Key::Period: {

                    msg += '.';
                    break;

                }
                case sf::Keyboard::Key::Space: {

                    msg += ' ';
                    break;

                }
            }        
        }   

        display();

    }

    std::string msg;

    unsigned int sizeCharacter = 15;
    unsigned int countCharacter = 50;
	
};

#endif //_CS_TYPing_WIND_
#ifndef _CS_CHAT_
#define _CS_CHAT_

#include "CSTextRender.h"
#include "CSTypingWindow.h"

class Chat : public CSRenderTexture {

public:

    Chat(sf::Vector2f center, sf::Vector2f size)
        : CSRenderTexture(center, size),
        elem1({ 0.f, 0.f }, { size.x, size.y - (size.y / 4) }),
        elem2({ 10.f, size.y + 10 - (size.y / 4) }, { size.x - 20, (size.y / 4) - 20 }) {
    
            _renderTexture.create(size.x, size.y);

            elem1.display();
            elem2.display();

            this->display();

    }

    void display() {

        _shape.setFillColor(sf::Color::White);
        _renderTexture.clear(sf::Color::White);

        _renderTexture.draw(elem1.getShape());//проверить в getshape инициализацию текстуры
        _renderTexture.draw(elem2.getShape()); //проверить в getshape инициализацию текстуры 

        _renderTexture.display();

        _update = true;        
        
    }

    sf::RectangleShape& getShape() {

        if (elem1._update || elem2._update)
            display();

        return CSRenderTexture::getShape();

    }

    bool update() {
    
        return elem1._update || elem2._update;
    
    }

    TextRender elem1;
    TypingWindow elem2;

};

#endif //_CS_CHAT_
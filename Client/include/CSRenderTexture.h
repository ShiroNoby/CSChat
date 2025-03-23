#ifndef _CS_REND_TEXT_
#define _CS_REND_TEXT_

#include "pch.h"

#include <SFML/Graphics.hpp>

class CSRenderTexture : public sf::NonCopyable {

public:

    explicit CSRenderTexture(sf::Vector2f center, sf::Vector2f size) 
        : _shape(center), startX(center.x), startY(center.y), sizeX(size.x), sizeY(size.y) {             
            _shape.setSize(size); 

            _renderTexture.create(sizeX, sizeY);
            _renderTexture.clear(sf::Color::White);
    }

    bool update() { return _update; }

    void setPosition(float x, float y) {
    
        _shape.setPosition(x, y);
    
    }

    sf::RectangleShape& getShape() {

        if (_update) {

            _texture = _renderTexture.getTexture();
            _shape.setTexture(&_texture);
            _shape.setPosition(startX, startY);

        }

        _update = false;
        return _shape;

    }
    
    sf::Texture _texture;
    sf::RectangleShape _shape;
    sf::RenderTexture _renderTexture;
    float startX, startY, sizeX, sizeY;
    bool _update = false;
	
};

#endif //_CS_REND_TEXT_
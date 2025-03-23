#ifndef _CS_SLIDER_
#define _CS_SLIDER_

#include "CSRenderTexture.h"

class CSSlider : public CSRenderTexture {

public:

    CSSlider(sf::Vector2f center, sf::Vector2f size)
        : CSRenderTexture(center, size) {}

    //getShape
    sf::RectangleShape& getShape() {

        if (_update) {

            _texture = _renderTexture.getTexture();
            _shape.setTexture(&_texture, true);
            _shape.setTextureRect(sf::IntRect(0, Y, sizeX, sizeY));
            _shape.setPosition(startX, startY);

        }

        _update = false;
        return _shape;

    }

    //up
    void up(unsigned int val = 50) {

        if (Y > 0) 
            Y -= val;
        else 
            Y = 0;

        _update = true;

    }

    //down
    void down(unsigned int val = 50) {

        //sizeY
        if ((int)(Y + sizeY) < _renderTexture.getSize().y)
            Y += val;
        else
            Y = _renderTexture.getSize().y - sizeY;

        _update = true;
    }

protected:

    int Y = 0;

};

#endif //_CS_SLIDER_
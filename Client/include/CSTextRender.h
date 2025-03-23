#ifndef _CS_TEXT_REND_
#define _CS_TEXT_REND_

#include "CSSlider.h"

#include <fstream>
#include <string>
#include <vector>

class TextRender : public CSSlider {

public:

    TextRender(sf::Vector2f center, sf::Vector2f size)
        : CSSlider(center, size) {
            this->display();
    }

    void display() {        

        _shape.setFillColor(sf::Color::White);
        _renderTexture.clear(sf::Color::White);

        unsigned int countLineCharacter = 0; //30 is count character in line

        //Y += _renderTexture.getSize().y / 2;
        Y = 0;
        correct = 0.f;

        const size_t size = list.size();

        if (size) 
            for (size_t index = 0, end = size; index < end; ++index)
                countLineCharacter += ((list[index].size() - 2) / countCharacter) + 1; //owerflow

        float renderSize = (countLineCharacter * sizeCharacter) + (countLineCharacter * offset);

        if (renderSize < sizeY)
            _renderTexture.create(sizeX, sizeY);
        else
            _renderTexture.create(sizeX, renderSize);

        _renderTexture.clear(sf::Color::White);

        if (!countLineCharacter)
            return;

        sf::Font font;
        font.loadFromFile(_atlas_);

        for (size_t index = 0, end = size; index < end; ++index) {

            sf::Text text;
            text.setFillColor(sf::Color::Black);
            text.setFont(font);
        
            if (list[index].substr(0, 2).compare("L.") == 0) {
            
                unsigned int tmp = ((list[index].size() - 2) / countCharacter) + 1;

                for (unsigned int in = 0; in < tmp; ++in) {

                    text.setCharacterSize(sizeCharacter);
                    text.setPosition(offsetL, correct + offset + (in * sizeCharacter));
                    text.setString(list[index].substr(in * countCharacter + 2, countCharacter)); //substr

                    _renderTexture.draw(text);

                }

                _renderTexture.display();

                correct += offset + (tmp * sizeCharacter);
            
            }
            if (list[index].substr(0, 2).compare("R.") == 0) {

                unsigned int tmp = ((list[index].size() - 2) / countCharacter) + 1;

                for (unsigned int in = 0; in < tmp; ++in) {

                    text.setCharacterSize(sizeCharacter);
                    text.setPosition(offsetR, correct + offset + (in * sizeCharacter));
                    text.setString(list[index].substr(in * countCharacter + 2, countCharacter)); //substr

                    _renderTexture.draw(text);

                }

                _renderTexture.display();

                correct += offset + (tmp * sizeCharacter);

            }        
        }

        _update = true;
    
    }

    void chat(const char* name) {

        pathFile = _chat_;
        pathFile += name;
        pathFile += ".txt";

        loadFromFile();
    
    }

    //load from file to msg
    void loadFromFile() {

        std::fstream file(pathFile, std::fstream::in);

        if (!file.is_open()) {

            file.clear();
            file.open(pathFile, file.out); // create file
            file.close();
            file.open(pathFile);
        }

        list.clear();        

        for (std::string str; std::getline(file, str);)
            list.push_back(str);

        file.close();

        display();
    }

    //add str to another name.txt or active name.txt
    void loadToFile(std::string* str, const char* name = nullptr, const char* prefix = "L.") {
            
        std::string path;

        if (name == nullptr) //check
            path = pathFile;
        else {

            path += _chat_;
            path += name;
            path += ".txt";

        }

        std::fstream file;

        file.open(path, std::fstream::app);

        if (!file.is_open()) {

            file.clear();
            file.open(path, file.out); // create file
            file.close(); 
            file.open(path, file.app);

        }
        
        file << prefix << str->c_str() << '\n';

        file.close();
    
    }

    //void close(name)

    std::vector<std::string> list;
    std::string pathFile;

    unsigned int sizeCharacter = 15;
    unsigned int countCharacter = 50; //syzeY - 2 offsetL(for L.) or offsetL + offsetR(for R.) / sizeCharacter

    const float offsetL = 30.f;
    const float offsetR = 150.f;
    const float offset = 20.f;
    float correct = 0.f;
	
};

#endif //_CS_TEXT_REND_
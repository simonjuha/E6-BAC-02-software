#pragma once
#include <memory>
#include <vector>
#include <Arduino.h>
#include "menuStructure/parameters.hpp"
#include "displayDriver/uiDraw.hpp"

class IDisplayable {
    public:
        virtual void display() = 0;
};

class StaticTextUI : public IDisplayable {
    public:
        StaticTextUI(std::string text){
            _text = text;
        }
        void display(){
            Serial.println(_text.c_str());
        }
    private:
        std::string _text;
};

class ParameterUI : public IDisplayable {
    public:
        ParameterUI(std::shared_ptr<IParameterControl> parameter){
            _parameter = parameter;
        }
        void display(){
            Serial.println(_parameter->name().c_str());
            Serial.println(_parameter->value().c_str());
        }
    private:
        std::shared_ptr<IParameterControl> _parameter;
};

class MenuUI {
    public:
        void addUIElement(std::shared_ptr<IDisplayable> displayable){
            _displayables.push_back(displayable);
        }
        void display(){
            for (auto displayable : _displayables)
            {
                displayable->display();
            }
        }
    private:
    std::vector<std::shared_ptr<IDisplayable>> _displayables;
};
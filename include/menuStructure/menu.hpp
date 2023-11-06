#pragma once
#include <memory>
#include <vector>
#include <Arduino.h>
#include "menuStructure/parameters.hpp"
#include "displayDriver/uiDraw.hpp"
#include "displayDriver/displayDriver.hpp"


class IDisplayable {
    public:
        virtual std::string string() = 0;
};

class StaticTextUI : public IDisplayable {
    public:
        StaticTextUI(std::string text){
            _text = text;
        }
        std::string string(){
            return _text;
        }
        
    private:
        std::string _text;
};

class ParameterUI : public IDisplayable {
    public:
        ParameterUI(std::shared_ptr<IParameterControl> parameter){
            _parameter = parameter;
        }
        std::string string(){
            return _parameter->name() + ": " + _parameter->value();
        }

    private:
        std::shared_ptr<IParameterControl> _parameter;
        bool _isSelected = false;
};

class MenuUI {
    public:
        void addUIElement(std::shared_ptr<IDisplayable> displayable){
            _displayables.push_back(displayable);
        }
        std::shared_ptr<IDisplayable> getSelected(){
            return _displayables[_index];
        }
        void refresh(){
            _lines.clear();
            for (auto displayable : _displayables)
            {
                _lines.push_back(displayable->string());
            }
            DisplayDriver::getInstance().setLines(_lines);
            DisplayDriver::getInstance().select(_index);
        }

        int size(){
            return _displayables.size()-1;
        }

        void next(){
            if(_index < size()){
                _index++;
            }else{
                _index = 0;
            }
            DisplayDriver::getInstance().select(_index);
        }
        void previous(){
            if(_index > 0){
                _index--;
            }else{
                _index = size();
            }
            DisplayDriver::getInstance().select(_index);
        }
    private:
    std::vector<std::shared_ptr<IDisplayable>> _displayables;
    std::shared_ptr<IDisplayable> _selected;
    std::vector<std::string> _lines;
    int _index = 0;

};
#pragma once
#include <memory>
#include <vector>
#include <Arduino.h>
#include "menuStructure/parameters.hpp"
#include "displayDriver/uiDraw.hpp"
#include "displayDriver/displayDriver.hpp"


class MenuUI {
    public:
        void addUIElement(std::shared_ptr<IParameterControl> displayable){
            _parameters.push_back(displayable);
        }
        std::shared_ptr<IParameterControl> getSelected(){
            return _selected;
        }
        void refresh(){
            _lines.clear();
            for(int i = 0; i < _parameters.size(); i++){
                _lines.push_back(getParameterString(_parameters[i]));
            }
            DisplayDriver::getInstance().setLines(_lines);
            DisplayDriver::getInstance().select(_selectedIndex);
        }

        int size(){
            return _parameters.size()-1;
        }

        void next(){
            if(_selectedIndex < size()){
                _selectedIndex++;
            }else{
                _selectedIndex = 0;
            }
            DisplayDriver::getInstance().select(_selectedIndex);
            _selected = _parameters[_selectedIndex];
        }
        void previous(){
            if(_selectedIndex > 0){
                _selectedIndex--;
            }else{
                _selectedIndex = size();
            }
            DisplayDriver::getInstance().select(_selectedIndex);
            _selected = _parameters[_selectedIndex];
        }
    private:
    std::string getParameterString(std::shared_ptr<IParameterControl> parameter){
        return parameter->name() + " : " + parameter->value();
    }
    std::vector<std::shared_ptr<IParameterControl>> _parameters;
    std::shared_ptr<IParameterControl> _selected;
    std::vector<std::string> _lines;
    int _selectedIndex = 0;

};
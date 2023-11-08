#pragma once
#include <memory>
#include <vector>
#include <Arduino.h>
#include "menuStructure/parameters.hpp"
#include "displayDriver/displayDriver.hpp"


class MenuUI {
    public:
        void addUIElement(std::shared_ptr<IParameterControl> displayable){
            _parameters.push_back(displayable);
        }
        std::shared_ptr<IParameterControl> getSelected(){
            if(_selected == nullptr){
                _selected = _parameters[_selectedIndex];
            }
            return _selected;
        }
        void refresh(){
            if(_parameters.size() == 0){
                ESP_LOGW("MenuUI", "No parameters to display");
                return;
            }
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

class MenuSelector{
    public:
        MenuSelector(){}
        void addMenu(MenuUI * menu){
            _menus.push_back(menu);
            if(_selectedMenu == nullptr){
                _selectedMenu = menu;
            }
        }
        void select(int index){
            if(index < _menus.size()){
                _selectedMenu = _menus[index];
                DisplayDriver::getInstance().clear();
                _selectedMenu->refresh();
            }else{
                ESP_LOGW("MenuSelector", "Invalid menu index");
            }
        }
        MenuUI* getSelectedMenu(){
            return _selectedMenu;
        }

    private:
        std::vector<MenuUI*> _menus;
        MenuUI* _selectedMenu;
};

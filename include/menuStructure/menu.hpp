#pragma once
#include <memory>
#include <vector>
#include <Arduino.h>
#include "menuStructure/parameters.hpp"
#include "displayDriver/displayDriver.hpp"


class MenuUI {
    public:
        void addUIElement(std::shared_ptr<IParameterControl> displayable){
            _parameters.push_back(displayable); // add parameter to vector
        }
        std::shared_ptr<IParameterControl> getSelected(){ // get the currently selected parameter
            if(_selected == nullptr){
                _selected = _parameters[_selectedIndex]; // if no parameter is selected, select the first one
            }
            return _selected;
        }
        void refresh(){
            if(_parameters.size() == 0){ // if no parameters, return
                ESP_LOGW("MenuUI", "No parameters to display");
                return;
            }
            _lines.clear(); // clear lines vector
            for(int i = 0; i < _parameters.size(); i++){
                _lines.push_back(getParameterString(_parameters[i])); // add parameter string to lines vector from parameter vector
            }
            DisplayDriver::getInstance().setLines(_lines);      // set lines to display (displayDriver)
            DisplayDriver::getInstance().select(_selectedIndex);// select the first parameter (displayDriver)
        }

        int size(){
            return _parameters.size()-1;
        }

        void next(){
            if(_selectedIndex < size()){ // if not last parameter, select next
                _selectedIndex++;
            }else{
                _selectedIndex = 0; // if last parameter, select first
            }
            DisplayDriver::getInstance().select(_selectedIndex); // set selected index (displayDriver)
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
    std::string getParameterString(std::shared_ptr<IParameterControl> parameter){ // get parameter as string -> name : value
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
        void addMenu(MenuUI * menu){ // add menu to vector
            _menus.push_back(menu);
            if(_selectedMenu == nullptr){
                _selectedMenu = menu;
            }
        }
        void select(int index){
            if(index < _menus.size()){
                _selectedMenu = _menus[index];
                _selectedMenu->refresh();
            }else{
                ESP_LOGW("MenuSelector", "Invalid menu index");
            }
        }
        MenuUI* getSelectedMenu(){ // get selected menu
            return _selectedMenu;
        }

    private:
        std::vector<MenuUI*> _menus;
        MenuUI* _selectedMenu;
};

#pragma once
#include "rotaryEncoderDriver/quardratureDriver.hpp"
#include "menuStructure/menu.hpp"

class SelectorRotary : public IRotaryObserver{
    public:
        SelectorRotary(){}
        SelectorRotary(MenuSelector *ui) : _rotary(35, 36)
        {
            _rotary.attach(this);
            _menuUISelector = ui;
        }
        void up(){
            _menuUISelector->getSelectedMenu()->next();

        }
        void down(){
            _menuUISelector->getSelectedMenu()->previous();
        }
    private:
        Quadrature _rotary;
        MenuSelector *_menuUISelector;
        MenuUI *_selected;
};

// change parameter value
class ValueRotary : public IRotaryObserver{
    public:
        ValueRotary(){}
        ValueRotary(MenuSelector *ui) : _rotary(2, 1)
        {
            _rotary.attach(this);
            _menuUISelector = ui;
        }
        void up(){
            parameter()->increment();
            _menuUISelector->getSelectedMenu()->refresh();
        }
        void down(){
            parameter()->decrement();
            _menuUISelector->getSelectedMenu()->refresh();
        }
    private:
        std::shared_ptr<IParameterControl> parameter(){
            return _menuUISelector->getSelectedMenu()->getSelected();
        }
        Quadrature _rotary;
        MenuSelector *_menuUISelector;
};

#pragma once
#include "rotaryEncoderDriver/quardratureDriver.hpp"
#include "menuStructure/menu.hpp"

class SelectorRotary : public IRotaryObserver{
    public:
        SelectorRotary(){}
        SelectorRotary(MenuUI *ui) : _rotary(35, 36)
        {
            _rotary.attach(this);
            _menuUI = ui;
        }
        void up(){
            _menuUI->next();
        }
        void down(){
            _menuUI->previous();
        }
    private:
        MenuUI *_menuUI;
        Quadrature _rotary;
        bool isUp;
        bool isDown;
};

// change parameter value
class ValueRotary : public IRotaryObserver{
    public:
        ValueRotary(){}
        ValueRotary(MenuUI *ui) : _rotary(2, 1)
        {
            _rotary.attach(this);
            _menuUI = ui;
            _parameter = _menuUI->getSelected();
        }
        void up(){
            update();
            _parameter->increment();
            _menuUI->refresh();
        }
        void down(){
            update();
            _parameter->decrement();
            _menuUI->refresh();
        }
    private:
        void update(){
            if(_menuUI->getSelected() != _parameter){
                _parameter = _menuUI->getSelected();
            }
        }
        std::shared_ptr<IParameterControl> _parameter;
        Quadrature _rotary;
        MenuUI *_menuUI;
        bool isUp;
        bool isDown;
};
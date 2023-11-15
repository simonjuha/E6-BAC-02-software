#pragma once
#include "rotaryEncoderDriver/quardratureDriver.hpp"
#include "menuStructure/menu.hpp"

class BaseRotary : public IRotaryObserver {
public:
    BaseRotary(int pin1, int pin2, MenuSelector *ui) : _rotary(pin1, pin2) {
        _rotary.attach(this);
        _menuUISelector = ui;
    }

protected:
    Quadrature _rotary;
    MenuSelector *_menuUISelector;
};

class SelectorRotary : public BaseRotary {
public:
    SelectorRotary() : BaseRotary(2, 1, nullptr) {}
    SelectorRotary(MenuSelector *ui) : BaseRotary(2, 1, ui) {}

    void up() {
        _menuUISelector->getSelectedMenu()->next();
    }

    void down() {
        _menuUISelector->getSelectedMenu()->previous();
    }
};

class ValueRotary : public BaseRotary {
public:
    ValueRotary() : BaseRotary(35, 36, nullptr) {}
    ValueRotary(MenuSelector *ui) : BaseRotary(35, 36, ui) {}
    void up() {
        parameter()->increment();
        _menuUISelector->getSelectedMenu()->refresh();
    }
    void down() {
        parameter()->decrement();
        _menuUISelector->getSelectedMenu()->refresh();
    }
    private:
        std::shared_ptr<IParameterControl> parameter(){
        return _menuUISelector->getSelectedMenu()->getSelected();
    }
};
#pragma once
#include <string>
#include <Arduino.h>

#include "parameters.hpp"
#include "displayDriver/uiDraw.hpp"

/* INTEGER PARAMETER */

IntParameter::IntParameter(std::string name, int min, int max, int defaultValue){
    _name = name;
    _min = min;
    _max = max;
    _value = defaultValue;
    _defaultValue = defaultValue;
}

void IntParameter::accept(IDrawVisitor * visitor){
    visitor->visit(this);
}

std::string IntParameter::name(){
    return _name;
}

std::string IntParameter::value(){
    return std::to_string(_value);
}

void IntParameter::increment(){
    if (_value < _max)
    {
        _value++;
    }
}

void IntParameter::decrement(){
    if (_value > _min)
    {
        _value--;
    }
}

void IntParameter::reset(){
    _value = _defaultValue;
}








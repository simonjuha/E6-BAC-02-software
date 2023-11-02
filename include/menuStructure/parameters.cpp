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
        update(_name, _value);
    }
}

void IntParameter::decrement(){
    if (_value > _min)
    {
        _value--;
        update(_name, _value);

    }
}

void IntParameter::reset(){
    _value = _defaultValue;
    update(_name, _value);
}

/* OPTION PARAMETER */

OptionParameter::OptionParameter(std::string name, std::vector<std::string> options, int defaultValue){
    _name = name;
    _options = options;
    _numOptions = options.size();
    _value = defaultValue;
    _defaultValue = defaultValue;
}

void OptionParameter::accept(IDrawVisitor * visitor){
    visitor->visit(this);
}

std::string OptionParameter::name(){
    return _name;
}

std::string OptionParameter::value(){
    return _options[_value];
}

void OptionParameter::increment(){
    if (_value < _numOptions - 1)
    {
        _value++;
        update(_name, _value);
    }
}

void OptionParameter::decrement(){
    if (_value > 0)
    {
        _value--;
        update(_name, _value);
    }
}

void OptionParameter::reset(){
    _value = _defaultValue;
    update(_name, _value);
}

/* FLOAT PARAMETER */

FloatParameter::FloatParameter(std::string name, float min, float max, float defaultValue){
    _name = name;
    _min = min;
    _max = max;
    _value = defaultValue;
    _defaultValue = defaultValue;
}

void FloatParameter::accept(IDrawVisitor * visitor){
    visitor->visit(this);
}

std::string FloatParameter::name(){
    return _name;
}

std::string FloatParameter::value(){
    return std::to_string(_value);
}

void FloatParameter::increment(){
    if (_value < _max)
    {
        _value++;
        update(_name, _value);
    }
}

void FloatParameter::decrement(){
    if (_value > _min)
    {
        _value--;
        update(_name, _value);
    }
}

void FloatParameter::reset(){
    _value = _defaultValue;
    update(_name, _value);
}











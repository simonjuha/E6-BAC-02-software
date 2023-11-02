#pragma once
#include <Arduino.h>

#include "displayDriver/uiDraw.hpp"
#include "menuStructure/parameters.hpp"
#include "displayDriver/displayDriver.hpp"

void DefaultDrawVisitor::visit(IntParameter * element){
    DisplayDriver::getInstance().writeLine((element->name() + " :visit: " + element->value() ).c_str());
}

void DefaultDrawVisitor::visit(OptionParameter * element){
    DisplayDriver::getInstance().writeLine((element->name() + " :visit: " + element->value() ).c_str());
}

void DefaultDrawVisitor::visit(FloatParameter * element){
    DisplayDriver::getInstance().writeLine((element->name() + " :visit: " + element->value() ).c_str());
}




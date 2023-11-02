#pragma once
#include <Arduino.h>

#include "displayDriver/uiDraw.hpp"
#include "menuStructure/parameters.hpp"

void DefaultDrawVisitor::visit(IntParameter * element){
    Serial.println((element->name() + " : " + element->value() ).c_str());
}

void DefaultDrawVisitor::visit(OptionParameter * element){
    Serial.println((element->name() + " : " + element->value() ).c_str());
}

void DefaultDrawVisitor::visit(FloatParameter * element){
    Serial.println((element->name() + " : " + element->value() ).c_str());
}




#pragma once
#include <Arduino.h>

#include "displayDriver/uiDraw.hpp"
#include "menuStructure/parameters.hpp"

void DefaultDrawVisitor::visit(IntParameter * element){
    Serial.println(element->name().c_str());
    Serial.println(element->value().c_str());
}



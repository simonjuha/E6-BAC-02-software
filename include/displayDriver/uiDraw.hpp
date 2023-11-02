#pragma once
#include <Arduino.h>


// forward declaration
class IDrawVisitor;
class IntParameter;

class IDrawVisitor {
    public:
    virtual void visit(IntParameter * element) = 0;
    virtual void visit(OptionParameter * element) = 0;
    virtual void visit(FloatParameter * element) = 0;
};

class DefaultDrawVisitor : public IDrawVisitor {
    public:
    void visit(IntParameter * element) override;
    void visit(OptionParameter * element) override;
    void visit(FloatParameter * element) override;
};

#include "displayDriver/uiDraw.cpp"
#pragma once
#include <string>
#include <vector>

// forward declaration
class IDrawVisitor;

class IDrawElement {
    public:
    virtual void accept(IDrawVisitor* visitor) = 0;
};

class IParameterString {
    public:
    virtual std::string name() = 0;
    virtual std::string value() = 0;
};

class IParameterControl{
    public:
    virtual void increment() = 0;
    virtual void decrement() = 0;
    virtual void reset() = 0;
};




class IntParameter : public IDrawElement, public IParameterString, public IParameterControl{
    public:
        IntParameter(std::string name = "unknown", int min = 0, int max = 100, int defaultValue = 0);
        void accept(IDrawVisitor* visitor) override;
        std::string name() override;
        std::string value() override;
        void increment() override;
        void decrement() override;
        void reset() override;
    private:
        int _defaultValue = 0;
        int _value = 0;
        int _min = 0;
        int _max = 100;
        std::string _name;
};

// parameters for selection options (enum of strings)
class OptionParameter : public IDrawElement, public IParameterString, public IParameterControl{
    public:
        OptionParameter(std::string name = "unknown", std::vector<std::string> options = {"option1", "option2", "option3"}, int defaultValue = 0);
        void accept(IDrawVisitor* visitor) override;
        std::string name() override;
        std::string value() override;
        void increment() override;
        void decrement() override;
        void reset() override;
    private:
        int _defaultValue;
        int _value;
        int _numOptions;
        std::vector<std::string> _options;
        std::string _name;
};

#include "menuStructure/parameters.cpp"
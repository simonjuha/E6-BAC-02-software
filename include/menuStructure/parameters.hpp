#pragma once
#include <string>
#include <vector>
#include "menuStructure/parameterObserver.hpp"

// forward declaration
class IDrawVisitor;

class IDrawElement {
    public:
    virtual void accept(IDrawVisitor* visitor) = 0;
};

class IParameterControl : public IDrawElement{
    public:
    virtual void increment() = 0;
    virtual void decrement() = 0;
    virtual void reset() = 0;
    virtual std::string name() = 0;
    virtual std::string value() = 0;
    virtual void accept(IDrawVisitor* visitor) = 0;
};


class IntParameter :    public IParameterControl, 
                        public DataUpdateSubject
{
    public:
        IntParameter(std::string name = "unknown", int min = 0, int max = 100, int defaultValue = 0);
        // IDrawElement
        void accept(IDrawVisitor* visitor) override;
        // IParameterString
        std::string name() override;
        std::string value() override;
        // IParameterControl
        void increment() override;
        void decrement() override;
        void reset() override;
    private:
        std::map<std::string, IParameterObserver*> _observers;
        int _defaultValue = 0;
        int _value = 0;
        int _min = 0;
        int _max = 100;
        std::string _name;
        
};

// parameters for selection options (enum of strings)
class OptionParameter : public IParameterControl,
                        public DataUpdateSubject
{
    public:
        OptionParameter(std::string name = "unknown", std::vector<std::string> options = {"option1", "option2", "option3"}, int defaultValue = 0);
        // IDrawElement
        void accept(IDrawVisitor* visitor) override;
        // IParameterString
        std::string name() override;
        std::string value() override;
        // IParameterControl
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

class FloatParameter :  public IParameterControl,
                        public DataUpdateSubject
{
    public:
        FloatParameter(std::string name = "unknown", float min = 0, float max = 100, float defaultValue = 0);
        // IDrawElement
        void accept(IDrawVisitor* visitor) override;
        // IParameterString
        std::string name() override;
        std::string value() override;
        // IParameterControl
        void increment() override;
        void decrement() override;
        void reset() override;
    private:
        float _defaultValue;
        float _value;
        float _min;
        float _max;
        std::string _name;
};


#include "menuStructure/parameters.cpp"
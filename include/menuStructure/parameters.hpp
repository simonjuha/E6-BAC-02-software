#pragma once
#include <string>
#include <vector>
#include "menuStructure/parameterObserver.hpp"

class IParameterControl{
    public:
    virtual void increment() = 0;
    virtual void decrement() = 0;
    virtual void reset() = 0;
    virtual std::string name() = 0;
    virtual std::string value() = 0;
};


class IntParameter :    public IParameterControl, 
                        public DataUpdateSubject
{
    public:
        IntParameter(std::string name = "unknown", int min = 0, int max = 100, int defaultValue = 0);
        // IParameterString
        std::string name() override;
        std::string value() override;
        // IParameterControl
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
class OptionParameter : public IParameterControl,
                        public DataUpdateSubject
{
    public:
        OptionParameter(std::string name = "unknown", std::vector<std::string> options = {"option1", "option2", "option3"}, int defaultValue = 0);
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
        FloatParameter(std::string name = "unknown", float min = 0, float max = 100, float defaultValue = 0, float increment = 0.01);
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
        float _increment;
        std::string _name;
};


#include "menuStructure/parameters.cpp"
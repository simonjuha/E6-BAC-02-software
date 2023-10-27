#include <string>
#include <vector>
#include <optional>

template <typename T = int>
class Parameter {
    public:
    // numeric parameter
    Parameter(std::string name = "unknown", T maxVal = 100, T minVal = 0, T defaultVal = 50, T stepSize = 1)
    : _name(name), _maxVal(maxVal), _minVal(minVal), _stepSize(stepSize), _value(defaultVal)
    {

    }

    Parameter(std::string name, std::vector<std::string> options)
    : _name(name), _options(options), _maxVal(options.size() - 1), _minVal(0), _stepSize(1), _value(0)
    {

    }

    bool increment(){
        if (_value + _stepSize <= _maxVal){
            _value += _stepSize;
            return true;
        }
        return false;
    }

    bool decrement(){
        if (_value - _stepSize >= _minVal){
            _value -= _stepSize;
            return true;
        }
        return false;
    }

    T value(){
        if(_options.has_value()){                   // if _options is set
            //return _options.value()[(_value];    // return the string at index
        }
        return _value;                              // else return the numeric value
    }

    std::string label(){
        if (_options.has_value()){                  // if _options is set
            return _options.value()[(int)_value];    // return the string at index
        }
        return std::to_string(_value);              // else return the numeric value
    }

    std::string name(){
        return _name;
    }

    private:
    std::string _name;
    T _value;
    T _maxVal;
    T _minVal;
    T _stepSize;
    std::optional< std::vector<std::string> > _options;
};
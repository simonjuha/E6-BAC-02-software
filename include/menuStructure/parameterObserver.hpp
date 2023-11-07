#include <string>
#include <map>
#include <memory>

#include <Arduino.h>

class IParameterObserver{
    public:
    virtual void update(const std::string& name, float newValue) = 0;
};

class IParameterSubject{
    public:
    virtual void attach(const std::string& name, std::shared_ptr<IParameterObserver> observer) = 0;
    virtual void detach(const std::string& name, std::shared_ptr<IParameterObserver> observer) = 0;
    virtual void update(const std::string& name, float newValue) = 0;
};

class ConcreteTestObserver : public IParameterObserver{
    public:
    void update(const std::string& name, float newValue) override{
        Serial.println("ConcreteTestObserver::update");
        Serial.print("name: ");
        Serial.println(name.c_str());
        Serial.print("newValue: ");
        Serial.println(newValue);
    }
};

class DataUpdateSubject : public IParameterSubject{
    public:
    void attach(const std::string& name, std::shared_ptr<IParameterObserver> observer) {
        _observers[name] = observer;
    }
    void detach(const std::string& name, std::shared_ptr<IParameterObserver> observer) {
        _observers.erase(name);
    }
    void update(const std::string& name, float newValue) {
        for (auto& observer : _observers)
        {
            observer.second->update(name, newValue);
        }
    }
    private:
    std::map<std::string, std::shared_ptr<IParameterObserver>> _observers;
};
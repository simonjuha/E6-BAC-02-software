#include <vector>
#include <algorithm>

class IControlSignalObserver{
    public:
    virtual void update() = 0;
};

class ControlSignalSubject{
    public:
    void attach(IControlSignalObserver* observer){
        _observers.push_back(observer);
    }
    void detach(IControlSignalObserver* observer){
        _observers.erase(std::remove(_observers.begin(), _observers.end(), observer), _observers.end());
    }
    void update(){
        for(auto observer : _observers){
            observer->update();
        }
    }
    private:
    std::vector<IControlSignalObserver*> _observers;
};

class IControlSignalRead{
    public:
    virtual int read() = 0;
};
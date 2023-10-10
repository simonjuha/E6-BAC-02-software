#include <vector>
#include <algorithm>

/*
    Rotary encoder driver
    0 = down
    1 = up
*/


// IRotaryObserver interface
class IRotaryObserver
{
    public:
    virtual void update(bool dir) = 0;
    
};

// IRotarySubject
class IRotarySubject
{
    public:
        virtual void attach(IRotaryObserver *observer);
        virtual void detach(IRotaryObserver *observer);
        virtual void notify(bool dir);
    private:
        std::vector<IRotaryObserver*> observers;
};

void IRotarySubject::attach(IRotaryObserver *observer)
{
    // add observer to vector
    observers.push_back(observer);
}

void IRotarySubject::detach(IRotaryObserver *observer)
{
    // remove observer from vector
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void IRotarySubject::notify(bool dir)
{
    // use short for loop to notify all observers about direction
    for (auto observer : observers)
    {
        observer->update(dir);
    }
}
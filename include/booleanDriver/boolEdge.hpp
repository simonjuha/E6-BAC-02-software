#pragma once

#include <vector>
#include <algorithm>

class IEdgeObserver{
    public:
        virtual void rise() = 0;
        virtual void fall() = 0;
        // checking if the interrupt flag is set and calling the corresponding method
        void tick(){
            if(_interruptRise){
                rise();
                _interruptRise = false;
            }
            if(_interruptFall){
                fall();
                _interruptFall = false;
            }
        }
        // interrupt methods are called by the subject (gpioDriver calls these methods through an interrupt, thus only setting a flag)
        void interruptRise(){
            _interruptRise = true;
        }
        void interruptFall(){
            _interruptFall = true;
        }
    private:
        bool _interruptRise = false;
        bool _interruptFall = false;
};

class EdgeSubject{
    public:
        void attach(IEdgeObserver* o);
        void detach(IEdgeObserver* o);
        void rise();
        void fall();
    private:
        std::vector<IEdgeObserver*> observers;
};

void EdgeSubject::attach(IEdgeObserver* o){
    observers.push_back(o); // add observer to vector
}

void EdgeSubject::detach(IEdgeObserver* o){
    observers.erase(std::remove(observers.begin(), observers.end(), o), observers.end()); // remove observer from vector
}

// sett all observers interrupt flags (rise)
void EdgeSubject::rise(){
    for(auto o : observers){
        o->interruptRise();
    }
}

// set all observers interrupt flags (fall)
void EdgeSubject::fall(){
    for(auto o : observers){
        o->interruptFall();
    }
}
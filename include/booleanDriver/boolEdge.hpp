#pragma once

#include <vector>
#include <algorithm>

class IEdgeObserver{
    public:
        virtual void rise() = 0;
        virtual void fall() = 0;
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
    observers.push_back(o);
}

void EdgeSubject::detach(IEdgeObserver* o){
    observers.erase(std::remove(observers.begin(), observers.end(), o), observers.end());
}

void EdgeSubject::rise(){
    for(auto o : observers){
        o->interruptRise();
    }
}

void EdgeSubject::fall(){
    for(auto o : observers){
        o->interruptFall();
    }
}
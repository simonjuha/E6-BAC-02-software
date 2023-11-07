#pragma once
#include <vector>

// IRotary interface
class IRotaryObserver {
    public:
        virtual void up() = 0;
        virtual void down() = 0;
        void tick(){
            if(upTick){
                upTick = false;
                up();
            }
            else if(downTick){
                downTick = false;
                down();
            }
        }
        // reserved for use with interrupts
        void interruptUp(){
            upTick = true;
        }
        void interruptDown(){
            downTick = true;
        }
    private:
        bool upTick = false;
        bool downTick = false;
};

// RotarySubject class
class RotarySubject {
    public:
        void attach(IRotaryObserver *obs){
            _observers.push_back(obs);
        }

        void detach(IRotaryObserver *obs){
            for(int i = 0; i < _observers.size(); i++){
                if(_observers[i] == obs){
                    _observers.erase(_observers.begin() + i);
                }
            }
        }

        void Up(){
            for(int i = 0; i < _observers.size(); i++){
                _observers[i]->interruptUp();
            }
        }

        void Down(){
            for(int i = 0; i < _observers.size(); i++){
                _observers[i]->interruptDown();
            }
        }

    private:
    std::vector<IRotaryObserver*> _observers;
};
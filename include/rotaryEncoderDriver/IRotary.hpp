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
            if(downTick){
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
            observers.push_back(obs);
        }

        void detach(IRotaryObserver *obs){
            for(int i = 0; i < observers.size(); i++){
                if(observers[i] == obs){
                    observers.erase(observers.begin() + i);
                }
            }
        }

        void Up(){
            for(int i = 0; i < observers.size(); i++){
                observers[i]->interruptUp();
            }
        }

        void Down(){
            for(int i = 0; i < observers.size(); i++){
                observers[i]->interruptDown();
            }
        }

    private:
    std::vector<IRotaryObserver*> observers;
};
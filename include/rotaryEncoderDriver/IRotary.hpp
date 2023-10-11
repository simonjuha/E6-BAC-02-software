#include <vector>

// IRotary interface
class IRotaryObserver {
    public:
        virtual void up() = 0;
        virtual void down() = 0;
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
                observers[i]->up();
            }
        }

        void Down(){
            for(int i = 0; i < observers.size(); i++){
                observers[i]->down();
            }
        }

    private:
    std::vector<IRotaryObserver*> observers;
};
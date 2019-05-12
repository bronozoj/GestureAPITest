#include <iostream>
#include <thread>
#include <cstdio>

#include "gestureapi.hpp"

char dirtext [9][20] = {
    "none", "clockwise", "counterclockwise", "up", "down", "left", "right",
    "forward", "backward"
};

class printThread{
    bool shouldExit = false;
    std::thread workerThread;
    gestureapi::GestureInput client;

    void threadfunc(){
        if(!client.begin()){
            std::cout << "cannot connect" << std::endl;
            return;
        }
        while(!shouldExit){
            int counter = client.input();
            if(counter == -1){
                std::cout << "connection error" << std::endl;
                return;
            }
            if(counter != 0)
            std::cout << "direction: " << dirtext[counter] << std::endl;
        }
    }

    public:
    void begin(){
        workerThread = std::thread(&printThread::threadfunc, this);
    }
    void end(){
        shouldExit = true;
        workerThread.join();
    }
};

int main(void){
    char buffer[50];
    std::cout << "dummy test app" << std::endl;
    printThread work;
    work.begin();
    fgets(buffer, sizeof(buffer), stdin);
    work.end();
    return 0;
}
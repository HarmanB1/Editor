#pragma once


#include <vector>
#include <string>

class Editor{
    public:
        Editor();
        ~Editor();
        void run();

    private:
        void doInput(int ch);
        void getInput();
        void doMouse();
        
        

        std::vector<std::string> content; //content of each line index representing y 
        int cursorX, cursorY;
        bool running; //indicator to stop running program


    

};


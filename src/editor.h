#pragma once
#include <ncurses.h>
#include <vector>
#include <string>
class Editor{
    public:
        Editor();
        ~Editor();
        void run();

    private:
        void moveInput(int ch);
        void getInput();
        void refreshScreen();

        std::vector<std::string> content;
        int cursorX, cursorY;


    

};
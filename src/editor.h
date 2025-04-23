#pragma once
#include <ncurses.h>>
#include <vector>
#include <string>
class Editor{
    public:
        Editor();
        ~Editor();
        void run();

    private:
        void userInput();
        void refreshScreen();

        std::vector<std::string> content;
        int cursorX, cursorY;


    

}
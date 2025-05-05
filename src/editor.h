#pragma once
#include "fileIO.h"
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
        void save(std::vector<std::string>& content, std::vector<std::string>& content_backup);
        void load(std::vector<std::string>& conten, std::vector<std::string>& content_backup);
        void find();
        void settings();
        
        

        std::vector<std::string> content; //content of each line index representing y 
        std::vector<std::string> backup_content;
        int cursorX, cursorY;
        bool running; //indicator to stop running program




};


#pragma once
#include <filesystem>
#include <vector>
#include <string>
#include <ncurses.h>

struct EditorSetting{
    bool autosave= true;
    bool lineNumb = true;
    bool wordWrap = false;
    bool saveOnClose = true;
    int textCol = COLOR_WHITE;
    int statusBarCol = COLOR_GREEN;
    int backgroundCol = COLOR_BLACK;
};

class Editor{
    public:
        Editor();
        ~Editor();
        void run();

    private:
        void doInput(int ch);
        void getInput();
        void doMouse();
        void save(std::string filepath, std::vector<std::string>& content, std::vector<std::string>& content_backup);
        void load(std::string& filepath, std::vector<std::string>& conten, std::vector<std::string>& content_backup);
        void find();
        void settings();
        void applyCol();
       // void refresh();
        
        
        EditorSetting setting;
       // void showSettings();
        void saveSetting();
       // void loadSetting();
       std::string getConfigPath();

       


        std::vector<std::string> content; //content of each line index representing y 
        std::vector<std::string> backup_content;
        int cursorX, cursorY;
        bool running; //indicator to stop running program
        std::string filepath;
        int scrollY; //tracks scroll pos
        




};


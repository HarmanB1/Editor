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
    std::string directory="";
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
        void load(std::string& filepath, std::vector<std::string>& content, std::vector<std::string>& content_backup);
        void direct(std::string& directory);
       // void current(std::string filepath);
       

        
        void settings();
        void applyCol();
        void updateStatus();
       // void refresh();
        
        
        EditorSetting setting;
       // void showSettings();
        void saveSetting();
        void loadSetting();
        std::string getConfigPath();
        
        std::vector<int> colArr = {COLOR_BLACK,COLOR_RED,COLOR_GREEN,COLOR_YELLOW, COLOR_BLUE, COLOR_MAGENTA,COLOR_CYAN, COLOR_WHITE};
        std::vector<std::string> colArrString = {"Black", "Red", "Green", "Yellow", "Blue", "Magenta", "Cyan", "White"};
        int textcol_index=7;
        int statusBarCol_index=2;
        int backgroundCol_index=0;

       


        std::vector<std::string> content; //content of each line index representing y 
        std::vector<std::string> backup_content;
        int cursorX, cursorY;
        bool running; //indicator to stop running program
        std::string filepath;
        std::string directory;
        int scrollY; //tracks scroll pos
        




};


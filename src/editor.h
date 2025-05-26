/**
 * @file editor.h
 * @brief Main editor class for the text editor application
 * 
 * Contains the core editor functionality including input handling, 
 * display management, and file operations.
 * 
 * @author Harman Bhogal
 * @date 2025-5-25
 */

#pragma once
#include <filesystem>
#include <vector>
#include <string>
#include <ncurses.h>
#include "redo.h"
#include <chrono>

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
        //main functions
        void doInput(int ch);
        void getInput();
        void doMouse();
        void save(std::string filepath, std::vector<std::string>& content, std::vector<std::string>& content_backup);
        void load(std::string& filepath, std::vector<std::string>& content, std::vector<std::string>& content_backup);
        void direct(std::string& directory, std::vector<std::string>& content, std::vector<std::string>& content_backup);
    
       void autoSave();
       void lineNumb();
       void wordWrap();
       void saveOnClose();




        //settings functions 
        void settings();
        void applyCol();
        void updateStatus();
 
       //settings related to editor settings struct
        EditorSetting setting;
        void saveSetting();
        void loadSetting();
        std::string getConfigPath();

        //variables for settings
        std::vector<int> colArr = {COLOR_BLACK,COLOR_RED,COLOR_GREEN,COLOR_YELLOW, COLOR_BLUE, COLOR_MAGENTA,COLOR_CYAN, COLOR_WHITE};
        std::vector<std::string> colArrString = {"Black", "Red", "Green", "Yellow", "Blue", "Magenta", "Cyan", "White"};
        int textcol_index=7;
        int statusBarCol_index=2;
        int backgroundCol_index=0;

       
        //members of editor class
        std::vector<std::string> content; //content of each line index representing y 
        std::vector<std::string> backup_content;
        size_t cursorX, cursorY;
        bool running; //indicator to stop running program
        std::string filepath;
        std::string directory;
        int scrollY; //tracks scroll pos

        //history and clipboard
        History editHistory;
        Clipboard editClipboard;

        //state management
        State getCurrentState() const;
        void applyState(const State& state);

        //time managment
        std::chrono::system_clock::time_point lastSaveTime;
        int autoSaveInterval;
        


        




};


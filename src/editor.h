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


/**
 * @struct EditorSettings
 * @brief Stores all user-configurable editor settings
 * 
 * Persisted to disk and loaded on startup
 */

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

/**
 * @class Editor
 * @brief Core text editor implementation
 * 
 * Handles all editor operations including:
 * - Real-time text rendering
 * - File management
 * - User configuration
 * - State history (undo/redo)
 */
class Editor{
    public:
        Editor(); ///< Initializes ncurses and loads settings
        ~Editor(); ///< Cleans up ncurses and saves state

        /**
         * @brief Main editor event loop
         * @note Handles input, rendering, and auto-save
         */
        void run();

    private:

        /* Core Functions */

        /**
         * @brief takes in ASCII key and depending on key does a specfic action(example esc ASCII key ends program).
         * @param ch is ASCII key value of mouse input.
         */
        void doInput(int ch); 

        void getInput(); //< Fetches input (keyboard/mouse) and processes character from user

        void doMouse();///< Handles mouse events.

        /**
         * @brief checks if file exists and if so, uses fileIO:save() to save file, also formats filepath to include directory.
         * 
         *@param filepath Path to the target file.
         *@param content Reference to a vector storing loaded lines.
         *@param content_backup Reference to a vector storing backup of loaded lines.
         */
        void save(std::string filepath, std::vector<std::string>& content, std::vector<std::string>& content_backup);

        /**
         * @brief checks if file exists and if so, uses fileIO:save() to save file, also formats filepath to include directory.
         * 
         *@param filepath Path to the target file.
         *@param content Reference to a vector storing loaded lines.
         *@param content_backup Reference to a vector storing backup of loaded lines.
         */
        void load(std::string& filepath, std::vector<std::string>& content, std::vector<std::string>& content_backup);

        /**
         * @brief Sets or changes the working directory
         * @param directory Reference to current directory path 
         * @param content Reference to current content
         * @param content_backup Reference to backup content
         */
        void direct(std::string& directory, std::vector<std::string>& content, std::vector<std::string>& content_backup);
    
        /**
         * @brief Performs automatic saving if enabled and interval has elapsed
         */
        void autoSave();

        /**
         * @brief Toggles display of line numbers and updates display
         */
        void lineNumb();

        /**
         * @brief Handles word wrapping if enabled
         */
        void wordWrap();

        /**
         * @brief Prompts to save when closing if enabled
         */
        void saveOnClose();

        /* Settings Functions */

        /**
         * @brief Displays and handles settings menu
         */
        void settings();

        /**
         * @brief Applies current color settings to the UI
         */
        void applyCol();

        /**
         * @brief Updates the status bar display
         */
        void updateStatus();
 
        /* functions related to settings struct*/
        EditorSetting setting; ///< Current editor settings

        /**
         * @brief Saves current settings to configuration file
         */
        void saveSetting();

        /**
         * @brief Loads settings from configuration file
         */
        void loadSetting();

        /**
         * @brief Gets the path to the configuration file
         * @return std::string containing the full config file path
         */
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


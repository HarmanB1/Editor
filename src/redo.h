/**
 * @file redo.h
 * @brief Contains class functions related to cutting, copying, pasting, redoing, and undoing
 * 
 * @author Harman Bhogal
 * @date 2025-5-25
 */

#pragma once
#include <vector>
#include <string>
#include <cstddef>

/**
 * @struct State
 * @brief Represents the editor state at a given time.
 */
struct State{
    std::vector<std::string> content;
    size_t cursorX;
    size_t cursorY;
    std::string filepath;
    std::string directory;

    /**
    * @brief Constructs a State object with the given parameters.
    * @param c Lines of text content
    * @param x Cursor horizontal position
    * @param y Cursor vertical position (line number)
    * @param f File path of the document
    * @param d Directory of the file
    */
    State(
        std::vector<std::string> c,
        size_t x,
        size_t y,
        std::string f,
        std::string d)
    
        : content(c), cursorX(x), cursorY(y), filepath(f), directory(d){}

};

/**
 * @class History
 * @brief Manages undo and redo stacks of editor states.
 */
class History{
    public:

        /**
         * @brief pushes current terminal state to undostack
         * 
         * @param state a state struct containing essential terminal info
         */      
        void pushState(const State& state);

        /**
         * @brief pushes terminal state onto redo and gets last state from undo
         * 
         * @param state a state struct containing essential terminal info
         * 
         * @return true if operation was succesful or false if undostack was empty
         */
        bool undo(State& state);

        /**
         * @brief pushes terminal state onto undo and gets last state from redo
         * 
         * @param state a state struct containing essential terminal info
         * 
         * @return true if operation was succesful or false if redstack was empty
         */
        bool redo(State& state);

        /**
         * @brief clears redo and undo stacks
        */
        void clearRedo();
        

    private:
        std::vector<State> undoStack;
        std::vector<State> redoStack;
        const int MAX_HISTORY = 100;


};

/**
 * @class Clipboard
 * @brief Manages undo and redo stacks of editor states.
 */
class Clipboard{
    public:

        /**
         * @brief cuts line from terminal by making it empty and saves it to clipboard
         * 
         * @param state a state struct containing essential terminal info
         */
        void cut(State& state);

        /**
         * @brief pastes line saved in clipboard vector
         * 
         * @param state a state struct containing essential terminal info
         */
        void copy(const State& state);

        /**
         * @brief pastes line saved in clipboard vector
         * 
         * @param state a state struct containing essential terminal info
         */

        void paste(State& state);
      
        
    private:
        std::vector<std::string> clipboard; //member variable to keep track of copied or cut state
};
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
        
        void pushState(const State& state);
        bool undo(State& state);
        bool redo(State& state);
        void clearRedo();
        //bool UndoOk() const;
        //bool RedoOk() const;

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
        void cut(State& state);
        void copy(const State& state);
        void paste(State& state);
        //bool hasContent() const;
        
    private:
        std::vector<std::string> clipboard;
};
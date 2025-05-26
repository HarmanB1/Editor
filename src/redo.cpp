/**
 * @class Redo
 * @brief Core undo/redo implemenation
 *
 * Handles all editor functionality including:
 * - undo
 * - redo
 * - copying
 * - pasting
 * - cutting
 */


#include "redo.h"


/**
 * @brief 
 * 
 * @param filepath The destination file path (relative to current directory)
 * 
 * @return true if save succeeded, false otherwise
 * 
 */


 /**
 * @brief pushes current terminal state to undostack
 * 
 * @param State a state struct containing essential terminal info
 */

void History::pushState(const State& state){
    if(undoStack.size()>= MAX_HISTORY){
        undoStack.erase(undoStack.begin(),undoStack.begin()+undoStack.size()/2);
    }
    undoStack.push_back(state);
    redoStack.clear();
}


/**
 * @brief pushes terminal state onto redo and gets last state from undo
 * 
 * @param State a state struct containing essential terminal info
 * 
 * @return true if operation was succesful or false if undostack was empty
 */
bool History::undo(State& state){
    if(undoStack.empty()){
        return false;
    }
    redoStack.push_back(state);
    state = undoStack.back();
    undoStack.pop_back();
    return true;

}

/**
 * @brief pushes terminal state onto undo and gets last state from redo
 * 
 * @param State a state struct containing essential terminal info
 * 
 * @return true if operation was succesful or false if redstack was empty
 */

bool History::redo(State& state){
    if(redoStack.empty()) return false;
    undoStack.push_back(state);
    state = redoStack.back();
    redoStack.pop_back();
    return true;
}

/**
 * @brief clears redo and undo stacks
 */

void History::clearRedo(){
    undoStack.clear();
    redoStack.clear();

}



//Clipboard functions 

/**
 * @brief cuts line from terminal by making it empty and saves it to clipboard
 * 
 * @param State a state struct containing essential terminal info
 */


void Clipboard::cut(State& state){
   if(state.cursorY >= state.content.size()) return;
   
    clipboard.clear();
    clipboard.push_back(state.content[state.cursorY]);
    state.content[state.cursorY] = "";
    state.cursorX = 0;
}

/**
 * @brief copies line the user has cursor on and saves to clipboard
 * 
 * @param State a state struct containing essential terminal info
 */

void Clipboard::copy(const State& state){
    if(state.cursorY >= state.content.size()) return;
    clipboard.clear();
    clipboard.push_back(state.content[state.cursorY]);
}

/**
 * @brief pastes line saved in clipboard vector
 * 
 * @param State a state struct containing essential terminal info
 */

void Clipboard::paste(State& state){
    if(clipboard.empty()) return;

    state.content[state.cursorY].insert(state.cursorX, clipboard[0]);
    state.cursorX += clipboard[0].size();

}
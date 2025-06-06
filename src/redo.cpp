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



void History::pushState(const State& state){
    if(undoStack.size()>= MAX_HISTORY){
        undoStack.erase(undoStack.begin(),undoStack.begin()+undoStack.size()/2);
    }
    undoStack.push_back(state);
    redoStack.clear();
}



bool History::undo(State& state){
    if(undoStack.empty()){
        return false;
    }
    redoStack.push_back(state);
    state = undoStack.back();
    undoStack.pop_back();
    return true;

}



bool History::redo(State& state){
    if(redoStack.empty()) return false;
    undoStack.push_back(state);
    state = redoStack.back();
    redoStack.pop_back();
    return true;
}



void History::clearRedo(){
    undoStack.clear();
    redoStack.clear();

}



//Clipboard functions 



void Clipboard::cut(State& state){
   if(state.cursorY >= state.content.size()) return;
   
    clipboard.clear();
    clipboard.push_back(state.content[state.cursorY]);
    state.content[state.cursorY] = "";
    state.cursorX = 0;
}


void Clipboard::copy(const State& state){
    if(state.cursorY >= state.content.size()) return;
    clipboard.clear();
    clipboard.push_back(state.content[state.cursorY]);
}



void Clipboard::paste(State& state){
    if(clipboard.empty()) return;

    state.content[state.cursorY].insert(state.cursorX, clipboard[0]);
    state.cursorX += clipboard[0].size();

}
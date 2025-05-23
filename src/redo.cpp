#include "redo.h"

//history implmenetatonis

void History::pushState(const State& state){
    if(undoStack.size()>= MAX_HISTORY){
        undoStack.erase(undoStack.begin());
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
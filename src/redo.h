#pragma once
#include <vector>
#include <string>
struct State{
    std::vector<std::string> content;
    size_t cursorX;
    size_t cursorY;
    std::string filepath;
    std::string directory;

    //initializer
    State(
        std::vector<std::string> c,
        size_t x,
        size_t y,
        std::string f,
        std::string d)
    
        : content(c), cursorX(x), cursorY(y), filepath(f), directory(d){}

    
};

class History{
    public:
        
        void pushState(const State& state);
        bool undo(State& state);
        bool redo(State& state);

    private:
        std::vector<State> undoStack;
        std::vector<State> redoStack;
        const int MAX_HISTORY = 100;






};


class Clipboard{
    pubilc:
        void cut
        void copy
        void paste
        
    private:
        std::vector<std::string> clipboard;
};
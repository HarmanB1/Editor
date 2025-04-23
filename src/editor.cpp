#include "editor.h"

Editor::Editor(): cursorX(0), cursorY(0) {
    //use ncurses library
    initscr(); //starts cursues
    raw();
    echo();
    keypad(stdscr, TRUE);
    content.push_back(""); //starts empty line

}

Editor::~Editor(){
    endwin();
}

void Editor::run(){
    while(true){
        refreshScreen();
        userInput();
    }
}
void Editor::userInput(){
    int ch = getch();
    switch(ch){
        case KEY_UP: if(cursorY> 0){
            cursorY--;
            break;
        }
        case KEY_DOWN: if(cursorY<content.size()-1){
            cursorY++;
            break;
        }
        case KEY_LEFT: if(cursorX >0){
            cursorX--;
            break;
        }
        case KEY_RIGHT: if(cursorX < content[cursorY].size()){
            cursorX++;
            break;
        }
        case 27:
        //acsi for escape
            return;
    }
}
#include "editor.h"

Editor::Editor(): cursorX(0), cursorY(0) {
    //use ncurses library
    initscr(); //starts cursues
    raw();
    echo();
    keypad(stdscr, TRUE);
    curs_set(1);  
    content.push_back(""); //starts empty line

}

Editor::~Editor(){
    endwin();
}

void Editor::run(){
    while(true){
        refreshScreen();
        getInput();
    }
}

void Editor::getInput(){
    int ch = getch();
    moveInput(ch);
}


void Editor::moveInput(int ch){
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
        case 10:
        //asci for enter
        {
            //for new line

        }
        case KEY_BACKSPACE:
            if(cursorX>0){
                //backspace
            }

        
        case 27:
        //acsi for escape
            return;
        default:
        if(isprint(ch)){
            content[cursorY].insert(cursorX, 1, ch);
        }
    }
    //have bound checker
}

void Editor::refreshScreen(){
   move(cursorY, 0); //move to start of changed line
   clrtoeol();

    //print line with updated content
   mvprintw(cursorY, 0, content[cursorY].c_str());

    //move back to og
   move(cursorY, cursorX);
   refresh();
}
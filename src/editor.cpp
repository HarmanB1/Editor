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
        clear();
        for(size_t i=0; i<content.size(); i++){
            mvprintw(i, 0, content[i].c_str());
        }
        move(cursorY, cursorX);
        refresh();
        getInput();
    }
}

void Editor::getInput(){
    int ch = getch();
    doInput(ch);
}


void Editor::doInput(int ch){
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
            //take part of line at cursorx
            std::string newLine = content[cursorY].substr(cursorX);
            content[cursorY]= content[cursorY].substr(0, cursorX);//put new content at line back at same spot
            content.insert(content.begin()+cursorY+1, newLine);//insert newline at proper spot in vector
            cursorY++;
            cursorX=0;
            break;

        }
        case KEY_BACKSPACE:
            if(cursorX>0){
                if(cursorX>0){
                    //normal deletion
                    content[cursorY].erase(cursorX-1,1);
                    cursorX--;
                }else if(cursorY>0){
                    //deletion if cursor is on start of line
                    cursorX = content[cursorY-1].size();
                    content[cursorY-1]+= content[cursorY];//concatenate line below to line above
                    content.erase(content.begin()+cursorY);
                    cursorY--;

                }
                break;

                //backspace
            }

        case ' ':
        //adding space
            content[cursorY].insert(cursorX, 1, ' ');
            cursorX++;
            break;
        case 27:
        //acsi for escape
            return;
        default:
            if(isprint(ch)){
                content[cursorY].insert(cursorX, 1, ch);
                cursorX++;
            }
            break;
    }
    //have bound checker
}




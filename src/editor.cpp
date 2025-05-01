#include "editor.h"
#include <iostream>

Editor::Editor(): cursorX(0), cursorY(0) {
   
    //use ncurses library
    initscr(); //starts cursues
    raw();
    echo();
    keypad(stdscr, TRUE);
    curs_set(1);  
    mousemask(ALL_MOUSE_EVENTS, NULL);
   printf("\033[?1003h\n"); // Enable mouse movement events
fflush(stdout);
    mouseinterval(0);

    content.push_back(""); //starts empty line
    running = true;

}

Editor::~Editor(){
    printf("\033[?1003l\n"); // Disable mouse events
fflush(stdout);
   
    endwin();
   
}

void Editor::run(){
    
    while(running){
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
    if (ch == KEY_MOUSE) {
        doMouse();
    } else {
        doInput(ch);
    }
    
    
    
}

void Editor::doMouse(){
    MEVENT event;
    if(getmouse(&event)== OK){
        if(event.bstate& BUTTON1_CLICKED){
             
            cursorY = event.y;
            cursorX = event.x;

            //bound checking
            if(cursorY >= content.size()){
                cursorY = content.size()-1;
            }
            if(cursorX > content[cursorY].size()){
                cursorX = content[cursorY].size();
            }
        }

    }
   


}


void Editor::doInput(int ch){
    switch(ch){
        case KEY_UP: if(cursorY> 0){
            
            cursorY--;
            cursorX=content[cursorY].size()-1;
            if(content[cursorY].size()==0){
                cursorX=0;
            }
            break;
        }
        else{
                break;
            }

        case KEY_DOWN: if(cursorY<content.size()-1){
            cursorY++;
            cursorX=content[cursorY].size()-1;
            if(content[cursorY].size()==0){
                cursorX=0;
            }
            break;
        }else{
            content.push_back("");
            cursorY++;
            cursorX=0;
            break;
        }

        case KEY_LEFT: if(cursorX >0){
            cursorX--;
            break;
        }else{
                break;
            }
            
        case KEY_RIGHT: if(cursorX < content[cursorY].size()){
            cursorX++;
            break;
            
        }else{
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
        case 127:
            
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
            

        case ' ':
        //adding space
            content[cursorY].insert(cursorX, 1, ' ');
            cursorX++;
            break;
        case 27:
        //acsi for escape
            clear();
            mvprintw(0, 0, "ENDING PROGRAM");
            refresh();
            napms(1000);
            running = false;
            break;
        default:
            if(isprint(ch)){
                content[cursorY].insert(cursorX, 1, ch);
             
                cursorX++;
            }

            break;
    }
    //have bound checker
}




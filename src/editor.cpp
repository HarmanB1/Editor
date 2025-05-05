///to do make sure mouse clicks are less percise
//have mouse clikc all aras
//have to do scrolling
//to do make sure to save on quit
//add settings colours
//fix bug where after no curosr is, 
//fix double repetition of letters


#include "editor.h"
#include <iostream>
#include <ncurses.h>


Editor::Editor(): cursorX(0), cursorY(0) {
   
    //use ncurses library
    initscr(); //starts cursues

    start_color();
    use_default_colors();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_GREEN);
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
        
        int row, col;
        getmaxyx(stdscr, row, col);
        attron(COLOR_PAIR(2));
        mvhline(row - 1, 0, ' ', col); // Draw empty line with color
        mvprintw(row - 1, 0, " ESC: Quit | Ctrl+S: Save | Ctrl+L: Load| Ctrl+F: Find | Ctrl+U: Settings Menu ");
        attroff(COLOR_PAIR(2));

        for(int i=0; i<content.size()&& i < row - 1; i++){
            mvprintw(i, 0, content[i].c_str());
        }
        move(cursorY, cursorX);
        refresh();
        getInput();
    }
}

void Editor::save(std::vector<std::string>& content, std::vector<std::string>& content_backup) {
    clear();
    attron(COLOR_PAIR(2));
    int row, col;
    getmaxyx(stdscr, row, col);
    mvhline(0, 0, ' ', col);
    mvprintw(0, 0, "ESC: Cancel | Type in path to save file:");
    mvprintw(1, 0, "filename: ");
    echo();
    curs_set(1);
    char pathInput[256];
    memset(pathInput, 0, sizeof(pathInput));
    move(1, 10);

    int ch;
    int i = 0;
    while (i < 255 && (ch = getch()) != '\n') {
        if (ch == 27) { // ESC
            noecho();
            curs_set(0);
            content = content_backup; // Restore content
            mvprintw(3, 0, "Save cancelled. Press any key to return.");
            getch();
            return;
        }
        if (isprint(ch)) {
            pathInput[i++] = ch;
            addch(ch);
        }
    }
    noecho();
    curs_set(0);
    std::string userPath(pathInput);

    if (userPath.empty()) {
        mvprintw(3, 0, "No file name entered. Press any key to return.");
        getch();
        return;
    }

    if (fileIO::save(userPath, content)) {
        mvprintw(3, 0, "File saved successfully.");
    } else {
        mvprintw(3, 0, "Failed to save file.");
    }
    mvprintw(4, 0, "Press any key to return.");
    getch();
}

        
void Editor::load(std::vector<std::string>& content, std::vector<std::string>& content_backup) {
    clear();
    attron(COLOR_PAIR(2));
    int row, col;
    getmaxyx(stdscr, row, col);
    mvhline(0, 0, ' ', col);
    mvprintw(0, 0, "ESC: Cancel | Type in path to load file:");
    mvprintw(1, 0, "filename: ");
    echo();
    curs_set(1);
    char pathInput[256];
    memset(pathInput, 0, sizeof(pathInput));
    move(1, 10);

    int ch;
    int i = 0;
    while (i < 255 && (ch = getch()) != '\n') {
        if (ch == 27) { // ESC
            noecho();
            curs_set(0);
            content = content_backup; // Restore
            mvprintw(3, 0, "Load cancelled. Press any key to return.");
            getch();
            return;
        }
        if (isprint(ch)) {
            pathInput[i++] = ch;
            addch(ch);
        }
    }
    noecho();
    curs_set(0);
    std::string userPath(pathInput);

    if (userPath.empty()) {
        mvprintw(3, 0, "No file name entered. Press any key to return.");
        getch();
        return;
    }

    content_backup = content;  // backup before loading

    if (fileIO::load(userPath, content)) {
        mvprintw(3, 0, "File loaded successfully.");
    } else {
        content = content_backup; // restore on failure
        mvprintw(3, 0, "Failed to load file.");
    }
    mvprintw(4, 0, "Press any key to return.");
    getch();
}

        
void Editor::find(){

}
        
void Editor::settings(){

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
        case 12: //for ctrl l
            backup_content= content;
            load(content, backup_content);
            break;

        case 19: //for ctrl s
            backup_content=content;
            save(content, backup_content);
            break;


        case 6: //for ctrl f

        case 21: //for ctrl u


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




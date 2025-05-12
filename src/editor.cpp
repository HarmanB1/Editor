//add settings
//settings txt
//add opton to clear
//add settings colours
//undo redo
//save status
//file diffing
//copy pasting cliboard
//bookmarks
//cursor pos
//cut text k
//refresh text from pull og 

//must fix saving of thing and of(test with files before )
//settings
//themes
#include "fileIO.h"
#include "editor.h"
#include <iostream>








Editor::Editor(): cursorX(0), cursorY(0), scrollY(0) {
   
    //use ncurses library
    initscr(); //starts cursues
    scrollok(stdscr, TRUE); 

    //colours
    start_color();
    use_default_colors();
    applyCol();


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

std::string Editor::getConfigPath(){
    namespace fs = std::filesystem;
    fs::path cwd = fs::current_path();
    fs::path config = cwd/ "../config/settings.cfg";
    return config.lexically_normal().string();
}


void Editor::applyCol(){
    init_pair(1, setting.textCol, setting.backgroundCol);
    init_pair(2, setting.textCol, setting.statusBarCol);


}


void Editor::run(){
    
    while(running){
        erase();
        
        int row, col;
        getmaxyx(stdscr, row, col);
        int visRows = row-1;

        if (cursorY < scrollY) {
            scrollY = cursorY;
        } else if (cursorY >= scrollY + visRows) {
            scrollY = cursorY - visRows + 1;
        }

        //status bar
        updateStatus();

        //printing
        for (int i = 0; i < visRows; i++) {
            int lineIdx = scrollY + i;
            if (lineIdx >= content.size()) break;
            mvprintw(i, 0, "%s", content[lineIdx].c_str());
        }
       
        

        int screenCursorY = cursorY - scrollY;
        move(screenCursorY, cursorX);
        refresh();
        getInput();
    }
}

void Editor::updateStatus(){
    int row, col;
    getmaxyx(stdscr, row, col);

    attron(COLOR_PAIR(2));
    mvhline(row-1, 0, ' ', col); // Draw empty line with color

    std::string nameDisplay = filepath.empty() ? "No file entered" : filepath;
    std::string posInfo = "Ln " + std::to_string(cursorY+1)+ ", Col" + std::to_string(cursorX +1);
    std::string bar = " ESC: Quit | ^S: Save | ^L: Load| ^F: New | ^U: Settings Menu | " + nameDisplay + " | "+posInfo;

    if(bar.length()>static_cast<size_t>(col)){
        bar = bar.substr(0, col-3)+ "...";
    }
    mvprintw(row-1, 0, bar.c_str());
    attroff(COLOR_PAIR(2));

}


void Editor::save(std::string filepath,std::vector<std::string>& content, std::vector<std::string>& content_backup) {
    
    std::string userPath(filepath);

    if (userPath.empty()) {
        clear();
        mvprintw(0, 0, "No file name entered or file. Press any key to return.");
        refresh();
        getch();
        return;
    }
   
    
    
    clear();
    if (fileIO::save(userPath, content)) {
        mvprintw(0, 0, "File saved successfully.");
    } else {
        mvprintw(0, 0, "Failed to save file.");
    }
    mvprintw(1, 0, "Press any key to return.");
    getch();
}

        
void Editor::load(std::string& filepath,std::vector<std::string>& content, std::vector<std::string>& content_backup) {
    clear();
    attron(COLOR_PAIR(2));
    int row, col;
    getmaxyx(stdscr, row, col);
    mvhline(0, 0, ' ', col);
    mvprintw(0, 0, "ESC: Cancel | Type in path to load file:");
    mvprintw(1, 0, "filename: ");
    
    curs_set(1);
    char pathInput[256];
    memset(pathInput, 0, sizeof(pathInput));
    move(1, 10);

    int ch;
    int i = 0; //x position
    noecho();
    while (i < 255 && (ch = getch()) != '\n') {
        
        if (ch == 27) { // ESC
            noecho();
            curs_set(1);
            content = content_backup; // Restore
            mvprintw(0, 0, "Load cancelled. Press any key to return.");
            getch();
            return;
        }
        if(ch == 127){
            
            if(i>0){
                    //deletion
                    i--;
                    pathInput[i] ='\0';
                    move(1,10);
                    clrtoeol();
                    printw("%s", pathInput);
                    move(1, 10+i);
                    
                }
                

        }    
        //noecho
        else if (isprint(ch)) {
            pathInput[i++] = ch;
            addch(ch);
            
        }
    }
    noecho();
    curs_set(1);
    filepath = pathInput;
    std::string userPath(pathInput);

    if (userPath.empty()) {
        mvprintw(0, 0, "No file name entered. Press any key to return.");
        filepath = "";
        getch();
        return;
    }

      // backup before loading

    if (fileIO::load(userPath, content)) {
        if(content.empty()){
            content.push_back("");
        }
        mvprintw(0, 0, "File loaded successfully.");
        cursorX=0;
        cursorY=0;
    } else {
        content = content_backup; // restore on failure
        
        mvprintw(0, 0, "Failed to load file.");
        filepath = "";
        
    }
    mvprintw(1, 0, "Press any key to return.");
    move(0,0);
    refresh();
    getch();
}

        

        
void Editor::settings(){
    int row, col;
    getmaxyx(stdscr, row, col);


    //creates backup of working window
    WINDOW* backup = newwin(row, col, 0, 0);
    copywin(stdscr, backup, 0, 0, 0, 0, row-1, col-1, 0);

    WINDOW* settingWIN = newwin(row, col, 0,0);
    box(settingWIN, 0, 0);
    keypad(settingWIN, TRUE);

    int selected = 0;
    bool inSetting = true;

    while (inSetting){
        werase(settingWIN);
        box(settingWIN, 0, 0);

        mvprintw(1, 2, "EDITOR SETTING");
        

        //settings display
        mvwprintw(settingWIN, 1, 2, "Editor Settings");
        mvwprintw(settingWIN, 3, 4, "%s Autosave: %s", selected == 0 ? ">" : " ", setting.autosave ? "ON" : "OFF");
        mvwprintw(settingWIN, 4, 4, "%s Display Line Number: %s", selected == 1 ? ">" : " ", setting.lineNumb ? "ON" : "OFF");
        mvwprintw(settingWIN, 5, 4, "%s Word Wrap: %d", selected == 2 ? ">" : " ", setting.wordWrap ? "ON" : "OFF");
        mvwprintw(settingWIN, 6, 4, "%s Ask for Save on close %s", selected == 4 ? ">" : " ", setting.saveOnClose? "ON" : "OFF");
        mvwprintw(settingWIN, 7, 4, "%s TextColour %s", selected == 5 ? ">" : " ", setting.textCol);
        mvwprintw(settingWIN, 8, 4, "%s Statusbar colour %s", selected == 6 ? ">" : " ", setting.statusBarCol);
        mvwprintw(settingWIN, 9, 4, "%s background colour %s", selected == 7 ? ">" : " ", setting.backgroundCol);
        mvwprintw(settingWIN, 10, 4, "%s Save Settings", selected == 8 ? ">" : " ");
        mvwprintw(settingWIN, 11, 4, "%s Exit Settings", selected == 9 ? ">" : " ");

        wrefresh(settingWIN);

        int ch = wgetch(settingWIN);
        switch(ch){
            case KEY_UP:
                selected = selected > 0 ? selected -1:9;
                break;
            case KEY_DOWN:
                selected = selected < 5 ? selected+1: 0;
                break;

            case KEY_LEFT:
            case KEY_RIGHT:
            case ' ':
                //acts as a toggle
                switch(selected){
                    case 0: setting.autosave = !setting.autosave; break;
                    case 1: setting.lineNumb = !setting.lineNumb; break;
                    case 2: setting.wordWrap = !setting.wordWrap; break;
                    case 3: setting.saveOnClose = !setting.saveOnClose; break;
                    case 
                    
                }


            case 10: //ascii for enter
                switch(selected){
                    case 4: saveSetting(); break;
                    case 5: inSetting = false; break;

                }
                   



        }


    }

  



    //restore window and delte
    copywin(backup, stdscr, 0, 0, 0, 0, row-1, col-1, 0);
    delwin(backup);
    delwin(settingWIN);
    refresh();


}

void Editor::saveSetting(){
    std::ofstream config_file(getConfigPath());
    if(config_file){
        config_file << setting.autosave << '\n'
        << setting.lineNumb << '\n'
        << setting.wordWrap << '\n'
        << setting.saveOnClose << '\n'
        << setting.textCol << '\n'
        << setting.statusBarCol << '\n'
        << setting.backgroundCol << '\n';
    }

}

void Editor::loadSetting(){
    std::ifstream config_file(getConfigPath());
    if(config_file){
        config_file >> setting.autosave
        >> setting.lineNumb
        >> setting.wordWrap 
        >> setting.saveOnClose
        >> setting.textCol
        >> setting.statusBarCol
        >> setting.backgroundCol;
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
        case 18: //for ctrl r
            refresh();
        case 12: //for ctrl l
            backup_content= content;
            content.clear();
            load(filepath, content, backup_content);
            content.push_back("");

            break;

        case 19: //for ctrl s
            //backup_content=content;
            save(filepath, content, backup_content);
            break;


        case 6: //for ctrl f
            filepath = "";
            
            clear();
            cursorX = cursorY = 0;
            break;
            

        case 21: //for ctrl u
            settings();




        case KEY_UP: if(cursorY> 0){
            
            cursorY--;
            cursorX=content[cursorY].size();
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
            cursorX=content[cursorY].size();
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
            
            if(filepath != ""){
                clear();
                mvprintw(0,0, "Save Y/N : ");
                refresh();
                bool done = true;
                while(done){
                    int ch = getch();
                    if(ch == 89 || ch == 121){
                        save(filepath, content, backup_content);
                        break;

                    }else if(ch == 78 || ch == 110 ){
                        break;

                    }else{
                        clear();
                        mvprintw(0,0, "error user did not answer y or n");
                        mvprintw(1,0, "Save Y/N : ");
                        refresh();
                    }

                }

            }
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




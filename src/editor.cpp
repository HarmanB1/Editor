/**
 * @file dditor.cpp
 * @brief Core text editor implementation
 *
 * Handles all editor functionality including:
 * - Keyboard/mouse input
 * - Screen rendering
 * - File I/O operations
 * - Undo/redo functionality
 * - Configuration management
 */

#include "fileIO.h"
#include "editor.h"
#include <iostream>

constexpr int COLSIZE = 8; //used for settings menu to indicate how many colour options



Editor::Editor(): cursorX(0), cursorY(0), scrollY(0) {
   
    //use ncurses library
    initscr(); //starts cursues
    scrollok(stdscr, TRUE); 

    //loadsettings 
    loadSetting(); 

    //checks for colours
    if(!has_colors()){
        endwin();
        printf("Terminal does not support color\n");
        exit(1);
    }

    //colours
    applyCol();
   
    //uses last saved time
    lastSaveTime = std::chrono::system_clock::now();
    
    // Default settings
    autoSaveInterval = 30;

    //sets terminal options
    raw();
    echo();
    keypad(stdscr, TRUE);
    curs_set(1);  
    mousemask(ALL_MOUSE_EVENTS, NULL);

    //escape code to terminal ||MAY BE DIFFERENT ON OTHER TERMINALS||
    printf("\033[?1003h\n"); 
    fflush(stdout);
    mouseinterval(0);


    content.push_back(""); //starts empty line

    backup_content = content;

    //this var determines running loop of program
    running = true;

}


Editor::~Editor(){
    
    //disables mouse controls and flushes out terminal
    printf("\033[?1003l\n"); 
    fflush(stdout);

    //take care of Ncurses sequences
    curs_set(1);
    echo();
    nocbreak();
    keypad(stdscr, FALSE);
    mouseinterval(0);
    
   //endwin and resets shell 
    endwin();
    reset_shell_mode(); 
    system("tput reset 2>/dev/null");
    system("stty sane 2>/dev/null");
   
}

void Editor::run(){
    //stores current time
    auto lastCheck = std::chrono::system_clock::now();

    //main loop
    while(running){
        auto now = std::chrono::system_clock::now();

        //checks difference between now and lastcheck and autosaves
        if (std::chrono::duration_cast<std::chrono::seconds>(now - lastCheck).count() >= 1) {
            autoSave();
            lastCheck = now;
        }

        //erases screen
        erase();
        
        //gets row col and sets visible rows, meaning rows interactable by user
        int row, col;
        getmaxyx(stdscr, row, col);
        int visRows = row-2;

        //handles scrolling logic regarding cursor
        if (static_cast<int>(cursorY) < scrollY) {
            scrollY = cursorY;
        } else if (static_cast<int>(cursorY) >= scrollY + visRows) {
            scrollY = cursorY - visRows + 1;
        }

        //status bar
        applyCol();
        
  
        //checks if setting linenumb is true
        //if true, then prints the linenumber accordingly on each line
        //otherwise prints normally 
        if(setting.lineNumb){
            lineNumb();
        }else{
            for (int i = 0; i < visRows; i++) {
                int lineIdx = scrollY + i;
                if (lineIdx >= static_cast<int>(content.size())) break;
            
                mvhline(i, 0, ' ', col);
                mvprintw(i, 0, "%s", content[lineIdx].c_str());
                
                
            }

        }
            
        //handles status bar logic
        updateStatus();
       
        //scrolling logic
        int screenCursorY = cursorY - scrollY;

        int screenCursorX = cursorX;
        if(setting.lineNumb){
            screenCursorX +=5; //5 for the linenumber characters
        }

        
        //moves cursors and refreshes then gets input
        move(screenCursorY, screenCursorX);
        refresh();
        getInput();

        wordWrap();
    }
}

void Editor::doInput(int ch){
    switch(ch){
        case 26: //for ctrl z for going undo
        {
            State currentState = getCurrentState();
            if(editHistory.undo(currentState)){
                applyState(currentState);
            }
                
            break;

        }
            
        case 25: //ctrl Y for redo
        {
            State currentState = getCurrentState();
            if(editHistory.redo(currentState)){
                applyState(currentState);
            }
                
            break;

        }

        case 24: //ctrl x for cut
        {
            editHistory.pushState(getCurrentState());
            State currentState = getCurrentState();
            editClipboard.cut(currentState);
            applyState(currentState);
            break;

        }
        
        case 3:{ //  ctrl c for copy
            editClipboard.copy(getCurrentState());
            break;
        }

        case 22://ctrl v
        {
            editHistory.pushState(getCurrentState());
            State currentState = getCurrentState();
            editClipboard.paste(currentState);
            applyState(currentState);
            break;
        }
            

        case 4: //for ctrl d direcotry
            direct(directory);
            break;
        
            
        case 12: //for ctrl l loading
            backup_content= content;
            content.clear();
            load(filepath, content, backup_content);
            content.push_back("");

            break;

        case 19: //for ctrl s saving
            save(filepath, content);
            break;


        case 6: //for ctrl f filepath
            filepath = "";
            
            clear();
            cursorX = cursorY = 0;
            break;
            

        case 21: //for ctrl u settings
            settings();
            refresh();


        /*keyboard functions*/
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
            editHistory.pushState(getCurrentState());
            //take part of line at cursorx
            std::string newLine = content[cursorY].substr(cursorX);
            content[cursorY]= content[cursorY].substr(0, cursorX);//put new content at line back at same spot
            content.insert(content.begin()+cursorY+1, newLine);//insert newline at proper spot in vector
            cursorY++;
            cursorX=0;
            break;

        }
        case 127: //backspace
            editHistory.pushState(getCurrentState());
            
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

           
            

        case ' ': //case space
        editHistory.pushState(getCurrentState());
        //adding space
            content[cursorY].insert(cursorX, 1, ' ');
            cursorX++;
            break;
        case 27: //acsi for escape
            
            saveOnClose();
            clear();
            
            mvprintw(0, 0, "ENDING PROGRAM");
            refresh();

            //timer to let user see message
            napms(1000);
            running = false;
            break;
        default: //regular character
            if(isprint(ch)){
                editHistory.pushState(getCurrentState());
                content[cursorY].insert(cursorX, 1, ch);
             
                cursorX++;

                wordWrap();
            }

            break;
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
             
            //gets mouse cursor info
            cursorY = event.y;
            cursorX = event.x;

            //checking for linenumb in order to shift cursor locations
            if(setting.lineNumb && cursorX >= 5){
                cursorX -=5;
            }else if(setting.lineNumb){
                cursorX =0;
            }

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

void Editor::save(std::string filepath,std::vector<std::string>& content) {

    editHistory.pushState(getCurrentState()); //savestate
    
    //format filepath to include specified directory
    std::filesystem::path directory_path = directory;
    std::filesystem::path userPath = directory_path / filepath;
    userPath = userPath.lexically_normal().string();

    //checks file path emptiness
    if (userPath.empty()) {
        clear();
        mvprintw(0, 0, "No file name entered or file. Press any key to return.");
        refresh();
        getch();
        return;
    }
   
    //clear area and print message based on action, then return 
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
    
    if (directory.empty()) {
        refresh();
        content = backup_content;
        
        mvprintw(0, 0, "Error: No directory set. Press Ctrl+D from main menu to set it.");
        getch();
        return;
    }
    editHistory.pushState(getCurrentState());
        
    //pops open new window asking user for file name
    clear();
    attron(COLOR_PAIR(2));
    int row, col;
    row = 0;
    getmaxyx(stdscr, row, col);
    mvhline(0, 0, ' ', col);
    mvprintw(0, 0, "ESC: Cancel | Type in file name such as: t1.txt (set directory beforehand from main menu using ^d, )");
    mvprintw(1, 0, "filename: ");
    
    curs_set(1);
    char pathInput[256];
    memset(pathInput, 0, sizeof(pathInput));
    move(1, 10);

    int ch;
    int i = 0; //x position
    noecho();

    //main while loop asking user for filepath
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

    //sets directory + userInput as as path to load from
    filepath = pathInput;

    std::filesystem::path directory_path = directory;
    std::filesystem::path userPath = directory_path / pathInput;
    userPath = userPath.lexically_normal().string();

    
    //checks for empty or non existing files

    if (userPath.empty()) {
        mvprintw(0, 0, "No file name entered. Press any key to return.");
        filepath = "";
        getch();
        return;
    }

    if(!fileIO::file_exists(userPath)){
        mvprintw(3, 0, "no file of that name exists, creating file");
        refresh();
        fileIO::file_create(filepath);
    }

    

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

void Editor::direct(std::string& directory){
    //saves state
    editHistory.pushState(getCurrentState());
    clear();
    attron(COLOR_PAIR(2));
    int row, col;
    row = 0;
    getmaxyx(stdscr, row, col);
    mvhline(0, 0, ' ', col);
    mvprintw(0, 0, "ESC: Cancel | Type in directory ex: /Users/JamesSmith/downloads/ , to save, use save settings in settings");
    mvprintw(1, 0, "Directory: ");
    
    curs_set(1);

    //initialiaze with previous directory saved
    char directoryUser[256];
    memset(directoryUser, 0, sizeof(directoryUser));
    strncpy(directoryUser, setting.directory.c_str(), sizeof(directoryUser)-1);//throw current directory into char string

    move(1, 10);
    printw("%s", setting.directory.c_str());

    int ch;
    int i = strlen(directoryUser);//x position
    noecho();

    //while loop to get directory from user
    while (i < 255 && (ch = getch()) != '\n') {
        
        if (ch == 27) { // ESC
            noecho();
            curs_set(1);
            
            mvprintw(0, 0, "Directory cancelled. Press any key to return.");
            getch();
            return;
        }
        if(ch == 127){
            
            if(i>0){//deletion
                    i--;
                    directoryUser[i] ='\0';
                    move(1,10);
                    clrtoeol();
                    printw("%s", directoryUser);
                    move(1, 10+i);
                    
                }
                

        }    
        //noecho
        else if (isprint(ch)) {
            directoryUser[i++] = ch;
            addch(ch);
            
        }
    }

    if(directory.empty()){
        directory = "./";
    }else{
        directory = directoryUser;
    }
    //ensure it ends with /
    size_t len = strlen(directoryUser);
    if(len>0 && directoryUser[len-1]!= '/'){
        if(len<sizeof(directoryUser)-1){
            directoryUser[len] = '/';
            directoryUser[len+1] = '\0';
        }
    }



    noecho();
    curs_set(1);
    directory = directoryUser;
    setting.directory = directory;
    
    mvprintw(1, 0, "Directory saved %s", directory.c_str());
    mvprintw(3, 0, "to save directory for next time, please save in settings, to return press any key");
    move(0,0);
    refresh();
    getch();
}

void Editor::autoSave(){
    //gets difference of now vs last and sees if it matches autosave interval
    if(!setting.autosave) return;

    auto now = std::chrono::system_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::seconds>(now-lastSaveTime).count();

    if(diff >= autoSaveInterval){
        if(!filepath.empty()){
            save(filepath, content);
            lastSaveTime = now;
        }
    }

}


void Editor::lineNumb(){
    if(!setting.lineNumb) return;

    int row, col;
    getmaxyx(stdscr, row, col);
    int visRows = row-2;

    //save cursor pos
    int saveY, saveX;
    getyx(stdscr, saveY, saveX);

    //prints linenumb

    for(int i=0; i < visRows; i++){
        int lineIdx = scrollY +i;
        if(lineIdx < static_cast<int>(content.size())){
            move(i, 0);
            attron(A_REVERSE);
            printw("%4d ", lineIdx +1);
            attroff(A_REVERSE);
            move(i, 5);
            printw("%s", content[lineIdx].c_str());

        }
    }



    move(saveY, saveX+5);
}

void Editor::wordWrap(){
    if (!setting.wordWrap) return;

    int row, col;
    getmaxyx(stdscr, row, col);
    const int maxLineWidth = col - (setting.lineNumb ? 5 : 0); // Account for line numbers

    std::vector<std::string> newContent;
    
    for (const auto& line : content) {
        if (line.length() <= static_cast<size_t>(maxLineWidth)) {
            newContent.push_back(line); 
            continue;
        }

        // Wrap the line
        size_t pos = 0;
        while (pos < line.length()) {
            
            size_t end = pos + maxLineWidth;
            if (end > line.length()) end = line.length();
            
            size_t spacePos = line.rfind(' ', end);
            if (spacePos != std::string::npos && spacePos > pos) {
                end = spacePos + 1; // Include the space
            }

            newContent.push_back(line.substr(pos, end - pos));
            pos = end;

            // Trim leading whitespace on next line (except first split)
            while (pos < line.length() && line[pos] == ' ') {
                pos++;
            }
        }
    }

    // Update content with wrapped lines
    if (!newContent.empty()) {
        editHistory.pushState(getCurrentState()); 
        content = newContent;
        
        // Adjust cursor position
        if (cursorY >= content.size()) {
            cursorY = content.size() - 1;
        }
        if (cursorX >= content[cursorY].size()) {
            cursorX = content[cursorY].size();
        }
    }

}


void Editor::saveOnClose(){
    if(!setting.saveOnClose) return;

    if(filepath != ""){
        clear();
        
        mvprintw(0,0, "Save Y/N : ");
        refresh();
        bool done = true;
        while(done){
            int ch = getch();
            if(ch == 89 || ch == 121){
                save(filepath, content);
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
}

void Editor::settings(){
    curs_set(0);  
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
        mvwprintw(settingWIN, 1, 2, "Editor Settings  use arrow key up and down to navigate, and use arrow keys, left, right or space bar to");
        mvwprintw(settingWIN, 2, 2, " change setting, press enter on save settings and exit settings");
        
        mvwprintw(settingWIN, 3, 4, "%s Autosave: %s", selected == 0 ? ">" : " ", setting.autosave ? "ON" : "OFF");
        mvwprintw(settingWIN, 4, 4, "%s Display Line Number: %s", selected == 1 ? ">" : " ", setting.lineNumb ? "ON" : "OFF");
        mvwprintw(settingWIN, 5, 4, "%s Word Wrap: %s", selected == 2 ? ">" : " ", setting.wordWrap ? "ON" : "OFF");
        mvwprintw(settingWIN, 6, 4, "%s Ask for Save on close %s", selected == 3 ? ">" : " ", setting.saveOnClose? "ON" : "OFF");

        //sorts through colour and saves index of colour related to actual colour strong corellated to index
        for(int i =0;i< colArr.size(); i++){
            if(colArr.at(i)== setting.textCol){
                textcol_index = i;
            }

        }
        for(int i =0;i< colArr.size(); i++){
            if(colArr.at(i)== setting.statusBarCol){
                statusBarCol_index = i;
            }

        }


        mvwprintw(settingWIN, 7, 4, "%s TextColour %s", selected == 4 ? ">" : " ", colArrString.at(textcol_index).c_str());
        mvwprintw(settingWIN, 8, 4, "%s Statusbar colour %s", selected == 5 ? ">" : " ", colArrString.at(statusBarCol_index).c_str());
       
        mvwprintw(settingWIN, 10, 4, "%s Save Settings", selected == 6 ? ">" : " ");
        mvwprintw(settingWIN, 11, 4, "%s Exit Settings", selected == 7 ? ">" : " ");

        wrefresh(settingWIN);

        //navigation logic
        int ch = wgetch(settingWIN);
        switch(ch){
            case KEY_UP:
                selected = selected > 0 ? selected -1:7;
                break;
            case KEY_DOWN:
                selected = selected < 7 ? selected+1: 0;
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
                    case 4:
                        textcol_index= (textcol_index+1)%COLSIZE;
                        setting.textCol= colArr.at(textcol_index);
                        applyCol();
                        break;
                    case 5:
                        statusBarCol_index= (statusBarCol_index+1)%COLSIZE;
                        setting.statusBarCol= colArr.at(statusBarCol_index);
                        applyCol();
                        break;
                    case 6:
                        backgroundCol_index= (backgroundCol_index+1)%COLSIZE;
                        setting.backgroundCol= colArr.at(backgroundCol_index);
                        applyCol();
                        break;    
                }
                break;


            case 10: //ascii for enter
                switch(selected){
                    case 6: saveSetting(); break;
                    case 7: inSetting = false; curs_set(1);   break;

                }
                break;
                   

        }


    }

  
    touchwin(stdscr);

    //restore window and delte
    copywin(backup, stdscr, 0, 0, 0, 0, row-1, col-1, 0);
    delwin(backup);
    delwin(settingWIN);
    refresh();


}

void Editor::applyCol(){
    //applies col may change depending on settings
    start_color();
    use_default_colors();
    //init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, setting.textCol, setting.statusBarCol);


}

void Editor::updateStatus(){
    int row, col;
    getmaxyx(stdscr, row, col);

    int visRows = row-2;

    attron(COLOR_PAIR(2));
    
    mvhline(visRows, 0, ' ', col); // Draw empty line with color
    mvhline(visRows+1, 0, ' ', col); // Draw empty line with color

    //prints strings
    std::string nameDisplay = filepath.empty() ? "No file entered" : filepath;
    std::string posInfo = "Ln " + std::to_string(cursorY+1)+ ", Col" + std::to_string(cursorX +1);
    std::string topLine = " ESC: Quit | ^S: Save | ^L: Load| ^F: New | ^U: Settings Menu | " + nameDisplay + " | " + setting.directory + " | " + posInfo;

    //formating for time
    auto now = std::chrono::system_clock::now();
    std::time_t saveTime = std::chrono::system_clock::to_time_t(lastSaveTime);
    std::tm tm = *std::localtime(&saveTime);
    char timeStr[20];
    std::strftime(timeStr, sizeof(timeStr), "%H: %M :%S", &tm);
    std::string saveTiming = "Last Saved: " + std::string(timeStr);

    std::string bottomLine = " ^Z: Undo | ^Y: Redo | ^X: cutline | ^C: copyline | ^V: pastline | ^D: setDirectory | "+ saveTiming+" save directory name for next session please save in settings";

    //format if window cuts off
    if(topLine.length()>static_cast<size_t>(col)){
        topLine = topLine.substr(0, col-3)+ "...";
        bottomLine = bottomLine.substr(0, col-3)+ "...";
    }
   
    

    mvprintw(visRows, 0, topLine.c_str());
    mvprintw(visRows+1, 0, bottomLine.c_str());
    attroff(COLOR_PAIR(2));

}

void Editor::saveSetting(){
    //opens config file, then writes settings
    std::ofstream config_file(getConfigPath());
    if(config_file){
        config_file << setting.autosave << '\n'
        << setting.lineNumb << '\n'
        << setting.wordWrap << '\n'
        << setting.saveOnClose << '\n'
        << setting.textCol << '\n'
        << setting.statusBarCol << '\n'
        << setting.backgroundCol << '\n'
        << setting.directory << '\n';
    }else{
        printf("erorr erorr");
    }

}

void Editor::loadSetting(){
    //loards settings from config file
    std::ifstream config_file(getConfigPath());
    if(config_file){
        config_file >> setting.autosave
        >> setting.lineNumb
        >> setting.wordWrap 
        >> setting.saveOnClose
        >> setting.textCol
        >> setting.statusBarCol
        >> setting.backgroundCol
        >> setting.directory;

        directory = setting.directory;
    }

    

}

std::string Editor::getConfigPath(){
    namespace fs = std::filesystem; //sets namespace just for this function
    fs::path cwd = fs::current_path();
    fs::path config = cwd/ "../cfg/settings.cfg";
    return config.lexically_normal().string();
}

State Editor::getCurrentState() const{
    return State(content, cursorX, cursorY, filepath, setting.directory);
}

void Editor::applyState(const State& state){
    //takes in state obj then sets state settings to current variables
    content = state.content;
    cursorX = state.cursorX;
    cursorY = state.cursorY;
    filepath = state.filepath;
    setting.directory = state.directory;

  
}




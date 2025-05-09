#include "fileIO.h"
#include <fstream>
#include <sys/stat.h>

bool fileIO::load(const std::string& filepath, std::vector<std::string>& content){
    if(!file_exists(filepath)){return false;}

    std::ifstream file(filepath);
    if(!file.is_open()){return false;}

    content.clear();

    std::string line;
    while(std::getline(file, line)){
        content.push_back(line);
    }

    file.close();
    return true;

}

bool fileIO::save(const std::string& filepath, std::vector<std::string>& content){
    std::ofstream file(filepath);
    if(!file.is_open()){return false;}

    for(const std::string& line:content){
        file << line << '\n';

    }
    file.close();
    return true;
}



bool fileIO::file_exists(const std::string& filepath){
    struct stat buffer;
    return (stat(filepath.c_str(), &buffer)==0);
}
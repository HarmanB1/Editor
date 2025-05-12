#pragma once

#include <string>
#include <fstream>
#include <vector>

class fileIO{
public:
    static bool load(const std::string& filepath, std::vector<std::string>& content);
    static bool save(const std::string& filepath, std::vector<std::string>& content);

    static bool file_exists(const std::string& filepath);
};



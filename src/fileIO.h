/**
 * @file fileIO.h
 * @brief Main file handling class for ediotr
 * 
 * Contains file loading, saving and creating systems
 * 
 * @author Harman Bhogal
 * @date 2025-5-25
 */

#pragma once
#include <string>
#include <fstream>
#include <vector>

class fileIO{
public:
    static bool load(const std::string& filepath, std::vector<std::string>& content);
    static bool save(const std::string& filepath, std::vector<std::string>& content);
    static bool file_create(const std::string& filepath);

    static bool file_exists(const std::string& filepath);
};



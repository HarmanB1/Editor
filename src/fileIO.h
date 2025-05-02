#pragma once



#include <string>
#include <vector>

class fileIO{
public:
    static bool load();
    static bool save();
private:
    static bool file_exists();
};



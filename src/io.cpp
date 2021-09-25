#include "include/io.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::string read_file(std::string filename)
{
    std::ifstream file(filename);

    if (file)
    {
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    std::cout << "Could not read file `" << filename << "`\n";
    exit(1);
}
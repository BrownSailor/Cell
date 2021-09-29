#include "include/io.h"
#include <iostream>
#include <fstream>
#include <sstream>

/* readFile
 *    Purpose: read a file into a string
 * Parameters: file name as a string
 *    Returns: string, exits if the file is invalid
 */
std::string readFile(std::string fileName)
{
    std::ifstream file(fileName);

    if (file)
    {
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    std::cout << "Could not read file `" << fileName << "`\n";
    exit(1);
}

/* writeFile
 *    Purpose: write a string into a text file
 * Parameters: file name as a string, code to write
 *    Returns: None
 */
void writeFile(std::string fileName, std::string code)
{
    std::ofstream file(fileName);

    if (file)
    {
        file << code;
        file.close();
        return;
    }

    std::cout << "Could not write to file `" << fileName << "`\n";
    exit(1);
}
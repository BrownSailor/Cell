#ifndef IO_H
#define IO_H

#include <string>

std::string readFile(std::string fileName);
void writeFile(std::string fileName, std::string code);

#endif
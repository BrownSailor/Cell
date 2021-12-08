#include <string>
#include <map>
#include <fstream>
#include <vector>

int strip_col(std::string line, int col);
std::map<int, std::string> lex_line(std::string line);
std::vector<std::string> lex_file(std::string input_file);
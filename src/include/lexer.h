#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include "token.h"

std::string remove_comments(std::string input_file);
int strip_col(std::string line, int col);
Token lex_word(std::string word);
std::map<int, Token> lex_line(std::string line);
std::vector<Token> lex_file(std::string input_file);

#endif
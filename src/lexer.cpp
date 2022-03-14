#include "include/lexer.h"
#include <iostream>

std::string remove_comments(std::string input_file)
{
    std::string file, line;
    std::ifstream in;
    in.open(input_file);

    while (getline(in, line))
    {
        file += line.substr(0, line.find("//")) + "\n";
    }

    in.close();

    return file;
}

int strip_col(std::string line, int col)
{
    while (col < line.size() && isspace(line[col])) col++;

    return col;
}

std::map<int, std::string> lex_line(std::string line)
{
    std::map<int, std::string> tokens;
    int col = strip_col(line, 0);
    while (col < line.size())
    {
        int col_end = line.find(' ', col);

        if (col_end < 0)
        {
            col_end = line.size();
        }

        tokens.insert({col, line.substr(col, col_end - col)});
        col = strip_col(line, col_end);
    }

    return tokens;
}

std::vector<std::string> lex_file(std::string input_file)
{
    // std::ifstream in;
    // in.open(input_file);
    std::string file = remove_comments(input_file);
    std::stringstream ss(file);

    std::vector<std::string> file_tokens;
    std::string line;

    int row = 0;
    while (getline(ss, line))
    {
        // line = line.substr(0, line.find("//"));
        std::map<int, std::string> col_vals = lex_line(line);

        for (auto [k, v] : col_vals)
        {
            file_tokens.push_back(input_file + ":" + std::to_string(row + 1) + ":" + std::to_string(k + 1) + ": " + v);
        }
        row++;
    }

    ss.clear();

    return file_tokens;
}
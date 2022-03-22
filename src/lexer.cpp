#include "include/lexer.h"
#include <iostream>

std::string remove_comments(std::string input_file)
{
    std::string file, line;
    std::ifstream in;

    try
    {
        in.open(input_file);
            
        while (getline(in, line))
        {
            file += (line.substr(0, line.find("//")) + "\n");
        }

        in.close();

        return file;
    }

    catch(const std::exception& e)
    {
        std::cout << "Usage: cell <SUBCOMMAND> [ARGS]\n";
        std::cout << "SUBCOMMANDS:\n";
        std::cout << "    sim <input files>                      Simulate the program\n";
        std::cout << "    com <executable name> <input files>    Compile the program\n";
        std::cerr << "ERROR: could not find file `" << input_file << "`\n";
        
        exit(EXIT_FAILURE);
        
        return "";
    }
}

int strip_col(std::string line, int col)
{
    while (col < line.size() && isspace(line[col])) col++;

    return col;
}

Token lex_word(std::string word)
{
    try
    {
        return { .type = TOKEN_INT, .int_val = std::stoi(word) };
    }
    catch(const std::exception& e)
    {
        return { .type = TOKEN_WORD, .str_val = word };
    }
}

std::map<int, Token> lex_line(std::string line)
{
    std::map<int, Token> tokens;
    int col = strip_col(line, 0);
    while (col < line.size())
    {
        int col_end = -1;
        if (line[col] == '"')
        {
            col_end = line.find('"', col + 1);
            if (col_end < 0)
            {
                col_end = line.size();
            }

            Token token = { .type = TOKEN_STR, .str_val = line.substr(col + 1, col_end - col - 1) };
            tokens.insert({ col, token });
            col = strip_col(line, col_end + 1);
        }

        else
        {
            col_end = line.find(' ', col);
            if (col_end < 0)
            {
                col_end = line.size();
            }

            tokens.insert({ col, lex_word(line.substr(col, col_end - col)) });
            col = strip_col(line, col_end);
        }
    }

    return tokens;
}

std::vector<Token> lex_file(std::string input_file)
{
    std::string file = remove_comments(input_file);
    std::stringstream ss(file);

    std::vector<Token> file_tokens;
    std::string line;

    int row = 0;
    while (getline(ss, line))
    {
        // line = line.substr(0, line.find("//"));
        std::map<int, Token> col_vals = lex_line(line);

        for (auto [col, v] : col_vals)
        {
            std::string location = input_file + ":" + 
                                   std::to_string(row + 1) + ":" + 
                                   std::to_string(col + 1) + ": " + 
                                   (v.type == TOKEN_INT ? std::to_string(v.int_val) : v.str_val);

            // std::string location = input_file + ":" + 
            //                        std::to_string(row + 1) + ":" + 
            //                        std::to_string(col + 1) + ": ";

            v.loc = location;
            file_tokens.push_back(v);
        }
        row++;
    }

    ss.clear();

    return file_tokens;
}
#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType
{
    TOKEN_ID,           // 0
    TOKEN_INT,          // 1
    TOKEN_COLON,        // 2
    TOKEN_DOUBLE_COLON, // 3
    TOKEN_COMMA,        // 4
    TOKEN_LBRACE,       // 5
    TOKEN_RBRACE,       // 6
    TOKEN_LBRACKET,     // 7
    TOKEN_RBRACKET,     // 8
    TOKEN_EQUALS,       // 9
    TOKEN_SEMI,         // 10
    TOKEN_EOF           // 11
};

struct Token
{
    std::string data;
    unsigned int type;
};

Token *init_token(std::string data, unsigned int type);

std::string token_to_string(int type);

#endif
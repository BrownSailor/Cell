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
    TOKEN_LPAREN,       // 7
    TOKEN_RPAREN,       // 8
    TOKEN_LBRACKET,     // 9
    TOKEN_RBRACKET,     // 10
    TOKEN_EQUALS,       // 11
    TOKEN_SEMI,         // 12
    TOKEN_QUESTION,     // 13
    TOKEN_AT,           // 14
    TOKEN_LT,           // 15
    TOKEN_GT,           // 16
    TOKEN_LTE,          // 17
    TOKEN_GTE,          // 18
    TOKEN_QUOTE,        // 19
    TOKEN_LINE,         // 20
    TOKEN_DOUBLE_LINE,  // 21
    TOKEN_AND,          // 22
    TOKEN_DOUBLE_AND,   // 23
    TOKEN_DOT,          // 24
    TOKEN_DASH,         // 25
    TOKEN_EOF           // 26
};

struct Token
{
    std::string data;
    unsigned int type;
};

Token *initToken(std::string data, unsigned int type);
std::string tokenTypeToString(unsigned int type);
std::string tokenToString(Token *token);

#endif
#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <list>
#include <unordered_map>
#include <fstream>

struct Token
{
    enum Type
    {
        /* Keywords */
        KEY_NUM,
        KEY_BOOL,
        KEY_STR,
        KEY_UNIT,
        KEY_NIL,
        KEY_PRINT,

        KEY_FUN,

        /* Symbols */
        TOK_LPAREN,
        TOK_LBRACK,
        TOK_LBRACE,
        TOK_RPAREN,
        TOK_RBRACK,
        TOK_RBRACE,

        TOK_ARROW,
        TOK_SEMI,
        TOK_COLON,

        TOK_ADD,
        TOK_SUB,
        TOK_MUL,
        TOK_DIV,
        TOK_MOD,
        TOK_SHL,
        TOK_SHR,

        TOK_AND,
        TOK_OR,
        TOK_NOT,

        TOK_EQ,
        TOK_NEQ,
        TOK_LT,
        TOK_LTE,
        TOK_GT,
        TOK_GTE,

        TOK_LOOP,
        TOK_IF,
        TOK_ELSE,

        /* Literals */
        TOK_NUM,
        TOK_TRU,
        TOK_FLS,
        TOK_STR,

        TOK_ID,
    };

    Type type;
    std::string data;

    int col;
    int row;
    std::string file;
};

Token new_token(Token::Type type, std::string data, int col, int row, std::string file);
std::list<Token> lex(const std::string &file);
void print_token(Token token);

#endif

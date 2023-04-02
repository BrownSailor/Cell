#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <list>
#include <unordered_map>
#include <fstream>

struct Token {
    enum Type {

        /* Keywords */
        KEY_NUM,
        KEY_BOOL,
        KEY_STR,
        KEY_NIL,

        KEY_FUN,

        KEY_PRINT,
        KEY_PRINTLN,

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

        TOK_IN,
        TOK_OUT,
        TOK_READ,
        TOK_WRITE,

        TOK_LOOP,
        TOK_TO,
        TOK_BY,
        TOK_IF,
        TOK_ELSE,

        /* Types */
        TOK_NUM,
        TOK_TRU,
        TOK_FLS,
        TOK_STR,
        TOK_ID,

        TOK_EOL,
    };

    Type type;
    std::string data;

    int col;
    int row;
    std::string file;
};

std::list<Token> lex(const std::string &file);
void print_token(Token token);

#endif

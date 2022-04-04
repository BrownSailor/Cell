#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <assert.h>

/*
 * Token 
 * contains the type of token and the value of the token, as well as the 
 * location the token is found at in the file
 */
struct Token
{
    enum Type
    {
        // Non built-ins
        TOK_ID,
        TOK_NUM,
        TOK_LIST,
        TOK_FUNC,
        TOK_PROG,

        // Intrinsic keywords
        TOK_RETURN,
        TOK_INT,
        TOK_CHAR,
        TOK_BOOL,
        TOK_STR,

        // Unary operators
        TOK_BANG,
        TOK_TILDA,
        TOK_ARR,

        // Binary operators
        TOK_PLUS,
        TOK_MINUS,  // also unary
        TOK_STAR,
        TOK_SLASH,
        TOK_PERCENT,
        TOK_LOR,
        TOK_LAND,
        TOK_SHL,
        TOK_SHR,

        // Boolean operations
        TOK_EQEQ,
        TOK_NEQ,
        TOK_LT,
        TOK_GT,
        TOK_LTE,
        TOK_GTE,

        // Single character tokens
        TOK_LBRACE,
        TOK_RBRACE,
        TOK_LPAREN,
        TOK_RPAREN,
        TOK_LBRACK,
        TOK_RBRACK,
        TOK_COL,
        TOK_EQ,
        TOK_COM,
        TOK_EOL,
        TOK_EOF
    };

    Type type;
    std::string data = "";
    int row = 0;
    int col = 0;
    std::string file = "";
};

extern std::unordered_map<std::string, Token::Type> INTRINSICS;

Token lex_word(const std::string &word, int row, int col, const std::string &file);
void lex_line(const std::string &line, std::list<Token> &tokens, int row, const std::string &file);
std::list<Token> lex(const std::string &input);

void print_token(const Token &token, std::ostream &out=std::cout, bool new_line=true);
void print_location(const Token &token, std::ostream &out=std::cout);
void print_lex(const std::list<Token> &tokens, std::ostream &out=std::cout);

#endif
#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

/*
 * Token 
 * contains the type of token and the value of the token, as well as the 
 * location the token is found at in the file
 */
struct Token
{
    enum Type
    {
        TOK_LBRACE,
        TOK_RBRACE,
        TOK_LPAREN,
        TOK_RPAREN,
        TOK_INT,
        TOK_RETURN,
        TOK_ID,
        TOK_NUM,
        TOK_COL,
        TOK_COM,
        TOK_STAR,
        TOK_EOL,
        TOK_EOF
    };

    Type type;
    std::string data = "";
    int line = 0;
};

Token lex_word(const std::string &word, int line);
void lex_line(const std::string &line, std::vector<Token> &tokens, int line_no);
std::vector<Token> lex(const std::string &input);

void print_token(const Token &token, std::ostream &out=std::cout);
void print_lex(const std::vector<Token> &tokens);

#endif
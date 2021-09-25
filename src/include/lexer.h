#ifndef LEXER_H
#define LEXER_H

#include "token.h"

struct Lexer
{
    std::string src;
    char currChar;
    unsigned int currIdx;
};

Lexer *init_lexer(std::string src);

void lexer_advance(Lexer *lexer);

char lexer_peek(Lexer *lexer);

void lexer_skip_whitespace(Lexer *lexer);

Token *lexer_next_token(Lexer *lexer);

Token *lexer_advance_with(Lexer *lexer, Token *token);

Token *lexer_advance_current(Lexer *lexer, int type);

Token *lexer_parse_id(Lexer *lexer);

Token *lexer_parse_number(Lexer *lexer);

#endif
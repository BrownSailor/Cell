#ifndef LEXER_H
#define LEXER_H

#include "token.h"

struct Lexer
{
    std::string src;
    char currChar;
    unsigned int currIdx;
};

Lexer *initLexer(std::string src);
void lexerAdvance(Lexer *lexer);
char lexerPeek(Lexer *lexer);
void lexerSkipWhitespace(Lexer *lexer);
Token *lexerNextToken(Lexer *lexer);
Token *lexerAdvanceWith(Lexer *lexer, Token *token);
Token *lexerAdvanceCurrent(Lexer *lexer, int type);
Token *lexerParseId(Lexer *lexer);
Token *lexerParseNumber(Lexer *lexer);

#endif
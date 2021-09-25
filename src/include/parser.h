#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"

struct Parser
{
    Lexer *lexer;
    Token *token;
};

Parser *initParser(Lexer *lexer);

Token *parserEat(Parser *parser, int type);

Tree *parserParse(Parser *parser);

#endif
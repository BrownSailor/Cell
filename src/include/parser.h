#ifndef PARSER_H
#define PARSER_H

#include "tree.h"
#include "lexer.h"

struct Parser
{
    Lexer *lexer;
    Token *token;
};

Parser *initParser(Lexer *lexer);
Token *parserEat(Parser *parser, unsigned int type);
Tree *parserParse(Parser *parser);
Tree *parserParseId(Parser *parser);
Tree *parserParseInt(Parser *parser);
Tree *parserParseExpr(Parser *parser);
Tree *parserParseList(Parser *parser);
Tree *parserParseBlock(Parser *parser);
Tree *parserParseCompound(Parser *parser);

#endif
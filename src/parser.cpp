#include "include/parser.h"
#include <iostream>

Parser *initParser(Lexer *lexer)
{
    Parser *parser = new Parser();
    parser->lexer = lexer;
    parser->token = lexerNextToken(lexer);

    return parser;
}

Token *parserEat(Parser *parser, int type)
{
    if (parser->token->type != type)
    {
        std::cout << "[Parser]: Unexpected token `" << tokenToString(parser->token)
                  << "`, expected: `" << tokenTypeToString(type) << "`\n";
    }
}

Tree *parserParse(Parser *parser)
{
}
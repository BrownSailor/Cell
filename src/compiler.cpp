#include "include/compiler.h"
#include <iostream>

void compile(std::string src)
{
    Lexer *lexer = initLexer(src);
    Token *token = lexerNextToken(lexer);

    while (token->type != TOKEN_EOF)
    {
        std::cout << tokenToString(token) << "\n";
        token = lexerNextToken(lexer);
    }

    delete token;
    delete lexer;
}

void compile_file(std::string filename)
{
    std::string src = readFile(filename);
    compile(src);
}
#include "include/compiler.h"
#include <iostream>

void compile(std::string src)
{
    Lexer *lexer = init_lexer(src);
    Token *token = lexer_next_token(lexer);

    while (token->type != TOKEN_EOF)
    {
        std::cout << "TOKEN: [" << token->data << "]\t[" << token_to_string(token->type) << "]\n";
        token = lexer_next_token(lexer);
    }
}

void compile_file(std::string filename)
{
    std::string src = read_file(filename);
    compile(src);
}
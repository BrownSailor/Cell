#include "include/lexer.h"

int main(int argc, char **argv)
{
    (void)(argc);
    
    std::vector<Token> tokens = lex(argv[1]);
    print_lex(tokens);

    return 0;
}
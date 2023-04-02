#include <iostream>

#include "lexer.h"
#include "parser.h"
#include "compiler.h"

void usage()
{
    std::cerr << "Usage: ./slang executableName inputFile\n";
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        usage();
    }

    std::string file = argv[1];

    /* lex file to tokens */
    std::list<Token> tokens = lex(file);

    /* parse abstract syntax tree */
    Node *root = parse_program(tokens);

    /* generate assembly based on syntax tree */
    compile_program(root);

    /* assemble and link object files */
    // std::string assemble = "nasm -felf64 " + file + ".asm";
    // std::string link = "ld -o " + file + " " + file + ".o";

    // system(assemble.c_str());
    // system(link.c_str());
    
    free_tree(root);
    return 0;
}

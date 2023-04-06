#include <iostream>

#include "lexer.h"
#include "built_in.h"
#include "parser.h"
#include "compiler.h"

void usage()
{
    std::cerr << "Usage: ./slang executableName inputFile\n";
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    (void)argv;
    initialize_built_ins();

    if (argc != 2)
    {
        usage();
    }

    std::string file = argv[1];

    /* lex file to tokens */
    std::list<Token> tokens = lex(file);

    /* parse abstract syntax tree */
    Node *root = parse_program(tokens);
    pretty_print(root);

    /* generate assembly based on syntax tree */
    compile(root);

    /* create a relocatable object file */
    // std::string assemble = "llc -march=x86_64 -filetype=obj " + file + ".ll -o " + file + ".o";
    // system(assemble.c_str());
    
    free_tree(root);
    return 0;
}

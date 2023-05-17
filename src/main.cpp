#include <iostream>

#include "lexer.hpp"
#include "built_in.hpp"
#include "parser.hpp"
#include "types.hpp"
#include "compiler.hpp"

/* optional include for debugging */
#include "debug.hpp"

void usage()
{
    std::cerr << "slang compiler\n";
    std::cerr << "Usage: ./slang file\n";
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    initialize_types();
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

    /* type check the AST */
    type_check(root);

    /* generate assembly based on syntax tree */
    compile(root);
    
    free_tree(root);
    free_built_ins();

    return 0;
}

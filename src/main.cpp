#include "include/assembler.h"

int main(int argc, char **argv)
{
    (void)(argc);

    std::list<Token> tokens = lex(argv[2]);
    std::cout << "\n=== TOKENS ===\n\n";
    print_lex(tokens);

    std::cout << "\n=== TREE ===\n\n";
    Node *root = parse_program(tokens);
    pretty_print(root);

    std::cout << "\n=== CODE ===\n\n";
    print_assembled(root);
    assemble("main.c", root);

    return 0;
}
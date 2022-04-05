#include "include/compiler.h"

int main(int argc, char **argv)
{
    (void)(argc);

    std::list<Token> tokens = lex(argv[2]);
    // print_lex(tokens);

    Node *root = parse_program(tokens);
    pretty_print(root);

    compile_and_link(argv[1], root);

    return 0;
}
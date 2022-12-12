#include "include/compiler.h"
#include "include/types.h"
#include <cstring>

int main(int argc, char **argv)
{
    (void)(argc);

    std::list<Token> tokens = lex(argv[2]);
    print_lex(tokens);

    Node *root = parse_program(tokens);
    root = set_types(root, root->scope);
    pretty_print(root);

    // if (!check_errors(root, root->scope))
    // {
    //     std::cerr << "Compilation failed\n";
    //     exit(EXIT_FAILURE);
    // }

    // compile_and_link(argv[1], root);

    return 0;
}

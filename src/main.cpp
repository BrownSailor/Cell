#include "include/util.h"
#include "include/compiler.h"
#include <cstring>

int main(int argc, char **argv)
{
    (void)(argc);

    std::list<Token> tokens = lex(argv[2]);
    Node *root = parse_program(tokens);

    if (argc > 3 && !strcmp(argv[3], "--debug")) pretty_print(root);

    // if (!check_errors(root, root->scope))
    // {
    //     std::cerr << "Compilation failed\n";
    //     exit(EXIT_FAILURE);
    // }

    compile_and_link(argv[1], root);

    return 0;
}

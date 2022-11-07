#include "include/type.h"
#include "include/compiler.h"

int main(int argc, char **argv)
{
    (void)(argc);

    std::list<Token> tokens = lex(argv[2]);
    Node *root = parse_program(tokens);

    if (!type_check(root, root->scope))
    {
        std::cerr << "Compilation failed\n";
        exit(EXIT_FAILURE);
    }

    compile_and_link(argv[1], root);

    return 0;
}

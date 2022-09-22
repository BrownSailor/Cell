#include "include/builtin.h"

std::string builtin_print(Node *root, std::string expr)
{
    std::string print = "";

    switch (root->token.type)
    {
        case Token::TOK_CHAR:
        {
            print += "printf(\"%c\", " + expr + ");\n";
            break;
        }

        case Token::TOK_STR:
        {
            print += "printf(\"%s\", " + expr + ");\n";
            break;
        }

        default: break;
    }

    return print;

}

std::string builtin_println(Node *root, std::string expr)
{
    std::string println = "";

    switch (root->token.type)
    {
        case Token::TOK_CHAR:
        {
            println += "printf(\"%c\\n\", " + expr + ");\n";
            break;
        }

        case Token::TOK_STR:
        {
            println += "printf(\"%s\\n\", " + expr + ");\n";
            break;
        }

        default: break;
    }

    return println;
}
#include "include/builtin.h"

std::string builtin_print(Node *root, const Scope &scope, std::string expr)
{
    std::string print = "";
    Token::Type type = eval_node(root, scope);

    if (!root->arr_dim)
    {
        switch (type)
        {
            case Token::TOK_CHAR:
            case Token::KEY_CHAR:
            {
                print += "printf(\"%c\", " + expr + ")";
                break;
            }
            case Token::TOK_STR:
            {
                print += "printf(\"%s\", " + expr + ")";
                break;
            }
            case Token::TOK_INT:
            case Token::KEY_INT:
            {
                print += "printf(\"%lld\", (long long)(" + expr + "))";
                break;
            }
            case Token::KEY_UINT:
            {
                print += "printf(\"%lu\", (unsigned long)(" + expr + "))";
                break;
            }

            default:
            {
                // TODO: fix this lol
                print += "printf(\"%lld\", (long long)(" + expr + "))";
                break;
            }
        }
    }
    else
    {
        switch ((root->arr_dim - root->children.size()))
        {
            case 0:
            {
                switch (type)
                {
                    case Token::TOK_CHAR:
                    case Token::KEY_CHAR:
                    {
                        print += "printf(\"%c\", " + expr + ")";
                        break;
                    }
                    case Token::TOK_STR:
                    {
                        print += "printf(\"%s\", " + expr + ")";
                        break;
                    }
                    case Token::TOK_INT:
                    {
                        print += "printf(\"%lld\", (long long)(" + expr + "))";
                        break;
                    }
                    case Token::TOK_UINT:
                    {
                        print += "printf(\"%lu\", (unsigned long)(" + expr + "))";
                        break;
                    }

                    default: break;
                }

                break;
            }

            default:
            {
                print += "printf(\"%s\", " + expr + ".to_string())";
                break;
            }
        }
    }

    return print;
}

std::string builtin_println(Node *root, const Scope &scope, std::string expr)
{
    std::string println = "";
    Token::Type type = eval_node(root, scope);

    if (!root->arr_dim)
    {
        switch (type)
        {
            case Token::TOK_CHAR:
            {
                println += "printf(\"%c\\n\", " + expr + ")";
                break;
            }
            case Token::TOK_STR:
            {
                println += "printf(\"%s\\n\", " + expr + ")";
                break;
            }
            case Token::TOK_INT:
            {
                println += "printf(\"%lld\\n\", (long long)(" + expr + "))";
                break;
            }
            case Token::TOK_UINT:
            {
                println += "printf(\"%lu\\n\", (unsigned long)(" + expr + "))";
                break;
            }

            default: break;
        }
    }
    else
    {
        switch ((root->arr_dim - root->children.size()))
        {
            case 0:
            {
                switch (type)
                {
                    case Token::TOK_CHAR:
                    {
                        println += "printf(\"%c\\n\", " + expr + ")";
                        break;
                    }
                    case Token::TOK_STR:
                    {
                        println += "printf(\"%s\\n\", " + expr + ")";
                        break;
                    }
                    case Token::TOK_INT:
                    {
                        println += "printf(\"%lld\\n\", (long long)(" + expr + "))";
                        break;
                    }
                    case Token::TOK_UINT:
                    {
                        println += "printf(\"%lu\\n\", (unsigned long)(" + expr + "))";
                        break;
                    }

                    default: break;
                }

                break;
            }
            default:
            {
                println += "printf(\"%s\\n\", " + expr + ".to_string())";
                break;
            }
        }
    }

    return println;
}

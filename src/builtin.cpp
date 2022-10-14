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
                print += "printf(\"%lld\", " + expr + ")";
                break;
            }
            case Token::KEY_UINT:
            {
                print += "printf(\"%llu\", " + expr + ")";
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
                        print += "printf(\"%lld\", " + expr + ")";
                        break;
                    }
                    case Token::KEY_UINT:
                    {
                        print += "printf(\"%llu\", " + expr + ")";
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
            case Token::KEY_CHAR:
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
            case Token::KEY_INT:
            {
                println += "printf(\"%lld\\n\", " + expr + ")";
                break;
            }
            case Token::KEY_UINT:
            {
                println += "printf(\"%llu\\n\", " + expr + ")";
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
                    case Token::KEY_CHAR:
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
                    case Token::KEY_INT:
                    {
                        println += "printf(\"%lld\\n\", " + expr + ")";
                        break;
                    }
                    case Token::KEY_UINT:
                    {
                        println += "printf(\"%llu\\n\", " + expr + ")";
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

std::string builtin_size(Node *root, std::string expr)
{
    (void)(root);
    std::string size = expr + ".size()";

    return size;
}

std::string builtin_pushb(Node *root, std::string expr1, std::string expr2)
{
    (void)(root);
    std::string pushb = expr1 + ".pushb(" + expr2 + ")";
    return pushb;
}

std::string builtin_popf(Node *root, std::string expr)
{
    (void)(root);
    std::string popf = expr + ".popf()";
    return popf;
}

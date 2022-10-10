#include "include/builtin.h"

std::string builtin_print(Node *root, const Scope &scope, std::string expr)
{
    std::string print = "";
    Token::Type type = eval_node(root, scope);
    (void)(type);

    if (!root->arr_size)
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
                print += "printf(\"%ld\", " + expr + ")";
                break;
            }
            case Token::KEY_UINT:
            {
                print += "printf(\"%lu\", " + expr + ")";
                break;
            }

            default: break;
        }
    }
    else
    {
        switch ((root->arr_size - root->children.size()))
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
                        print += "printf(\"%ld\", " + expr + ")";
                        break;
                    }
                    case Token::KEY_UINT:
                    {
                        print += "printf(\"%lu\", " + expr + ")";
                        break;
                    }

                    default: break;
                }

                break;
            }
            case 1:
            {
                switch (type)
                {
                    case Token::KEY_CHAR:
                    {
                        print += "printf(\"%s\", " + expr + ")";
                        break;
                    }

                    default:
                    {
                        print += "printf(\"%x\", " + expr + ")";
                        break;
                    }
                }

                break;
            }

            default:
            {
                print += "printf(\"%x\", " + expr + ")";
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
    (void)(type);

    if (!root->arr_size)
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
                println += "printf(\"%ld\\n\", " + expr + ")";
                break;
            }
            case Token::KEY_UINT:
            {
                println += "printf(\"%lu\\n\", " + expr + ")";
                break;
            }

            default: break;
        }
    }
    else
    {
        switch ((root->arr_size - root->children.size()))
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
                        println += "printf(\"%ld\\n\", " + expr + ")";
                        break;
                    }
                    case Token::KEY_UINT:
                    {
                        println += "printf(\"%lu\\n\", " + expr + ")";
                        break;
                    }

                    default: break;
                }

                break;
            }
            case 1:
            {
                switch (type)
                {
                    case Token::KEY_CHAR:
                    {
                        println += "printf(\"%s\\n\", " + expr + ")";
                        break;
                    }

                    default:
                    {
                        println += "printf(\"%x\\n\", " + expr + ")";
                        break;
                    }
                }

                break;
            }

            default:
            {
                println += "printf(\"%x\\n\", " + expr + ")";
                break;
            }
        }
    }

    return println;
}

#include "include/builtin.h"

std::string builtin_print(Node *root, const Scope &scope, std::string expr)
{
    (void)(scope);
    std::string print = "";
    // Token::Type type = eval_node(root, scope);
    Token::Type type = Token::TOK_NONE;

    if (!root->arr_dim)
    {
        switch (type)
        {
            case Token::KEY_BOOL:
            case Token::TOK_BOOL:
            {
                print += "printf(\"%s\", " + expr + " ? \"tru\" : \"fls\")";
                break;
            }
            case Token::KEY_CHAR:
            case Token::TOK_CHAR:
            {
                print += "printf(\"%c\", " + expr + ")";
                break;
            }
            case Token::TOK_STR:
            {
                print += "printf(\"%s\", " + expr + ")";
                break;
            }
            case Token::KEY_INT:
            case Token::TOK_INT:
            {
                print += "printf(\"%lld\", (long long)(" + expr + "))";
                break;
            }
            case Token::KEY_UINT:
            case Token::TOK_UINT:
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
                    case Token::KEY_BOOL:
                    case Token::TOK_BOOL:
                    {
                        print += "printf(\"%d\", " + expr + " ? \"tru\" : \"fls\")";
                        break;
                    }
                    case Token::KEY_CHAR:
                    case Token::TOK_CHAR:
                    {
                        print += "printf(\"%c\", " + expr + ")";
                        break;
                    }
                    case Token::TOK_STR:
                    {
                        print += "printf(\"%s\", " + expr + ")";
                        break;
                    }
                    case Token::KEY_INT:
                    case Token::TOK_INT:
                    {
                        print += "printf(\"%lld\", (long long)(" + expr + "))";
                        break;
                    }
                    case Token::KEY_UINT:
                    case Token::TOK_UINT:
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
    (void)(scope);
    std::string println = "";
    // Token::Type type = eval_node(root, scope);
    Token::Type type = Token::TOK_NONE;

    if (!root->arr_dim)
    {
        switch (type)
        {
            case Token::KEY_BOOL:
            case Token::TOK_BOOL:
            {
                println += "printf(\"%s\\n\", " + expr + " ? \"tru\" : \"fls\")";
                break;
            }
            case Token::KEY_CHAR:
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
            case Token::KEY_INT:
            case Token::TOK_INT:
            {
                println += "printf(\"%lld\\n\", (long long)(" + expr + "))";
                break;
            }
            case Token::KEY_UINT:
            case Token::TOK_UINT:
            {
                println += "printf(\"%lu\\n\", (unsigned long)(" + expr + "))";
                break;
            }

            default:
            {
                // TODO: fix this lol
                println += "printf(\"%lld\\n\", (long long)(" + expr + "))";
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
                    case Token::KEY_BOOL:
                    case Token::TOK_BOOL:
                    {
                        println += "printf(\"%d\\n\", " + expr + " ? \"tru\" : \"fls\")";
                        break;
                    }
                    case Token::KEY_CHAR:
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
                    case Token::KEY_INT:
                    case Token::TOK_INT:
                    {
                        println += "printf(\"%lld\\n\", (long long)(" + expr + "))";
                        break;
                    }
                    case Token::KEY_UINT:
                    case Token::TOK_UINT:
                    {
                        println += "printf(\"%lu\\n\", (unsigned long)(" + expr + "))";
                        break;
                    }

                    default:
                    {
                        // TODO: fix this lol
                        println += "printf(\"%lld\\n\", (long long)(" + expr + "))";
                        break;
                    }
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

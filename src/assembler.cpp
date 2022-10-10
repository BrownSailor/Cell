#include "include/assembler.h"

/*
 * assemble_unary
 *    Purpose: writes assembly for a unary operation
 * Parameters: node of the unary operation
 *    Returns: a string of assembly code
 */
std::string assemble_unary(Node *root, const Scope &scope)
{
    std::string expr = "";

    if (root == nullptr)
    {
        return "";
    }

    expr += root->token.data;
    if (root->children.size())
    {
        expr += "(";
        expr += assemble_expr(root->children.front(), scope);
        expr += ")";
    }

    return expr;
}

/*
 * assemble_binary
 *    Purpose: writes assembly for a binary operation
 * Parameters: node of the binary operation
 *    Returns: a string of assembly code
 */
std::string assemble_binary(Node *root, const Scope &scope)
{
    std::string expr = "";

    if (root == nullptr)
    {
        return "";
    }

    expr += "(";
    expr += assemble_expr(root->children.front(), scope);
    expr += ")";

    expr += root->token.data;

    expr += "(";
    expr += assemble_expr(root->children.back(), scope);
    expr += ")";

    return expr;
}

std::string assemble_type(Node *root, bool ptr)
{
    std::string type = "";

    /* Converting the type of the variable to the corresponding C type. */
    switch (root->token.type)
    {
        case Token::KEY_BOOL:
        {
            type += "int8_t";
            break;
        }
        case Token::KEY_CHAR:
        {
            type += "char";
            break;
        }
        case Token::KEY_INT:
        {
            type += "int64_t";
            break;
        }
        case Token::KEY_UINT:
        {
            type += "uint64_t";
            break;
        }
        case Token::KEY_VOID:
        {
            type += "void";
            break;
        }

        default:
        {
            print_error("Unknown type `" + root->token.data + "`", root->token);
            exit(EXIT_FAILURE);
        }
    }

    if (ptr)
    {
        for (int i = 0; i < root->arr_size; i++)
        {
            type += '*';
        }
    }

    return type;
}

std::string assemble_expr(Node *root, const Scope &scope)
{
    std::string expr = "";

    switch (root->token.type)
    {
        case Token::KEY_RETURN:
        {
            expr += "return " + assemble_expr(root->children.front(), scope);
            break;
        }

        case Token::KEY_DUMP:
        {
            expr += builtin_print(root->children.front(), scope, assemble_expr(root->children.front(), scope));
            break;
        }

        case Token::KEY_DUMPLN:
        {
            expr += builtin_println(root->children.front(), scope, assemble_expr(root->children.front(), scope));
            break;
        }

        case Token::TOK_CHAR:
        {
            expr += "'" + root->token.data + "'";
            break;
        }

        case Token::TOK_STR:
        {
            expr += "\"" + root->token.data + "\"";
            break;
        }

        case Token::TOK_PLUS:
        case Token::TOK_STAR:
        case Token::TOK_SLASH:
        case Token::TOK_PERCENT:
        case Token::TOK_EQEQ:
        case Token::TOK_NEQ:
        case Token::TOK_LT:
        case Token::TOK_LTE:
        case Token::TOK_GT:
        case Token::TOK_GTE:
        case Token::TOK_LAND:
        case Token::TOK_LOR:
        {
            expr += assemble_binary(root, scope);
            break;
        }

        case Token::TOK_MINUS:
        {
            if (root->children.size() == 1)
            {
                expr += assemble_unary(root, scope);
            }
            else
            {
                expr += assemble_binary(root, scope);
            }

            break;
        }

        case Token::TOK_ARR:
        {
            expr += "{";
            for (auto it = root->children.begin(); it != root->children.end(); std::advance(it, 1))
            {
                expr += assemble_expr(*it, scope);
                expr += ",";
            }
            expr[expr.size() - 1] = '}';

            break;
        }

        case Token::TOK_ID:
        {
            switch (root->type)
            {
                case Node::NODE_VAR:
                {
                    expr += root->token.data;

                    for (auto it = root->children.begin(); it != root->children.end(); std::advance(it, 1))
                    {
                        expr += "[";
                        expr += assemble_expr(*it, scope);
                        expr += "]";
                    }
                    break;
                }
                case Node::NODE_VAR_ASN:
                {
                    if (root->children.size() > 1)
                    {
                        expr += root->token.data;
                        auto end = root->children.begin();
                        std::advance(end, root->children.size() - 1);

                        for (auto it = root->children.begin(); it != end; std::advance(it, 1))
                        {
                            expr += "[";
                            expr += assemble_expr(*it, scope);
                            expr += "]";
                        }

                        expr += " = ";
                    }
                    else
                    {
                        expr += root->token.data + " = ";
                    }

                    expr += assemble_expr(root->children.front(), scope);
                    break;
                }
                case Node::NODE_VAR_DEC:
                {
                    expr += assemble_type(root->children.front()) + " ";
                    expr += root->token.data;
                    break;
                }
                case Node::NODE_VAR_DEC_ASN:
                {
                    expr += assemble_type(root->children.front(), false) + " ";

                    if (root->arr_size)
                    {
                        expr += root->token.data;
                        for (int i = 0; i < root->arr_size; i++)
                        {
                            expr += "[]";
                        }

                        expr += " = ";
                    }
                    else
                    {
                        expr += root->token.data + " = ";
                    }

                    expr += assemble_expr(root->children.back(), scope);
                    break;
                }
                case Node::NODE_FUNC_CALL:
                {
                    expr += root->token.data + "(";
                    for (auto it = root->children.begin(); it != root->children.end(); std::advance(it, 1))
                    {
                        expr += assemble_expr(*it, scope) + ",";
                    }
                    expr[expr.size() - 1] = ')';

                    break;
                }

                default: break;
            }

            break;
        }

        default:
        {
            expr += root->token.data;
            break;
        }
    }

    expr += (root->semi ? ";\n" : "");

    return expr;
}

std::string assemble_if(Node *root)
{
    std::string iff = (root->token.type == Token::TOK_IF ? "if (" : root->token.type == Token::TOK_ELIF ? "else if (" : "else");

    if (root->token.type != Token::TOK_ELSE)
    {
        iff += assemble_expr(root->children.front(), root->scope);
        iff += ")";
    }
    iff += " {\n";

    if (root->children.back()->token.type == Token::TOK_ELSE ||
        root->children.back()->token.type == Token::TOK_ELIF)
    {
        auto it = root->children.begin();
        std::advance(it, 1);

        auto end = root->children.begin();
        std::advance(end, root->children.size() - 1);

        iff += assemble_body(it, end, root->scope);
        iff += "}\n";

        iff += assemble_if(*it);
    }
    else
    {
        auto it = root->children.begin();

        if (root->token.type != Token::TOK_ELSE)
        {
            std::advance(it, 1);
        }

        iff += assemble_body(it, root->children.end(), root->scope);
        iff += "}\n";
    }

    return iff;
}

std::string assemble_loop(Node *root)
{
    std::string loop = "";

    auto x = root->children.begin();

    switch (root->children.front()->children.size())
    {
        case 0:
        {
            loop += "for (;;) {\n";
            std::advance(x, 1);
            loop += assemble_body(x, root->children.end(), root->scope);
            loop += "}\n";
            break;
        }
        case 3:
        {
            auto it = root->children.front()->children.begin();
            loop += "for (";
            loop += assemble_expr(*it, root->scope);
            std::advance(it, 1);

            loop += ";";
            loop += assemble_expr(*it, root->scope);
            std::advance(it, 1);

            loop += ";";
            loop += assemble_expr(*it, root->scope) + ") {\n";

            std::advance(x, 1);
            loop += assemble_body(x, root->children.end(), root->scope);

            loop += "}\n";

            break;
        }
    }

    return loop;
}

std::string assemble_body(std::list<Node *>::iterator &it, const std::list<Node *>::iterator &end, const Scope &scope)
{
    std::string body = "";
    while (it != end)
    {
        switch ((*it)->token.type)
        {
            case Token::TOK_IF:
            {
                body += assemble_if(*it);
                break;
            }
            case Token::TOK_LOOP:
            {
                body += assemble_loop(*it);
                break;
            }
            default:
            {
                body += assemble_expr(*it, scope);
                break;
            }
        }
        std::advance(it, 1);
    }

    return body;
}

std::string assemble_function(Node *root)
{
    std::string function = "";

    auto x = root->children.begin();
    std::advance(x, 1);

    std::string type = assemble_type(*x);
    std::string name = root->token.data;

    if (name == "main")
    {
        type = "int32_t";
    }
    std::string args = "";

    if (root->children.front()->token.type == Token::TOK_LIST)
    {
        auto it = root->children.front()->children.begin();
        for (size_t i = 0; i < root->children.front()->children.size(); i++)
        {
            args += assemble_expr(*it, root->scope);
            if (i != root->children.front()->children.size() - 1)
            {
                args += ", ";
            }
            std::advance(it, 1);
        }
    }

    function += type + " " + name + "(" + args + ") {\n";
    std::advance(x, 1);

    function += assemble_body(x, root->children.end(), root->scope);
    function += "}\n";

    return function;
}

std::string assemble_program(Node *root)
{
    std::string program = "";
    program += "#include <stdio.h>\n";
    program += "#include <stdint.h>\n";

    for (Node *child : root->children)
    {
        program += assemble_function(child);
    }

    return program;
}

/*
 * assemble
 *    Purpose: writes native assembly for a program
 * Parameters: node of the program
 *    Returns: a string of assembly code
 */
void assemble(std::string filename, Node *root)
{
    std::ofstream out;
    out.open(filename);

    out << assemble_program(root);
}

void print_assembled(Node *root)
{
    std::cout << assemble_program(root);
}

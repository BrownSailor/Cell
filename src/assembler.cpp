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

std::string assemble_type(Node *root)
{
    std::string type = "";

    /* Converting the type of the variable to the corresponding C type. */
    switch (root->token.type)
    {
        case Token::KEY_BYTE:
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

        case Token::KEY_SHORT:
        {
            type += "int16_t";
            break;
        }

        case Token::KEY_INT:
        {
            type += "int32_t";
            break;
        }

        case Token::KEY_LONG:
        {
            type += "int64_t";
            break;
        }

        case Token::KEY_ARR:
        {
            type += assemble_type(root->children.front());
            type += "*";
        }

        default:
        {
            print_error("Unknown type `" + root->token.data + "`", root->token);
            exit(EXIT_FAILURE);
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
            expr += "return " + assemble_expr(root->children.front(), scope) + ";\n";
            break;
        }

        case Token::KEY_PRINT:
        {
            expr += builtin_print(root->children.front(), assemble_expr(root->children.front(), scope));
            break;
        }

        case Token::KEY_PRINTLN:
        {
            expr += builtin_println(root->children.front(), assemble_expr(root->children.front(), scope));
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

        case Token::TOK_ID:
        {
            switch (root->children.size())
            {
                case 0:
                {
                    expr += root->token.data;
                    break;
                }
                case 1:
                {
                    expr += assemble_expr(root->children.back(), scope) + " ";
                    expr += root->token.data;
                    break;
                }
                case 2:
                {
                    expr += assemble_type(root->children.front()) + " ";
                    expr += root->token.data + " = ";
                    expr += assemble_expr(root->children.back(), scope);
                    break;
                }
            }
            
            break;
        }

        default:
        {
            expr += root->token.data;
            break;
        }
    }

    return expr;
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
    for (auto it = x; it != root->children.end(); std::advance(it, 1))
    {
        function += assemble_expr(*it, root->scope);
    }

    function += "}\n";

    return function;
}

std::string assemble_program(Node *root)
{
    std::string program = "";
    program += "#include <stdio.h>\n";

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
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

std::string assemble_post_unary(Node *root, const Scope &scope)
{
    std::string expr = "";

    if (root == nullptr)
    {
        return "";
    }

    if (root->children.size())
    {
        expr += "(" + assemble_expr(root->children.front(), scope) + ")";
        expr += root->token.data;
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

std::string assemble_type(Node *root, size_t size)
{
    std::string type = "";

    if (size == 1 && root->token.type == Token::KEY_CHAR)
    {
        type += "__built_in::string";
        return type;
    }

    if (size)
    {
        type += "__built_in::array<" + assemble_type(root, size - 1) + ">";
        return type;
    }

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
            if (structs.count(root->token.data))
            {
                type += root->token.data + "*";
                break;
            }
            print_error("Unknown type `" + root->token.data + "`", root->token);
            exit(EXIT_FAILURE);
        }
    }

    return type;
}

std::string assemble_expr(Node *root, const Scope &scope, Node *parent, size_t arr_size)
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
        case Token::TOK_SHL:
        case Token::TOK_SHR:
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

        case Token::TOK_BANG:
        case Token::TOK_TILDA:
        case Token::TOK_INC:
        case Token::TOK_DEC:
        {
            expr += assemble_unary(root, scope);
            break;
        }

        case Token::TOK_POST_INC:
        case Token::TOK_POST_DEC:
        {
            expr += assemble_post_unary(root, scope);
            break;
        }

        case Token::TOK_ACCESS:
        {
            expr += "->" + assemble_expr(root->children.front(), scope);
            break;
        }

        case Token::TOK_ARR:
        {
            expr += "__built_in::array<" + assemble_type(parent->children.front(), arr_size - 1) + ">(";
            expr += "{";
            for (auto it = root->children.begin(); it != root->children.end(); std::advance(it, 1))
            {
                expr += assemble_expr(*it, scope, parent, arr_size - 1);
                expr += ",";
            }
            expr[expr.size() - 1] = '}';
            expr += ")";

            break;
        }

        case Token::KEY_NEW:
        case Token::KEY_DELETE:
        {
            expr += root->token.data + " " + assemble_expr(root->children.front(), scope);
            break;
        }

        case Token::TOK_ID:
        {
            switch (root->type)
            {
                case Node::NODE_VAR:
                {
                    expr += root->token.data;

                    if (root->arr_dim)
                    {
                        for (auto it = root->children.begin(); it != root->children.end(); std::advance(it, 1))
                        {
                            expr += "[";
                            expr += assemble_expr(*it, scope);
                            expr += "]";
                        }
                    }

                    if (root->children.size() && root->children.front()->token.type == Token::TOK_ACCESS)
                    {
                        expr += assemble_expr(root->children.front(), scope);
                    }
                    break;
                }
                case Node::NODE_VAR_ASN:
                {
                    if (root->arr_dim && (root->children.size() == root->arr_dim + 1 || root->children.size() == root->arr_dim + 2))
                    {
                        expr += root->token.data;

                        auto begin = root->children.begin();
                        if (root->children.front()->token.type == Token::TOK_ACCESS)
                        {
                            std::advance(begin, 1);
                            expr += assemble_expr(root->children.front(), scope);
                        }

                        auto end = root->children.begin();
                        std::advance(end, root->children.size() - 1);

                        for (auto it = begin; it != end; std::advance(it, 1))
                        {
                            expr += "[";
                            expr += assemble_expr(*it, scope);
                            expr += "]";
                        }

                        expr += " = ";
                        expr += assemble_expr(root->children.back(), scope, scope.at(root->token.data), root->arr_dim);
                    }
                    else
                    {
                        expr += root->token.data;

                        if (root->children.front()->token.type == Token::TOK_ACCESS)
                        {
                            expr += assemble_expr(root->children.front(), scope);
                        }
                        
                        // pretty_print(scope.at(root->token.data));
                        expr += " = " + assemble_expr(root->children.back(), scope, scope.at(root->token.data), root->arr_dim);
                    }

                    break;
                }
                case Node::NODE_VAR_DEC:
                {
                    std::string type = assemble_type(root->children.front(), root->arr_dim);
                    expr += type + " " + root->token.data;

                    if (structs.count(type.substr(0, type.size() - 1)) && parent == nullptr)
                    {
                        expr += " = new " + type.substr(0, type.size() - 1);
                    }

                    break;
                }
                case Node::NODE_VAR_DEC_ASN:
                {
                    std::string type = assemble_type(root->children.front(), root->arr_dim);
                    expr += type + " " + root->token.data + " = ";

                    if (root->arr_dim && root->children.back()->token.type == Token::TOK_ARR)
                    {
                        expr += assemble_expr(root->children.back(), scope, root, root->arr_dim);
                    }
                    else
                    {
                        expr += assemble_expr(root->children.back(), scope);
                    }

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
        case 1:
        {
            loop += "for (;";
            loop += assemble_expr(root->children.front()->children.front(), root->scope);
            loop += ";) {\n";
            std::advance(x, 1);

            loop += assemble_body(x, root->children.end(), root->scope);
            loop += "}\n";
            break;
        }
        case 2:
        {
            loop += "for (";
            bool b = false;

            // initializer
            if (root->children.front()->children.front()->type == Node::NODE_VAR_DEC_ASN ||
                root->children.front()->children.front()->type == Node::NODE_VAR_ASN)
            {
                loop += assemble_expr(root->children.front()->children.front(), root->scope);
                b = true;
            }
            loop += ";";

            if (b)
            {
                loop += assemble_expr(root->children.front()->children.back(), root->scope);
            }
            else
            {
                loop += assemble_expr(root->children.front()->children.front(), root->scope);
            }
            loop += ";";

            // increment
            if (root->children.front()->children.back()->type == Node::NODE_VAR_ASN ||
                root->children.front()->children.back()->token.type == Token::TOK_INC ||
                root->children.front()->children.back()->token.type == Token::TOK_DEC ||
                root->children.front()->children.back()->token.type == Token::TOK_POST_INC ||
                root->children.front()->children.back()->token.type == Token::TOK_POST_DEC)
            {
                loop += assemble_expr(root->children.front()->children.back(), root->scope);
            }
            loop += ") {\n";
            std::advance(x, 1);

            loop += assemble_body(x, root->children.end(), root->scope) + "}\n";

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

std::string assemble_body(std::list<Node *>::iterator &it, const std::list<Node *>::iterator &end, const Scope &scope, Node *parent)
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
                body += assemble_expr(*it, scope, parent);
                break;
            }
        }
        std::advance(it, 1);
    }

    return body;
}

std::string assemble_struct(Node *root)
{
    std::string object = "struct ";
    object += root->token.data + " {\n";

    auto it = root->children.begin();
    object += assemble_body(it, root->children.end(), root->scope, root);
    object += "};\n";

    return object;
}

std::string assemble_function(Node *root)
{
    std::string function = "";

    auto x = root->children.begin();
    std::advance(x, 1);

    std::string type = assemble_type(*x, (*x)->arr_dim);
    std::string name = root->token.data;
    std::string args = "";

    if (name == "main")
    {
        type = "int32_t";
        args = "int32_t argc, char** argv";
    }
    else
    {
        if (root->children.front()->token.type == Token::TOK_LIST)
        {
            auto it = root->children.front()->children.begin();
            for (size_t i = 0; i < root->children.front()->children.size(); i++)
            {
                args += assemble_type((*it)->children.front(), (*it)->arr_dim);

                if ((*it)->arr_dim)
                {
                    args += "&";
                }

                args += " " + (*it)->token.data;
                if (i != root->children.front()->children.size() - 1)
                {
                    args += ", ";
                }
                std::advance(it, 1);
            }
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
    program += "#include \"std/util.h\"\n";
    program += "#include \"std/array.h\"\n";
    program += "#include \"std/string.h\"\n";

    for (Node *child : root->children)
    {
        if (child->token.type == Token::KEY_CPP)
        {
            program += child->token.data;
        }
        else if (child->token.type == Token::KEY_STRUCT)
        {
            program += assemble_struct(child);
        }
        else
        {
            program += assemble_function(child);
        }
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

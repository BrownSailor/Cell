#include "include/assembler.h"

/*
 * assemble_unary
 *    Purpose: writes assembly for a unary operation
 * Parameters: node of the unary operation
 *    Returns: a string of assembly code
 */
std::string assemble_unary(Node *root, const std::unordered_map<std::string, Node *> &scope)
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
        expr += assemble_unary(root->children.front(), scope);
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
std::string assemble_binary(Node *root, const std::unordered_map<std::string, Node *> &scope)
{
    std::string expr = "";

    if (root == nullptr)
    {
        return "";
    }

    if (root->children.size())
    {
        expr += "(";
        expr += assemble_binary(root->children.front(), scope);
        expr += ")";
    }
    expr += root->token.data;

    if (root->children.size())
    {
        expr += "(";
        expr += assemble_binary(root->children.back(), scope);
        expr += ")";
    }

    return expr;
}

std::string assemble_type(Node *root)
{
    std::string type = "";

    /* Converting the type of the variable to the corresponding C type. */
    if (root->token.type == Token::TOK_BYTE || root->token.type == Token::TOK_BOOL)
    {
        type += "int8_t";
    }
    else if (root->token.type == Token::TOK_CHAR)
    {
        type += "char";
    }
    else if (root->token.type == Token::TOK_SHORT)
    {
        type += "int16_t";
    }
    else if (root->token.type == Token::TOK_INT)
    {
        type += "int32_t";
    }
    else if (root->token.type == Token::TOK_LONG)
    {
        type += "int64_t";
    }
    else if (root->token.type == Token::TOK_STR)
    {
        type += "char*";
    }
    else if (root->token.type == Token::TOK_VOID)
    {
        type += "void";
    }
    else if (root->token.type == Token::TOK_ARR)
    {
        type += assemble_type(root->children.front());
        type += "*";
    }
    else
    {
        print_error("Unknown type `" + root->token.data + "`", root->token);
        exit(EXIT_FAILURE);
    }

    return type;
}

/*
 * assemble_expr
 *    Purpose: writes assembly for an expression
 * Parameters: node of the expression
 *    Returns: a string of assembly code
 */
std::string assemble_expr(Node *root, const std::unordered_map<std::string, Node *> &scope)
{
    std::string expr = "";

    if (root->token.type == Token::TOK_RETURN)
    {
        expr += "return " + assemble_expr(root->children.front(), scope) + ";\n";
    }
    else if (root->token.type == Token::TOK_PRINT)
    {
        if (!root->children.front()->is_arr && !root->children.front()->is_idx)
        {
            std::string param = assemble_expr(root->children.front(), scope);

            if (root->children.front()->expr_type == Token::TOK_LONG ||
                root->children.front()->token.type == Token::TOK_LONG ||
                root->children.front()->token.type == Token::TOK_NUM)
            {
                expr += "printf(\"%lld\", " + param + ");\n";
            }
            else if (root->children.front()->expr_type == Token::TOK_INT ||
                     root->children.front()->token.type == Token::TOK_INT)
            {
                expr += "printf(\"%d\", " + param + ");\n";
            }
            else if (root->children.front()->expr_type == Token::TOK_SHORT ||
                     root->children.front()->token.type == Token::TOK_SHORT)
            {
                expr += "printf(\"%d\", " + param + ");\n";
            }
            else if (root->children.front()->expr_type == Token::TOK_CHAR ||
                     root->children.front()->token.type == Token::TOK_CHAR)
            {
                expr += "printf(\"%c\", " + param + ");\n";
            }
            else if (root->children.front()->expr_type == Token::TOK_BYTE ||
                     root->children.front()->token.type == Token::TOK_BYTE)
            {
                expr += "printf(\"%d\", " + param + ");\n";
            }
            else if (root->children.front()->expr_type == Token::TOK_STR ||
                     root->children.front()->token.type == Token::TOK_STR)
            {
                expr += "printf(\"%s\", " + param + ");\n";
            }
            else if (root->children.front()->expr_type == Token::TOK_BOOL ||
                     root->children.front()->token.type == Token::TOK_BOOL)
            {
                expr += "printf(\"%d\", " + param + ");\n";
            }
        }
        else if (root->children.front()->is_arr)
        {
            if (root->children.front()->expr_type == Token::TOK_CHAR ||
                root->children.front()->token.type == Token::TOK_CHAR)
            {
                expr += "printf(\"%s\", " + assemble_expr(root->children.front(), scope) + ");\n";
            }
        }
        else if (root->children.front()->children.front()->is_idx)
        {
            if (root->children.front()->expr_type == Token::TOK_CHAR ||
                root->children.front()->token.type == Token::TOK_CHAR)
            {
                expr += "printf(\"%c\", " + assemble_expr(root->children.front(), scope) + ");\n";
            }
        }
    }
    else if (root->token.type == Token::TOK_PRINTLN)
    {
        if (!root->children.front()->is_arr && !root->children.front()->is_idx)
        {
            if (root->children.front()->expr_type == Token::TOK_LONG ||
                root->children.front()->token.type == Token::TOK_LONG ||
                root->children.front()->token.type == Token::TOK_NUM)
            {
                expr += "printf(\"%lld\\n\", " + assemble_expr(root->children.front(), scope) + ");\n";
            }
            else if (root->children.front()->expr_type == Token::TOK_INT ||
                     root->children.front()->token.type == Token::TOK_INT)
            {
                expr += "printf(\"%d\\n\", " + assemble_expr(root->children.front(), scope) + ");\n";
            }
            else if (root->children.front()->expr_type == Token::TOK_SHORT ||
                     root->children.front()->token.type == Token::TOK_SHORT)
            {
                expr += "printf(\"%d\\n\", " + assemble_expr(root->children.front(), scope) + ");\n";
            }
            else if (root->children.front()->expr_type == Token::TOK_CHAR ||
                     root->children.front()->token.type == Token::TOK_CHAR)
            {
                expr += "printf(\"%c\\n\", " + assemble_expr(root->children.front(), scope) + ");\n";
            }
            else if (root->children.front()->expr_type == Token::TOK_BYTE ||
                     root->children.front()->token.type == Token::TOK_BYTE)
            {
                expr += "printf(\"%d\\n\", " + assemble_expr(root->children.front(), scope) + ");\n";
            }
            else if (root->children.front()->expr_type == Token::TOK_STR ||
                     root->children.front()->token.type == Token::TOK_STR)
            {
                expr += "printf(\"%s\\n\", " + assemble_expr(root->children.front(), scope) + ");\n";
            }
            else if (root->children.front()->expr_type == Token::TOK_BOOL ||
                     root->children.front()->token.type == Token::TOK_BOOL)
            {
                expr += "printf(\"%d\\n\", " + assemble_expr(root->children.front(), scope) + ");\n";
            }
        }
        else if (root->children.front()->is_arr)
        {
            if (root->children.front()->expr_type == Token::TOK_CHAR ||
                root->children.front()->token.type == Token::TOK_CHAR)
            {
                expr += "printf(\"%s\\n\", " + assemble_expr(root->children.front(), scope) + ");\n";
            }
        }
        else if (root->children.front()->children.front()->is_idx)
        {
            if (root->children.front()->expr_type == Token::TOK_CHAR ||
                root->children.front()->token.type == Token::TOK_CHAR)
            {
                expr += "printf(\"%c\\n\", " + assemble_expr(root->children.front(), scope) + ");\n";
            }
        }
    }
    else if (root->token.type == Token::TOK_ID)
    {
        if (root->children.size() == 1)
        {
            if (root->children.front()->expr_type == Token::TOK_IDX)
            {
                expr += root->token.data + "["; 
                expr += assemble_expr(root->children.front(), scope);
                expr += "]";
            }
            else if (root->children.front()->expr_type == Token::TOK_ARR)
            {
                expr += assemble_type(root->children.front()) + "* ";
                expr += root->token.data;
            }
            else if (INTRINSICS.count(root->children.front()->token.data))
            {
                expr += assemble_type(root->children.front());
                expr += " " + root->token.data;
            }
            else
            {
                expr += root->token.data + " = " + assemble_expr(root->children.back(), scope) + ";\n";
            }
        }
        else if (root->children.size() == 2)
        {
            if (root->is_arr)
            {
                expr += assemble_type(root->children.front()) + "* " + root->token.data + " = " + assemble_expr(root->children.back(), scope) + ";\n";
            }
            else
            {
                expr += assemble_type(root->children.front()) + " " + root->token.data + " = " + assemble_expr(root->children.back(), scope) + ";\n";
            }
        }
        else
        {
            expr += root->token.data;
        }
    }
    else if (root->token.type == Token::TOK_NUM)
    {
        expr += root->token.data;
    }
    else if (root->token.type == Token::TOK_STR)
    {
        expr += "\"" + root->token.data + "\"";
    }
    else if (root->children.size() == 1 &&
            (root->token.type == Token::TOK_MINUS ||
             root->token.type == Token::TOK_TILDA ||
             root->token.type == Token::TOK_BANG ||
             root->token.type == Token::TOK_INC ||
             root->token.type == Token::TOK_DEC))
    {
        expr += assemble_unary(root, scope) + ";\n";
    }
    else if (root->children.size() == 2 && 
            (root->token.type == Token::TOK_PLUS ||
             root->token.type == Token::TOK_MINUS ||
             root->token.type == Token::TOK_STAR ||
             root->token.type == Token::TOK_SLASH ||
             root->token.type == Token::TOK_PERCENT ||
             root->token.type == Token::TOK_EQEQ ||
             root->token.type == Token::TOK_NEQ ||
             root->token.type == Token::TOK_LT ||
             root->token.type == Token::TOK_LTE ||
             root->token.type == Token::TOK_GT ||
             root->token.type == Token::TOK_GTE ||
             root->token.type == Token::TOK_LAND ||
             root->token.type == Token::TOK_LOR))
    {
        expr += assemble_binary(root, scope);
    }

    return expr;
}

std::string assemble_if(Node *root)
{
    std::string iff = "";
    auto it = root->children.begin();

    if (root->token.type == Token::TOK_IF)
    {
        iff += "if (" + assemble_expr(root->children.front(), root->scope) + ") {\n";
        std::advance(it, 1);
    }
    else if (root->token.type == Token::TOK_ELSE &&
            (root->children.front()->token.type == Token::TOK_EQEQ ||
             root->children.front()->token.type == Token::TOK_NEQ ||
             root->children.front()->token.type == Token::TOK_LT ||
             root->children.front()->token.type == Token::TOK_LTE ||
             root->children.front()->token.type == Token::TOK_GT ||
             root->children.front()->token.type == Token::TOK_GTE ||
             root->children.front()->token.type == Token::TOK_LAND ||
             root->children.front()->token.type == Token::TOK_LOR))
    {
        iff += "else if (" + assemble_expr(root->children.front(), root->scope) + ") {\n";
        std::advance(it, 1);
    }
    else
    {
        iff += "else {\n";
    }

    while (it != root->children.end())
    {
        if ((*it)->token.type == Token::TOK_IF ||
            (*it)->token.type == Token::TOK_ELSE)
        {
            iff += assemble_if(*it);
        }
        else if ((*it)->token.type == Token::TOK_LOOP)
        {
            iff += assemble_loop(*it);
        }
        else
        {
            iff += assemble_expr(*it, root->scope);
        }
        std::advance(it, 1);
    }

    // iff += ";\n}\n";
    iff += "}\n";
    return iff;
}

/*
 * assemble_loop
 *    Purpose: writes assembly for a loop
 * Parameters: node of the loop
 *    Returns: a string of assembly code
 */
std::string assemble_loop(Node *root)
{
    std::string loop = "";
    auto list_it = root->children.front()->children.begin();

    if (root->children.front()->children.size() == 3 ||
       (root->children.front()->children.size() == 2 && 
       (*list_it)->token.type == Token::TOK_ID))
    {
        loop += assemble_expr(*list_it, root->scope);
        std::advance(list_it, 1);
    }

    loop += "while (";
    loop += assemble_expr(*list_it, root->scope);
    std::advance(list_it, 1);
    loop += ") {\n";

    auto it = root->children.begin();
    std::advance(it, 1);

    while (it != root->children.end())
    {
        if ((*it)->token.type == Token::TOK_IF ||
            (*it)->token.type == Token::TOK_ELSE)
        {
            loop += assemble_if(*it);
        }
        else if ((*it)->token.type == Token::TOK_LOOP)
        {
            loop += assemble_loop(*it);
        }
        else
        {
            loop += assemble_expr(*it, root->scope);
        }
        std::advance(it, 1);
    }

    if (root->children.front()->children.size() == 3 ||
       (root->children.front()->children.size() == 2 &&
        list_it != root->children.front()->children.end()))
    {
        loop += assemble_expr(*list_it, root->scope);
        std::advance(list_it, 1);
    }

    loop += "}\n";
    
    return loop;
}

/*
 * assemble_function
 *    Purpose: writes assembly for a function
 * Parameters: node of the function
 *    Returns: a string of assembly code
 */
std::string assemble_function(Node *root)
{
    std::string function = "";

    auto x = root->children.begin();
    std::advance(x, 1);

    // TODO: add type checking for inferred void return type
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
        if ((*it)->token.type == Token::TOK_IF ||
            (*it)->token.type == Token::TOK_ELSE)
        {
            function += assemble_if(*it);
        }
        else if ((*it)->token.type == Token::TOK_LOOP)
        {
            function += assemble_loop(*it);
        }
        else
        {
            function += assemble_expr(*it, root->scope);
        }
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
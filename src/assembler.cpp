#include "include/assembler.h"

/*
 * assemble_unary
 *    Purpose: writes assembly for a unary operation
 * Parameters: node of the unary operation
 *    Returns: a string of assembly code
 */
std::string assemble_unary(Node *root)
{
    std::string expr = "";
    auto it = root->children.begin();

    if (root->token.type == Token::TOK_MINUS)
    {
        expr += assemble_expr(*it);
        expr += "\tneg rax\n";
    }
    else if (root->token.type == Token::TOK_TILDA)
    {
        expr += assemble_expr(*it);
        expr += "\tnot rax\n";
    }
    else if (root->token.type == Token::TOK_BANG)
    {
        expr += assemble_expr(*it);
        expr += "\tcmp rax, 0\n";
        expr += "\tsete al\n";
    }

    return expr;
}

/*
 * assemble_binary
 *    Purpose: writes assembly for a binary operation
 * Parameters: node of the binary operation
 *    Returns: a string of assembly code
 */
std::string assemble_binary(Node *root)
{
    std::string expr = "";
    auto it = root->children.begin();

    // root contains the token of the operation and has left and right children
    if (root->token.type == Token::TOK_PLUS)
    {
        expr += assemble_expr(*it);
        std::advance(it, 1);

        expr += "\tpush rax\n";
        expr += assemble_expr(*it);
        std::advance(it, 1);

        expr += "\tpop rcx\n";
        expr += "\tadd rax, rcx\n";
    }
    else if (root->token.type == Token::TOK_MINUS)
    {
        expr += assemble_expr(*it);
        std::advance(it, 1);

        expr += "\tpush rax\n";
        expr += assemble_expr(*it);
        std::advance(it, 1);

        expr += "\tpop rcx\n";
        expr += "\txchg rax, rcx\n";
        expr += "\tsub rax, rcx\n";
    }
    else if (root->token.type == Token::TOK_STAR)
    {
        expr += assemble_expr(*it);
        std::advance(it, 1);

        expr += "\tpush rax\n";
        expr += assemble_expr(*it);
        std::advance(it, 1);

        expr += "\tpop rcx\n";
        expr += "\tmul rcx\n";
    }
    else if (root->token.type == Token::TOK_SLASH)
    {
        expr += assemble_expr(*it);
        std::advance(it, 1);

        expr += "\txor rdx, rdx\n";
        expr += "\tpush rax\n";
        expr += assemble_expr(*it);
        std::advance(it, 1);

        expr += "\tpop rcx\n";
        expr += "\txchg rax, rcx\n";
        expr += "\tdiv rcx\n";
    }
    else if (root->token.type == Token::TOK_PERCENT)
    {
        expr += assemble_expr(*it);
        std::advance(it, 1);

        expr += "\txor rdx, rdx\n";
        expr += "\tpush rax\n";
        expr += assemble_expr(*it);
        std::advance(it, 1);

        expr += "\tpop rcx\n";
        expr += "\txchg rax, rcx\n";
        expr += "\tdiv rcx\n";
        expr += "\txchg rax, rdx\n";
    }

    return expr;
}

/*
 * assemble_expr
 *    Purpose: writes assembly for an expression
 * Parameters: node of the expression
 *    Returns: a string of assembly code
 */
std::string assemble_expr(Node *root)
{
    std::string expr = "";

    if (root->token.type == Token::TOK_NUM)
    {
        expr += "\tmov rax, " + root->token.data + "\n";
    }
    else if (root->token.type == Token::TOK_PLUS || 
             root->token.type == Token::TOK_STAR || 
             root->token.type == Token::TOK_SLASH || 
             root->token.type == Token::TOK_PERCENT)
    {
        expr += assemble_binary(root);
    }
    else if (root->token.type == Token::TOK_MINUS && root->children.size() == 2)
    {
        expr += assemble_binary(root);
    }
    else if (root->token.type == Token::TOK_TILDA || 
            root->token.type == Token::TOK_BANG || 
            root->token.type == Token::TOK_MINUS || 
            root->token.type == Token::TOK_ARR)
    {
        expr += assemble_unary(root);
    }
    else
    {
        for (auto it = root->children.begin(); it != root->children.end(); std::advance(it, 1))
        {
            expr += assemble_expr(*it);
        }
    }

    return expr;
}

/*
 * assemble_statement
 *    Purpose: writes assembly for a statement
 * Parameters: node of the statement
 *    Returns: a string of assembly code
 */
std::string assemble_statement(Node *root)
{
    std::string statement = "";
    for (Node *child : root->children)
    {
        statement += assemble_expr(child);
    }

    statement += "\tret\n";
    return statement;
}

/*
 * assemble_function
 *    Purpose: writes assembly for a function
 * Parameters: node of the function
 *    Returns: a string of assembly code
 */
std::string assemble_function(Node *root)
{
    std::string code = "";

    // write function name
    code += "global _" + root->token.data + "\n";
    code += "_" + root->token.data + ":\n";

    // TODO: do something for function type instead of just dropping it
    if (root->children.front()->token.type == Token::TOK_ID)
    {
        root->children.pop_front();
    }
    
    // TODO: do something for function params
    if (root->children.front()->token.type == Token::TOK_LIST)
    {
        root->children.pop_front();
    }

    code += assemble_statement(root);
    
    return code;
}

/*
 * assemble_nasm
 *    Purpose: writes nasm 64-bit assembly for a program
 * Parameters: node of the program
 *    Returns: a string of assembly code
 */
void assemble_nasm(std::string filename, Node *root)
{
    std::ofstream out;
    out.open(filename);

    out << "default rel\n";
    out << "\n";
    out << "section .text\n";

    out << assemble_function(root);
}
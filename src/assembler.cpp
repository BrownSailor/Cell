#include "include/assembler.h"

std::string assemble_expr(Node *root)
{
    std::string expr = "";

    for (auto it = root->children.begin(); it != root->children.end(); it++)
    {
        if ((*it)->token.type == Token::TOK_MINUS)
        {
            expr += assemble_expr(*it);
            expr += "\tneg rax\n";
        }
        else if ((*it)->token.type == Token::TOK_TILDA)
        {
            expr += assemble_expr(*it);
            expr += "\tnot rax\n";
        }
        else if ((*it)->token.type == Token::TOK_BANG)
        {
            expr += assemble_expr(*it);
            expr += "\tcmp rax, 0\n";
            expr += "\tsete al\n";
        }
        else
        {
            expr += "\tmov rax, " + (*it)->token.data + "\n";
        }
    }

    return expr;
}

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

std::string assemble_function(Node *root)
{
    std::string code = "";

    // write function name
    code += "global _" + root->token.data + "\n";
    code += "_" + root->token.data + ":\n";

    // TODO: do something for function type instead of just dropping it
    root->children.pop_front();

    code += assemble_statement(root);
    
    return code;
}

void assemble_nasm(std::string filename, Node *root)
{
    std::ofstream out;
    out.open(filename);

    out << "default rel\n";
    out << "\n";
    out << "section .text\n";

    out << assemble_function(root);
}
#include "include/assembler.h"

std::string assemble_expr(Node *root)
{
    std::string expr = "\tmov rax, ";
    expr += root->children.front()->token.data + "\n";

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
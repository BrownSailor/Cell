#include "include/assembler.h"

// global jump address variable for short circuit assembling
int jmp_addr = 0;

/*
 * assemble_unary
 *    Purpose: writes assembly for a unary operation
 * Parameters: node of the unary operation
 *    Returns: a string of assembly code
 */
std::string assemble_unary(Node *root, const std::unordered_map<std::string, Node *> &scope)
{
    std::string expr = "";
    auto it = root->children.begin();

    if (root->token.type == Token::TOK_MINUS)
    {
        expr += assemble_expr(*it, scope);
        expr += "\tneg\t\trax\n";
    }
    else if (root->token.type == Token::TOK_TILDA)
    {
        expr += assemble_expr(*it, scope);
        expr += "\tnot\t\trax\n";
    }
    else if (root->token.type == Token::TOK_BANG)
    {
        expr += assemble_expr(*it, scope);
        expr += "\tcmp\t\trax, 0\n";
        expr += "\tsete\tal\n";
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
    auto it = root->children.begin();

    // arith
    if (root->token.type == Token::TOK_PLUS)
    {
        expr += assemble_expr(*it, scope);
        std::advance(it, 1);

        expr += "\tpush\trax\n";
        expr += assemble_expr(*it, scope);
        std::advance(it, 1);

        expr += "\tpop\t\trcx\n";
        expr += "\tadd\t\trax, rcx\n";
    }
    else if (root->token.type == Token::TOK_MINUS)
    {
        std::advance(it, 1);
        expr += assemble_expr(*it, scope);

        expr += "\tpush\trax\n";
        std::advance(it, -1);

        expr += assemble_expr(*it, scope);
        std::advance(it, 2);

        expr += "\tpop\t\trcx\n";
        expr += "\tsub\t\trax, rcx\n";
    }
    else if (root->token.type == Token::TOK_STAR)
    {
        expr += assemble_expr(*it, scope);
        std::advance(it, 1);

        expr += "\tpush\trax\n";
        expr += assemble_expr(*it, scope);
        std::advance(it, 1);

        expr += "\tpop\t\trcx\n";
        expr += "\tmul\t\trcx\n";
    }
    else if (root->token.type == Token::TOK_SLASH)
    {
        std::advance(it, 1);
        expr += assemble_expr(*it, scope);

        expr += "\txor\t\trdx, rdx\n";
        expr += "\tpush\trax\n";
        std::advance(it, -1);

        expr += assemble_expr(*it, scope);
        std::advance(it, 2);

        expr += "\tpop\t\trcx\n";
        expr += "\tdiv\t\trcx\n";
    }
    else if (root->token.type == Token::TOK_PERCENT)
    {
        std::advance(it, 1);
        expr += assemble_expr(*it, scope);

        expr += "\txor\t\trdx, rdx\n";
        expr += "\tpush\trax\n";
        std::advance(it, -1);

        expr += assemble_expr(*it, scope);
        std::advance(it, 2);

        expr += "\tpop\t\trcx\n";
        expr += "\tdiv\t\trcx\n";
        expr += "\txchg\trax, rdx\n";
    }
    else if (root->token.type == Token::TOK_SHL)
    {
        std::advance(it, 1);
        expr += assemble_expr(*it, scope);

        expr += "\tpush\trax\n";
        std::advance(it, -1);

        expr += assemble_expr(*it, scope);
        std::advance(it, 2);

        expr += "\tpop\t\trcx\n";
        expr += "\tshl\t\trax, cl\n";
    }
    else if (root->token.type == Token::TOK_SHR)
    {
        std::advance(it, 1);
        expr += assemble_expr(*it, scope);

        expr += "\tpush\trax\n";
        std::advance(it, -1);

        expr += assemble_expr(*it, scope);
        std::advance(it, 2);

        expr += "\tpop\t\trcx\n";
        expr += "\tshr\t\trax, cl\n";
    }

    // boolean
    else if (root->token.type == Token::TOK_EQEQ)
    {
        expr += assemble_expr(*it, scope);
        std::advance(it, 1);

        expr += "\tpush\trax\n";
        expr += assemble_expr(*it, scope);
        std::advance(it, 1);

        expr += "\tpop\t\trcx\n";
        expr += "\tcmp\t\trax, rcx\n";
        expr += "\tmov\t\trax, 0\n";
        expr += "\tsete\tal\n";
    }
    else if (root->token.type == Token::TOK_NEQ)
    {
        expr += assemble_expr(*it, scope);
        std::advance(it, 1);

        expr += "\tpush\trax\n";
        expr += assemble_expr(*it, scope);
        std::advance(it, 1);

        expr += "\tpop\t\trcx\n";
        expr += "\tcmp\t\trax, rcx\n";
        expr += "\tmov\t\trax, 0\n";
        expr += "\tsetne\tal\n";
    }

    // for comparisons, first value goes into rcx and second goes into rax, so we swap the comparison
    else if (root->token.type == Token::TOK_LT)
    {
        expr += assemble_expr(*it, scope);
        std::advance(it, 1);

        expr += "\tpush\trax\n";
        expr += assemble_expr(*it, scope);
        std::advance(it, 1);

        expr += "\tpop\t\trcx\n";
        expr += "\tcmp\t\trax, rcx\n";
        expr += "\tmov\t\trax, 0\n";
        expr += "\tsetg\tal\n";
    }
    else if (root->token.type == Token::TOK_LTE)
    {
        expr += assemble_expr(*it, scope);
        std::advance(it, 1);

        expr += "\tpush\trax\n";
        expr += assemble_expr(*it, scope);
        std::advance(it, 1);

        expr += "\tpop\t\trcx\n";
        expr += "\tcmp\t\trax, rcx\n";
        expr += "\tmov\t\trax, 0\n";
        expr += "\tsetge\tal\n";
    }
    else if (root->token.type == Token::TOK_GT)
    {
        expr += assemble_expr(*it, scope);
        std::advance(it, 1);

        expr += "\tpush\trax\n";
        expr += assemble_expr(*it, scope);
        std::advance(it, 1);

        expr += "\tpop\t\trcx\n";
        expr += "\tcmp\t\trax, rcx\n";
        expr += "\tmov\t\trax, 0\n";
        expr += "\tsetl\tal\n";
    }
    else if (root->token.type == Token::TOK_GTE)
    {
        expr += assemble_expr(*it, scope);
        std::advance(it, 1);

        expr += "\tpush\trax\n";
        expr += assemble_expr(*it, scope);
        std::advance(it, 1);

        expr += "\tpop\t\trcx\n";
        expr += "\tcmp\t\trax, rcx\n";
        expr += "\tmov\t\trax, 0\n";
        expr += "\tsetle\tal\n";
    }
    else if (root->token.type == Token::TOK_LAND)
    {
        expr += assemble_expr(*it, scope);
        std::advance(it, 1);
        
        expr += "\tcmp\t\trax, 0\n";
        expr += "\tmov\t\trax, 0\n";
        expr += "\tsetne\tal\n";
        expr += "\tje jmp_addr_" + std::to_string(jmp_addr) + "\n\n";

        expr += "\tpush rax\n";
        expr += assemble_expr(*it, scope);
        std::advance(it, 1);

        expr += "\tpop\t\trcx\n";
        expr += "\tand\t\trax, rcx\n";
        expr += "jmp_addr_" + std::to_string(jmp_addr++) + ":\n";
    }
    else if (root->token.type == Token::TOK_LOR)
    {
        expr += assemble_expr(*it, scope);
        std::advance(it, 1);

        expr += "\tcmp\t\trax, 0\n";
        expr += "\tmov\t\trax, 0\n";
        expr += "\tsetne\tal\n";
        expr += "\tcmp\t\trax, 1\n";
        expr += "\tje\t\tjmp_addr_" + std::to_string(jmp_addr) + "\n\n";

        expr += "\tpush\trax\n";
        expr += assemble_expr(*it, scope);
        std::advance(it, 1);

        expr += "\tpop\t\trcx\n";
        expr += "\tor\t\trax, rcx\n";
        expr += "jmp_addr_" + std::to_string(jmp_addr++) + ":\n";
    }

    return expr;
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

    // TODO: use type of variable to select the correct type of register, i.e. eax vs rax

    if (root->token.type == Token::TOK_DUMP)
    {
        expr += assemble_expr(root->children.front(), scope);
        expr += "\tpush\trax\n";
        expr += "\tpop\t\trdi\n";
        expr += "\tcall\t_dump\n";
    }
    else if (root->token.type == Token::TOK_ID)
    {
        if (root->children.size() == 2)
        {
            // second child has the expression to parse
            auto it = root->children.begin();
            std::advance(it, 1);

            expr += assemble_expr(*it, scope);
            expr += "\tpush\trax\n";
            expr += "\tmov\t\t[rbp-" + std::to_string(scope.at(root->token.data)->offset) + "], eax\n";
        }
        else if (root->children.size() == 1 && !TYPES.count(root->token.type))
        {
            auto it = root->children.begin();
            expr += assemble_expr(*it, scope);
            expr += "\tmov\t\t[rbp-" + std::to_string(scope.at(root->token.data)->offset) + "], eax\n";
        }
        else if (root->children.size() == 0)
        {
            expr += "\tmov\t\teax, [rbp-" + std::to_string(scope.at(root->token.data)->offset) + "]\n"; 
        }
    }
    else if (root->token.type == Token::TOK_NUM)
    {
        expr += "\tmov\t\trax, " + root->token.data + "\n";
    }
    else if (root->token.type == Token::TOK_PLUS || 
             root->token.type == Token::TOK_STAR || 
             root->token.type == Token::TOK_SLASH || 
             root->token.type == Token::TOK_PERCENT ||
             root->token.type == Token::TOK_SHL || 
             root->token.type == Token::TOK_SHR || 
             root->token.type == Token::TOK_EQEQ || 
             root->token.type == Token::TOK_NEQ ||
             root->token.type == Token::TOK_LT || 
             root->token.type == Token::TOK_LTE ||
             root->token.type == Token::TOK_GT ||
             root->token.type == Token::TOK_GTE ||
             root->token.type == Token::TOK_LAND ||
             root->token.type == Token::TOK_LOR)
    {
        expr += assemble_binary(root, scope);
    }
    else if ((root->token.type == Token::TOK_MINUS ||
              root->token.type == Token::TOK_STAR) && 
              root->children.size() == 2)
    {
        expr += assemble_binary(root, scope);
    }
    else if (root->token.type == Token::TOK_TILDA || 
             root->token.type == Token::TOK_BANG || 
             root->token.type == Token::TOK_MINUS || 
             root->token.type == Token::TOK_STAR)
    {
        expr += assemble_unary(root, scope);
    }
    else
    {
        for (auto it = root->children.begin(); it != root->children.end(); std::advance(it, 1))
        {
            expr += assemble_expr(*it, scope);
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
std::string assemble_statement(Node *root, const std::unordered_map<std::string, Node *> &scope)
{
    std::string statement = "";
    for (Node *child : root->children)
    {
        statement += assemble_expr(child, scope);
    }

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
    std::string function = "";

    // write function name
    function += "global _" + root->token.data + "\n";
    function += "_" + root->token.data + ":\n";

    // function prologue
    function += "\tpush\trbp\n";
    function += "\tmov\t\trbp, rsp\n";

    // push argc and argv on the stack if we are assembling main
    if (root->token.data == "main")
    {
        function += "\tmov\t\t[rbp-" + std::to_string(root->children.front()->children.front()->offset) + "], edi\n";
        function += "\tmov\t\t[rbp-" + std::to_string(root->children.front()->children.back()->offset) + "], rsi\n";
    }

    // TODO: do something for function type instead of just dropping it
    if (INTRINSICS.count(root->children.front()->token.data))
    {
        root->children.pop_front();
    }
    
    // TODO: do something for function params
    if (root->children.front()->token.type == Token::TOK_LIST)
    {
        root->children.pop_front();
    }

    function += assemble_statement(root, root->scope);
    
    // function epilogue
    function += "\tmov\t\trsp, rbp\n";
    function += "\tpop\t\trbp\n";
    function += "\tret\n";

    return function;
}

std::string assemble_program(Node *root)
{
    std::string program = "default rel\n\n";
    program += ";; dump function written in C, compiled to nasm with -O3\n"
        ";; dump takes a 64-bit unsigned integer from rdi and prints it to stdout\n"
        "_dump:\n"
        "\tsub     rsp, 40\n"
        "\tmov     BYTE [rsp+31], 10\n"
        "\ttest    rdi, rdi\n"
        "\tje      .L4\n"
        "\tmov     r8, -3689348814741910323\n"
        "\tmov     esi, 30\n"
".L3:\n"
        "\tmov     rax, rdi\n"
        "\tsub     rsi, 1\n"
        "\tmul     r8\n"
        "\tmov     rax, rdi\n"
        "\tshr     rdx, 3\n"
        "\tlea     rcx, [rdx+rdx*4]\n"
        "\tadd     rcx, rcx\n"
        "\tsub     rax, rcx\n"
        "\tadd     eax, 48\n"
        "\tmov     BYTE [rsp+1+rsi], al\n"
        "\tmov     rax, rdi\n"
        "\tmov     rdi, rdx\n"
        "\tcmp     rax, 9\n"
        "\tja      .L3\n"
        "\tmov     edx, 32\n"
        "\tsub     rdx, rsi\n"
".L2:\n"
        "\tadd     rsi, rsp\n"
        "\tmov     edi, 1\n"
        "\txor     eax, eax\n"
        "\tmov     rax, 0x2000004\n"
        "\tsyscall\n"
        "\tadd     rsp, 40\n"
        "\tret\n"
".L4:\n"
        "\tmov     edx, 2\n"
        "\tmov     esi, 30\n"
        "\tjmp     .L2\n\n";

    program += "section .text\n";
    for (Node *child : root->children)
    {
        program += assemble_function(child);
    }

    return program;
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

    out << assemble_program(root);
}
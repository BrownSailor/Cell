#include "include/assembler.h"

// global jump address variable for short circuit assembling
int jmp = 0;

int get_stack_alignment(Node *root)
{
    if (root == nullptr)
    {
        return 0;
    }

    int stack_alignment = 0;
    for (auto it = root->children.begin(); it != root->children.end(); std::advance(it, 1))
    {
        stack_alignment = std::max(stack_alignment, get_stack_alignment(*it));
    }

    if (root->scope.size())
    {
        for (auto [k, v] : root->scope)
        {
            stack_alignment = std::max(stack_alignment, v->offset);
        }
    }

    return stack_alignment;
}

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
        expr += "    neg     rax\n";
    }
    else if (root->token.type == Token::TOK_TILDA)
    {
        expr += assemble_expr(*it, scope);
        expr += "    not     rax\n";
    }
    else if (root->token.type == Token::TOK_BANG)
    {
        expr += assemble_expr(*it, scope);
        expr += "    cmp     rax, 0\n";
        expr += "    sete    al\n";
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
        if ((root->children.front()->token.type == Token::TOK_NUM ||
             root->children.front()->token.type == Token::TOK_ID))
        {
            expr += "    mov     eax, " + assemble_expr(*it, scope) + "\n";
            std::advance(it, 1);
        }
        else
        {
            expr += assemble_expr(*it, scope);
            std::advance(it, 1);
        }

        if (root->children.back()->token.type == Token::TOK_NUM ||
            root->children.back()->token.type == Token::TOK_ID)
        {
            expr += "    add     eax, " + assemble_expr(*it, scope) + "\n";
        }
        else
        {
            expr += "    push    rax\n";
            expr += assemble_expr(*it, scope);
            expr += "    mov     ecx, eax\n";
            expr += "    pop     rax\n";
            expr += "    add     eax, ecx\n";
        }
    }
    else if (root->token.type == Token::TOK_MINUS)
    {
        if ((root->children.front()->token.type == Token::TOK_NUM ||
             root->children.front()->token.type == Token::TOK_ID))
        {
            expr += "    mov     eax, " + assemble_expr(*it, scope) + "\n";
            std::advance(it, 1);
        }
        else
        {
            expr += assemble_expr(*it, scope);
            std::advance(it, 1);
        }

        if (root->children.back()->token.type == Token::TOK_NUM ||
            root->children.back()->token.type == Token::TOK_ID)
        {
            expr += "    sub     eax, " + assemble_expr(*it, scope) + "\n";
        }
        else
        {
            expr += "    push    rax\n";
            expr += assemble_expr(*it, scope);
            expr += "    mov     ecx, eax\n";
            expr += "    pop     rax\n";
            expr += "    sub     eax, ecx\n";
        }
    }
    else if (root->token.type == Token::TOK_STAR)
    {
        if ((root->children.front()->token.type == Token::TOK_NUM ||
             root->children.front()->token.type == Token::TOK_ID))
        {
            expr += "    mov     eax, " + assemble_expr(*it, scope) + "\n";
            std::advance(it, 1);
        }
        else
        {
            expr += assemble_expr(*it, scope);
            std::advance(it, 1);
        }

        if (root->children.back()->token.type == Token::TOK_NUM ||
            root->children.back()->token.type == Token::TOK_ID)
        {
            expr += "    mul     " + assemble_expr(*it, scope) + "\n";
        }
        else
        {
            expr += "    push    rax\n";
            expr += assemble_expr(*it, scope);
            expr += "    mov     ecx, eax\n";
            expr += "    pop     rax\n";
            expr += "    mul     ecx\n";
        }
    }
    else if (root->token.type == Token::TOK_SLASH)
    {
        if ((root->children.front()->token.type == Token::TOK_NUM ||
             root->children.front()->token.type == Token::TOK_ID))
        {
            expr += "    mov     eax, " + assemble_expr(*it, scope) + "\n";
            std::advance(it, 1);
        }
        else
        {
            expr += assemble_expr(*it, scope);
            std::advance(it, 1);
        }

        if (root->children.back()->token.type == Token::TOK_NUM ||
            root->children.back()->token.type == Token::TOK_ID)
        {
            expr += "    xor     rdx, rdx\n";
            expr += "    mov     ecx, " + assemble_expr(*it, scope) + "\n";
            expr += "    div     ecx\n";
        }
        else
        {
            expr += "    push    rax\n";
            expr += assemble_expr(*it, scope);
            expr += "    mov     ecx, eax\n";
            expr += "    pop     rax\n";
            expr += "    xor     rdx, rdx\n";
            expr += "    div     ecx\n";
        }
    }
    else if (root->token.type == Token::TOK_PERCENT)
    {
        if ((root->children.front()->token.type == Token::TOK_NUM ||
             root->children.front()->token.type == Token::TOK_ID))
        {
            expr += "    mov     eax, " + assemble_expr(*it, scope) + "\n";
            std::advance(it, 1);
        }
        else
        {
            expr += assemble_expr(*it, scope);
            std::advance(it, 1);
        }

        if (root->children.back()->token.type == Token::TOK_NUM ||
            root->children.back()->token.type == Token::TOK_ID)
        {
            expr += "    xor     rdx, rdx\n";
            expr += "    mov     ecx, " + assemble_expr(*it, scope) + "\n";
            expr += "    div     ecx\n";
            expr += "    mov     eax, edx\n";
        }
        else
        {
            expr += "    push    rax\n";
            expr += assemble_expr(*it, scope);
            expr += "    mov     ecx, eax\n";
            expr += "    pop     rax\n";
            expr += "    xor     rdx, rdx\n";
            expr += "    div     ecx\n";
            expr += "    mov     eax, edx\n";
        }
    }
    else if (root->token.type == Token::TOK_SHL)
    {
        if ((root->children.front()->token.type == Token::TOK_NUM ||
             root->children.front()->token.type == Token::TOK_ID))
        {
            expr += "    mov     eax, " + assemble_expr(*it, scope) + "\n";
            std::advance(it, 1);
        }
        else
        {
            expr += assemble_expr(*it, scope);
            std::advance(it, 1);
        }

        if (root->children.back()->token.type == Token::TOK_NUM ||
            root->children.back()->token.type == Token::TOK_ID)
        {
            expr += "    shl     " + assemble_expr(*it, scope) + "\n";
        }
        else
        {
            expr += "    push    rax\n";
            expr += assemble_expr(*it, scope);
            expr += "    mov     ecx, eax\n";
            expr += "    pop     rax\n";
            expr += "    shl     eax, cl\n";
        }
    }
    else if (root->token.type == Token::TOK_SHR)
    {
        if ((root->children.front()->token.type == Token::TOK_NUM ||
             root->children.front()->token.type == Token::TOK_ID))
        {
            expr += "    mov     eax, " + assemble_expr(*it, scope) + "\n";
            std::advance(it, 1);
        }
        else
        {
            expr += assemble_expr(*it, scope);
            std::advance(it, 1);
        }

        if (root->children.back()->token.type == Token::TOK_NUM ||
            root->children.back()->token.type == Token::TOK_ID)
        {
            expr += "    shr     " + assemble_expr(*it, scope) + "\n";
        }
        else
        {
            expr += "    push    rax\n";
            expr += assemble_expr(*it, scope);
            expr += "    mov     ecx, eax\n";
            expr += "    pop     rax\n";
            expr += "    shr     eax, cl\n";
        }
    }

    // boolean
    else if (root->token.type == Token::TOK_EQEQ ||
             root->token.type == Token::TOK_NEQ ||
             root->token.type == Token::TOK_LT ||
             root->token.type == Token::TOK_LTE ||
             root->token.type == Token::TOK_GT ||
             root->token.type == Token::TOK_GTE)
    {
        if ((root->children.front()->token.type == Token::TOK_NUM ||
             root->children.front()->token.type == Token::TOK_ID))
        {
            expr += "    mov     eax, " + assemble_expr(*it, scope) + "\n";
            std::advance(it, 1);
        }
        else
        {
            expr += assemble_expr(*it, scope);
            std::advance(it, 1);
        }

        if (root->children.back()->token.type == Token::TOK_NUM ||
            root->children.back()->token.type == Token::TOK_ID)
        {
            expr += "    cmp     eax, " + assemble_expr(*it, scope) + "\n";
        }
        else
        {
            expr += "    push    rax\n";
            expr += assemble_expr(*it, scope);
            expr += "    mov     ecx, eax\n";
            expr += "    pop     rax\n";
            expr += "    cmp     eax, ecx\n";
        }
        expr += "    mov     rax, 0\n";

        if (root->token.type == Token::TOK_EQEQ)
        {
            expr += "    sete    al\n";
        }
        else if (root->token.type == Token::TOK_NEQ)
        {
            expr += "    setne   al\n";
        }
        else if (root->token.type == Token::TOK_LT)
        {
            expr += "    setl    al\n";
        }
        else if (root->token.type == Token::TOK_LTE)
        {
            expr += "    setle   al\n";
        }
        else if (root->token.type == Token::TOK_GT)
        {
            expr += "    setg    al\n";
        }
        else if (root->token.type == Token::TOK_GTE)
        {
            expr += "    setge   al\n";
        }
    }

    // logical operations
    else if (root->token.type == Token::TOK_LAND)
    {
        expr += assemble_expr(*it, scope);
        std::advance(it, 1);
        expr += "    cmp     al, 0\n";
        expr += "    je      .j_" + std::to_string(jmp) + "\n";

        expr += assemble_expr(*it, scope);
        std::advance(it, 1);
        expr += "    cmp     al, 0\n";
        expr += "    mov     rax, 0\n";
        expr += "    setne   al\n";
        expr += ".j_" + std::to_string(jmp++) + ":\n";
    }
    else if (root->token.type == Token::TOK_LOR)
    {
        expr += assemble_expr(*it, scope);
        std::advance(it, 1);
        expr += "    cmp     al, 0\n";
        expr += "    jne     .j_" + std::to_string(jmp) + "\n";

        expr += assemble_expr(*it, scope);
        std::advance(it, 1);
        expr += "    cmp     al, 0\n";
        expr += "    mov     rax, 0\n";
        expr += "    setne   al\n";
        expr += ".j_" + std::to_string(jmp++) + ":\n";
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
        if (root->children.front()->token.type == Token::TOK_NUM)
        {
            expr += "    mov     rdi, " + assemble_expr(root->children.front(), scope) + "\n";
        }
        else if (root->children.front()->token.type == Token::TOK_ID)
        {
            expr += "    mov     edi, " + assemble_expr(root->children.front(), scope) + "\n";
        }
        else
        {
            expr += assemble_expr(root->children.front(), scope);
            expr += "    mov     rdi, rax\n";
        }
        expr += "    call    _dump\n";
    }
    else if (root->token.type == Token::TOK_RETURN)
    {
        expr += "    mov     rax, " + assemble_expr(root->children.front(), scope) + "\n";
    }
    else if (root->token.type == Token::TOK_ID)
    {
        if (root->children.size() == 2)
        {
            // second child has the expression to parse
            auto it = root->children.begin();
            std::advance(it, 1);

            if ((*it)->token.type == Token::TOK_NUM)
            {
                expr += "    mov     DWORD [rbp-" + std::to_string(scope.at(root->token.data)->offset) + "], ";
                expr += assemble_expr(*it, scope) + "\n";
            }
            else
            {
                expr += "    mov     eax, " + assemble_expr(*it, scope) + "\n";
                expr += "    mov     DWORD [rbp-" + std::to_string(scope.at(root->token.data)->offset) + "], eax\n";
            }
        }
        else if (root->children.size() == 1 && !TYPES.count(root->token.type))
        {
            auto it = root->children.begin();
            if ((*it)->token.type == Token::TOK_NUM)
            {
                expr += "    mov     DWORD [rbp-" + std::to_string(scope.at(root->token.data)->offset) + "], ";
                expr += assemble_expr(*it, scope) + "\n";
            }
            else
            {
                if ((*it)->token.type == Token::TOK_ID)
                {
                    expr += "    mov     eax, " + assemble_expr(*it, scope) + "\n";
                }
                else
                {
                    expr += assemble_expr(*it, scope);
                }
                expr += "    mov     DWORD [rbp-" + std::to_string(scope.at(root->token.data)->offset) + "], eax\n";
            }
        }
        else if (root->children.size() == 0)
        {
            expr += "DWORD [rbp-" + std::to_string(scope.at(root->token.data)->offset) + "]"; 
        }
    }
    else if (root->token.type == Token::TOK_NUM)
    {
        // expr += "    mov     rax, " + root->token.data + "\n";
        expr += root->token.data;
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

std::string assemble_if(Node *root)
{
    std::string iff = "";

    // parse condition
    iff += assemble_expr(root->children.front(), root->scope);

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

    // init and cond given
    if (root->children.front()->children.size() == 3 ||
       (root->children.front()->children.size() == 2 &&
       (*list_it)->token.type == Token::TOK_ID))
    {
        loop += assemble_expr(*list_it, root->scope);
        std::advance(list_it, 1);
    }

    loop += ".L" + std::to_string(root->block_id) + "_s:\n";

    loop += assemble_expr(*list_it, root->scope);
    loop += "    cmp     rax, 0\n";
    loop += "    je      .L" + std::to_string(root->block_id) + "_e\n";
    std::advance(list_it, 1);
    
    // parse body of loop
    auto it = root->children.begin();
    std::advance(it, 1);
    while (it != root->children.end())
    {
        if ((*it)->token.type == Token::TOK_LOOP)
        {
            loop += assemble_loop(*it);
        }
        else
        {
            loop += assemble_expr(*it, root->scope);
        }
        std::advance(it, 1);
    }

    // cond and incr given
    if (root->children.front()->children.size() == 3 ||
       (root->children.front()->children.size() == 2 &&
        list_it != root->children.front()->children.end()))
    {
        loop += assemble_expr(*list_it, root->scope);
        std::advance(list_it, 1);
    }
    loop += "    jmp     .L" + std::to_string(root->block_id) + "_s\n";
    loop += ".L" + std::to_string(root->block_id) + "_e:\n";

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

    // write function name
    function += "global _" + root->token.data + "\n";
    function += "_" + root->token.data + ":\n";

    // function prologue
    function += "    push    rbp\n";
    function += "    mov     rbp, rsp\n";

    int stack_align = get_stack_alignment(root);
    function += "    sub     rsp, " + std::to_string(stack_align) + "\n";

    // push argc and argv on the stack if we are assembling main
    if (root->token.data == "main" &&
        root->children.front()->children.size() == 2)
    {
        function += "    mov     [rbp-" + std::to_string(root->children.front()->children.front()->offset) + "], edi\n";
        function += "    mov     [rbp-" + std::to_string(root->children.front()->children.back()->offset) + "], rsi\n";
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

    for (Node *child : root->children)
    {
        if (child->token.type == Token::TOK_LOOP)
        {
            function += assemble_loop(child);
        }
        else
        {
            function += assemble_expr(child, root->scope);
        }
    }
    
    // function epilogue
    function += "    mov     rsp, rbp\n";
    function += "    pop     rbp\n";
    function += "    ret\n";

    return function;
}

std::string assemble_program(Node *root)
{
    std::string program = "default rel\n\n";
    program += ";; dump function written in C, compiled to nasm with -O3\n"
        ";; dump takes a 64-bit unsigned integer from rdi and prints it to stdout\n"
        "_dump:\n"
        "    sub     rsp, 40\n"
        "    mov     esi, 19\n"
        "    mov     r8, -3689348814741910323\n"
        "    mov     BYTE [rsp+19], 10\n"
        ".L2:\n"
        "    mov     rax, rdi\n"
        "    sub     rsi, 1\n"
        "    mul     r8\n"
        "    mov     rax, rdi\n"
        "    shr     rdx, 3\n"
        "    lea     rcx, [rdx+rdx*4]\n"
        "    add     rcx, rcx\n"
        "    sub     rax, rcx\n"
        "    add     eax, 48\n"
        "    mov     BYTE [rsp+rsi], al\n"
        "    mov     rax, rdi\n"
        "    mov     rdi, rdx\n"
        "    cmp     rax, 9\n"
        "    ja      .L2\n"
        "    mov     edx, 20\n"
        "    mov     edi, 1\n"
        "    xor     eax, eax\n"
        "    sub     rdx, rsi\n"
        "    add     rsi, rsp\n"
        "    mov     rax, 0x2000004\n"
        "    syscall\n"
        "    add     rsp, 40\n"
        "    ret\n\n";

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
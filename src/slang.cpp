#include "include/slang.h"

int MEM_CAPACITY = 0;

/*
 * Un-escape a string via this stackoverflow thread
 * https://stackoverflow.com/questions/5612182/convert-string-with-explicit-escape-sequence-into-relative-character
 */
std::string unescape(const std::string& s)
{
    std::string res;
    std::string::const_iterator it = s.begin();
    while (it != s.end())
    {
        char c = *it++;
        if (c == '\\' && it != s.end())
        {
            switch (*it++) {
            case '\\': c = '\\'; break;
            case 'n': c = '\n'; break;
            case 't': c = '\t'; break;
            // all other escapes
            default: 
            // invalid escape sequence - skip it. alternatively you can copy it as is, throw an exception...
            continue;
            }
        }
        res += c;
    }

    return res;
}

std::vector<Op> parse_blocks(std::vector<Token> tokens)
{
    std::vector<Token> tokens_r = tokens;
    std::reverse(tokens_r.begin(), tokens_r.end());

    std::stack<int> stack;
    std::vector<Op> program;
    std::unordered_map<std::string, Macro> macros;
    std::unordered_map<std::string, unsigned long long> memory;
    // int curr_func = 0;

    int i = 0;
    while (tokens_r.size() > 0)
    {
        Token token = tokens_r.back();
        tokens_r.pop_back();

        Op op;

        if (token.type == TOKEN_INT)
        {
            op = { .type = OP_PUSH_INT, .value = token.int_val, .loc = token.loc };
        }

        else if (token.type == TOKEN_CHAR)
        {
            op = { .type = OP_PUSH_INT, .value = token.int_val, .loc = token.loc };
        }

        else if (token.type == TOKEN_STR)
        {
            op = { .type = OP_PUSH_STR, .data = unescape(token.str_val), .loc = token.loc };
        }

        else if (token.type == TOKEN_WORD)
        {
            if (BUILT_IN_WORDS.count(token.str_val))
            {
                op = { .type = BUILT_IN_WORDS[token.str_val], .loc = token.loc };
            }

            else if (macros.count(token.str_val))
            {
                std::vector<Token> macro_r = macros[token.str_val].body;
                std::reverse(macro_r.begin(), macro_r.end());

                tokens_r.insert(tokens_r.end(), macro_r.begin(), macro_r.end());
                continue;
            }

            else if (memory.count(token.str_val))
            {
                op = { .type = OP_PUSH_MEM, .value = memory[token.str_val], .loc = token.loc };
            }

            else
            {
                print_error(token.loc, "invalid command");
            }
        }

        // std::cout << token.str_val << "\n";

        if (op.type == OP_IF)
        {
            program.push_back(op);
            stack.push(i);

            i++;
        }

        else if (op.type == OP_ELIF)
        {
            program.push_back(op);
            int do_i = stack.top();
            stack.pop();

            int do_before = program[do_i].value;
            if (program[do_before].type == OP_IF)
            {
                program[do_i].value = i + 1;
                stack.push(i);
            }
            else if (program[do_before].type == OP_ELIF)
            {
                program[do_before].value = i;
                program[do_i].value = i + 1;
                stack.push(i);
            }

            i++;
        }
            
        else if (op.type == OP_ELSE)
        {
            program.push_back(op);
            int do_i = stack.top();
            stack.pop();

            int do_before = program[do_i].value;
            if (program[do_before].type == OP_IF)
            {
                program[do_i].value = i + 1;
                stack.push(i);
            }
            else if (program[do_before].type == OP_ELIF)
            {
                program[do_before].value = i;
                program[do_i].value = i + 1;
                stack.push(i);
            }

            i++;
        }

        else if (op.type == OP_END)
        {
            program.push_back(op);
            int block_i = stack.top();
            stack.pop();

            if (program[block_i].type == OP_ELSE)
            {
                program[block_i].value = i;
                program[i].value = i + 1;
            }

            else if (program[block_i].type == OP_DO)
            {
                int do_before = program[block_i].value;

                if (program[do_before].type == OP_WHILE)
                {
                    program[i].value = do_before;
                    program[block_i].value = i + 1;
                }

                else if (program[do_before].type == OP_IF)
                {
                    program[i].value = i + 1;
                    program[block_i].value = i + 1;
                }

                else if (program[do_before].type == OP_ELIF)
                {
                    program[do_before].value = i;
                    program[i].value = i + 1;
                    program[block_i].value = i + 1;
                }
            }

            else
            {
                print_error(program[block_i].loc, "ERROR: `end` keyword can only be used in conjunction with `if-else`, `do`, or `while`", true);
            }

            i++;
        }

        else if (op.type == OP_WHILE)
        {
            program.push_back(op);
            stack.push(i);

            i++;
        }

        else if (op.type == OP_DO)
        {
            program.push_back(op);
            int while_i = stack.top();
            stack.pop();
            program[i].value = while_i;
            stack.push(i);

            i++;
        }

        else if (op.type == OP_MEM)
        {
            if (tokens_r.size() == 0)
            {
                print_error(op.loc, "ERROR: expected identifier after `mem`", true);
            }

            else
            {
                token = tokens_r.back();
                tokens_r.pop_back();

                if (token.type != TOKEN_WORD)
                {
                    print_error(token.loc, "ERROR: expected word after `mem`", true);
                }

                else
                {
                    if (memory.count(token.str_val))
                    {
                        print_error(token.loc, "ERROR: redefinition of existing memory `" + token.str_val + "`", true);
                    }

                    if (BUILT_IN_WORDS.count(token.str_val))
                    {
                        print_error(token.loc, "ERROR: cannot define memory for built-in operation `" + token.str_val + "`", true);
                    }

                    std::string name = token.str_val;
                    std::stack<int> mem_size;

                    while (tokens_r.size())
                    {
                        token = tokens_r.back();
                        tokens_r.pop_back();

                        if (token.type == TOKEN_WORD && token.str_val == "end")
                        {
                            break;
                        }

                        else if (token.type == TOKEN_INT)
                        {
                            mem_size.push(token.int_val);
                        }

                        else if (token.type == TOKEN_WORD)
                        {
                            if (token.str_val == "+")
                            {
                                int a = mem_size.top();
                                mem_size.pop();

                                int b = mem_size.top();
                                mem_size.pop();

                                mem_size.push(a + b);
                            }

                            else if (token.str_val == "*")
                            {
                                int a = mem_size.top();
                                mem_size.pop();

                                int b = mem_size.top();
                                mem_size.pop();

                                mem_size.push(a * b);
                            }

                            else if (macros.count(token.str_val))
                            {
                                std::vector<Token> macro_r = macros[token.str_val].body;
                                std::reverse(macro_r.begin(), macro_r.end());

                                tokens_r.insert(tokens_r.end(), macro_r.begin(), macro_r.end());
                            }
                        }
                    }

                    if (mem_size.size() != 1)
                    {
                        print_error(token.loc, "ERROR: cannot use multiple values for memory size declaration", true);
                    }

                    memory[name] = MEM_CAPACITY;
                    MEM_CAPACITY += mem_size.top();
                    mem_size.pop();
                }
            }
        }

        else if (op.type == OP_DEF)
        {
            if (tokens_r.size() == 0)
            {
                print_error(op.loc, "ERROR: expected identifier after `def`", true);
            }

            else
            {
                token = tokens_r.back();
                tokens_r.pop_back();

                if (token.type != TOKEN_WORD)
                {
                    print_error(token.loc, "ERROR: expected word after `def`", true);
                }

                else
                {
                    if (macros.count(token.str_val))
                    {
                        print_error(token.loc, "ERROR: redefinition of already existing macro `" + token.str_val + "`", true);
                    }

                    if (BUILT_IN_WORDS.count(token.str_val))
                    {
                        print_error(token.loc, "ERROR: cannot define macro for built-in operation `" + token.str_val + "`", true);
                    }
                    // Macro macro = { .loc = op.loc, .body = {} };
                    std::string name = token.str_val;
                    macros[name] = { .loc = op.loc, .body = {} };
                    int num_end = 0;

                    while (tokens_r.size())
                    {
                        token = tokens_r.back();
                        tokens_r.pop_back();
                        
                        if (token.type == TOKEN_WORD && token.str_val == "end" && num_end == 0)
                        {
                            break;
                        }

                        else
                        {
                            macros[name].body.push_back(token);
                            if (token.type == TOKEN_WORD)
                            {
                                if (token.str_val == "if" or token.str_val == "while" or token.str_val == "def")
                                {
                                    num_end++;
                                }

                                else if (token.str_val == "end")
                                {
                                    num_end--;
                                }
                            }
                        }
                    }

                    if (token.type != TOKEN_WORD or token.str_val != "end")
                    {
                        print_error(token.loc, "ERROR: expected `end` to close `def`, found `" + token.str_val + "` instead", true);
                    }
                }
            }
        }

        else if (op.type == OP_USE)
        {
            if (tokens_r.size() == 0)
            {
                print_error(op.loc, "ERROR: expected file path after `use`", true);
            }

            else
            {
                token = tokens_r.back();
                tokens_r.pop_back();

                if (token.type != TOKEN_STR)
                {
                    print_error(token.loc, "ERROR: expected string after `use`", true);
                }

                else
                {
                    std::vector<Token> file = lex_file(token.str_val);
                    std::reverse(file.begin(), file.end());

                    tokens_r.insert(tokens_r.end(), file.begin(), file.end());
                }
            }
        }

        else
        {
            program.push_back(op);
            i++;
        }
    }

    if (stack.size() > 0)
    {
        print_error(program[stack.top()].loc, "ERROR: unclosed block");
    }

    return program;
}

std::vector<Op> load_program(std::string input_file)
{
    std::vector<Op> program;
    std::vector<Token> lexed_file = lex_file(input_file);
    program = parse_blocks(lexed_file);

    return program;
}

void compile_program(std::vector<Op> program, std::string output_file)
{
    std::ofstream out;
    out.open(output_file);

    out << "default rel\n\n";
    out << "dump:\n";
    out << "    sub     rsp, 40\n";
    out << "    mov     BYTE [rsp+31], 10\n";
    out << "    lea     rdx, [rsp+30]\n\n";

    out << ".L2:\n";
    out << "    movsx   rax, edi\n";
    out << "    mov     ecx, edi\n";
    out << "    lea     r8, [rsp+32]\n";
    out << "    imul    rax, rax, 1717986919\n";
    out << "    sar     ecx, 31\n";
    out << "    sub     r8, rdx\n";
    out << "    sar     rax, 34\n";
    out << "    sub     eax, ecx\n";
    out << "    lea     ecx, [rax+rax*4]\n";
    out << "    add     ecx, ecx\n";
    out << "    sub     edi, ecx\n";
    out << "    add     edi, 48\n";
    out << "    mov     BYTE [rdx], dil\n";
    out << "    mov     edi, eax\n";
    out << "    mov     rax, rdx\n";
    out << "    sub     rdx, 1\n";
    out << "    test    edi, edi\n";
    out << "    jne     .L2\n";
    out << "    lea     rsi, [rsp+32]\n";
    out << "    mov     rdx, r8\n";
    out << "    mov     edi, 1\n";
    out << "    sub     rax, rsi\n";
    out << "    lea     rsi, [rsp+32+rax]\n";
    out << "    mov     rax, 0x2000004\n";
    out << "    syscall\n";
    out << "    add     rsp, 40\n";
    out << "    ret\n\n";

    out << "global _main\n";
    out << "section .text\n";
    out << "_main:\n";

    std::vector<std::string> strs;

    uint64_t i = 0;
    for (i = 0; i < (uint64_t)(program.size()); i++)
    {
        Op op = program[i];
        out << "\naddr_" << i << ":\n";

        switch (op.type)
        {
            case OP_PUSH_INT:
            {
                out << "    ;; -- push int --\n";
                out << "    mov rax, " << op.value << "\n";
                out << "    push rax\n";
                break;
            }

            case OP_PUSH_STR:
            {
                out << "    ;; -- push str --\n";
                out << "    mov rax, " << op.data.size() << "\n";
                out << "    push rax\n";
                out << "    lea rax, [str_" << strs.size() << "]\n";
                out << "    push rax\n";
                strs.push_back(op.data);

                break;
            }

            case OP_PUSH_MEM:
            {
                out << "    ;; -- push mem --\n";
                out << "    xor rdi, rdi\n";
                out << "    lea rdi, [mem]\n";
                out << "    add rdi, " << op.value << "\n";
                out << "    push rdi\n";
                break;
            }

            case OP_DUMP:
            {
                out << "    ;; -- dump --\n";
                out << "    pop rdi\n";
                out << "    call dump\n";
                break;
            }

            case OP_DUP:
            {
                out << "    ;; -- dup --\n";
                out << "    pop rax\n";
                out << "    push rax\n";
                out << "    push rax\n";
                break;
            }

            case OP_DROP:
            {
                out << "    ;; -- drop --\n";
                out << "    pop rax\n";
                break;
            }

            case OP_SWAP:
            {
                out << "    ;; -- swap --\n";
                out << "    pop rax\n";
                out << "    pop rbx\n";
                out << "    push rax\n";
                out << "    push rbx\n";
                break;
            }

            case OP_OVER:
            {
                out << "    ;; -- over --\n";
                out << "    pop rax\n";
                out << "    pop rbx\n";
                out << "    push rbx\n";
                out << "    push rax\n";
                out << "    push rbx\n";
                break;
            }

            case OP_PLUS:
            {
                out << "    ;; -- plus --\n";
                out << "    pop rax\n";
                out << "    pop rbx\n";
                out << "    add rax, rbx\n";
                out << "    push rax\n";
                break;
            }

            case OP_MINUS:
            {
                out << "    ;; -- minus --\n";
                out << "    pop rbx\n";
                out << "    pop rax\n";
                out << "    sub rax, rbx\n";
                out << "    push rax\n";
                break;
            }

            case OP_MUL:
            {
                out << "    ;; -- mul --\n";
                out << "    pop rax\n";
                out << "    pop rbx\n";
                out << "    mul rbx\n";
                out << "    push rax\n";
                break;
            }

            case OP_DIVMOD:
            {
                out << "    ;; -- divmod --\n";
                out << "    xor rdx, rdx\n";
                out << "    pop rbx\n";
                out << "    pop rax\n";
                out << "    div rbx\n";
                out << "    push rax\n";
                out << "    push rdx\n";
                break;
            }

            case OP_SHL:
            {
                out << "    ;; -- shl (<<) --\n";
                out << "    pop rcx\n";
                out << "    pop rbx\n";
                out << "    shl rbx, cl\n";
                out << "    push rbx\n";
                break;
            }

            case OP_SHR:
            {
                out << "    ;; -- shr (>>) --\n";
                out << "    pop rcx\n";
                out << "    pop rbx\n";
                out << "    shr rbx, cl\n";
                out << "    push rbx\n";
                break;
            }

            case OP_NOT:
            {
                out << "    ;; -- not --\n";
                out << "    pop rax\n";
                out << "    not rax\n";
                out << "    push rax\n";
                break;
            }

            case OP_OR:
            {
                out << "    ;; -- or --\n";
                out << "    pop rcx\n";
                out << "    pop rbx\n";
                out << "    or rbx, rcx\n";
                out << "    push rbx\n";
                break;
            }

            case OP_XOR:
            {
                out << "    ;; -- xor --\n";
                out << "    pop rcx\n";
                out << "    pop rbx\n";
                out << "    xor rbx, rcx\n";
                out << "    push rbx\n";
                break;
            }

            case OP_AND:
            {
                out << "    ;; -- and --\n";
                out << "    pop rcx\n";
                out << "    pop rbx\n";
                out << "    and rbx, rcx\n";
                out << "    push rbx\n";
                break;
            }

            case OP_EQ:
            {
                out << "    ;; -- equal --\n";
                out << "    pop rax\n";
                out << "    pop rbx\n";
                out << "    mov rcx, 1\n";
                out << "    mov rdx, 0\n";
                out << "    cmp rax, rbx\n";
                out << "    mov rax, rdx\n";
                out << "    cmove rax, rcx\n";
                out << "    push rax\n";
                break;
            }

            case OP_NEQ:
            {
                out << "    ;; -- equal --\n";
                out << "    pop rax\n";
                out << "    pop rbx\n";
                out << "    mov rcx, 0\n";
                out << "    mov rdx, 1\n";
                out << "    cmp rax, rbx\n";
                out << "    mov rax, rdx\n";
                out << "    cmove rax, rcx\n";
                out << "    push rax\n";
                break;
            }

            case OP_GT:
            {
                out << "    ;; -- greater than --\n";
                out << "    pop rbx\n";
                out << "    pop rax\n";
                out << "    mov rcx, 1\n";
                out << "    mov rdx, 0\n";
                out << "    cmp rax, rbx\n";
                out << "    mov rax, rdx\n";
                out << "    cmovg rax, rcx\n";
                out << "    push rax\n";
                break;
            }

            case OP_LT:
            {
                out << "    ;; -- less than --\n";
                out << "    pop rbx\n";
                out << "    pop rax\n";
                out << "    mov rcx, 1\n";
                out << "    mov rdx, 0\n";
                out << "    cmp rax, rbx\n";
                out << "    mov rax, rdx\n";
                out << "    cmovl rax, rcx\n";
                out << "    push rax\n";
                break;
            }

            case OP_GTE:
            {
                out << "    ;; -- greater than or equal --\n";
                out << "    pop rbx\n";
                out << "    pop rax\n";
                out << "    mov rcx, 1\n";
                out << "    mov rdx, 0\n";
                out << "    cmp rax, rbx\n";
                out << "    mov rax, rdx\n";
                out << "    cmovge rax, rcx\n";
                out << "    push rax\n";
                break;
            }

            case OP_LTE:
            {
                out << "    ;; -- less than or equal --\n";
                out << "    pop rbx\n";
                out << "    pop rax\n";
                out << "    mov rcx, 1\n";
                out << "    mov rdx, 0\n";
                out << "    cmp rax, rbx\n";
                out << "    mov rax, rdx\n";
                out << "    cmovle rax, rcx\n";
                out << "    push rax\n";
                break;
            }

            case OP_IF:
            {
                out << "    ;; -- if --\n";
                // out << "    pop rax\n";
                // out << "    test rax, rax\n";
                // out << "    jz addr_" << op.value << "\n";
                break;
            }

            case OP_ELIF:
            {
                out << "    ;; -- elif --\n";
                out << "    jmp addr_" << op.value << "\n";
                break;
            }

            case OP_ELSE:
            {
                out << "    ;; -- else --\n";
                out << "    jmp addr_" << op.value << "\n";
                break;
            }

            case OP_END:
            {
                out << "    ;; -- end --\n";

                if (i + 1 != op.value)
                {
                    out << "    jmp addr_" << op.value << "\n";
                }
                break;
            }

            case OP_WHILE:
            {
                out << "    ;; -- while --\n";
                break;
            }

            case OP_DO:
            {
                out << "    ;; -- do --\n";
                out << "    pop rax\n";
                out << "    test rax, rax\n";
                out << "    jz addr_" << op.value << "\n";
                break;
            }

            case OP_LOAD_8:
            {
                out << "    ;; -- load8 --\n";
                out << "    pop rax\n";
                out << "    xor rbx, rbx\n";
                out << "    mov bl, [rax]\n";
                out << "    push rbx\n";
                break;
            }

            case OP_STORE_8:
            {
                out << "    ;; -- store8 --\n";
                out << "    pop rbx\n";
                out << "    pop rax\n";
                out << "    mov [rax], bl\n";
                break;
            }

            case OP_LOAD_16:
            {
                out << "    ;; -- load16 --\n";
                out << "    pop rax\n";
                out << "    xor rbx, rbx\n";
                out << "    mov bx, [rax]\n";
                out << "    push rbx\n";
                break;
            }

            case OP_STORE_16:
            {
                out << "    ;; -- store16 --\n";
                out << "    pop rbx\n";
                out << "    pop rax\n";
                out << "    mov [rax], bx\n";
                break;
            }

            case OP_LOAD_32:
            {
                out << "    ;; -- load32 --\n";
                out << "    pop rax\n";
                out << "    xor rbx, rbx\n";
                out << "    mov ebx, [rax]\n";
                out << "    push rbx\n";
                break;
            }

            case OP_STORE_32:
            {
                out << "    ;; -- store32 --\n";
                out << "    pop rbx\n";
                out << "    pop rax\n";
                out << "    mov [rax], ebx\n";
                break;
            }

            case OP_LOAD_64:
            {
                out << "    ;; -- load64 --\n";
                out << "    pop rax\n";
                out << "    xor rbx, rbx\n";
                out << "    mov rbx, [rax]\n";
                out << "    push rbx\n";
                break;
            }

            case OP_STORE_64:
            {
                out << "    ;; -- store64 --\n";
                out << "    pop rbx\n";
                out << "    pop rax\n";
                out << "    mov [rax], rbx\n";
                break;
            }
            
            // order to pass parameters for syscall:
            // rax (syscall number), then rdi, rsi, rdx, rcx, r8, r9
            case OP_SYS_0:
            {
                out << "    ;; -- sys0 --\n";
                out << "    pop rax\n";
                out << "    syscall\n";
                out << "    push rax\n";
                break;
            }

            case OP_SYS_1:
            {
                out << "    ;; -- sys1 --\n";
                out << "    pop rax\n";
                out << "    pop rdi\n";
                out << "    syscall\n";
                out << "    push rax\n";
                break;
            }

            case OP_SYS_2:
            {
                out << "    ;; -- sys2 --\n";
                out << "    pop rax\n";
                out << "    pop rdi\n";
                out << "    pop rsi\n";
                out << "    syscall\n";
                out << "    push rax\n";
                break;
            }

            case OP_SYS_3:
            {
                out << "    ;; -- sys3 --\n";
                out << "    pop rax\n";
                out << "    pop rdi\n";
                out << "    pop rsi\n";
                out << "    pop rdx\n";
                out << "    syscall\n";
                out << "    push rax\n";
                break;
            }

            case OP_SYS_4:
            {
                out << "    ;; -- sys4 --\n";
                out << "    pop rax\n";
                out << "    pop rdi\n";
                out << "    pop rsi\n";
                out << "    pop rdx\n";
                out << "    pop rcx\n";
                out << "    syscall\n";
                out << "    push rax\n";
                break;
            }

            case OP_SYS_5:
            {
                out << "    ;; -- sys5 --\n";
                out << "    pop rax\n";
                out << "    pop rdi\n";
                out << "    pop rsi\n";
                out << "    pop rdx\n";
                out << "    pop rcx\n";
                out << "    pop r8\n";
                out << "    syscall\n";
                out << "    push rax\n";
                break;
            }

            case OP_SYS_6:
            {
                out << "    ;; -- sys6 --\n";
                out << "    pop rax\n";
                out << "    pop rdi\n";
                out << "    pop rsi\n";
                out << "    pop rdx\n";
                out << "    pop rcx\n";
                out << "    pop r8\n";
                out << "    pop r9\n";
                out << "    syscall\n";
                out << "    push rax\n";
                break;
            }

            default:
            {
                break;
            }
        }
    }
    
    out << "\naddr_" << i << ":\n";
    out << "    ;; -- exit --\n";
    out << "    mov rax, 0x2000001\n";
    out << "    mov rdi, 0\n";
    out << "    syscall\n";

    out << "\nsection .data\n";
    for (int i = 0; i < (int)(strs.size()); i++)
    {
        out << "str_" << i << ": db ";

        for (auto c : strs[i])
        {
            out << (int)(c) << ",";
        }
        out << (int)('\0') << "\n";
    }

    out << "\nsection .bss\n";
    out << "mem: resb " << MEM_CAPACITY << "\n";

    out.close();
}

void usage()
{
    std::cout << "USAGE: ./slang [flags] <executable name> <input file>\n\n";
    std::cout << "FLAGS:\n";
    std::cout << "  -d --debug\t\t Generate readable assembly at `<executable_name>.asm`\n";
}

#include "include/slang.h"

void print_error(std::string loc, std::string msg, bool flag)
{
    std::size_t last_colon = loc.find_last_of(':');
    if (flag)
    {
        std::cerr << loc.substr(0, last_colon) << ": " << msg << "\n";
    }

    else
    {
        std::cerr << loc.substr(0, last_colon) << ": " << msg << loc.substr(last_colon) << "\n";
    }

    exit(EXIT_FAILURE);
}

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

            else
            {
                print_error(token.loc, "invalid command");
            }
        }

        if (op.type == OP_IF)
        {
            program.push_back(op);
            stack.push(i);

            i++;
        }
            
        else if (op.type == OP_ELSE)
        {
            program.push_back(op);
            int if_i = stack.top();
            stack.pop();

            if (program[if_i].type != OP_IF)
            {
                std::cout << program[if_i].value << "\n";
                print_error(program[if_i].loc, "ERROR: `else` keyword can only be used in conjunction with `if`", true);
            }

            program[if_i].value = i + 1;
            stack.push(i);

            i++;
        }

        else if (op.type == OP_END)
        {
            program.push_back(op);
            int block_i = stack.top();
            stack.pop();

            if (program[block_i].type == OP_IF || program[block_i].type == OP_ELSE)
            {
                program[block_i].value = i;
                program[i].value = i + 1;
            }

            else if (program[block_i].type == OP_DO)
            {
                program[i].value = program[block_i].value;
                program[block_i].value = i + 1;
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

                    while (tokens_r.size())
                    {
                        token = tokens_r.back();
                        tokens_r.pop_back();

                        if (token.type == TOKEN_WORD && token.str_val == "end")
                        {
                            break;
                        }

                        else
                        {
                            macros[name].body.push_back(token);
                        }
                    }

                    if (token.type != TOKEN_WORD or token.str_val != "end")
                    {
                        print_error(token.loc, "ERROR: expected `end` to close `def`, found `" + token.str_val + "` instead", true);
                    }
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

void simulate_program(std::vector<Op> program)
{
    std::stack<int> stack;
    byte memory[STR_CAPACITY + MEM_CAPACITY];
    memset(memory, (byte)(0), STR_CAPACITY + MEM_CAPACITY);
    int str_size = 0;

    int i = 0;
    while (i < program.size())
    {
        Op op = program[i];

        switch (op.type)
        {
            case OP_PUSH_INT:
            {
                stack.push(op.value);
                i++;

                break;
            }

            case OP_PUSH_STR:
            {
                stack.push(op.data.size());

                if (op.addr != -1)
                {
                    stack.push(op.addr);
                }
                else
                {
                    op.addr = str_size;
                    memcpy(&memory[str_size], &op.data[0], op.data.size() * sizeof(char));
                    str_size += op.data.size();
                    stack.push(op.addr);
                }
                i++;

                break;
            }

            case OP_PLUS:
            {
                int a = stack.top();
                stack.pop();

                int b = stack.top();
                stack.pop();

                stack.push(a + b);
                i++;

                break;
            }

            case OP_MINUS:
            {
                int a = stack.top();
                stack.pop();

                int b = stack.top();
                stack.pop();

                stack.push(-a + b);
                i++;

                break;
            }

            case OP_EQ:
            {
                int a = stack.top();
                stack.pop();

                int b = stack.top();
                stack.pop();

                a == b ? stack.push(1) : stack.push(0);
                i++;

                break;
            }

            case OP_NEQ:
            {
                int a = stack.top();
                stack.pop();

                int b = stack.top();
                stack.pop();

                a != b ? stack.push(1) : stack.push(0);
                i++;

                break;
            }

            case OP_DUMP:
            {
                std::cout << stack.top() << "\n";
                stack.pop();
                i++;
                
                break;
            }

            case OP_IF:
            {
                int cond = stack.top();
                stack.pop();

                cond == 0 ? i = op.value : i++;

                break;
            }

            case OP_ELSE:
            {
                i = op.value;
                break;
            }

            case OP_END:
            {
                i = op.value;
                break;
            }

            case OP_DUP:
            {
                int a = stack.top();
                stack.push(a);
                i++;

                break;
            }

            case OP_DUP2:
            {
                int a = stack.top();
                stack.pop();
                int b = stack.top();
                stack.push(a);
                stack.push(b);
                stack.push(a);
                i++;

                break;
            }

            case OP_DROP:
            {
                stack.pop();
                i++;

                break;
            }

            case OP_SWAP:
            {
                int a = stack.top();
                stack.pop();
                int b = stack.top();
                stack.pop();

                stack.push(a);
                stack.push(b);

                i++;

                break;
            }

            case OP_OVER:
            {
                int a = stack.top();
                stack.pop();

                int b = stack.top();
                stack.pop();

                stack.push(b);
                stack.push(a);
                stack.push(b);

                i++;

                break;
            }

            case OP_GT:
            {
                int a = stack.top();
                stack.pop();

                int b = stack.top();
                stack.pop();

                b > a ? stack.push(1) : stack.push(0);
                i++;
                break;
            }

            case OP_LT:
            {
                int a = stack.top();
                stack.pop();

                int b = stack.top();
                stack.pop();

                a > b ? stack.push(1) : stack.push(0);
                i++;
                break;
            }

            case OP_GTE:
            {
                int a = stack.top();
                stack.pop();

                int b = stack.top();
                stack.pop();

                b >= a ? stack.push(1) : stack.push(0);
                i++;
                break;
            }

            case OP_LTE:
            {
                int a = stack.top();
                stack.pop();

                int b = stack.top();
                stack.pop();

                a >= b ? stack.push(1) : stack.push(0);
                i++;
                break;
            }

            case OP_INC:
            {
                int a = stack.top();
                stack.pop();
                a++;
                stack.push(a);

                i++;
                break;
            }

            case OP_DEC:
            {
                int a = stack.top();
                stack.pop();
                a--;
                stack.push(a);

                i++;
                break;
            }

            case OP_SHL:
            {
                int a = stack.top();
                stack.pop();
                int b = stack.top();
                stack.pop();

                stack.push(b << a);

                i++;
                break;
            }

            case OP_SHR:
            {
                int a = stack.top();
                stack.pop();
                int b = stack.top();
                stack.pop();

                stack.push(b >> a);

                i++;
                break;
            }

            case OP_LOR:
            {
                int a = stack.top();
                stack.pop();
                int b = stack.top();
                stack.pop();

                stack.push(a | b);

                i++;
                break;
            }

            case OP_XOR:
            {
                int a = stack.top();
                stack.pop();
                int b = stack.top();
                stack.pop();

                stack.push(a ^ b);

                i++;
                break;
            }

            case OP_LAND:
            {
                int a = stack.top();
                stack.pop();
                int b = stack.top();
                stack.pop();

                stack.push(a & b);

                i++;
                break;
            }

            case OP_WHILE:
            {
                i++;
                break;
            }

            case OP_DO:
            {
                int a = stack.top();
                stack.pop();

                a == 0 ? i = op.value : i++;
                break;
            }

            case OP_DEF:
            {
                break;
            }

            case OP_MEM:
            {
                stack.push(STR_CAPACITY);
                i++;
                break;
            }

            case OP_LOAD:
            {
                int addr = stack.top();
                stack.pop();
                stack.push(memory[addr]);
                i++;
                break;
            }

            case OP_STORE:
            {
                int value = stack.top();
                stack.pop();
                int addr = stack.top();
                stack.pop();

                memory[addr] = (value & ((1ULL << 8) - 1));
                i++;
                break;
            }

            case OP_SYS_WRITE:
            {
                int fd = stack.top();
                stack.pop();
                
                int addr = stack.top();
                stack.pop();

                int size = stack.top();
                stack.pop();

                for (int c = 0; c < size; c++)
                {
                    std::cout << memory[addr + c];
                }

                i++;
                break;
            }

            case OP_SYS_EXIT:
            {
                int exit_code = stack.top();
                stack.pop();
                exit(exit_code);
                break;
            }

            default:
            {
                std::cerr << "Unreachable\n";
                break;
            }
        }
    }
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

    int i = 0;
    for (i = 0; i < program.size(); i++)
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

            case OP_DUMP:
            {
                out << "    ;; -- dump --\n";
                out << "    pop rdi\n";
                out << "    call dump\n";
                break;
            }

            case OP_IF:
            {
                out << "    ;; -- if --\n";
                out << "    pop rax\n";
                out << "    test rax, rax\n";
                out << "    jz addr_" << op.value << "\n";
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
                    break;
                }
                break;
            }

            case OP_DEF:
            {
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

            case OP_DUP2:
            {
                out << "    ;; -- dup2 --\n";
                out << "    pop rbx\n";
                out << "    pop rax\n";
                out << "    push rax\n";
                out << "    push rbx\n";
                out << "    push rax\n";
                out << "    push rbx\n";
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

            case OP_INC:
            {
                out << "    ;; -- inc --\n";
                out << "    pop rax\n";
                out << "    add rax, 1\n";
                out << "    push rax\n";
                break;
            }

            case OP_DEC:
            {
                out << "    ;; -- dec --\n";
                out << "    pop rax\n";
                out << "    sub rax, 1\n";
                out << "    push rax\n";
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

            case OP_LOR:
            {
                out << "    ;; -- lor (|) --\n";
                out << "    pop rcx\n";
                out << "    pop rbx\n";
                out << "    or rbx, rcx\n";
                out << "    push rbx\n";
                break;
            }

            case OP_XOR:
            {
                out << "    ;; -- xor (^) --\n";
                out << "    pop rcx\n";
                out << "    pop rbx\n";
                out << "    xor rbx, rcx\n";
                out << "    push rbx\n";
                break;
            }

            case OP_LAND:
            {
                out << "    ;; -- and (&) --\n";
                out << "    pop rcx\n";
                out << "    pop rbx\n";
                out << "    and rbx, rcx\n";
                out << "    push rbx\n";
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

            case OP_MEM:
            {
                out << "    ;; -- mem --\n";
                out << "    xor rdi, rdi\n";
                out << "    lea rdi, [mem]\n";
                out << "    push rdi\n";
                break;
            }

            case OP_LOAD:
            {
                out << "    ;; -- load (,) --\n";
                out << "    pop rax\n";
                out << "    xor rbx, rbx\n";
                out << "    mov bl, [rax]\n";
                out << "    push rbx\n";
                break;
            }

            case OP_STORE:
            {
                out << "    ;; -- store (.) --\n";
                out << "    pop rbx\n";
                out << "    pop rax\n";
                out << "    mov [rax], bl\n";
                break;
            }

            case OP_SYS_WRITE:
            {
                out << "    ;; -- write --\n";

                // order to pass parameters for syscall:
                // rax (syscall number), then rdi, rsi, rdx, rcx, r8, r9
                out << "    mov rax, 0x2000004\n";
                out << "    pop rdi\n";
                out << "    pop rsi\n";
                out << "    pop rdx\n";
                out << "    syscall\n";
                break;
            }

            case OP_SYS_EXIT:
            {
                out << "    ;; -- exit --\n";
                out << "    mov rax, 0x2000001\n";
                out << "    pop rdi\n";
                out << "    syscall\n";
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
    for (int i = 0; i < strs.size(); i++)
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
    std::cout << "Usage: cell <SUBCOMMAND> [ARGS]\n";
    std::cout << "SUBCOMMANDS:\n";
    std::cout << "    sim <input files>                      Simulate the program\n";
    std::cout << "    com <executable name> <input files>    Compile the program\n";
}

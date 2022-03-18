#include "include/cell.h"

void print_error(std::string loc, std::string msg)
{
    std::size_t last_colon = loc.find_last_of(':');
    std::cerr << loc.substr(0, last_colon) << ": " << msg << loc.substr(last_colon) << "\n";
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

Code parse_op(Token token)
{
    switch (token.type)
    {
        case TOKEN_INT:
        {
            return { .type = OP_PUSH_INT, .value = token.int_val, .loc = token.loc };
        }

        case TOKEN_STR:
        {
            return { .type = OP_PUSH_STR, .data = unescape(token.str_val), .loc = token.loc };
        }

        case TOKEN_WORD:
        {
            if (BUILT_IN_WORDS.count(token.str_val))
            {
                return { .type = BUILT_IN_WORDS[token.str_val], .loc = token.loc };
            }

            else
            {
                print_error(token.loc, "invalid command");
                return {};
            }
        }

        default:
        {
            print_error(token.loc, "invalid command");
            return {};
        }
    }
}

std::vector<Code> parse_blocks(std::vector<Code> program)
{
    std::stack<int> stack;

    for (int i = 0; i < program.size(); i++)
    {
        Code op = program[i];

        switch (op.type)
        {
            case OP_IF:
            {
                stack.push(i);
                break;
            }
            
            case OP_ELSE:
            {
                int if_i = stack.top();
                stack.pop();

                if (program[if_i].type != OP_IF)
                {
                    print_error(program[if_i].loc, "ERROR: `else` keyword can only be used in conjunction with `if`");
                }

                program[if_i] = { OP_IF, i + 1 };
                stack.push(i);

                break;
            }

            case OP_END:
            {
                int block_i = stack.top();
                stack.pop();

                if (program[block_i].type == OP_IF || program[block_i].type == OP_ELSE)
                {
                    program[block_i] = { program[block_i].type, i };
                    program[i] = { OP_END, i + 1 };
                }

                else if (program[block_i].type == OP_DO)
                {
                    program[i] = { OP_END, program[block_i].value};
                    program[block_i] = { OP_DO, i + 1 };
                }

                else
                {
                    print_error(program[block_i].loc, "ERROR: `end` keyword can only be used in conjunction with `if-else`, `do`, or `while`");
                }
                
                break;
            }

            case OP_WHILE:
            {
                stack.push(i);
                break;
            }

            case OP_DO:
            {
                int while_i = stack.top();
                stack.pop();
                program[i] = { OP_DO, while_i };
                stack.push(i);

                break;
            }

            default:
            {
                break;
            }
        }
    }

    if (stack.size() > 0)
    {
        print_error(program[stack.top()].loc, "ERROR: unclosed block");
    }

    return program;
}

std::vector<Code> load_program(std::string input_file)
{
    std::vector<Code> program;
    std::stringstream ss;

    std::string file = remove_comments(input_file);
    ss << file;
    std::string line;

    std::vector<Token> lexed_file = lex_file(input_file);

    for (auto token : lexed_file)
    {
        program.push_back(parse_op(token));
    }

    program = parse_blocks(program);

    ss.clear();

    return program;
}

void simulate_program(std::vector<Code> program)
{
    std::stack<int> stack;
    byte memory[STR_CAPACITY + MEM_CAPACITY];
    memset(memory, (byte)(0), STR_CAPACITY + MEM_CAPACITY);
    int str_size = 0;

    int i = 0;
    while (i < program.size())
    {
        Code op = program[i];

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

            case OP_PRINT:
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

void compile_program(std::vector<Code> program, std::string output_file)
{
    std::ofstream out;
    out.open(output_file);

    out << "default rel\n\n";
    out << "print:\n";
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
        Code op = program[i];
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

            case OP_PRINT:
            {
                out << "    ;; -- print --\n";
                out << "    pop rdi\n";
                out << "    call print\n";
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

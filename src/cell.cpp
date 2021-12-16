#include "include/cell.h"

code push(int x)
{
    return { OP_PUSH, x };
}

code plus()
{
    return { OP_PLUS, };
}

code minus()
{
    return { OP_MINUS, };
}

code equal()
{
    return { OP_EQUAL, };
}

code dump()
{
    return { OP_DUMP, };
}

code iff()
{
    return { OP_IF, };
}

code elze()
{
    return { OP_ELSE, };
}

code end()
{
    return { OP_END, };
}

code dup()
{
    return { OP_DUP, };
}

code gt()
{
    return { OP_GT, };
}

code lt()
{
    return { OP_LT, };
}

code gte()
{
    return { OP_GTE, };
}

code lte()
{
    return { OP_LTE, };
}

code inc()
{
    return { OP_INC, };
}

code dec()
{
    return { OP_DEC, };
}

bool string_is_int(std::string s)
{
    char *p;
    strtol(s.c_str(), &p, 10);
    return *p == 0;
}

code parse_op(std::string value, std::string msg)
{
    if (value == "+")
    {
        return plus();
    }

    if (value == "-")
    {
        return minus();
    }

    if (value == "=")
    {
        return equal();
    }

    if (value == ".")
    {
        return dump();
    }

    if (value == "if")
    {
        return iff();
    }

    if (value == "else")
    {
        return elze();
    }

    if (value == "end")
    {
        return end();
    }

    if (value == "dup")
    {
        return dup();
    }

    if (value == ">")
    {
        return gt();
    }

    if (value == "<")
    {
        return lt();
    }

    if (value == ">=")
    {
        return gte();
    }

    if (value == "<=")
    {
        return lte();
    }

    if (value == "inc")
    {
        return inc();
    }

    if (value == "dec")
    {
        return dec();
    }

    try 
    {
        return push(std::stoi(value));
    }
    
    catch (const std::exception& e)
    {
        std::size_t last_colon = msg.find_last_of(':');
        std::cerr << msg.substr(0, last_colon) << ": invalid command" << msg.substr(last_colon) << "\n";
        exit(EXIT_FAILURE);
    }
}

std::vector<code> parse_blocks(std::vector<code> program)
{
    std::stack<int> stack;

    for (int i = 0; i < program.size(); i++)
    {
        code elem = program[i];
        if (elem.op_type == OP_IF)
        {
            stack.push(i);
        }

        else if (elem.op_type == OP_ELSE)
        {
            int if_i = stack.top();
            stack.pop();

            program[if_i] = { OP_IF, i + 1 };
            stack.push(i);
        }

        else if (elem.op_type == OP_END)
        {
            int block_i = stack.top();
            stack.pop();

            if (program[block_i].op_type == OP_IF || program[block_i].op_type == OP_ELSE)
            {
                program[block_i] = { program[block_i].op_type, i };
            }
        }
    }

    return program;
}

std::vector<code> load_program(std::string input_file)
{
    std::vector<code> program;
    std::ifstream in;
    try
    {
        in.open(input_file);
        std::string line;

        std::vector<std::string> lexed_file = lex_file(input_file);
        int curr = 0;
        while (getline(in, line))
        {
            std::vector<std::string> tokens;
            int col = strip_col(line, 0);

            while (col < line.size())
            {
                int col_end = line.find(' ', col);

                if (col_end < 0)
                {
                    col_end = line.size();
                }

                tokens.push_back(line.substr(col, col_end - col));
                col = strip_col(line, col_end);
            }

            for (auto token : tokens)
            {
                program.push_back(parse_op(token, lexed_file[curr]));
                curr++;
            }
        }

        program = parse_blocks(program);
    }

    catch(const std::exception& e)
    {
        usage();
        std::cerr << "ERROR: could not find file `" << input_file << "`\n";
        exit(EXIT_FAILURE);
    }

    return program;
}

void simulate_program(std::vector<code> program)
{
    std::stack<int> stack;
    int i = 0;
    while (i < program.size())
    {
        code elem = program[i];
        if (elem.op_type == OP_PUSH)
        {
            stack.push(elem.value);
            i++;
        }

        else if (elem.op_type == OP_PLUS)
        {
            int a = stack.top();
            stack.pop();

            int b = stack.top();
            stack.pop();

            stack.push(a + b);
            i++;
        }

        else if (elem.op_type == OP_MINUS)
        {
            int a = stack.top();
            stack.pop();

            int b = stack.top();
            stack.pop();

            stack.push(-a + b);
            i++;
        }

        else if (elem.op_type == OP_EQUAL)
        {
            int a = stack.top();
            stack.pop();

            int b = stack.top();
            stack.pop();

            a == b ? stack.push(1) : stack.push(0);
            i++;
        }

        else if (elem.op_type == OP_DUMP)
        {
            std::cout << stack.top() << "\n";
            stack.pop();
            i++;
        }

        else if (elem.op_type == OP_IF)
        {
            int cond = stack.top();
            stack.pop();

            cond == 0 ? i = elem.value : i++;
        }

        else if (elem.op_type == OP_ELSE)
        {
            i = elem.value;
        }

        else if (elem.op_type == OP_END)
        {
            i++;
        }

        else if (elem.op_type == OP_DUP)
        {
            int a = stack.top();
            stack.push(a);
            i++;
        }

        else if (elem.op_type == OP_GT)
        {
            int a = stack.top();
            stack.pop();

            int b = stack.top();
            stack.pop();

            b > a ? stack.push(1) : stack.push(0);
            i++;
        }

        else if (elem.op_type == OP_LT)
        {
            int a = stack.top();
            stack.pop();

            int b = stack.top();
            stack.pop();

            a > b ? stack.push(1) : stack.push(0);
            i++;
        }

        else if (elem.op_type == OP_GTE)
        {
            int a = stack.top();
            stack.pop();

            int b = stack.top();
            stack.pop();

            b >= a ? stack.push(1) : stack.push(0);
            i++;
        }

        else if (elem.op_type == OP_LTE)
        {
            int a = stack.top();
            stack.pop();

            int b = stack.top();
            stack.pop();

            a >= b ? stack.push(1) : stack.push(0);
            i++;
        }

        else if (elem.op_type == OP_INC)
        {
            int a = stack.top();
            stack.pop();
            a++;
            stack.push(a);

            i++;
        }

        else if (elem.op_type == OP_DEC)
        {
            int a = stack.top();
            stack.pop();
            a--;
            stack.push(a);

            i++;
        }

        else
        {
            std::cerr << "Unreachable\n";
        }
    }
}

void compile_program(std::vector<code> program, std::string output_file)
{
    std::ofstream out;
    out.open(output_file);

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

    for (int i = 0; i < program.size(); i++)
    {
        code elem = program[i];
        if (elem.op_type == OP_PUSH)
        {
            out << "    ;; -- push --\n";
            out << "    push " << elem.value << "\n";
        }

        else if (elem.op_type == OP_PLUS)
        {
            out << "    ;; -- plus --\n";
            out << "    pop rax\n";
            out << "    pop rbx\n";
            out << "    add rax, rbx\n";
            out << "    push rax\n";
        }

        else if (elem.op_type == OP_MINUS)
        {
            out << "    ;; -- minus --\n";
            out << "    pop rbx\n";
            out << "    pop rax\n";
            out << "    sub rax, rbx\n";
            out << "    push rax\n";
        }

        else if (elem.op_type == OP_EQUAL)
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
        }

        else if (elem.op_type == OP_DUMP)
        {
            out << "    ;; -- dump --\n";
            out << "    pop rdi\n";
            out << "    call dump\n";
        }

        else if (elem.op_type == OP_IF)
        {
            out << "    ;; -- if --\n";
            out << "    pop rax\n";
            out << "    test rax, rax\n";
            out << "    jz addr_" << elem.value << "\n";
        }

        else if (elem.op_type == OP_ELSE)
        {
            out << "    ;; -- else --\n";
            out << "    jmp addr_" << elem.value << "\n";
            out << "addr_" << i + 1 << ":\n";
        }

        else if (elem.op_type == OP_END)
        {
            out << "    ;; -- end --\n";
            out << "addr_" << i << ":\n";
        }

        else if (elem.op_type == OP_DUP)
        {
            out << "    ;; -- dup --\n";
            out << "    pop rax\n";
            out << "    push rax\n";
            out << "    push rax\n";
        }

        else if (elem.op_type == OP_GT)
        {
            out << "    ;; -- greater than --\n";
            out << "    pop rax\n";
            out << "    pop rbx\n";
            out << "    mov rcx, 1\n";
            out << "    mov rdx, 0\n";
            out << "    cmp rax, rbx\n";
            out << "    mov rax, rdx\n";
            out << "    cmovg rax, rcx\n";
            out << "    push rax\n";
        }

        else if (elem.op_type == OP_LT)
        {
            out << "    ;; -- less than --\n";
            out << "    pop rax\n";
            out << "    pop rbx\n";
            out << "    mov rcx, 1\n";
            out << "    mov rdx, 0\n";
            out << "    cmp rax, rbx\n";
            out << "    mov rax, rdx\n";
            out << "    cmovl rax, rcx\n";
            out << "    push rax\n";
        }

        else if (elem.op_type == OP_GTE)
        {
            out << "    ;; -- greater than or equal --\n";
            out << "    pop rax\n";
            out << "    pop rbx\n";
            out << "    mov rcx, 1\n";
            out << "    mov rdx, 0\n";
            out << "    cmp rax, rbx\n";
            out << "    mov rax, rdx\n";
            out << "    cmovge rax, rcx\n";
            out << "    push rax\n";
        }

        else if (elem.op_type == OP_LTE)
        {
            out << "    ;; -- less than or equal --\n";
            out << "    pop rax\n";
            out << "    pop rbx\n";
            out << "    mov rcx, 1\n";
            out << "    mov rdx, 0\n";
            out << "    cmp rax, rbx\n";
            out << "    mov rax, rdx\n";
            out << "    cmovle rax, rcx\n";
            out << "    push rax\n";
        }

        else if (elem.op_type == OP_INC)
        {
            out << "    ;; -- inc --\n";
            out << "    pop rax\n";
            out << "    add rax, 1\n";
            out << "    push rax\n";
        }

        else if (elem.op_type == OP_DEC)
        {
            out << "    ;; -- dec --\n";
            out << "    pop rax\n";
            out << "    sub rax, 1\n";
            out << "    push rax\n";
        }

    }
    out << "    ;; -- exit --\n";
    out << "    mov rax, 0x2000001\n";
    out << "    mov rdi, 0\n";
    out << "    syscall\n";

    out.close();
}

void usage()
{
    std::cout << "Usage: cell <SUBCOMMAND> [ARGS]\n";
    std::cout << "SUBCOMMANDS:\n";
    std::cout << "    sim <input files>                      Simulate the program\n";
    std::cout << "    com <executable name> <input files>    Compile the program\n";
}

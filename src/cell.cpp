#include "include/cell.h"

void print_error(std::string loc, std::string msg)
{
    std::size_t last_colon = loc.find_last_of(':');
    std::cerr << loc.substr(0, last_colon) << ": " << msg << loc.substr(last_colon) << "\n";
    exit(EXIT_FAILURE);
}

code parse_op(std::string value, std::string msg)
{
    if (value == "+")
    {
        return { .type = OP_PLUS, .loc = msg };
    }

    if (value == "-")
    {
        return { .type = OP_MINUS, .loc = msg };
    }

    if (value == "=")
    {
        return { .type = OP_EQUAL, .loc = msg };
    }

    if (value == "dump")
    {
        return { .type = OP_DUMP, .loc = msg };
    }

    if (value == "if")
    {
        return { .type = OP_IF, .loc = msg };
    }

    if (value == "else")
    {
        return { .type = OP_ELSE, .loc = msg };
    }

    if (value == "end")
    {
        return { .type = OP_END, .loc = msg };
    }

    if (value == "dup")
    {
        return { .type = OP_DUP, .loc = msg };
    }

    if (value == "dup2")
    {
        return { .type = OP_DUP2, .loc = msg };
    }

    if (value == ">")
    {
        return { .type = OP_GT, .loc = msg };
    }

    if (value == "<")
    {
        return { .type = OP_LT, .loc = msg };
    }

    if (value == ">=")
    {
        return { .type = OP_GTE, .loc = msg };
    }

    if (value == "<=")
    {
        return { .type = OP_LTE, .loc = msg };
    }

    if (value == "inc")
    {
        return { .type = OP_INC, .loc = msg };
    }

    if (value == "dec")
    {
        return { .type = OP_DEC, .loc = msg };
    }

    if (value == "while")
    {
        return { .type = OP_WHILE, .loc = msg };
    }

    if (value == "do")
    {
        return { .type = OP_DO, .loc = msg };
    }

    if (value == "mem")
    {
        return { .type = OP_MEM, .loc = msg };
    }

    if (value == ",")
    {
        return { .type = OP_LOAD, .loc = msg };
    }

    if (value == ".")
    {
        return { .type = OP_STORE, .loc = msg };
    }

    if (value == "write")
    {
        return { .type = OP_WRITE, .loc = msg };
    }

    if (value == "exit")
    {
        return { .type = OP_EXIT, .loc = msg };
    }

    try 
    {
        return { .type = OP_PUSH, .value = std::stoi(value), .loc = msg };
    }
    
    catch (const std::exception& e)
    {
        print_error(msg, "invalid command");
        return {};
    }
}

std::vector<code> parse_blocks(std::vector<code> program)
{
    std::stack<int> stack;

    for (int i = 0; i < program.size(); i++)
    {
        code op = program[i];
        if (op.type == OP_IF)
        {
            stack.push(i);
        }

        else if (op.type == OP_ELSE)
        {
            int if_i = stack.top();
            stack.pop();

            if (program[if_i].type != OP_IF)
            {
                print_error(program[if_i].loc, "ERROR: `else` keyword can only be used in conjunction with `if`");
            }

            program[if_i] = { OP_IF, i + 1 };
            stack.push(i);
        }

        else if (op.type == OP_END)
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
        }

        else if (op.type == OP_WHILE)
        {
            stack.push(i);
        }

        else if (op.type == OP_DO)
        {
            int while_i = stack.top();
            stack.pop();
            program[i] = { OP_DO, while_i };
            stack.push(i);
        }
    }

    if (stack.size() > 0)
    {
        print_error(program[stack.top()].loc, "ERROR: unclosed block");
    }

    return program;
}

std::vector<code> load_program(std::string input_file)
{
    std::vector<code> program;
    std::stringstream ss;

    std::string file = remove_comments(input_file);
    ss << file;
    std::string line;

    std::vector<std::string> lexed_file = lex_file(input_file);

    int curr = 0;
    while (getline(ss, line))
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

    ss.clear();

    return program;
}

void simulate_program(std::vector<code> program)
{
    std::stack<int> stack;
    byte memory[MEM_CAPACITY];
    memset(memory, (byte)(0), MEM_CAPACITY);

    int i = 0;
    while (i < program.size())
    {
        code op = program[i];
        if (op.type == OP_PUSH)
        {
            stack.push(op.value);
            i++;
        }

        else if (op.type == OP_PLUS)
        {
            int a = stack.top();
            stack.pop();

            int b = stack.top();
            stack.pop();

            stack.push(a + b);
            i++;
        }

        else if (op.type == OP_MINUS)
        {
            int a = stack.top();
            stack.pop();

            int b = stack.top();
            stack.pop();

            stack.push(-a + b);
            i++;
        }

        else if (op.type == OP_EQUAL)
        {
            int a = stack.top();
            stack.pop();

            int b = stack.top();
            stack.pop();

            a == b ? stack.push(1) : stack.push(0);
            i++;
        }

        else if (op.type == OP_DUMP)
        {
            std::cout << stack.top() << "\n";
            stack.pop();
            i++;
        }

        else if (op.type == OP_IF)
        {
            int cond = stack.top();
            stack.pop();

            cond == 0 ? i = op.value : i++;
        }

        else if (op.type == OP_ELSE)
        {
            i = op.value;
        }

        else if (op.type == OP_END)
        {
            i = op.value;
        }

        else if (op.type == OP_DUP)
        {
            int a = stack.top();
            stack.push(a);
            i++;
        }

        else if (op.type == OP_DUP2)
        {
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.push(a);
            stack.push(b);
            stack.push(a);
            i++;
        }

        else if (op.type == OP_GT)
        {
            int a = stack.top();
            stack.pop();

            int b = stack.top();
            stack.pop();

            b > a ? stack.push(1) : stack.push(0);
            i++;
        }

        else if (op.type == OP_LT)
        {
            int a = stack.top();
            stack.pop();

            int b = stack.top();
            stack.pop();

            a > b ? stack.push(1) : stack.push(0);
            i++;
        }

        else if (op.type == OP_GTE)
        {
            int a = stack.top();
            stack.pop();

            int b = stack.top();
            stack.pop();

            b >= a ? stack.push(1) : stack.push(0);
            i++;
        }

        else if (op.type == OP_LTE)
        {
            int a = stack.top();
            stack.pop();

            int b = stack.top();
            stack.pop();

            a >= b ? stack.push(1) : stack.push(0);
            i++;
        }

        else if (op.type == OP_INC)
        {
            int a = stack.top();
            stack.pop();
            a++;
            stack.push(a);

            i++;
        }

        else if (op.type == OP_DEC)
        {
            int a = stack.top();
            stack.pop();
            a--;
            stack.push(a);

            i++;
        }

        else if (op.type == OP_WHILE)
        {
            i++;
        }

        else if (op.type == OP_DO)
        {
            int a = stack.top();
            stack.pop();

            a == 0 ? i = op.value : i++;
        }

        else if (op.type == OP_MEM)
        {
            stack.push(0);
            i++;
        }

        else if (op.type == OP_LOAD)
        {
            int addr = stack.top();
            stack.pop();
            stack.push(memory[addr]);
            i++;
        }

        else if (op.type == OP_STORE)
        {
            int value = stack.top();
            stack.pop();
            int addr = stack.top();
            stack.pop();

            memory[addr] = (value & ((1ULL << 8) - 1));
            i++;
        }

        else if (op.type == OP_WRITE)
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
        }

        else if (op.type == OP_EXIT)
        {
            int exit_code = stack.top();
            stack.pop();
            exit(exit_code);
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

    int i = 0;
    for (i = 0; i < program.size(); i++)
    {
        code op = program[i];
        out << "\naddr_" << i << ":\n";

        if (op.type == OP_PUSH)
        {
            out << "    ;; -- push --\n";
            out << "    push " << op.value << "\n";
        }

        else if (op.type == OP_PLUS)
        {
            out << "    ;; -- plus --\n";
            out << "    pop rax\n";
            out << "    pop rbx\n";
            out << "    add rax, rbx\n";
            out << "    push rax\n";
        }

        else if (op.type == OP_MINUS)
        {
            out << "    ;; -- minus --\n";
            out << "    pop rbx\n";
            out << "    pop rax\n";
            out << "    sub rax, rbx\n";
            out << "    push rax\n";
        }

        else if (op.type == OP_EQUAL)
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

        else if (op.type == OP_DUMP)
        {
            out << "    ;; -- dump --\n";
            out << "    pop rdi\n";
            out << "    call dump\n";
        }

        else if (op.type == OP_IF)
        {
            out << "    ;; -- if --\n";
            out << "    pop rax\n";
            out << "    test rax, rax\n";
            out << "    jz addr_" << op.value << "\n";
        }

        else if (op.type == OP_ELSE)
        {
            out << "    ;; -- else --\n";
            out << "    jmp addr_" << op.value << "\n";
        }

        else if (op.type == OP_END)
        {
            out << "    ;; -- end --\n";

            if (i + 1 != op.value)
            {
                out << "    jmp addr_" << op.value << "\n";
            }
        }

        else if (op.type == OP_DUP)
        {
            out << "    ;; -- dup --\n";
            out << "    pop rax\n";
            out << "    push rax\n";
            out << "    push rax\n";
        }

        else if (op.type == OP_DUP2)
        {
            out << "    ;; -- dup2 --\n";
            out << "    pop rbx\n";
            out << "    pop rax\n";
            out << "    push rax\n";
            out << "    push rbx\n";
            out << "    push rax\n";
            out << "    push rbx\n";
        }

        else if (op.type == OP_GT)
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
        }

        else if (op.type == OP_LT)
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
        }

        else if (op.type == OP_GTE)
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
        }

        else if (op.type == OP_LTE)
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
        }

        else if (op.type == OP_INC)
        {
            out << "    ;; -- inc --\n";
            out << "    pop rax\n";
            out << "    add rax, 1\n";
            out << "    push rax\n";
        }

        else if (op.type == OP_DEC)
        {
            out << "    ;; -- dec --\n";
            out << "    pop rax\n";
            out << "    sub rax, 1\n";
            out << "    push rax\n";
        }

        else if (op.type == OP_WHILE)
        {
            out << "    ;; -- while --\n";
        }

        else if (op.type == OP_DO)
        {
            out << "    ;; -- do --\n";
            out << "    pop rax\n";
            out << "    test rax, rax\n";
            out << "    jz addr_" << op.value << "\n";
        }

        else if (op.type == OP_MEM)
        {
            out << "    ;; -- mem --\n";
            out << "    xor rdi, rdi\n";
            out << "    lea rdi, [mem]\n";
            out << "    push rdi\n";
        }

        else if (op.type == OP_LOAD)
        {
            out << "    ;; -- load (,) --\n";
            out << "    pop rax\n";
            out << "    xor rbx, rbx\n";
            out << "    mov bl, [rax]\n";
            out << "    push rbx\n";
        }

        else if (op.type == OP_STORE)
        {
            out << "    ;; -- store (.) --\n";
            out << "    pop rbx\n";
            out << "    pop rax\n";
            out << "    mov [rax], bl\n";
        }

        else if (op.type == OP_WRITE)
        {
            out << "    ;; -- write --\n";

            // order to pass parameters for syscall:
            // rax (syscall number), then rdi, rsi, rdx, rcx, r8, r9
            out << "    mov rax, 0x2000004\n";
            out << "    pop rdi\n";
            out << "    pop rsi\n";
            out << "    pop rdx\n";
            out << "    syscall\n";
        }

        else if (op.type == OP_EXIT)
        {
            out << "    ;; -- exit --\n";
            out << "    mov rax, 0x2000001\n";
            out << "    pop rdi\n";
            out << "    syscall\n";
        }
    }
    
    out << "\naddr_" << i << ":\n";
    out << "    ;; -- exit --\n";
    out << "    mov rax, 0x2000001\n";
    out << "    mov rdi, 0\n";
    out << "    syscall\n";

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

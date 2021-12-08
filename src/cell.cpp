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

code dump()
{
    return { OP_DUMP, };
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

    if (value == "dump")
    {
        return dump();
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

    for (auto elem : program)
    {
        if (elem.op_type == OP_PUSH)
        {
            stack.push(elem.value);
        }

        else if (elem.op_type == OP_PLUS)
        {
            int a = stack.top();
            stack.pop();

            int b = stack.top();
            stack.pop();

            stack.push(a + b);
        }

        else if (elem.op_type == OP_MINUS)
        {
            int a = stack.top();
            stack.pop();

            int b = stack.top();
            stack.pop();

            stack.push(-a + b);
        }

        else if (elem.op_type == OP_DUMP)
        {
            std::cout << stack.top() << "\n";
            stack.pop();
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

    for (auto elem : program)
    {
        if (elem.op_type == OP_PUSH)
        {
            out << "\n    ;; push " << elem.value << "\n";
            out << "    push " << elem.value << "\n";
        }

        else if (elem.op_type == OP_PLUS)
        {
            out << "\n    ;; plus\n";
            out << "    pop rax\n";
            out << "    pop rbx\n";
            out << "    add rax, rbx\n";
            out << "    push rax\n";
        }

        else if (elem.op_type == OP_MINUS)
        {
            out << "\n    ;; minus\n";
            out << "    pop rbx\n";
            out << "    pop rax\n";
            out << "    sub rax, rbx\n";
            out << "    push rax\n";
        }

        else if (elem.op_type == OP_DUMP)
        {
            out << "\n    ;; dump\n";
            out << "    pop rdi\n";
            out << "    call dump\n";
        }
    }
    out << "\n    ;; exit\n";
    out << "    mov rax, 0x2000001\n";
    out << "    mov rdi, 0\n";
    out << "    syscall\n";
}

void usage()
{
    std::cout << "Usage: cell <SUBCOMMAND> [ARGS]\n";
    std::cout << "SUBCOMMANDS:\n";
    std::cout << "    sim <input files>                      Simulate the program\n";
    std::cout << "    com <executable name> <input files>    Compile the program\n";
}

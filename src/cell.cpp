/* Cell:    a stack-based programming language written in C++
 * Author:  Shreyas Ravi
 * Created: September 24 2021
 */

#include <iostream>
#include <fstream>
#include <stack>
#include <vector>

// Operations
enum
{
    OP_PUSH,  // push value to stack
    OP_PLUS,  // add top two values on stack
    OP_MINUS, // subtract top two values on stack
    OP_MUL,   // subtract top two values on stack
    OP_DIV,   // subtract top two values on stack
    OP_MOD,   // subtract top two values on stack
    OP_DUMP,  // print top value on stack to console
    LEN_OPS   // number of valid operations
};

struct code
{
    int op_type;
    int value;
};

code push(int x);
code plus();
code minus();
code dump();
bool string_is_int(std::string s);
code parse_op(std::string value);
std::vector<code> load_program(std::string input_file);
void simulate_program(std::vector<code> program);
void compile_program(std::vector<code> program, std::string output_file);
void usage();

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

code parse_op(std::string value)
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

    return push(std::stoi(value));
}

std::vector<code> load_program(std::string input_file)
{
    std::vector<code> program;
    std::ifstream in;
    try
    {
        in.open(input_file);
        std::string value;
        while (!in.eof())
        {
            in >> value;
            program.push_back(parse_op(value));
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

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        usage();
        std::cerr << "ERROR: no subcommand provided\n";
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "sim") == 0)
    {
        if (argc < 3)
        {
            usage();
            std::cerr << "ERROR: no input file provided\n";
            exit(EXIT_FAILURE);
        }

        std::vector<code> program = load_program(argv[2]);
        simulate_program(program);
    }

    else if (strcmp(argv[1], "com") == 0)
    {
        if (argc < 4)
        {
            usage();

            if (strstr(".cll", argv[2]) == NULL)
                std::cerr << "ERROR: no input file provided\n";
            else
                std::cerr << "ERROR: no executable name provided\n";

            exit(EXIT_FAILURE);
        }

        std::vector<code> program = load_program(argv[3]);
        std::string output_file = argv[2];

        std::string asm_command = "nasm -fmacho64 -o " + output_file + ".o " + output_file + ".asm";
        std::string lnk_command = "ld " + output_file + ".o -o " + output_file + ".out -macosx_version_min 11.0 -L /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib -lSystem";
        std::string rmf_command = "rm " + output_file + ".asm " + output_file + ".o";

        std::cout << "[INFO] Generating " << output_file << ".asm\n";
        compile_program(program, output_file + ".asm");

        std::cout << "[CMD] " << asm_command << "\n";
        system(asm_command.c_str());

        std::cout << "[CMD] " << lnk_command << "\n";
        system(lnk_command.c_str());

        // std::cout << "[CMD] " << rmf_command << "\n";
        // system(rmf_command.c_str());
    }

    else
    {
        usage();
        std::cerr << "ERROR: unknown subcommand " << argv[1] << "\n";
        exit(EXIT_FAILURE);
    }
}
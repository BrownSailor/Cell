/* slang:   a stack-based programming language written in C++
 * Author:  Shreyas Ravi
 * Created: September 24 2021
 */

#include "include/slang.h"
#include "include/lexer.h"
#include <string.h>

using namespace std;

int main(int argc, char **argv)
{

    if (argc != 3 && argc != 4)
    {
        usage();
        exit(EXIT_FAILURE);
    }

    std::vector<Op> program = load_program(argv[argc - 1]);
    std::string output_file = argv[argc - 2];

    std::string asm_command = "nasm -fmacho64 -o " + output_file + ".o " + output_file + ".asm";
    std::string lnk_command = "ld " + output_file + ".o -o " + output_file + " -macosx_version_min 11.0 -L /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib -lSystem";
    std::string rmf_command = "rm " + output_file + ".asm " + output_file + ".o";

    std::cout << "[INFO] Generating " << output_file << ".asm\n";
    compile_program(program, output_file + ".asm");

    std::cout << "[CMD] " << asm_command << "\n";
    system(asm_command.c_str());

    std::cout << "[CMD] " << lnk_command << "\n";
    system(lnk_command.c_str());

    if (argc == 3)
    {
        std::cout << "[CMD] " << rmf_command << "\n";
        system(rmf_command.c_str());
    }
}
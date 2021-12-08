/* Cell:    a stack-based programming language written in C++
 * Author:  Shreyas Ravi
 * Created: September 24 2021
 */

#include "include/cell.h"
#include "include/lexer.h"

using namespace std;

int main(int argc, char **argv)
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

        // cout << "[CMD] " << rmf_command << "\n";
        // system(rmf_command.c_str());
    }

    else
    {
        usage();
        std::cerr << "ERROR: unknown subcommand " << argv[1] << "\n";
        exit(EXIT_FAILURE);
    }
}
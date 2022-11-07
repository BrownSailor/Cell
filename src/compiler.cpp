#include "include/compiler.h"

/*
 * compile_and_link
 *    Purpose: assemble, compile, and link the AST and write the output to the given file
 * Parameters: filename - output file name, root - the root of the AST, out - the output stream for command printing
 *    Returns: none
 *      Notes: out is defaulted to std::cout
 */
void compile_and_link(std::string filename, Node *root, std::ostream &out)
{
    std::string cmp_command = "clang++ -O3 -o " + filename + " ." + filename + ".cc";
    std::string rmf_command = "rm ." + filename + ".cc";

    out << "[INFO] Compiling " << filename << ".sl to " << filename << "\n";
    assemble("." + filename + ".cc", root);

    system(cmp_command.c_str());
    system(rmf_command.c_str());
}

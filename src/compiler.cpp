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
    std::string asm_command = "nasm -fmacho64 -o " + filename + ".o " + filename + ".asm";
    std::string lnk_command = "ld " + filename + ".o -o " + filename + " -macosx_version_min 11.0 -L /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib -lSystem";
    // std::string rmf_command = "rm " + filename + ".asm " + filename + ".o";

    out << "[INFO] Generating " << filename << ".asm\n";
    assemble_nasm(filename + ".asm", root);

    out << "[CMD] " << asm_command << "\n";
    system(asm_command.c_str());

    out << "[CMD] " << lnk_command << "\n";
    system(lnk_command.c_str());
}
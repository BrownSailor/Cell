#include "include/compiler.h"
#include <iostream>

/* compile
 *    Purpose: compile a program given input file
 * Parameters: input file name
 *    Returns: None;
 */
void compile(std::string fileName)
{
    std::string src = readFile(fileName);

    Lexer *lexer = initLexer(src);
    Parser *parser = initParser(lexer);
    Tree *root = parserParse(parser);

    std::string s = asmInit(root);

    std::string filePath = fileName.substr(0, fileName.find('.'));
    filePath = filePath.find('/') != std::string::npos ? filePath.substr(filePath.find('/') + 1) : filePath;
    writeFile(filePath + ".asm", s);

    std::string run = "nasm -fmacho64 -o " + filePath + ".o " + filePath + ".asm " + "&& ld " + filePath + ".o -o " + filePath + ".out -macosx_version_min 11.0 -L \
    /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib -lSystem";
    system(run.c_str());

    std::string toFile = "echo \"" + s + "\" > " + filePath + ".txt";
    system(toFile.c_str());

    std::string removeUnwantedFiles = "rm " + filePath + ".asm " + filePath + ".o";
    system(removeUnwantedFiles.c_str());

    delete lexer;
    delete root;
    delete parser;
}
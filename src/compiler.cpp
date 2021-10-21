#include "include/compiler.h"

/* compile
 *    Purpose: compile a program given input file
 * Parameters: input file name
 *    Returns: None
 */
void compile(std::string fileName)
{
    std::string src = readFile(fileName);

    Lexer *lexer = initLexer(src);
    Parser *parser = initParser(lexer);
    Tree *root = parserParse(parser);

    std::vector<Tree *> list;
    Visitor *visitor = initVisitor();
    Tree *optimizedRoot = visitorVisit(visitor, root, list);

    std::string s = asmInit(optimizedRoot, list);

    // generate .asm, .o, and .out files
    std::string filePath = fileName.substr(0, fileName.find('.'));
    filePath = filePath.find('/') != std::string::npos ? filePath.substr(filePath.find('/') + 1) : filePath;
    writeFile(filePath + ".asm", s);

    std::string run = "nasm -fmacho64 -o " + filePath + ".o " + filePath + ".asm " + "&& ld " + filePath + ".o -o " + filePath + ".out -macosx_version_min 11.0 -L \
    /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib -lSystem";
    system(run.c_str());

    std::string toFile = "echo \"" + s + "\" > " + filePath + ".txt";
    system(toFile.c_str());

    // free memory
    delete root;
    delete parser;
    delete lexer;
}
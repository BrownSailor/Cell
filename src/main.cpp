#include "include/compiler.h"
#include "include/io.h"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Please specify an input file.\n";
        return 1;
    }

    compile_file(argv[1]);

    return 0;
}
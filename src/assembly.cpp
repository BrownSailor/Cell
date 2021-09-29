#include "include/assembly.h"
#include <iostream>
#include <sstream>

std::string asmCompound(Tree *tree)
{
    std::string assembly;
    for (int i = 0; i < tree->children.size(); i++)
    {
        Tree *child = tree->children[i];
        assembly += asmMain(child);
    }

    return assembly;
}

std::string asmFuncDef(Tree *tree)
{
    std::string assembly;
    if (tree->data->type == TREE_FUNC_DEF)
    {
        std::ostringstream out;
        out << "\t;; -- " << tree->name << " --\n"
            << tree->name << ":\n";

        assembly += out.str();

        Tree *data = tree->data;
        std::string dataData = asmMain(data->data);
        assembly += dataData;
    }

    return assembly;
}

std::string asmAssign(Tree *tree)
{
}

std::string asmVariable(Tree *tree)
{
    return "rdi";
}

std::string asmInt(Tree *tree)
{
}

std::string asmCall(Tree *tree)
{
    std::string assembly;
    if (tree->name == "ret")
    {
        Tree *firstArg = tree->data->children.size() ? tree->data->children[0] : nullptr;
        std::cout << firstArg->type << "\n";

        std::string embedAsm = asmMain(firstArg);
        std::ostringstream out;
        out << "\t;; -- return " << (firstArg->intValue ? firstArg->intValue : 0) << " --\n"
            << "\tmov rdi, " << embedAsm << "\n"
            << "\tret\n";

        assembly += out.str();
    }

    return assembly;
}

std::string asmMain(Tree *tree)
{
    switch (tree->type)
    {
    case TREE_COMPOUND:
        return asmCompound(tree);

    case TREE_FUNC_DEF:
        return asmFuncDef(tree);

    case TREE_ASSIGN:
        return asmAssign(tree);

    case TREE_VARIABLE:
        return asmVariable(tree);

    case TREE_INT:
        return asmInt(tree);

    case TREE_CALL:
        return asmCall(tree);

    default:
        std::cout << "[ASM]: No method of generating assembly for Tree of type `" << tree->type << "`\n";
        exit(1);
    }

    return "";
}

std::string asmInit(Tree *tree)
{
    std::ostringstream out;
    out << "\n\tglobal _main" << tree->name << "\n"
        << "\tsection .text\n"
        << "_main:\n"
        << "\tcall main\n"
        << "\tmov rax, rdi\n"
        << "\tmov rax, 0x2000001\n"
        << "\tsyscall\n";

    std::string value = asmMain(tree) + out.str();

    return value;
}
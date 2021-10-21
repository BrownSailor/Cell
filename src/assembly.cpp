#include "include/assembly.h"

std::string registers[] = {"rdi", "rsi", "rdx", "rcx", "r8d", "r9d"};

static Tree *varLookUp(std::vector<Tree *> &list, std::string name)
{
    for (size_t i = 0; i < list.size(); i++)
    {
        Tree *tree = list[i];
        if (tree->type == TREE_VAR && tree->name == name)
        {
            return tree;
        }
    }

    return nullptr;
}

std::string asmCompound(Tree *tree, std::vector<Tree *> &list)
{
    std::string assembly;
    for (size_t i = 0; i < tree->children.size(); i++)
    {
        Tree *child = tree->children[i];
        assembly += asmMain(child, list);
    }

    return assembly;
}

std::string asmFuncDef(Tree *tree, std::vector<Tree *> &list)
{
    std::string assembly;

    if (tree->data->type == TREE_FUNC_DEC)
    {
        assembly = "\t;; -- " + tree->name + " --\n";
        assembly += "\tglobal _" + tree->name + "\n";
        assembly += "\tsection .text\n";

        assembly += "_" + tree->name + ":\n";
        assembly += "\tpush rbp\n"
                    "\tmov rbp, rsp\n";

        for (int i = 0; i < tree->data->children.size(); i++)
        {
            std::cout << tree->data->children[i]->name << "\n";
            std::cout << tree->data->children[i]->byteSize << "\n";

            Tree *argument = initTree(TREE_VAR);
            argument->name = tree->data->children[i]->name;
            argument->byteSize = tree->data->children[i]->byteSize;

            list.push_back(argument);

            if (i < 6)
            {
                assembly += "\tmov [rbp - " + std::to_string(argument->byteSize) + "], " + registers[i] + "\n";
            }
        }

        std::string dataData = asmMain(tree->data->data, list);
        assembly += dataData;
    }

    return assembly;
}

std::string asmAssign(Tree *tree, std::vector<Tree *> &list)
{
}

std::string asmVariable(Tree *tree, std::vector<Tree *> &list, int id)
{
    Tree *variable = varLookUp(list, tree->name);

    if (variable == nullptr)
    {
        std::cerr << "[ASM]: `" << tree->name << "` is not defined.\n";
        exit(EXIT_FAILURE);
    }

    std::string assembly = "\tmov rax, [rbp - " + std::to_string(variable->byteSize) + "]\n";

    return assembly;
}

std::string asmInt(Tree *tree, std::vector<Tree *> &list)
{
    std::string assembly = "\tmov rax, " + std::to_string(tree->intValue) + "\n";

    return assembly;
}

std::string asmCall(Tree *tree, std::vector<Tree *> &list)
{
    std::string assembly;
    if (tree->name == "ret")
    {
        Tree *firstArg = tree->data->children.size() ? tree->data->children[0] : nullptr;

        std::string embedAsm = asmMain(firstArg, list);
        assembly = "\n\t;; -- return --\n" + embedAsm + "\tpop rbp\n\tret\n";
    }

    return assembly;
}

std::string asmAccess(Tree *tree, std::vector<Tree *> &list)
{
    Tree *left = varLookUp(list, tree->name);
    std::string assembly = asmMain(left, list);
    assembly += "\tadd rax, " + std::to_string(tree->data->intValue * 8) + "\n";
    assembly += "\tmov rax, [rax]\n";

    return assembly;
}

std::string asmMain(Tree *tree, std::vector<Tree *> &list)
{
    switch (tree->type)
    {
    case TREE_COMPOUND:
        return asmCompound(tree, list);

    case TREE_FUNC_DEC:
        return asmFuncDef(tree, list);

    case TREE_ASSIGN:
        return asmAssign(tree, list);

    case TREE_VAR:
        return asmVariable(tree, list, 0);

    case TREE_INT:
        return asmInt(tree, list);

    case TREE_CALL:
        return asmCall(tree, list);

    case TREE_ACCESS:
        return asmAccess(tree, list);

    default:
        std::cerr << "[ASM]: No method of generating assembly for Tree of type `" << tree->type << "`\n";
        exit(EXIT_FAILURE);
    }

    return "";
}

std::string asmInit(Tree *tree, std::vector<Tree *> &list)
{

    return asmMain(tree, list);
}
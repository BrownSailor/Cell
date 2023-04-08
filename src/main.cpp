#include <iostream>

#include "lexer.h"
#include "built_in.h"
#include "parser.h"
#include "types.h"
#include "compiler.h"

/*
 * pretty_print_tabs
 *    Purpose: print the number of tabs necessary as well as the ending character
 * Parameters: num_tabs - the number of tabs to print
 *    Returns: none
 */
static void pretty_print_tabs(int num_tabs)
{
    for (int i = 0; i < num_tabs - 1; i++)
    {
        std::cerr << "    ";
    }
    std::cerr << "   \u2502\n";

    for (int i = 0; i < num_tabs - 1; i++)
    {
        std::cerr << "    ";
    }
    std::cerr << "   \u2514";
}

/*
 * pretty_print_helper
 *    Purpose: pretty print a node and its children recursively
 * Parameters: node - the node to start printing from, num_tabs - the number of tabs to print, out - the output stream
 *    Returns: none
 *      Notes: the output stream is defaulted to std::cout
 */
static void pretty_print_helper(Node *node, int num_tabs)
{
    switch (node->type)
    {
        case Node::NODE_PROG:
        {
            std::cerr << "program\n";
            break;
        }
        case Node::NODE_FUN_IN:
        {
            std::cerr << "fn_in\n";
            break;
        }
        case Node::NODE_FUN_OUT:
        {
            std::cerr << "fn_out\n";
            break;
        }
        case Node::NODE_BODY:
        {
            std::cerr << "body\n";
            break;
        }
        default:
        {
            uint32_t type_id = node->datatype & ((1 << 20) - 1);
            uint32_t type_no = (node->datatype & (((1 << 12) - 1) << 20)) >> 20;
            std::cerr << node->token.data << ": " << type_idens[type_id] << " " << type_no << "\n";
            break;
        }
    }

    for (auto it = node->children.begin(); it != node->children.end(); std::advance(it, 1))
    {
        pretty_print_tabs(num_tabs + 1);
        pretty_print_helper(*it, num_tabs + 1);
    }
}

/*
 * pretty_print
 *    Purpose: pretty print a tree using the helper function
 * Parameters: node - the node to start printing the tree from, out - the output stream
 *    Returns: none
 *      Notes: the output stream is defaulted to std::cout
 */
void pretty_print(Node *node)
{
    pretty_print_helper(node, 0);
    std::cerr << "\n";
}

void usage()
{
    std::cerr << "Usage: ./slang executableName inputFile\n";
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    initialize_built_ins();
    initialize_types();

    if (argc != 2)
    {
        usage();
    }

    std::string file = argv[1];

    /* lex file to tokens */
    std::list<Token> tokens = lex(file);

    /* parse abstract syntax tree */
    Node *root = parse_program(tokens);

    /* type check the AST */
    // type_check(root);
    pretty_print(root);

    /* generate assembly based on syntax tree */
    // compile(root);

    /* create a relocatable object file */
    // std::string assemble = "llc -march=x86_64 -filetype=obj " + file + ".ll -o " + file + ".o";
    // system(assemble.c_str());
    
    free_tree(root);
    return 0;
}

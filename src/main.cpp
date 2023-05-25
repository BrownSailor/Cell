#include "parser.hpp"

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
 *      Notes: the output stream is defaulted to std::cerr
 */
static void pretty_print_helper(std::unique_ptr<Node> node, int num_tabs)
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
        case Node::NODE_VAR_ASN:
        {
            std::cerr << "set\n";
            break;
        }
        default:
        {
            std::cerr << node->token.data << "\n";
            break;
        }
    }

    for (auto it = node->children.begin(); it != node->children.end(); std::advance(it, 1))
    {
        pretty_print_tabs(num_tabs + 1);
        pretty_print_helper(std::move(*it), num_tabs + 1);
    }
}

/*
 * pretty_print
 *    Purpose: pretty print a tree using the helper function
 * Parameters: node - the node to start printing the tree from
 *    Returns: none
 */
void pretty_print(std::unique_ptr<Node> node)
{
    pretty_print_helper(std::move(node), 0);
    std::cerr << "\n";
}

void usage()
{
    std::cerr << "slang compiler\n";
    std::cerr << "Usage: ./slang file\n";
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        usage();
    }

    std::string file = argv[1];

    /* lex file to tokens */
    std::list<Token> tokens = lex(file);

    std::unique_ptr<Node> root = parse_program(tokens);

    pretty_print(std::move(root));

    return 0;
}

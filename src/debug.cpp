#include "debug.hpp"

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
        case Node::NODE_VAR_ASN:
        {
            std::cerr << "set\n";
            break;
        }
        default:
        {
            std::cerr << node->token.data << ": ";
            switch (node->type_scheme.type)
            {
                case TypeScheme::ALPHA:
                {
                    uint32_t arr_size = node->type_scheme.alpha >> 28;

                    if (arr_size)
                    {
                        std::cerr << "[ " << arr_size << " ";
                        std::cerr << type_idens[arr_size << 28 ^ node->type_scheme.alpha] << " ]";
                    }
                    else
                    {
                        std::cerr << type_idens[node->type_scheme.alpha];
                    }
                    break;
                }
                case TypeScheme::FUN_TYPE:
                {
                    for (auto t : node->type_scheme.fun_type.second)
                    {
                        std::cerr << type_idens[t] << " ";
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }

            std::cerr << "\n";
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
 * Parameters: node - the node to start printing the tree from
 *    Returns: none
 */
void pretty_print(Node *node)
{
    pretty_print_helper(node, 0);
    std::cerr << "\n";
}


#include "include/util.h"

/*
 * get_node_type
 *    Purpose: print a string representation of the type of a given node
 * Parameters: node - the given node, out - an output stream
 *    Returns: none
 */
std::string node_type_to_str(Node *node)
{
    switch (node->type)
    {
        case Node::NODE_LIT:
        {
            return "NODE_LIT";
        }
        case Node::NODE_OP:
        {
            return "NODE_OP";
        }
        case Node::NODE_KEY:
        {
            return "NODE_KEY";
        }
        case Node::NODE_VAR:
        {
            return "NODE_VAR";
        }
        case Node::NODE_VAR_ASN:
        {
            return "NODE_VAR_ASN";
        }
        case Node::NODE_VAR_DEC:
        {
            return "NODE_VAR_DEC";
        }
        case Node::NODE_VAR_DEC_ASN:
        {
            return "NODE_VAR_DEC_ASN";
        }
        case Node::NODE_LIST:
        {
            return "NODE_LIST";
        }
        case Node::NODE_TYPE:
        {
            return "NODE_TYPE";
        }
        case Node::NODE_TYPE_DEF:
        {
            return "NODE_TYPE_DEF";
        }
        case Node::NODE_FUNC_CALL:
        {
            return "NODE_FUNC_CALL";
        }
        case Node::NODE_FUNC_DEC:
        {
            return "NODE_FUNC_DEC";
        }

        default:
        {
            return "NODE_NONE";
        }
    }
}

/*
 * pretty_print_tabs
 *    Purpose: print the number of tabs necessary as well as the ending character
 * Parameters: num_tabs - the number of tabs to print
 *    Returns: none
 */
void pretty_print_tabs(int num_tabs, std::ostream &out)
{
    for (int i = 0; i < num_tabs - 1; i++)
    {
        out << "    ";
    }
    out << "   \u2502\n";

    for (int i = 0; i < num_tabs - 1; i++)
    {
        out << "    ";
    }
    out << "   \u2514";
}

/*
 * pretty_print_helper
 *    Purpose: pretty print a node and its children recursively
 * Parameters: node - the node to start printing from, num_tabs - the number of tabs to print, out - the output stream
 *    Returns: none
 *      Notes: the output stream is defaulted to std::cout
 */
void pretty_print_helper(Node *node, const Scope &scope, int num_tabs, std::ostream &out)
{
    print_token(node->token, out, false);

    if (node->arr_dim)
    {
        out << " ";
        for (size_t i = 0; i < node->arr_dim; i++)
        {
            out << "[]";
        }
    }

    if (node->semi)
    {
        out << ";";
    }

    if (node->type != Node::NODE_NONE)
    {
        out << " (" << node_type_to_str(node) << ")";
    }

    // if (get_type(node, scope) != "void")
    // {
    //     out << " [ " << get_type(node, scope) << " ]";
    // }

    if (node->scope.size())
    {
        print_scope(node->scope, out);
    }

    out << "\n";
    num_tabs++;

    for (auto it = node->children.begin(); it != node->children.end(); std::advance(it, 1))
    {
        pretty_print_tabs(num_tabs, out);
        pretty_print_helper(*it, scope, num_tabs, out);
    }
    num_tabs--;
}

void print_scope(const Scope &scope, std::ostream &out)
{
    out << " { ";
    for (auto x : scope)
    {
        out << x.first << " ";
    }
    out << "}";
}

/*
 * pretty_print
 *    Purpose: pretty print a tree using the helper function
 * Parameters: node - the node to start printing the tree from, out - the output stream
 *    Returns: none
 *      Notes: the output stream is defaulted to std::cout
 */
void pretty_print(Node *node, std::ostream &out)
{
    pretty_print_helper(node, node->scope, 0, out);
    out << "\n";
}

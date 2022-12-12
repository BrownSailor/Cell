#include "include/error.h"

// check for unbound variables, types, or functions
bool check_bound(Node *root, const Scope &scope)
{
    if (root == nullptr) return true;

    if (root->type == Node::NODE_VAR)
    {
        if (!scope.count(root->token.data) && !types.count(root->token.data))
        {
            print_error("unknown identifier `" + root->token.data + "`", root->token);
            return false;
        }
    }

    for (auto it = root->children.begin(); it != root->children.end(); std::advance(it, 1))
    {
        switch ((*it)->type)
        {
            case Node::NODE_TYPE_DEF:
            case Node::NODE_FUNC_DEC:
            {
                return check_bound(*it, (*it)->scope);
            }

            default:
            {
                switch ((*it)->token.type)
                {
                    case Token::TOK_IF:
                    case Token::TOK_LOOP:
                    {
                        return check_bound(*it, (*it)->scope);
                    }

                    default: return check_bound(*it, scope);
                }
            }
        }
    }

    return true;
}

bool check_errors(Node *root, const Scope &scope)
{
    return check_bound(root, scope) && check_types(root, scope);
}

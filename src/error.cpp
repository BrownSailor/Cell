#include "include/error.h"

// true if no errors, false if error exists
bool state = true;

// check for unbound variables, types, or functions
void check_bound(Node *root, const Scope &scope)
{
    if (root == nullptr) return;

    if (root->type == Node::NODE_VAR)
    {
        if (!scope.count(root->token.data) && !types.count(root->token.data))
        {
            print_error("unknown identifier `" + root->token.data + "`", root->token);
            state = false;
        }
    }

    for (auto it = root->children.begin(); it != root->children.end(); std::advance(it, 1))
    {
        switch ((*it)->type)
        {
            case Node::NODE_TYPE_DEF:
            case Node::NODE_FUNC_DEC:
            {
                check_bound(*it, (*it)->scope);
                break;
            }

            default:
            {
                switch ((*it)->token.type)
                {
                    case Token::TOK_IF:
                    case Token::TOK_LOOP:
                    {
                        check_bound(*it, (*it)->scope);
                        break;
                    }

                    default: check_bound(*it, scope); break;
                }
            }
        }
    }
}

std::unordered_set<std::string> PREDEFINED_TYPES = {
    "bool",
    "char",
    "int",
    "uint"
};

std::string node_to_type(Node *node)
{
    if (node->arr_dim == 0)
    {
        switch (node->token.type)
        {
            case Token::KEY_BOOL:
            {
                return "bool";
            }
            case Token::KEY_CHAR:
            {
                return "char";
            }
            case Token::KEY_INT:
            {
                return "int";
            }
            case Token::KEY_UINT:
            {
                return "uint";
            }

            default: return node->token.data;
        }
    }

    Node *tmp = new_node(node->token);
    std::string type = "";
    for (size_t i = 0; i < node->arr_dim; i++)
    {
        type += '[';
    }
    type += node_to_type(tmp);
    for (size_t i = 0; i < node->arr_dim; i++)
    {
        type += ']';
    }

    return type;
}

/*
 * get_type
 *    Purpose: evaluate a node to a specific type
 * Parameters: node - the node to evaluate, scope - the scope to reference
 *    Returns: a string describing the type of the node's evaluation
 */
std::string get_type(Node *node, const Scope &scope)
{
    switch (node->type)
    {
        case Node::NODE_LIT:
        {
            switch (node->token.type)
            {
                case Token::TOK_CHAR:
                {
                    return "char";
                }
                case Token::TOK_INT:
                {
                    return "int";
                }
                case Token::TOK_STR:
                {
                    return "[char]";
                }

                default: return "void";
            }
        }
        case Node::NODE_KEY:
        {
            switch (node->token.type)
            {
                case Token::KEY_TRU:
                case Token::KEY_FLS:
                {
                    return "bool";
                }
                case Token::KEY_NEW:
                {
                    return node->children.front()->token.data;
                }

                default: return "keyword";
            }
        }
        case Node::NODE_OP:
        {
            switch (node->token.type)
            {
                case Token::TOK_EQEQ:
                case Token::TOK_NEQ:
                case Token::TOK_LT:
                case Token::TOK_LTE:
                case Token::TOK_GT:
                case Token::TOK_GTE:
                case Token::TOK_BANG:
                case Token::TOK_LAND:
                case Token::TOK_LOR:
                {
                    return "bool";
                }

                // TODO: fix this lol
                default: return get_type(node->children.front(), scope);
            }
        }
        case Node::NODE_VAR:
        case Node::NODE_VAR_ASN:
        case Node::NODE_VAR_DEC:
        case Node::NODE_VAR_DEC_ASN:
        {
            if (scope.count(node->token.data))
            {
                return node_to_type(scope.at(node->token.data)->children.front());
            }
            else if (types.count(node->token.data))
            {
                return get_type(types.at(node->token.data)->children.front(), types);
            }
            return "void";
        }
        case Node::NODE_TYPE_DEF:
        {
            return node->token.data;
        }
        case Node::NODE_FUNC_CALL:
        {
            auto it = functions.at(node->token.data)->children.begin();
            std::advance(it, 1);
            return node_to_type(*it);
        }
        case Node::NODE_FUNC_DEC:
        {
            auto it = node->children.begin();
            std::advance(it, 1);

            if (PREDEFINED_TYPES.contains((*it)->token.data))
            {
                return (*it)->token.data;
            }
            return get_type(*it, scope);
        }

        default: return "void";
    }
}

void check_op_types(Node *root, const Scope &scope)
{
    switch (root->token.type)
    {
        // unary
        case Token::TOK_BANG:
        {
            std::string type = get_type(root->children.front(), scope);
            if (type != "bool")
            {
                print_error("expected type `bool`, found type `" + type + "` instead", root->children.front()->token);
                state = false;
            }
            break;
        }
        case Token::TOK_TILDA:
        {
            std::string type = get_type(root->children.front(), scope);
            if (type != "uint" && type != "int" && type != "bool")
            {
                print_error("expected type `bool`, `int`, or `uint`, found type `" + type + "` instead", root->children.front()->token);
                state = false;
            }
            break;
        }
        case Token::TOK_INC:
        case Token::TOK_DEC:
        case Token::TOK_POST_INC:
        case Token::TOK_POST_DEC:
        {
            std::string type = get_type(root->children.front(), scope);
            if (type != "char" && type != "int" && type != "uint")
            {
                print_error("expected type `char`, `int`, or `uint`, found type `" + type + "` instead", root->children.front()->token);
                state = false;
            }
            break;
        }

        // binary
        case Token::TOK_PLUS:
        {
            std::string type1 = get_type(root->children.front(), scope);
            std::string type2 = get_type(root->children.back(), scope);

            if (type1 != "int" && type1 != "uint" && type1 != "[char]")
            {
                print_error("expected type `int`, `uint`, or `[char]`, found `" + type1 + "` instead", root->children.front()->token);
                state = false;

                if (type2 != "char" && type2 != "int" && type2 != "uint" && type2 != "[char]")
                {
                    print_error("expected type `char`, `int`, `uint`, or `[char]`, found `" + type2 + "` instead", root->children.back()->token);
                    state = false;
                }
            }

            if (type1 == "int" || type1 == "uint")
            {
                if (type2 != "int" && type2 != "uint")
                {
                    print_error("expected type `int` or `uint`, found `" + type2 + "` instead", root->children.back()->token);
                    state = false;
                }
            }
            else if (type1 == "[char]")
            {
                if (type2 != "char" && type2 != "[char]")
                {
                    print_error("expected type `char` or `[char]`, found `" + type2 + "` instead", root->children.back()->token);
                    state = false;
                }
            }
            break;
        }
        case Token::TOK_MINUS:
        case Token::TOK_STAR:
        case Token::TOK_SLASH:
        case Token::TOK_PERCENT:
        case Token::TOK_SHL:
        case Token::TOK_SHR:
        {
            std::string type1 = get_type(root->children.front(), scope);
            std::string type2 = get_type(root->children.back(), scope);

            if (type1 != "int" && type1 != "uint")
            {
                print_error("expected type `int` or `uint`, found `" + type1 + "` instead", root->children.front()->token);
                state = false;
            }

            if (type2 != "int" && type2 != "uint")
            {
                print_error("expected type `int` or `uint`, found `" + type2 + "` instead", root->children.back()->token);
            }
            break;
        }
        case Token::TOK_EQEQ:
        case Token::TOK_NEQ:
        case Token::TOK_LT:
        case Token::TOK_LTE:
        case Token::TOK_GT:
        case Token::TOK_GTE:
        {
            std::string type1 = get_type(root->children.front(), scope);
            std::string type2 = get_type(root->children.back(), scope);
            if (type1 != type2)
            {
                print_error("expected matching types, found type `" + type1 + "` and `" + type2 + "` instead", root->children.front()->token);
                state = false;
            }
            break;
        }
        case Token::TOK_LOR:
        case Token::TOK_LAND:
        {
            std::string type1 = get_type(root->children.front(), scope);
            std::string type2 = get_type(root->children.back(), scope);

            if (type1 != "bool")
            {
                print_error("expected type `bool`, found type `" + type1 + "` instead", root->children.front()->token);
                state = false;
            }

            if (type2 != "bool")
            {
                print_error("expected type `bool`, found type `" + type2 + "` instead", root->children.back()->token);
                state = false;
            }
            break;
        }

        default: break;
    }
}

void check_var_types(Node *root, const Scope &scope)
{
    switch (root->type)
    {
        case Node::NODE_VAR_ASN:
        {
            std::string type1 = get_type(scope.at(root->token.data), scope);
            std::string type2 = get_type(root->children.front(), scope);

            if (type1 != type2)
            {
                print_error("expected type `" + type1 + "`, found type `" + type2 + "` instead", root->children.front()->token);
                state = false;
            }

            break;
        }
        case Node::NODE_VAR_DEC_ASN:
        {
            std::string type1 = node_to_type(root->children.front());
            std::string type2 = get_type(root->children.back(), scope);

            if (type1 != type2)
            {
                print_error("expected type `" + type1 + "`, found type `" + type2 + "` instead", root->children.front()->token);
                state = false;
            }
            break;
        }

        default: break;
    }
}

void check_func_call_types(Node *root, const Scope &scope)
{
    Node *args = functions.at(root->token.data)->children.front();

    if (args->children.size() != root->children.size())
    {
        print_error("expected " + std::to_string(args->children.size()) + " arguments for function, found " + std::to_string(root->children.size()) + " arguments instead", root->token);
        state = false;
    }

    // check args
    for (auto it1 = args->children.begin(), it2 = root->children.begin(); it1 != args->children.end() && it2 != root->children.end(); std::advance(it1, 1), std::advance(it2, 1))
    {
        std::string type1 = node_to_type((*it1)->children.front());
        std::string type2 = get_type(*it2, scope);

        if (type1 != type2)
        {
            print_error("expected type `" + type1 + "`, found type `" + type2 + "` instead", (*it2)->token);
            state = false;
        }
    }
}

void check_types(Node *root, const Scope &scope)
{
    if (root == nullptr) return;

    switch (root->type)
    {
        case Node::NODE_OP:
        {
            check_op_types(root, scope);
            break;
        }
        case Node::NODE_VAR_ASN:
        case Node::NODE_VAR_DEC_ASN:
        {
            check_var_types(root, scope);
            break;
        }
        case Node::NODE_FUNC_CALL:
        {
            check_func_call_types(root, scope);
            break;
        }

        default: break;
    }

    for (auto it = root->children.begin(); it != root->children.end(); std::advance(it, 1))
    {
        switch ((*it)->type)
        {
            case Node::NODE_TYPE_DEF:
            case Node::NODE_FUNC_DEC:
            {
                check_types(*it, (*it)->scope);
                break;
            }

            default:
            {
                switch ((*it)->token.type)
                {
                    case Token::TOK_IF:
                    case Token::TOK_LOOP:
                    {
                        check_types(*it, (*it)->scope);
                        break;
                    }

                    default: check_types(*it, scope); break;
                }
            }
        }
    }
}

bool check_errors(Node *root, const Scope &scope)
{
    check_bound(root, scope);
    check_types(root, scope);

    return state;
}

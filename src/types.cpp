#include "include/types.h"

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
 * set_types
 *    Purpose: evaluate all node to a specific type
 * Parameters: node - the node to evaluate, scope - the scope to reference
 *    Returns: the evaluated node
 */
Node *set_types(Node *node, const Scope &scope)
{
    if (node == nullptr) return nullptr;

    for (auto it = node->children.begin(); it != node->children.end(); std::advance(it, 1))
    {
        switch ((*it)->type)
        {
            case Node::NODE_TYPE_DEF:
            case Node::NODE_FUNC_DEC:
            {
                *it = set_types(*it, (*it)->scope);
                break;
            }

            default:
            {
                switch ((*it)->token.type)
                {
                    case Token::TOK_IF:
                    case Token::TOK_LOOP:
                    {
                        *it = set_types(*it, (*it)->scope);
                        break;
                    }

                    default: *it = set_types(*it, scope); break;
                }
            }
        }
    }

    switch (node->type)
    {
        case Node::NODE_LIT:
        {
            switch (node->token.type)
            {
                case Token::TOK_CHAR:
                {
                    node->datatype = "char";
                    return node;
                }
                case Token::TOK_INT:
                {
                    node->datatype = "int";
                    return node;
                }
                case Token::TOK_STR:
                {
                    node->datatype = "str";
                    return node;
                }

                default: return node;
            }
        }
        case Node::NODE_KEY:
        {
            switch (node->token.type)
            {
                case Token::KEY_TRU:
                case Token::KEY_FLS:
                {
                    node->datatype = "bool";
                    return node;
                }
                case Token::KEY_NEW:
                {
                    set_types(node->children.front(), scope);
                    node->datatype = node->children.front()->token.data;
                    return node;
                }

                default: node->datatype = "keyword"; return node;
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
                    node->datatype = "bool";
                    return node;
                }

                // TODO: fix this lol
                default: return node;
            }
        }
        case Node::NODE_VAR:
        case Node::NODE_VAR_ASN:
        case Node::NODE_VAR_DEC:
        case Node::NODE_VAR_DEC_ASN:
        {
            if (scope.count(node->token.data))
            {
                node->datatype = scope.at(node->token.data)->datatype;
            }
            else if (types.count(node->token.data))
            {
                node->datatype = types.at(node->token.data)->datatype;
            }

            return node;
        }
        case Node::NODE_TYPE_DEF:
        {
            node->datatype = node->token.data;
            return node;
        }
        case Node::NODE_FUNC_CALL:
        {
            auto it = functions.at(node->token.data)->children.begin();
            std::advance(it, 1);
            node->datatype = node_to_type(*it);
            return node;
        }
        case Node::NODE_FUNC_DEC:
        {
            auto it = node->children.begin();
            std::advance(it, 1);

            if (BUILTIN_TYPES.count((*it)->token.data))
            {
                node->datatype = (*it)->token.data;
                return node;
            }
            set_types(*it, scope);
            node->datatype = (*it)->datatype;
            return node;
        }

        default: return node;
    }

    return node;
}

bool check_op_types(Node *root, const Scope &scope)
{
    (void)(scope);
    switch (root->token.type)
    {
        // unary
        case Token::TOK_BANG:
        {
            std::string type = root->children.front()->datatype;
            if (type != "bool")
            {
                print_error("expected type `bool`, found type `" + type + "` instead", root->children.front()->token);
                return false;
            }
            break;
        }
        case Token::TOK_TILDA:
        {
            std::string type = root->children.front()->datatype;
            if (type != "uint" && type != "int" && type != "bool")
            {
                print_error("expected type `bool`, `int`, or `uint`, found type `" + type + "` instead", root->children.front()->token);
                return false;
            }
            break;
        }
        case Token::TOK_INC:
        case Token::TOK_DEC:
        case Token::TOK_POST_INC:
        case Token::TOK_POST_DEC:
        {
            std::string type = root->children.front()->datatype;
            if (type != "char" && type != "int" && type != "uint")
            {
                print_error("expected type `char`, `int`, or `uint`, found type `" + type + "` instead", root->children.front()->token);
                return false;
            }
            break;
        }

        // binary
        case Token::TOK_PLUS:
        {
            std::string type1 = root->children.front()->datatype;
            std::string type2 = root->children.back()->datatype;

            if (type1 != "int" && type1 != "uint" && type1 != "[char]")
            {
                print_error("expected type `int`, `uint`, or `[char]`, found `" + type1 + "` instead", root->children.front()->token);
                return false;

                if (type2 != "char" && type2 != "int" && type2 != "uint" && type2 != "[char]")
                {
                    print_error("expected type `char`, `int`, `uint`, or `[char]`, found `" + type2 + "` instead", root->children.back()->token);
                    return false;
                }
            }

            if (type1 == "int" || type1 == "uint")
            {
                if (type2 != "int" && type2 != "uint")
                {
                    print_error("expected type `int` or `uint`, found `" + type2 + "` instead", root->children.back()->token);
                    return false;
                }
            }
            else if (type1 == "[char]")
            {
                if (type2 != "char" && type2 != "[char]")
                {
                    print_error("expected type `char` or `[char]`, found `" + type2 + "` instead", root->children.back()->token);
                    return false;
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
            std::string type1 = root->children.front()->datatype;
            std::string type2 = root->children.back()->datatype;

            if (type1 != "int" && type1 != "uint")
            {
                print_error("expected type `int` or `uint`, found `" + type1 + "` instead", root->children.front()->token);
                return false;
            }

            if (type2 != "int" && type2 != "uint")
            {
                print_error("expected type `int` or `uint`, found `" + type2 + "` instead", root->children.back()->token);
                return false;
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
            std::string type1 = root->children.front()->datatype;
            std::string type2 = root->children.back()->datatype;
            if (type1 != type2)
            {
                print_error("expected matching types, found type `" + type1 + "` and `" + type2 + "` instead", root->children.front()->token);
                return false;
            }
            break;
        }
        case Token::TOK_LOR:
        case Token::TOK_LAND:
        {
            std::string type1 = root->children.front()->datatype;
            std::string type2 = root->children.back()->datatype;

            if (type1 != "bool")
            {
                print_error("expected type `bool`, found type `" + type1 + "` instead", root->children.front()->token);
                return false;
            }

            if (type2 != "bool")
            {
                print_error("expected type `bool`, found type `" + type2 + "` instead", root->children.back()->token);
                return false;
            }
            break;
        }

        default: return true;
    }

    return true;
}

bool check_var_types(Node *root, const Scope &scope)
{
    switch (root->type)
    {
        case Node::NODE_VAR_ASN:
        {
            std::string type1 = scope.at(root->token.data)->datatype;
            std::string type2 = root->children.front()->datatype;

            if (type1 != type2)
            {
                print_error("expected type `" + type1 + "`, found type `" + type2 + "` instead", root->children.front()->token);
                return false;
            }

            break;
        }
        case Node::NODE_VAR_DEC_ASN:
        {
            std::string type1 = root->children.front()->datatype;
            std::string type2 = root->children.back()->datatype;

            if (type1 != type2)
            {
                print_error("expected type `" + type1 + "`, found type `" + type2 + "` instead", root->children.front()->token);
                return false;
            }
            break;
        }

        default: return true;
    }

    return true;
}

bool check_func_call_types(Node *root, const Scope &scope)
{
    (void)(scope);
    Node *args = functions.at(root->token.data)->children.front();

    if (args->children.size() != root->children.size())
    {
        print_error("expected " + std::to_string(args->children.size()) + " arguments for function, found " + std::to_string(root->children.size()) + " arguments instead", root->token);
        return false;
    }

    // check args
    for (auto it1 = args->children.begin(), it2 = root->children.begin(); it1 != args->children.end() && it2 != root->children.end(); std::advance(it1, 1), std::advance(it2, 1))
    {
        std::string type1 = (*it1)->children.front()->datatype;
        std::string type2 = (*it2)->children.front()->datatype;

        if (type1 != type2)
        {
            print_error("expected type `" + type1 + "`, found type `" + type2 + "` instead", (*it2)->token);
            return false;
        }
    }

    return true;
}

bool check_types(Node *root, const Scope &scope)
{
    if (root == nullptr) return true;

    switch (root->type)
    {
        case Node::NODE_OP:
        {
            return check_op_types(root, scope);
            break;
        }
        case Node::NODE_VAR_ASN:
        case Node::NODE_VAR_DEC_ASN:
        {
            return check_var_types(root, scope);
            break;
        }
        case Node::NODE_FUNC_CALL:
        {
            return check_func_call_types(root, scope);
            break;
        }

        default: return true;
    }

    for (auto it = root->children.begin(); it != root->children.end(); std::advance(it, 1))
    {
        switch ((*it)->type)
        {
            case Node::NODE_TYPE_DEF:
            case Node::NODE_FUNC_DEC:
            {
                return check_types(*it, (*it)->scope);
            }

            default:
            {
                switch ((*it)->token.type)
                {
                    case Token::TOK_IF:
                    case Token::TOK_LOOP:
                    {
                        return check_types(*it, (*it)->scope);
                    }

                    default: return check_types(*it, scope);
                }
            }
        }
    }

    return true;
}
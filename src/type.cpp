#include "include/type.h"

bool state = true;

std::unordered_map<Token::Type, std::string> REPR_TYPE = {
    { Token::TOK_BOOL, "bool" },
    { Token::TOK_CHAR, "char" },
    { Token::TOK_INT, "int" },
    { Token::TOK_UINT, "uint" },
    { Token::TOK_STR, "[char]" },
    { Token::KEY_BOOL, "keyword" },
    { Token::KEY_CHAR, "keyword" }, 
    { Token::KEY_INT, "keyword" },
    { Token::KEY_UINT, "keyword" }
};

void print_type_error(Token::Type lhs, Token::Type rhs, const Token &loc, size_t arr_size)
{
    std::string r = "";
    for (size_t i = 0; i < arr_size; i++)
    {
        r += "[";
    }
    r += REPR_TYPE[rhs];
    for (size_t i = 0; i < arr_size; i++)
    {
        r += "]";
    }

    print_error("Expected type `" + REPR_TYPE[lhs] + "` but found type `" + r + "` instead.", loc);
}

size_t tree_height(Node *root)
{
    Node *curr = root;
    size_t height = 0;

    while (curr->children.size())
    {
        curr = curr->children.front();
        height++;
    }

    return std::max(height - 1, 0UL);
}

void type_checker(Node *root, const Scope &scope)
{
    if (root == nullptr) return;

    if (root->arr_dim == 0)
    {
        switch (root->type)
        {
            case Node::NODE_VAR_ASN:
            case Node::NODE_VAR_DEC_ASN:
            {
                Token::Type lhs = eval_node(root, scope);
                Token::Type rhs = eval_node(root->children.back(), scope);

                if (root->children.back()->token.type == Token::TOK_ARR)
                {
                    print_type_error(lhs, rhs, root->children.back()->token, tree_height(root->children.back()));
                    state = false;
                }
                else if (lhs != rhs)
                {
                    print_type_error(lhs, rhs, root->children.back()->token);
                    state = false;
                }
                break;
            }

            default: break;
        }
    }

    for (auto it = root->children.begin(); it != root->children.end(); std::advance(it, 1))
    {
        switch ((*it)->token.type)
        {
            case Token::KEY_STRUCT:
            case Token::TOK_FUNC:
            case Token::TOK_IF:
            case Token::TOK_ELIF:
            case Token::TOK_ELSE:
            case Token::TOK_LOOP:
            {
                type_check(*it, (*it)->scope);
                break;
            }

            default: type_check(*it, scope); break;
        }
    }
}

bool type_check(Node *root, const Scope &scope)
{
    type_checker(root, scope);
    return state;
}
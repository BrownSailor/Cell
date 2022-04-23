#include "include/visitor.h"

/*
 * visit_expr
 *    Purpose: visit each node in the tree and simplify possible expressions
 * Parameters: root - the root of the tree to visit
 *    Returns: the simplified expression tree
 */
Node *visit_expr(Node *root)
{
    for (auto it = root->children.begin(); it != root->children.end(); std::advance(it, 1))
    {
        *it = visit_expr(*it);
    }

    if (root == nullptr)
    {
        return nullptr;
    }
    else if ((root->token.type == Token::TOK_MINUS ||
              root->token.type == Token::TOK_TILDA ||
              root->token.type == Token::TOK_BANG) && 
              root->children.front()->token.type == Token::TOK_NUM &&
              root->children.size() == 1)
    {
        int a = std::stoi(root->children.front()->token.data);
        if (root->token.type == Token::TOK_MINUS)
        {
            a = -a;
        }
        else if (root->token.type == Token::TOK_TILDA)
        {
            a = ~a;
        }
        else if (root->token.type == Token::TOK_BANG)
        {
            a = !a;
        }

        Node *new_root = new_node({ .type = Token::TOK_NUM, .data = std::to_string(a) });

        return new_root;
    }
    else if ((root->token.type == Token::TOK_PLUS ||
        root->token.type == Token::TOK_MINUS ||
        root->token.type == Token::TOK_STAR ||
        root->token.type == Token::TOK_SLASH ||
        root->token.type == Token::TOK_PERCENT ||
        root->token.type == Token::TOK_SHL ||
        root->token.type == Token::TOK_SHR ||
        root->token.type == Token::TOK_LAND ||
        root->token.type == Token::TOK_LOR ||
        root->token.type == Token::TOK_EQEQ ||
        root->token.type == Token::TOK_NEQ ||
        root->token.type == Token::TOK_LT ||
        root->token.type == Token::TOK_GT ||
        root->token.type == Token::TOK_LTE ||
        root->token.type == Token::TOK_GTE) &&
        root->children.front()->token.type == Token::TOK_NUM &&
        root->children.back()->token.type == Token::TOK_NUM)
    {
        Node *new_root = new_node({ .type = Token::TOK_NUM });

        int a = std::stoi(root->children.front()->token.data);
        int b = std::stoi(root->children.back()->token.data);

        if (root->token.type == Token::TOK_PLUS)
        {
            new_root->token.data = std::to_string(a + b);
        }
        else if (root->token.type == Token::TOK_MINUS)
        {
            new_root->token.data = std::to_string(a - b);
        }
        else if (root->token.type == Token::TOK_STAR)
        {
            new_root->token.data = std::to_string(a * b);
        }
        else if (root->token.type == Token::TOK_SLASH)
        {
            new_root->token.data = std::to_string(a / b);
        }
        else if (root->token.type == Token::TOK_PERCENT)
        {
            new_root->token.data = std::to_string(a % b);
        }
        else if (root->token.type == Token::TOK_SHL)
        {
            new_root->token.data = std::to_string(a << b);
        }
        else if (root->token.type == Token::TOK_SHR)
        {
            new_root->token.data = std::to_string(a >> b);
        }
        else if (root->token.type == Token::TOK_LAND)
        {
            new_root->token.data = std::to_string(a && b);
        }
        else if (root->token.type == Token::TOK_LOR)
        {
            new_root->token.data = std::to_string(a || b);
        }
        else if (root->token.type == Token::TOK_EQEQ)
        {
            new_root->token.data = std::to_string(a == b);
        }
        else if (root->token.type == Token::TOK_NEQ)
        {
            new_root->token.data = std::to_string(a != b);
        }
        else if (root->token.type == Token::TOK_LT)
        {
            new_root->token.data = std::to_string(a < b);
        }
        else if (root->token.type == Token::TOK_GT)
        {
            new_root->token.data = std::to_string(a > b);
        }
        else if (root->token.type == Token::TOK_LTE)
        {
            new_root->token.data = std::to_string(a <= b);
        }
        else if (root->token.type == Token::TOK_GTE)
        {
            new_root->token.data = std::to_string(a >= b);
        }

        return new_root;
    }

    return root;
}

/*
 * visit_type
 *    Purpose: visit each node in the tree and assign a type to each expression
 * Parameters: root - the root of the tree to visit
 *    Returns: the expression tree with types assigned
 */
// Node *visit_type(Node *root)
// {
//     for (auto it = root->children.begin(); it != root->children.end(); std::advance(it, 1))
//     {
//         *it = visit_expr(*it);
//     }

//     if (root == nullptr)
//     {
//         return nullptr;
//     }
//     else if (root->token.type == Token::TOK_ID)
//     {
//         if (root->scope)
//     }

//     return root;
// }
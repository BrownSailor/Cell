#include "parser.h"

static Node *parse_expr(std::list<Token> &tokens, Scope &scope);
static Node *parse_statement(std::list<Token> &tokens, Scope &scope);

static int eat_open_parens(std::list<Token> &tokens, Token::Type paren_type)
{
    int parens = 0;

    while (tokens.size() && tokens.front().type == paren_type)
    {
        parens++;
        tokens.pop_front();
    }

    return parens;
}

static void eat_close_parens(std::list<Token> &tokens, int num, Token::Type paren_type)
{
    while (num && tokens.size() && tokens.front().type == paren_type)
    {
        tokens.pop_front();
        num--;
    }

    if (num)
    {
        std::cerr << "Parentheses issue!\n";
        exit(EXIT_FAILURE);
    }
}

static Node *new_node(Token token)
{
    Node *node = new Node;
    node->token = token;

    return node;
}

static Node *unary(Node *op, Node *operand)
{
    op->children.push_back(operand);
    op->type = Node::NODE_OP;
    return op;
}

static Node *binary(Node *left, Node *op, Node *right)
{
    op->children.push_back(left);
    op->children.push_back(right);
    op->type = Node::NODE_OP;
    return op;
}

static Node *parse_fact(std::list<Token> &tokens, Scope &scope)
{
    Node *node = nullptr;

    switch (tokens.front().type)
    {
        case Token::TOK_NUM:
        case Token::TOK_TRU:
        case Token::TOK_FLS:
        case Token::TOK_STR:
        {
            node = new_node(tokens.front());
            node->type = Node::NODE_LIT;
            tokens.pop_front();
            break;
        }

        case Token::KEY_NUM:
        case Token::KEY_BOOL:
        case Token::KEY_STR:
        case Token::KEY_NIL:
        {
            node = new_node(tokens.front());
            node->type = Node::NODE_KEY;
            tokens.pop_front();
            break;
        }

        case Token::TOK_IN:
        {
            node = new_node(tokens.front());
            node->type = Node::NODE_KEY;
            tokens.pop_front();
            break;
        }

        case Token::TOK_OUT:
        {
            node = new_node(tokens.front());
            node->type = Node::NODE_KEY;
            tokens.pop_front();
            break;
        }

        case Token::TOK_LPAREN:
        {
            int p = eat_open_parens(tokens, Token::TOK_LPAREN);
            node = parse_expr(tokens, scope);
            eat_close_parens(tokens, p, Token::TOK_RPAREN);
            break;
        }

        case Token::TOK_NOT:
        case Token::TOK_SUB:
        {
            Node *op = new_node(tokens.front());
            tokens.pop_front();
            node = unary(op, parse_expr(tokens, scope));
            break;
        }

        case Token::TOK_ID:
        {
            node = new_node(tokens.front());
            tokens.pop_front();
            node->type = Node::NODE_VAR;
            break;
        }

        case Token::KEY_PRINT:
        case Token::KEY_PRINTLN:
        {
            node = new_node(tokens.front());
            tokens.pop_front();
            node->type = Node::NODE_FUN_CALL;
            node->children.push_back(parse_expr(tokens, scope));
            break;
        }

        default:
        {
            break;
        }
    }

    return node;
}

static Node *parse_term(std::list<Token> &tokens, Scope &scope)
{
    Node *node = parse_fact(tokens, scope);

    while (tokens.front().type == Token::TOK_MUL ||
           tokens.front().type == Token::TOK_DIV ||
           tokens.front().type == Token::TOK_MOD ||
           tokens.front().type == Token::TOK_SHL ||
           tokens.front().type == Token::TOK_SHR)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_fact(tokens, scope));
    }

    return node;
}

static Node *parse_add_sub(std::list<Token> &tokens, Scope &scope)
{
    Node *node = parse_term(tokens, scope);

    while (tokens.front().type == Token::TOK_ADD ||
           tokens.front().type == Token::TOK_SUB)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_term(tokens, scope));
    }

    return node;
}

static Node *parse_lt_gt(std::list<Token> &tokens, Scope &scope)
{
    Node *node = parse_add_sub(tokens, scope);

    while (tokens.front().type == Token::TOK_LT ||
           tokens.front().type == Token::TOK_GT ||
           tokens.front().type == Token::TOK_LTE ||
           tokens.front().type == Token::TOK_GTE)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_add_sub(tokens, scope));
    }

    return node;
}

static Node *parse_eq_neq(std::list<Token> &tokens, Scope &scope)
{
    Node *node = parse_lt_gt(tokens, scope);

    while (tokens.front().type == Token::TOK_EQ ||
           tokens.front().type == Token::TOK_NEQ)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_lt_gt(tokens, scope));
    }

    return node;
}

static Node *parse_and(std::list<Token> &tokens, Scope &scope)
{
    Node *node = parse_eq_neq(tokens, scope);

    while (tokens.front().type == Token::TOK_AND)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_eq_neq(tokens, scope));
    }

    return node;
}

static Node *parse_or(std::list<Token> &tokens, Scope &scope)
{
    Node *node = parse_and(tokens, scope);

    while (tokens.front().type == Token::TOK_OR)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_and(tokens, scope));
    }

    return node;
}

static Node *parse_in_out(std::list<Token> &tokens, Scope &scope)
{
    Node *node = parse_or(tokens, scope);

    while (tokens.front().type == Token::TOK_READ ||
           tokens.front().type == Token::TOK_WRITE)
    {
        tokens.pop_front();
        node->children.push_back(parse_or(tokens, scope));
    }

    return node;
}

static Node *parse_expr(std::list<Token> &tokens, Scope &scope)
{
    Node *node = parse_in_out(tokens, scope);

    return node;
}

static Node *parse_if(std::list<Token> &tokens, Scope &scope)
{
    Node *node = new_node(tokens.front());
    tokens.pop_front();

    /* eat condition */
    if (node->token.type == Token::TOK_IF || tokens.front().type != Token::TOK_LBRACE)
    {
        node->children.push_back(parse_expr(tokens, scope));
    }

    Node *body = new Node;
    body->type = Node::NODE_FUN_BODY;
    node->children.push_back(body);

    /* eat { */
    tokens.pop_front();
    while (tokens.size() && tokens.front().type != Token::TOK_RBRACE)
    {
        body->children.push_back(parse_statement(tokens, node->scope));
    }

    /* eat } */
    tokens.pop_front();

    if (tokens.front().type == Token::TOK_ELSE)
    {
        Node *els = parse_if(tokens, scope);
        node->children.push_back(els);
    }

    return node;
}

static Node *parse_loop(std::list<Token> &tokens, Scope &scope)
{
    (void)tokens;
    (void)scope;
    return nullptr;
}

static Node *parse_statement(std::list<Token> &tokens, Scope &scope)
{
    Node *node;
    switch (tokens.front().type)
    {
        case Token::TOK_LOOP:
        {
            node = parse_loop(tokens, scope);
            break;
        }
        case Token::TOK_IF:
        {
            node = parse_if(tokens, scope);
            break;
        }
        default:
        {
            node = parse_expr(tokens, scope);
            break;
        }
    }

    return node;
}

static Node *parse_function(std::list<Token> &tokens)
{
    /* eat fn */
    tokens.pop_front();

    Node *node = new_node(tokens.front());
    node->type = Node::NODE_FUN_DEC;
    tokens.pop_front();

    Node *in = new Node, *out = new Node, *body = new Node;
    in->type = Node::NODE_FUN_IN;
    out->type = Node::NODE_FUN_OUT;
    body->type = Node::NODE_FUN_BODY;

    node->children.push_back(in);
    node->children.push_back(out);
    node->children.push_back(body);

    /* eat colon */
    tokens.pop_front();

    /* args */
    while (tokens.front().type != Token::TOK_ARROW)
    {
        in->children.push_back(new_node(tokens.front()));
        tokens.pop_front();
    }

    /* eat arrow */
    tokens.pop_front();

    /* returns */
    while (tokens.front().type != Token::TOK_LBRACE)
    {
        out->children.push_back(new_node(tokens.front()));
        tokens.pop_front();
    }

    /* eat { */
    tokens.pop_front();
    while (tokens.size() && tokens.front().type != Token::TOK_RBRACE)
    {
        body->children.push_back(parse_statement(tokens, node->scope));
    }

    /* eat } */
    tokens.pop_front();
    return node;
}

Node *parse_program(std::list<Token> &tokens)
{
    (void)parse_if;
    (void)parse_loop;

    Node *node = new Node;
    node->type = Node::NODE_PROG;

    while (tokens.size())
    {
        if (tokens.front().type == Token::KEY_FUN)
        {
            node->children.push_back(parse_function(tokens));
        }
        else
        {
            node->children.push_back(parse_statement(tokens, node->scope));
        }
    }

    return node;
}

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
        std::cout << "    ";
    }
    std::cout << "   \u2502\n";

    for (int i = 0; i < num_tabs - 1; i++)
    {
        std::cout << "    ";
    }
    std::cout << "   \u2514";
}

/*
 * pretty_print_helper
 *    Purpose: pretty print a node and its children recursively
 * Parameters: node - the node to start printing from, num_tabs - the number of tabs to print, out - the output stream
 *    Returns: none
 *      Notes: the output stream is defaulted to std::cout
 */
static void pretty_print_helper(Node *node, const Scope &scope, int num_tabs)
{
    switch (node->type)
    {
        case Node::NODE_PROG:
        {
            std::cout << "program\n";
            break;
        }
        case Node::NODE_FUN_IN:
        {
            std::cout << "fn_in\n";
            break;
        }
        case Node::NODE_FUN_OUT:
        {
            std::cout << "fn_out\n";
            break;
        }
        case Node::NODE_FUN_BODY:
        {
            std::cout << "fn_body\n";
            break;
        }
        default:
        {
            std::cout << node->token.data << "\n";
            break;
        }
    }

    for (auto it = node->children.begin(); it != node->children.end(); std::advance(it, 1))
    {
        pretty_print_tabs(num_tabs + 1);
        pretty_print_helper(*it, scope, num_tabs + 1);
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
    Scope scope = node->scope;
    pretty_print_helper(node, scope, 0);
    std::cout << "\n";
}

void free_tree(Node *node)
{
    if (node == nullptr) return;

    for (auto child : node->children)
    {
        free_tree(child);
    }

    delete node;
}

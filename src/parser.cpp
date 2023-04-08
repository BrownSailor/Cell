#include "parser.h"

typedef std::unordered_set<std::string> Scope;

std::unordered_map<std::string, std::unordered_set<Node *>> functions;
std::stack<std::unordered_set<std::string>> scopes;

static Node *parse_expr(std::list<Token> &tokens);
static Node *parse_statement(std::list<Token> &tokens);

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
    std::cout << tokens.front().row << " " << tokens.front().col << " " << tokens.front().data << "\n";
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

static Node *parse_fact(std::list<Token> &tokens)
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
            node = parse_expr(tokens);
            eat_close_parens(tokens, p, Token::TOK_RPAREN);
            break;
        }

        case Token::TOK_NOT:
        case Token::TOK_SUB:
        {
            Node *op = new_node(tokens.front());
            tokens.pop_front();
            node = unary(op, parse_expr(tokens));
            break;
        }

        case Token::TOK_ID:
        {
            std::cout << tokens.front().data << "\n";
            node = new_node(tokens.front());
            tokens.pop_front();
            node->type = Node::NODE_VAR;
            break;
        }

        default:
        {
            break;
        }
    }

    return node;
}

static Node *parse_term(std::list<Token> &tokens)
{
    Node *node = parse_fact(tokens);

    while (tokens.front().type == Token::TOK_MUL ||
           tokens.front().type == Token::TOK_DIV ||
           tokens.front().type == Token::TOK_MOD ||
           tokens.front().type == Token::TOK_SHL ||
           tokens.front().type == Token::TOK_SHR)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_fact(tokens));
    }

    return node;
}

static Node *parse_add_sub(std::list<Token> &tokens)
{
    Node *node = parse_term(tokens);

    while (tokens.front().type == Token::TOK_ADD ||
           tokens.front().type == Token::TOK_SUB)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_term(tokens));
    }

    return node;
}

static Node *parse_lt_gt(std::list<Token> &tokens)
{
    Node *node = parse_add_sub(tokens);

    while (tokens.front().type == Token::TOK_LT ||
           tokens.front().type == Token::TOK_GT ||
           tokens.front().type == Token::TOK_LTE ||
           tokens.front().type == Token::TOK_GTE)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_add_sub(tokens));
    }

    return node;
}

static Node *parse_eq_neq(std::list<Token> &tokens)
{
    Node *node = parse_lt_gt(tokens);

    while (tokens.front().type == Token::TOK_EQ ||
           tokens.front().type == Token::TOK_NEQ)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_lt_gt(tokens));
    }

    return node;
}

static Node *parse_and(std::list<Token> &tokens)
{
    Node *node = parse_eq_neq(tokens);

    while (tokens.front().type == Token::TOK_AND)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_eq_neq(tokens));
    }

    return node;
}

static Node *parse_or(std::list<Token> &tokens)
{
    Node *node = parse_and(tokens);

    while (tokens.front().type == Token::TOK_OR)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_and(tokens));
    }

    return node;
}

static Node *parse_in_out(std::list<Token> &tokens)
{
    Node *node = parse_or(tokens);

    while (tokens.front().type == Token::TOK_READ ||
           tokens.front().type == Token::TOK_WRITE)
    {
        tokens.pop_front();
        node->children.push_back(parse_or(tokens));
    }

    return node;
}

static Node *parse_expr(std::list<Token> &tokens)
{
    Node *node = parse_in_out(tokens);

    if (functions.count(node->token.data))
    {
        node->type = Node::NODE_FUN_CALL;
        // Node *in = functions[node->token.data]->children.front();
        // if (in->children.size() > 1 || in->children.front()->token.type != Token::KEY_NIL)
        // {
        //     for (size_t i = 0; i < in->children.size(); i++)
        //     {
        //         node->children.push_back(parse_expr(tokens));
        //     }
        // }

        node->children.push_back(parse_expr(tokens));

        return node;
    }

    /* TODO: parse variable assignment syntax */
    if (tokens.front().type == Token::TOK_COLON)
    {
        node->type = Node::NODE_VAR_ASN;
        tokens.pop_front();
        node->children.push_back(parse_expr(tokens));
        scopes.top().insert(node->token.data);
    }
    else
    {
        if (scopes.top().count(node->token.data))
        {
            node->type = Node::NODE_VAR;
        }

        /* throw an error if we have a variable identifier that is not within scope */
    }

    return node;
}

static Node *parse_if(std::list<Token> &tokens)
{
    Node *node = new_node(tokens.front());
    node->type = Node::NODE_IF;
    tokens.pop_front();

    /* eat condition */
    if (node->token.type == Token::TOK_IF || tokens.front().type != Token::TOK_LBRACE)
    {
        node->children.push_back(parse_expr(tokens));
    }

    Node *body = new Node;
    body->type = Node::NODE_BODY;
    node->children.push_back(body);

    /* eat { */
    tokens.pop_front();
    scopes.push(Scope());
    while (tokens.size() && tokens.front().type != Token::TOK_RBRACE)
    {
        body->children.push_back(parse_statement(tokens));
    }

    /* eat } */
    scopes.pop();
    tokens.pop_front();

    if (tokens.front().type == Token::TOK_ELSE)
    {
        Node *els = parse_if(tokens);
        node->children.push_back(els);
    }

    return node;
}

static Node *parse_loop(std::list<Token> &tokens)
{
    /* TODO: parse loop syntax */
    (void)tokens;
    return nullptr;
}

static Node *parse_statement(std::list<Token> &tokens)
{
    Node *node;
    switch (tokens.front().type)
    {
        case Token::TOK_LOOP:
        {
            node = parse_loop(tokens);
            break;
        }
        case Token::TOK_IF:
        {
            node = parse_if(tokens);
            break;
        }
        default:
        {
            node = parse_expr(tokens);
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
    functions[tokens.front().data].insert(node);

    node->type = Node::NODE_FUN_DEC;
    tokens.pop_front();

    Node *in = new Node, *out = new Node, *body = new Node;
    in->type = Node::NODE_FUN_IN;
    out->type = Node::NODE_FUN_OUT;
    body->type = Node::NODE_BODY;

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
    scopes.push(Scope());
    while (tokens.size() && tokens.front().type != Token::TOK_RBRACE)
    {
        body->children.push_back(parse_statement(tokens));
    }

    /* eat } */
    scopes.pop();
    tokens.pop_front();
    return node;
}

Node *parse_program(std::list<Token> &tokens)
{
    (void)parse_loop;

    Node *node = new Node;
    node->type = Node::NODE_PROG;

    scopes.push(Scope());
    while (tokens.size())
    {
        if (tokens.front().type == Token::KEY_FUN)
        {
            node->children.push_back(parse_function(tokens));
        }
        else
        {
            node->children.push_back(parse_statement(tokens));
        }
    }

    return node;
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

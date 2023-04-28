#include "parser.hpp"

typedef std::unordered_set<std::string> Scope;
std::stack<Scope> scopes;

std::unordered_map<std::string, std::unordered_set<Node *>> functions;

static Node *parse_expr(std::list<Token> &tokens);
static Node *parse_statement(std::list<Token> &tokens);

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
            /* pop left paren */
            tokens.pop_front();
            node = parse_expr(tokens);

            if (tokens.front().type != Token::TOK_RPAREN)
            {
                std::cerr << "Parentheses mismatch!\n";
                std::cout << tokens.front().file << ":" << tokens.front().row << ":" << tokens.front().col << "\n";
                exit(EXIT_FAILURE);
            }

            tokens.pop_front();
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

static Node *parse_range(std::list<Token> &tokens)
{
    Node *node = parse_add_sub(tokens);
    
    if (tokens.front().type == Token::TOK_RANGE)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_add_sub(tokens));
    }

    return node;
}

static Node *parse_lt_gt(std::list<Token> &tokens)
{
    Node *node = parse_range(tokens);

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
        node->children.push_back(parse_expr(tokens));
    }

    return node;
}

static Node *parse_expr(std::list<Token> &tokens)
{
    Node *node = parse_in_out(tokens);

    if (functions.count(node->token.data))
    {
        node->type = Node::NODE_FUN_CALL;

        /* more than one function argument */
        if (tokens.front().type == Token::TOK_LPAREN)
        {
            tokens.pop_front();
            while (tokens.size() && tokens.front().type != Token::TOK_RPAREN)
            {
                node->children.push_back(parse_expr(tokens));
            }

            /* expect ) */
            tokens.pop_front();
        }
        else
        {
            node->children.push_back(parse_expr(tokens));
        }

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
    scopes.push(scopes.top());
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
    Node *loop = new_node(tokens.front());
    loop->type = Node::NODE_LOOP;
    tokens.pop_front();
    scopes.push(scopes.top());

    /* 
     * loop (conditional while loop)
     * @ cond {
     *     ;; do something
     * }
     */
    loop->children.push_back(parse_expr(tokens));

    Node *body = new Node;
    body->type = Node::NODE_BODY;

    /* eat { */
    tokens.pop_front();
    while (tokens.size() && tokens.front().type != Token::TOK_RBRACE)
    {
        body->children.push_back(parse_statement(tokens));
    }

    /* eat } */
    scopes.pop();
    tokens.pop_front();

    loop->children.push_back(body);

    return loop;
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

#include "parser.hpp"

typedef std::unordered_set<std::string> Scope;
std::stack<Scope> scopes;

static std::unordered_map<std::string, uint64_t> functions;
static std::unique_ptr<Node> parse_expr(std::list<Token> &tokens);

static std::unique_ptr<Node> new_node(Token token)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();
    node->token = token;

    return node;
}

static std::unique_ptr<Node> unary(std::unique_ptr<Node> op, std::unique_ptr<Node> operand)
{
    op->children.push_back(std::move(operand));
    op->type = Node::NODE_OP;
    return op;
}

static std::unique_ptr<Node> binary(std::unique_ptr<Node> left, std::unique_ptr<Node> op, std::unique_ptr<Node> right)
{
    op->children.push_back(std::move(left));
    op->children.push_back(std::move(right));
    op->type = Node::NODE_OP;
    return op;
}

static std::unique_ptr<Node> parse_fact(std::list<Token> &tokens)
{
    std::unique_ptr<Node> node = nullptr;

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
        case Token::KEY_UNIT:
        {
            node = new_node(tokens.front());
            node->type = Node::NODE_KEY;
            tokens.pop_front();
            break;
        }

        case Token::KEY_PRINT:
        {
            node = new_node(tokens.front());
            node->type = Node::NODE_BUILT_IN;
            tokens.pop_front();
            node->children.push_back(parse_expr(tokens));
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
                print_location(tokens.front());
                exit(EXIT_FAILURE);
            }

            tokens.pop_front();
            break;
        }

        case Token::TOK_NOT:
        case Token::TOK_SUB:
        {
            std::unique_ptr<Node> op = new_node(tokens.front());
            tokens.pop_front();
            node = unary(std::move(op), parse_fact(tokens));
            break;
        }

        case Token::TOK_ID:
        {
            node = new_node(tokens.front());
            tokens.pop_front();
            node->type = Node::NODE_VAR;

            if (functions.count(node->token.data))
            {
                node->type = Node::NODE_FUN_CALL;

                /* more than one function argument */
                for (size_t i = 0; i < functions[node->token.data]; i++)
                {
                    node->children.push_back(parse_expr(tokens));
                }
            }

            if (tokens.front().type == Token::TOK_LBRACK)
            {
                tokens.pop_front();

                while (tokens.front().type != Token::TOK_RBRACK)
                {
                    node->children.push_back(parse_expr(tokens));
                }

                /* expect ] */
                tokens.pop_front();
            }
            break;
        }

        case Token::TOK_LBRACK:
        {
            node = new_node(tokens.front());
            node->token.data = "[]";
            node->type = Node::NODE_LIT;
            tokens.pop_front();

            while (tokens.front().type != Token::TOK_RBRACK)
            {
                node->children.push_back(parse_expr(tokens));
            }

            /* expect ] */
            tokens.pop_front();
            break;
        }

        default:
        {
            break;
        }
    }

    return node;
}

static std::unique_ptr<Node> parse_term(std::list<Token> &tokens)
{
    std::unique_ptr<Node> node = parse_fact(tokens);

    while (tokens.front().type == Token::TOK_MUL ||
           tokens.front().type == Token::TOK_DIV ||
           tokens.front().type == Token::TOK_MOD ||
           tokens.front().type == Token::TOK_SHL ||
           tokens.front().type == Token::TOK_SHR)
    {
        std::unique_ptr<Node> op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(std::move(node), std::move(op), parse_fact(tokens));
    }

    return node;
}

static std::unique_ptr<Node> parse_add_sub(std::list<Token> &tokens)
{
    std::unique_ptr<Node> node = parse_term(tokens);

    while (tokens.front().type == Token::TOK_ADD ||
           tokens.front().type == Token::TOK_SUB)
    {
        std::unique_ptr<Node> op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(std::move(node), std::move(op), parse_term(tokens));
    }

    return node;
}

static std::unique_ptr<Node> parse_lt_gt(std::list<Token> &tokens)
{
    std::unique_ptr<Node> node = parse_add_sub(tokens);

    while (tokens.front().type == Token::TOK_LT ||
           tokens.front().type == Token::TOK_GT ||
           tokens.front().type == Token::TOK_LTE ||
           tokens.front().type == Token::TOK_GTE)
    {
        std::unique_ptr<Node> op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(std::move(node), std::move(op), parse_add_sub(tokens));
    }

    return node;
}

static std::unique_ptr<Node> parse_eq_neq(std::list<Token> &tokens)
{
    std::unique_ptr<Node> node = parse_lt_gt(tokens);

    while (tokens.front().type == Token::TOK_EQ ||
           tokens.front().type == Token::TOK_NEQ)
    {
        std::unique_ptr<Node> op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(std::move(node), std::move(op), parse_lt_gt(tokens));
    }

    return node;
}

static std::unique_ptr<Node> parse_and(std::list<Token> &tokens)
{
    std::unique_ptr<Node> node = parse_eq_neq(tokens);

    while (tokens.front().type == Token::TOK_AND)
    {
        std::unique_ptr<Node> op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(std::move(node), std::move(op), parse_eq_neq(tokens));
    }

    return node;
}

static std::unique_ptr<Node> parse_or(std::list<Token> &tokens)
{
    std::unique_ptr<Node> node = parse_and(tokens);

    while (tokens.front().type == Token::TOK_OR)
    {
        std::unique_ptr<Node> op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(std::move(node), std::move(op), parse_and(tokens));
    }

    return node;
}

static std::unique_ptr<Node> parse_if(std::list<Token> &tokens)
{
    std::unique_ptr<Node> node = new_node(tokens.front());
    node->type = Node::NODE_IF;
    tokens.pop_front();

    /* eat condition */
    if (node->token.type == Token::TOK_IF || tokens.front().type != Token::TOK_LBRACE)
    {
        node->children.push_back(parse_expr(tokens));
    }

    std::unique_ptr<Node> body = std::make_unique<Node>();
    body->type = Node::NODE_BODY;

    /* eat { */
    tokens.pop_front();
    scopes.push(scopes.top());
    while (tokens.size() && tokens.front().type != Token::TOK_RBRACE)
    {
        body->children.push_back(parse_expr(tokens));
    }

    node->children.push_back(std::move(body));

    /* eat } */
    scopes.pop();
    tokens.pop_front();

    if (tokens.front().type == Token::TOK_ELSE)
    {
        node->children.push_back(parse_if(tokens));
    }

    return node;
}

static std::unique_ptr<Node> parse_loop(std::list<Token> &tokens)
{
    std::unique_ptr<Node> loop = new_node(tokens.front());
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

    std::unique_ptr<Node> body = std::make_unique<Node>();
    body->type = Node::NODE_BODY;

    /* eat { */
    tokens.pop_front();
    while (tokens.size() && tokens.front().type != Token::TOK_RBRACE)
    {
        body->children.push_back(parse_expr(tokens));
    }

    /* eat } */
    scopes.pop();
    tokens.pop_front();

    loop->children.push_back(std::move(body));

    return loop;
}

static std::unique_ptr<Node> parse_expr(std::list<Token> &tokens)
{
    if (tokens.front().type == Token::TOK_IF)
    {
        return parse_if(tokens);
    }

    if (tokens.front().type == Token::TOK_LOOP)
    {
        return parse_loop(tokens);
    }

    std::unique_ptr<Node> node = parse_or(tokens);

    /* TODO: parse variable assignment syntax */
    if (tokens.front().type == Token::TOK_COLON)
    {
        if (node->type != Node::NODE_VAR)
        {
            print_location(node->token);
            std::cerr << "Cannot assign expression to non-variable type\n";
            exit(EXIT_FAILURE);
        }
        scopes.top().insert(node->token.data);

        std::unique_ptr<Node> asn = std::make_unique<Node>();
        asn->type = Node::NODE_VAR_ASN;
        asn->children.push_back(std::move(node));
        tokens.pop_front();

        asn->children.push_back(parse_expr(tokens));
        
        return asn;
    }

    return node;
}

static std::unique_ptr<Node> parse_function(std::list<Token> &tokens)
{
    /* eat fn */
    tokens.pop_front();

    std::unique_ptr<Node> node = new_node(tokens.front());

    node->type = Node::NODE_FUN_DEC;
    tokens.pop_front();

    std::unique_ptr<Node> in, out, body;
    in = std::make_unique<Node>();
    out = std::make_unique<Node>();
    body = std::make_unique<Node>();

    in->type = Node::NODE_FUN_IN;
    out->type = Node::NODE_FUN_OUT;
    body->type = Node::NODE_BODY;

    /* eat colon */
    tokens.pop_front();

    /* args */
    while (tokens.front().type != Token::TOK_ARROW)
    {
        in->children.push_back(new_node(tokens.front()));
        tokens.pop_front();
    }

    bool nil_input = in->children.front()->token.type == Token::KEY_UNIT;
    functions[node->token.data] = !nil_input ? in->children.size() : 0;

    /* eat arrow */
    tokens.pop_front();

    /* return */
    out->children.push_back(new_node(tokens.front()));
    tokens.pop_front();

    if (tokens.front().type != Token::TOK_LBRACE)
    {
        std::cerr << "Expected `{`\n";
        exit(EXIT_FAILURE);
    }

    /* eat { */
    tokens.pop_front();
    scopes.push(Scope());

    /* parse input declarations (has to be first in function) */
    for (size_t i = 0; i < in->children.size() && !nil_input; i++)
    {
        std::unique_ptr<Node> arg = new_node(tokens.front());
        tokens.pop_front();
        arg->type = Node::NODE_VAR;
        scopes.top().insert(arg->token.data);

        body->children.push_back(std::move(arg));
    }

    while (tokens.size() && tokens.front().type != Token::TOK_RBRACE)
    {
        body->children.push_back(parse_expr(tokens));
    }

    node->children.push_back(std::move(in));
    node->children.push_back(std::move(out));
    node->children.push_back(std::move(body));

    /* eat } */
    scopes.pop();
    tokens.pop_front();
    return node;
}

std::unique_ptr<Node> parse_program(std::list<Token> &tokens)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();
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
            node->children.push_back(parse_expr(tokens));
        }
    }
    scopes.pop();

    return node;
}

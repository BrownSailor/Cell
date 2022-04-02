#include "include/parser.h"

Node *newNode(Token token)
{
    Node *node = new Node();
    node->token = token;

    return node;
}

Node *unary(Node *op, Node *node)
{
    op->children.push_back(node);

    return op;
}

Node *binary(Node *left, Node *op, Node *right)
{
    op->children.push_back(left);
    op->children.push_back(right);

    return op;
}

Node *parse_fact(std::list<Token> &tokens)
{
    Node *node = nullptr;

    if (tokens.front().type == Token::TOK_LPAREN)
    {
        tokens.pop_front();
        node = parse_expr(tokens);
        if (tokens.front().type != Token::TOK_RPAREN)
        {
            print_error("Expected ')'", tokens.front().row, tokens.front().col);
        }
        tokens.pop_front();
    }
    else if (tokens.front().type == Token::TOK_TILDA || 
        tokens.front().type == Token::TOK_ARR || 
        tokens.front().type == Token::TOK_BANG || 
        tokens.front().type == Token::TOK_MINUS)
    {
        Node *op = newNode(tokens.front());
        tokens.pop_front();

        node = unary(op, parse_expr(tokens));
    }
    else if (tokens.front().type == Token::TOK_ID ||
             tokens.front().type == Token::TOK_NUM)
    {
        node = newNode(tokens.front());
        tokens.pop_front();
    }

    return node;
}

Node *parse_term(std::list<Token> &tokens)
{
    Node *node = parse_fact(tokens);

    while (tokens.front().type == Token::TOK_STAR ||
           tokens.front().type == Token::TOK_SLASH ||
           tokens.front().type == Token::TOK_PERCENT)
    {
        Node *op = newNode(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_fact(tokens));
    }

    return node;
}

Node *parse_expr(std::list<Token> &tokens)
{
    Node *node = parse_term(tokens);

    while (tokens.front().type == Token::TOK_PLUS ||
           tokens.front().type == Token::TOK_MINUS)
    {
        Node *op = newNode(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_term(tokens));
    }

    return node;
}

Node *parse_statement(std::list<Token> &tokens)
{
    // return keyword
    if (tokens.front().type != Token::TOK_RETURN)
    {
        print_error("Expected `return`, found `" + tokens.front().data + "` instead.", tokens.front().row, tokens.front().col);
    }

    Node *node = newNode(tokens.front());
    tokens.pop_front();

    while (tokens.front().type != Token::TOK_EOL && tokens.front().type != Token::TOK_RBRACE)
    {
        node->children.push_back(parse_expr(tokens));
    }

    if (tokens.front().type == Token::TOK_EOL)
    {
        tokens.pop_front();
    }

    return node;
}

Node *parse_function(std::list<Token> &tokens)
{
    // function name
    Node *node = newNode(tokens.front());
    tokens.pop_front();

    if (tokens.front().type == Token::TOK_LPAREN)
    {
        tokens.pop_front();
        while (tokens.front().type != Token::TOK_RPAREN)
        {
            if (tokens.front().type == Token::TOK_COM)
            {
                tokens.pop_front();
            }

            node->children.push_back(parse_expr(tokens));
        }
        tokens.pop_front();
    }

    // expect a colon
    if (tokens.front().type != Token::TOK_COL)
    {
        print_error("Expected `:`, found `" + tokens.front().data + "` instead.", tokens.front().row, tokens.front().col);
    }
    tokens.pop_front();

    // expect type of function
    node->children.push_back(parse_expr(tokens));

    // conditional expect EOL
    if (tokens.front().type == Token::TOK_EOL)
    {
        tokens.pop_front();
    }
    
    // expect open brace
    if (tokens.front().type != Token::TOK_LBRACE)
    {
        print_error("Expected `{`, found `" + tokens.front().data + "` instead.", tokens.front().row, tokens.front().col);
    }
    tokens.pop_front();

    // conditional expect EOL
    if (tokens.front().type == Token::TOK_EOL)
    {
        tokens.pop_front();
    }
    
    // parse statements
    while (tokens.front().type != Token::TOK_RBRACE)
    {
        node->children.push_back(parse_statement(tokens));
    }

    if (tokens.front().type != Token::TOK_RBRACE)
    {
        print_error("Expected `}`, found `" + tokens.front().data + "` instead.", tokens.front().row, tokens.front().col);
    }
    tokens.pop_front();

    return node;
}

Node *parse_program(std::list<Token> &tokens)
{
    Node *root = parse_function(tokens);

    return root;
}

void pretty_print_tabs(int num_tabs)
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

void pretty_print_helper(Node *node, int num_tabs)
{
    print_token(node->token);
    std::cout << "\n";

    num_tabs++;
    
    auto it = node->children.begin();
    for (size_t i = 0; i < node->children.size(); i++)
    {
        pretty_print_tabs(num_tabs);
        pretty_print_helper(*it, num_tabs);
        std::advance(it, 1);
    }
    num_tabs--;
}

void pretty_print(Node *node)
{
    int num_tabs = 0;
    pretty_print_helper(node, num_tabs);
    std::cout << "\n";
}

void print_error(std::string message, int row, int col)
{
    std::cerr << std::to_string(row) << ":" << std::to_string(col) << ":" << " ERROR: " << message << std::endl;
    exit(EXIT_FAILURE);
}
#include "include/parser.h"

Node *newNode(Token token)
{
    Node *node = new Node();
    node->token = token;

    return node;
}

Node *parse_expr(std::list<Token> &tokens)
{
    Node *node = nullptr;
    if (tokens.front().type == Token::TOK_NUM || 
        tokens.front().type == Token::TOK_ID)
    {
        node = newNode(tokens.front());
        tokens.pop_front();
    }
    else if (tokens.front().type == Token::TOK_MINUS || 
             tokens.front().type == Token::TOK_TILDA ||
             tokens.front().type == Token::TOK_ARR)
    {
        node = newNode(tokens.front());
        tokens.pop_front();

        node->children.push_back(parse_expr(tokens));
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

void pretty_print(Node *node, int depth)
{
    for (int i = 0; i < depth; i++)
    {
        std::cout << "\t";
    }

    print_token(node->token);

    for (Node *child : node->children)
    {
        pretty_print(child, depth + 1);
    }
}

void print_error(std::string message, int row, int col)
{
    std::cerr << std::to_string(row) << ":" << std::to_string(col) << ":" << " ERROR: " << message << std::endl;
    exit(EXIT_FAILURE);
}
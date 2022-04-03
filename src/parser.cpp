#include "include/parser.h"

/*
 * new_node
 *    Purpose: creates a new node with the given token
 * Parameters: token - the token to be stored in the node
 *    Returns: a pointer to the new node
 */
Node *new_node(Token token)
{
    Node *node = new Node();
    node->token = token;

    return node;
}

/*
 * unary
 *    Purpose: creates a new node representing a unary operation
 * Parameters: op - the node containing the unary operation, token - the token to apply the op to
 *    Returns: a pointer to the new node
 */
Node *unary(Node *op, Node *node)
{
    op->children.push_back(node);

    return op;
}

/*
 * binary
 *    Purpose: creates a new node representing a binary operation
 * Parameters: left - the node containing the left operand, op - the node containing the binary operation, 
 *             right - the node containing the right operand
 *    Returns: a pointer to the new node
 */
Node *binary(Node *left, Node *op, Node *right)
{
    op->children.push_back(left);
    op->children.push_back(right);

    return op;
}

/*
 * parse_fact
 *    Purpose: parses a factor 
 * Parameters: tokens - the list of tokens to parse
 *    Returns: a pointer to the new node
 */
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
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = unary(op, parse_expr(tokens));
    }
    else if (tokens.front().type == Token::TOK_ID ||
             tokens.front().type == Token::TOK_NUM)
    {
        node = new_node(tokens.front());
        tokens.pop_front();

        if (tokens.front().type == Token::TOK_COL)
        {
            tokens.pop_front();
            node->children.push_back(parse_expr(tokens));
        }
    }

    return node;
}

/*
 * parse_term
 *    Purpose: parses a term 
 * Parameters: tokens - the list of tokens to parse
 *    Returns: a pointer to the new node
 */
Node *parse_term(std::list<Token> &tokens)
{
    Node *node = parse_fact(tokens);

    while (tokens.front().type == Token::TOK_STAR ||
           tokens.front().type == Token::TOK_SLASH ||
           tokens.front().type == Token::TOK_PERCENT)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_fact(tokens));
    }

    return node;
}

/*
 * parse_expr
 *    Purpose: parses a expression 
 * Parameters: tokens - the list of tokens to parse
 *    Returns: a pointer to the new node
 */
Node *parse_expr(std::list<Token> &tokens)
{
    Node *node = parse_term(tokens);

    while (tokens.front().type == Token::TOK_PLUS ||
           tokens.front().type == Token::TOK_MINUS)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_term(tokens));
    }

    return node;
}

/*
 * parse_statement
 *    Purpose: parses a statement 
 * Parameters: tokens - the list of tokens to parse
 *    Returns: a pointer to the new node
 */
Node *parse_statement(std::list<Token> &tokens)
{
    // return keyword
    if (tokens.front().type != Token::TOK_RETURN)
    {
        print_error("Expected `return`, found `" + tokens.front().data + "` instead.", tokens.front().row, tokens.front().col);
    }

    Node *node = new_node(tokens.front());
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

/*
 * parse_function
 *    Purpose: parses a function 
 * Parameters: tokens - the list of tokens to parse
 *    Returns: a pointer to the new node
 */
Node *parse_function(std::list<Token> &tokens)
{
    // function name
    Node *node = new_node(tokens.front());
    tokens.pop_front();

    if (tokens.front().type == Token::TOK_LPAREN)
    {
        tokens.pop_front();
        Node *args = new_node({ .type = Token::TOK_LIST });

        while (tokens.front().type != Token::TOK_RPAREN)
        {
            if (tokens.front().type == Token::TOK_COM)
            {
                tokens.pop_front();
            }

            args->children.push_back(parse_expr(tokens));
        }
        tokens.pop_front();

        node->children.push_back(args);
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

/*
 * parse_program
 *    Purpose: parses a program 
 * Parameters: tokens - the list of tokens to parse
 *    Returns: a pointer to the new node
 */
Node *parse_program(std::list<Token> &tokens)
{
    Node *root = parse_function(tokens);

    return root;
}

/*
 * pretty_print_tabs
 *    Purpose: print the number of tabs necessary as well as the ending character
 * Parameters: num_tabs - the number of tabs to print
 *    Returns: none
 */
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

/*
 * pretty_print_helper
 *    Purpose: pretty print a node and its children recursively
 * Parameters: node - the node to start printing from, num_tabs - the number of tabs to print
 *    Returns: none
 */
void pretty_print_helper(Node *node, int num_tabs)
{
    print_token(node->token);

    num_tabs++;
    
    for (auto it = node->children.begin(); it != node->children.end(); std::advance(it, 1))
    {
        pretty_print_tabs(num_tabs);
        pretty_print_helper(*it, num_tabs);
    }
    num_tabs--;
}

/*
 * pretty_print
 *    Purpose: pretty print a tree using the helper function
 * Parameters: node - the node to start printing the tree from
 *    Returns: none
 */
void pretty_print(Node *node)
{
    int num_tabs = 0;
    pretty_print_helper(node, num_tabs);
    std::cout << "\n";
}

/*
 * print_error
 *    Purpose: print an error and exit compilation with a failure exit status
 * Parameters: message - the message to print, row - the row of the error, col - the column of the error
 *    Returns: none
 */
void print_error(std::string message, int row, int col)
{
    std::cerr << std::to_string(row) << ":" << std::to_string(col) << ":" << " ERROR: " << message << std::endl;
    exit(EXIT_FAILURE);
}
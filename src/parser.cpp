#include "include/parser.h"

int eat_open_parens(std::list<Token> &tokens)
{
    int parens = 0;
    while (tokens.front().type == Token::TOK_LPAREN)
    {
        parens++;
        tokens.pop_front();
    }

    return parens;
}

void eat_close_parens(std::list<Token> &tokens, int parens)
{
    while (tokens.front().type == Token::TOK_RPAREN)
    {
        parens--;
        tokens.pop_front();
    }

    if (parens)
    {
        print_error("expected `)`", tokens.front());
        exit(EXIT_FAILURE);
    }
}

Node *new_node(Token token)
{
    Node *node = new Node;
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

Node *parse_fact(std::list<Token> &tokens, Scope &scope)
{
    Node *node = new Node;

    switch (tokens.front().type)
    {
        case Token::TOK_LPAREN:
        {
            int p = eat_open_parens(tokens);
            node = parse_expr(tokens, scope);
            eat_close_parens(tokens, p);
            break;
        }
        
        case Token::TOK_TILDA:
        case Token::TOK_BANG:
        case Token::TOK_MINUS:
        {
            Node *op = new_node(tokens.front());
            tokens.pop_front();
            node = unary(op, parse_expr(tokens, scope));
            break;
        }
        
        case Token::TOK_ID:
        case Token::TOK_NUM:
        case Token::TOK_CHAR:
        case Token::TOK_STR:
        case Token::KEY_RETURN:
        case Token::KEY_PRINT:
        case Token::KEY_PRINTLN:
        case Token::KEY_BYTE:
        case Token::KEY_CHAR:
        case Token::KEY_BOOL:
        case Token::KEY_SHORT:
        case Token::KEY_INT:
        case Token::KEY_LONG:
        {
            node = new_node(tokens.front());
            tokens.pop_front();
            break;
        }

        default: break;
    }

    return node;
}

Node *parse_term(std::list<Token> &tokens, Scope &scope)
{
    Node *node = new Node;
    node = parse_fact(tokens, scope);

    while (tokens.front().type == Token::TOK_STAR ||
           tokens.front().type == Token::TOK_SLASH ||
           tokens.front().type == Token::TOK_PERCENT ||
           tokens.front().type == Token::TOK_SHL || 
           tokens.front().type == Token::TOK_SHR)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_fact(tokens, scope));
    }

    return node;
}

Node *parse_add_sub(std::list<Token> &tokens, Scope &scope)
{
    Node *node = new Node;
    node = parse_term(tokens, scope);

    while (tokens.front().type == Token::TOK_PLUS ||
           tokens.front().type == Token::TOK_MINUS)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_term(tokens, scope));
    }

    return node;
}

Node *parse_lt_gt(std::list<Token> &tokens, Scope &scope)
{
    Node *node = new Node;
    node = parse_add_sub(tokens, scope);

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

Node *parse_eq_neq(std::list<Token> &tokens, Scope &scope)
{
    Node *node = new Node;
    node = parse_lt_gt(tokens, scope);

    while (tokens.front().type == Token::TOK_EQEQ ||
           tokens.front().type == Token::TOK_NEQ)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_lt_gt(tokens, scope));
    }

    return node;
}

Node *parse_and(std::list<Token> &tokens, Scope &scope)
{
    Node *node = new Node;
    node = parse_eq_neq(tokens, scope);

    while (tokens.front().type == Token::TOK_LAND)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_eq_neq(tokens, scope));
    }

    return node;
}

Node *parse_or(std::list<Token> &tokens, Scope &scope)
{
    Node *node = new Node;
    node = parse_and(tokens, scope);

    while (tokens.front().type == Token::TOK_LOR)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_and(tokens, scope));
    }

    return node;
}

Node *parse_expr(std::list<Token> &tokens, Scope &scope)
{
    Node *node = new Node;

    switch (tokens.front().type)
    {
        case Token::KEY_RETURN:
        case Token::KEY_PRINT:
        case Token::KEY_PRINTLN:
        {
            node = new_node(tokens.front());
            tokens.pop_front();
            node->children.push_back(parse_expr(tokens, scope));
            break;
        }

        default:
        {
            node = parse_or(tokens, scope);

            if (tokens.front().type == Token::TOK_COL)
            {
                tokens.pop_front();
                node->children.push_back(parse_or(tokens, scope));

                // redefinition of already existing variable
                if (scope.count(node->token.data))
                {
                    print_error("redefinition of `" + node->token.data + "`", node->token);
                    print_warning("note: previous definition is here", scope.at(node->token.data)->token);
                    exit(EXIT_FAILURE);
                }

                scope.insert({ node->token.data, node });
            }

            if (tokens.front().type == Token::TOK_EQ)
            {
                tokens.pop_front();
                node->children.push_back(parse_or(tokens, scope));

                if (!scope.count(node->token.data))
                {
                    print_error("unknown identifier `" + node->token.data + "`", node->token);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    return node;
}

Node *parse_statement(std::list<Token> &tokens, Scope &scope)
{
    Node *node = new Node();

    if (tokens.front().type != Token::TOK_NUM && 
        tokens.front().type != Token::TOK_ID && 
        !INTRINSICS.count(tokens.front().data))
    {
        print_error("expected identifier", tokens.front());
        exit(EXIT_FAILURE);
    }

    if (tokens.front().type == Token::TOK_EOL)
    {
        tokens.pop_front();
    }
    
    node = parse_expr(tokens, scope);

    while (tokens.front().type != Token::TOK_EOL && tokens.front().type != Token::TOK_RBRACE)
    {
        node->children.push_back(parse_expr(tokens, scope));
    }

    if (tokens.front().type == Token::TOK_EOL)
    {
        tokens.pop_front();
    }

    return node;
}

Node *parse_if(std::list<Token> &tokens, Scope &scope)
{
    (void)(tokens);
    (void)(scope);
    return new Node();
}

Node *parse_loop(std::list<Token> &tokens, Scope &scope)
{
    (void)(tokens);
    (void)(scope);
    return new Node();
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
    Token func_name = tokens.front();
    func_name.type = Token::TOK_FUNC;
    Node *node = new_node(func_name);
    tokens.pop_front();

    // conditional expect on function params
    Node *args = new_node({ .type = Token::TOK_LIST });

    if (tokens.front().type == Token::TOK_LPAREN)
    {
        int p = eat_open_parens(tokens);

        while (tokens.front().type != Token::TOK_RPAREN)
        {
            if (tokens.front().type == Token::TOK_COM)
            {
                tokens.pop_front();
            }
            args->children.push_back(parse_expr(tokens, node->scope));
        }
        
        eat_close_parens(tokens, p);
    }

    node->children.push_back(args);

    // conditional expect on function type
    if (tokens.front().type == Token::TOK_COL)
    {
        tokens.pop_front();

        // expect type of function
        if (tokens.front().type != Token::TOK_ID && 
            !INTRINSICS.count(tokens.front().data))
        {
            print_error("expected identifier", tokens.front());
            exit(EXIT_FAILURE);
        }
        node->children.push_back(parse_expr(tokens, node->scope));
    }
    else
    {
        node->children.push_back(new_node({ .type = Token::KEY_VOID }));
    }

    // conditional expect EOL
    if (tokens.front().type == Token::TOK_EOL)
    {
        tokens.pop_front();
    }
    
    // expect open brace
    if (tokens.front().type != Token::TOK_LBRACE)
    {
        print_error("expected `{`", tokens.front());
        exit(EXIT_FAILURE);
    }
    tokens.pop_front();
    
    // parse statements
    while (tokens.front().type != Token::TOK_RBRACE)
    {
        if (tokens.front().type == Token::TOK_EOL)
        {
            tokens.pop_front();
        }

        // stop parsing function body when the first return statement is hit
        if (tokens.front().type == Token::KEY_RETURN)
        {
            node->children.push_back(parse_statement(tokens, node->scope));
            break;
        }
        else if (tokens.front().type == Token::TOK_LOOP)
        {
            node->children.push_back(parse_loop(tokens, node->scope));
        }
        else if (tokens.front().type == Token::TOK_IF || tokens.front().type == Token::TOK_ELSE)
        {
            node->children.push_back(parse_if(tokens, node->scope));
        }
        else
        {
            node->children.push_back(parse_statement(tokens, node->scope));
        }
    }
    tokens.pop_front();

    auto it = node->children.begin();
    std::advance(it, 1);

    if ((*it)->token.type != Token::KEY_VOID && 
        node->children.back()->token.type != Token::KEY_RETURN)
    {
        print_error("expected return statement for non-void function", tokens.front());
        print_warning("note: function type declared here", (*it)->token);
        exit(EXIT_FAILURE);
    }
    else if ((*it)->token.type == Token::KEY_VOID && 
             node->children.back()->token.type == Token::KEY_RETURN)
    {
        print_error("found return statement in void function", node->children.back()->token);
        exit(EXIT_FAILURE);
    }

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
    Node *node = new Node();
    node->token.type = Token::TOK_PROG;

    // TODO: add argc and argv to main function's scope

    while (tokens.size())
    {
        while (tokens.front().type == Token::TOK_EOL || tokens.front().type == Token::TOK_EOF)
        {
            tokens.pop_front();
        }

        if (tokens.size())
        {
            node->children.push_back(parse_function(tokens));
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
void pretty_print_tabs(int num_tabs, std::ostream &out)
{
    for (int i = 0; i < num_tabs - 1; i++)
    {
        out << "    ";
    }
    out << "   \u2502\n";

    for (int i = 0; i < num_tabs - 1; i++)
    {
        out << "    ";
    }
    out << "   \u2514";
}

/*
 * pretty_print_helper
 *    Purpose: pretty print a node and its children recursively
 * Parameters: node - the node to start printing from, num_tabs - the number of tabs to print, out - the output stream
 *    Returns: none
 *      Notes: the output stream is defaulted to std::cout
 */
void pretty_print_helper(Node *node, int num_tabs, std::ostream &out)
{
    print_token(node->token, out, false);
    out << "\n";
    num_tabs++;
    
    for (auto it = node->children.begin(); it != node->children.end(); std::advance(it, 1))
    {
        pretty_print_tabs(num_tabs, out);
        pretty_print_helper(*it, num_tabs, out);
    }
    num_tabs--;
}

/*
 * pretty_print
 *    Purpose: pretty print a tree using the helper function
 * Parameters: node - the node to start printing the tree from, out - the output stream
 *    Returns: none
 *      Notes: the output stream is defaulted to std::cout
 */
void pretty_print(Node *node, std::ostream &out)
{
    pretty_print_helper(node, 0, out);
    out << "\n";
}

/*
 * print_warning
 *    Purpose: print a warning message
 * Parameters: message - the message to print, token - the token containing the error
 *    Returns: none
 */
void print_warning(std::string message, const Token &token)
{
    print_location(token, std::cerr);
    std::cerr << ": " << message << "\n";
}

/*
 * print_error
 *    Purpose: print an error and exit compilation with a failure exit status
 * Parameters: message - the message to print, token - the token containing the error
 *    Returns: none
 */
void print_error(std::string message, const Token &token)
{
    print_location(token, std::cerr);
    std::cerr << ": error: " << message << "\n";
}
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
 * find_in_tree
 *    Purpose: searches a tree for a token with a certain string data
 * Parameters: node - tree to search, value - data to search for
 *    Returns: true if the value exists in the tree, false otherwise
 */
bool find_in_tree(Node *node, std::string value)
{
    if (node == nullptr)
    {
        return false;
    }

    if (node->token.data == value)
    {
        return true;
    }

    bool ans = false;
    for (auto it = node->children.begin(); it != node->children.end(); std::advance(it, 1))
    {
        ans |= find_in_tree(*it, value);
    }

    return ans;
}

Token check_scope(Node *node, const std::unordered_map<std::string, Node *> &scope)
{
    if (node == nullptr)
    {
        return {};
    }

    if (node->token.type == Token::TOK_ID &&
        !scope.count(node->token.data))
    {
        return node->token;
    }

    Token ans = {};
    for (auto it = node->children.begin(); it != node->children.end(); std::advance(it, 1))
    {
        Token check = check_scope(*it, scope);

        if (check.data != "")
        {
            ans = check;
            return ans;
        }
    }

    return ans;
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

    if (op->token.type == Token::TOK_BANG)
    {
        op->expr_type = Token::TOK_BOOL;
    }
    else
    {
        op->expr_type = op->children.front()->expr_type;
    }

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

    op->expr_type = std::max(left->expr_type, right->expr_type);

    return op;
}

/*
 * parse_fact
 *    Purpose: parses a factor 
 * Parameters: tokens - the list of tokens to parse
 *    Returns: a pointer to the new node
 */
Node *parse_fact(std::list<Token> &tokens, std::unordered_map<std::string, Node *> &scope)
{
    Node *node = new Node();

    if (tokens.front().type == Token::TOK_LPAREN)
    {
        tokens.pop_front();
        node = parse_expr(tokens, scope);
        if (tokens.front().type != Token::TOK_RPAREN)
        {
            print_error("expected ')'", tokens.front());
            exit(EXIT_FAILURE);
        }
        tokens.pop_front();
    }
    else if (tokens.front().type == Token::TOK_TILDA || 
             tokens.front().type == Token::TOK_STAR || 
             tokens.front().type == Token::TOK_BANG || 
             tokens.front().type == Token::TOK_MINUS ||
             tokens.front().type == Token::TOK_INC ||
             tokens.front().type == Token::TOK_DEC)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = unary(op, parse_fact(tokens, scope));
    }
    else if (tokens.front().type == Token::TOK_ID ||
             tokens.front().type == Token::TOK_NUM ||
             INTRINSICS.count(tokens.front().data))
    {
        node = new_node(tokens.front());

        if (tokens.front().type == Token::TOK_NUM)
        {
            int64_t x = std::stoll(tokens.front().data);

            if (x >= -(1LL << 7) && x < (1LL << 7))
            {
                node->expr_type = Token::TOK_BYTE;
            }
            else if (x >= -(1LL << 15) && x < (1LL << 15))
            {
                node->expr_type = Token::TOK_SHORT;
            }
            else if (x >= -(1LL << 31) && x < (1LL << 31))
            {
                node->expr_type = Token::TOK_INT;
            }
            else
            {
                node->expr_type = Token::TOK_LONG;
            }
        }
        else if (tokens.front().type == Token::TOK_ID)
        {
            if (scope.count(tokens.front().data))
            {
                node->expr_type = scope[tokens.front().data]->expr_type;
            }
        }
        tokens.pop_front();

        if (tokens.front().type == Token::TOK_INC ||
            tokens.front().type == Token::TOK_DEC)
        {
            node->children.push_back(new_node(tokens.front()));
            tokens.pop_front();
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
Node *parse_term(std::list<Token> &tokens, std::unordered_map<std::string, Node *> &scope)
{
    Node *node = new Node();
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

        if (node->children.front()->token.type == Token::TOK_ID)
        {
            node->children.front()->expr_type = node->expr_type;
            scope[node->children.front()->token.data]->expr_type = node->expr_type;
            scope[node->children.front()->token.data]->children.front()->token.type = node->expr_type;
        }
        if (node->children.back()->token.type == Token::TOK_ID)
        {
            node->children.back()->expr_type = node->expr_type;
            scope[node->children.back()->token.data]->expr_type = node->expr_type;
            scope[node->children.back()->token.data]->children.front()->token.type = node->expr_type;
        }
    }

    return node;
}

/*
 * parse_add_sub
 *    Purpose: parses an addition or subtraction expression 
 * Parameters: tokens - the list of tokens to parse
 *    Returns: a pointer to the new node
 */
Node *parse_add_sub(std::list<Token> &tokens, std::unordered_map<std::string, Node *> &scope)
{
    Node *node = new Node();
    node = parse_term(tokens, scope);

    while (tokens.front().type == Token::TOK_PLUS ||
           tokens.front().type == Token::TOK_MINUS)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_term(tokens, scope));

        if (node->children.front()->token.type == Token::TOK_ID)
        {
            node->children.front()->expr_type = node->expr_type;
            scope[node->children.front()->token.data]->expr_type = node->expr_type;
            scope[node->children.front()->token.data]->children.front()->token.type = node->expr_type;
        }
        if (node->children.back()->token.type == Token::TOK_ID)
        {
            node->children.back()->expr_type = node->expr_type;
            scope[node->children.back()->token.data]->expr_type = node->expr_type;
            scope[node->children.back()->token.data]->children.front()->token.type = node->expr_type;
        }
    }

    return node;
}

/*
 * parse_lt_gt
 *    Purpose: parses a comparitive expression 
 * Parameters: tokens - the list of tokens to parse
 *    Returns: a pointer to the new node
 */
Node *parse_lt_gt(std::list<Token> &tokens, std::unordered_map<std::string, Node *> &scope)
{
    Node *node = new Node();
    node = parse_add_sub(tokens, scope);

    while (tokens.front().type == Token::TOK_LT ||
           tokens.front().type == Token::TOK_GT ||
           tokens.front().type == Token::TOK_LTE ||
           tokens.front().type == Token::TOK_GTE)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_add_sub(tokens, scope));

        if (node->children.front()->token.type == Token::TOK_ID)
        {
            node->children.front()->expr_type = node->expr_type;
            scope[node->children.front()->token.data]->expr_type = node->expr_type;
            scope[node->children.front()->token.data]->children.front()->token.type = node->expr_type;
        }
        if (node->children.back()->token.type == Token::TOK_ID)
        {
            node->children.back()->expr_type = node->expr_type;
            scope[node->children.back()->token.data]->expr_type = node->expr_type;
            scope[node->children.back()->token.data]->children.front()->token.type = node->expr_type;
        }

        node->expr_type = Token::TOK_BOOL;
    }

    return node;
}

/*
 * parse_eq_neq
 *    Purpose: parses an equivalence expression 
 * Parameters: tokens - the list of tokens to parse
 *    Returns: a pointer to the new node
 */
Node *parse_eq_neq(std::list<Token> &tokens, std::unordered_map<std::string, Node *> &scope)
{
    Node *node = new Node();
    node = parse_lt_gt(tokens, scope);

    while (tokens.front().type == Token::TOK_EQEQ ||
           tokens.front().type == Token::TOK_NEQ)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_lt_gt(tokens, scope));

        if (node->children.front()->token.type == Token::TOK_ID)
        {
            node->children.front()->expr_type = node->expr_type;
            scope[node->children.front()->token.data]->expr_type = node->expr_type;
            scope[node->children.front()->token.data]->children.front()->token.type = node->expr_type;
        }
        if (node->children.back()->token.type == Token::TOK_ID)
        {
            node->children.back()->expr_type = node->expr_type;
            scope[node->children.back()->token.data]->expr_type = node->expr_type;
            scope[node->children.back()->token.data]->children.front()->token.type = node->expr_type;
        }

        node->expr_type = Token::TOK_BOOL;
    }

    return node;
}

/*
 * parse_and
 *    Purpose: parses a logical and expression 
 * Parameters: tokens - the list of tokens to parse
 *    Returns: a pointer to the new node
 */
Node *parse_and(std::list<Token> &tokens, std::unordered_map<std::string, Node *> &scope)
{
    Node *node = new Node();
    node = parse_eq_neq(tokens, scope);

    while (tokens.front().type == Token::TOK_LAND)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_eq_neq(tokens, scope));

        if (node->children.front()->token.type == Token::TOK_ID)
        {
            node->children.front()->expr_type = node->expr_type;
            scope[node->children.front()->token.data]->expr_type = node->expr_type;
            scope[node->children.front()->token.data]->children.front()->token.type = node->expr_type;
        }
        if (node->children.back()->token.type == Token::TOK_ID)
        {
            node->children.back()->expr_type = node->expr_type;
            scope[node->children.back()->token.data]->expr_type = node->expr_type;
            scope[node->children.back()->token.data]->children.front()->token.type = node->expr_type;
        }

        node->expr_type = Token::TOK_BOOL;
    }

    return node;
}

/*
 * parse_or
 *    Purpose: parses a logical or expression 
 * Parameters: tokens - the list of tokens to parse
 *    Returns: a pointer to the new node
 */
Node *parse_or(std::list<Token> &tokens, std::unordered_map<std::string, Node *> &scope)
{
    Node *node = new Node();
    node = parse_and(tokens, scope);

    if (tokens.front().type == Token::TOK_LOR)
    {
        Node *op = new_node(tokens.front());
        tokens.pop_front();

        node = binary(node, op, parse_and(tokens, scope));
        
        if (node->children.front()->token.type == Token::TOK_ID)
        {
            node->children.front()->expr_type = node->expr_type;
            scope[node->children.front()->token.data]->expr_type = node->expr_type;
            scope[node->children.front()->token.data]->children.front()->token.type = node->expr_type;
        }
        if (node->children.back()->token.type == Token::TOK_ID)
        {
            node->children.back()->expr_type = node->expr_type;
            scope[node->children.back()->token.data]->expr_type = node->expr_type;
            scope[node->children.back()->token.data]->children.front()->token.type = node->expr_type;
        }

        node->expr_type = Token::TOK_BOOL;
    }

    return node;
}

/*
 * parse_expr
 *    Purpose: parses an expression 
 * Parameters: tokens - the list of tokens to parse
 *    Returns: a pointer to the new node
 */
Node *parse_expr(std::list<Token> &tokens, std::unordered_map<std::string, Node *> &scope)
{
    Node *node = new Node();

    if (tokens.front().type == Token::TOK_RETURN)
    {
        node = new_node({ .type = Token::TOK_RETURN });
        tokens.pop_front();
        node->children.push_back(parse_expr(tokens, scope));
    }
    else if (tokens.front().type == Token::TOK_PRINT)
    {
        node = new_node({ .type = Token::TOK_PRINT });
        tokens.pop_front();
        node->children.push_back(parse_expr(tokens, scope));
    }
    else
    {
        node = parse_or(tokens, scope);

        // variable declaration
        if (tokens.front().type == Token::TOK_COL)
        {
            tokens.pop_front();
            node->children.push_back(parse_or(tokens, scope));

            if (scope.count(node->token.data))
            {
                print_error("redefinition of `" + node->token.data + "`", node->token);
                print_warning("note: previous definition is here", scope.at(node->token.data)->token);
                exit(EXIT_FAILURE);
            }

            scope.insert({ node->token.data, node });
            node->expr_type = node->children.front()->token.type;
        }

        // variable initialization
        if (tokens.front().type == Token::TOK_EQ)
        {
            tokens.pop_front();
            node->children.push_back(parse_or(tokens, scope));

            if (!scope.count(node->token.data))
            {
                // TODO: figure out actual size of variable types
                node->expr_type = node->children.front()->expr_type;
                node->children.push_front(new_node({ .type = node->expr_type }));
                scope.insert({ node->token.data, node });
            }

            Token scope_check = check_scope(node, scope);
            if (scope_check.data != "")
            {
                print_error("unknown identifier `" + scope_check.data + "`", scope_check);
                exit(EXIT_FAILURE);
            }
        }
    }

    return node;
}

/*
 * parse_statement
 *    Purpose: parses a statement 
 * Parameters: tokens - the list of tokens to parse
 *    Returns: a pointer to the new node
 */
Node *parse_statement(std::list<Token> &tokens, std::unordered_map<std::string, Node *> &scope)
{
    Node *node = new Node();

    if (tokens.front().type != Token::TOK_NUM && 
        tokens.front().type != Token::TOK_ID && 
        !INTRINSICS.count(tokens.front().data))
    {
        print_error("expected identifier", tokens.front());
        exit(EXIT_FAILURE);
    }
    
    node = parse_expr(tokens, scope);

    while (tokens.front().type != Token::TOK_EOL && tokens.front().type != Token::TOK_RBRACE)
    {
        node->children.push_back(parse_expr(tokens, scope));
        
        Token scope_check = check_scope(node, scope);
        if (scope_check.data != "")
        {
            print_error("unknown identifier `" + scope_check.data + "`", scope_check);
            exit(EXIT_FAILURE);
        }
    }

    if (tokens.front().type == Token::TOK_EOL)
    {
        tokens.pop_front();
    }

    return node;
}

/*
 * parse_if
 *    Purpose: parses a loop
 * Parameters: tokens - the list of tokens to parse, scope - the variables that are in scope of the if block
 *    Returns: a pointer to the new node
 */
Node *parse_if(std::list<Token> &tokens, std::unordered_map<std::string, Node *> &scope)
{
    Node *node = new_node(tokens.front());
    node->scope = scope;
    tokens.pop_front();

    int paren = 0;
    while (tokens.front().type == Token::TOK_LPAREN)
    {
        paren++;
        tokens.pop_front();
    }

    // parse condition
    if (tokens.front().type != Token::TOK_LBRACE)
    {
        node->children.push_back(parse_expr(tokens, node->scope));
    }

    if (paren)
    {
        while (paren--)
        {
            if (tokens.front().type != Token::TOK_RPAREN)
            {
                print_error("expected `)`", tokens.front());
                exit(EXIT_FAILURE);
            }
            tokens.pop_front();
        }
    }

    Token scope_check = check_scope(node, node->scope);
    if (scope_check.data != "")
    {
        print_error("unknown identifier `" + scope_check.data + "`", scope_check);
        exit(EXIT_FAILURE);
    }

    if (tokens.front().type == Token::TOK_EOL)
    {
        tokens.pop_front();
    }

    if (tokens.front().type != Token::TOK_LBRACE)
    {
        print_token(tokens.front());
        print_error("expected `{`", tokens.front());
        exit(EXIT_FAILURE);
    }
    tokens.pop_front();

    if (tokens.front().type == Token::TOK_EOL)
    {
        tokens.pop_front();
    }

    while (tokens.front().type != Token::TOK_RBRACE)
    {
        if (tokens.front().type == Token::TOK_LOOP)
        {
            node->children.push_back(parse_loop(tokens, node->scope));
        }
        else if (tokens.front().type == Token::TOK_IF)
        {
            node->children.push_back(parse_if(tokens, node->scope));
        }
        else
        {
            node->children.push_back(parse_statement(tokens, node->scope));
        }
    }

    tokens.pop_front();

    if (tokens.front().type == Token::TOK_EOL)
    {
        tokens.pop_front();
    }

    return node;
}

/*
 * parse_loop
 *    Purpose: parses a loop
 * Parameters: tokens - the list of tokens to parse, scope - the variables that are in scope of the loop
 *    Returns: a pointer to the new node
 */
Node *parse_loop(std::list<Token> &tokens, std::unordered_map<std::string, Node *> &scope)
{
    Node *node = new_node(tokens.front());
    node->scope = scope;
    tokens.pop_front();

    int paren = 0;
    while (tokens.front().type == Token::TOK_LPAREN)
    {
        paren++;
        tokens.pop_front();
    }

    // parse initialization, condition, and increment
    Node *list = new_node({ .type = Token::TOK_LIST });

    // initialization
    list->children.push_back(parse_expr(tokens, node->scope));

    // optional comma to separate condition
    if (tokens.front().type == Token::TOK_COM)
    {
        tokens.pop_front();
    }

    // condition
    if (tokens.front().type != Token::TOK_LBRACE &&
        tokens.front().type != Token::TOK_RPAREN)
    {
        list->children.push_back(parse_expr(tokens, node->scope));
    }
     
    // optional comma to separate increment
    if (tokens.front().type == Token::TOK_COM)
    {
        tokens.pop_front();
    }

    // increment
    if (tokens.front().type != Token::TOK_LBRACE &&
        tokens.front().type != Token::TOK_RPAREN)
    {
        list->children.push_back(parse_expr(tokens, node->scope));
    }

    if (paren)
    {
        while (paren--)
        {
            if (tokens.front().type != Token::TOK_RPAREN)
            {
                print_error("expected `)`", tokens.front());
                exit(EXIT_FAILURE);
            }
            tokens.pop_front();
        }
    }

    Token scope_check = check_scope(list, node->scope);
    if (scope_check.data != "")
    {
        print_error("unknown identifier `" + scope_check.data + "`", scope_check);
        exit(EXIT_FAILURE);
    }
    node->children.push_back(list);

    if (tokens.front().type == Token::TOK_EOL)
    {
        tokens.pop_front();
    }

    if (tokens.front().type != Token::TOK_LBRACE)
    {
        print_token(tokens.front());
        print_error("expected `{`", tokens.front());
        exit(EXIT_FAILURE);
    }
    tokens.pop_front();

    if (tokens.front().type == Token::TOK_EOL)
    {
        tokens.pop_front();
    }

    while (tokens.front().type != Token::TOK_RBRACE)
    {
        if (tokens.front().type == Token::TOK_LOOP)
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
    Token func_name = tokens.front();
    func_name.type = Token::TOK_FUNC;
    Node *node = new_node(func_name);
    tokens.pop_front();

    // conditional expect on function params
    Node *args = new_node({ .type = Token::TOK_LIST });

    if (tokens.front().type == Token::TOK_LPAREN)
    {
        tokens.pop_front();
        while (tokens.front().type != Token::TOK_RPAREN)
        {
            if (tokens.front().type == Token::TOK_COM)
            {
                tokens.pop_front();
            }

            args->children.push_back(parse_expr(tokens, node->scope));
        }
        tokens.pop_front();
    }

    node->children.push_back(args);

    // conditional expect on function type
    if (tokens.front().type == Token::TOK_COL)
    {
        tokens.pop_front();

        // expect type of function
        if (tokens.front().type != Token::TOK_ID && 
            !INTRINSICS.count(tokens.front().data) &&
            tokens.front().data != "*")
        {
            print_error("expected identifier", tokens.front());
            exit(EXIT_FAILURE);
        }
        node->children.push_back(parse_expr(tokens, node->scope));
    }
    else
    {
        node->children.push_back(new_node({ .type = Token::TOK_VOID }));
        node->children.back()->expr_type = Token::TOK_VOID;
    }
    node->expr_type = node->children.back()->expr_type;

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

    // conditional expect EOL
    if (tokens.front().type == Token::TOK_EOL)
    {
        tokens.pop_front();
    }
    
    // parse statements
    while (tokens.front().type != Token::TOK_RBRACE)
    {
        // stop parsing function body when the first return statement is hit
        if (tokens.front().type == Token::TOK_RETURN)
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

    if ((*it)->token.type != Token::TOK_VOID && 
        node->children.back()->token.type != Token::TOK_RETURN)
    {
        print_error("expected return statement for non-void function", tokens.front());
        print_warning("note: function type declared here", (*it)->token);
        exit(EXIT_FAILURE);
    }
    else if ((*it)->token.type == Token::TOK_VOID && 
             node->children.back()->token.type == Token::TOK_RETURN)
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
    if (node->expr_type != Token::TOK_NONE)
    {
        out << ": [ ";
        out << token_id_to_str(node->expr_type);
        out << " ]";
    }
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
#include "include/parser.h"

Scope global = Scope();

int eat_open_parens(std::list<Token> &tokens, char p)
{
    Token::Type t = (p == '(' ? Token::TOK_LPAREN : p == '[' ? Token::TOK_LBRACK : Token::TOK_LBRACE);

    int parens = 0;
    while (tokens.front().type == t)
    {
        parens++;
        tokens.pop_front();
    }

    return parens;
}

void eat_close_parens(std::list<Token> &tokens, int parens, char p)
{
    Token::Type t = (p == ')' ? Token::TOK_RPAREN : p == ']' ? Token::TOK_RBRACK : Token::TOK_RBRACE);

    while (parens && tokens.front().type == t)
    {
        parens--;
        tokens.pop_front();
    }

    if (parens)
    {
        std::string error = "expected `";
        error += p;
        error += '`';

        print_error(error, tokens.front());
        exit(EXIT_FAILURE);
    }
}

Node *new_node(Token token)
{
    Node *node = new Node;
    node->token = token;

    return node;
}

/*
 * eval_node
 *    Purpose: evaluate a node to a specific type
 * Parameters: node - the node to evaluate, scope - the scope to reference
 *    Returns: a token describing the type of the node's evaluation
 */
Token::Type eval_node(Node *node, const Scope &scope)
{
    switch (node->type)
    {
        case Node::NODE_LIT:
        case Node::NODE_KEY:
        {
            return node->token.type;
        }
        case Node::NODE_OP:
        {
            return std::max(eval_node(node->children.front(), scope), eval_node(node->children.back(), scope));
        }
        case Node::NODE_VAR:
        case Node::NODE_VAR_ASN:
        case Node::NODE_VAR_DEC_ASN:
        {
            return eval_node(scope.at(node->token.data)->children.front(), scope);
        }

        case Node::NODE_FUNC_CALL:
        {
            auto it = global.at(node->token.data)->children.begin();
            std::advance(it, 1);
            return eval_node(*it, scope);
        }

        default: return Token::TOK_NONE;
    }
}

Node *unary(Node *op, Node *node)
{
    op->children.push_back(node);
    op->type = Node::NODE_OP;
    return op;
}

Node *binary(Node *left, Node *op, Node *right)
{
    op->children.push_back(left);
    op->children.push_back(right);
    op->type = Node::NODE_OP;

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

        case Token::TOK_LBRACK:
        {
            int p = eat_open_parens(tokens, '[');
            node = parse_expr(tokens, scope);
            eat_close_parens(tokens, p, ']');
            node->arr_dim = p;
            break;
        }

        case Token::TOK_LBRACE:
        {
            node = new_node({ .type = Token::TOK_ARR });
            tokens.pop_front();

            while (tokens.front().type != Token::TOK_RBRACE)
            {
                node->children.push_back(parse_expr(tokens, scope));

                if (tokens.front().type == Token::TOK_COM)
                {
                    tokens.pop_front();
                }
            }
            tokens.pop_front();

            break;
        }

        case Token::TOK_TILDA:
        case Token::TOK_BANG:
        case Token::TOK_MINUS:
        case Token::TOK_INC:
        case Token::TOK_DEC:
        {
            Node *op = new_node(tokens.front());
            tokens.pop_front();
            node = unary(op, parse_expr(tokens, scope));
            break;
        }

        case Token::TOK_INT:
        case Token::TOK_CHAR:
        case Token::TOK_STR:
        {
            node = new_node(tokens.front());
            tokens.pop_front();
            node->type = Node::NODE_LIT;
            break;
        }
        case Token::KEY_CHAR:
        case Token::KEY_BOOL:
        case Token::KEY_INT:
        case Token::KEY_UINT:
        {
            node = new_node(tokens.front());
            tokens.pop_front();
            node->type = Node::NODE_KEY;
            break;
        }
        case Token::KEY_SIZE:
        case Token::KEY_POPF:
        case Token::KEY_PUSHB:
        {
            node = new_node(tokens.front());
            tokens.pop_front();
            node->type = Node::NODE_KEY;

            int p = eat_open_parens(tokens);
            while (tokens.front().type != Token::TOK_RPAREN)
            {
                node->children.push_back(parse_expr(tokens, scope));
            }
            eat_close_parens(tokens, p);
            break;
        }
        case Token::TOK_ID:
        {
            node = new_node(tokens.front());
            tokens.pop_front();

            if (global.count(node->token.data))
            {
                node->type = Node::NODE_FUNC_CALL;
                int np = eat_open_parens(tokens);

                for (size_t i = 0; i < global.at(node->token.data)->children.front()->children.size(); i++)
                {
                    node->children.push_back(parse_expr(tokens, scope));

                    if (tokens.front().type == Token::TOK_COM)
                    {
                        tokens.pop_front();
                        if (tokens.front().type == Token::TOK_EOL || tokens.front().type == Token::TOK_RPAREN)
                        {
                            print_error("expected identifier, found `" + tokens.front().data + "`", tokens.front());
                            exit(EXIT_FAILURE);
                        }
                    }
                }
                eat_close_parens(tokens, np);
            }
            else
            {
                node->type = Node::NODE_VAR;

                if (scope.count(node->token.data))
                {
                    node->arr_dim = scope.at(node->token.data)->arr_dim;
                }
            }

            while (tokens.front().type == Token::TOK_LBRACK)
            {
                tokens.pop_front();
                while (tokens.front().type != Token::TOK_RBRACK)
                {
                    node->children.push_back(parse_expr(tokens, scope));
                }
                tokens.pop_front();
            }

            if (tokens.front().type == Token::TOK_INC ||
                tokens.front().type == Token::TOK_DEC)
            {
                Node *op = new_node(tokens.front());
                op->token.type = (tokens.front().type == Token::TOK_INC ? Token::TOK_POST_INC : Token::TOK_POST_DEC);
                tokens.pop_front();
                node = unary(op, node);
            }
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
        case Token::KEY_DUMP:
        case Token::KEY_DUMPLN:
        {
            node = new_node(tokens.front());
            tokens.pop_front();
            node->children.push_back(parse_expr(tokens, scope));
            node->type = Node::NODE_KEY;
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
                if (scope.count(node->token.data) || global.count(node->token.data))
                {
                    print_error("redefinition of `" + node->token.data + "`", node->token);
                    print_warning("note: previous definition is here", (scope.count(node->token.data) ? scope.at(node->token.data)->token : global.at(node->token.data)->token));
                    exit(EXIT_FAILURE);
                }

                node->type = Node::NODE_VAR_DEC;
                node->arr_dim = node->children.front()->arr_dim;
                scope.insert({ node->token.data, node });
            }

            if (tokens.front().type == Token::TOK_EQ)
            {
                tokens.pop_front();
                node->children.push_back(parse_expr(tokens, scope));

                node->type = (node->type == Node::NODE_VAR_DEC ? Node::NODE_VAR_DEC_ASN : Node::NODE_VAR_ASN);

                if (!scope.count(node->token.data) && !global.count(node->token.data))
                {
                    print_error("unknown identifier `" + node->token.data + "`", node->token);
                    exit(EXIT_FAILURE);
                }
            }


            if ((node->type == Node::NODE_VAR || node->type == Node::NODE_FUNC_CALL) && !scope.count(node->token.data) && !global.count(node->token.data))
            {
                print_error("unknown identifier `" + node->token.data + "`", node->token);
                exit(EXIT_FAILURE);
            }

            break;
        }
    }

    return node;
}

Node *parse_statement(std::list<Token> &tokens, Scope &scope)
{
    Node *node = new Node();

    if (tokens.front().type != Token::TOK_INT &&
        tokens.front().type != Token::TOK_ID &&
        !INTRINSICS.count(tokens.front().data) &&
        tokens.front().type != Token::TOK_LPAREN)
    {
        print_error("expected expression", tokens.front());
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
        node->semi = true;
        tokens.pop_front();
    }

    return node;
}

Node *parse_if(std::list<Token> &tokens, Scope &scope)
{
    // TODO: implement single line if statements
    // expect ? or |
    Node *node = new_node(tokens.front());
    node->scope = scope;
    tokens.pop_front();

    if (tokens.front().type != Token::TOK_LBRACE)
    {
        if (node->token.type == Token::TOK_ELSE)
        {
            node->token.type = Token::TOK_ELIF;
        }

        int p = eat_open_parens(tokens);
        node->children.push_back(parse_expr(tokens, node->scope));
        eat_close_parens(tokens, p);
    }

    // expect {
    tokens.pop_front();
    while (tokens.front().type != Token::TOK_RBRACE)
    {
        if (tokens.front().type == Token::TOK_EOL)
        {
            tokens.pop_front();
        }

        parse_body(node, tokens, node->scope);
    }
    tokens.pop_front();

    if (tokens.front().type == Token::TOK_EOL)
    {
        tokens.pop_front();
    }

    if (tokens.front().type == Token::TOK_ELSE)
    {
        node->children.push_back(parse_if(tokens, scope));
    }

    return node;
}

Node *parse_loop(std::list<Token> &tokens, Scope &scope)
{
    // expect @
    Node *node = new_node(tokens.front());
    node->scope = scope;
    tokens.pop_front();

    Node *args = new_node({ .type = Token::TOK_LIST });

    // expect loop args
    int p = eat_open_parens(tokens);

    int count_args = 0;
    while (tokens.front().type != Token::TOK_LBRACE)
    {
        count_args++;
        if (count_args > 3)
        {
            print_error("too many arguments in loop statement", tokens.front());
            print_warning("note: loops may accept 0-3 expressions", tokens.front());
            exit(EXIT_FAILURE);
        }

        args->children.push_back(parse_expr(tokens, node->scope));

        if (tokens.front().type == Token::TOK_COM ||
            tokens.front().type == Token::TOK_EOL)
        {
            tokens.pop_front();
        }
    }
    eat_close_parens(tokens, p);
    tokens.pop_front();

    if (tokens.front().type == Token::TOK_EOL)
    {
        tokens.pop_front();
    }

    node->children.push_back(args);
    parse_body(node, tokens, node->scope);

    if (tokens.front().type == Token::TOK_EOL)
    {
        tokens.pop_front();
    }

    tokens.pop_front();

    if (tokens.front().type == Token::TOK_EOL)
    {
        tokens.pop_front();
    }

    return node;
}

Node *parse_body(Node *node, std::list<Token> &tokens, Scope &scope)
{
    while (tokens.front().type != Token::TOK_RBRACE)
    {
        switch (tokens.front().type)
        {
            case Token::TOK_LOOP:
            {
                node->children.push_back(parse_loop(tokens, scope));
                break;
            }
            case Token::TOK_IF:
            {
                node->children.push_back(parse_if(tokens, scope));
                break;
            }
            default:
            {
                node->children.push_back(parse_statement(tokens, scope));
                break;
            }
        }
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

    // std::cout << "Parse function: ";
    // print_token(tokens.front());

    global.insert({ tokens.front().data, node });
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

        parse_body(node, tokens, node->scope);
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
    Node *node = new Node();
    node->token.type = Token::TOK_PROG;

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
 * get_node_type
 *    Purpose: print a string representation of the type of a given node
 * Parameters: node - the given node, out - an output stream
 *    Returns: none
 */
std::string node_type_to_str(Node *node)
{
    switch (node->type)
    {
        case Node::NODE_LIT:
        {
            return "NODE_LIT";
        }
        case Node::NODE_OP:
        {
            return "NODE_OP";
        }
        case Node::NODE_KEY:
        {
            return "NODE_KEY";
        }
        case Node::NODE_VAR:
        {
            return "NODE_VAR";
        }
        case Node::NODE_VAR_ASN:
        {
            return "NODE_VAR_ASN";
        }
        case Node::NODE_VAR_DEC:
        {
            return "NODE_VAR_DEC";
        }
        case Node::NODE_VAR_DEC_ASN:
        {
            return "NODE_VAR_DEC_ASN";
        }
        case Node::NODE_FUNC_CALL:
        {
            return "NODE_FUNC_CALL";
        }
        case Node::NODE_FUNC_DEC:
        {
            return "NODE_FUNC_DEC";
        }

        default:
        {
            return "NODE_NONE";
        }
    }
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
void pretty_print_helper(Node *node, const Scope &scope, int num_tabs, std::ostream &out)
{
    print_token(node->token, out, false);

    if (node->arr_dim)
    {
        for (size_t i = 0; i < node->arr_dim; i++)
        {
            out << "*";
        }
    }

    if (node->semi)
    {
        out << ";";
    }

    if (node->type != Node::NODE_NONE)
    {
        out << " (" << node_type_to_str(node) << ")";
    }

    if (eval_node(node, scope) != Token::TOK_NONE)
    {
        out << " [" << token_id_to_str(eval_node(node, scope)) << "]";
    }

    if (node->scope.size())
    {
        print_scope(node->scope, out);
    }

    out << "\n";
    num_tabs++;

    for (auto it = node->children.begin(); it != node->children.end(); std::advance(it, 1))
    {
        pretty_print_tabs(num_tabs, out);
        pretty_print_helper(*it, scope, num_tabs, out);
    }
    num_tabs--;
}

void print_scope(const Scope &scope, std::ostream &out)
{
    out << " [ ";
    for (auto x : scope)
    {
        out << x.first << " ";
    }
    out << "]";
}

void total_scope(Node *root, Scope &scope)
{
    if (root == nullptr)
    {
        return;
    }

    for (auto x : root->scope)
    {
        scope.insert(x);
    }

    for (auto it = root->children.begin(); it != root->children.end(); std::advance(it, 1))
    {
        total_scope(*it, scope);
    }
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
    Scope scope = node->scope;
    total_scope(node, scope);

    pretty_print_helper(node, scope, 0, out);
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

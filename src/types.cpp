#include "types.hpp"

static std::unordered_map<std::string, TypeScheme> functions;

typedef std::unordered_map<std::string, TypeScheme> TypeScope;
std::stack<TypeScope> type_scopes;

std::unique_ptr<Node> curr_fun_outs;

/* TypeScheme operator overloading */
bool operator==(const TypeScheme &left, const TypeScheme &right)
{
    if (left.type == right.type)
    {
        switch (left.type)
        {
            case TypeScheme::ALPHA:
            {
                return left.alpha == right.alpha && left.arr_dim == right.arr_dim;
            }
            case TypeScheme::FUN_TYPE:
            {
                return left.params == right.params && left.alpha == right.alpha && left.arr_dim == right.arr_dim;
            }
            default:
            {
                return true;
            }
        }
    }
    else
    {
        if (left.type == TypeScheme::NONE || right.type == TypeScheme::NONE)
        {
            return false;
        }
        return left.alpha == right.alpha && left.arr_dim == right.arr_dim;
    }
}

bool operator!=(const TypeScheme &left, const TypeScheme &right)
{
    return !(left == right);
}

/* Types */
uint32_t type_id = 0;

std::unordered_map<std::string, uint32_t> type_names;
std::unordered_map<uint32_t, std::string> type_idens;

static void new_type(std::string type)
{
    type_names[type] = type_id;
    type_idens[type_id++] = type;
}

TypeScheme construct_type(uint32_t type)
{
    TypeScheme ts(TypeScheme::ALPHA);
    ts.alpha = type;
    return ts;
}

static void initialize_types()
{
    new_type("key");
    new_type("unit");
    new_type("num");
    new_type("bool");
    new_type("str");
}

static void type_check_expr(std::unique_ptr<Node> &root);

static void type_check_key(std::unique_ptr<Node> &root)
{
    root->type_scheme = construct_type(type_names["key"]);
}

static void type_check_built_in(std::unique_ptr<Node> &root)
{
    switch (root->token.type)
    {
        case Token::KEY_PRINT:
        {
            type_check_expr(root->children.front());
            TypeScheme ts = root->children.front()->type_scheme;
            if (ts == construct_type(type_names["num"]) ||
                ts == construct_type(type_names["bool"]) ||
                ts == construct_type(type_names["str"]))
            {
                root->type_scheme = construct_type(type_names["unit"]);
            }
            else
            {
                std::cerr << "Expected input of type `num`, `bool`, or `str`\n";
                exit(EXIT_FAILURE);
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

static void type_check_literal(std::unique_ptr<Node> &root)
{
    switch (root->token.type)
    {
        case Token::TOK_NUM:
        {
            root->type_scheme = construct_type(type_names["num"]);
            break;
        }
        case Token::TOK_TRU:
        case Token::TOK_FLS:
        {
            root->type_scheme = construct_type(type_names["bool"]);
            break;
        }
        case Token::TOK_STR:
        {
            root->type_scheme = construct_type(type_names["str"]);
            break;
        }
        case Token::TOK_LBRACK:
        {
            type_check_expr(root->children.front());
            TypeScheme ts = root->children.front()->type_scheme;

            for (size_t i = 1; i < root->children.size(); i++)
            {
                type_check_expr(root->children[i]);
                if (ts != root->children[i]->type_scheme)
                {
                    print_location(root->children[i]->token);
                    std::cerr << "Cannot initialize array of multiple types\n";
                    exit(EXIT_FAILURE);
                }
            }

            root->type_scheme = TypeScheme(TypeScheme::ALPHA);
            root->type_scheme.arr_dim = root->children.front()->type_scheme.arr_dim + 1;

            switch (ts.type)
            {
                case TypeScheme::ALPHA:
                case TypeScheme::FUN_TYPE:
                {
                    root->type_scheme.alpha = ts.alpha;
                    break;
                }
                default:
                {
                    break;
                }
            }

            break;
        }
        default:
        {
            root->type_scheme = TypeScheme();
            break;
        }
    }
}

static void type_check_un_op(std::unique_ptr<Node> &root)
{
    std::unique_ptr<Node> &operand = root->children.front();
    type_check_expr(root->children.front());
    TypeScheme ts = operand->type_scheme;

    switch (root->token.type)
    {
        case Token::TOK_SUB:
        {
            if (ts != construct_type(type_names["num"]))
            {
                print_location(operand->token);
                std::cerr << "Type of arithmetic negation operand does not match `num`\n";
                exit(EXIT_FAILURE);
            }
            root->type_scheme = construct_type(type_names["num"]);
            break;
        }
        case Token::TOK_NOT:
        {
            if (ts != construct_type(type_names["bool"]))
            {
                print_location(operand->token);
                std::cerr << "Type of logical negation operand does not match `bool`\n";
                exit(EXIT_FAILURE);
            }
            root->type_scheme = construct_type(type_names["bool"]);
            break;
        }
        default:
        {
            root->type_scheme = TypeScheme();
            break;
        }
    }
}

static unsigned height(std::unique_ptr<Node> &root)
{
    if (root == nullptr) return 0;
    if (!root->children.size()) return 1;
    return 1 + std::max(height(root->children.front()), height(root->children.back()));
}

static void type_check_bin_op(std::unique_ptr<Node> &root)
{
    std::unique_ptr<Node> &left = root->children.front(); 
    std::unique_ptr<Node> &right = root->children.back(); 
    type_check_expr(root->children.front());
    type_check_expr(root->children.back());

    switch (root->token.type)
    {
        case Token::TOK_MUL:
        {
            if (right->type_scheme == construct_type(type_names["key"]))
            {
                if (left->type_scheme != construct_type(type_names["num"]))
                {
                    print_location(left->token);
                    std::cerr << "Expected value of type `num` for size of array\n";
                    exit(EXIT_FAILURE);
                }

                root->type_scheme = TypeScheme(TypeScheme::ALPHA);
                root->type_scheme.arr_dim = height(left);
                root->type_scheme.alpha = type_names[right->token.data];

                break;
            }
            else 
            {
                if (left->type_scheme != construct_type(type_names["num"]) || left->type_scheme != right->type_scheme)
                {
                    print_location(left->token);
                    std::cerr << "Type of arithmetic binary operand does not match `num`\n";
                    exit(EXIT_FAILURE);
                }
                root->type_scheme = construct_type(type_names["num"]);
            }

            break;
        }
        case Token::TOK_ADD:
        case Token::TOK_SUB:
        case Token::TOK_DIV:
        case Token::TOK_MOD:
        case Token::TOK_SHL:
        case Token::TOK_SHR:
        {
            if (left->type_scheme != construct_type(type_names["num"]) || left->type_scheme != right->type_scheme)
            {
                print_location(left->token);
                std::cerr << "Type of arithmetic binary operand does not match `num`\n";
                exit(EXIT_FAILURE);
            }
            root->type_scheme = construct_type(type_names["num"]);
            break;
        }
        case Token::TOK_LT:
        case Token::TOK_LTE:
        case Token::TOK_GT:
        case Token::TOK_GTE:
        {
            if (left->type_scheme != construct_type(type_names["num"]) || left->type_scheme != right->type_scheme)
            {
                print_location(left->token);
                std::cerr << "Type of arithmetic comparison operand does not match `num`\n";
                exit(EXIT_FAILURE);
            }
            root->type_scheme = construct_type(type_names["bool"]);
            break;
        }
        case Token::TOK_EQ:
        case Token::TOK_NEQ:
        {
            if (left->type_scheme != right->type_scheme)
            {
                print_location(left->token);
                std::cerr << "Type of logical comparison operands do not match\n";
                exit(EXIT_FAILURE);
            }
            root->type_scheme = construct_type(type_names["bool"]);
            break;
        }
        case Token::TOK_AND:
        case Token::TOK_OR:
        {
            if (left->type_scheme != construct_type(type_names["bool"]) || left->type_scheme != right->type_scheme)
            {
                print_location(left->token);
                std::cerr << "Type of logical binary operand does not match `bool`\n";
                exit(EXIT_FAILURE);
            }
            root->type_scheme = construct_type(type_names["bool"]);
            break;
        }
        default:
        {
            root->type_scheme = TypeScheme();
            break;
        }
    }
}

static void type_check_op(std::unique_ptr<Node> &root)
{
    switch (root->children.size())
    {
        case 1:
        {
            type_check_un_op(root);
            break;
        }
        case 2:
        {
            type_check_bin_op(root);
            break;
        }
        default:
        {
            break;
        }
    }
}

static void type_check_fun_call(std::unique_ptr<Node> &root)
{
    root->type_scheme = TypeScheme(TypeScheme::FUN_TYPE);

    if (root->children.size())
    {
        for (size_t i = 0; i < root->children.size(); i++)
        {
            std::unique_ptr<Node> &node = root->children[i];
            type_check_expr(node);

            switch (node->type_scheme.type)
            {
                case TypeScheme::ALPHA:
                case TypeScheme::FUN_TYPE:
                {
                    root->type_scheme.params.push_back(node->type_scheme.alpha);
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }
    else
    {
        root->type_scheme.params.push_back(type_names["unit"]);
    }

    if (root->type_scheme.params != functions[root->token.data].params)
    {
        print_location(root->token);
        std::cerr << "Invalid parameter types to function `" << root->token.data << "`\n";
        exit(EXIT_FAILURE);
    }

    root->type_scheme.alpha = functions[root->token.data].alpha;
}

static void type_check_var(std::unique_ptr<Node> &root)
{
    TypeScheme ts = type_scopes.top()[root->token.data];
    uint32_t arr_dim = ts.arr_dim;

    if (ts.type == TypeScheme::ALPHA && arr_dim)
    {
        for (size_t i = 0; i < root->children.size(); i++)
        {
            type_check_expr(root->children[i]);
            if (root->children[i]->type_scheme != construct_type(type_names["num"]))
            {
                print_location(root->children[i]->token);
                std::cerr << "Expected array index of type `num`\n";
                exit(EXIT_FAILURE);
            }
        }

        if (root->children.size() > arr_dim)
        {
            print_location(root->token);
            std::cerr << "Cannot index more times than declared array dimension\n";
            exit(EXIT_FAILURE);
        }

        root->type_scheme = ts;
        root->type_scheme.arr_dim = arr_dim - root->children.size();
    }
    else
    {
        root->type_scheme = ts;
    }
}

static void type_check_var_asn(std::unique_ptr<Node> &root)
{
    std::unique_ptr<Node> &left = root->children.front();
    std::unique_ptr<Node> &right = root->children.back();
    type_check_expr(right);

    TypeScheme ts = right->type_scheme;

    if (ts == construct_type(type_names["unit"]))
    {
        print_location(right->token);
        std::cerr << "Cannot assign variable to statement of type `unit`\n";
        exit(EXIT_FAILURE);
    }

    if (left->children.size())
    {
        uint32_t index_depth = 0;

        for (size_t i = 0; i < root->children.size(); i++)
        {
            type_check_expr(root->children[i]);
            switch (root->children[i]->type_scheme.type)
            {
                case TypeScheme::ALPHA:
                case TypeScheme::FUN_TYPE:
                {
                    if (root->children[i]->type_scheme != construct_type(type_names["num"]))
                    {
                        print_location(root->children[i]->token);
                        std::cerr << "Expected array index of type `num`\n";
                        exit(EXIT_FAILURE);
                    }

                    index_depth++;
                    break;
                }
                default:
                {
                    print_location(root->children[i]->token);
                    std::cerr << "Expected array index of type `num`\n";
                    exit(EXIT_FAILURE);
                }
            }
        }

        TypeScheme a = type_scopes.top()[left->token.data];
        uint32_t arr_dim = a.arr_dim;
        if (!arr_dim)
        {
            print_location(left->token);
            std::cerr << "Can only index variables of type array\n";
            exit(EXIT_FAILURE);
        }

        if (index_depth > arr_dim)
        {
            print_location(left->token);
            std::cerr << "Cannot index more times than declared array dimension\n";
            exit(EXIT_FAILURE);
        }

        a.arr_dim = arr_dim - index_depth;

        if (a != ts)
        {
            print_location(left->token);
            std::cerr << "Array slice types do not match\n";
            exit(EXIT_FAILURE);
        }

        left->type_scheme = a;
    }
    else
    {
        if (type_scopes.top().count(left->token.data) && ts != type_scopes.top()[left->token.data])
        {
            print_location(left->token);
            std::cerr << "Initially defined type of variable `" << left->token.data << "` does not match new definition\n";
            exit(EXIT_FAILURE);
        }

        left->type_scheme = ts;
        type_scopes.top()[left->token.data] = ts;
    }

    root->type_scheme = TypeScheme(TypeScheme::ALPHA);
    root->type_scheme.alpha = type_names["unit"];
}

static void type_check_if(std::unique_ptr<Node> &root)
{
    switch (root->children.size())
    {
        /* only body, no condition (else clause) */
        case 1:
        {
            std::unique_ptr<Node> &body = root->children.front();

            type_scopes.push(type_scopes.top());
            for (size_t i = 0; i < body->children.size(); i++)
            {
                type_check_expr(body->children[i]);
            }
            type_scopes.pop();

            if (body->children.size())
            {
                body->type_scheme = body->children.back()->type_scheme;
            }
            else
            {
                body->type_scheme = construct_type(type_names["unit"]);
            }

            root->type_scheme = body->type_scheme;
            break;
        }
        /* condition and body (if without else clause) */
        case 2:
        {
            std::unique_ptr<Node> &cond = root->children.front();
            type_check_expr(cond);

            std::unique_ptr<Node> &body = root->children.back();
            TypeScheme ts = cond->type_scheme;

            if (ts != construct_type(type_names["bool"]))
            {
                print_location(cond->token);
                std::cerr << "Type of conditional in if-clause does not match type `bool`\n";
                exit(EXIT_FAILURE);
            }

            type_scopes.push(type_scopes.top());
            for (size_t i = 0; i < body->children.size(); i++)
            {
                type_check_expr(body->children[i]);
            }
            type_scopes.pop();

            if (body->children.size())
            {
                body->type_scheme = body->children.back()->type_scheme;
            }
            else
            {
                body->type_scheme = construct_type(type_names["unit"]);
            }

            if (body->type_scheme != construct_type(type_names["unit"]))
            {
                print_location(root->token);
                std::cerr << "Cannot compile typed if-expression without else-branch\n";
                exit(EXIT_FAILURE);
            }

            root->type_scheme = body->type_scheme;
            break;
        }
        /* condition and body (if with else clause) */
        case 3:
        {
            std::unique_ptr<Node> &cond = root->children.front();
            std::unique_ptr<Node> &body = root->children[1];
            std::unique_ptr<Node> &els = root->children.back();

            type_check_expr(cond);
            TypeScheme ts = cond->type_scheme;

            if (ts != construct_type(type_names["bool"]))
            {
                print_location(cond->token);
                std::cerr << "Type of conditional in if-else-clause does not match type `bool`\n";
                exit(EXIT_FAILURE);
            }
            
            type_scopes.push(type_scopes.top());

            for (size_t i = 0; i < body->children.size(); i++)
            {
                type_check_expr(body->children[i]);
            }
            type_scopes.pop();

            if (body->children.size())
            {
                body->type_scheme = body->children.back()->type_scheme;
            }
            else
            {
                body->type_scheme = construct_type(type_names["unit"]);
            }

            std::unique_ptr<Node> p = std::move(curr_fun_outs);
            type_check_if(els);
            curr_fun_outs = std::move(p);

            if (body->type_scheme != els->type_scheme)
            {
                print_location(root->token);
                std::cerr << "Type of if-clause does not match type of else-clause\n";
                exit(EXIT_FAILURE);
            }

            root->type_scheme = body->type_scheme;

            root->children.front() = std::move(cond);
            root->children[1] = std::move(body);
            root->children.back() = std::move(els);

            break;
        }
        default:
        {
            break;
        }
    }

    if (curr_fun_outs != nullptr)
    {
        root->type_scheme = TypeScheme(TypeScheme::FUN_TYPE);

        switch (root->children.back()->children.back()->type_scheme.type)
        {
            case TypeScheme::ALPHA:
            case TypeScheme::FUN_TYPE:
            {
                root->type_scheme.alpha = root->children.back()->children.back()->type_scheme.alpha;
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

static void type_check_loop(std::unique_ptr<Node> &root)
{
    type_check_expr(root->children.front());
    if (root->children.front()->type_scheme != construct_type(type_names["bool"]))
    {
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < root->children.back()->children.size(); i++)
    {
        type_check_expr(root->children.back()->children[i]);
    }

    root->type_scheme = TypeScheme(TypeScheme::ALPHA);
    root->type_scheme.alpha = type_names["unit"];
}

static void type_check_expr(std::unique_ptr<Node> &root)
{
    switch (root->type)
    {
        case Node::NODE_LIT:
        {
            type_check_literal(root);
            break;
        }
        case Node::NODE_OP:
        {
            type_check_op(root);
            break;
        }
        case Node::NODE_KEY:
        {
            type_check_key(root);
            break;
        }
        case Node::NODE_FUN_CALL:
        {
            type_check_fun_call(root);
            break;
        }
        case Node::NODE_BUILT_IN:
        {
            type_check_built_in(root);
            break;
        }
        case Node::NODE_VAR:
        {
            type_check_var(root);
            break;
        }
        case Node::NODE_VAR_ASN:
        {
            type_check_var_asn(root);
            break;
        }
        case Node::NODE_IF:
        {
            type_check_if(root);
            break;
        }
        case Node::NODE_LOOP:
        {
            type_check_loop(root);
            break;
        }
        default:
        {
            break;
        }
    }
}

static void type_check_fun_dec(std::unique_ptr<Node> &root)
{
    type_scopes.push(TypeScope());

    std::unique_ptr<Node> &in = root->children.front();
    std::unique_ptr<Node> &out = root->children[1];
    std::unique_ptr<Node> &body = root->children.back();

    root->type_scheme = TypeScheme(TypeScheme::FUN_TYPE);

    bool unit_input = in->children.front()->token.type == Token::KEY_UNIT;
    for (size_t i = 0; i < in->children.size(); i++)
    {
        /* TODO: handle array types as parameters as well */
        root->type_scheme.params.push_back(type_names[in->children[i]->token.data]);
    }

    /* TODO: handle array types as returns as well */
    root->type_scheme.alpha = type_names[out->children.front()->token.data];
    functions[root->token.data] = root->type_scheme;

    size_t i = 0;

    if (!unit_input)
    {
        for (; i < root->children.front()->children.size(); i++)
        {
            TypeScheme ts = TypeScheme(TypeScheme::ALPHA);
            ts.alpha = root->type_scheme.params[i];
            type_scopes.top()[body->children[i]->token.data] = ts;
            body->children[i]->type_scheme = ts;
        }

        curr_fun_outs = std::move(out);
    }

    for (; i < body->children.size(); i++)
    {
        type_check_expr(body->children[i]);
    }

    if (!unit_input)
    {
        out = std::move(curr_fun_outs);
    }

    type_scopes.pop();
}

static void type_check_program(std::unique_ptr<Node> &root)
{
    type_scopes.push(TypeScope());
    for (size_t i = 0; i < root->children.size(); i++)
    {
        /* TODO: type check function declarations instead of just statements */
        if (root->children[i]->type == Node::NODE_FUN_DEC)
        {
            type_check_fun_dec(root->children[i]);
        }
        else
        {
            type_check_expr(root->children[i]);
        }
    }

    type_scopes.pop();
}

void type_check(std::unique_ptr<Node> &root)
{
    initialize_types();
    type_check_program(root);
}

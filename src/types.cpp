#include "types.hpp"

static std::unordered_map<std::string, TypeScheme> functions;

typedef std::unordered_map<std::string, TypeScheme> TypeScope;
std::stack<TypeScope> type_scopes;

std::unique_ptr<Node> curr_fun_outs = nullptr;

/* TypeScheme operator overloading */
bool operator==(const TypeScheme &left, const TypeScheme &right)
{
    if (left.type == right.type)
    {
        switch (left.type)
        {
            case TypeScheme::ALPHA:
            {
                return left.alpha == right.alpha;
            }
            case TypeScheme::FUN_TYPE:
            {
                return left.params == right.params && left.alpha == right.alpha;
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
        return left.alpha == right.alpha;
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
    new_type("nil");
    new_type("num");
    new_type("bool");
    new_type("str");
}

static std::unique_ptr<Node> type_check_expr(std::unique_ptr<Node> root);
static std::unique_ptr<Node> type_check_expr(std::unique_ptr<Node> root);

static std::unique_ptr<Node> type_check_key(std::unique_ptr<Node> root)
{
    root->type_scheme = construct_type(type_names["key"]);
    return root;
}

std::unique_ptr<Node> type_check_built_in(std::unique_ptr<Node> root)
{
    switch (root->token.type)
    {
        case Token::KEY_PRINT:
        {
            root->children.front() = type_check_expr(std::move(root->children.front()));
            TypeScheme ts = root->children.front()->type_scheme;
            if (ts == construct_type(type_names["num"]) ||
                ts == construct_type(type_names["bool"]) ||
                ts == construct_type(type_names["str"]))
            {
                root->type_scheme = construct_type(type_names["nil"]);
                return root;
            }
            std::cerr << "Expected input of type `num`, `bool`, or `str`\n";
            exit(EXIT_FAILURE);
        }
        default:
        {
            break;
        }
    }

    return root;
}

static std::unique_ptr<Node> type_check_literal(std::unique_ptr<Node> root)
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
            root->children.front() = type_check_expr(std::move(root->children.front()));
            TypeScheme ts = root->children.front()->type_scheme;

            for (size_t i = 1; i < root->children.size(); i++)
            {
                root->children[i] = type_check_expr(std::move(root->children[i]));
                if (ts != root->children[i]->type_scheme)
                {
                    print_location(root->children[i]->token);
                    std::cerr << "Cannot initialize array of multiple types\n";
                    exit(EXIT_FAILURE);
                }
            }

            root->type_scheme = TypeScheme(TypeScheme::ALPHA);
            uint32_t dimen = root->children.front()->type_scheme.alpha >> 28;
            root->type_scheme.alpha = (dimen + 1) << 28;

            switch (ts.type)
            {
                case TypeScheme::ALPHA:
                case TypeScheme::FUN_TYPE:
                {
                    root->type_scheme.alpha |= ts.alpha;
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

    return root;
}

static std::unique_ptr<Node> type_check_un_op(std::unique_ptr<Node> root)
{
    std::unique_ptr<Node> operand = type_check_expr(std::move(root->children.front()));
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

    root->children.front() = std::move(operand);
    return root;
}

static std::unique_ptr<Node> type_check_bin_op(std::unique_ptr<Node> root)
{
    std::unique_ptr<Node> left = type_check_expr(std::move(root->children.front()));
    std::unique_ptr<Node> right = type_check_expr(std::move(root->children.back()));

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
                if (left->token.type != Token::TOK_NUM)
                {
                    print_location(left->token);
                    std::cerr << "Expected constant value of type `num` for size of array\n";
                    exit(EXIT_FAILURE);
                }

                root->type_scheme = TypeScheme(TypeScheme::ALPHA);
                root->type_scheme.alpha = (1 << 28);
                root->type_scheme.alpha |= type_names[right->token.data];

                break;
            }
            else if (right->type_scheme == construct_type(type_names["num"]))
            {
                uint32_t r_alpha = right->type_scheme.alpha;
                uint32_t arr_size = r_alpha >> 28;
                if (arr_size)
                {
                    root->type_scheme = TypeScheme(TypeScheme::ALPHA);
                    root->type_scheme.alpha = (arr_size + 1) << 28;
                    root->type_scheme.alpha |= type_names[right->token.data];
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

    root->children.front() = std::move(left);
    root->children.back() = std::move(right);

    return root;
}

static std::unique_ptr<Node> type_check_op(std::unique_ptr<Node> root)
{
    switch (root->children.size())
    {
        case 1:
        {
            root = type_check_un_op(std::move(root));
            break;
        }
        case 2:
        {
            root = type_check_bin_op(std::move(root));
            break;
        }
        default:
        {
            break;
        }
    }

    return root;
}

static std::unique_ptr<Node> type_check_fun_call(std::unique_ptr<Node> root)
{
    root->type_scheme = TypeScheme(TypeScheme::FUN_TYPE);

    if (root->children.size())
    {
        for (size_t i = 0; i < root->children.size(); i++)
        {
            std::unique_ptr<Node> node = type_check_expr(std::move(root->children[i]));

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

            root->children[i] = std::move(node);
        }
        
        root->type_scheme.alpha = functions[root->token.data].alpha;
        return root;
    }
    else
    {
        root->type_scheme.params.push_back(type_names["nil"]);
        root->type_scheme.alpha = functions[root->token.data].alpha;
        return root;
    }

    print_location(root->token);
    std::cerr << "Invalid parameter types to function `" << root->token.data << "`\n";
    exit(EXIT_FAILURE);
}

static std::unique_ptr<Node> type_check_var(std::unique_ptr<Node> root)
{
    TypeScheme ts = type_scopes.top()[root->token.data];

    uint32_t arr_size = ts.alpha >> 28;
    if (ts.type == TypeScheme::ALPHA && arr_size)
    {
        for (size_t i = 0; i < root->children.size(); i++)
        {
            root->children[i] = type_check_expr(std::move(root->children[i]));
            if (root->children[i]->type_scheme != construct_type(type_names["num"]))
            {
                print_location(root->children[i]->token);
                std::cerr << "Expected array index of type `num`\n";
                exit(EXIT_FAILURE);
            }
        }

        if (root->children.size() > arr_size)
        {
            print_location(root->token);
            std::cerr << "Cannot index more times than declared array dimension\n";
            exit(EXIT_FAILURE);
        }

        root->type_scheme = ts;
        root->type_scheme.alpha &= ~(0b1111 << 28);
        root->type_scheme.alpha |= (arr_size - root->children.size()) << 28;
    }
    else
    {
        root->type_scheme = ts;
    }

    return root;
}

static std::unique_ptr<Node> type_check_var_asn(std::unique_ptr<Node> root)
{
    std::unique_ptr<Node> left = std::move(root->children.front());
    std::unique_ptr<Node> right = type_check_expr(std::move(root->children.back()));

    TypeScheme ts = right->type_scheme;

    if (ts == construct_type(type_names["nil"]))
    {
        print_location(right->token);
        std::cerr << "Cannot assign variable to statement of type `nil`\n";
        exit(EXIT_FAILURE);
    }

    if (left->children.size())
    {
        uint32_t index_depth = 0;

        for (size_t i = 0; i < root->children.size(); i++)
        {
            type_check_expr(std::move(root->children[i]));
            switch (root->children[i]->type_scheme.type)
            {
                case TypeScheme::ALPHA:
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
                case TypeScheme::FUN_TYPE:
                {
                    auto ret = root->children[i]->type_scheme.alpha;
                    if (ret != type_names["num"])
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
        uint32_t arr_size = a.alpha >> 28;
        if (!arr_size)
        {
            print_location(left->token);
            std::cerr << "Can only index variables of type array\n";
            exit(EXIT_FAILURE);
        }

        if (index_depth > arr_size)
        {
            print_location(left->token);
            std::cerr << "Cannot index more times than declared array dimension\n";
            exit(EXIT_FAILURE);
        }

        a.alpha &= ~(0b1111 << 28);
        a.alpha |= (arr_size - index_depth) << 28;

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
    root->type_scheme.alpha = type_names["nil"];

    root->children.front() = std::move(left);
    root->children.back() = std::move(right);

    return root;
}

static std::unique_ptr<Node> type_check_if(std::unique_ptr<Node> root)
{
    switch (root->children.size())
    {
        /* only body, no condition (else clause) */
        case 1:
        {
            std::unique_ptr<Node> body = std::move(root->children.front());

            type_scopes.push(type_scopes.top());
            for (size_t i = 0; i < body->children.size(); i++)
            {
                body->children[i] = type_check_expr(std::move(body->children[i]));
            }
            type_scopes.pop();

            if (body->children.size())
            {
                body->type_scheme = body->children.back()->type_scheme;
            }
            else
            {
                body->type_scheme = construct_type(type_names["nil"]);
            }

            root->type_scheme = body->type_scheme;
            root->children.front() = std::move(body);

            break;
        }
        /* condition and body (if without else clause) */
        case 2:
        {
            std::unique_ptr<Node> cond = type_check_expr(std::move(root->children.front()));
            std::unique_ptr<Node> body = std::move(root->children.back());

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
                body->children[i] = type_check_expr(std::move(body->children[i]));
            }
            type_scopes.pop();

            if (body->children.size())
            {
                body->type_scheme = body->children.back()->type_scheme;
            }
            else
            {
                body->type_scheme = construct_type(type_names["nil"]);
            }

            if (body->type_scheme != construct_type(type_names["nil"]))
            {
                print_location(root->token);
                std::cerr << "Cannot compile typed if-expression without else-branch\n";
                exit(EXIT_FAILURE);
            }

            root->type_scheme = body->type_scheme;

            root->children.front() = std::move(cond);
            root->children.back() = std::move(body);

            break;
        }
        /* condition and body (if with else clause) */
        case 3:
        {
            std::unique_ptr<Node> cond = type_check_expr(std::move(root->children.front()));
            std::unique_ptr<Node> body = std::move(root->children[1]);
            std::unique_ptr<Node> els = std::move(root->children.back());

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
                body->children[i] = type_check_expr(std::move(body->children[i]));
            }
            type_scopes.pop();

            if (body->children.size())
            {
                body->type_scheme = body->children.back()->type_scheme;
            }
            else
            {
                body->type_scheme = construct_type(type_names["nil"]);
            }

            std::unique_ptr<Node> p = std::move(curr_fun_outs);
            els = type_check_if(std::move(els));
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

    return root;
}

static std::unique_ptr<Node> type_check_loop(std::unique_ptr<Node> root)
{
    root->children.front() = type_check_expr(std::move(root->children.front()));
    if (root->children.front()->type_scheme != construct_type(type_names["bool"]))
    {
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < root->children.back()->children.size(); i++)
    {
        root->children.back()->children[i] = type_check_expr(std::move(root->children.back()->children[i]));
    }

    root->type_scheme = TypeScheme(TypeScheme::ALPHA);
    root->type_scheme.alpha = type_names["nil"];

    return root;
}

static std::unique_ptr<Node> type_check_expr(std::unique_ptr<Node> root)
{
    switch (root->type)
    {
        case Node::NODE_LIT:
        {
            root = type_check_literal(std::move(root));
            break;
        }
        case Node::NODE_OP:
        {
            root = type_check_op(std::move(root));
            break;
        }
        case Node::NODE_KEY:
        {
            root = type_check_key(std::move(root));
            break;
        }
        case Node::NODE_FUN_CALL:
        {
            root = type_check_fun_call(std::move(root));
            break;
        }
        case Node::NODE_BUILT_IN:
        {
            root = type_check_built_in(std::move(root));
            break;
        }
        case Node::NODE_VAR:
        {
            root = type_check_var(std::move(root));
            break;
        }
        case Node::NODE_VAR_ASN:
        {
            root = type_check_var_asn(std::move(root));
            break;
        }
        case Node::NODE_IF:
        {
            root = type_check_if(std::move(root));
            break;
        }
        case Node::NODE_LOOP:
        {
            root = type_check_loop(std::move(root));
            break;
        }
        default:
        {
            break;
        }
    }

    return root;
}

static std::unique_ptr<Node> type_check_fun_dec(std::unique_ptr<Node> root)
{
    type_scopes.push(TypeScope());

    std::unique_ptr<Node> in = std::move(root->children.front());
    std::unique_ptr<Node> out = std::move(root->children[1]);
    std::unique_ptr<Node> body = std::move(root->children.back());

    root->type_scheme = TypeScheme(TypeScheme::FUN_TYPE);

    bool nil_input = in->children.front()->token.type == Token::KEY_NIL;
    for (size_t i = 0; i < in->children.size(); i++)
    {
        /* TODO: handle array types as parameters as well */
        root->type_scheme.params.push_back(type_names[in->children[i]->token.data]);
    }

    /* TODO: handle array types as returns as well */
    root->type_scheme.alpha = type_names[out->children.front()->token.data];

    root->children.front() = std::move(in);

    functions[root->token.data] = root->type_scheme;

    size_t i = 0;

    if (!nil_input)
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
        body->children[i] = type_check_expr(std::move(body->children[i]));
    }

    type_scopes.pop();

    root->children[1] = std::move(nil_input ? out : curr_fun_outs);
    root->children.back() = std::move(body);

    return root;
}

static std::unique_ptr<Node> type_check_program(std::unique_ptr<Node> root)
{
    type_scopes.push(TypeScope());
    for (size_t i = 0; i < root->children.size(); i++)
    {
        /* TODO: type check function declarations instead of just statements */
        if (root->children[i]->type == Node::NODE_FUN_DEC)
        {
            root->children[i] = type_check_fun_dec(std::move(root->children[i]));
        }
        else
        {
            root->children[i] = type_check_expr(std::move(root->children[i]));
        }
    }

    type_scopes.pop();
    return root;
}

std::unique_ptr<Node> type_check(std::unique_ptr<Node> root)
{
    initialize_types();
    return type_check_program(std::move(root));
}

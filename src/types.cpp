#include "types.hpp"
#include "error.hpp"

typedef std::unordered_map<std::string, TypeScheme> TypeScope;
std::stack<TypeScope> type_scopes;

void print_type(TypeScheme ts)
{
    switch (ts.type)
    {
        case TypeScheme::ALPHA:
        {
            std::cerr << "ALPHA:\t";
            if (ts.alpha >> 28)
            {
                std::cerr << (ts.alpha >> 28) << " * ";
                ts.alpha &= ~(0b1111 << 28);
                std::cerr << type_idens[ts.alpha] << "\n";
            }
            else
            {
                std::cerr << type_idens[ts.alpha] << "\n";
            }
            break;
        }
        case TypeScheme::FUN_TYPE:
        {
            std::cerr << "FUN_TYPE:\tIN:\t";
            for (auto x : ts.fun_type.first)
            {
                if (x >> 28)
                {
                    std::cerr << (x >> 28) << " * ";
                    x &= ~(0b1111 << 28);
                }
                std::cerr << type_idens[x] << "\t";
            }
            std::cerr << "OUT:\t";
            for (auto x : ts.fun_type.second)
            {
                if (x >> 28)
                {
                    std::cerr << (x >> 28) << " * ";
                    x &= ~(0b1111 << 28);
                }
                std::cerr << type_idens[x] << "\t";
            }
            std::cerr << "\n";
            break;
        }
        default:
        {
            std::cerr << "NONE\n";
            break;
        }
    }
}

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
                return left.fun_type == right.fun_type;
            }
            default:
            {
                return true;
            }
        }
    }

    if (left.type == TypeScheme::ALPHA && right.type == TypeScheme::FUN_TYPE)
    {
        return right.fun_type.second.size() == 1 && right.fun_type.second.front() == left.alpha;
    }

    if (right.type == TypeScheme::ALPHA && left.type == TypeScheme::FUN_TYPE)
    {
        return left.fun_type.second.size() == 1 && left.fun_type.second.front() == right.alpha;
    }

    return false;
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

void initialize_types()
{
    new_type("key");
    new_type("nil");
    new_type("num");
    new_type("bool");
    new_type("str");
}

static void type_check_expr(Node *root, Node *func);
static void type_check_statement(Node *root, Node *func);

static void type_check_key(Node *root)
{
    root->type_scheme = construct_type(type_names["key"]);
}

static void type_check_literal(Node *root)
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
            auto children = root->children;
            type_check_expr(children.front(), nullptr);
            TypeScheme ts = children.front()->type_scheme;

            for (size_t i = 1; i < children.size(); i++)
            {
                type_check_expr(children[i], nullptr);
                if (ts != children[i]->type_scheme)
                {
                    print_location(children[i]->token);
                    std::cerr << "Cannot initialize array of multiple types\n";
                    exit(EXIT_FAILURE);
                }
            }

            root->type_scheme = TypeScheme(TypeScheme::ALPHA);
            uint32_t dimen = children.front()->type_scheme.alpha >> 28;
            root->type_scheme.alpha = (dimen + 1) << 28;

            switch (ts.type)
            {
                case TypeScheme::ALPHA:
                {
                    root->type_scheme.alpha |= ts.alpha;
                    break;
                }
                case TypeScheme::FUN_TYPE:
                {
                    root->type_scheme.alpha |= ts.fun_type.second.front();
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

static void type_check_un_op(Node *root)
{
    Node *operand = root->children.front();
    type_check_expr(operand, nullptr);

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

static void type_check_bin_op(Node *root)
{
    Node *left = root->children.front(), *right = root->children.back();
    type_check_expr(left, nullptr);
    type_check_expr(right, nullptr);

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
                uint32_t r_alpha = right->type_scheme.type == TypeScheme::FUN_TYPE ? right->type_scheme.fun_type.second.front() : right->type_scheme.alpha;
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
}

static void type_check_op(Node *root)
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

static void type_check_fun_call(Node *root)
{
    root->type_scheme = TypeScheme(TypeScheme::FUN_TYPE);

    if (root->children.size())
    {
        for (auto node : root->children)
        {
            type_check_expr(node, nullptr);

            switch (node->type_scheme.type)
            {
                case TypeScheme::ALPHA:
                {
                    root->type_scheme.fun_type.first.push_back(node->type_scheme.alpha);
                    break;
                }
                case TypeScheme::FUN_TYPE:
                {
                    root->type_scheme.fun_type.first = node->type_scheme.fun_type.second;
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
        root->type_scheme.fun_type.first.push_back(type_names["nil"]);
    }

    std::string fun_name = root->token.data;
    for (auto node : functions[fun_name])
    {
        if (node->type_scheme.fun_type.first == root->type_scheme.fun_type.first)
        {
            root->type_scheme.fun_type.second = node->type_scheme.fun_type.second;
            return;
        }
    }

    print_location(root->token);
    std::cerr << "Invalid parameter types to function `" << root->token.data << "`\n";
    exit(EXIT_FAILURE);
}

static void type_check_var(Node *root)
{
    TypeScheme ts = type_scopes.top()[root->token.data];

    uint32_t arr_size = ts.alpha >> 28;
    if (ts.type == TypeScheme::ALPHA && arr_size)
    {
        auto children = root->children;
        for (size_t i = 0; i < children.size(); i++)
        {
            type_check_expr(children[i], nullptr);
            if (children[i]->type_scheme != construct_type(type_names["num"]))
            {
                print_location(children[i]->token);
                std::cerr << "Expected array index of type `num`\n";
                exit(EXIT_FAILURE);
            }
        }

        if (children.size() > arr_size)
        {
            print_location(root->token);
            std::cerr << "Cannot index more times than declared array dimension\n";
            exit(EXIT_FAILURE);
        }

        root->type_scheme = ts;
        root->type_scheme.alpha &= ~(0b1111 << 28);
        root->type_scheme.alpha |= (arr_size - children.size()) << 28;
    }
    else
    {
        root->type_scheme = ts;
    }
}

static void type_check_var_asn(Node *root, Node *func)
{
    Node *left = root->children.front();
    Node *right = root->children.back();

    type_check_expr(right, func);
    TypeScheme ts = right->type_scheme;

    if (ts == construct_type(type_names["nil"]))
    {
        print_location(right->token);
        std::cerr << "Cannot assign variable to statement of type `nil`\n";
        exit(EXIT_FAILURE);
    }

    if (left->children.size())
    {
        auto children = left->children;
        uint32_t index_depth = 0;

        for (size_t i = 0; i < children.size(); i++)
        {
            type_check_expr(children[i], nullptr);
            switch (children[i]->type_scheme.type)
            {
                case TypeScheme::ALPHA:
                {
                    if (children[i]->type_scheme != construct_type(type_names["num"]))
                    {
                        print_location(children[i]->token);
                        std::cerr << "Expected array index of type `num`\n";
                        exit(EXIT_FAILURE);
                    }

                    index_depth++;
                    break;
                }
                case TypeScheme::FUN_TYPE:
                {
                    auto rets = children[i]->type_scheme.fun_type.second;
                    for (size_t j = 0; j < rets.size(); j++)
                    {
                        if (rets[j] != type_names["num"])
                        {
                            print_location(children[j]->token);
                            std::cerr << "Expected array index of type `num`\n";
                            exit(EXIT_FAILURE);
                        }

                        index_depth++;
                    }
                    break;
                }
                default:
                {
                    print_location(children[i]->token);
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
}

static void type_check_if(Node *root, Node *func)
{
    switch (root->children.size())
    {
        /* only body, no condition (else clause) */
        case 1:
        {
            Node *body = root->children.front();

            type_scopes.push(type_scopes.top());
            for (auto child : body->children)
            {
                type_check_statement(child, func);
            }
            type_scopes.pop();
            body->type_scheme = body->children.back()->type_scheme;

            root->type_scheme = body->type_scheme;
            break;
        }
        /* condition and body (if without else clause) */
        case 2:
        {
            Node *cond = root->children.front();
            Node *body = root->children.back();

            type_check_expr(cond, nullptr);

            TypeScheme ts = cond->type_scheme;

            if (ts != construct_type(type_names["bool"]))
            {
                print_location(cond->token);
                std::cerr << "Type of conditional in if-clause does not match type `bool`\n";
                exit(EXIT_FAILURE);
            }

            type_scopes.push(type_scopes.top());
            for (auto child : body->children)
            {
                type_check_statement(child, func);
            }
            type_scopes.pop();
            body->type_scheme = body->children.back()->type_scheme;

            switch (body->type_scheme.type)
            {
                case TypeScheme::ALPHA:
                {
                    if (body->type_scheme != construct_type(type_names["nil"]))
                    {
                        print_location(root->token);
                        std::cerr << "Cannot compile typed if-expression without else-branch\n";
                        exit(EXIT_FAILURE);
                    }

                    break;
                }
                case TypeScheme::FUN_TYPE:
                {
                    if (body->type_scheme.fun_type.second.size() > 1 ||
                        body->type_scheme.fun_type.second.front() != type_names["nil"])
                    {
                        print_location(root->token);
                        std::cerr << "Cannot compile typed if-expression without else-branch\n";
                        exit(EXIT_FAILURE);
                    }

                    break;
                }
                default:
                {
                    break;
                }
            }

            root->type_scheme = body->type_scheme;
            break;
        }
        /* condition and body (if with else clause) */
        case 3:
        {
            Node *cond = root->children.front();
            Node *body = root->children[1];
            Node *els = root->children.back();

            type_check_expr(cond, nullptr);

            TypeScheme ts = cond->type_scheme;

            if (ts != construct_type(type_names["bool"]))
            {
                print_location(cond->token);
                std::cerr << "Type of conditional in if-else-clause does not match type `bool`\n";
                std::cerr << type_idens[cond->type_scheme.alpha] << "\n";
                exit(EXIT_FAILURE);
            }
            
            type_scopes.push(type_scopes.top());
            for (auto child : body->children)
            {
                type_check_statement(child, func);
            }
            type_scopes.pop();

            body->type_scheme = body->children.back()->type_scheme;

            type_check_if(els, func);

            if (body->type_scheme != root->children.back()->type_scheme)
            {
                print_location(root->token);
                std::cerr << "Type of if-clause does not match type of else-clause\n";
                exit(EXIT_FAILURE);
            }

            root->type_scheme = body->type_scheme;

            break;
        }
        default:
        {
            break;
        }
    }

    if (func != nullptr && root == func->children.back())
    {
        root->type_scheme = TypeScheme(TypeScheme::FUN_TYPE);

        size_t num_outs = func->children[1]->children.size();
        size_t size = root->children.back()->children.size();
        auto back = root->children.back()->children;

        for (size_t i = num_outs; i > 0; i--)
        {

            switch (back[size - i]->type_scheme.type)
            {
                case TypeScheme::ALPHA:
                {
                    root->type_scheme.fun_type.second.push_back(back[size - i]->type_scheme.alpha);
                    break;
                }
                case TypeScheme::FUN_TYPE:
                {
                    for (size_t j = 0; j < back[size - i]->type_scheme.fun_type.second.size(); j++)
                    {
                        root->type_scheme.fun_type.second.push_back(back[size - i]->type_scheme.fun_type.second[j]);
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
            root->type_scheme.fun_type.second.push_back(root->children.back()->children[size - i]->type_scheme.alpha);
        }
    }
}

static void type_check_expr(Node *root, Node *func)
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
        case Node::NODE_VAR:
        {
            type_check_var(root);
            break;
        }
        case Node::NODE_VAR_ASN:
        {
            type_check_var_asn(root, func);
            break;
        }
        case Node::NODE_IF:
        {
            type_check_if(root, func);
            break;
        }
        default:
        {
            break;
        }
    }
}

static void type_check_loop(Node *root)
{
    type_check_expr(root->children.front(), nullptr);
    if (root->children.front()->type_scheme != construct_type(type_names["bool"]))
    {
        exit(EXIT_FAILURE);
    }

    for (auto child : root->children.back()->children)
    {
        type_check_statement(child, nullptr);
    }

    root->type_scheme = TypeScheme(TypeScheme::ALPHA);
    root->type_scheme.alpha = type_names["nil"];
}

static void type_check_statement(Node *root, Node *func)
{
    switch (root->type)
    {
        case Node::NODE_LOOP:
        {
            type_check_loop(root);
            break;
        }
        default:
        {
            type_check_expr(root, func);
            break;
        }
    }
}

static void type_check_fun_dec(Node *root)
{
    type_scopes.push(TypeScope());
    Node *in = root->children.front();
    Node *out = root->children[1];
    Node *body = root->children.back();

    root->type_scheme = TypeScheme(TypeScheme::FUN_TYPE);

    bool nil_input = in->children.front()->token.type == Token::KEY_NIL;
    for (auto type : in->children)
    {
        /* TODO: handle array types as parameters as well */
        root->type_scheme.fun_type.first.push_back(type_names[type->token.data]);
    }

    for (auto type : out->children)
    {
        /* TODO: handle array types as returns as well */
        root->type_scheme.fun_type.second.push_back(type_names[type->token.data]);
    }

    size_t i = 0;

    if (!nil_input)
    {
        for (; i < in->children.size() && !nil_input; i++)
        {
            auto child = body->children[i];
            TypeScheme ts = TypeScheme(TypeScheme::ALPHA);
            ts.alpha = root->type_scheme.fun_type.first[i];
            type_scopes.top()[child->token.data] = ts;
            child->type_scheme = ts;
        }
    }

    for (; i < body->children.size(); i++)
    {
        auto child = body->children[i];
        type_check_statement(child, root);
    }

    type_scopes.pop();
}

void type_check(Node *root)
{
    type_scopes.push(TypeScope());
    for (auto child : root->children)
    {
        /* TODO: type check function declarations instead of just statements */
        if (child->type == Node::NODE_FUN_DEC)
        {
            type_check_fun_dec(child);
        }
        else
        {
            type_check_statement(child, nullptr);
        }
    }
}

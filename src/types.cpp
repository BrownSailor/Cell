#include "types.hpp"

typedef std::unordered_map<std::string, TypeScheme> TypeScope;
std::stack<TypeScope> type_scopes;

/* TypeScheme operator overloading */
bool operator==(const TypeScheme &left, const TypeScheme &right)
{
    if (right.type == TypeScheme::FUN_TYPE)
    {
        return left.type == right.type && left.fun_type == right.fun_type;
    }

    return left.type == right.type && (left.type == TypeScheme::NONE ? true : left.alpha_type == right.alpha_type);
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

static TypeScheme construct_type(uint32_t type)
{
    TypeScheme ts(TypeScheme::ALPHA);
    ts.alpha_type = type;
    return ts;
}

void initialize_types()
{
    new_type("nil");
    new_type("num");
    new_type("bool");
    new_type("str");
    new_type("num..num");
}

static void type_check_expr(Node *root);
static void type_check_statement(Node *root);

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
    type_check_expr(operand);

    switch (root->token.type)
    {
        case Token::TOK_SUB:
        {
            if (operand->type_scheme != construct_type(type_names["num"]))
            {
                exit(EXIT_FAILURE);
            }
            root->type_scheme = construct_type(type_names["num"]);
            break;
        }
        case Token::TOK_NOT:
        {
            if (operand->type_scheme != construct_type(type_names["bool"]))
            {
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
    type_check_expr(left);
    type_check_expr(right);

    switch (root->token.type)
    {
        case Token::TOK_ADD:
        case Token::TOK_SUB:
        case Token::TOK_MUL:
        case Token::TOK_DIV:
        case Token::TOK_MOD:
        case Token::TOK_SHL:
        case Token::TOK_SHR:
        {
            if (left->type_scheme != construct_type(type_names["num"]) || left->type_scheme != right->type_scheme)
            {
                exit(EXIT_FAILURE);
            }
            root->type_scheme = construct_type(type_names["num"]);
            break;
        }
        case Token::TOK_RANGE:
        {
            if (left->type_scheme != construct_type(type_names["num"]) || left->type_scheme != right->type_scheme)
            {
                exit(EXIT_FAILURE);
            }
            root->type_scheme = construct_type(type_names["num..num"]);
            break;
        }
        case Token::TOK_LT:
        case Token::TOK_LTE:
        case Token::TOK_GT:
        case Token::TOK_GTE:
        {
            if (left->type_scheme != construct_type(type_names["num"]) || left->type_scheme != right->type_scheme)
            {
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
    for (auto node : root->children)
    {
        type_check_expr(node);
        root->type_scheme.fun_type.first.push_back(node->type_scheme.alpha_type);
    }

    std::string fun_name = root->token.data;
    for (auto node : functions[fun_name])
    {
        TypeScheme declared = node->type_scheme;

        if (declared.fun_type.first == root->type_scheme.fun_type.first)
        {
            root->type_scheme.fun_type.second = declared.fun_type.second;
            return;
        }
    }

    exit(EXIT_FAILURE);
}

static void type_check_var(Node *root)
{
    root->type_scheme = type_scopes.top()[root->token.data];
}

static void type_check_var_asn(Node *root)
{
    type_check_expr(root->children.front());
    if (type_scopes.top().count(root->token.data) &&
        root->children.front()->type_scheme != type_scopes.top()[root->token.data])
    {
        exit(EXIT_FAILURE);
    }

    type_scopes.top()[root->token.data] = root->children.front()->type_scheme;
    root->type_scheme = root->children.front()->type_scheme;
}

static void type_check_expr(Node *root)
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
            type_check_var_asn(root);
            break;
        }
        default:
        {
            break;
        }
    }
}

static void type_check_if(Node *root)
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
                type_check_statement(child);
            }
            type_scopes.pop();

            break;
        }
        /* condition and body (if without else clause) */
        case 2:
        {
            Node *cond = root->children.front();
            Node *body = root->children.back();

            type_check_expr(cond);
            if (cond->type_scheme != construct_type(type_names["bool"]))
            {
                exit(EXIT_FAILURE);
            }

            type_scopes.push(type_scopes.top());
            for (auto child : body->children)
            {
                type_check_statement(child);
            }
            type_scopes.pop();

            break;
        }
        /* condition and body (if with else clause) */
        case 3:
        {
            Node *cond = root->children.front();
            Node *body = root->children[1];
            Node *els = root->children.back();

            type_check(cond);
            if (cond->type_scheme != construct_type(type_names["bool"]))
            {
                exit(EXIT_FAILURE);
            }
            
            type_scopes.push(type_scopes.top());
            for (auto child : body->children)
            {
                type_check_statement(child);
            }
            type_scopes.pop();

            type_check_if(els);

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
    type_check_expr(root->children.front());
    if (root->children.front()->type_scheme != construct_type(type_names["bool"]))
    {
        exit(EXIT_FAILURE);
    }

    for (auto child : root->children.back()->children)
    {
        type_check_statement(child);
    }
}

static void type_check_statement(Node *root)
{
    switch (root->type)
    {
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
            type_check_expr(root);
            break;
        }
    }
}

void type_check(Node *root)
{
    type_scopes.push(TypeScope());
    for (auto child : root->children)
    {
        /* TODO: type check function declarations instead of just statements */
        type_check_statement(child);
    }
}

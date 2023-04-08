#include "types.h"

uint32_t type_id = 0;

std::unordered_map<std::string, uint32_t> type_names;
std::unordered_map<uint32_t, std::string> type_idens;

static void new_type(std::string type)
{
    type_names[type] = type_id;
    type_idens[type_id++] = type;
}

static uint32_t construct_type(uint32_t num, uint32_t id)
{
    id &= ~((1 << 20) - 1);
    return (num << 20) | id;
}

void initialize_types()
{
    new_type("num");
    new_type("bool");
    new_type("str");
}

static void type_check_expr(Node *root);
static void type_check_statement(Node *root);

static void type_check_literal(Node *root)
{
    switch (root->token.type)
    {
        case Token::TOK_NUM:
        {
            root->datatype = construct_type(1, type_names["num"]);
            break;
        }
        case Token::TOK_TRU:
        case Token::TOK_FLS:
        {
            root->datatype = construct_type(1, type_names["bool"]);
            break;
        }
        case Token::TOK_STR:
        {
            root->datatype = construct_type(1, type_names["str"]);
            break;
        }
        default:
        {
            root->datatype = 0;
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
            if (operand->datatype != construct_type(1, type_names["num"]))
            {
                exit(EXIT_FAILURE);
            }
            root->datatype = construct_type(1, type_names["num"]);
            break;
        }
        case Token::TOK_NOT:
        {
            if (operand->datatype != construct_type(1, type_names["bool"]))
            {
                exit(EXIT_FAILURE);
            }
            root->datatype = construct_type(1, type_names["bool"]);
            break;
        }
        default:
        {
            root->datatype = 0;
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
            if (left->datatype != construct_type(1, type_names["num"]) || left->datatype != right->datatype)
            {
                exit(EXIT_FAILURE);
            }
            root->datatype = construct_type(1, type_names["num"]);
            break;
        }
        case Token::TOK_LT:
        case Token::TOK_LTE:
        case Token::TOK_GT:
        case Token::TOK_GTE:
        {
            if (left->datatype != construct_type(1, type_names["num"]) || left->datatype != right->datatype)
            {
                exit(EXIT_FAILURE);
            }
            root->datatype = construct_type(1, type_names["bool"]);
            break;
        }
        case Token::TOK_EQ:
        case Token::TOK_NEQ:
        {
            if (left->datatype != right->datatype)
            {
                exit(EXIT_FAILURE);
            }
            root->datatype = construct_type(1, type_names["bool"]);
            break;
        }
        case Token::TOK_AND:
        case Token::TOK_OR:
        {
            if (left->datatype != construct_type(1, type_names["bool"]) || left->datatype != right->datatype)
            {
                exit(EXIT_FAILURE);
            }
            root->datatype = construct_type(1, type_names["bool"]);
            break;
        }
        default:
        {
            root->datatype = 0;
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
    (void)root;
}

static void type_check_var(Node *root)
{
    (void)root;
}

static void type_check_var_asn(Node *root)
{
    (void)root;
}

static void type_check_if(Node *root)
{
    (void)root;
}

static void type_check_else(Node *root)
{
    (void)root;
}

static void type_check_loop(Node *root)
{
    (void)root;
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

static void type_check_statement(Node *root)
{
    switch (root->type)
    {
        case Node::NODE_IF:
        {
            type_check_if(root);
            break;
        }
        case Node::NODE_ELSE:
        {
            type_check_else(root);
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
    for (auto child : root->children)
    {
        /* TODO: type check function declarations instead of just statements */
        type_check_statement(child);
    }
}

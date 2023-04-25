#include "compiler.hpp"

uint32_t tmp_id = 0;
uint32_t lab_id = 0;

static void compile_expr(Node *node);
static void compile_statement(Node *node);

static std::string get_type(Node *node)
{
    switch (node->token.type)
    {
        case Token::KEY_NIL:
        {
            return "void";
        }
        case Token::KEY_NUM:
        {
            return "i64";
        }
        case Token::KEY_BOOL:
        {
            return "i1";
        }
        default:
        {
            return "";
        }
    }
}

static void compile_literal(Node *node)
{
    switch (node->token.type)
    {
        case Token::TOK_NUM:
        {
            std::cout << "    %t" << tmp_id << " = add i64 0, " << node->token.data << "\n";
            node->id = tmp_id++;
            break;
        }
        case Token::TOK_TRU:
        {
            std::cout << "    %t" << tmp_id << " = add i1 0, 1\n";
            node->id = tmp_id++;
            break;
        }
        case Token::TOK_FLS:
        {
            std::cout << "    %t" << tmp_id << " = add i1 0, 0\n";
            node->id = tmp_id++;
            break;
        }
        default:
        {
            /* TODO: implement string literals */
            break;
        }
    }
}

static void compile_un_op(Node *node)
{
    Node *operand = node->children.front();
    compile_expr(operand);

    switch (node->token.type)
    {
        case Token::TOK_NOT:
        {
            std::cout << "    %t" << tmp_id << " = xor i1 1, %t" << operand->id << "\n";
            node->id = tmp_id++;
            break;
        }
        case Token::TOK_SUB:
        {
            std::cout << "    %t" << tmp_id << " = i64 mul -1, %t" << operand->id << "\n";
            node->id = tmp_id++;
            break;
        }
        default:
        {
            break;
        }
    }
}

static void compile_bin_op(Node *node)
{
    Node *left = node->children.front();
    Node *right = node->children.back();

    compile_expr(left);
    compile_expr(right);

    switch (node->token.type)
    {
        case Token::TOK_ADD:
        {
            std::cout << "    %t" << tmp_id << " = add i64 %t" << left->id << ", %t" << right->id << "\n";
            node->id = tmp_id++;
            break;
        }
        case Token::TOK_SUB:
        {
            std::cout << "    %t" << tmp_id << " = sub i64 %t" << left->id << ", %t" << right->id << "\n";
            node->id = tmp_id++;
            break;
        }
        case Token::TOK_MUL:
        {
            std::cout << "    %t" << tmp_id << " = mul i64 %t" << left->id << ", %t" << right->id << "\n";
            node->id = tmp_id++;
            break;
        }
        case Token::TOK_DIV:
        {
            std::cout << "    %t" << tmp_id << " = sdiv i64 %t" << left->id << ", %t" << right->id << "\n";
            node->id = tmp_id++;
            break;
        }
        case Token::TOK_SHL:
        {
            std::cout << "    %t" << tmp_id << " = shl i64 %t" << left->id << ", %t" << right->id << "\n";
            node->id = tmp_id++;
            break;
        }
        case Token::TOK_SHR:
        {
            std::cout << "    %t" << tmp_id << " = ashr i64 %t" << left->id << ", %t" << right->id << "\n";
            node->id = tmp_id++;
            break;
        }
        case Token::TOK_EQ:
        {
            std::cout << "    %t" << tmp_id << " = icmp eq i64 %t" << left->id << ", %t" << right->id << "\n";
            node->id = tmp_id++;
            break;
        }
        case Token::TOK_NEQ:
        {
            std::cout << "    %t" << tmp_id << " = icmp ne i64 %t" << left->id << ", %t" << right->id << "\n";
            node->id = tmp_id++;
            break;
        }
        case Token::TOK_LT:
        {
            std::cout << "    %t" << tmp_id << " = icmp slt i64 %t" << left->id << ", %t" << right->id << "\n";
            node->id = tmp_id++;
            break;
        }
        case Token::TOK_LTE:
        {
            std::cout << "    %t" << tmp_id << " = icmp sle i64 %t" << left->id << ", %t" << right->id << "\n";
            node->id = tmp_id++;
            break;
        }
        case Token::TOK_GT:
        {
            std::cout << "    %t" << tmp_id << " = icmp sgt i64 %t" << left->id << ", %t" << right->id << "\n";
            node->id = tmp_id++;
            break;
        }
        case Token::TOK_GTE:
        {
            std::cout << "    %t" << tmp_id << " = icmp sge i64 %t" << left->id << ", %t" << right->id << "\n";
            node->id = tmp_id++;
            break;
        }
        case Token::TOK_AND:
        {
            std::cout << "    %t" << tmp_id << " = and i1 %t" << left->id << ", %t" << right->id << "\n";
            node->id = tmp_id++;
            break;
        }
        case Token::TOK_OR:
        {
            std::cout << "    %t" << tmp_id << " = or i1 %t" << left->id << ", %t" << right->id << "\n";
            node->id = tmp_id++;
            break;
        }
        default:
        {
            return;
        }
    }
}

static void compile_op(Node *node)
{
    switch (node->children.size())
    {
        case 1:
        {
            compile_un_op(node);
            break;
        }
        case 2:
        {
            compile_bin_op(node);
            break;
        }
        default:
        {
            break;
        }
    }
}

static void compile_fun_call(Node *node)
{
    /* TODO: find function declaration node that matches function call type scheme */
    Node *fn_info;

    for (auto func : functions[node->token.data])
    {
        if (node->type_scheme == func->type_scheme)
        {
            fn_info = func;
            break;
        }
    }

    Node *in = fn_info->children.front();
    Node *out = fn_info->children[1];

    for (auto child : node->children)
    {
        compile_expr(child);
    }

    /* TODO: get_type needs to be modified for functions that output more than 1 value */
    std::cout << "    call " << get_type(out->children.front()) << " @";
    std::cout << node->token.data;

    /* support function overloading by creating unique identifiers based on param types */
    for (auto child : in->children)
    {
        std::cout << "_" << child->token.data;
    }

    std::cout << "(";

    for (size_t i = 0; i < in->children.size(); i++)
    {
        std::cout << get_type(in->children[i]) << " ";
        std::cout << "%t" << node->children[i]->id << " ";
    }

    std::cout << ")\n";
}

static void compile_expr(Node *node)
{
    switch (node->type)
    {
        case Node::NODE_LIT:
        {
            compile_literal(node);
            break;
        }
        case Node::NODE_OP:
        {
            compile_op(node);
            break;
        }
        case Node::NODE_FUN_CALL:
        {
            compile_fun_call(node);
            break;
        }
        default:
        {
            break;
        }
    }
}

static void compile_if(Node *node)
{
    switch (node->children.size())
    {
        /* only body, no condition (else clause) */
        case 1:
        {
            Node *body = node->children.front();
            for (auto child : body->children)
            {
                compile_statement(child);
            }
            break;
        }
        /* condition and body (if without else clause) */
        case 2:
        {
            Node *cond = node->children.front();
            Node *body = node->children.back();

            compile_expr(cond);

            uint64_t start = lab_id++;
            uint64_t end = lab_id++;

            std::cout << "    br i1 %t" << cond->id << ", label %l" << start << ", label %l" << end << "\n";
            std::cout << "l" << start << ":\n";
            for (size_t i = 0; i < body->children.size(); i++)
            {
                compile_statement(body->children[i]);
            }
            std::cout << "    br label %l" << end << "\n";
            std::cout << "l" << end << ":\n";
            break;
        }
        /* condition and body (if with else clause) */
        case 3:
        {
            Node *cond = node->children.front();
            Node *body = node->children[1];
            Node *els = node->children.back();

            compile_expr(cond);

            uint64_t start = lab_id++;
            uint64_t mid = lab_id++;
            uint64_t end = lab_id++;

            std::cout << "    br i1 %t" << cond->id << ", label %l" << start << ", label %l" << mid << "\n";
            std::cout << "l" << start << ":\n";
            for (size_t i = 0; i < body->children.size(); i++)
            {
                compile_statement(body->children[i]);
            }
            std::cout << "    br label %l" << end << "\n";
            std::cout << "l" << mid << ":\n";

            compile_if(els);

            std::cout << "    br label %l" << end << "\n";
            std::cout << "l" << end << ":\n";
            break;
        }
    }
}

static void compile_statement(Node *node)
{
    switch (node->type)
    {
        case Node::NODE_LOOP:
        {
            /* TODO: compile loop */
            break;
        }
        case Node::NODE_IF:
        {
            compile_if(node);
            break;
        }
        default:
        {
            compile_expr(node);
            break;
        }
    }
}

static void compile_program(Node *node)
{
    std::cout << "target triple = \"x86_64-pc-linux-gnu\"\n\n";

    std::cout << "declare void @print_bool(i1)\n";
    std::cout << "declare void @println_bool(i1)\n";
    std::cout << "declare void @print_num(i64)\n";
    std::cout << "declare void @println_num(i64)\n\n";

    std::cout << "define i32 @main(i32, i8**) {\n";
    std::cout << "entry:\n";

    for (auto child : node->children)
    {
        switch (child->type)
        {
            case Node::NODE_FUN_DEC:
            {
                /* TODO: compile function */
                break;
            }
            default:
            {
                compile_statement(child);
                break;
            }
        }
    }

    std::cout << "    ret i32 0\n";
    std::cout << "}\n\n";
}

void compile(Node *node)
{
    compile_program(node);
}

#include "compiler.hpp"

typedef std::unordered_map<std::string, uint64_t> CompScope;
std::stack<CompScope> comp_scopes;
std::unordered_set<uint64_t> func_param_ids;

/* map tmp_ids of arrays to tmp_id of size */
std::unordered_map<uint64_t, uint64_t> array_data;

uint64_t tmp_id = 0;
uint64_t lab_id = 0;

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
            switch (node->type_scheme.type)
            {
                case TypeScheme::ALPHA:
                {
                    uint32_t alpha = node->type_scheme.alpha;
                    alpha &= ~(0b1111 << 28);
                    // if (alpha >> 28)
                    // {
                    //     alpha &= ~(0b1111 << 28);
                    //     if (alpha == type_names["num"])
                    //     {
                    //         return "i64, i64 %t" + std::to_string(array_data[node->id]) + ", align 16";
                    //     }
                    //     if (alpha == type_names["bool"])
                    //     {
                    //         return "i1, i64 %t" + std::to_string(array_data[node->id]) + ", align 16";
                    //     }
                    // }
                    if (alpha == type_names["num"])
                    {
                        return "i64";
                    }
                    if (alpha == type_names["bool"])
                    {
                        return "i1";
                    }
                }
                case TypeScheme::FUN_TYPE:
                {
                    uint32_t alpha = node->type_scheme.fun_type.second.front();
                    if (alpha >> 28)
                    {
                        return "%__array";
                    }
                    if (alpha == type_names["num"])
                    {
                        return "i64";
                    }
                    if (alpha == type_names["bool"])
                    {
                        return "i1";
                    }
                }
                default:
                {
                    break;
                }
            }
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
            break;
        }
    }
}

static void compile_key(Node *node)
{
    (void)node;
    // switch (node->token.type)
    // {
    //     case Token::KEY_NUM:
    //     {
    //         std::cout << "    %t" << tmp_id << " = add i64 0, 8\n";
    //         node->id = tmp_id++;
    //         break;
    //     }
    //     case Token::KEY_BOOL:
    //     {
    //         std::cout << "    %t" << tmp_id << " = add i64 0, 8\n";
    //         node->id = tmp_id++;
    //         break;
    //     }
    //     default:
    //     {
    //         break;
    //     }
    // }
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
            if (right->type == Node::NODE_KEY || (right->type_scheme.type == TypeScheme::ALPHA && right->type_scheme.alpha >> 28))
            {
                array_data[tmp_id] = left->id;
                // std::cout << "    %t" << tmp_id << " = alloca i64, i64 %t" << left->id << ", align 16\n";
                // node->id = tmp_id++;
    
                // uint32_t real_size_tmp = tmp_id;
                // std::cout << "    %t" << tmp_id++ << " = mul i64 %t" << left->id << ", %t" << right->id << "\n";

                // uint32_t zero_tmp = tmp_id;
                // std::cout << "    %t" << tmp_id++ << " = alloca i8, i64 %t" << real_size_tmp << "\n";
                // std::cout << "    call void @llvm.memset.p0i8.i64(i8* %t" << zero_tmp << ", i8 0, i64 %t" << real_size_tmp << ", i32 0, i1 false)\n";
                // 
                // std::cout << "    %t" << tmp_id++ << " = insertvalue %__array undef, i8* %t" << zero_tmp << ", 0\n";
                // std::cout << "    %t" << tmp_id++ << " = insertvalue %__array %t" << tmp_id - 2 << ", i64 %t" << left->id << ", 1\n";

                // node->id = tmp_id;
                // std::cout << "    %t" << tmp_id++ << " = insertvalue %__array %t" << tmp_id - 2 << ", i64 %t" << right->id << ", 2\n";
            }
            else
            {
                node->id = tmp_id;
                std::cout << "    %t" << tmp_id++ << " = mul i64 %t" << left->id << ", %t" << right->id << "\n";
            }
            break;
        }
        case Token::TOK_DIV:
        {
            std::cout << "    %t" << tmp_id << " = sdiv i64 %t" << left->id << ", %t" << right->id << "\n";
            node->id = tmp_id++;
            break;
        }
        case Token::TOK_MOD:
        {
            std::cout << "    %t" << tmp_id << " = srem i64 %t" << left->id << ", %t" << right->id << "\n";
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
    Node *fn_info = nullptr;

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
    if (out->children.front()->token.type != Token::KEY_NIL)
    {
        std::cout << "    %t" << tmp_id << " = call ";
        node->id = tmp_id++;
    }
    else
    {
        std::cout << "    call ";
    }
    std::cout << get_type(out->children.front()) << " @" << node->token.data;

    /* support function overloading by creating unique identifiers based on param types */
    for (auto child : in->children)
    {
        std::cout << "_" << child->token.data;
    }

    std::cout << "(";

    if (in->children.size() && in->children.front()->token.type != Token::KEY_NIL)
    {
        for (size_t i = 0; i < in->children.size() - 1; i++)
        {
            std::cout << get_type(in->children[i]) << " ";
            std::cout << "%t" << node->children[i]->id << ", ";
        }
        std::cout << get_type(in->children.back()) << " ";
        std::cout << "%t" << node->children.back()->id;
    }
    std::cout << ")\n";

    if (node->type_scheme.fun_type.second.size() == 1)
    {
        uint32_t alpha = node->type_scheme.fun_type.second.front();
        node->type_scheme = TypeScheme(TypeScheme::ALPHA);
        node->type_scheme.alpha = alpha;
    }
}

static void compile_var(Node *node)
{
    bool fun_arg = func_param_ids.count(comp_scopes.top()[node->token.data]) != 0;
    if (!fun_arg)
    {
        std::string type = get_type(node);
        uint32_t source = comp_scopes.top()[node->token.data];

        if (node->children.size())
        {
            compile_statement(node->children.front());
            std::cout << "    %t" << tmp_id++ << " = sub i64 %t" << tmp_id - 2 << ", 1\n";
            std::cout << "    %t" << tmp_id << " = getelementptr " << type << ", " << type << "* %t" << source << ", i64 %t" << tmp_id - 1 << "\n";

            // std::cout << "    %t" << tmp_id++ << " = call i8* @__array_at(%__array* %t" << source << ", i64 %t" << tmp_id - 2 << ")\n";
            // std::cout << "    %t" << tmp_id << " = bitcast i8* %t" << tmp_id - 1 << " to " << type << "*\n";
            source = tmp_id++;
        }

        std::cout << "    %t" << tmp_id << " = load " << type << ", " << type << "* %t" << source << "\n";
        node->id = tmp_id++;
    }
    else
    {
        node->id = comp_scopes.top()[node->token.data];
    }
}

static void compile_var_asn(Node *node)
{
    Node *left = node->children.front();
    Node *right = node->children.back();

    compile_statement(right);
    std::string type = get_type(right);

    TypeScheme ts = right->type_scheme;
    switch (ts.type)
    {
        case TypeScheme::ALPHA:
        {
            uint32_t alpha = ts.alpha;
            uint32_t arr_size = alpha >> 28;
            
            if (arr_size)
            {
                alpha &= ~(0b1111 << 28);
            }
            /* TODO: handle multi-dimensional arrays */
            /* TODO: handle non-zero initialization */

            if (!comp_scopes.top().count(left->token.data))
            {
                if (arr_size)
                {
                    std::cout << "    %t" << tmp_id << " = alloca " << type << ", i64 %t" << array_data[tmp_id] << "\n";
                    comp_scopes.top()[left->token.data] = tmp_id;
                    node->id = tmp_id++;

                    /* TODO: change for bools */
                    std::cout << "    %t" << tmp_id++ << " = mul i64 %t" << array_data[node->id] << ", 8\n";
                    std::cout << "    %t" << tmp_id << " = bitcast " << type << "* %t" << node->id << " to i8*\n";

                    std::cout << "    call void @llvm.memset.p0i8.i64(i8* %t" << tmp_id++ << ", i8 0, i64 %t" << tmp_id - 2 << ", i32 0, i1 false)\n";

                    return;
                }
                else
                {
                    std::cout << "    %t" << tmp_id << " = alloca " << type << "\n";
                }
                comp_scopes.top()[left->token.data] = tmp_id;
                node->id = tmp_id++;
            }

            uint32_t source = comp_scopes.top()[left->token.data];
            if (left->children.size())
            {
                compile_statement(left->children.front());
                std::cout << "    %t" << tmp_id++ << " = sub i64 %t" << tmp_id - 2 << ", 1\n";
                std::cout << "    %t" << tmp_id << " = getelementptr " << type << ", " << type << "* %t" << source << ", i64 %t" << tmp_id - 1 << "\n";

                // std::cout << "    %t" << tmp_id++ << " = call i8* @__array_at(%__array* %t" << source << ", i64 %t" << tmp_id - 2 << ")\n";
                // std::cout << "    %t" << tmp_id << " = bitcast i8* %t" << tmp_id - 1 << " to " << type << "*\n";
                source = tmp_id++;
            }

            std::cout << "    store " << type << " %t" << right->id << ", " << type << "* %t" << source << "\n";
        }
        case TypeScheme::FUN_TYPE:
        {
            break;
        }
        default:
        {
            break;
        }
    }
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
        case Node::NODE_KEY:
        {
            compile_key(node);
            break;
        }
        case Node::NODE_FUN_CALL:
        {
            compile_fun_call(node);
            break;
        }
        case Node::NODE_VAR:
        {
            compile_var(node);
            break;
        }
        case Node::NODE_VAR_ASN:
        {
            compile_var_asn(node);
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

            for (auto child : body->children)
            {
                compile_statement(child);
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

            for (auto child : body->children)
            {
                compile_statement(child);
            }

            std::cout << "    br label %l" << end << "\n";
            std::cout << "l" << mid << ":\n";

            compile_if(els);

            std::cout << "    br label %l" << end << "\n";
            std::cout << "l" << end << ":\n";

            if (node->type_scheme.alpha != type_names["nil"])
            {
                node->id = tmp_id;
                std::cout << "    %t" << tmp_id++ << " = select i1 %t" << cond->id;
                std::cout << ", i64 %t" << body->children.back()->id;
                std::cout << ", i64 %t" << els->children.back()->children.back()->id << "\n";
                /* %2 = select i1 %1, [4 x i8]* @.fls, [4 x i8]* @.tru\n"; */
            }

            break;
        }
        default:
        {
            break;
        }
    }
}

static void compile_loop(Node *node)
{
    uint64_t start = lab_id++;
    uint64_t mid = lab_id++;
    uint64_t end = lab_id++;
    
    std::cout << "    br label %l" << start << "\n";
    std::cout << "l" << start << ":\n";
    
    compile_expr(node->children.front());
    std::cout << "    br i1 %t" << node->children.front()->id << ", label %l" << mid << ", label %l" << end << "\n";
    
    std::cout << "l" << mid << ":\n";
    
    for (auto child : node->children.back()->children)
    {
        compile_statement(child);
    }
    std::cout << "    br label %l" << start << "\n";
    
    std::cout << "l" << end << ":\n";
}

static void compile_statement(Node *node)
{
    switch (node->type)
    {
        case Node::NODE_LOOP:
        {
            compile_loop(node);
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

static void compile_fun_dec(Node *node)
{
    comp_scopes.push(CompScope());

    Node *in = node->children.front();
    Node *out = node->children[1];
    Node *body = node->children.back();

    /* TODO: multiple return types */
    std::cout << "define " << get_type(out->children.front()) << " @" << node->token.data;

    for (auto child : in->children)
    {
        std::cout << "_" << child->token.data;
    }

    std::cout << "(";

    if (in->children.size() && in->children.front()->token.type != Token::KEY_NIL)
    {
        for (size_t i = 0; i < in->children.size() - 1; i++)
        {
            auto child = in->children[i];
            std::cout << get_type(child) << " %t" << tmp_id << ", ";
            comp_scopes.top()[body->children[i]->token.data] = tmp_id;

            func_param_ids.insert(tmp_id);
            body->children[i]->id = tmp_id++;
        }
        std::cout << get_type(in->children.back()) << " %t" << tmp_id;
        comp_scopes.top()[body->children[in->children.size() - 1]->token.data] = tmp_id;

        func_param_ids.insert(tmp_id);
        body->children[in->children.size() - 1]->id = tmp_id++;
    }
    std::cout << ") {\n";

    size_t i = in->children.front()->token.type != Token::KEY_NIL ? in->children.size() : 0;
    for (; i < body->children.size(); i++)
    {
        auto child = body->children[i];
        compile_statement(child);
    }

    if (node->type_scheme.fun_type.second.front() != type_names["nil"])
    {
        std::cout << "    ret " << get_type(out->children.front()) << " %t" << body->children.back()->id;
    }
    else
    {
        std::cout << "    ret void";
    }
    std::cout << "\n}\n\n";
    comp_scopes.pop();
    func_param_ids.clear();
}

static void compile_program(Node *node)
{
    (void)compile_fun_dec;

    std::cout << "target triple = \"x86_64-pc-linux-gnu\"\n";

    std::cout << "@.tru = private unnamed_addr constant [4 x i8] c\"tru\\00\", align 1\n";
    std::cout << "@.fls = private unnamed_addr constant [4 x i8] c\"fls\\00\", align 1\n";
    std::cout << "@.truln = private unnamed_addr constant [5 x i8] c\"tru\\0A\\00\", align 1\n";
    std::cout << "@.flsln = private unnamed_addr constant [5 x i8] c\"fls\\0A\\00\", align 1\n";

    std::cout << "@.lld = private unnamed_addr constant [5 x i8] c\"%lld\\00\", align 1\n";
    std::cout << "@.lldln = private unnamed_addr constant [6 x i8] c\"%lld\\0A\\00\", align 1\n";

    std::cout << "declare i32 @printf(i8*, ...)\n";

    std::cout << "define void @print_bool(i1 %val) {\n";
    std::cout << "    %1 = icmp eq i1 %val, 0 \n";
    std::cout << "    %2 = select i1 %1, [4 x i8]* @.fls, [4 x i8]* @.tru\n";
    std::cout << "    %3 = bitcast [4 x i8]* %2 to i8*\n";
    std::cout << "    %4 = call i32 (i8*, ...) @printf(i8* %3)\n";
    std::cout << "    ret void\n";
    std::cout << "}\n";

    std::cout << "define void @println_bool(i1 %val) {\n";
    std::cout << "    %1 = icmp eq i1 %val, 0\n";
    std::cout << "    %2 = select i1 %1, [5 x i8]* @.flsln, [5 x i8]* @.truln\n";
    std::cout << "    %3 = bitcast [5 x i8]* %2 to i8*\n";
    std::cout << "    %4 = call i32 (i8*, ...) @printf(i8* %3)\n";
    std::cout << "    ret void\n";
    std::cout << "}\n";

    std::cout << "define void @print_num(i64 %val) {\n";
    std::cout << "    %1 = bitcast [5 x i8]* @.lld to i8*\n";
    std::cout << "    %2 = call i32 (i8*, ...) @printf(i8* %1, i64 %val)\n";
    std::cout << "    ret void\n";
    std::cout << "}\n";

    std::cout << "define void @println_num(i64 %val) {\n";
    std::cout << "    %1 = bitcast [6 x i8]* @.lldln to i8*\n";
    std::cout << "    %2 = call i32 (i8*, ...) @printf(i8* %1, i64 %val)\n";
    std::cout << "    ret void\n";
    std::cout << "}\n\n";

    std::cout << "declare void @llvm.memset.p0i8.i64(i8* nocapture, i8, i64, i32, i1)\n\n";

    // std::cout << "%__array = type { i8*, i64, i64 }\n";
    // std::cout << "define i8* @__array_at(%__array* %0, i64 %1) {\n";
    // std::cout << "    %3 = getelementptr %__array, %__array* %0, i32 0, i32 2\n";
    // std::cout << "    %4 = load i64, i64* %3\n";
    // std::cout << "    %5 = sub i64 %1, 1\n";
    // std::cout << "    %6 = mul i64 %5, %4\n";
    // std::cout << "    %7 = getelementptr %__array, %__array* %0, i32 0, i32 0\n";
    // std::cout << "    %8 = load i8*, i8** %7\n";
    // std::cout << "    %9 = getelementptr i8, i8* %8, i64 %6\n";
    // std::cout << "    ret i8* %9\n";
    // std::cout << "}\n\n";

    /* compile all functions first */
    for (auto child : node->children)
    {
        if (child->type == Node::NODE_FUN_DEC)
        {
            compile_fun_dec(child);
        }
    }

    std::cout << "define i32 @main(i32, i8**) {\n";
    std::cout << "entry:\n";
    
    comp_scopes.push(CompScope());
    for (auto child : node->children)
    {
        if (child->type != Node::NODE_FUN_DEC)
        {
            compile_statement(child);
        }
    }

    std::cout << "    ret i32 0\n";
    std::cout << "}\n\n";
    comp_scopes.pop();
}

void compile(Node *node)
{
    compile_program(node);
}

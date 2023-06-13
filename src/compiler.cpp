#include "compiler.hpp"

static std::unique_ptr<llvm::LLVMContext> context;
static std::unique_ptr<llvm::Module> module;
static std::unique_ptr<llvm::IRBuilder<>> builder;

static std::unordered_map<std::string, llvm::AllocaInst *> values;
static llvm::Constant *compile_literal(std::unique_ptr<Node> &root);
static llvm::Value *compile_expr(std::unique_ptr<Node> &root);

static llvm::Type *get_llvm_type(TypeScheme ts)
{
    if (ts == construct_type(type_names["num"]))
    {
        return llvm::Type::getInt64Ty(*context);
    }
    if (ts == construct_type(type_names["bool"]))
    {
        return llvm::Type::getInt1Ty(*context);
    }
    if (ts == construct_type(type_names["str"]))
    {
        return llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(*context));
    }
    return llvm::Type::getVoidTy(*context);
}

static void get_arr_sizes(std::unique_ptr<Node> &arr_sizes, std::list<uint64_t> &sizes)
{
    if (arr_sizes == nullptr) return;
    if (!arr_sizes->children.size())
    {
        sizes.push_back(std::stoi(arr_sizes->token.data));
        return;
    }
    get_arr_sizes(arr_sizes->children.front(), sizes);
    get_arr_sizes(arr_sizes->children.back(), sizes);
}

static llvm::Type *get_llvm_type(TypeScheme ts, std::list<uint64_t> &arr_sizes)
{
    if (ts.arr_dim == 0)
    {
        return get_llvm_type(ts);
    }

    ts.arr_dim--;
    uint64_t size = arr_sizes.front();
    arr_sizes.pop_front();
    return llvm::ArrayType::get(get_llvm_type(ts, arr_sizes), size);
}

static llvm::AllocaInst *allocate_var(llvm::Function *fun, const std::string &name, llvm::Type *ty)
{
    llvm::IRBuilder<> tmp_builder(&fun->getEntryBlock(), fun->getEntryBlock().begin());
    return tmp_builder.CreateAlloca(ty, nullptr, name);
}

static void initialize_built_ins()
{
    llvm::Function::Create(
        llvm::FunctionType::get(llvm::Type::getInt32Ty(*context), { llvm::Type::getInt8PtrTy(*context) }, true),
        llvm::Function::ExternalLinkage, "printf", module.get()
    );

    llvm::Type *num_input = get_llvm_type(construct_type(type_names["num"]));
    llvm::Type *bool_input = get_llvm_type(construct_type(type_names["bool"]));
    llvm::Type *str_input = get_llvm_type(construct_type(type_names["str"]));

    llvm::Function *print_num =
        llvm::Function::Create(
            llvm::FunctionType::get(llvm::Type::getVoidTy(*context), { num_input }, false),
            llvm::Function::ExternalLinkage, "print_num", module.get()
        );

    llvm::Function *print_bool =
        llvm::Function::Create(
            llvm::FunctionType::get(llvm::Type::getVoidTy(*context), { bool_input }, false),
            llvm::Function::ExternalLinkage, "print_bool", module.get()
        );

    llvm::Function *print_str =
        llvm::Function::Create(
            llvm::FunctionType::get(llvm::Type::getVoidTy(*context), { str_input }, false),
            llvm::Function::ExternalLinkage, "print_str", module.get()
        );

    llvm::BasicBlock *bb;

    bb = llvm::BasicBlock::Create(*context, "entry", print_num);
    builder->SetInsertPoint(bb);
    llvm::Value *num_format = builder->CreateGlobalString("%lld\n");
    builder->CreateCall(module->getFunction("printf"), { num_format, print_num->arg_begin() });
    builder->CreateRetVoid();

    bb = llvm::BasicBlock::Create(*context, "entry", print_bool);
    builder->SetInsertPoint(bb);
    llvm::Value *bool_format = builder->CreateGlobalString("%lld\n");
    builder->CreateCall(module->getFunction("printf"), { bool_format, print_bool->arg_begin() });
    builder->CreateRetVoid();

    bb = llvm::BasicBlock::Create(*context, "entry", print_str);
    builder->SetInsertPoint(bb);
    llvm::Value *str_format = builder->CreateGlobalString("%s\n");
    builder->CreateCall(module->getFunction("printf"), { str_format, print_str->arg_begin() });
    builder->CreateRetVoid();
}

static llvm::ArrayType *get_llvm_type_array_literal(std::unique_ptr<Node> &root)
{
    if (root->type_scheme.arr_dim == 1)
    {
        return llvm::ArrayType::get(get_llvm_type(root->children.front()->type_scheme), root->children.size());
    }

    return llvm::ArrayType::get(get_llvm_type_array_literal(root->children.front()), root->children.size());
}

static llvm::Constant *compile_literal(std::unique_ptr<Node> &root)
{
    if (root->type_scheme.arr_dim)
    {
        std::vector<llvm::Constant *> vals;
        for (size_t i = 0; i < root->children.size(); i++)
        {
            vals.push_back(compile_literal(root->children[i]));
        }

        llvm::ArrayType *arr_type = get_llvm_type_array_literal(root);
        llvm::GlobalVariable *array = new llvm::GlobalVariable(
            *module.get(), arr_type, true, llvm::GlobalVariable::PrivateLinkage,
            llvm::ConstantArray::get(arr_type, vals)
        );
        return array;
    }

    if (root->type_scheme == construct_type(type_names["num"]))
    {
        return llvm::ConstantInt::get(*context, llvm::APInt(64, llvm::StringRef(root->token.data), 10));
    }
    if (root->type_scheme == construct_type(type_names["bool"]))
    {
        return llvm::ConstantInt::get(*context, llvm::APInt(1, root->token.type == Token::TOK_TRU));
    }
    if (root->type_scheme == construct_type(type_names["str"]))
    {
        return builder->CreateGlobalString(root->token.data);
    }
    return nullptr;
}

static llvm::Value *compile_un_op(std::unique_ptr<Node> &root)
{
    llvm::Value *operand = compile_expr(root->children.front());
    if (operand)
    {
        switch (root->token.type)
        {
            case Token::TOK_NOT:
            {
                return builder->CreateNot(operand);
            }
            case Token::TOK_SUB:
            {
                return builder->CreateNeg(operand);
            }
            default:
            {
                return nullptr;
            }
        }
    }

    return nullptr;
}

static llvm::Value *compile_bin_op(std::unique_ptr<Node> &root)
{
    llvm::Value *left = compile_expr(root->children.front());
    llvm::Value *right = compile_expr(root->children.back());

    if (!left || !right)
    {
        return nullptr;
    }

    switch (root->token.type)
    {
        case Token::TOK_ADD:
        {
            return builder->CreateAdd(left, right);
        }
        case Token::TOK_SUB:
        {
            return builder->CreateSub(left, right);
        }
        case Token::TOK_MUL:
        {
            return builder->CreateMul(left, right);
        }
        case Token::TOK_DIV:
        {
            return builder->CreateSDiv(left, right);
        }
        case Token::TOK_MOD:
        {
            return builder->CreateSRem(left, right);
        }
        case Token::TOK_EQ:
        {
            return builder->CreateICmpEQ(left, right);
        }
        case Token::TOK_NEQ:
        {
            return builder->CreateICmpNE(left, right);
        }
        case Token::TOK_LT:
        {
            return builder->CreateICmpSLT(left, right);
        }
        case Token::TOK_GT:
        {
            return builder->CreateICmpSGT(left, right);
        }
        case Token::TOK_LTE:
        {
            return builder->CreateICmpSLE(left, right);
        }
        case Token::TOK_GTE:
        {
            return builder->CreateICmpSGE(left, right);
        }
        case Token::TOK_AND:
        {
            return builder->CreateAnd(left, right);
        }
        case Token::TOK_OR:
        {
            return builder->CreateOr(left, right);
        }
        case Token::TOK_SHL:
        {
            return builder->CreateShl(left, right);
        }
        case Token::TOK_SHR:
        {
            return builder->CreateAShr(left, right);
        }
        default:
        {
            return nullptr;
        }
    }
}

static llvm::Value *compile_op(std::unique_ptr<Node> &root)
{
    switch (root->children.size())
    {
        case 1:
        {
            return compile_un_op(root);
        }
        case 2:
        {
            return compile_bin_op(root);
        }
        default:
        {
            return nullptr;
        }
    }
}

static llvm::Value *compile_fun_call(std::unique_ptr<Node> &root)
{
    llvm::Function *fun = module->getFunction(root->token.data);

    if (root->children.size())
    {
        std::vector<llvm::Value *> args;
        for (size_t i = 0; i < root->children.size(); i++)
        {
            args.push_back(compile_expr(root->children[i]));
        }

        return builder->CreateCall(fun, args);
    }

    return builder->CreateCall(fun);
}

static llvm::Value *compile_built_in(std::unique_ptr<Node> &root)
{
    switch (root->token.type)
    {
        case Token::KEY_PRINT:
        {
            if (root->children.front()->type_scheme == construct_type(type_names["num"]))
            {
                root->token.data = "print_num";
                return compile_fun_call(root);
            }
            else if (root->children.front()->type_scheme == construct_type(type_names["bool"]))
            {
                root->token.data = "print_bool";
                return compile_fun_call(root);
            }
            else if (root->children.front()->type_scheme == construct_type(type_names["str"]))
            {
                root->token.data = "print_str";
                return compile_fun_call(root);
            }
            return nullptr;
        }
        default:
        {
            return nullptr;
        }
    }
}

static llvm::Value *compile_var(std::unique_ptr<Node> &root)
{
    llvm::AllocaInst *alloc = values[root->token.data];
    return builder->CreateLoad(alloc->getAllocatedType(), alloc, root->token.data.c_str());
}

static llvm::Value *compile_var_asn(std::unique_ptr<Node> &root)
{
    if (root->children.back()->token.type == Token::TOK_MUL && root->children.back()->type_scheme.arr_dim)
    {
        if (!values[root->children.front()->token.data])
        {
            llvm::Function *f = builder->GetInsertBlock()->getParent();
            std::list<uint64_t> sizes;
            get_arr_sizes(root->children.back()->children.front(), sizes);

            llvm::AllocaInst *alloc = allocate_var(f, root->children.front()->token.data,
                                                   get_llvm_type(root->children.front()->type_scheme, sizes));
            values[root->children.front()->token.data] = alloc;
        }
    }
    else
    {
        llvm::Value *rhs = compile_expr(root->children.back());

        if (!values[root->children.front()->token.data])
        {
            llvm::Function *f = builder->GetInsertBlock()->getParent();
            llvm::AllocaInst *alloc;

            if (root->children.back()->type_scheme.arr_dim && root->children.back()->type == Node::NODE_LIT)
            {
                alloc = allocate_var(f, root->children.front()->token.data, get_llvm_type_array_literal(root->children.back()));
            }
            else if (root->children.back()->type_scheme.arr_dim)
            {
                alloc = allocate_var(f, root->children.front()->token.data, values[root->children.back()->token.data]->getType());
            }
            else
            {
                alloc = allocate_var(f, root->children.front()->token.data, get_llvm_type(root->children.back()->type_scheme));
            }

            values[root->children.front()->token.data] = alloc;
        }

        builder->CreateStore(rhs, values[root->children.front()->token.data]);
    }

    return nullptr;
}

static llvm::Value *compile_lone_if(std::unique_ptr<Node> &root)
{
    std::unique_ptr<Node> &cond = root->children.front();
    std::unique_ptr<Node> &body = root->children.back();

    llvm::Function *fun = builder->GetInsertBlock()->getParent();
    llvm::Value *cond_val = compile_expr(cond);

    llvm::BasicBlock *then_blk = llvm::BasicBlock::Create(*context, "", fun);
    llvm::BasicBlock *end_blk = llvm::BasicBlock::Create(*context, "", fun);

    builder->CreateCondBr(cond_val, then_blk, end_blk);
    builder->SetInsertPoint(then_blk);

    for (size_t i = 0; i < body->children.size(); i++)
    {
        compile_expr(body->children[i]);
    }
    builder->CreateBr(end_blk);
    builder->SetInsertPoint(end_blk);

    return llvm::Constant::getNullValue(llvm::Type::getVoidTy(*context));
}

static void compile_if_else(std::unique_ptr<Node> &root,
                            llvm::Function *fun,
                            llvm::BasicBlock *merge_blk,
                            std::vector<llvm::Value *> &phis,
                            std::vector<llvm::BasicBlock *> &blks)
{
    switch (root->children.size())
    {
        case 1:
        {
            std::unique_ptr<Node> &body = root->children.front();
            for (size_t i = 0; i < body->children.size() - 1; i++)
            {
                compile_expr(body->children[i]);
            }
            phis.push_back(compile_expr(body->children.back()));
            break;
        }
        case 3:
        {
            std::unique_ptr<Node> &cond = root->children.front();
            std::unique_ptr<Node> &then = root->children[1];
            std::unique_ptr<Node> &els = root->children.back();

            llvm::Value *cond_val = compile_expr(cond);

            llvm::BasicBlock *then_blk = llvm::BasicBlock::Create(*context, "", fun);
            llvm::BasicBlock *else_blk = llvm::BasicBlock::Create(*context, "", fun);

            builder->CreateCondBr(cond_val, then_blk, else_blk);
            builder->SetInsertPoint(then_blk);

            for (size_t i = 0; i < then->children.size() - 1; i++)
            {
                compile_expr(then->children[i]);
            }
            phis.push_back(compile_expr(then->children.back()));
            blks.push_back(then_blk);

            builder->CreateBr(merge_blk);
            builder->SetInsertPoint(else_blk);
            compile_if_else(els, fun, merge_blk, phis, blks);
            blks.push_back(else_blk);

            break;
        }
        default:
        {
            break;
        }
    }

}

static llvm::Value *compile_if(std::unique_ptr<Node> &root)
{
    switch (root->children.size())
    {
        case 2:
        {
            return compile_lone_if(root);
        }
        case 3:
        {
            llvm::Function *fun = builder->GetInsertBlock()->getParent();
            llvm::BasicBlock *merge_blk = llvm::BasicBlock::Create(*context, "", fun);
            std::vector<llvm::Value *> phis;
            std::vector<llvm::BasicBlock *> blks;

            TypeScheme ts = root->type_scheme;
            bool unit_statement = ts == construct_type(type_names["unit"]);
            compile_if_else(root, fun, merge_blk, phis, blks);

            builder->CreateBr(merge_blk);
            builder->SetInsertPoint(merge_blk);
            if (!unit_statement)
            {
                llvm::PHINode *phi_node = builder->CreatePHI(get_llvm_type(ts), phis.size());
                for (size_t i = 0; i < phis.size(); i++)
                {
                    phi_node->addIncoming(phis[i], blks[i]);
                }

                return phi_node;
            }
            return llvm::Constant::getNullValue(llvm::Type::getVoidTy(*context));
        }
        default:
        {
            return nullptr;
        }
    }
}

static llvm::Value *compile_loop(std::unique_ptr<Node> &root)
{
    std::unique_ptr<Node> &cond = root->children.front();
    std::unique_ptr<Node> &body = root->children.back();

    llvm::Function *fun = builder->GetInsertBlock()->getParent();

    llvm::BasicBlock *body_blk = llvm::BasicBlock::Create(*context, "", fun);
    llvm::BasicBlock *cond_blk = llvm::BasicBlock::Create(*context, "", fun);
    llvm::BasicBlock *exit_blk = llvm::BasicBlock::Create(*context, "", fun);

    builder->CreateBr(cond_blk);
    builder->SetInsertPoint(cond_blk);
    llvm::Value *cond_val = compile_expr(cond);
    builder->CreateCondBr(cond_val, body_blk, exit_blk);

    builder->SetInsertPoint(body_blk);
    for (size_t i = 0; i < body->children.size(); i++)
    {
        compile_expr(body->children[i]);
    }
    builder->CreateBr(cond_blk);
    builder->SetInsertPoint(exit_blk);

    return llvm::Constant::getNullValue(llvm::Type::getVoidTy(*context));
}

static llvm::Value *compile_expr(std::unique_ptr<Node> &root)
{
    switch (root->type)
    {
        case Node::NODE_LIT:
        {
            return compile_literal(root);
        }
        case Node::NODE_OP:
        {
            return compile_op(root);
        }
        case Node::NODE_FUN_CALL:
        {
            return compile_fun_call(root);
        } 
        case Node::NODE_BUILT_IN:
        {
            return compile_built_in(root);
        }
        case Node::NODE_VAR:
        {
            return compile_var(root);
        }
        case Node::NODE_VAR_ASN:
        {
            return compile_var_asn(root);
        }
        case Node::NODE_IF:
        {
            return compile_if(root);
        }
        case Node::NODE_LOOP:
        {
            return compile_loop(root);
        }
        default:
        {
            return nullptr;
        }
    }
}

static void compile_fun_dec(std::unique_ptr<Node> &root)
{
    std::unique_ptr<Node> &in = root->children.front();
    std::unique_ptr<Node> &out = root->children[1];
    std::unique_ptr<Node> &body = root->children.back();

    bool unit_input = in->children.front()->token.type == Token::KEY_UNIT;
    bool unit_output = out->children.front()->token.type == Token::KEY_UNIT;

    llvm::FunctionType *fun_type;

    if (!unit_input)
    {
        std::vector<llvm::Type *> arg_types;
        for (size_t i = 0; i < in->children.size() && !unit_input; i++)
        {
            arg_types.push_back(get_llvm_type(body->children[i]->type_scheme));
        }

        fun_type = llvm::FunctionType::get(
            !unit_output ? get_llvm_type(body->children.back()->type_scheme) : llvm::Type::getVoidTy(*context), 
            arg_types, false
        );
    }
    else
    {
        fun_type = llvm::FunctionType::get(
            !unit_output ? get_llvm_type(body->children.back()->type_scheme) : llvm::Type::getVoidTy(*context),
            false
        );
    }

    llvm::Function *fun = llvm::Function::Create(fun_type, llvm::Function::ExternalLinkage, root->token.data, module.get());

    if (!unit_input)
    {
        size_t i = 0;
        for (auto &arg : fun->args())
        {
            arg.setName(body->children[i++]->token.data);
        }
    }

    llvm::BasicBlock *bb = llvm::BasicBlock::Create(*context, "entry", fun);
    builder->SetInsertPoint(bb);
    values.clear();

    if (!unit_input)
    {
        for (auto &arg : fun->args())
        {
            llvm::AllocaInst *alloc = allocate_var(fun, std::string(arg.getName()), arg.getType());
            builder->CreateStore(&arg, alloc);
            values[std::string(arg.getName())] = alloc;
        }
    }

    size_t i = !unit_input ? in->children.size() : 0;
    if (body->children.size())
    {
        for (; i < body->children.size() - !unit_output; i++)
        {
            compile_expr(body->children[i]);
        }

        if (!unit_output)
        {
            builder->CreateRet(compile_expr(body->children.back()));
        }
        else
        {
            builder->CreateRetVoid();
        }
    }
    else
    {
        builder->CreateRetVoid();
    }

    llvm::verifyFunction(*fun);
}

static void compile_program(std::unique_ptr<Node> &root)
{
    for (size_t i = 0; i < root->children.size(); i++)
    {
        if (root->children[i]->type == Node::NODE_FUN_DEC)
        {
            compile_fun_dec(root->children[i]);
        }
    }

    llvm::FunctionType *ft = llvm::FunctionType::get(llvm::Type::getInt32Ty(*context), false);
    llvm::Function *f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "main", module.get());
    llvm::BasicBlock *bb = llvm::BasicBlock::Create(*context, "entry", f);
    builder->SetInsertPoint(bb);

    for (size_t i = 0; i < root->children.size(); i++)
    {
        if (root->children[i]->type != Node::NODE_FUN_DEC)
        {
            compile_expr(root->children[i]);
        }
    }

    llvm::Value *ret_zero = llvm::ConstantInt::get(*context, llvm::APInt(32, 0));
    builder->CreateRet(ret_zero);

    llvm::verifyFunction(*f);
}

void compile(std::unique_ptr<Node> &root)
{
    context = std::make_unique<llvm::LLVMContext>();
    module = std::make_unique<llvm::Module>("", *context);
    builder = std::make_unique<llvm::IRBuilder<>>(*context);

    initialize_built_ins();
    compile_program(root);

    /* executable generation */
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    auto target_triple = llvm::sys::getDefaultTargetTriple();
    llvm::outs() << "target triple = \"" << target_triple << "\"\n";

    std::string error;
    auto target = llvm::TargetRegistry::lookupTarget(target_triple, error);

    auto cpu = "generic";
    auto features = "";
    llvm::TargetOptions opt;
    auto reloc_model = llvm::Optional<llvm::Reloc::Model>();
    auto machine = target->createTargetMachine(target_triple, cpu, features, opt, reloc_model);

    module->setDataLayout(machine->createDataLayout());
    module->print(llvm::outs(), nullptr);
}

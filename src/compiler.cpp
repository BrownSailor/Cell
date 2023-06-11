#include "compiler.hpp"

static std::unique_ptr<llvm::LLVMContext> context;
static std::unique_ptr<llvm::Module> module;
static std::unique_ptr<llvm::IRBuilder<>> builder;

static std::unordered_map<std::string, llvm::AllocaInst *> values;

llvm::Type *get_llvm_type(TypeScheme ts)
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

static llvm::Value *compile_expr(std::unique_ptr<Node> root);

static llvm::Value *compile_literal(std::unique_ptr<Node> root)
{
    switch (root->token.type)
    {
        case Token::TOK_NUM:
        {
            return llvm::ConstantInt::get(*context, llvm::APInt(64, llvm::StringRef(root->token.data), 10));
        }
        case Token::TOK_TRU:
        case Token::TOK_FLS:
        {
            return llvm::ConstantInt::get(*context, llvm::APInt(1, root->token.type == Token::TOK_TRU));
        }
        case Token::TOK_STR:
        {
            return builder->CreateGlobalString(root->token.data);
        }
        default:
        {
            break;
        }
    }
    return nullptr;
}

static llvm::Value *compile_un_op(std::unique_ptr<Node> root)
{
    llvm::Value *operand = compile_expr(std::move(root->children.front()));
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

static llvm::Value *compile_bin_op(std::unique_ptr<Node> root)
{
    llvm::Value *left = compile_expr(std::move(root->children.front()));
    llvm::Value *right = compile_expr(std::move(root->children.back()));

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

static llvm::Value *compile_op(std::unique_ptr<Node> root)
{
    switch (root->children.size())
    {
        case 1:
        {
            return compile_un_op(std::move(root));
        }
        case 2:
        {
            return compile_bin_op(std::move(root));
        }
        default:
        {
            return nullptr;
        }
    }
}

static llvm::Value *compile_fun_call(std::unique_ptr<Node> root)
{
    llvm::Function *fun = module->getFunction(root->token.data);

    if (root->children.size())
    {
        std::vector<llvm::Value *> args;
        for (size_t i = 0; i < root->children.size(); i++)
        {
            args.push_back(compile_expr(std::move(root->children[i])));
        }

        return builder->CreateCall(fun, args);
    }

    return builder->CreateCall(fun);
}

static llvm::Value *compile_built_in(std::unique_ptr<Node> root)
{
    switch (root->token.type)
    {
        case Token::KEY_PRINT:
        {
            if (root->children.front()->type_scheme == construct_type(type_names["num"]))
            {
                root->token.data = "print_num";
                return compile_fun_call(std::move(root));
            }
            else if (root->children.front()->type_scheme == construct_type(type_names["bool"]))
            {
                root->token.data = "print_bool";
                return compile_fun_call(std::move(root));
            }
            else if (root->children.front()->type_scheme == construct_type(type_names["str"]))
            {
                root->token.data = "print_str";
                return compile_fun_call(std::move(root));
            }
            return nullptr;
        }
        default:
        {
            return nullptr;
        }
    }
}

static llvm::Value *compile_var(std::unique_ptr<Node> root)
{
    llvm::AllocaInst *alloc = values[root->token.data];
    return builder->CreateLoad(alloc->getAllocatedType(), alloc, root->token.data.c_str());
}

static llvm::Value *compile_var_asn(std::unique_ptr<Node> root)
{
    llvm::Value *rhs = compile_expr(std::move(root->children.back()));

    if (!values[root->children.front()->token.data])
    {
        llvm::Function *f = builder->GetInsertBlock()->getParent();
        llvm::AllocaInst *alloc = allocate_var(f, root->children.front()->token.data, get_llvm_type(root->children.front()->type_scheme));
        values[root->children.front()->token.data] = alloc;
    }

    builder->CreateStore(rhs, values[root->children.front()->token.data]);

    return nullptr;
}

static llvm::Value *compile_lone_if(std::unique_ptr<Node> root)
{
    std::unique_ptr<Node> cond = std::move(root->children.front());
    std::unique_ptr<Node> body = std::move(root->children.back());

    llvm::Function *fun = builder->GetInsertBlock()->getParent();
    llvm::Value *cond_val = compile_expr(std::move(cond));

    llvm::BasicBlock *then_blk = llvm::BasicBlock::Create(*context, "", fun);
    llvm::BasicBlock *end_blk = llvm::BasicBlock::Create(*context, "", fun);

    builder->CreateCondBr(cond_val, then_blk, end_blk);
    builder->SetInsertPoint(then_blk);

    for (size_t i = 0; i < body->children.size(); i++)
    {
        compile_expr(std::move(body->children[i]));
    }
    builder->CreateBr(end_blk);
    builder->SetInsertPoint(end_blk);

    root->children.back() = std::move(body);
    root->children.front() = std::move(cond);

    return llvm::Constant::getNullValue(llvm::Type::getVoidTy(*context));
}

static void compile_if_else(std::unique_ptr<Node> root,
                            llvm::Function *fun,
                            llvm::BasicBlock *merge_blk,
                            std::vector<llvm::Value *> &phis,
                            std::vector<llvm::BasicBlock *> &blks)
{
    switch (root->children.size())
    {
        case 1:
        {
            std::unique_ptr<Node> body = std::move(root->children.front());
            for (size_t i = 0; i < body->children.size() - 1; i++)
            {
                compile_expr(std::move(body->children[i]));
            }
            phis.push_back(compile_expr(std::move(body->children.back())));

            root->children.front() = std::move(body);
            break;
        }
        case 3:
        {
            std::unique_ptr<Node> cond = std::move(root->children.front());
            std::unique_ptr<Node> then = std::move(root->children[1]);
            std::unique_ptr<Node> els = std::move(root->children.back());

            llvm::Value *cond_val = compile_expr(std::move(cond));

            llvm::BasicBlock *then_blk = llvm::BasicBlock::Create(*context, "", fun);
            llvm::BasicBlock *else_blk = llvm::BasicBlock::Create(*context, "", fun);

            builder->CreateCondBr(cond_val, then_blk, else_blk);
            builder->SetInsertPoint(then_blk);

            for (size_t i = 0; i < then->children.size() - 1; i++)
            {
                compile_expr(std::move(then->children[i]));
            }
            phis.push_back(compile_expr(std::move(then->children.back())));
            blks.push_back(then_blk);

            builder->CreateBr(merge_blk);
            builder->SetInsertPoint(else_blk);
            compile_if_else(std::move(els), fun, merge_blk, phis, blks);
            blks.push_back(else_blk);

            root->children.front() = std::move(cond);
            root->children[1] = std::move(then);
            root->children.back() = std::move(els);
            break;
        }
        default:
        {
            break;
        }
    }

}

static llvm::Value *compile_if(std::unique_ptr<Node> root)
{
    switch (root->children.size())
    {
        case 2:
        {
            return compile_lone_if(std::move(root));
        }
        case 3:
        {
            llvm::Function *fun = builder->GetInsertBlock()->getParent();
            llvm::BasicBlock *merge_blk = llvm::BasicBlock::Create(*context, "", fun);
            std::vector<llvm::Value *> phis;
            std::vector<llvm::BasicBlock *> blks;

            TypeScheme ts = root->type_scheme;
            bool nil_statement = ts == construct_type(type_names["nil"]);
            compile_if_else(std::move(root), fun, merge_blk, phis, blks);

            builder->CreateBr(merge_blk);
            builder->SetInsertPoint(merge_blk);
            if (!nil_statement)
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

static llvm::Value *compile_loop(std::unique_ptr<Node> root)
{
    std::unique_ptr<Node> cond = std::move(root->children.front());
    std::unique_ptr<Node> body = std::move(root->children.back());

    llvm::Function *fun = builder->GetInsertBlock()->getParent();

    llvm::BasicBlock *body_blk = llvm::BasicBlock::Create(*context, "", fun);
    llvm::BasicBlock *cond_blk = llvm::BasicBlock::Create(*context, "", fun);
    llvm::BasicBlock *exit_blk = llvm::BasicBlock::Create(*context, "", fun);

    builder->CreateBr(cond_blk);
    builder->SetInsertPoint(cond_blk);
    llvm::Value *cond_val = compile_expr(std::move(cond));
    builder->CreateCondBr(cond_val, body_blk, exit_blk);

    builder->SetInsertPoint(body_blk);
    for (size_t i = 0; i < body->children.size(); i++)
    {
        compile_expr(std::move(body->children[i]));
    }
    builder->CreateBr(cond_blk);
    builder->SetInsertPoint(exit_blk);

    root->children.back() = std::move(body);
    root->children.front() = std::move(cond);

    return llvm::Constant::getNullValue(llvm::Type::getVoidTy(*context));
}

static llvm::Value *compile_expr(std::unique_ptr<Node> root)
{
    switch (root->type)
    {
        case Node::NODE_LIT:
        {
            return compile_literal(std::move(root));
        }
        case Node::NODE_OP:
        {
            return compile_op(std::move(root));
        }
        case Node::NODE_FUN_CALL:
        {
            return compile_fun_call(std::move(root));
        } 
        case Node::NODE_BUILT_IN:
        {
            return compile_built_in(std::move(root));
        }
        case Node::NODE_VAR:
        {
            return compile_var(std::move(root));
        }
        case Node::NODE_VAR_ASN:
        {
            return compile_var_asn(std::move(root));
        }
        case Node::NODE_IF:
        {
            return compile_if(std::move(root));
        }
        case Node::NODE_LOOP:
        {
            return compile_loop(std::move(root));
        }
        default:
        {
            return nullptr;
        }
    }
}

static std::unique_ptr<Node> compile_fun_dec(std::unique_ptr<Node> root)
{
    std::unique_ptr<Node> in = std::move(root->children.front());
    std::unique_ptr<Node> out = std::move(root->children[1]);
    std::unique_ptr<Node> body = std::move(root->children.back());

    bool nil_input = in->children.front()->token.type == Token::KEY_NIL;
    bool nil_output = out->children.front()->token.type == Token::KEY_NIL;

    llvm::FunctionType *fun_type;

    if (!nil_input)
    {
        std::vector<llvm::Type *> arg_types;
        for (size_t i = 0; i < in->children.size() && !nil_input; i++)
        {
            arg_types.push_back(get_llvm_type(body->children[i]->type_scheme));
        }

        fun_type = llvm::FunctionType::get(
            !nil_output ? get_llvm_type(body->children.back()->type_scheme) : llvm::Type::getVoidTy(*context), 
            arg_types, false
        );
    }
    else
    {
        fun_type = llvm::FunctionType::get(
            !nil_output ? get_llvm_type(body->children.back()->type_scheme) : llvm::Type::getVoidTy(*context),
            false
        );
    }

    llvm::Function *fun = llvm::Function::Create(fun_type, llvm::Function::ExternalLinkage, root->token.data, module.get());

    if (!nil_input)
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

    if (!nil_input)
    {
        for (auto &arg : fun->args())
        {
            llvm::AllocaInst *alloc = allocate_var(fun, std::string(arg.getName()), arg.getType());
            builder->CreateStore(&arg, alloc);
            values[std::string(arg.getName())] = alloc;
        }
    }

    size_t i = !nil_input ? in->children.size() : 0;
    if (body->children.size())
    {
        for (; i < body->children.size() - !nil_output; i++)
        {
            compile_expr(std::move(body->children[i]));
        }

        if (!nil_output)
        {
            builder->CreateRet(compile_expr(std::move(body->children.back())));
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

    root->children.front() = std::move(in);
    root->children[1] = std::move(out);
    root->children.back() = std::move(body);

    return root;
}

static void compile_program(std::unique_ptr<Node> root)
{
    for (size_t i = 0; i < root->children.size(); i++)
    {
        if (root->children[i]->type == Node::NODE_FUN_DEC)
        {
            root->children[i] = compile_fun_dec(std::move(root->children[i]));
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
            compile_expr(std::move(root->children[i]));
        }
    }

    llvm::Value *ret_zero = llvm::ConstantInt::get(*context, llvm::APInt(32, 0));
    builder->CreateRet(ret_zero);

    llvm::verifyFunction(*f);
}

void compile(std::unique_ptr<Node> root)
{
    context = std::make_unique<llvm::LLVMContext>();
    module = std::make_unique<llvm::Module>("", *context);
    builder = std::make_unique<llvm::IRBuilder<>>(*context);

    initialize_built_ins();
    compile_program(std::move(root));

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

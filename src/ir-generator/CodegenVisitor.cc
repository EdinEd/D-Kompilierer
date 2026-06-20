#include "CodegenVisitor.hh"
#include "llvm/IR/Verifier.h"
#include "../ast/ASTNode.hh"

CodegenVisitor::CodegenVisitor()
{
    context = std::make_unique<llvm::LLVMContext>();
    builder = std::make_unique<llvm::IRBuilder<>>(*context);
    module = std::make_unique<llvm::Module>("Compiler", *context);
}

void CodegenVisitor::safeOutput()
{
    std::error_code errorCode;
    llvm::raw_fd_ostream dest("output.ll", errorCode, llvm::sys::fs::OF_None);

    if (errorCode)
    {
        std::cerr << "Could not open output file: " << errorCode.message() << std::endl;
        return;
    }
    module->print(dest, nullptr);
    dest.flush();

    std::cout << "Succesfully saved output." << std::endl;
}

llvm::Type *CodegenVisitor::getLLVMType(Type t)
{
    switch (t)
    {
    case Type::INTEGER:
        return llvm::Type::getInt32Ty(*context);
    case Type::STRING:
        return llvm::PointerType::getUnqual(*context);
    case Type::BOOLEAN:
        return llvm::Type::getInt1Ty(*context);
    case Type::VOID:
        return llvm::Type::getVoidTy(*context);
    default:
        return nullptr;
    }
}

llvm::Value *CodegenVisitor::logError(const std::string &str)
{
    std::cerr << "IR Generation Error: " << str << std::endl;
    return nullptr;
}

llvm::Function *CodegenVisitor::declareFunction(const FunctionDefNode *functionDef)
{
    std::vector<llvm::Type *> paramTypes;
    for (const auto &param : functionDef->parameters)
    {
        paramTypes.push_back(getLLVMType(param.type));
    }

    llvm::Type *returnType = getLLVMType(functionDef->return_type);
    llvm::FunctionType *funcType = llvm::FunctionType::get(returnType, paramTypes, false);

    return llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, functionDef->name, module.get());
}

void CodegenVisitor::visit(UnitNode *rootNode)
{
    for (const auto &func : rootNode->functions)
    {
        declareFunction(func.get());
    }

    for (const auto &func : rootNode->functions)
    {
        visit(func.get());
    }
    module->print(llvm::outs(), nullptr);
    safeOutput();
}

void CodegenVisitor::visit(FunctionDefNode *node)
{
    llvm::Function *function = module->getFunction(node->name);

    if (!function)
    {
        function = declareFunction(node);
    }

    llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(*context, "entry", function);
    builder->SetInsertPoint(entryBlock);

    namedValues.clear();

    unsigned Idx = 0;
    for (auto &Arg : function->args())
    {
        std::string argName = node->parameters[Idx].identifier;
        Arg.setName(argName);
        llvm::AllocaInst *alloca = builder->CreateAlloca(Arg.getType(), nullptr, argName);
        builder->CreateStore(&Arg, alloca);
        namedValues[argName] = alloca;
        Idx++;
    }

    if (node->body)
    {
        node->body->accept(*this);

        if (!builder->GetInsertBlock()->getTerminator())
        {
            if (node->return_type == Type::VOID)
            {
                builder->CreateRetVoid();
            }
            else
            {
                builder->CreateRet(llvm::UndefValue::get(getLLVMType(node->return_type)));
            }
        }

        if (llvm::verifyFunction(*function, &llvm::errs()))
        {
            logError("LLVM Verification failed for function: " + node->name);
            // function->eraseFromParent();
        }
        return;
    }
    function->eraseFromParent();
}

void CodegenVisitor::visit(BlockNode *node)
{
    for (const auto &stmt : node->statements)
    {
        stmt->accept(*this);
    }
}

void CodegenVisitor::visit(ReturnNode *node)
{
    if (node->expression)
    {
        node->expression->accept(*this);

        if (current_value)
        {
            builder->CreateRet(current_value);
        }
    }
    else
    {
        builder->CreateRetVoid();
    }
}

void CodegenVisitor::visit(WhileNode *node)
{
    node->condition->accept(*this);
    if (node->body)
    {
        node->body->accept(*this);
    }
}

void CodegenVisitor::visit(IfNode *node)
{
    node->condition->accept(*this);
    if (node->then_body)
    {
        node->then_body->accept(*this);
    }
    for (const auto &eif : node->else_ifs)
    {
        eif.condition->accept(*this);
        if (eif.body)
        {
            eif.body->accept(*this);
        }
    }
    if (node->else_body)
    {
        node->else_body->accept(*this);
    }
}

void CodegenVisitor::visit(VarDeclNode *node)
{
    llvm::Type *varType = getLLVMType(node->type);
    llvm::AllocaInst *alloca = builder->CreateAlloca(varType, nullptr, node->name);
    namedValues[node->name] = alloca;

    if (node->init_value)
    {
        node->init_value->accept(*this);
        if (current_value)
        {
            builder->CreateStore(current_value, alloca);
        }
    }
}

void CodegenVisitor::visit(BinaryOpNode *node)
{
    if (node->op == OperationType::ASSIGN)
    {
        VarRefNode *varRef = dynamic_cast<VarRefNode *>(node->left.get());
        if (!varRef)
        {
            current_value = logError("Left side of assignment must be a variable");
            return;
        }
        auto it = namedValues.find(varRef->name);
        if (it == namedValues.end())
        {
            current_value = logError("Unknown variable: " + varRef->name);
            return;
        }
        node->right->accept(*this);
        if (current_value)
        {
            builder->CreateStore(current_value, it->second);
        }
        return;
    }

    node->left->accept(*this);
    llvm::Value *L = current_value;
    if (!L)
        return;

    node->right->accept(*this);
    llvm::Value *R = current_value;
    if (!R)
        return;

    switch (node->op)
    {
    case OperationType::ADD:
        current_value = builder->CreateAdd(L, R, "addtmp");
        break;
    case OperationType::SUBTRACT:
        current_value = builder->CreateSub(L, R, "subtmp");
        break;
    case OperationType::MULTIPLY:
        current_value = builder->CreateMul(L, R, "multmp");
        break;
    case OperationType::DIVIDE:
        current_value = builder->CreateSDiv(L, R, "divtmp");
        break;
    case OperationType::MODULO:
        current_value = builder->CreateSRem(L, R, "modtmp");
        break;
    case OperationType::EQ:
        current_value = builder->CreateICmpEQ(L, R, "eqtmp");
        break;
    case OperationType::NEQ:
        current_value = builder->CreateICmpNE(L, R, "neqtmp");
        break;
    case OperationType::LESS:
        current_value = builder->CreateICmpSLT(L, R, "lttmp");
        break;
    case OperationType::LEQ:
        current_value = builder->CreateICmpSLE(L, R, "letmp");
        break;
    case OperationType::GEQ:
        current_value = builder->CreateICmpSGE(L, R, "getmp");
        break;
    case OperationType::GREATER:
        current_value = builder->CreateICmpSGT(L, R, "gttmp");
        break;
    default:
        current_value = logError("Unknown binary operator");
        break;
    }
}

void CodegenVisitor::visit(VarRefNode *node)
{
    auto it = namedValues.find(node->name);
    if (it == namedValues.end())
    {
        current_value = logError("Unknown variable: " + node->name);
        return;
    }
    current_value = builder->CreateLoad(it->second->getAllocatedType(), it->second, node->name);
}

void CodegenVisitor::visit(StringNode *node)
{
    current_value = builder->CreateGlobalString(node->value, "str");
}

void CodegenVisitor::visit(NumberNode *node)
{
    current_value = llvm::ConstantInt::get(*context, llvm::APInt(32, node->value, true));
}

void CodegenVisitor::visit(BoolNode *node)
{
    current_value = llvm::ConstantInt::get(*context, llvm::APInt(1, node->value ? 1 : 0));
}

void CodegenVisitor::visit(FunctionCallNode *node)
{
    llvm::Function *callee = module->getFunction(node->function_name);
    if (!callee)
    {
        current_value = logError("Unknown function: " + node->function_name);
        return;
    }
    if (callee->arg_size() != node->params.size())
    {
        current_value = logError("Incorrect number of arguments for " + node->function_name);
        return;
    }

    std::vector<llvm::Value *> args;
    for (const auto &param : node->params)
    {
        param->accept(*this);
        if (!current_value)
            return;
        args.push_back(current_value);
    }

    current_value = builder->CreateCall(callee, args, "calltmp");
}

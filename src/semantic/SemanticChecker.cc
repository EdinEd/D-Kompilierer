#include "SemanticChecker.hh"

bool SemanticChecker::hasErrors() const
{
    return !errors.empty();
}

void SemanticChecker::printErrors() const
{
    for (const auto &error : errors)
    {
        std::cerr << error->format() << std::endl;
    }
}

void SemanticChecker::visit(UnitNode *rootNode)
{
    rootSymbolTable = std::make_shared<Symboltable>();
    symbolTableStack.push(rootSymbolTable.get());
    for (const auto &func : rootNode->functions)
    {
        visit(func.get());
    }
    symbolTableStack.pop();
}

void SemanticChecker::visit(FunctionDefNode *node)
{
    constructNewSymbolTable();
    addFunctionParametersToTable(*node, *symbolTableStack.top());
    functionBodyMode = true;
    functionReturnType = node->return_type;
    currentFunctionLocation = node->location;

    if (node->body)
    {
        node->body->accept(*this);
    }

    validateFuncDeclCorrectness(functionReturnType, functionBodyHasReturn);

    functionBodyMode = false;
    functionBodyHasReturn = false;
    symbolTableStack.pop();
}

void SemanticChecker::validateFuncDeclCorrectness(const Type &returnType, const bool &hasReturn)
{
    if (returnType == Type::VOID && hasReturn)
    {
        errors.push_back(std::make_unique<GenericSemanticError>(
            "Void function should not return a value", currentFunctionLocation));
    }
    else if (returnType != Type::VOID && !hasReturn)
    {
        errors.push_back(std::make_unique<GenericSemanticError>(
            "Non-void function should have a return statement", currentFunctionLocation));
    }
}

Symboltable *SemanticChecker::constructNewSymbolTable()
{
    auto newTable = std::make_unique<Symboltable>(symbolTableStack.top());
    Symboltable *childPtr = newTable.get();
    symbolTableStack.top()->addChild(std::move(newTable));
    symbolTableStack.push(childPtr);
    return childPtr;
}

bool SemanticChecker::addFunctionParametersToTable(const FunctionDefNode &node, Symboltable &symbolTable)
{
    IdentifierInfo paramInfo;
    bool errorOccured = false;
    for (const auto &param : node.parameters)
    {
        paramInfo = {node.location, param.type};
        if (!symbolTable.insert(param.identifier, paramInfo))
        {
            auto declInfo = symbolTable.identify(param.identifier);
            errors.push_back(std::make_unique<AlreadyDefinedError>(
                param.identifier, "Parameter", declInfo->location, node.location));
            errorOccured = true;
        }
    }
    return !errorOccured;
}

void SemanticChecker::visit(FunctionCallNode *node)
{
    auto funcSignature = functionTable.identify(node->function_name);
    if (!validateFuncSignature(node, funcSignature))
    {
        current_exp_type = Type::INVALID;
        return;
    }

    for (int i = 0; i < node->params.size(); ++i)
    {
        node->params[i]->accept(*this);
        const Type arg_type = current_exp_type;

        if (arg_type != funcSignature->parameterTypes[i])
        {
            errors.push_back(std::make_unique<ArgumentTypeMismatchError>(
                funcSignature->parameterTypes[i], arg_type,
                node->function_name, i + 1, node->location));
            current_exp_type = Type::INVALID;
            return;
        }
    }
    current_exp_type = funcSignature->returnType;
}

bool SemanticChecker::validateFuncSignature(const FunctionCallNode *node, const FunctionSignature *funcSignature)
{
    if (!funcSignature)
    {
        errors.push_back(std::make_unique<UndefinedError>(
            node->function_name, "function", node->location));
        return false;
    }
    if (funcSignature->parameterTypes.size() != node->params.size())
    {
        errors.push_back(std::make_unique<WrongArgumentCountError>(
            node->function_name, funcSignature->parameterTypes.size(),
            node->params.size(), node->location));
        return false;
    }
    return true;
}

void SemanticChecker::visit(BlockNode *node)
{
    const bool isFunctionBody = functionBodyMode;
    functionBodyMode = false;
    if (!isFunctionBody)
    {
        constructNewSymbolTable();
    }

    for (const auto &stmt : node->statements)
    {
        stmt->accept(*this);
        if (isFunctionBody && stmt->node_type == ASTNodeType::RETURN)
        {
            functionBodyHasReturn = true;
        }
        if (stmt->node_type == ASTNodeType::RETURN)
        {
            if (current_exp_type != functionReturnType)
            {
                errors.push_back(std::make_unique<TypeMismatchError>(
                    functionReturnType, current_exp_type, ASTNodeType::RETURN, stmt->location));
            }
        }
    }

    if (!isFunctionBody)
    {
        symbolTableStack.pop();
    }
}

void SemanticChecker::visit(ReturnNode *node)
{
    if (node->expression)
    {
        node->expression->accept(*this);
    }
}

void SemanticChecker::visit(WhileNode *node)
{
    node->condition->accept(*this);
    if (current_exp_type != Type::BOOLEAN)
    {
        errors.push_back(std::make_unique<TypeMismatchError>(
            Type::BOOLEAN, current_exp_type, ASTNodeType::WHILE, node->condition->location));
    }

    if (node->body)
    {
        node->body->accept(*this);
    }
}

void SemanticChecker::visit(VarDeclNode *node)
{
    IdentifierInfo varInfo{node->location, node->type};
    if (!symbolTableStack.top()->insert(node->name, varInfo))
    {
        auto declInfo = symbolTableStack.top()->identify(node->name);
        errors.push_back(std::make_unique<AlreadyDefinedError>(
            node->name, "variable", declInfo->location, node->location));
        return;
    }
    if (node->init_value)
    {
        node->init_value->accept(*this);
        validateBinaryOpTypes(node->location, node->type, current_exp_type);
    }
}

void SemanticChecker::visit(BinaryOpNode *node)
{
    node->left->accept(*this);
    const Type left_exp_type = current_exp_type;

    node->right->accept(*this);
    const Type right_exp_type = current_exp_type;

    switch (node->op)
    {
    case OperationType::ADD:
    case OperationType::SUBTRACT:
    case OperationType::MULTIPLY:
    case OperationType::DIVIDE:
    case OperationType::MODULO:
        validateBinaryOpCalcTypes(node->location, left_exp_type, right_exp_type);
        break;
    case OperationType::ASSIGN:
        validateBinaryOpTypes(node->location, left_exp_type, right_exp_type);
        break;
    default:
        errors.push_back(std::make_unique<GenericSemanticError>(
            "Unknown binary operator", node->location));
        current_exp_type = Type::INVALID;
        break;
    }
}

void SemanticChecker::validateBinaryOpTypes(const SourceLocation &location, const Type &leftType, const Type &rightType)
{
    if (leftType == Type::INVALID || rightType == Type::INVALID)
    {
        current_exp_type = Type::INVALID;
    }
    else if (leftType != rightType)
    {
        errors.push_back(std::make_unique<TypeMismatchError>(
            leftType, rightType, ASTNodeType::BINARY_OP, location));
        current_exp_type = Type::INVALID;
    }
    else
    {
        current_exp_type = leftType;
    }
}

void SemanticChecker::validateBinaryOpCalcTypes(const SourceLocation &location, const Type &leftType, const Type &rightType)
{
    if (leftType != Type::INTEGER)
    {
        errors.push_back(std::make_unique<GenericSemanticError>(
            "Binary operator requires integer operands", location));
        current_exp_type = Type::INVALID;
    }
    validateBinaryOpTypes(location, leftType, rightType);
}

void SemanticChecker::visit(VarRefNode *node)
{
    IdentifierInfo *info = symbolTableStack.top()->identify(node->name);
    if (!info)
    {
        errors.push_back(std::make_unique<UndefinedError>(
            node->name, "variable", node->location));
        current_exp_type = Type::INVALID;
        return;
    }
    current_exp_type = info->type;
}

void SemanticChecker::visit(StringNode *node)
{
    current_exp_type = Type::STRING;
}

void SemanticChecker::visit(NumberNode *node)
{
    current_exp_type = Type::INTEGER;
}

void SemanticChecker::visit(BoolNode* node){
    current_exp_type = Type::BOOLEAN;
}


std::shared_ptr<Symboltable> SemanticChecker::getCurrentVariableSymbolTable()
{
    return rootSymbolTable;
}

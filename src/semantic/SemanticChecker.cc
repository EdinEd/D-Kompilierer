#include "SemanticChecker.hh"

bool SemanticChecker::hasErrors() const
{
    return !errors.empty();
}

void SemanticChecker::printErrors() const
{
    for (const auto &error : errors)
    {
        std::cerr << error.message << " at " << error.location << std::endl;
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

    if (node->body)
    {
        node->body->accept(*this);
    }

    symbolTableStack.pop();
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
            errors.push_back({"Error: Parameter '" + param.identifier + "' already defined", declInfo->location});
            errorOccured = true;
        }
    }
    return !errorOccured;
}

void SemanticChecker::visit(FunctionCallNode *node)
{
    auto funcSignature = functionTable.identify(node->function_name);
    if (!funcSignature)
    {
        errors.push_back({"Error: Function '" + node->function_name + "' not defined", node->location});
        return;
    }
    if (funcSignature->parameterTypes.size() != node->params.size())
    {
        errors.push_back({"Error: Function '" + node->function_name + "' called with incorrect number of arguments", node->location});
        return;
    }

    for (int i = 0; i < node->params.size(); ++i)
    {
        node->params[i]->accept(*this);
        const Type arg_type = current_exp_type;
        
        if (arg_type != funcSignature->parameterTypes[i])
        {
            errors.push_back({"Error: Argument type mismatch for parameter " + std::to_string(i + 1) + " in function '" + node->function_name + "'", node->location});
            return;
        }
    }
    current_exp_type = funcSignature->returnType;
}

void SemanticChecker::visit(BlockNode *node)
{
    const bool isFunctionBody = functionBodyMode;
    if (!isFunctionBody)
    {
        constructNewSymbolTable();
    }
    functionBodyMode = false;

    for (const auto &stmt : node->statements)
    {
        stmt->accept(*this);
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

void SemanticChecker::visit(VarDeclNode *node)
{
    IdentifierInfo varInfo{node->location, node->type};
    if (!symbolTableStack.top()->insert(node->name, varInfo))
    {
        auto declInfo = symbolTableStack.top()->identify(node->name);
        errors.push_back({"Error: Variable '" + node->name + "' already defined", declInfo->location});
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
        errors.push_back({"Error: Unknown binary operator", node->location});
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
        errors.push_back({"Error: Type mismatch in binary operation", location});
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
        errors.push_back({"Error: Binary operator requires integer operands", location});
        current_exp_type = Type::INVALID;
    }
    validateBinaryOpTypes(location, leftType, rightType);
}

void SemanticChecker::visit(VarRefNode *node)
{
    IdentifierInfo *info = symbolTableStack.top()->identify(node->name);
    if (!info)
    {
        errors.push_back({"Error: Variable '" + node->name + "' not defined", node->location});
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

std::shared_ptr<Symboltable> SemanticChecker::getCurrentVariableSymbolTable()
{
    return rootSymbolTable;
}
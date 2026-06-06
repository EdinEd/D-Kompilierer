#include "SemanticChecker.hh"

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
            std::cerr << "Error: Parameter '" << param.identifier << "' already defined at " << declInfo->location << std::endl;
            errorOccured = true;
        }
    }
    return !errorOccured;
}

void SemanticChecker::visit(FunctionCallNode *node)
{
    // Check if function is defined
    auto funcSignature = functionTable.identify(node->function_name);
    if (!funcSignature)
    {
        std::cerr << "Error: Function '" << node->function_name << "' not defined at " << node->location << std::endl;
        return;
    }
    if (funcSignature->parameterTypes.size() != node->params.size())
    {
        std::cerr << "Error: Function '" << node->function_name << "' called with incorrect number of arguments at " << node->location << std::endl;
        return;
    }

    for (const auto &param : node->params)
    {
        param->accept(*this);
    }
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
        std::cerr << "Error: Variable '" << node->name << "' already defined at " << declInfo->location << std::endl;
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
        std::cerr << "Error: Unknown binary operator at " << node->location << std::endl;
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
        std::cerr << "Error: Type mismatch in binary operation at " << location << std::endl;
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
        std::cerr << "Error: Binary operator requires integer operands at " << location << std::endl;
        current_exp_type = Type::INVALID;
    }
    validateBinaryOpTypes(location, leftType, rightType);
}

void SemanticChecker::visit(VarRefNode *node)
{
    IdentifierInfo *info = symbolTableStack.top()->identify(node->name);
    if (!info)
    {
        std::cerr << "Error: Variable '" << node->name << "' not defined at " << node->location << std::endl;
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
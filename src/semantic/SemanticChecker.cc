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
    // TODO: Überarbeiten
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
    }
    if (node->init_value)
    {
        node->init_value->accept(*this);
    }
}

void SemanticChecker::visit(BinaryOpNode *node)
{
    node->left->accept(*this);
    node->right->accept(*this);
}

void SemanticChecker::visit(VarRefNode *node)
{
    IdentifierInfo *info = symbolTableStack.top()->identify(node->name);
    if (!info)
    {
        std::cerr << "Error: Variable '" << node->name << "' not defined at " << node->location << std::endl;
    }
}

void SemanticChecker::visit(StringNode *node)
{
}

void SemanticChecker::visit(NumberNode *node)
{
}

std::shared_ptr<Symboltable> SemanticChecker::getCurrentVariableSymbolTable()
{
    return rootSymbolTable;
}
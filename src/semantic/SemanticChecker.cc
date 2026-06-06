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
    IdentifierInfo funcInfo{node->location, node->return_type};
    if (!symbolTableStack.top()->insert(node->name, funcInfo))
    {
        std::cerr << "Error: Function '" << node->name << "' already defined at " << node->location << std::endl;
    }
    if (node->body)
    {
        node->body->accept(*this);
    }
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
    auto newTable = std::make_unique<Symboltable>(symbolTableStack.top());
    Symboltable *childPtr = newTable.get();
    symbolTableStack.top()->addChild(std::move(newTable));
    symbolTableStack.push(childPtr);
    for (const auto &stmt : node->statements)
    {
        stmt->accept(*this);
    }
    symbolTableStack.pop();
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
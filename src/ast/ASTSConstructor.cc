#include "ASTSConstructor.hh"

void ASTSConstructor::visit(UnitNode *rootNode)
{
    symbolTableStack.push(std::make_shared<Symboltable>());
    for (const auto &func : rootNode->functions)
    {
        visit(func.get());
    }
}

void ASTSConstructor::visit(FunctionDefNode *node)
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

void ASTSConstructor::visit(BlockNode *node)
{
    auto newTable = std::make_shared<Symboltable>(symbolTableStack.top());
    symbolTableStack.push(newTable);
    for (const auto &stmt : node->statements)
    {
        stmt->accept(*this);
    }
    symbolTableStack.pop();
}

void ASTSConstructor::visit(ReturnNode *node)
{
    if (node->expression)
    {
        node->expression->accept(*this);
    }
}

void ASTSConstructor::visit(VarDeclNode *node)
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

void ASTSConstructor::visit(BinaryOpNode *node)
{
    node->left->accept(*this);
    node->right->accept(*this);
}

void ASTSConstructor::visit(VarRefNode *node)
{
    IdentifierInfo *info = symbolTableStack.top()->identify(node->name);
    if (!info)
    {
        std::cerr << "Error: Variable '" << node->name << "' not defined at " << node->location << std::endl;
    }
}

void ASTSConstructor::visit(StringNode *node)
{

}

void ASTSConstructor::visit(NumberNode *node)
{

}

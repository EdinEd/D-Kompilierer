#include "ASTTraversal.hh"
#include <iostream>

std::string printRetrunType(Type &t)
{
    switch (t)
    {
    case Type::INTEGER:
        return "INTEGER";
    case Type::STRING:
        return "STRING";
    case Type::BOOLEAN:
        return "BOOLEAN";
    case Type::VOID:
        return "VOID";
    }
}

void ASTtraversal::visit(UnitNode *rootNode)
{
    for (const auto &func : rootNode->functions)
    {
        std::cout << "Visiting function: " << func->name << " -> " << printRetrunType(func->return_type) << std::endl;
        visit(func.get());
    }
}

void ASTtraversal::visit(FunctionDefNode *node)
{
    if (node->body)
    {
        std::cout << "Visiting function body of: " << node->name << std::endl;
    }
}
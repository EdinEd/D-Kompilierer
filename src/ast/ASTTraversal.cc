#include "ASTTraversal.hh"
#include <iostream>

void ASTtraversal::visit(UnitNode *rootNode)
{
    for (const auto &func : rootNode->functions)
    {
        std::cout << "Visiting function: " << func->name << std::endl;
        visit(func.get());
    }
}

void ASTtraversal::visit(FunctionDefNode *node)
{
    for (const auto &block : node->body)
    {
        std::cout << "Visiting block in function: " << node->name << std::endl;
    }
}
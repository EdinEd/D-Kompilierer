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

std::string getNodeTypeName(ASTNodeType &t)
{
    switch (t)
    {
    case ASTNodeType::NUMBER:
        return "NUMBER";
    case ASTNodeType::STRING:
        return "STRING";
    case ASTNodeType::VAR_REF:
        return "VAR_REF";
    case ASTNodeType::BINARY_OP:
        return "BINARY_OP";
    case ASTNodeType::VAR_DECL:
        return "VAR_DECL";
    case ASTNodeType::RETURN:
        return "RETURN";
    case ASTNodeType::BLOCK:
        return "BLOCK";
    case ASTNodeType::FUNCTION_DEF:
        return "FUNCTION_DEF";
    case ASTNodeType::UNIT:
        return "UNIT";
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
        for (const auto &stmt : node->body->statements)
        {
            std::cout << getNodeTypeName(stmt->node_type) << std::endl;
        }
    }
}
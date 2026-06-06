#include "GlobalDeclPass.hh"

void GlobalDeclPass::visit(UnitNode *rootNode)
{
    for (const auto &child : rootNode->functions)
    {
        child->accept(*this);
    }
}

void GlobalDeclPass::visit(FunctionDefNode *node)
{
    FunctionSignature signature;
    signature.returnType = node->return_type;
    signature.location = node->location;
    for (const auto &param : node->parameters)
    {
        signature.parameterTypes.push_back(param.type);
    }
    if (!functionTable.insert(node->name, signature))
    {
        // TODO: Custom error class
        std::cerr << "Error: Function '" << node->name << "' already defined at " << signature.location << std::endl;
    }
}
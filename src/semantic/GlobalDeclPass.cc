#include "GlobalDeclPass.hh"

void GlobalDeclPass::visit(UnitNode *rootNode)
{
    for (const auto &child : rootNode->functions)
    {
        child->accept(*this);
    }
}

bool GlobalDeclPass::hasErrors() const
{
    return !errors.empty();
}

void GlobalDeclPass::printErrors() const
{
    for (const auto &error : errors)
    {
        std::cerr << error->format() << std::endl;
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
        errors.push_back(std::make_unique<AlreadyDefinedError>(
            node->name, "function", signature.location, node->location));
    }
}

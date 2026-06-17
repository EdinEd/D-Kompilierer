#include "GlobalDeclPass.hh"
#include "SemanticExceptions.hh"
#include <memory>

void GlobalDeclPass::visit(UnitNode *rootNode)
{
    for (const auto &child : rootNode->functions)
    {
        child->accept(*this);
    }
    checkGlobalSemantics();
}

bool GlobalDeclPass::checkGlobalSemantics()
{
    auto mainFunctionSignature = functionTable.identify(mainFunctionName);
    auto dummyLocation = SourceLocation(0, 0, 0, 0);
    if (!mainFunctionSignature)
    {
        errors.push_back(std::make_unique<GenericSemanticError>(
            "A 'haupt' function must be declared", dummyLocation));
        return false;
    }
    auto returnType = mainFunctionSignature->returnType;
    if (returnType != Type::VOID)
    {
        errors.push_back(std::make_unique<GenericSemanticError>(
            "The 'haupt' function should return an VOID", dummyLocation));
        return false;
    }
    return true;
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

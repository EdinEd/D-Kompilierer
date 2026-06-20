#include "CodegenVisitor.hh"
#include <memory>
#include "../ast/ASTNode.hh"

CodegenVisitor::CodegenVisitor()
{
    context = std::make_unique<llvm::LLVMContext>();
    std::make_unique<llvm::IRBuilder<>>(*context);
    module = std::make_unique<llvm::Module>("Compiler", *context);
}
void CodegenVisitor::visit(UnitNode *rootNode)
{

    module->print(llvm::outs(), nullptr);
}

void CodegenVisitor::visit(NumberNode *node)
{
    current_value = llvm::ConstantInt::get(*context, llvm::APInt(32, node->value, true));
}

llvm::Value *CodegenVisitor::logError(const std::string &str)
{
    // TODO: Das hier speäter in Codegen Visiotr mit custom fehlerklasse etc.
    std::cerr << "IR Generation Error: " << str << std::endl;
    return nullptr;
}

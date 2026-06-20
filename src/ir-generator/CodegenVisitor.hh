#pragma once
#include "../ast/ASTVisitor.hh"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <memory>
#include <map>
#include <iostream>

class CodegenVisitor : public ASTVisitor
{
public:
    CodegenVisitor();

    void visit(UnitNode *rootNode) override;

private:
    std::unique_ptr<llvm::LLVMContext> context;
    std::unique_ptr<llvm::IRBuilder<>> builder;
    std::unique_ptr<llvm::Module> module;
    std::map<std::string, llvm::Value *> namedValues;

    llvm::Value *current_value = nullptr;
    llvm::Value *logError(const std::string &str);

    void visit(FunctionDefNode *node) override {};
    void visit(FunctionCallNode *node) override {};
    void visit(BlockNode *node) override {};
    void visit(ReturnNode *node) override {};
    void visit(WhileNode *node) override {};
    void visit(VarDeclNode *node) override {};
    void visit(BinaryOpNode *node) override {};
    void visit(VarRefNode *node) override {};
    void visit(StringNode *node) override {};
    void visit(NumberNode *node) override;
    void visit(BoolNode *node) override {};
    void visit(IfNode *node) override {};
};

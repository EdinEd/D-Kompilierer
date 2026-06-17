#pragma once
#include "../symboltable/FunctionSymboltable.hh"
#include "../ast/ASTNode.hh"
#include "../semantic/SemanticExceptions.hh"
#include <memory>
#include <vector>
class GlobalDeclPass : public ASTVisitor
{
public:
    GlobalDeclPass() = default;
    void visit(UnitNode *rootNode) override;
    const FunctionSymboltable &getFunctionTable() const
    {
        return functionTable;
    }
    bool hasErrors() const;
    void printErrors() const;

private:
    FunctionSymboltable functionTable;
    std::vector<std::unique_ptr<SemanticException>> errors;

    void visit(FunctionDefNode *node) override;
    void visit(FunctionCallNode *node) override {};
    void visit(BlockNode *node) override {};
    void visit(ReturnNode *node) override {};
    void visit(WhileNode *node) override {};
    void visit(VarDeclNode *node) override {};
    void visit(BinaryOpNode *node) override {};
    void visit(VarRefNode *node) override {};
    void visit(StringNode *node) override {};
    void visit(NumberNode *node) override {};
    void visit(BoolNode *node) override {};
    void visit(IfNode *node) override {};

    bool checkGlobalSemantics();
    const std::string mainFunctionName = "haupt";
};
#pragma once
#include "../symboltable/FunctionSymboltable.hh"
#include "../ast/ASTNode.hh"

class GlobalDeclPass : public ASTVisitor
{
public:
    GlobalDeclPass(FunctionSymboltable &functionTable)
        : functionTable(functionTable) {}
    void visit(UnitNode *rootNode) override;
    const FunctionSymboltable &getFunctionTable() const
    {
        return functionTable;
    }

private:
    FunctionSymboltable &functionTable;

    void visit(FunctionDefNode *node) override;
    void visit(FunctionCallNode *node) override {};
    void visit(BlockNode *node) override {};
    void visit(ReturnNode *node) override {};
    void visit(VarDeclNode *node) override {};
    void visit(BinaryOpNode *node) override {};
    void visit(VarRefNode *node) override {};
    void visit(StringNode *node) override {};
    void visit(NumberNode *node) override {};
};
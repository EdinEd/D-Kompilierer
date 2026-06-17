#pragma once
#include "ASTVisitor.hh"
class ASTPrinter : public ASTVisitor
{
public:
    void visit(UnitNode *rootNode) override;
    void visit(FunctionDefNode *node) override;
    void visit(FunctionCallNode *node) override;
    void visit(BlockNode *node) override;
    void visit(ReturnNode *node) override;
    void visit(WhileNode *node) override;
    void visit(VarDeclNode *node) override;
    void visit(BinaryOpNode *node) override;
    void visit(VarRefNode *node) override;
    void visit(StringNode *node) override;
    void visit(NumberNode *node) override;
    void visit(BoolNode *node) override;
    void visit(IfNode *node) override;

private:
    int indent_level = 0;
    void printIndent();
};
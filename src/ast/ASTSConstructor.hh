#pragma once
#include "ASTVisitor.hh"
#include "../symboltable/Symboltable.h"
#include "ASTNode.hh"
#include <stack>

class ASTSConstructor : public ASTVisitor {
public:
    ASTSConstructor(): symbolTableStack(std::stack<std::shared_ptr<Symboltable>>()) {};
    std::shared_ptr<Symboltable> getCurrentSymbolTable();
    
    void visit(UnitNode* rootNode) override;
    void visit(FunctionDefNode* node)override;
    void visit(BlockNode* node) override;
    void visit(ReturnNode* node) override;
    void visit(VarDeclNode* node) override;
    void visit(BinaryOpNode* node) override;
    void visit(VarRefNode* node) override;
    void visit(StringNode* node) override;
    void visit(NumberNode* node) override;
private:
    std::stack<std::shared_ptr<Symboltable>> symbolTableStack;
};
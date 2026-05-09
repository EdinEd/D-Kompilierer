#pragma once
#include "ASTNode.hh"

class ASTtraversal {
public:
    void visit(UnitNode* rootNode);

private:
    void visit(FunctionDefNode* node);
    void visit(BlockNode* node);
    void visit(ReturnNode* node);
    void visit(VarDeclNode* node);
    void visit(BinaryOpNode* node);
    void visit(VarRefNode* node);
    void visit(StringNode* node);
    void visit(NumberNode* node);
};
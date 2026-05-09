#pragma once

class ASTNode;
class NumberNode;
class StringNode;
class VarRefNode;
class BinaryOpNode;
class VarDeclNode;
class ReturnNode;
class BlockNode;
class FunctionDefNode;
class UnitNode;

class ASTVisitor
{
public:
    virtual void visit(UnitNode *rootNode) = 0;
    virtual ~ASTVisitor();

    virtual void visit(FunctionDefNode *node) = 0;
    virtual void visit(BlockNode *node) = 0;
    virtual void visit(ReturnNode *node) = 0;
    virtual void visit(VarDeclNode *node) = 0;
    virtual void visit(BinaryOpNode *node) = 0;
    virtual void visit(VarRefNode *node) = 0;
    virtual void visit(StringNode *node) = 0;
    virtual void visit(NumberNode *node) = 0;
};
#include "ASTNode.hh"

void NumberNode::accept(ASTVisitor &visitor)
{
    visitor.visit(this);
}

void StringNode::accept(ASTVisitor &visitor)
{
    visitor.visit(this);
}

void VarRefNode::accept(ASTVisitor &visitor)
{
    visitor.visit(this);
}

void BinaryOpNode::accept(ASTVisitor &visitor)
{
    visitor.visit(this);
}

void VarDeclNode::accept(ASTVisitor &visitor)
{
    visitor.visit(this);
}

void ReturnNode::accept(ASTVisitor &visitor)
{
    visitor.visit(this);
}

void BlockNode::accept(ASTVisitor &visitor)
{
    visitor.visit(this);
}

void FunctionDefNode::accept(ASTVisitor &visitor)
{
    visitor.visit(this);
}

void FunctionCallNode::accept(ASTVisitor &visitor)
{
    visitor.visit(this);
}

void UnitNode::accept(ASTVisitor &visitor)
{
    visitor.visit(this);
}
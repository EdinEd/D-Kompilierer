#ifndef ASTNODE_HH
#define ASTNODE_HH
#include <iostream>
#include <memory>
#include "SourceLocation.hh"

enum class OperationType
{
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    ASSIGN
};

enum class Type
{
    INTEGER,
    STRING,
    BOOLEAN,
    VOID
};
class ASTNode
{
public:
    ASTNode(SourceLocation loc) : location(loc) {}
    virtual ~ASTNode() = default;
    SourceLocation location;

private:
};

class NumberNode : public ASTNode
{
public:
    NumberNode(SourceLocation loc, int value) : ASTNode(loc), value(value) {}

private:
    int value;
};

class StringNode : public ASTNode
{
public:
    StringNode(SourceLocation loc, const std::string &value) : ASTNode(loc), value(value) {}

private:
    std::string value;
};

class VarRefNode : public ASTNode
{
public:
    std::string name;
    VarRefNode(SourceLocation loc, const std::string &n) : ASTNode(loc), name(n) {}
};

class BinaryOpNode : public ASTNode
{
public:
    BinaryOpNode(SourceLocation loc, OperationType op, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r)
        : ASTNode(loc), op(op), left(std::move(l)), right(std::move(r)) {}

private:
    OperationType op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
};

class VarDeclNode : public ASTNode
{
public:
    Type type;
    std::string name;
    std::unique_ptr<ASTNode> init_value;

    VarDeclNode(SourceLocation loc, Type t, const std::string &n, std::unique_ptr<ASTNode> init)
        : ASTNode(loc), type(t), name(n), init_value(std::move(init)) {}
};

class ReturnNode : public ASTNode
{
public:
    std::unique_ptr<ASTNode> expression;

    ReturnNode(SourceLocation loc, std::unique_ptr<ASTNode> expr) : ASTNode(loc), expression(std::move(expr)) {}
};

class BlockNode : public ASTNode
{
public:
    std::vector<std::unique_ptr<ASTNode>> statements;

    void addStatement(std::unique_ptr<ASTNode> stmt)
    {
        statements.push_back(std::move(stmt));
    }
};
class FunctionDefNode : public ASTNode
{
public:
    Type return_type;
    std::string name;
    // std::vector<std::pair<Type, std::string>> parameters;
    std::vector<std::unique_ptr<BlockNode>> body;
    FunctionDefNode(SourceLocation loc, Type ret_type, const std::string &name)
        : ASTNode(loc), return_type(ret_type), name(name) {}
};

class UnitNode : public ASTNode
{
public:
    std::vector<std::unique_ptr<FunctionDefNode>> functions;

    UnitNode(SourceLocation l) : ASTNode(l) {}
    void addFunction(std::unique_ptr<FunctionDefNode> func) {
        functions.push_back(std::move(func));
    }
};

#endif // ! ASTNODE_HH
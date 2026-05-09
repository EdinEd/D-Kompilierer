#ifndef ASTNODE_HH
#define ASTNODE_HH
#include <iostream>
#include <memory>
#include "SourceLocation.hh"
#include "ASTVisitor.hh"


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

enum class ASTNodeType
{
    NUMBER,
    STRING,
    VAR_REF,
    BINARY_OP,
    VAR_DECL,
    RETURN,
    BLOCK,
    FUNCTION_DEF,
    UNIT
};
class ASTNode
{
public:
    ASTNode(SourceLocation loc, ASTNodeType type) : location(loc), node_type(type) {}
    virtual ~ASTNode() = default;
    SourceLocation location;
    ASTNodeType node_type;
    virtual void accept(ASTVisitor& visitor) = 0;
};

class NumberNode : public ASTNode
{
public:
    NumberNode(SourceLocation loc, int &value) : ASTNode(loc, ASTNodeType::NUMBER), value(value) {}
    int value;
    void accept(ASTVisitor& visitor) override;
};

class StringNode : public ASTNode
{
public:
    StringNode(SourceLocation loc, const std::string &value) : ASTNode(loc, ASTNodeType::STRING), value(value) {}
    std::string value;
    void accept(ASTVisitor& visitor) override;
};

class VarRefNode : public ASTNode
{
public:
    std::string name;
    VarRefNode(SourceLocation loc, const std::string &n) : ASTNode(loc, ASTNodeType::VAR_REF), name(n) {}
    void accept(ASTVisitor& visitor) override;
};

class BinaryOpNode : public ASTNode
{
public:
    BinaryOpNode(SourceLocation loc, OperationType op, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r)
        : ASTNode(loc, ASTNodeType::BINARY_OP), op(op), left(std::move(l)), right(std::move(r)) {}
    OperationType op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
    void accept(ASTVisitor& visitor) override;
};

class VarDeclNode : public ASTNode
{
public:
    Type type;
    std::string name;
    std::unique_ptr<ASTNode> init_value;

    VarDeclNode(SourceLocation loc, Type t, const std::string &n, std::unique_ptr<ASTNode> init)
        : ASTNode(loc, ASTNodeType::VAR_DECL), type(t), name(n), init_value(std::move(init)) {}
    void accept(ASTVisitor& visitor) override;
};

class ReturnNode : public ASTNode
{
public:
    std::unique_ptr<ASTNode> expression;

    ReturnNode(SourceLocation loc, std::unique_ptr<ASTNode> expr) : ASTNode(loc, ASTNodeType::RETURN), expression(std::move(expr)) {}
    void accept(ASTVisitor& visitor) override;
};

class BlockNode : public ASTNode
{
public:
    std::vector<std::unique_ptr<ASTNode>> statements;

    void addStatement(std::unique_ptr<ASTNode> stmt)
    {
        statements.push_back(std::move(stmt));
    }
    BlockNode(SourceLocation loc) : ASTNode(loc, ASTNodeType::BLOCK) {}
    void accept(ASTVisitor& visitor) override;
};
class FunctionDefNode : public ASTNode
{
public:
    Type return_type;
    std::string name;
    // std::vector<std::pair<Type, std::string>> parameters;
    std::unique_ptr<BlockNode> body;
    FunctionDefNode(SourceLocation loc, Type ret_type, const std::string &name, std::unique_ptr<BlockNode> body = nullptr)
        : ASTNode(loc, ASTNodeType::FUNCTION_DEF), return_type(ret_type), name(name), body(std::move(body)) {}
    void accept(ASTVisitor& visitor) override;
};

class UnitNode : public ASTNode
{
public:
    std::vector<std::unique_ptr<FunctionDefNode>> functions;

    UnitNode(SourceLocation l) : ASTNode(l, ASTNodeType::UNIT) {}
    void addFunction(std::unique_ptr<FunctionDefNode> func)
    {
        functions.push_back(std::move(func));
    }
    void accept(ASTVisitor& visitor) override;
};

#endif // ! ASTNODE_HH
#pragma once

enum class OperationType
{
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    ASSIGN,
    MODULO
};

enum class Type
{
    INVALID = -1 ,
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
    FUNCTION_CALL,
    UNIT,
    WHILE
};

inline std::string toString(Type returnType)
{
    switch (returnType)
    {
    case Type::INTEGER:
        return "INTEGER";
    case Type::STRING:
        return "STRING";
    case Type::BOOLEAN:
        return "BOOLEAN";
    case Type::VOID:
        return "VOID";
    case Type::INVALID:
        return "INVALID";
    default:
        return "UNKNOWN";
    }
}
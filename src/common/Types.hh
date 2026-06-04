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
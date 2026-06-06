#pragma once
#include "../common/SourceLocation.hh"
#include "../common/Types.hh"
#include <string>
#include <sstream>

struct SemanticError
{
    std::string message;
    SourceLocation location;
};

class SemanticException
{
public:
    explicit SemanticException(SourceLocation location) : location(location) {}
    virtual ~SemanticException() = default;
    virtual std::string format() const = 0;

    SourceLocation location;
};

class TypeMismatchError : public SemanticException
{
public:
    TypeMismatchError(Type expected, Type actual, ASTNodeType context, SourceLocation location)
        : SemanticException(location), expected(expected), actual(actual), context(context) {}

    std::string format() const override
    {
        std::ostringstream oss;
        oss << "Error: Type mismatch at " << location
            << ": expected '" << toString(expected)
            << "' but got '" << toString(actual)
            << "' in " << astNodeTypeToString(context);
        return oss.str();
    }

    Type expected;
    Type actual;
    ASTNodeType context;

private:
    static std::string astNodeTypeToString(ASTNodeType type)
    {
        switch (type)
        {
        case ASTNodeType::RETURN:      return "return statement";
        case ASTNodeType::BINARY_OP:   return "binary operation";
        case ASTNodeType::VAR_DECL:    return "variable declaration";
        case ASTNodeType::FUNCTION_CALL: return "function call";
        default:                       return "expression";
        }
    }
};

class ArgumentTypeMismatchError : public SemanticException
{
public:
    ArgumentTypeMismatchError(Type expected, Type actual, std::string functionName,
                              int parameterIndex, SourceLocation location)
        : SemanticException(location), expected(expected), actual(actual),
          functionName(std::move(functionName)), parameterIndex(parameterIndex) {}

    std::string format() const override
    {
        std::ostringstream oss;
        oss << "Error: Argument type mismatch at " << location
            << " for parameter " << parameterIndex
            << " in function '" << functionName
            << "': expected '" << toString(expected)
            << "' but got '" << toString(actual) << "'";
        return oss.str();
    }

    Type expected;
    Type actual;
    std::string functionName;
    int parameterIndex;
};

class AlreadyDefinedError : public SemanticException
{
public:
    AlreadyDefinedError(std::string identifier, std::string kind,
                        SourceLocation originalLocation, SourceLocation location)
        : SemanticException(location), identifier(std::move(identifier)),
          kind(std::move(kind)), originalLocation(originalLocation) {}

    std::string format() const override
    {
        std::ostringstream oss;
        oss << "Error: " << kind << " '" << identifier
            << "' already defined at " << originalLocation
            << ", redefined at " << location;
        return oss.str();
    }

    std::string identifier;
    std::string kind;
    SourceLocation originalLocation;
};

class UndefinedError : public SemanticException
{
public:
    UndefinedError(std::string identifier, std::string kind, SourceLocation location)
        : SemanticException(location), identifier(std::move(identifier)),
          kind(std::move(kind)) {}

    std::string format() const override
    {
        std::ostringstream oss;
        oss << "Error: " << kind << " '" << identifier
            << "' not defined at " << location;
        return oss.str();
    }

    std::string identifier;
    std::string kind;
};

class WrongArgumentCountError : public SemanticException
{
public:
    WrongArgumentCountError(std::string functionName, size_t expected, size_t actual,
                            SourceLocation location)
        : SemanticException(location), functionName(std::move(functionName)),
          expected(expected), actual(actual) {}

    std::string format() const override
    {
        std::ostringstream oss;
        oss << "Error: Function '" << functionName
            << "' called with incorrect number of arguments at " << location
            << ": expected " << expected << " but got " << actual;
        return oss.str();
    }

    std::string functionName;
    size_t expected;
    size_t actual;
};

class GenericSemanticError : public SemanticException
{
public:
    GenericSemanticError(std::string message, SourceLocation location)
        : SemanticException(location), message(std::move(message)) {}

    std::string format() const override
    {
        std::ostringstream oss;
        oss << "Error: " << message << " at " << location;
        return oss.str();
    }

    std::string message;
};

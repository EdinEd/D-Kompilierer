

#ifndef FUNCTIONSYMBOLTABLE_H
#define FUNCTIONSYMBOLTABLE_H
#include <unordered_map>
#include <memory>
#include <vector>
#include "../common/SourceLocation.hh"
#include "../common/Types.hh"

struct FunctionSignature
{
    std::vector<Type> parameterTypes;
    Type returnType;
    SourceLocation location;
};

class FunctionSymboltable
{
public:
    FunctionSymboltable() = default;
    FunctionSignature *identify(const std::string &identifierName);
    bool insert(const std::string &identifierName, FunctionSignature signature);
    const std::unordered_map<std::string, FunctionSignature> &getTable() const;

private:
    std::unordered_map<std::string, FunctionSignature> table;
};

#endif // FUNCTIONSYMBOLTABLE_H

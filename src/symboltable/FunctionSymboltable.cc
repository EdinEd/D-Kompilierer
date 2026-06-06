#include "FunctionSymboltable.hh"

const FunctionSignature *FunctionSymboltable::identify(const std::string &identifierName) const
{
    auto it = table.find(identifierName);

    if (it != table.end())
    {
        return &it->second;
    }
    else
    {
        return nullptr;
    }
}

bool FunctionSymboltable::insert(const std::string &identifierName, FunctionSignature signature)
{
    if (table.find(identifierName) != table.end())
    {
        return false;
    }
    table[identifierName] = signature;
    return true;
}

const std::unordered_map<std::string, FunctionSignature> &FunctionSymboltable::getTable() const
{
    return table;
}


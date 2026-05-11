//
// Created by Edwin Edich on 03.05.26.
//

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include <unordered_map>
#include <memory>
#include "../common/SourceLocation.hh"
#include "../common/Types.hh"

struct IdentifierInfo
{
    SourceLocation location;
    Type type;
};
//
class Symboltable
{
public:
    Symboltable(std::shared_ptr<Symboltable> parent = nullptr) : parent(parent) {}
    IdentifierInfo *identify(std::string& identifierName);
    std::shared_ptr<Symboltable> getParent();
    bool insert(std::string& identifierName, IdentifierInfo info);
private:
    std::unordered_map<std::string, IdentifierInfo> table;
    std::shared_ptr<Symboltable> parent;
};

#endif // SYMBOLTABLE_H

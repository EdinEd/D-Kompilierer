//
// Created by Edwin Edich on 03.05.26.
//

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include <unordered_map>
#include <memory>
#include <iostream>
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
    IdentifierInfo *identify(std::string &identifierName);
    bool insert(std::string &identifierName, IdentifierInfo info);
    void addChild(std::shared_ptr<Symboltable> child);
    
    const std::unordered_map<std::string, IdentifierInfo>& getTable() const;
    const std::vector<std::shared_ptr<Symboltable>>& getChilds() const;
    std::shared_ptr<Symboltable> getParent();

private:
    std::unordered_map<std::string, IdentifierInfo> table;
    std::vector<std::shared_ptr<Symboltable>> childsVector;
    std::shared_ptr<Symboltable> parent;
};

#endif // SYMBOLTABLE_H

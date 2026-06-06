//
// Created by Edwin Edich on 03.05.26.
//

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include <unordered_map>
#include <memory>
#include <iostream>
#include <vector>
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
    Symboltable(Symboltable *parent = nullptr) : parent(parent) {}
    IdentifierInfo *identify(const std::string &identifierName);
    bool insert(const std::string &identifierName, IdentifierInfo info);
    void addChild(std::unique_ptr<Symboltable> child);

    const std::unordered_map<std::string, IdentifierInfo> &getTable() const;
    const std::vector<std::unique_ptr<Symboltable>> &getChilds() const;
    Symboltable *getParent();

private:
    std::unordered_map<std::string, IdentifierInfo> table;
    std::vector<std::unique_ptr<Symboltable>> childsVector;
    Symboltable *parent;
};

#endif // SYMBOLTABLE_H

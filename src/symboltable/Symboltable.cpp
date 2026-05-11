//
// Created by Edwin Edich on 03.05.26.
//

#include "Symboltable.h"

IdentifierInfo *Symboltable::identify(std::string &identifierName)
{
    auto it = table.find(identifierName);
    if (it != table.end())
    {
        return &it->second;
    }
    else if (parent)
    {
        return parent->identify(identifierName);
    }
    else
    {
        return nullptr;
    }
}

std::shared_ptr<Symboltable> Symboltable::getParent()
{
    return parent;
}

bool Symboltable::insert(std::string &identifierName, IdentifierInfo info)
{
    if (table.find(identifierName) != table.end())
    {
        return false;
    }
    table[identifierName] = info;
    return true;
}

void Symboltable::addChild(std::shared_ptr<Symboltable> child)
{
    childsVector.push_back(child);
}

const std::unordered_map<std::string, IdentifierInfo> &Symboltable::getTable() const
{
    return table;
}

const std::vector<std::shared_ptr<Symboltable>> &Symboltable::getChilds() const
{
    return childsVector;
}
#pragma once
#include "Symboltable.h"

class SymboltablePrinter {
public:
    static void printSymboltable(const std::shared_ptr<Symboltable>& table, int indent = 0);
};
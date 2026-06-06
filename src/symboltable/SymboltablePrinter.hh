#pragma once
#include "Symboltable.hh"
#include <string>

class SymboltablePrinter {
public:
    static void printSymboltable(const Symboltable* table, const std::string& prefix = "");
};

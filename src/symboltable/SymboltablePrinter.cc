#include "SymboltablePrinter.hh"
#include "../common/Types.hh"
#include <algorithm>
#include <iostream>
#include <vector>

void SymboltablePrinter::printSymboltable(const Symboltable* table, const std::string& prefix)
{
    if (!table) return;

    const auto& entries = table->getTable();
    const auto& children = table->getChilds();

    std::vector<std::pair<std::string, IdentifierInfo>> sortedEntries(entries.begin(), entries.end());
    std::sort(sortedEntries.begin(), sortedEntries.end(),
              [](const auto& a, const auto& b) { return a.first < b.first; });

    size_t totalItems = sortedEntries.size() + children.size();
    size_t itemIndex = 0;

    for (const auto& [name, info] : sortedEntries) {
        bool isLast = (itemIndex == totalItems - 1);
        std::cout << prefix << (isLast ? "└── " : "├── ")
                  << name << " : " << toString(info.type)
                  << " (Line: " << info.location.line_begin << ":" << info.location.column_begin
                  << " … " << info.location.line_end << ":" << info.location.column_end << ")"
                  << std::endl;
        itemIndex++;
    }

    for (const auto& child : children) {
        bool isLast = (itemIndex == totalItems - 1);
        std::cout << prefix << (isLast ? "└── " : "├── ") << "[scope]" << std::endl;
        printSymboltable(child.get(), prefix + (isLast ? "    " : "│   "));
        itemIndex++;
    }
}

#include "SymboltablePrinter.hh"

std::string typeToString(Type type)
{
    switch (type)
    {
    case Type::INTEGER:
        return "INTEGER";
    case Type::STRING:
        return "STRING";
    case Type::BOOLEAN:
        return "BOOLEAN";
    case Type::VOID:
        return "VOID";
    default:
        return "UNKNOWN";
    }
}

void printIndent(int indent)
{
    for (int i = 0; i < indent; ++i)
    {
        std::cout << "__";
    }
}

void SymboltablePrinter::printSymboltable(const Symboltable* table, int indent)
{
    if (!table)
        return;
    for (const auto &entry : table->getTable())
    {
        printIndent(indent);
        std::cout << entry.first << " : " << typeToString(entry.second.type) << " (defined at " << entry.second.location << ")" << std::endl;
    }
    for (const auto &child : table->getChilds())
    {
        printIndent(indent);
        std::cout << "Subtable:" << std::endl;
        SymboltablePrinter::printSymboltable(child.get(), indent + 1);
    }
}
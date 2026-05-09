#include <iostream>
#include "Driver.hh"
#include "ASTNode.hh"
#include "ASTTraversal.hh"

void traverseAST(UnitNode *rootNode)
{
    // Hier kannst du die ASTTraversal-Klasse verwenden, um den AST zu durchlaufen
    ASTtraversal traversal;
    std::cout << "==== PRINTING AST ====" << std::endl;
    traversal.visit(rootNode);
}

int main(int argc, char *argv[])
{
    int res = 0;
    Driver drv;
    for (int i = 1; i < argc; ++i)
        if (argv[i] == std::string("-p"))
            drv.trace_parsing = true;
        else if (argv[i] == std::string("-s"))
            drv.trace_scanning = true;
        else if (!drv.parse(argv[i]))
            std::cout << "Fertig" << '\n';
        else
            res = 1;
    if (drv.errors.empty())
    {
        std::cout << "Parsing successful!" << std::endl;
        }
    else
    {
        std::cout << "Parsing completed with errors." << std::endl;
        std::cout << "Parsing failed with " << drv.errors.size() << " error(s)." << std::endl;
        return 1;
    }
    if (drv.ast_root.get() != nullptr)
    {
        std::cout << "AST generated successfully!" << std::endl;
        traverseAST(drv.ast_root.get());
    }
    else
    {
        std::cout << "Failed to generate AST." << std::endl;
    }
    return 0;
}

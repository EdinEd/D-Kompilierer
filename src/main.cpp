#include <iostream>
#include "Driver.hh"
#include "ASTNode.hh"
#include "ASTPrinter.hh"
#include "ASTSConstructor.hh"
#include <memory>

void traverseAST(UnitNode *rootNode)
{
    std::shared_ptr<ASTVisitor> visitorPtr = std::make_shared<ASTSConstructor>();
    std::cout << "==== PRINTING AST ====" << std::endl;
    visitorPtr->visit(rootNode);
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

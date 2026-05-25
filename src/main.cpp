#include <iostream>
#include "Driver.hh"
#include "ASTNode.hh"
#include "ASTPrinter.hh"
#include "ASTSConstructor.hh"
#include "symboltable/SymboltablePrinter.hh"
#include "manual/parser/MParser.hh"
#include <memory>

void printAST(UnitNode *rootNode)
{
    std::cout << "==== PRINTING AST ====" << std::endl;
    ASTPrinter printer;
    printer.visit(rootNode);
}

std::shared_ptr<Symboltable> constructSymbolTable(UnitNode *rootNode)
{
    std::cout << "==== CONSTRUCTING SYMBOLTABLE ====" << std::endl;
    ASTSConstructor visitor;
    visitor.visit(rootNode);
    return visitor.getCurrentSymbolTable();
}

void printSymbolTable(const std::shared_ptr<Symboltable> &symbolTable)
{
    std::cout << "==== PRINTING SYMBOLTABLE ====" << std::endl;
    SymboltablePrinter::printSymboltable(symbolTable.get());
}

void traverseAST(UnitNode *rootNode)
{
    printAST(rootNode);
    auto symbolTable = constructSymbolTable(rootNode);
    printSymbolTable(symbolTable);
}

int main(int argc, char *argv[])
{
    try {
        int res = 0;
        Driver drv;
        Parser parser = Parser();
        bool parsing_success = false;
        bool manual_mode = false;
        for (int i = 1; i < argc; ++i)
        {
            if (argv[i] == std::string("-m"))
            {
                manual_mode = true;
            }
            else if (argv[i] == std::string("-p"))
                drv.trace_parsing = true;
            else if (argv[i] == std::string("-s"))
                drv.trace_scanning = true;
            else
            {
                if (manual_mode)
                {
                    parsing_success = parser.parse(argv[i]);
                }
                else
                {
                    parsing_success = drv.parse(argv[i]);
                }
            }
        }

        if (parsing_success)
        {
            std::cout << "Parsing successful!" << std::endl;
            std::cout << "AST generated successfully!" << std::endl;
            if (manual_mode)
                traverseAST(parser.getAstRoot());
            else
                traverseAST(drv.ast_root.get());
        }
        else
        {
            std::cout << "Parsing completed with errors." << std::endl;
            if (!manual_mode)
                std::cout << "Parsing failed with " << drv.errors.size() << " error(s)." << std::endl;
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred." << std::endl;
        return 1;
    }
    return 0;
}

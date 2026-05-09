#include <iostream>
#include "Driver.hh"

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
    if(drv.ast_root.get() != nullptr) {
        std::cout << "AST generated successfully!" << std::endl;
    } else {
        std::cout << "Failed to generate AST." << std::endl;
    }
    if (drv.errors.empty())
    {
        std::cout << "Parsing successful!" << std::endl;
        return 0;
    }
    else
    {
        std::cout << "Parsing failed with " << drv.errors.size() << " error(s)." << std::endl;
        return 1;
    }
}

#ifndef DRIVER_HH
#define DRIVER_HH
#include <string>
#include <map>
#include <memory>
#include "parser.hh"
#include "../ast/ASTNode.hh"

#define YY_DECL yy::parser::symbol_type yylex(Driver &drv)
YY_DECL;

class Driver
{
public:
    Driver();
    std::unique_ptr<ASTNode> ast_root;
    std::vector<std::string> errors;
    void error(const yy::location& loc, const std::string& msg);

    int parse(const std::string &f);
    std::string file;
    bool trace_parsing;

    // Handling the scanner.
    void scan_begin();
    void scan_end();
    // Whether to generate scanner debug traces.
    bool trace_scanning;
    yy::location location;
};

#endif // ! DRIVER_HH
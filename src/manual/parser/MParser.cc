#include "MParser.hh"
#include "Sets.hh"

void Parser::parse(const std::string &filename)
{
    lexer = Lexer(filename);
    ast_root = root();
}

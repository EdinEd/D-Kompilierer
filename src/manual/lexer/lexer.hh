#pragma once
#include "../../common/Token.hh"

class Lexer {
public:
    Lexer(const std::string &source) : source(source), position(0) {}
    Token getNextToken();  
private:
    std::string source;
    size_t position;
    Token number();
    Token identifier();
    Token string();
};
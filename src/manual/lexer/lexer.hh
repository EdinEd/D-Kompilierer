#pragma once
#include "../../common/Token.hh"

class Lexer
{
public:
    Lexer(const std::string &source) : source(source), position(0) {}
    Token getNextToken()
    {
        return Token(TokenType::FUNK, "funk", SourceLocation(1, 1, 1, 1));
    };

private:
    std::string source;
    size_t position;
    Token number();
    Token identifier();
    Token string();
};
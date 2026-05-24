#pragma once
#include <string>
#include <map>
#include "../../common/Token.hh"

class Lexer
{
public:
    Lexer(const std::string &source);
    Token getNextToken();

private:
    std::string source;
    size_t position;
    int line;
    int column;

    Token number();
    Token identifier();
    Token string();

    char peek() const;
    char get();
    bool isAtEnd() const;
    void skipWhitespaceAndComments();

    static const std::map<std::string, TokenType> keywords;
};
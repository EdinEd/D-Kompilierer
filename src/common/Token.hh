#pragma once
#include <string>
#include "SourceLocation.hh"

enum class TokenType
{
    NUMBER,
    IDENTIFIER,
    STRING,
    PLUS,
    MINUS,
    ASSIGN,
    SEMICOLON,
    LBRACE,
    RBRACE,
    LPAREN,
    RPAREN,
    RETURN,
    TYPE_INT,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_VOID,
    ARROW,
    FUNK,
    END_OF_FILE,
    UNKNOWN
};

class Token
{
public:
    TokenType type;
    std::string lexeme;
    SourceLocation loc;

    Token(TokenType t, std::string lex, SourceLocation l)
        : type(t), lexeme(lex), loc(l) {}

    bool operator==(Token const &other) const
    {
        return this->type == other.type;
    }
};
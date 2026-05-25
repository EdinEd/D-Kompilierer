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

inline const std::string tokenTypeName(const TokenType &type)
{
    switch (type)
    {
    case TokenType::NUMBER:       return "NUMBER";
    case TokenType::IDENTIFIER:   return "IDENTIFIER";
    case TokenType::STRING:       return "STRING";
    case TokenType::PLUS:         return "PLUS";
    case TokenType::MINUS:        return "MINUS";
    case TokenType::ASSIGN:       return "ASSIGN";
    case TokenType::SEMICOLON:    return "SEMICOLON";
    case TokenType::LBRACE:       return "LBRACE";
    case TokenType::RBRACE:       return "RBRACE";
    case TokenType::LPAREN:       return "LPAREN";
    case TokenType::RPAREN:       return "RPAREN";
    case TokenType::RETURN:       return "RETURN";
    case TokenType::TYPE_INT:     return "TYPE_INT";
    case TokenType::TYPE_STRING:  return "TYPE_STRING";
    case TokenType::TYPE_BOOL:    return "TYPE_BOOL";
    case TokenType::TYPE_VOID:    return "TYPE_VOID";
    case TokenType::ARROW:        return "ARROW";
    case TokenType::FUNK:         return "FUNK";
    case TokenType::END_OF_FILE:  return "END_OF_FILE";
    case TokenType::UNKNOWN:      return "UNKNOWN";
    }
    return "???";
}
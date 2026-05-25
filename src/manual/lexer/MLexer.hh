#pragma once
#include <string>
#include <unordered_map>
#include <stdexcept>
#include "../../common/Token.hh"

struct LexerException : public std::runtime_error
{
    SourceLocation loc;
    char invalid_char;

    LexerException(SourceLocation l, char c)
        : std::runtime_error("invalid character: " + std::string(1, c)),
          loc(l), invalid_char(c) {}
};

class Lexer
{
public:
    explicit Lexer(const std::string &source);

    Token getNextToken();

private:
    static const std::unordered_map<std::string, TokenType> keywords;

    std::string source;
    size_t pos;
    int line;
    int column;

    Token readNumber();
    Token readIdentifierOrKeyword();
    Token readString();

    void skipWhitespace();
    void skipLineComment();

    char peek() const;
    char peek(size_t offset) const;
    char advance();
    bool isAtEnd() const;

    bool matches(const std::string &text) const;
    bool isAsciiLetter(char c) const;
    bool isGermanLetter() const;
    bool isLetterStart() const;
    bool isLetterContinuation() const;

    SourceLocation currentLocation() const;
    SourceLocation spanningLocation(const SourceLocation &start) const;
};

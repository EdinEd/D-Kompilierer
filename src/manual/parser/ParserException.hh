#pragma once
#include <stdexcept>
#include <sstream>
#include "../../common/SourceLocation.hh"
#include "../../common/Token.hh"

struct ParserException : public std::runtime_error
{
    SourceLocation loc;
    std::string expected;
    TokenType actual;

    ParserException(SourceLocation l, const std::string &exp, TokenType act)
        : std::runtime_error(formatMessage(l, exp, act)),
          loc(l), expected(exp), actual(act) {}

private:
    static std::string formatMessage(const SourceLocation &l, const std::string &exp, TokenType act)
    {
        std::stringstream ss;
        ss << "Syntax error at " << l << ": expected " << exp
           << ", but got " << tokenTypeName(act);
        return ss.str();
    }
};
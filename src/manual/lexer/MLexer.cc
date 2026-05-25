#include "MLexer.hh"
#include <cctype>

const std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"zurück", TokenType::RETURN},
    {"zahl",   TokenType::TYPE_INT},
    {"faden",  TokenType::TYPE_STRING},
    {"funk",   TokenType::FUNK},
    {"leer",   TokenType::TYPE_VOID}
};

Lexer::Lexer(const std::string &source)
    : source(source), pos(0), line(1), column(1) {}

Token Lexer::getNextToken()
{
    skipWhitespace();
    if (matches("€€"))
        skipLineComment();
    skipWhitespace();

    SourceLocation loc = currentLocation();

    if (isAtEnd())
        return Token(TokenType::END_OF_FILE, "", loc);

    if (isdigit(peek()))
        return readNumber();

    if (isLetterStart())
        return readIdentifierOrKeyword();

    if (peek() == '"')
        return readString();

    advance();

    switch (peek(-1))
    {
    case '(': return Token(TokenType::LPAREN, "(",  loc);
    case ')': return Token(TokenType::RPAREN, ")",  loc);
    case '{': return Token(TokenType::LBRACE, "{",  loc);
    case '}': return Token(TokenType::RBRACE, "}",  loc);
    case ';': return Token(TokenType::SEMICOLON, ";", loc);
    case '=': return Token(TokenType::ASSIGN, "=",  loc);
    case '+': return Token(TokenType::PLUS, "+",    loc);

    case '-':
        if (!isAtEnd() && peek() == '>')
        {
            advance();
            return Token(TokenType::ARROW, "->", spanningLocation(loc));
        }
        return Token(TokenType::MINUS, "-", loc);

    default:
        throw LexerException(loc, peek(-1));
    }
}

Token Lexer::readNumber()
{
    SourceLocation start = currentLocation();
    std::string lexeme;

    while (!isAtEnd() && isdigit(peek()))
        lexeme += advance();

    return Token(TokenType::NUMBER, lexeme, spanningLocation(start));
}

Token Lexer::readIdentifierOrKeyword()
{
    SourceLocation start = currentLocation();
    std::string lexeme;

    while (!isAtEnd() && isLetterContinuation())
    {
        if (isGermanLetter())
        {
            lexeme += advance();
            lexeme += advance();
        }
        else
        {
            lexeme += advance();
        }
    }

    auto it = keywords.find(lexeme);
    TokenType type = (it != keywords.end()) ? it->second : TokenType::IDENTIFIER;

    return Token(type, lexeme, spanningLocation(start));
}

Token Lexer::readString()
{
    SourceLocation start = currentLocation();
    advance();
    std::string content;

    while (!isAtEnd() && peek() != '"')
    {
        if (peek() == '\\')
        {
            content += advance();
            if (!isAtEnd() && peek() != '"')
                content += advance();
        }
        else
        {
            content += advance();
        }
    }

    if (!isAtEnd())
        advance();

    return Token(TokenType::STRING, content, spanningLocation(start));
}

void Lexer::skipWhitespace()
{
    while (!isAtEnd())
    {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
        {
            advance();
            continue;
        }
        break;
    }
}

void Lexer::skipLineComment()
{
    advance();   
    advance();   
    advance();   
    advance();   
    advance();   
    advance();   
    while (!isAtEnd() && peek() != '\n')
        advance();
}

char Lexer::peek()           const { return peek(0); }

char Lexer::peek(size_t offset) const
{
    if (pos + offset >= source.length())
        return '\0';
    return source[pos + offset];
}

char Lexer::advance()
{
    if (isAtEnd())
        return '\0';
    char c = source[pos++];
    if (c == '\n')
    {
        line++;
        column = 1;
    }
    else
    {
        column++;
    }
    return c;
}

bool Lexer::isAtEnd() const
{
    return pos >= source.length();
}

bool Lexer::matches(const std::string &text) const
{
    if (pos + text.length() > source.length())
        return false;
    return source.compare(pos, text.length(), text) == 0;
}

bool Lexer::isAsciiLetter(char c) const
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

bool Lexer::isGermanLetter() const
{
    return matches("Ä") || matches("Ö") || matches("Ü")
        || matches("ä") || matches("ö") || matches("ü")
        || matches("ß");
}

bool Lexer::isLetterStart() const
{
    return isAsciiLetter(peek()) || isGermanLetter();
}

bool Lexer::isLetterContinuation() const
{
    char c = peek();
    return isAsciiLetter(c) || isdigit(c) || c == '_' || isGermanLetter();
}

SourceLocation Lexer::currentLocation() const
{
    return SourceLocation(line, column, line, column);
}

SourceLocation Lexer::spanningLocation(const SourceLocation &start) const
{
    return SourceLocation(start.line_begin, start.column_begin, line, column);
}

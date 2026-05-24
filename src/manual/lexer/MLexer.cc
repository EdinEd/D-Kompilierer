#include "MLexer.hh"
#include <cctype>

const std::map<std::string, TokenType> Lexer::keywords = {
    {"zurück", TokenType::RETURN},
    {"zahl", TokenType::TYPE_INT},
    {"faden", TokenType::TYPE_STRING},
    {"funk", TokenType::FUNK},
    {"leer", TokenType::TYPE_VOID}
};

Lexer::Lexer(const std::string &source)
    : source(source), position(0), line(1), column(1) {}

bool Lexer::isAtEnd() const
{
    return position >= source.length();
}

char Lexer::peek() const
{
    if (isAtEnd())
        return '\0';
    return source[position];
}

char Lexer::get()
{
    if (isAtEnd())
        return '\0';
    char c = source[position++];
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

void Lexer::skipWhitespaceAndComments()
{
    while (!isAtEnd())
    {
        char c = peek();
        if (isspace(c))
        {
            get();
        }
        else if (c == '\xe2' && position + 2 < source.length() && 
                 source[position + 1] == '\x82' && source[position + 2] == '\xac')
        {
            // Check for €€
            if (position + 5 < source.length() &&
                source[position + 3] == '\xe2' && source[position + 4] == '\x82' && source[position + 5] == '\xac')
            {
                for (int i = 0; i < 6; ++i) get();
                while (!isAtEnd() && peek() != '\n')
                {
                    get();
                }
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
}

Token Lexer::getNextToken()
{
    skipWhitespaceAndComments();

    int startLine = line;
    int startCol = column;

    if (isAtEnd())
    {
        return Token(TokenType::END_OF_FILE, "", SourceLocation(startLine, startCol, line, column));
    }

    char c = peek();

    if (isdigit(c))
        return number();
    
    // Check for identifier start: [a-zA-ZÄÖÜäöüß]
    // UTF-8 encoding for German characters:
    // Ä: \xc3\x84, Ö: \xc3\x96, Ü: \xc3\x9c
    // ä: \xc3\xa4, ö: \xc3\xb6, ü: \xc3\xbc, ß: \xc3\x9f
    if (isalpha(c) || 
        (c == '\xc3' && position + 1 < source.length() && 
         (source[position+1] == '\x84' || source[position+1] == '\x96' || source[position+1] == '\x9c' ||
          source[position+1] == '\xa4' || source[position+1] == '\xb6' || source[position+1] == '\xbc' ||
          source[position+1] == '\x9f')))
    {
        return identifier();
    }

    if (c == '"')
        return string();

    get();
    std::string lexeme(1, c);
    TokenType type = TokenType::UNKNOWN;

    switch (c)
    {
    case '(':
        type = TokenType::LPAREN;
        break;
    case ')':
        type = TokenType::RPAREN;
        break;
    case '{':
        type = TokenType::LBRACE;
        break;
    case '}':
        type = TokenType::RBRACE;
        break;
    case ';':
        type = TokenType::SEMICOLON;
        break;
    case '=':
        type = TokenType::ASSIGN;
        break;
    case '+':
        type = TokenType::PLUS;
        break;
    case '-':
        if (peek() == '>')
        {
            get();
            lexeme = "->";
            type = TokenType::ARROW;
        }
        else
        {
            type = TokenType::MINUS;
        }
        break;
    }

    return Token(type, lexeme, SourceLocation(startLine, startCol, line, column));
}

Token Lexer::number()
{
    int startLine = line;
    int startCol = column;
    std::string lexeme;
    while (isdigit(peek()))
    {
        lexeme += get();
    }
    return Token(TokenType::NUMBER, lexeme, SourceLocation(startLine, startCol, line, column));
}

Token Lexer::identifier()
{
    int startLine = line;
    int startCol = column;
    std::string lexeme;

    while (!isAtEnd())
    {
        char c = peek();
        if (isalnum(c) || c == '_' || (unsigned char)c > 127)
        {
            // for simplicity, all non-ASCII are consumed as part of identifier if it started correctly
            lexeme += get();
        }
        else
        {
            break;
        }
    }

    if (keywords.count(lexeme))
    {
        return Token(keywords.at(lexeme), lexeme, SourceLocation(startLine, startCol, line, column));
    }
    return Token(TokenType::IDENTIFIER, lexeme, SourceLocation(startLine, startCol, line, column));
}

Token Lexer::string()
{
    int startLine = line;
    int startCol = column;
    get(); // skip opening "
    std::string lexeme;
    while (!isAtEnd() && peek() != '"')
    {
        if (peek() == '\\')
        {
            lexeme += get(); // keep backslash
            if (!isAtEnd())
                lexeme += get(); // keep escaped char
        }
        else
        {
            lexeme += get();
        }
    }
    if (!isAtEnd())
        get(); // skip closing "

    return Token(TokenType::STRING, lexeme, SourceLocation(startLine, startCol, line, column));
}

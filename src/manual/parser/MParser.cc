#include "MParser.hh"
#include "Sets.hh"
#include <sstream>

Parser::Parser()
    : lexer(Lexer("")), ast_root(nullptr), current_token(Token(TokenType::END_OF_FILE, "", SourceLocation()))
{
}

void Parser::parse(const std::string &filename)
{
    lexer = Lexer(filename);
    this->advance();
    ast_root = std::move(parse_root());
}

TokenType Parser::peek()
{
    return current_token.type;
}

bool Parser::match(const TokenType &expected)
{
    if (current_token.type == expected)
    {
        advance();
        return true;
    }
    return false;
}

Token Parser::consume(const TokenType &expected, const std::string error_msg)
{
    if (current_token.type == expected)
    {
        Token consumed_token = current_token;
        advance();
        return consumed_token;
    }
    std::stringstream msg;
    msg << "Syntax error at position " << current_token.loc << ": " << error_msg;
    throw std::runtime_error(msg.str());
}

void Parser::advance()
{
    this->current_token = lexer.getNextToken();
}

std::unique_ptr<UnitNode> Parser::parse_root()
{
    if (sets::first_functions(current_token) || current_token.type == TokenType::END_OF_FILE)
    {
        auto root = std::make_unique<UnitNode>(current_token.loc);
        parse_functions(ast_root.get());
        consume(TokenType::END_OF_FILE, "Expected end of file");
        return root;
    }
    throw std::runtime_error("Syntax error: File should begin with function or be empty");
}

void Parser::parse_functions(UnitNode *parent)
{
    if (sets::follow_functions(current_token))
    {
        return;
    }
    else if (sets::first_function(current_token))
    {
        auto function = parse_function();
        parent->addFunction(std::move(function));
        parse_functions(parent);
    }
    throw std::runtime_error("Syntax error: File should begin with function or be empty");
}

std::unique_ptr<FunctionDefNode> Parser::parse_function()
{
    auto function = std::make_unique<FunctionDefNode>(current_token.loc);
    if (sets::first_function(current_token))
    {
        consume(TokenType::FUNK, "Expected Keyword: FUNK");
        consume(TokenType::IDENTIFIER, "Expected an identifier");
        consume(TokenType::LPAREN, "Expected an LPAREN");
        consume(TokenType::RPAREN, "Expected RPAREN");
        parse_function2(function.get());
    }
    return function;
}

void Parser::parse_function2(FunctionDefNode *parent)
{
    if (peek() == TokenType::ARROW)
    {
        consume(TokenType::ARROW, "Expected Keyword: ARROW (->)");
        parent->return_type = parse_type();
        // parse_body();
    }
    else if (sets::first_body(current_token))
    {
        // parse_body();
    }

    throw std::runtime_error("Expected -> or Body definition");
}

Type Parser::parse_type()
{
    switch (peek())
    {
    case TokenType::TYPE_INT:
        consume(TokenType::TYPE_INT, "Expected type: int");
        return Type::INTEGER;
    case TokenType::TYPE_STRING:
        consume(TokenType::TYPE_STRING, "Expected type: string");
        return Type::STRING;
    case TokenType::TYPE_BOOL:
        consume(TokenType::TYPE_BOOL, "Expected type: bool");
        return Type::BOOLEAN;
    case TokenType::TYPE_VOID:
        consume(TokenType::TYPE_VOID, "Expected type: void");
        return Type::VOID;
    default:
        throw std::runtime_error("Unexpected type");
    }
}

#include "MParser.hh"
#include "Sets.hh"
#include <sstream>

void Parser::parse(const std::string &filename)
{
    lexer = Lexer(filename);
    this->advance();
    ast_root = std::move(parse_root());
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
        this->ast_root = std::make_unique<UnitNode>(current_token.loc);
        auto root = std::move(parse_functions());
        consume(TokenType::END_OF_FILE, "Expected end of file");
        return root;
    }
    throw std::runtime_error("Syntax error: File should begin with function or be empty");
}

std::unique_ptr<UnitNode> Parser::parse_functions()
{
    if (sets::follow_functions(current_token))
    {
        return nullptr;
    }
    else if (sets::first_function(current_token))
    {
        auto function = parse_function();
        ast_root->addFunction(std::move(function));
        parse_functions();
    }
    // hier fehler, wenn gar nicht matched
    return std::make_unique<UnitNode>(current_token.loc); // TODO: Durch richtige return ersetzen
}

std::unique_ptr<FunctionDefNode> Parser::parse_function()
{
    std::unique_ptr<FunctionDefNode> function;
    if (sets::first_function(current_token))
    {
        consume(TokenType::FUNK, "Expected Keyword: FUNK");
        consume(TokenType::IDENTIFIER, "Expected an identifier");
        consume(TokenType::LPAREN, "Expected an LPAREN");
        consume(TokenType::RPAREN, "Expected RPAREN");
        //auto function = parse_function2();
    }
    return function;
}

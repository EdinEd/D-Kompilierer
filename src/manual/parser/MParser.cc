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
        auto identifier_token = consume(TokenType::IDENTIFIER, "Expected an identifier");
        function->name = identifier_token.lexeme;
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
        parent->body = parse_body();
        return;
    }
    else if (sets::first_body(current_token))
    {
        parent->return_type = Type::VOID;
        parent->body = parse_body();
        return;
    }

    throw std::runtime_error("Expected -> or Body definition");
}

std::unique_ptr<BlockNode> Parser::parse_body()
{

    if (sets::first_body(current_token))
    {
        auto body = std::make_unique<BlockNode>(current_token.loc);
        consume(TokenType::LBRACE, "Expected {");
        parse_contents(body.get());
        consume(TokenType::RBRACE, "Expected }");
        return body;
    }
    throw std::runtime_error("Expected Body definition");
}

void Parser::parse_contents(BlockNode *parent)
{
    if (sets::follow_contents(current_token))
    {
        return;
    }
    else if (sets::first_content_item(current_token))
    {
        parent->addStatement(std::move(parse_content_item()));
        parse_contents(parent);
        return;
    }
    throw std::runtime_error("Expected empty or content_item");
}

std::unique_ptr<ASTNode> Parser::parse_content_item()
{
    if (sets::first_return_statement(current_token))
    {
        return parse_return_statement();
    }
    else if (sets::first_var_decl(current_token))
    {
        return parse_var_decl();
    }
    else if (sets::first_body(current_token))
    {
        return parse_body();
    }
    else if (sets::first_identifier_call(current_token))
    {
        return parse_identifier_call();
    }
    throw std::runtime_error("Unexpected content");
}

std::unique_ptr<ReturnNode> Parser::parse_return_statement(){
    if(sets::first_return_statement(current_token)){
        consume(TokenType::RETURN, "Expected return");
        return parse_return_statement2();
    }
    throw std::runtime_error("Unexpected return");
}

std::unique_ptr<ReturnNode> Parser::parse_return_statement2(){
    auto return_node = std::make_unique<ReturnNode>(current_token.loc);
    if(peek() == TokenType::NUMBER){
        auto number_token = consume(TokenType::NUMBER, "Expected return");
        return_node->expression = std::make_unique<NumberNode>(number_token.loc, std::stoi(number_token.lexeme));
        return return_node;
    }
    else if(peek() == TokenType::STRING){
        auto string_token = consume(TokenType::STRING, "Expected string");
        return_node->expression = std::make_unique<StringNode>(string_token.loc, string_token.lexeme);
        return return_node;
    }
    else if(peek() == TokenType::IDENTIFIER){
        auto identifier_token = consume(TokenType::IDENTIFIER, "Expected identifier");
        return_node->expression = std::make_unique<VarRefNode>(identifier_token.loc, identifier_token.lexeme);
        return return_node;
    }
    throw std::runtime_error("Unexpected return");
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

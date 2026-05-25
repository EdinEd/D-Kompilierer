#include "MParser.hh"
#include "Sets.hh"
#include <fstream>
#include "ParserException.hh"

Parser::Parser()
    : lexer(Lexer("")), ast_root(nullptr), current_token(Token(TokenType::END_OF_FILE, "", SourceLocation()))
{
}

bool Parser::parse(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Could not open file: " << filename << std::endl;
        return false;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    lexer = Lexer(buffer.str());
    this->advance();
    ast_root = std::move(parse_root());
    return true;
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
    throw ParserException(current_token.loc, error_msg, current_token.type);
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
        parse_functions(root.get());
        consume(TokenType::END_OF_FILE, "Expected end of file");
        return root;
    }
    throw ParserException(current_token.loc, "function or empty file", current_token.type);
}

UnitNode *Parser::getAstRoot() const
{
    return ast_root.get();
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
        return;
    }
    throw ParserException(current_token.loc, "function or empty file", current_token.type);
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

    throw ParserException(current_token.loc, "-> or body definition", current_token.type);
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
    throw ParserException(current_token.loc, "body definition", current_token.type);
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
    throw ParserException(current_token.loc, "empty or content_item", current_token.type);
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
    throw ParserException(current_token.loc, "content item", current_token.type);
}

std::unique_ptr<ReturnNode> Parser::parse_return_statement()
{
    if (sets::first_return_statement(current_token))
    {
        consume(TokenType::RETURN, "Expected return");
        return parse_return_statement2();
    }
    throw ParserException(current_token.loc, "return statement", current_token.type);
}

std::unique_ptr<ReturnNode> Parser::parse_return_statement2()
{
    auto return_node = std::make_unique<ReturnNode>(current_token.loc);
    if (peek() == TokenType::NUMBER)
    {
        auto number_token = consume(TokenType::NUMBER, "Expected return");
        consume(TokenType::SEMICOLON, "Expected semicolon");
        return_node->expression = std::make_unique<NumberNode>(number_token.loc, std::stoi(number_token.lexeme));
        return return_node;
    }
    else if (peek() == TokenType::STRING)
    {
        auto string_token = consume(TokenType::STRING, "Expected string");
        consume(TokenType::SEMICOLON, "Expected semicolon");
        return_node->expression = std::make_unique<StringNode>(string_token.loc, string_token.lexeme);
        return return_node;
    }
    else if (peek() == TokenType::IDENTIFIER)
    {
        auto identifier_token = consume(TokenType::IDENTIFIER, "Expected identifier");
        consume(TokenType::SEMICOLON, "Expected semicolon");
        return_node->expression = std::make_unique<VarRefNode>(identifier_token.loc, identifier_token.lexeme);
        return return_node;
    }
    throw ParserException(current_token.loc, "return statement", current_token.type);
}

std::unique_ptr<VarDeclNode> Parser::parse_var_decl()
{
    auto var_decl_node = std::make_unique<VarDeclNode>(current_token.loc);
    if (peek() == TokenType::TYPE_INT)
    {
        consume(TokenType::TYPE_INT, "");
        var_decl_node->type = Type::INTEGER;
        auto identifier_token = consume(TokenType::IDENTIFIER, "Expected identifier");
        var_decl_node->name = identifier_token.lexeme;
        parse_var_decl_int(var_decl_node.get());
        return var_decl_node;
    }
    else if (peek() == TokenType::TYPE_STRING)
    {
        consume(TokenType::TYPE_STRING, "");
        var_decl_node->type = Type::STRING;
        auto identifier_token = consume(TokenType::IDENTIFIER, "Expected identifier");
        var_decl_node->name = identifier_token.lexeme;
        parse_var_decl_string(var_decl_node.get());
        return var_decl_node;
    }
    throw ParserException(current_token.loc, "variable declaration", current_token.type);
}

void Parser::parse_var_decl_int(VarDeclNode *parent)
{
    if (peek() == TokenType::SEMICOLON)
    {
        consume(TokenType::SEMICOLON, "Expected ;");
        return;
    }
    if (peek() == TokenType::ASSIGN)
    {
        consume(TokenType::ASSIGN, "Expected =");
        auto number_token = consume(TokenType::NUMBER, "Expected number");
        parent->init_value = std::make_unique<NumberNode>(number_token.loc, std::stoi(number_token.lexeme));
        consume(TokenType::SEMICOLON, "Expected ;");
        return;
    }
    throw ParserException(current_token.loc, "; or assignment in int declaration", current_token.type);
}

void Parser::parse_var_decl_string(VarDeclNode *parent)
{
    if (peek() == TokenType::SEMICOLON)
    {
        consume(TokenType::SEMICOLON, "Expected ;");
        return;
    }
    if (peek() == TokenType::ASSIGN)
    {
        consume(TokenType::ASSIGN, "Expected =");
        auto string_token = consume(TokenType::STRING, "Expected string");
        parent->init_value = std::make_unique<StringNode>(string_token.loc, string_token.lexeme);
        consume(TokenType::SEMICOLON, "Expected ;");
        return;
    }
    throw ParserException(current_token.loc, "; or assignment in string declaration", current_token.type);
}

std::unique_ptr<ASTNode> Parser::parse_identifier_call()
{
    if (sets::first_identifier_call(current_token))
    {
        auto identifier_token = consume(TokenType::IDENTIFIER, "Expected identifier");

        return parse_identifier_call2(identifier_token);
    }
    throw ParserException(current_token.loc, "identifier", current_token.type);
}

std::unique_ptr<ASTNode> Parser::parse_identifier_call2(const Token &identifier_token)
{
    if (sets::first_function_call(current_token))
    {
        return parse_function_call(identifier_token);
    }
    else if (sets::first_var_assign(current_token))
    {
        return parse_var_assign(identifier_token);
    }

    throw ParserException(current_token.loc, "'(' for function call or '=' for variable assignment", current_token.type);
}

std::unique_ptr<ASTNode> Parser::parse_function_call(const Token &identifier_token)
{
    if (sets::first_function_call(current_token))
    {
        consume(TokenType::LPAREN, "Expected '('");
        consume(TokenType::RPAREN, "Expected ')'");
        consume(TokenType::SEMICOLON, "Expected ';'");
        // TODO: Anstatt VarRefNode -> FuncRefNode
        return std::make_unique<VarRefNode>(identifier_token.loc, identifier_token.lexeme);
    }
    throw ParserException(current_token.loc, "function call", current_token.type);
}

std::unique_ptr<BinaryOpNode> Parser::parse_var_assign(const Token &identifier_token)
{

    if (sets::first_var_assign(current_token))
    {
        auto assign_token = consume(TokenType::ASSIGN, "Expected =");
        auto assign_right_node = parse_var_assign2();

        auto assign_left_node = std::make_unique<VarRefNode>(identifier_token.loc, identifier_token.lexeme);
        auto assign_node = std::make_unique<BinaryOpNode>(assign_token.loc, OperationType::ASSIGN, std::move(assign_left_node), std::move(assign_right_node));
        return assign_node;
    }
    throw ParserException(current_token.loc, "'=' for variable assignment", current_token.type);
}

std::unique_ptr<ASTNode> Parser::parse_var_assign2()
{
    if (peek() == TokenType::NUMBER)
    {
        auto number_token = consume(TokenType::NUMBER, "Expected number");
        consume(TokenType::SEMICOLON, "Expected ';'");
        return std::make_unique<NumberNode>(number_token.loc, std::stoi(number_token.lexeme));
    }
    if (peek() == TokenType::STRING)
    {
        auto string_token = consume(TokenType::STRING, "Expected string");
        consume(TokenType::SEMICOLON, "Expected ';'");
        return std::make_unique<StringNode>(string_token.loc, string_token.lexeme);
    }
    if (peek() == TokenType::IDENTIFIER)
    {
        auto identifier_token = consume(TokenType::IDENTIFIER, "Expected identifier");
        consume(TokenType::SEMICOLON, "Expected ';'");
        return std::make_unique<VarRefNode>(identifier_token.loc, identifier_token.lexeme);
    }
    throw ParserException(current_token.loc, "initializer expression", current_token.type);
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
        throw ParserException(current_token.loc, "type", current_token.type);
    }
}

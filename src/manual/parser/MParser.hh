#pragma once
#include <string>
#include "../lexer/lexer.hh"
#include "../../ast/ASTNode.hh"

class Parser
{
public:
    Parser();
    void parse(const std::string &filename);
    bool match(const TokenType &expected);
    Token consume(const TokenType &expected, const std::string error_msg);
    void advance();
    TokenType peek();

private:
    Lexer lexer;
    std::unique_ptr<UnitNode> ast_root;
    Token current_token;

    std::unique_ptr<UnitNode> parse_root();
    void parse_functions(UnitNode *parent);
    std::unique_ptr<FunctionDefNode> parse_function();
    void parse_function2(FunctionDefNode *parent);
    std::unique_ptr<BlockNode> parse_body();
    Type parse_type();
    std::unique_ptr<VarDeclNode> parse_var_decl();
    void parse_var_decl_int(VarDeclNode *parent);
    void parse_var_decl_string(VarDeclNode *parent);
    std::unique_ptr<BinaryOpNode> parse_var_assign(const Token &identifier_token);
    std::unique_ptr<ASTNode> parse_var_assign2();
    void parse_contents(BlockNode *parent);
    std::unique_ptr<ASTNode> parse_content_item();
    std::unique_ptr<ASTNode> parse_identifier_call();
    std::unique_ptr<ASTNode> parse_identifier_call2(const Token &identifier_token);
    std::unique_ptr<ReturnNode> parse_return_statement();
    std::unique_ptr<ReturnNode> parse_return_statement2();
    std::unique_ptr<ASTNode> parse_function_call(const Token &identifier_token);
};
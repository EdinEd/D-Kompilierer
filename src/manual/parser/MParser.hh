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

private:
    Lexer lexer;
    std::unique_ptr<UnitNode> ast_root;
    Token current_token;

    std::unique_ptr<UnitNode> parse_root();
    std::unique_ptr<UnitNode> parse_functions();
    std::unique_ptr<FunctionDefNode> parse_function();
    std::unique_ptr<FunctionDefNode> parse_function2();
    std::unique_ptr<ASTNode> parse_body();
    std::unique_ptr<ASTNode> parse_type();
    std::unique_ptr<ASTNode> parse_var_decl();
    std::unique_ptr<ASTNode> parse_var_decl_int();
    std::unique_ptr<ASTNode> parse_var_decl_string();
    std::unique_ptr<ASTNode> parse_var_assign();
    std::unique_ptr<ASTNode> parse_var_assign2();
    std::unique_ptr<ASTNode> parse_contents();
    std::unique_ptr<ASTNode> parse_content_item();
    std::unique_ptr<ASTNode> parse_identifier_call();
    std::unique_ptr<ASTNode> parse_identifiert_call2();
    std::unique_ptr<ASTNode> parse_return_statement();
    std::unique_ptr<ASTNode> parse_return_statement2();
    std::unique_ptr<ASTNode> parse_function_call();
};
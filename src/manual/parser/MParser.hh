#pragma once
#include <string>
#include "../lexer/lexer.hh"
#include "../../ast/ASTNode.hh"

class Parser
{
public:
    Parser() = default;
    void parse(const std::string &filename);

private:
    Lexer lexer;
    ASTNode *ast_root;

    ASTNode *root();
    ASTNode *functions();
    ASTNode *function();
    ASTNode *function2();
    ASTNode *body();
    ASTNode *type();
    ASTNode *var_decl();
    ASTNode *var_decl_int();
    ASTNode *var_decl_string();
    ASTNode *var_assign();
    ASTNode *var_assign2();
    ASTNode *contents();
    ASTNode *content_item();
    ASTNode *identifier_call();
    ASTNode *identifiert_call2();
    ASTNode *return_statement();
    ASTNode *return_statement2();
    ASTNode *function_call();
};
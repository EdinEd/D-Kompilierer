#pragma once
#include "../../common/Token.hh"

namespace Sets
{
    // --- FIRST SETS ---
    static bool first_functions(const Token &token);
    static bool first_function(const Token &token);
    static bool first_function_prime(const Token &token);
    static bool first_type(const Token &token);
    static bool first_var_decl(const Token &token);
    static bool first_var_decl_int(const Token &token);
    static bool first_var_decl_string(const Token &token);
    static bool first_var_assign(const Token &token);
    static bool first_var_assign_prime(const Token &token);
    static bool first_body(const Token &token);
    static bool first_contents(const Token &token);
    static bool first_content_item(const Token &token);
    static bool first_identifier_call(const Token &token);
    static bool first_identifier_call_prime(const Token &token);
    static bool first_return_statement(const Token &token);
    static bool first_return_statement_prime(const Token &token);
    static bool first_function_call(const Token &token);

    // --- FOLLOW SETS ---
    static bool follow_functions(const Token &token);
    static bool follow_function(const Token &token);
    static bool follow_function_prime(const Token &token);
    static bool follow_type(const Token &token);
    static bool follow_var_decl(const Token &token);
    static bool follow_var_decl_int(const Token &token);
    static bool follow_var_decl_string(const Token &token);
    static bool follow_var_assign(const Token &token);
    static bool follow_var_assign_prime(const Token &token);
    static bool follow_body(const Token &token);
    static bool follow_contents(const Token &token);
    static bool follow_content_item(const Token &token);
    static bool follow_identifier_call(const Token &token);
    static bool follow_identifier_call_prime(const Token &token);
    static bool follow_return_statement(const Token &token);
    static bool follow_return_statement_prime(const Token &token);
    static bool follow_function_call(const Token &token);
}
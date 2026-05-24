#pragma once
#include "../../common/Token.hh"

namespace sets
{
    // ==========================================
    //                 FIRST SETS
    // ==========================================

    inline bool first_functions(const Token &token)
    {
        switch (token.type)
        {
        case TokenType::FUNK:
            return true;
        default:
            return false; // Kann auch Epsilon sein
        }
    }

    inline bool first_function(const Token &token)
    {
        return token.type == TokenType::FUNK;
    }

    inline bool first_function_prime(const Token &token)
    {
        switch (token.type)
        {
        case TokenType::ARROW:
        case TokenType::LBRACE:
            return true;
        default:
            return false;
        }
    }

    inline bool first_type(const Token &token)
    {
        switch (token.type)
        {
        case TokenType::TYPE_INT:
        case TokenType::TYPE_VOID:
        case TokenType::TYPE_STRING:
            return true;
        default:
            return false;
        }
    }

    inline bool first_var_decl(const Token &token)
    {
        switch (token.type)
        {
        case TokenType::TYPE_INT:
        case TokenType::TYPE_STRING:
            return true;
        default:
            return false;
        }
    }

    inline bool first_var_decl_int(const Token &token)
    {
        switch (token.type)
        {
        case TokenType::SEMICOLON:
        case TokenType::ASSIGN:
            return true;
        default:
            return false;
        }
    }

    inline bool first_var_decl_string(const Token &token)
    {
        switch (token.type)
        {
        case TokenType::SEMICOLON:
        case TokenType::ASSIGN:
            return true;
        default:
            return false;
        }
    }

    inline bool first_var_assign(const Token &token)
    {
        return token.type == TokenType::ASSIGN;
    }

    inline bool first_var_assign_prime(const Token &token)
    {
        switch (token.type)
        {
        case TokenType::NUMBER:
        case TokenType::STRING:
        case TokenType::IDENTIFIER:
            return true;
        default:
            return false;
        }
    }

    inline bool first_body(const Token &token)
    {
        return token.type == TokenType::LBRACE;
    }

    inline bool first_contents(const Token &token)
    {
        switch (token.type)
        {
        case TokenType::RETURN:
        case TokenType::TYPE_INT:
        case TokenType::TYPE_STRING:
        case TokenType::LBRACE:
        case TokenType::IDENTIFIER:
            return true;
        default:
            return false; // Kann auch Epsilon sein
        }
    }

    inline bool first_content_item(const Token &token)
    {
        switch (token.type)
        {
        case TokenType::RETURN:
        case TokenType::TYPE_INT:
        case TokenType::TYPE_STRING:
        case TokenType::LBRACE:
        case TokenType::IDENTIFIER:
            return true;
        default:
            return false;
        }
    }

    inline bool first_identifier_call(const Token &token)
    {
        return token.type == TokenType::IDENTIFIER;
    }

    inline bool first_identifier_call_prime(const Token &token)
    {
        switch (token.type)
        {
        case TokenType::LPAREN:
        case TokenType::ASSIGN:
            return true;
        default:
            return false;
        }
    }

    inline bool first_return_statement(const Token &token)
    {
        return token.type == TokenType::RETURN;
    }

    inline bool first_return_statement_prime(const Token &token)
    {
        switch (token.type)
        {
        case TokenType::NUMBER:
        case TokenType::STRING:
        case TokenType::IDENTIFIER:
            return true;
        default:
            return false;
        }
    }

    inline bool first_function_call(const Token &token)
    {
        return token.type == TokenType::LPAREN;
    }

    // ==========================================
    //                 FOLLOW SETS
    // ==========================================

    inline bool follow_functions(const Token &token)
    {
        return token.type == TokenType::END_OF_FILE;
    }

    inline bool follow_function(const Token &token)
    {
        switch (token.type)
        {
        case TokenType::FUNK:
        case TokenType::END_OF_FILE:
            return true;
        default:
            return false;
        }
    }

    inline bool follow_function_prime(const Token &token)
    {
        switch (token.type)
        {
        case TokenType::FUNK:
        case TokenType::END_OF_FILE:
            return true;
        default:
            return false;
        }
    }

    inline bool follow_type(const Token &token)
    {
        return token.type == TokenType::LBRACE;
    }

    // Hilfskonstrukt für die wiederkehrende Statement-Menge im Block
    // { RETURN, TYPE_INT, TYPE_STRING, LBRACE, IDENTIFIER, RBRACE }
    inline bool is_statement_follow(const Token &token)
    {
        switch (token.type)
        {
        case TokenType::RETURN:
        case TokenType::TYPE_INT:
        case TokenType::TYPE_STRING:
        case TokenType::LBRACE:
        case TokenType::IDENTIFIER:
        case TokenType::RBRACE:
            return true;
        default:
            return false;
        }
    }

    inline bool follow_var_decl(const Token &token)
    {
        return is_statement_follow(token);
    }

    inline bool follow_var_decl_int(const Token &token)
    {
        return is_statement_follow(token);
    }

    inline bool follow_var_decl_string(const Token &token)
    {
        return is_statement_follow(token);
    }

    inline bool follow_var_assign(const Token &token)
    {
        return is_statement_follow(token);
    }

    inline bool follow_var_assign_prime(const Token &token)
    {
        return is_statement_follow(token);
    }

    inline bool follow_body(const Token &token)
    {
        switch (token.type)
        {
        case TokenType::FUNK:
        case TokenType::END_OF_FILE:
        case TokenType::RETURN:
        case TokenType::TYPE_INT:
        case TokenType::TYPE_STRING:
        case TokenType::LBRACE:
        case TokenType::IDENTIFIER:
        case TokenType::RBRACE:
            return true;
        default:
            return false;
        }
    }

    inline bool follow_contents(const Token &token)
    {
        return token.type == TokenType::RBRACE;
    }

    inline bool follow_content_item(const Token &token)
    {
        return is_statement_follow(token);
    }

    inline bool follow_identifier_call(const Token &token)
    {
        return is_statement_follow(token);
    }

    inline bool follow_identifier_call_prime(const Token &token)
    {
        return is_statement_follow(token);
    }

    inline bool follow_return_statement(const Token &token)
    {
        return is_statement_follow(token);
    }

    inline bool follow_return_statement_prime(const Token &token)
    {
        return is_statement_follow(token);
    }

    inline bool follow_function_call(const Token &token)
    {
        return is_statement_follow(token);
    }
}
%skeleton "lalr1.cc" /* C++ Skeleton */
%require "3.8.1"
%header

%define api.token.raw
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  # include <string>
  class Driver;
}

// Der Parser erhält eine Referenz auf den "Driver" (Kontext)
%param { Driver& drv }
%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code {
  #include "Driver.hh"
}

%define api.token.prefix {TOK_}
%token
  EURO   "€"
  LPAREN  "("
  RPAREN  ")"
  RETURN "zurück"
  TYPE_INT "int"
  ARROW  "->"
  LBRACE  "{"
  RBRACE  "}"
  SEMICOLON ";"
  FUNK "funk"
  TYPE_VOID "leer"
;

%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"
//%nterm <int> body functions function content_item contents;

%printer { yyo << $$; } <*>;

%%
%start unit;
unit: 
    functions  { 
        
    };

functions:
  %empty                 {}
| functions function     {};

function:
  FUNK IDENTIFIER LPAREN RPAREN ARROW type body{ };
  | FUNK IDENTIFIER LPAREN RPAREN body { }
  | error RPAREN{ yyerrok; };

type:
  TYPE_INT { 
  }
  | TYPE_VOID { 
  };

body:
  LBRACE contents RBRACE {  };

contents:
  %empty                 {  }
| contents content_item  {};

content_item:
  RETURN NUMBER SEMICOLON {std::cout << $2 << std::endl;}
| body                    {}
| error SEMICOLON         {yyerrok;}
%%

void yy::parser::error (const location_type& l, const std::string& m)
{
  drv.error(l, m);
}
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
%nterm <int> body functions function content_item contents bodys;

%printer { yyo << $$; } <*>;

%%
%start unit;
unit: 
    functions  { 
        drv.result = $1; 
    };

functions:
  %empty                 {}
| functions function     {$$ = $2;};

function:
  FUNK IDENTIFIER LPAREN RPAREN ARROW type body{ $$ = $7;};
  | FUNK IDENTIFIER LPAREN RPAREN body { $$ = $5;};

type:
  TYPE_INT { 
  }
  | TYPE_VOID { 
  };

body:
  LBRACE contents RBRACE { $$ = $2; };

contents:
  %empty                 { $$ = 0; }
| contents content_item  { $$ = $2; };

content_item:
  RETURN NUMBER SEMICOLON { $$ = $2; }
| body                    { $$ = $1; };
%%

void yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
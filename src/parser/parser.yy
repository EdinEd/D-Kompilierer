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
;

%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"
%nterm <int> body functions function;

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
  FUNK IDENTIFIER LPAREN RPAREN ARROW type LBRACE body RBRACE { $$ = $8;
  };

type:
  TYPE_INT { 
  };

body:
  %empty        { $$ = 0; }
| RETURN NUMBER SEMICOLON { 
      $$ = $2; // Reicht die Zahl an 'unit' weiter
  };
%%

void yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
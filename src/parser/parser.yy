%skeleton "lalr1.cc" /* C++ Skeleton */
%require "3.8.1"
%header

%define api.token.raw
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  # include <string>
  #include <vector>
  #include <memory>
  class Driver;
  class ASTNode;
  class BlockNode;
  class FunctionDefNode;
  class UnitNode;
  enum class Type;
}

// Der Parser erhält eine Referenz auf den "Driver" (Kontext)
%param { Driver& drv }
%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code {
  #include "Driver.hh"
  #include "../ast/SourceLocation.hh"
  #include "../ast/ASTNode.hh"

  inline SourceLocation toASTLoc(const yy::location& l) {
      return SourceLocation(
          l.begin.line, 
          l.begin.column, 
          l.end.line, 
          l.end.column
      );
  }
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
%nterm <std::unique_ptr<UnitNode>> unit;
%nterm <std::unique_ptr<BlockNode>> body contents;
%nterm <std::unique_ptr<UnitNode>> functions;
%nterm <std::unique_ptr<FunctionDefNode>> function;
%nterm <std::unique_ptr<ASTNode>> content_item;
%nterm <Type> type;

%printer { yyo << $$; } <int> <std::string>;
%printer { yyo << "[AST Node]"; } <std::unique_ptr<ASTNode>> <std::unique_ptr<BlockNode>> <std::unique_ptr<UnitNode>> <std::unique_ptr<FunctionDefNode>> <Type>; 
%printer { yyo << "[Function List]"; } <std::vector<std::unique_ptr<FunctionDefNode>>>;

%%
%start unit;
unit: 
    functions  { 
        drv.ast_root = std::move($1);
    };

functions:
  %empty                 {$$ = std::make_unique<UnitNode>(toASTLoc(@$));}
| functions function     {$1->addFunction(std::move($2)); $$ = std::move($1);};

function:
  FUNK IDENTIFIER LPAREN RPAREN ARROW type body{ $$ = std::make_unique<FunctionDefNode>(toASTLoc(@$), $6, $2, std::move($7)); }
  | FUNK IDENTIFIER LPAREN RPAREN body { $$ = std::make_unique<FunctionDefNode>(toASTLoc(@$), Type::VOID, $2, std::move($5)); }
  | error RPAREN{ yyerrok; };

type:
  TYPE_INT { $$ = Type::INTEGER; }
  | TYPE_VOID { $$ = Type::VOID; };

body:
  LBRACE contents RBRACE { $$ = std::move($2); };

contents:
  %empty                 { $$ = std::make_unique<BlockNode>(toASTLoc(@$)); }
| contents content_item  {$1->addStatement(std::move($2)); $$ = std::move($1);};

content_item:
  RETURN NUMBER SEMICOLON { $$ = std::make_unique<ReturnNode>(toASTLoc(@$), std::make_unique<ASTNode>(toASTLoc(@2), ASTNodeType::NUMBER)); }
| body                    {}
| error SEMICOLON         {yyerrok;}
%%

void yy::parser::error (const location_type& l, const std::string& m)
{
  drv.error(l, m);
}
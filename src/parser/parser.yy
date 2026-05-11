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
  class VarDeclNode;
  class ReturnNode;
  class NumberNode;
  class StringNode;
  class BinaryOpNode;
  enum class Type;
  enum class OperationType;
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
  TYPE_INT "zahl"
  TYPE_STRING "faden"
  ARROW  "->"
  LBRACE  "{"
  RBRACE  "}"
  SEMICOLON ";"
  FUNK "funk"
  TYPE_VOID "leer"
  ASSIGN "="
;

%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"
%token <std::string> STRING
//%nterm <int> body functions function content_item contents;
%nterm <std::unique_ptr<UnitNode>> unit;
%nterm <std::unique_ptr<BlockNode>> body contents;
%nterm <std::unique_ptr<UnitNode>> functions;
%nterm <std::unique_ptr<FunctionDefNode>> function;
%nterm <std::unique_ptr<ASTNode>> content_item;
%nterm <Type> type;
%nterm <std::unique_ptr<VarDeclNode>> var_decl;
%nterm <std::unique_ptr<ReturnNode>> return_statement;
%nterm <std::unique_ptr<BinaryOpNode>> var_assign;

%printer { yyo << $$; } <int> <std::string>;
%printer { yyo << "[AST Node]"; } <std::unique_ptr<ASTNode>> <std::unique_ptr<BlockNode>> <std::unique_ptr<UnitNode>> <std::unique_ptr<FunctionDefNode>> <Type>; 

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
  | TYPE_VOID { $$ = Type::VOID; }
  | TYPE_STRING { $$ = Type::STRING; };

var_decl:
  TYPE_INT IDENTIFIER SEMICOLON { $$ = std::make_unique<VarDeclNode>(toASTLoc(@$), Type::INTEGER, $2, nullptr); }
  | TYPE_STRING IDENTIFIER SEMICOLON { $$ = std::make_unique<VarDeclNode>(toASTLoc(@$), Type::STRING, $2, nullptr); }
  | TYPE_INT IDENTIFIER ASSIGN NUMBER SEMICOLON { $$ = std::make_unique<VarDeclNode>(toASTLoc(@$), Type::INTEGER, $2, std::make_unique<NumberNode>(toASTLoc(@4), $4)); }
  | TYPE_STRING IDENTIFIER ASSIGN STRING SEMICOLON { $$ = std::make_unique<VarDeclNode>(toASTLoc(@$), Type::STRING, $2, std::make_unique<StringNode>(toASTLoc(@4), $4)); };

var_assign:
  IDENTIFIER ASSIGN NUMBER SEMICOLON { $$ = std::make_unique<BinaryOpNode>(toASTLoc(@$), OperationType::ASSIGN, std::make_unique<VarRefNode>(toASTLoc(@1), $1), std::make_unique<NumberNode>(toASTLoc(@3), $3)); }
  | IDENTIFIER ASSIGN STRING SEMICOLON { $$ = std::make_unique<BinaryOpNode>(toASTLoc(@$), OperationType::ASSIGN, std::make_unique<VarRefNode>(toASTLoc(@1), $1), std::make_unique<StringNode>(toASTLoc(@3), $3)); };

body:
  LBRACE contents RBRACE { $$ = std::move($2); };

contents:
  %empty                 { $$ = std::make_unique<BlockNode>(toASTLoc(@$)); }
| contents content_item  {$1->addStatement(std::move($2)); $$ = std::move($1);};

content_item:
  return_statement { $$ = std::move($1); }
| var_decl                {$$ = std::move($1);}
| body                    {$$ = std::move($1);}
| var_assign              {$$ = std::move($1);}
| error SEMICOLON         {yyerrok;}

return_statement:
  RETURN NUMBER SEMICOLON { $$ = std::make_unique<ReturnNode>(toASTLoc(@$), std::make_unique<NumberNode>(toASTLoc(@2), $2)); }
| RETURN STRING SEMICOLON { $$ = std::make_unique<ReturnNode>(toASTLoc(@$), std::make_unique<StringNode>(toASTLoc(@2), $2)); }
| RETURN IDENTIFIER SEMICOLON { $$ = std::make_unique<ReturnNode>(toASTLoc(@$), std::make_unique<VarRefNode>(toASTLoc(@2), $2)); }
| RETURN error SEMICOLON { yyerrok; };
%%

void yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << "Syntax error at " << l << ": " << m << std::endl;
  drv.error(l, m);
}
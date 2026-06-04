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
  #include "../common/SourceLocation.hh"
  #include "../common/Types.hh"
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
  STAR "*"
  PLUS "+"
  MINUS "-"
  SLASH "/"
  PERCENT "%"
;

%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"
%token <std::string> STRING
%nterm <std::unique_ptr<UnitNode>> unit;
%nterm <std::unique_ptr<BlockNode>> body contents;
%nterm <std::unique_ptr<UnitNode>> functions;
%nterm <std::unique_ptr<FunctionDefNode>> function;
%nterm <std::unique_ptr<ASTNode>> content_item;
%nterm <std::unique_ptr<ASTNode>> expr;
%nterm <Type> function_type;
%nterm <Type> variable_type;
%nterm <std::unique_ptr<VarDeclNode>> var_decl;
%nterm <std::unique_ptr<ReturnNode>> return_statement;
%nterm <std::unique_ptr<BinaryOpNode>> var_assign;

%printer { yyo << $$; } <int> <std::string>;
%printer { yyo << "[AST Node]"; } <std::unique_ptr<ASTNode>> <std::unique_ptr<BlockNode>> <std::unique_ptr<UnitNode>> <std::unique_ptr<FunctionDefNode>> <Type>; 

%left PLUS MINUS
%left STAR SLASH PERCENT

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
  FUNK IDENTIFIER LPAREN RPAREN ARROW function_type body{ $$ = std::make_unique<FunctionDefNode>(toASTLoc(@$), $6, $2, std::move($7)); }
  | FUNK IDENTIFIER LPAREN RPAREN body { $$ = std::make_unique<FunctionDefNode>(toASTLoc(@$), Type::VOID, $2, std::move($5)); }
  | error RPAREN{ yyerrok; };

function_type:
  TYPE_VOID { $$ = Type::VOID; }
  | variable_type { $$ = $1; };

variable_type:
  TYPE_INT { $$ = Type::INTEGER; }
  | TYPE_STRING { $$ = Type::STRING; };

var_decl:
  variable_type IDENTIFIER SEMICOLON { $$ = std::make_unique<VarDeclNode>(toASTLoc(@$), $1, $2, nullptr); }
  | variable_type IDENTIFIER ASSIGN expr SEMICOLON { $$ = std::make_unique<VarDeclNode>(toASTLoc(@$), $1, $2, std::move($4)); };

var_assign:
  IDENTIFIER ASSIGN expr SEMICOLON { $$ = std::make_unique<BinaryOpNode>(toASTLoc(@$), OperationType::ASSIGN, std::make_unique<VarRefNode>(toASTLoc(@1), $1), std::move($3)); };

expr:
  NUMBER { $$ = std::make_unique<NumberNode>(toASTLoc(@$), $1); }
  | STRING { $$ = std::make_unique<StringNode>(toASTLoc(@$), $1); }
  | IDENTIFIER { $$ = std::make_unique<VarRefNode>(toASTLoc(@$), $1); }
  | expr PLUS expr { $$ = std::make_unique<BinaryOpNode>(toASTLoc(@$), OperationType::ADD, std::move($1), std::move($3)); }
  | expr MINUS expr { $$ = std::make_unique<BinaryOpNode>(toASTLoc(@$), OperationType::SUBTRACT, std::move($1), std::move($3)); }
  | expr STAR expr { $$ = std::make_unique<BinaryOpNode>(toASTLoc(@$), OperationType::MULTIPLY, std::move($1), std::move($3)); }
  | expr SLASH expr { $$ = std::make_unique<BinaryOpNode>(toASTLoc(@$), OperationType::DIVIDE, std::move($1), std::move($3)); }
  | expr PERCENT expr { $$ = std::make_unique<BinaryOpNode>(toASTLoc(@$), OperationType::MODULO, std::move($1), std::move($3)); }
  | LPAREN expr RPAREN { $$ = std::move($2); };

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
  RETURN expr SEMICOLON { $$ = std::make_unique<ReturnNode>(toASTLoc(@$), std::move($2)); }
| RETURN error SEMICOLON { yyerrok; };
%%

void yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << "Syntax error at " << l << ": " << m << std::endl;
  drv.error(l, m);
}
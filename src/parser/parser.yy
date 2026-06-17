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
  #include "../ast/ASTNode.hh"
  #include "../common/SourceLocation.hh"
  #include "../common/Types.hh"
  class Driver;
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
  WHILE "solange"
  TYPE_INT "zahl"
  TYPE_STRING "faden"
  TYPE_BOOL "logik"
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
  COMMA ","
  TRUE "richtig"
  FALSE "falsch"
  EQ "=="
  NEQ "!="
  LESS "<"
  LEQ "<="
  GEQ ">="
  GREATER ">"
  IF "wenn"
  ELSE "sonst"
;

%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"
%token <std::string> STRING
%nterm <bool> bool_expr;
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
%nterm <std::unique_ptr<WhileNode>> while_statement;
%nterm <std::unique_ptr<BinaryOpNode>> var_assign;
%nterm <FunctionParameter> param;
%nterm <std::vector<FunctionParameter>> param_list params;
%nterm <std::unique_ptr<FunctionCallNode>> func_call;
%nterm <std::vector<std::unique_ptr<ASTNode>>> expr_list exprs;
%nterm <std::unique_ptr<IfNode>> if_statement;
%nterm <std::vector<ElseIfBranch>> else_ifs;



%printer { yyo << $$; } <int> <std::string>;
%printer { yyo << "[AST Node]"; } <std::unique_ptr<ASTNode>> <std::unique_ptr<BlockNode>> <std::unique_ptr<UnitNode>> <std::unique_ptr<FunctionDefNode>> <Type>; 

%left EQ NEQ LESS LEQ GEQ GREATER
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
  FUNK IDENTIFIER LPAREN param_list RPAREN ARROW function_type body{ $$ = std::make_unique<FunctionDefNode>(toASTLoc(@$), $7, $2, std::move($8), std::move($4)); }
  | FUNK IDENTIFIER LPAREN param_list RPAREN body { $$ = std::make_unique<FunctionDefNode>(toASTLoc(@$), Type::VOID, $2, std::move($6), std::move($4)); }
  | error RPAREN{ yyerrok; };

param:
  variable_type IDENTIFIER { $$ = FunctionParameter{ $1, $2 }; };

params:
  param { $$ = std::vector<FunctionParameter>{ $1 }; }
  | params COMMA param { $1.push_back($3); $$ = std::move($1); };

param_list:
  %empty { $$ = std::vector<FunctionParameter>{}; }
  | params { $$ = std::move($1); };

function_type:
  TYPE_VOID { $$ = Type::VOID; }
  | variable_type { $$ = $1; };

variable_type:
  TYPE_INT { $$ = Type::INTEGER; }
  | TYPE_STRING { $$ = Type::STRING; }
  | TYPE_BOOL {$$ = Type::BOOLEAN;};

var_decl:
  variable_type IDENTIFIER SEMICOLON { $$ = std::make_unique<VarDeclNode>(toASTLoc(@$), $1, $2, nullptr); }
  | variable_type IDENTIFIER ASSIGN expr SEMICOLON { $$ = std::make_unique<VarDeclNode>(toASTLoc(@$), $1, $2, std::move($4)); };

var_assign:
  IDENTIFIER ASSIGN expr SEMICOLON { $$ = std::make_unique<BinaryOpNode>(toASTLoc(@$), OperationType::ASSIGN, std::make_unique<VarRefNode>(toASTLoc(@1), $1), std::move($3)); };

expr:
  NUMBER { $$ = std::make_unique<NumberNode>(toASTLoc(@$), $1); }
  | STRING { $$ = std::make_unique<StringNode>(toASTLoc(@$), $1); }
  | IDENTIFIER { $$ = std::make_unique<VarRefNode>(toASTLoc(@$), $1); }
  | bool_expr {$$ = std::make_unique<BoolNode>(toASTLoc(@$), $1);}
  | func_call { $$ = std::move($1); }
  | expr PLUS expr { $$ = std::make_unique<BinaryOpNode>(toASTLoc(@$), OperationType::ADD, std::move($1), std::move($3)); }
  | expr MINUS expr { $$ = std::make_unique<BinaryOpNode>(toASTLoc(@$), OperationType::SUBTRACT, std::move($1), std::move($3)); }
  | expr STAR expr { $$ = std::make_unique<BinaryOpNode>(toASTLoc(@$), OperationType::MULTIPLY, std::move($1), std::move($3)); }
  | expr SLASH expr { $$ = std::make_unique<BinaryOpNode>(toASTLoc(@$), OperationType::DIVIDE, std::move($1), std::move($3)); }
  | expr PERCENT expr { $$ = std::make_unique<BinaryOpNode>(toASTLoc(@$), OperationType::MODULO, std::move($1), std::move($3)); }
  | expr EQ expr { $$ = std::make_unique<BinaryOpNode>(toASTLoc(@$), OperationType::EQ, std::move($1), std::move($3)); }
  | expr NEQ expr { $$ = std::make_unique<BinaryOpNode>(toASTLoc(@$), OperationType::NEQ, std::move($1), std::move($3)); }
  | expr LESS expr { $$ = std::make_unique<BinaryOpNode>(toASTLoc(@$), OperationType::LESS, std::move($1), std::move($3)); }
  | expr LEQ expr { $$ = std::make_unique<BinaryOpNode>(toASTLoc(@$), OperationType::LEQ, std::move($1), std::move($3)); }
  | expr GEQ expr { $$ = std::make_unique<BinaryOpNode>(toASTLoc(@$), OperationType::GEQ, std::move($1), std::move($3)); }
  | expr GREATER expr { $$ = std::make_unique<BinaryOpNode>(toASTLoc(@$), OperationType::GREATER, std::move($1), std::move($3)); }
  | LPAREN expr RPAREN { $$ = std::move($2); };

bool_expr:
  TRUE {$$ = true;}
  | FALSE {$$ = false;};

expr_list:
  %empty { $$ = std::vector<std::unique_ptr<ASTNode>>{}; }
| exprs { $$ = std::move($1); };

exprs:
  expr { std::vector<std::unique_ptr<ASTNode>> list; list.push_back(std::move($1)); $$ = std::move(list);}
| exprs COMMA expr { $1.push_back(std::move($3)); $$ = std::move($1);};

body:
  LBRACE contents RBRACE { $$ = std::move($2); };

contents:
  %empty                 { $$ = std::make_unique<BlockNode>(toASTLoc(@$)); }
| contents content_item  {$1->addStatement(std::move($2)); $$ = std::move($1);};

content_item:
  return_statement { $$ = std::move($1); }
| var_decl                {$$ = std::move($1);}
| while_statement         {$$ = std::move($1);}
| body                    {$$ = std::move($1);}
| var_assign              {$$ = std::move($1);}
| func_call SEMICOLON     {$$ = std::move($1);}
| if_statement            {$$ = std::move($1);}
| error SEMICOLON         {yyerrok;};

while_statement:
  WHILE LPAREN expr RPAREN body { $$ = std::make_unique<WhileNode>(toASTLoc(@$), std::move($3), std::move($5)); };

func_call:
  IDENTIFIER LPAREN expr_list RPAREN {$$ = std::make_unique<FunctionCallNode>(toASTLoc(@$), $1, std::move($3)); };

return_statement:
  RETURN expr SEMICOLON { $$ = std::make_unique<ReturnNode>(toASTLoc(@$), std::move($2)); }
| RETURN error SEMICOLON { yyerrok; };

if_statement:
  IF LPAREN expr RPAREN body else_ifs
    { $$ = std::make_unique<IfNode>(toASTLoc(@$), std::move($3), std::move($5), std::move($6), nullptr); }
| IF LPAREN expr RPAREN body else_ifs ELSE body
    { $$ = std::make_unique<IfNode>(toASTLoc(@$), std::move($3), std::move($5), std::move($6), std::move($8)); }
;

else_ifs:
  %empty
    { $$ = std::vector<ElseIfBranch>{}; }
| else_ifs ELSE IF LPAREN expr RPAREN body
    { $1.push_back(ElseIfBranch{std::move($5), std::move($7)}); $$ = std::move($1); }
;
%%

void yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << "Syntax error at " << l << ": " << m << std::endl;
  drv.error(l, m);
}
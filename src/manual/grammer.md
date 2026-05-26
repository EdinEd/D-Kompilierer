# Grammar

> This grammar is intentionally **right-recursive** (e.g. `functions → function functions`) so it can be parsed directly by a hand-written **recursive-descent parser** using a single token of lookahead (LL(1)).

---

```
root:
    functions $

functions:
    %empty
  | function functions

function:
    FUNK IDENTIFIER LPAREN RPAREN function'

function':
    ARROW type body
  | body

type:
    TYPE_INT
  | TYPE_VOID
  | TYPE_STRING

var_decl:
    TYPE_INT IDENTIFIER var_decl_int
  | TYPE_STRING IDENTIFIER var_decl_string

var_decl_int:
    SEMICOLON
  | ASSIGN NUMBER SEMICOLON

var_decl_string:
    SEMICOLON
  | ASSIGN STRING SEMICOLON

var_assign:
    ASSIGN var_assign'

var_assign':
    NUMBER SEMICOLON
  | STRING SEMICOLON
  | IDENTIFIER SEMICOLON

body:
    LBRACE contents RBRACE

contents:
    %empty
  | content_item contents

content_item:
    return_statement
  | var_decl
  | body
  | identifier_call

identifier_call:
    IDENTIFIER identifier_call'

identifiert_call':
    function_call
  | var_assign

return_statement:
    RETURN return_statement'

return_statement':
    NUMBER SEMICOLON
  | STRING SEMICOLON
  | IDENTIFIER SEMICOLON

function_call:
    LPAREN RPAREN SEMICOLON
```

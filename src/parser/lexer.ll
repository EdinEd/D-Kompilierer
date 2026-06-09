%{
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <cstring>
# include <string>
#include <iostream>
# include "Driver.hh"
# include "parser.hh"
%}

%option noyywrap nounput noinput batch debug
%x COMMENT

%{
  // A number symbol corresponding to the value in S.
  yy::parser::symbol_type
  make_NUMBER (const std::string &s, const yy::parser::location_type& loc);
  yy::parser::symbol_type make_STRING(const std::string &s, const yy::parser::location_type& loc);
%}

id      [a-zA-ZÄÖÜäöü][a-zA-Z_0-9ÄÖÜäöü]*
string  \"(\\.|[^\\"])*\"
int     [0-9]+
blank   [ \t\r]

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}
%%
%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = drv.location;
  // Code run each time yylex is called.
  loc.step ();
%}

{blank}+   loc.step ();
\n+        loc.lines (yyleng); loc.step ();
"€€"       {BEGIN (COMMENT);};
<COMMENT>[^\n]* {loc.step();};
<COMMENT>"\n" {loc.lines (yyleng); loc.step ();BEGIN(INITIAL);};

"("        return yy::parser::make_LPAREN (loc);
")"        return yy::parser::make_RPAREN (loc);
"zurück"   return yy::parser::make_RETURN (loc);
"solange"  return yy::parser::make_WHILE (loc);
"zahl"      return yy::parser::make_TYPE_INT (loc);
"faden"     return yy::parser::make_TYPE_STRING (loc);
"->"       return yy::parser::make_ARROW (loc);
"{"        return yy::parser::make_LBRACE (loc);
"}"        return yy::parser::make_RBRACE (loc);
";"        return yy::parser::make_SEMICOLON (loc);
"funk"     return yy::parser::make_FUNK (loc);
"leer"     return yy::parser::make_TYPE_VOID (loc);
"="        return yy::parser::make_ASSIGN (loc);
"+"        return yy::parser::make_PLUS (loc);
"-"        return yy::parser::make_MINUS (loc);
"*"        return yy::parser::make_STAR (loc);
"/"        return yy::parser::make_SLASH (loc);
"%"        return yy::parser::make_PERCENT (loc);
","        return yy::parser::make_COMMA (loc);


{int}      return make_NUMBER (yytext, loc);
{string}   return make_STRING (yytext, loc);
{id}       return yy::parser::make_IDENTIFIER (yytext, loc);
.          {
             throw yy::parser::syntax_error
               (loc, "invalid character: " + std::string(yytext));
}
<<EOF>>    return yy::parser::make_YYEOF (loc);
%%

yy::parser::symbol_type
make_NUMBER (const std::string &s, const yy::parser::location_type& loc)
{
  long n = strtol (s.c_str(), NULL, 10);
  return yy::parser::make_NUMBER ((int) n, loc);
}
yy::parser::symbol_type 
make_STRING(const std::string &s, const yy::parser::location_type& loc)
{
  if(s.length() > 2){
    return yy::parser::make_STRING(s.substr(1, s.length() - 2), loc);
  }
  return yy::parser::make_STRING("", loc);
}

void Driver::scan_begin ()
{
  yy_flex_debug = trace_scanning;
  if (file.empty () || file == "-")
    yyin = stdin;
  else if (!(yyin = fopen (file.c_str (), "r")))
    {
      std::cerr << "cannot open " << file << ": " << strerror (errno) << '\n';
      exit (EXIT_FAILURE);
    }
}

void Driver::scan_end ()
{
  fclose (yyin);
}
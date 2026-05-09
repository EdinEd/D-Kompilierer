%{
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <cstring> // strerror
# include <string>
# include "Driver.hh"
# include "parser.hh"
%}

%option noyywrap nounput noinput batch debug
%x COMMENT

%{
  // A number symbol corresponding to the value in S.
  yy::parser::symbol_type
  make_NUMBER (const std::string &s, const yy::parser::location_type& loc);
%}

id      [a-zA-ZÄÖÜ][a-zA-Z_0-9ÄÖÜ]*
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
"zahl"      return yy::parser::make_TYPE_INT (loc);
"faden"     return yy::parser::make_TYPE_STRING (loc);
"->"       return yy::parser::make_ARROW (loc);
"{"        return yy::parser::make_LBRACE (loc);
"}"        return yy::parser::make_RBRACE (loc);
";"        return yy::parser::make_SEMICOLON (loc);
"funk"     return yy::parser::make_FUNK (loc);
"leer"     return yy::parser::make_TYPE_VOID (loc);


{int}      return make_NUMBER (yytext, loc);
{string}   return yy::parser::make_STRING (yytext, loc);
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
  errno = 0;
  long n = strtol (s.c_str(), NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    throw yy::parser::syntax_error (loc, "integer is out of range: " + s);
  return yy::parser::make_NUMBER ((int) n, loc);
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
CHAR [a-zA-Z]
DIG [0-9]
ID ({CHAR}|_)({CHAR}|{DIG}|_)*
NUM {DIG}+
DUB {NUM}(\.{DIG}+)?(E[+-]?{DIG}+)?

%{
#include <stdlib.h> /* for atoi call */
#include "cal.tab.h"
#include <cmath>
%}



%%


{NUM} {
	yylval.num = atoi(yytext);
	return NUMBER;
}

{DUB} {
	yylval.dub = atof(yytext);
	return DOUBLE;
}

"<=" { return LE;}
">=" { return GE;}
"==" { return EQ;}
"!=" { return NE;}


[!()%*/+-={},><^;] { return *yytext;}


"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }

"sin"		{ return SIN; }
"cos"		{ return COS; }
"tan"		{ return TAN; }
"pi"		{ yylval.dub = M_PI; return DOUBLE; }
"print"		{ return PRINT; }

"int" 		{ return INT; }
"float" 	{ return FLOAT; }
"char"		{ return CHAR; }

{ID}        {  yylval.txt = strdup(yytext); return ID; }

[ \t\n]+ {}

%%

int yywrap()
{ 
 return 1;
} 

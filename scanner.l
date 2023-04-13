%{
#include "y.tab.h"
%}
%%
bool|char|int|real|string|if|else|while|for|null|function|return|void|do {yylval=yytext; return KEYWORD;}
[0-9]+\.[0-9]+ {yylval=atoi(yytext); return REAL;}
[0-9]+ {yylval=atoi(yytext); return INTEGER;}
true|false {yylval=(yytext[0]==t); return BOOL;}
\".*\" {yylval=yytext; return STRING;}
\'[a-zA-Z0-9]\' {yylval=yytext[1]; return CHARACTER;}
[a-zA-Z][a-zA-Z0-9_]* {yylval=yytext; return IDENTIFIER;}
[-+*/] {return *yytext;}
%%

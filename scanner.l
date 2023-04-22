%{
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"	//generated via yacc -d
%}

%option noyywrap

%%

function			{yylval.s = yytext; return FUNCTION;}
if				{yylval.s = yytext; return IF;}
else				{yylval.s = yytext; return ELSE;}
void				{yylval.s = yytext; return VOID;}
int				{yylval.s = yytext; return INT;}
arg->				{yylval.s = yytext; return ARG;}
[a-zA-Z][a-zA-Z0-9]*		{yylval.s = yytext; return IDENTIFIER;}
[0-9]+				{yylval.i = atof(yytext); return INTEGER;}
[0-9]+(\.[0-9]+)		{yylval.f = atof(yytext); return REAL;}
[-+()*/{}]			{return yytext[0];}
[ \t\f\v\n]			{ ; }

%%

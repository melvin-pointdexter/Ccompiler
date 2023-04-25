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
do				{yylval.s = yytext; return DO;}
while				{yylval.s = yytext; return WHILE;}
for				{yylval.s = yytext; return FOR;}
var				{yylval.s = yytext; return VAR;}
return				{yylval.s = yytext; return RETURN;}
null				{yylval.s = yytext; return NULL;}
void				{yylval.s = yytext; return VOID;}
arg->				{yylval.s = yytext; return ARG;}
int\*				{yylval.s = yytext; return INT_P;}
real\*				{yylval.s = yytext; return REAL_P;}
char\*				{yylval.s = yytext; return CHAR_P;}
int				{yylval.s = yytext; return INT;}
real				{yylval.s = yytext; return REAL;}
char				{yylval.s = yytext; return CHAR;}
bool				{yylval.s = yytext; return BOOL;}
string				{yylval.s = yytext; return STRING;}
[a-zA-Z][a-zA-Z0-9]*		{yylval.s = yytext; return IDENTIFIER;}
[0-9]+				{yylval.i = atof(yytext); return INTEGER_LITERAL;}
[0-9]+(\.[0-9]+)		{yylval.f = atof(yytext); return REAL_LITERAL;}
\'[a-zA-Z0-9]\'			{yylval.c = atof(yytext[1]); return CHAR_LITERAL;}
\"[a-zA-Z0-9]*\"		{yylval.s = yytext; return STRING_LITERAL;}
true|false			{yylval.b = atof(yytext); return BOOL_LITERAL;}
[-+()*/{}]			{return yytext[0];}
[ \t\f\v\n]			{ ; }

%%

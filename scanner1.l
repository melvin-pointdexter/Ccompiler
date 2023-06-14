%{
#include "ast.h"
#include "syTable.h"
#include <stdio.h>
//#include <stdlib.h>
#include <ctype.h>
#include "y.tab.h"	//generated via yacc -d
int yyerror(char *msg);
%}

%option noyywrap

%%

function				{yylval.s = yytext; return FUNCTION;}
if					{yylval.s = yytext; return IF;}
else					{yylval.s = yytext; return ELSE;}
do					{yylval.s = yytext; return DO;}
while					{yylval.s = yytext; return WHILE;}
for					{yylval.s = yytext; return FOR;}
var					{yylval.s = yytext; return VAR;}
return					{yylval.s = yytext; return RETURN;}
null					{yylval.s = yytext; return NULL_P;}
void					{yylval.s = yytext; return VOID;}
arg->					{yylval.s = yytext; return ARG;}
int\*					{yylval.s = yytext; return INT_P;}
real\*					{yylval.s = yytext; return REAL_P;}
char\*					{yylval.s = yytext; return CHAR_P;}
int					{yylval.s = yytext; return INT;}
real					{yylval.s = yytext; return REAL;}
char					{yylval.s = yytext; return CHAR;}
bool					{yylval.s = yytext; return BOOL;}
string					{yylval.s = yytext; return STRING;}
true					{yylval.s = yytext; return BOOL_LITERAL;}
false					{yylval.s = yytext; return BOOL_LITERAL;}
[a-zA-Z][a-zA-Z0-9]*			{yylval.n = mknode(yytext, 0, DONT_ADD_TAB); return IDENTIFIER;}
[0-9]+					{yylval.n = mknode(yytext, 0, DONT_ADD_TAB); return INTEGER_LITERAL;}
0[xX][0-9A-F]+				{yylval.n = mknode(yytext, 0, DONT_ADD_TAB);  return INTEGER_LITERAL;}
[0-9]+(\.[0-9]+)			{yylval.s = yytext; return REAL_LITERAL;}
[0-9]+(\.[0-9]+)?([eE][-+]?[0-9]+)	{yylval.s = yytext; return REAL_LITERAL;}
'.'					{if (isprint(yytext[1])){yylval.s = yytext; return CHAR_LITERAL; } yyerror("current character is not printable."); }
\/\*([^\/\*]|\*[^\/]|[^\*]\/)*\*\/	{ }
\".*\\\".*\"				{yyerror("double quotes are not allowed.");}
\"[^\"\n]*\"				{yylval.n = mknode(yytext,0,DONT_ADD_TAB); return STRING_LITERAL;}
[-+()*/{}\;\|\,=\[\]\(\)\:<>&]		{yylval.c = yytext[0];return yytext[0];}
[ \t\f\v\n]				{ }
\|\|					{yylval.s = yytext; return OR;}
\&\&					{yylval.s = yytext; return AND;}
\=\=					{yylval.s = yytext; return EQ;}
\!\=					{yylval.s = yytext; return NE;}
\>\=					{yylval.s = yytext; return GE;}
\<\=					{yylval.s = yytext; return LE;}
.					{yyerror(yytext);}

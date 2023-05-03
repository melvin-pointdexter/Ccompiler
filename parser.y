%{
#include <stdio.h>
//#include <stdlib.h>
#include "lex.yy.c"

extern int yylex();
%}

%union {
  float f;
  char *s;
  int i;
  unsigned int b;
  char c;
}

%token <s> REAL_LITERAL
%token <s> INTEGER_LITERAL
%token <s> BOOL_LITERAL
%token <s> CHAR_LITERAL
%token <s> FUNCTION RETURN IF ELSE DO WHILE FOR VAR NULL_P VOID ARG INT_P REAL_P CHAR_P INT REAL CHAR BOOL STRING STRING_LITERAL IDENTIFIER
%token <s> OR AND EQ NE GE LE

%type <s> func proc_statement func_statement statement expr args type proc varlist argdecl arglist

%right '='
%left OR
%left AND
%left EQ NE
%left LE GE '<' '>'
%left '+' '-'
%left '*' '/'
%left '(' '['

%%

S       : func S
        | proc
        ;

func    : FUNCTION IDENTIFIER '(' args ')' ':' type '{' func_statement '}'
	| FUNCTION IDENTIFIER '(' ')' ':' type '{' func_statement '}'
	;
	
proc    : FUNCTION IDENTIFIER '(' args ')' ':' VOID '{' proc_statement '}'
	| FUNCTION IDENTIFIER '(' ')' ':' VOID '{' proc_statement '}'
	;

type	: INT    {printf("int\n"); printf("int: %s, %s", yylval.s, yytext);}
	| REAL    {printf("%s, %s", yylval.s, yytext);}
	| CHAR    {printf("%s",yytext);}    
	| BOOL    {printf("%s, %s", yylval.s, yytext);}
	| INT_P    {printf("%s, %s", yylval.s, yytext);}
	| REAL_P    {printf("%s, %s", yylval.s, yytext);}
	| CHAR_P    {printf("%s, %s", yylval.s, yytext);}
	;

args    : args ';' ARG argdecl
        | ARG argdecl
        ;

argdecl : arglist ':' type
        ;

arglist : IDENTIFIER ',' arglist
        | IDENTIFIER
	;

proc_statement	: IF '(' expr ')' '{' proc_statement '}' proc_statement
		| WHILE '(' expr ')' '{' proc_statement '}' proc_statement 
		| RETURN ';'
		| statement proc_statement
		|
		;
		
func_statement	: statement func_statement
		| IF '(' expr ')' '{' func_statement '}' func_statement
		| IF '(' expr ')' '{' proc_statement '}' func_statement
		| WHILE '(' expr ')' '{' func_statement '}' func_statement
		| WHILE '(' expr ')' '{' proc_statement '}' func_statement
		| RETURN expr ';' func_statement
		| RETURN expr ';'
		;

statement	: VAR varlist ':' type ';'		{printf("varlist statemt");}
		| expr ';'
		| IDENTIFIER '=' expr ';'
		| func
		| proc
		;

varlist	: IDENTIFIER ',' varlist
	| IDENTIFIER '=' expr ',' varlist
	| IDENTIFIER '=' expr
	| IDENTIFIER
	;

expr	: IDENTIFIER
	| REAL_LITERAL
	| INTEGER_LITERAL
	| BOOL_LITERAL
	| expr '+' expr
	| expr '-' expr
	| expr '*' expr
	| expr '/' expr
	| expr OR expr
	| expr AND expr
	| expr EQ expr
	| expr NE expr
	| expr GE expr
	| expr LE expr
	| expr '>' expr
	| expr '<' expr
	| '|' STRING_LITERAL '|'
	| '*' IDENTIFIER
	| '&' IDENTIFIER
	;

%%

int yyerror(char *msg) {
     fprintf(stderr, "\n%s\nyylval:%s\n", msg, yylval.s);
     //exit(1);
    return 1;
}

int main() {
    yyparse();
    return 0;
}


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

%token <f> REAL_LITERAL
%token <i> INTEGER_LITERAL
%token <b> BOOL_LITERAL
%token <c> CHAR_LITERAL
%token <s> FUNCTION RETURN IF ELSE DO WHILE FOR VAR NULL_P VOID ARG INT_P REAL_P CHAR_P INT REAL CHAR BOOL STRING STRING_LITERAL IDENTIFIER
%token OR AND EQ NE GE LE

%type <s> func body expr args type proc varlist argdecl arglist

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

func    : FUNCTION IDENTIFIER '(' args ')' ':' type '{' body '}'
	| FUNCTION IDENTIFIER '(' ')' ':' type '{' body '}'
	;

type	: INT    {printf("int\n"); printf("int: %d, %s", yylval.i, yytext);}
	| REAL    {printf("%f, %s", yylval.f, yytext);}
	| CHAR    {printf("%s",yytext);}    
	| BOOL    {printf("%u, %s", yylval.b, yytext);}
	| INT_P    {printf("%s, %s", yylval.s, yytext);}
	| REAL_P    {printf("%s, %s", yylval.s, yytext);}
	| CHAR_P    {printf("%s, %s", yylval.s, yytext);}
	;

proc    : FUNCTION IDENTIFIER '(' args ')' ':' VOID '{' body '}'
	| FUNCTION IDENTIFIER '(' ')' ':' VOID '{' body '}'
	;

args    : args ';' ARG argdecl
        | ARG argdecl
        ;

argdecl : arglist ':' type
        ;

arglist : IDENTIFIER ',' arglist
        | IDENTIFIER
	;

body	: IF '(' expr ')' '{' body '}' 
	| WHILE '(' expr ')' '{' body '}'
	| RETURN expr ';' body
	| VAR varlist ':' type ';' body
	| expr ';' body
	| IDENTIFIER '=' expr ';' body
	| 
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
	;

%%

int yyerror(char *msg) {
     fprintf(stderr, "\n%s\nyylval:%s\n", msg, yylval);
     //exit(1);
    return 1;
}

int main() {
    yyparse();
    return 0;
}


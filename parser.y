%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
void yyerror(char *msg);
%}

%union {
  float f;
  char *s;
  int i;
  bool b;
  char c;
}

%token <f> REAL_LITERAL
%token <i> INTEGER_LITERAL
%token <b> BOOL_LITERAL
%token <c> CHAR_LITERAL
%token <s> FUNCTION IF ELSE DO WHILE FOR VAR RETURN NULL VOID ARG INT_P REAL_P CHAR_P INT REAL CHAR BOOL STRING IDENTIFIER

%type <s> func body expr args

%left '||'
%left '&&'
%left '==' '!=' '<=' '>=' '<' '>'
%left '+' '-'
%left '*' '/'
%left '(' '['

%%

S       : func S
        | proc
        ;

func    : FUNCTION IDENTIFIER '(' args ')' ':' type '{' statement '}'
	;

type	: INT 
	| REAL
	| CHAR
	| BOOL
	| INT_P
	| REAL_P
	| CHAR_P
	;

proc    : FUNCTION IDENTIFIER '(' args ')' ':' VOID '{' statement '}'
	;

args    : args ';' ARG vardecl
        | ARG vardecl
        ;

vardecl : list ':' type
        ;

list    : IDENTIFIER ',' list
        | IDENTIFIER
	;

body	:	{/* complete */}
	;

%%

void yyerror(char *msg) {
     fprintf(stderr, "%s\n", msg);
     exit(1);
}

int main() {
    yyparse();
    return 0;
}

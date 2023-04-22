%{
#include <stdio.h>
#include <stdlib.h>

int indent;
extern int yylex();
void yyerror(char *msg);
%}

%union {
  float f;
  char *s;
  int i;
}

%token <f> REAL
%token <i> INTEGER
%token <s> FUNCTION IF ELSE VOID INT IDENTIFIER ARG

%type <s> func body expr args

%%

S    : S func
     | func
     |
     ;

func : FUNCTION IDENTIFIER '(' args ')' ':' INTEGER '{' body '}'	{printf("function");}
     | FUNCTION IDENTIFIER '(' args ')' ':' VOID '{' body '}'		{printf("procedure");}

args : arg ';' args
     | arg
     ;

arg  : ARG list ':' INTEGER
     ;

list : IDENTIFIER ',' list
     | IDENTIFIER
     ;

body : IF '{' body '}' body
     | IF '{' body '}' ELSE '{' body '}' body
     | expr body
     | func body
     ;

expr : IDENTIFIER '=' IDENTIFIER '+' IDENTIFIER				{printf("%s = %s + %s", $1, $3, $5);}
     ;

%%

void yyerror(char *msg) {
     fprintf(stderr, "%s\n", msg);
     exit(1);
}

int main() {
    indent=0;
    yyparse();
    return 0;
}

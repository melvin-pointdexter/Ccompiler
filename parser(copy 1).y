%{
#include <stdio.h>
//#include <stdlib.h>
#include "lex.yy.c"
#include "ast.h"

extern int yylex();
%}

%union {
  float f;
  char *s;
  int i;
  unsigned int b;
  char c;
  struct node* n;
}

%token <s> REAL_LITERAL
%token <s> INTEGER_LITERAL
%token <s> BOOL_LITERAL
%token <s> CHAR_LITERAL
%token <s> FUNCTION RETURN IF ELSE DO WHILE FOR VAR NULL_P VOID ARG INT_P REAL_P CHAR_P INT REAL CHAR BOOL STRING STRING_LITERAL
%token <n> IDENTIFIER
%token <s> OR AND EQ NE GE LE
%type <n> S proc func assign_statement statement statements_list expr args type varlist var_decl_list 
%type <n> argdecl arglist func_prod_list body

%right '='
%left OR
%left AND
%left EQ NE
%left LE GE '<' '>'
%left '+' '-'
%left '*' '/'
%left '(' '['

%nonassoc ELSE
%%

S       : func_prod_list		{ $$ = mknode("Code", 1, NODE_PROGRAM); $$->nodes[0] = $1 ;} //printAst($$,0);}
        ;

func    : FUNCTION IDENTIFIER '(' args ')' ':' type '{' body RETURN expr ';' '}' { /*$$-> = mknode("Func", 12, NODE_FUNC_PROD_LIST); $$->nodes[0] = $1; $$->nodes[1] = $2;*/ }
	| FUNCTION IDENTIFIER '(' ')' ':' type '{' body RETURN expr ';' '}'
	;
	
proc    : FUNCTION IDENTIFIER '(' args ')' ':' VOID '{' body '}'   { $$ = mknode("Func", 4, NODE_PROC); $$->nodes[0] = $2;
														  $$->nodes[1] = mknode($4 ,0, ADD_TAB);
														$$->nodes[2] = mknode("RET VOID", 0, NODE_TERMINAL);														  
														$$->nodes[3] = $9;}
	| FUNCTION IDENTIFIER '(' ')' ':' VOID '{' body '}'		{ $$ = mknode("Func", 3, NODE_PROC); printf("Node\n"); $$->nodes[0] = $2; printf("B");
														  $$->nodes[1] = mknode("RET VOID", 0, NODE_TERMINAL);
														  $$->nodes[2] = $8;}
	;

type	: INT    { $$ = mknode("INT", 0, DONT_ADD_TAB);}
	| REAL    { $$ = mknode("REAL", 0, DONT_ADD_TAB);}
	| CHAR    { $$ = mknode("CHAR", 0, DONT_ADD_TAB);} 
	| BOOL    { $$ = mknode("BOOL", 0, DONT_ADD_TAB);}
	| INT_P    { $$ = mknode("INT_P", 0, DONT_ADD_TAB);}
	| REAL_P    { $$ = mknode("REAL_P", 0, DONT_ADD_TAB);}
	| CHAR_P    { $$ = mknode("CHAR_P", 0, DONT_ADD_TAB);}
	;

args    : args ';' ARG argdecl {$$ = mknode("", 2, DONT_ADD_TAB); $$->nodes[0] = $1; $$->nodes[1] = $4;}
        | ARG argdecl {$$ = mknode("ARGS", 1 , DONT_ADD_TAB); $$->nodes[0] = $2;}
        ;

argdecl : arglist ':' type {$$ = mknode("", 2, DONT_ADD_TAB); $$->nodes[0] = $3; $$->nodes[1] = $1;}
        ;

arglist : IDENTIFIER ',' arglist { $$ = mknode(',', 2, DONT_ADD_TAB); $$->nodes[0] = $1; $$->nodes[0] = $3;}
        | IDENTIFIER { $$ = $1;}
	;					
	
body: func_prod_list var_decl_list statements_list 
	| func_prod_list var_decl_list
	| func_prod_list statements_list
	| func_prod_list
	| var_decl_list statements_list
	| var_decl_list { $$ = $1; }
	| statements_list
	| { $$ = mknode("NONE", 0, DONT_ADD_TAB);}
	;
	 
func_prod_list : func				{ $$ = $1; }
		| func func_prod_list		{ $$ = mknode(NULL, 2, NODE_FUNC_PROD_LIST); $$->nodes[0] = $1; $$->nodes[1] = $2; }
		| proc				{ $$ = $1; }
		| proc func_prod_list		{ $$ = mknode(NULL, 2, NODE_FUNC_PROD_LIST); $$->nodes[0] = $1; $$->nodes[1] = $2; }
		;
		
var_decl_list : VAR varlist ':' type ';' { $$ = mknode($4, 1, DONT_ADD_TAB); $$->nodes[0] = $2;}
		| VAR varlist ':' type ';' var_decl_list { $$ = mknode($4, 2, DONT_ADD_TAB); $$->nodes[0] = $2; $$ -> nodes[1] = $6;}
		| STRING stringlist ';' { $$ = mknode("String declaration", 0, DONT_ADD_TAB);}
		| STRING stringlist ';' var_decl_list { $$ = mknode("String declaration", 0, DONT_ADD_TAB);}
		;
		
		
body_of_nested_statement : var_decl_list statements_list
			  | var_decl_list
			  | statements_list
			  |
			  ;		


statements_list : statement
		| statement statements_list
		;

		
assign_statement	: IDENTIFIER '=' expr ';'
				| IDENTIFIER '[' expr ']' '=' expr ';'
				| '*' IDENTIFIER '=' expr ';'
				;		

cond_else_statement 	:  IF "(" expr ")" statement ';' ELSE statement ';'
						|  IF "(" expr ")" "{" body_of_nested_statement "}"
									ELSE "{" body_of_nested_statement "}"
						;

cond_statement: IF '(' expr ')' '{' body_of_nested_statement '}'
					;			

statement	:  IF '(' expr ')' statement ';' 
		| cond_statement 
		| assign_statement
		| '{' body_of_nested_statement '}'
		;			

varlist	: IDENTIFIER ',' varlist  { $$ = $1;}
	| IDENTIFIER '=' expr ',' varlist
	| IDENTIFIER '=' expr  { $$ = mknode("=", 2, DONT_ADD_TAB); $$->nodes[0] = $1; $$->nodes[1] = $3;}
	| IDENTIFIER { $$ = $1;}
	;

stringlist 	: IDENTIFIER '[' INTEGER_LITERAL ']' ',' stringlist
	| IDENTIFIER '[' INTEGER_LITERAL ']' '=' STRING_LITERAL ',' stringlist  
	| IDENTIFIER '[' INTEGER_LITERAL ']' '=' STRING_LITERAL
	| IDENTIFIER '[' INTEGER_LITERAL ']'
	;

expr	: '(' expr ')' 
	| IDENTIFIER  { $$ = $1;}
	| REAL_LITERAL  { $$ = $1;}
	| INTEGER_LITERAL  { $$ = mknode($1, 0, DONT_ADD_TAB);}
	| BOOL_LITERAL  { $$ = mknode($1, 0, DONT_ADD_TAB);}
	| STRING_LITERAL
	| IDENTIFIER '[' expr ']'
	| expr '+' expr  { $$ = mknode("+", 2, ADD_TAB); $$->nodes[0] = $1; $$->nodes[1] = $3;}
	| expr '-' expr { $$ = mknode("-", 2, ADD_TAB); $$->nodes[0] = $1; $$->nodes[1] = $3;}
	| expr '*' expr { $$ = mknode("*", 2, ADD_TAB); $$->nodes[0] = $1; $$->nodes[1] = $3;}
	| expr '/' expr { $$ = mknode("/", 2, ADD_TAB); $$->nodes[0] = $1; $$->nodes[1] = $3;}
	| expr OR expr { $$ = mknode("||", 2, ADD_TAB); $$->nodes[0] = $1; $$->nodes[1] = $3;}
	| expr AND expr { $$ = mknode("&&", 2, ADD_TAB); $$->nodes[0] = $1; $$->nodes[1] = $3;}
	| expr EQ expr { $$ = mknode("==", 2, ADD_TAB); $$->nodes[0] = $1; $$->nodes[1] = $3;}
	| expr NE expr { $$ = mknode("!=", 2, ADD_TAB); $$->nodes[0] = $1; $$->nodes[1] = $3;}
	| expr GE expr { $$ = mknode(">=", 2, ADD_TAB); $$->nodes[0] = $1; $$->nodes[1] = $3;}
	| expr LE expr { $$ = mknode("<=", 2, ADD_TAB); $$->nodes[0] = $1; $$->nodes[1] = $3;}
	| expr '>' expr { $$ = mknode(">", 2, ADD_TAB); $$->nodes[0] = $1; $$->nodes[1] = $3;}
	| expr '<' expr { $$ = mknode("<", 2, ADD_TAB); $$->nodes[0] = $1; $$->nodes[1] = $3;}
	| '+'	INTEGER_LITERAL
	| '+' REAL_LITERAL
	| '-' INTEGER_LITERAL
	| '-' REAL_LITERAL
	| '|' STRING_LITERAL '|'
	| '*' IDENTIFIER
	| '&' IDENTIFIER
	;

%%

int yyerror(char *msg) {
     fprintf(stderr, "\n%s\nerror in line %d after: %s\n", msg, yylineno, yylval.s);
     //exit(1);
    return 1;
}

int main() {
    yyparse();
    return 0;
}



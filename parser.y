%{
#include <stdio.h>
//#include <stdlib.h>
#include "lex.yy.c"
#include "ast.c"

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
%token <s> FUNCTION RETURN IF ELSE DO WHILE FOR VAR NULL_P VOID ARG INT_P REAL_P CHAR_P INT REAL CHAR BOOL STRING STRING_LITERAL IDENTIFIER
%token <c> '(' ')' '{' '}' ':' ';'
%token <s> OR AND EQ NE GE LE

%type <n> S proc func statement statement_include_ret expr args type varlist argdecl arglist func_prod_list body

%right '='
%left OR
%left AND
%left EQ NE
%left LE GE '<' '>'
%left '+' '-'
%left '*' '/'
%left '(' '['

%%

S       : func_prod_list		{ $$ = mknode("Code", 1, NODE_PROGRAM); $$->nodes[0] = $1 ; printAst($$,0);}
        ;

func    : FUNCTION IDENTIFIER '(' args ')' ':' type '{' body RETURN expr ';' '}' { /*$$-> = mknode("Func", 12, NODE_FUNC_PROD_LIST); $$->nodes[0] = $1; $$->nodes[1] = $2;*/ }
	| FUNCTION IDENTIFIER '(' ')' ':' type '{' body RETURN expr ';' '}'
	;
	
proc    : FUNCTION IDENTIFIER '(' args ')' ':' VOID '{' body '}'
	| FUNCTION IDENTIFIER '(' ')' ':' VOID '{' body '}'		{ $$ = mknode("Func", 2, NODE_PROC); $$->nodes[0] = mknode($2,0,NODE_TERMINAL);$$->nodes[1] = mknode($6,0,NODE_TERMINAL);}
	;

type	: INT    {printf("int: %s", yylval.s);} // $0 is INT, type is $$
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
	
body: func_prod_list var_decl_list statements_list 
	| func_prod_list var_decl_list
	| func_prod_list statements_list
	| func_prod_list
	| var_decl_list statements_list
	| var_decl_list
	| statements_list
	|
	;
	 
func_prod_list : func				{ $$ = $1; }
		| func func_prod_list		{ $$ = mknode(NULL, 0, NODE_FUNC_PROD_LIST); $$->nodes[0] = $1; $$->nodes[1] = $2; }
		| proc				{ $$ = $1; }
		| proc func_prod_list		{ $$ = mknode(NULL, 0, NODE_FUNC_PROD_LIST); $$->nodes[0] = $1; $$->nodes[1] = $2; }
		;
		
var_decl_list : VAR varlist ':' type ';'
		| VAR varlist ':' type ';' var_decl_list
		;
		
		
body_of_nested_statement : var_decl_list statements_list
			  | var_decl_list
			  | statements_list
			  |
			  ;		

/*
func_statement	: func_statement statement
		| func_statement statement_include_ret
		| func_statement RETURN expr ';'
		| body_no_statement func_statement
		| body_no_statement RETURN expr  ';'
		;


body_of_nested_statement_include_ret:   VAR varlist ':' type ';' body_of_nested_statement_include_ret
						| body_of_nested_statement  statement_include_ret
						| 
						;

*/

statements_list : statement
		| statement statements_list
		;
		
assign_statement	: IDENTIFIER '=' expr ';'
				| IDENTIFIER '[' expr ']' '=' expr ';'
				| '*' IDENTIFIER '=' expr ';'
				;		

statement	:  assign_statement
		| IF '(' expr ')' '{' body_of_nested_statement '}'
		| WHILE '(' expr ')' '{' body_of_nested_statement '}'
		;

statement_include_ret	: statement RETURN expr ';'
					/*| IF '(' expr ')' '{' body_of_nested_statement_include_ret '}'
					| WHILE '(' expr ')' '{' body_of_nested_statement_include_ret '}'*/
					| RETURN expr ';' statement
					;
					
			// TODO: find all combinations:
			/*
statements_can_use_ret_list : statements_list statements_can_use_ret_list
			     | RETURN expr ';' statements_can_use_ret_list
			     | RETURN expr ';'
			     |
			     ;*/
				
				
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
	
	/*| '|' STRING_LITERAL '|'
	| '*' IDENTIFIER
	| '&' IDENTIFIER
	;*/

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



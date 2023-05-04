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
%token <s> OR AND EQ NE GE LE '*' '|' '&' '{' '}'

<<<<<<< Updated upstream
%type <s> proc func statement statement_include_ret expr args type varlist argdecl arglist
=======
%type <s> func proc_statement func_statement statement expr args type proc varlist argdecl arglist proc_codeblock func_codeblock codeblock
>>>>>>> Stashed changes

%right '='
%left OR
%left AND
%left EQ NE
%left LE GE '<' '>'
%left '+' '-'
%left '*' '/'
%left '(' '['

%%

S       : func S	{ printAst($1, 0); }
        | proc		{ printAst($1, 0); }
        ;

<<<<<<< Updated upstream
func    : FUNCTION IDENTIFIER '(' args ')' ':' type '{' body RETURN expr ';' '}'
	| FUNCTION IDENTIFIER '(' ')' ':' type '{' body RETURN expr ';' '}'
=======
func    : FUNCTION IDENTIFIER '(' args ')' ':' type '{' func_statement '}'	{ $$ = mknode("FUNCTION", ); }
	| FUNCTION IDENTIFIER '(' ')' ':' type '{' func_statement '}'
>>>>>>> Stashed changes
	;
	
proc    : FUNCTION IDENTIFIER '(' args ')' ':' VOID '{' body '}'
	| FUNCTION IDENTIFIER '(' ')' ':' VOID '{' body '}'
	;

<<<<<<< Updated upstream
type	: INT    {printf("int: %s", yylval.s);} // $0 is INT, type is $$
	| REAL    {printf("%s, %s", yylval.s, yytext);}
	| CHAR    {printf("%s",yytext);}    
	| BOOL    {printf("%s, %s", yylval.s, yytext);}
	| INT_P    {printf("%s, %s", yylval.s, yytext);}
	| REAL_P    {printf("%s, %s", yylval.s, yytext);}
	| CHAR_P    {printf("%s, %s", yylval.s, yytext);}
=======
type	: INT		{}
	| REAL		{}
	| CHAR		{}    
	| BOOL		{}
	| INT_P		{}
	| REAL_P	{}
	| CHAR_P	{}
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
	 
func_prod_list : func
		| func func_prod_list
		| proc
		| proc func_prod_list
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
=======

proc_statement	: 
		| statement proc_statement
		| IF '(' expr ')' '{' proc_codeblock '}' proc_statement
		| WHILE '(' expr ')' '{' proc_codeblock '}' proc_statement 
		| '{' proc_codeblock '}' proc_statement
		| RETURN ';' proc_statement
		;
		
func_statement	: statement func_statement
		| IF '(' expr ')' '{' func_codeblock '}' func_statement
		| WHILE '(' expr ')' '{' func_codeblock '}' func_statement
		| '{' func_codeblock '}' func_statement
		| RETURN expr ';' func_statement
		| RETURN expr ';'
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
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
				
				
=======
proc_codeblock	: codeblock
		| codeblock RETURN ';'
		| proc_codeblock '{' proc_codeblock '}'
		;

func_codeblock	: codeblock
		| codeblock RETURN expr ';'
		| func_codeblock '{' func_codeblock '}'
		;

codeblock	: 
		| VAR varlist ':' type ';' codeblock
		| codeblock expr ';'
		| codeblock IDENTIFIER '=' expr ';'
		;

>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
	;*/
=======
	| IDENTIFIER '(' ')'
	| IDENTIFIER '(' arglist ')'
	;
>>>>>>> Stashed changes

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


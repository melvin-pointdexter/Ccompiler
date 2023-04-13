%{
%}

%token INTEGER REAL BOOL STRING CHARACTER IDENTIFIER KEYWORD

%%

program:
	function program
	|function
	;

function:
	KEYWORD IDENTIFIER '(' args ')' ':' KEYWORD '{' body '}'
	;

args:
	'arg->' args2 ':' KEYWORD ';' args
	|
	;

args2:
	IDENTIFIER ',' args2
	|IDENTIFIER

body:	expr body
	|expr

expr:
	IDENTIFIER	{//unfinished!}

%%

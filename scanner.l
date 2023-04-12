#include "y.tab.h"
%%
[0-9]+\.[0-9]+ {return REAL;}
[0-9]+ {return INTEGER;}
true|false {return BOOL;}
\".*\" {return STRING;}
\'[a-zA-Z0-9]\' {return CHARACTER;}
[a-zA-Z][a-zA-Z0-9_] {return IDENTIFIER;}
%%

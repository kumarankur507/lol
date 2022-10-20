%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "y.tab.h"
%}

%%
"a"		   {return (TOK_A);}
"b"		   {return (TOK_B);}
"c"		   {return (TOK_C);}
"d"		   {return (TOK_D);}
"\n"		   { return (newline); }
.    {fprintf(stderr,"Invalid input\n"); 
        exit(1);}
%%

int yywrap() {return -1;}
%{
#include <stdio.h>
#include <stdlib.h>
int yylex();
int yyerror(char *s);
%}

%token TOK_A TOK_B newline

%%
ST : S newline {printf("Valid\n"); return 1;}
    | error newline {printf("Invalid\n"); return 1;}
    
S : TOK_A S TOK_B | TOK_A TOK_B
  ;
%%

int yyerror(char* s) { return 0; }

int main()
{
    while(1)
      yyparse();
    return 0;
}
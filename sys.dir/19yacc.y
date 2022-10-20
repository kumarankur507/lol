%{
#include <stdio.h>
#include <stdlib.h>
int yylex();
int yyerror(char *s);
%}

%token TOK_A TOK_B TOK_C TOK_D newline

%%

ST : S newline {printf("Valid\n"); return 1;}
   | error newline   {printf("Invalid\n"); return 1;}

S : A TOK_A | TOK_B A TOK_C | TOK_D TOK_C | TOK_B TOK_D TOK_A
A : TOK_D

  ;

%%

int yyerror(char* s) { return 0; }

int main()
{
    while(1)
      yyparse();
    return 0;
}

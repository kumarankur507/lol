%{
#include<ctype.h>
%}
digit [0-9]
operator ("="|"<>"|"<"|"<="|">"|">=")
keyword ("char"|"int"|"long int"|"float"|"double")
separator [;{},=&~]

%%
{keyword}[[:space:]] {
    for(int i=0;i<yytext[i+1]!='\0';i++)
        yytext[i]=toupper(yytext[i]);
    printf("%s",yytext);return 0;}

([+-]?{digit}+)[[:space:]] {   printf("INTEGER",yytext);return 0;}

([+-]?{digit}+\.{digit}*)[[:space:]] {printf("FLOAT",yytext);return 0;}
([+-]?{digit}+\.{digit}*[e][+-]?{digit}+)[[:space:]] {printf("FLOAT",yytext);return 0;}
([+-]?{digit}+[e][+-]?{digit}+)[[:space:]] {printf("FLOAT",yytext);return 0;}
([+-]?{digit}+"/"[+-]?{digit}+)[[:space:]] {printf("FLOAT",yytext);return 0;}

{operator}[[:space:]] {    printf("%s",yytext);return 0;  }

({separator}|"["|"]"|"("|")")[[:space:]] {   printf("%s",yytext);return 0;  }

. { printf("Invalid token\n",yytext);return 0;}
%%

int yywrap() {}
int main() {
yylex();
return 0;
}
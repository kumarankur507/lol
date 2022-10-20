digit [0-9]
letter [a-zA-Z]
iden {letter}+
digits {digit}+
optional_fraction ("."{digits})?
optional_exponent ("E"("+"|"-")?{digits})?
num ({digits}{optional_fraction}{optional_exponent})
relop ("="|"<>"|"<"|"<="|">"|">=")

%%
{iden}[[:space:]] {printf("identifier!\n"); return 0;}
{num}[[:space:]] {printf("number!\n"); return 0;}
{relop}[[:space:]] {printf("relop!\n"); return 0;}
. {printf("not recog!\n"); return 0;}
%%


int yywrap() {}
int main(){
    yylex();
    return 0;
}
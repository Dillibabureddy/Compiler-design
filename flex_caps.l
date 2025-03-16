%{
int count;
%}
%%
[A-Z] { printf("%s is Capital Letter\n",yytext); 
count++;}

. { printf("%s is not Capital Letter\n",yytext);}
\n {return 0;}
%%
int yywrap(){}
int main(){
yylex();
printf("count : %d\n" ,count);
return 0;
}


%{
#include <stdio.h>
%}

%option noyywrap

%%

[ \t\n]+               /* Ignore whitespace */
"if"|"else"|"while"|"return"|"int"|"float" { printf("KEYWORD: %s\n", yytext); }
[0-9]+                { printf("NUMBER: %s\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]* { printf("IDENTIFIER: %s\n", yytext); }
"=="|"!="|"<="|">="|"<"|">" { printf("RELATIONAL OPERATOR: %s\n", yytext); }
"="|"+"|"-"|"*"|"/" { printf("OPERATOR: %s\n", yytext); }
"(" | ")" | "{" | "}" | ";" | "," { printf("SPECIAL SYMBOL: %s\n", yytext); }
.                     { printf("UNKNOWN: %s\n", yytext); }

%%

int main() {
    yylex();
    return 0;
}

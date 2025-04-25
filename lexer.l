%{
#include <stdio.h>
#include <stdlib.h>

int linha = 0;
int coluna = 0;
%}

DIGITO      [0-9]
LETRA       [a-zA-Z]
ID          {LETRA}({LETRA}|{DIGITO})*
NUM         {DIGITO}+

%%
"def"        { printf("DEF\n"); coluna += yyleng; }
"int"        { printf("INT\n"); coluna += yyleng; }
"if"         { printf("IF\n"); coluna += yyleng; }
"else"       { printf("ELSE\n"); coluna += yyleng; }
"print"      { printf("PRINT\n"); coluna += yyleng; }
"return"     { printf("RETURN\n"); coluna += yyleng; }

"<="         { printf("LE\n"); coluna += yyleng; }
">="         { printf("GE\n"); coluna += yyleng; }
"=="         { printf("EQ\n"); coluna += yyleng; }
"!="         { printf("NE\n"); coluna += yyleng; }
"<"          { printf("LT\n"); coluna += yyleng; }
">"          { printf("GT\n"); coluna += yyleng; }

"+"          { printf("PLUS\n"); coluna += yyleng; }
"-"          { printf("MINUS\n"); coluna += yyleng; }
"*"          { printf("TIMES\n"); coluna += yyleng; }
"/"          { printf("DIV\n"); coluna += yyleng; }
"="          { printf("ASSIGN\n"); coluna += yyleng; }
"("          { printf("LPAREN\n"); coluna += yyleng; }
")"          { printf("RPAREN\n"); coluna += yyleng; }
"{"          { printf("LBRACE\n"); coluna += yyleng; }
"}"          { printf("RBRACE\n"); coluna += yyleng; }
","          { printf("COMMA\n"); coluna += yyleng; }
";"          { printf("SEMICOLON\n"); coluna += yyleng; }

{ID}          { printf("ID(%s)\n", yytext); coluna += yyleng; }
{NUM}         { printf("NUM(%s)\n", yytext); coluna += yyleng; }

[ \t]+        { coluna += yyleng; }
\n            { linha++; coluna = 1; }
.             { printf("Erro lexico em linha %d, coluna %d: %s\n", linha, coluna, yytext); coluna += yyleng; }

%%

int main(int argc, char **argv) {
    ++argv, --argc;
    if (argc > 0) {
        yyin = fopen(argv[0], "r");
        if (!yyin) {
            perror(argv[0]);
            exit(1);
        }
    }
    else {
        yyin = stdin;
    }

    yylex();
    return 0;
}

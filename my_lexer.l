%{
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"

char currentID[32];
char currentValue[32]; //int,char,double,bool
%}

%option yylineno

LETTER              [a-zA-Z]
DIGIT               [0-9]
INTEGER             [0-9]+
WHITESPACE          [ \t\r\f\v]+
IDENTIFIER          (_|{LETTER})({LETTER}|{DIGIT}|_)*
DOUBLE              (([0-9]*\.[0-9]+)|([0-9]+\.[0-9]*))
EXP                 ([eE][-+]?{INTEGER})
REAL                ({INTEGER}{EXP}|{DOUBLE}{EXP}?)


%%

[[\-+*/<>}:|&!^%\]]     { return *yytext; }
">="                    { return  GE;}
"<="                    { return  LE;}
"=="                    { return  EQ;}
"!="                    { return  NE;}
"+="                    { return ADD_EQ;}
"-="                    { return SUB_EQ;}
"*="                    { return MULT_EQ;}
"/="                    { return DIV_EQ;}
"="                     { return ASSIGN_OP;}
"{"                     { return LEFT_BRACE;}
"("                     { return LEFT_ROUND;}
")"                     { return RIGHT_ROUND;}

    /* Reserved Words */
"for"                   { return  FOR;}
"while"                 { return  WHILE;}
"continue"              { return  CONTINUE;}
"break"                 { return  BREAK;}
"return"                { return  RETURN;}
"if"                    { return  IF;}
"else"                  { return  ELSE;}
"printf"                { return  PRINT;}

";"                     { return SEMICOLON;}
","                     { return COMMA;}

    /* DataTypes and likes */
"void"                  { return  VOID;}
"int"                   { return INT_TYPE;}
"double"                { return DOUBLE_TYPE;}
"char"                  { return CHAR_TYPE;}
"string"                { return STRING_TYPE;}
"bool"                  { return BOOL_TYPE;}

    /* Values of Types */
"true"                      {
                                yylval.bVal = true;
                                return BOOL;
                            }
"false"                     {  
                                yylval.bVal = false;
                                return BOOL;
                            }
{IDENTIFIER}                {
                                yylval.idName = strdup(yytext);
                                strcpy(currentID, yylval.idName);
                                return IDENTIFIER;
                            }
(\'.\')                     {
                                yylval.cVal = yytext[1];
                                strcpy(currentValue, yytext);
                                return CHAR; }
{INTEGER}                   {
                                yylval.iVal = atoi(yytext);
                                return INTEGER;
                            }
{REAL}                      {
                                strcpy(currentValue, yytext);
                                yylval.dVal = atof(yytext);
                                return DOUBLE;
                            }
{WHITESPACE}                 ;
\"[^\"\n]*\"                {
                                return STRING;
                            }
\n                          ;
.                           //{printf("Unrecognized token\n");}

%%

int yywrap(void){
    return 1;
}
%{
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "lex.yy.c"

int yylex(void);
int yyerror(char *s);

//outputting logs:
std::ofstream myfile;
%} 
%token INT_TYPE 
%token DOUBLE_TYPE 
%token BOOL_TYPE 
%token STRING_TYPE 
%token VOID_TYPE
%token IDENTIFIER
%token IF
%token PRINT
%token FOR
%token WHILE
%token BREAK
%token RETURN
%token INTEGER
%token DOUBLE
%token BOOL
%token STRING

// https://docs.microsoft.com/en-us/cpp/c-language/precedence-and-order-of-evaluation?view=msvc-170#precedence-and-associativity-of-c-operators
%nonassoc PRECELSE
%nonassoc ELSE
%left     ','
%right    '='
%left     OR
%left     AND	
%left     EQUAL NOTEQUAL
%left     '<' '>' LTE GTE
%left     '+' '-' 
%left     '*' '/' '%'
%right    UMINUS '!'
%left INCREMENT DECREMENT
%%

Program     : DeclList
            ;

DeclList    : DeclList Decl
            | Decl
            ;

Decl    : AssignExpr ';'
        | DeclarationStmt
        | FuncDecl
        ;

VarType : INT_TYPE 
        | DOUBLE_TYPE
        | BOOL_TYPE
        | STRING_TYPE
        ;

FuncDecl    : VarType IDENTIFIER '(' Params ')' StmtBlock
            | VOID_TYPE IDENTIFIER '(' Params ')' StmtBlock
            ;

Params  : Params ',' VarType IDENTIFIER
        | VarType IDENTIFIER
        |
        ;

StmtBlock   : '{' Statements '}'
            | '{' '}' //empty body
            ;

Statements  : Statements Statement
            | Statement
            ;

Statement   : Expression ';'
            | DeclarationStmt
            | IfStmt
            | WhileStmt
            | ForStmt
            | BreakStmt
            | ReturnStmt
            | PrintStmt
            ;

DeclarationStmt : VarType IDENTIFIER ';'
                ;

IfStmt  : IF '(' Expression ')' StmtBlock ELSE StmtBlock
        | IF '(' Expression ')' StmtBlock %prec PRECELSE
        ;

WhileStmt   : WHILE '(' Expression ')' StmtBlock
            ;

ForStmt : FOR '(' AssignExpr ';' Expression ';' Expression ')' StmtBlock
        ;

BreakStmt   : BREAK ';'
            ;

ReturnStmt  : RETURN ';'
            | RETURN Expression ';'
            ;

PrintArgs   : IDENTIFIER ',' PrintArgs
            | IDENTIFIER
            |
            ; 

PrintStmt   : PRINT '(' STRING ',' PrintArgs ')' ';'
            | PRINT '(' IDENTIFIER ')' ';' 
            | PRINT '(' STRING ')' ';'
            ;

Expression  : AssignExpr
            | Constant 
            | IDENTIFIER
            | FuncInvoke
            | '(' Expression ')'
            | ArithmeticExpr
            | EqualityExpr
            | RelationalExpr
            | LogicalExpr
            | PostfixExpr
            ;

AssignExpr  : IDENTIFIER '=' Expression
            | VarType IDENTIFIER '=' Expression
            ;


Constant    : INTEGER
            | DOUBLE
            | BOOL
            | STRING
            ;

FuncInvoke  : IDENTIFIER '(' Args ')' 
            ;

Args    : Args ',' Expression
        | Expression
        |
        ;

ArithmeticExpr  : Expression '+' Expression 
                | Expression '-' Expression 
                | Expression '*' Expression 
                | Expression '/' Expression 
                | Expression '%' Expression 
                | '-' Expression %prec UMINUS 
                ;

EqualityExpr    : Expression EQUAL Expression
                | Expression NOTEQUAL Expression
                ;

RelationalExpr  : Expression '<' Expression
                | Expression '>' Expression
                | Expression LTE Expression
                | Expression GTE Expression
                ;

LogicalExpr : Expression AND Expression
            | Expression OR Expression
            | '!' Expression
            ;

PostfixExpr : IDENTIFIER INCREMENT 
            | IDENTIFIER DECREMENT 
            ;
%%

int main(int argc, char *argv[]){
    printf("\n\n----------- Simple Programming Language Compiler ----------- \n\n");
    yyin = fopen(argv[1], "r");
    //myfile.open("log.txt");
    yyparse();
    //myfile.close();
    fclose(yyin);
    return 0;
}
int yyerror(char *s) {
    printf("Error in line: %d, with message %s at token: %s\n", yylineno, s, yytext);
    myfile << "Error in line: "<< yylineno << ", with message "<< s << " at token: "<<yytext<<"\n";
}
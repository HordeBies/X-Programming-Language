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

%union {
    int iVal;
    char cVal;
    char *idName;
    char *sVal;
    double dVal;
    bool bVal;
    char* op;
    char value[32]; //general value for int, double, bool, char 
}

//Data Types
%token INT_TYPE
%token DOUBLE_TYPE
%token CHAR_TYPE
%token STRING_TYPE
%token BOOL_TYPE
%token VOID

// Reserved Keywords
%token SEMICOLON
%token LEFT_BRACE
%token IF
%token ELSE
%token FOR
%token WHILE
%token BREAK
%token CONTINUE
%token RETURN
%token PRINT
%token STRING

// Operators
%token EQ
%token NE
%token GE
%token LE


%token<idName> IDENTIFIER
%token<iVal> INTEGER 
%token<dVal> DOUBLE 
%token<cVal> CHAR 
%token<bVal> BOOL

%token          LEFT_ROUND RIGHT_ROUND //NODE
%token          ADD_EQ SUB_EQ MULT_EQ DIV_EQ //NODE

//Order of operations
%right<op>      ASSIGN_OP
%left       COMMA
%left       LOGIC_OR
%left       LOGIC_AND
%left       EQ NE
%left       LE GE '<' '>'
%left       '-' '+'
%left       '*' '/' '%'
%right      '!'


%nonassoc DECLAR
%nonassoc PRECEED_ELSE
%nonassoc ELSE

%%
//Base
start   : start line_statement  {printf("start found\n");}
        | line_statement        {printf("start found\n");}
        ;
line_statement  : function      {printf("line_statemenet found\n");}
                | declaration   {printf("line_statement found\n");}
                ;
//Function declarations
function_base   : data_type identifier  {printf("function_base found\n");}
                ;

function    : function_base LEFT_ROUND argument_list RIGHT_ROUND scope_statement    {printf("function found\n");}
            | function_base LEFT_ROUND argument_list RIGHT_ROUND SEMICOLON          {printf("function found\n");}
            ;
//Arguments
argument_list   : arguments {printf("argument_list found\n");}
                |
                ;
arguments   : arguments COMMA argument  {printf("arguments found\n");}
            | argument                  {printf("arguments found\n");}
            ;

argument    : data_type identifier      {printf("argument found\n");}
            ;
//Statements
scope_statement : LEFT_BRACE statements '}' {printf("scope_statement found\n");}
                ;
statements  : statements statement  {printf("statements found\n");}
            |
            ;
statement   : scope_statement  {printf("atomic_statement found\n");}
            | atomic_statement   {printf("scope_statement found\n");}
            ;
atomic_statement    : if_block                          {printf("atomic_statement found\n");}
                    | while_block                       {printf("atomic_statement found\n");}
                    | for_block                         {printf("atomic_statement found\n");}
                    | declaration                       {printf("atomic_statement found\n");}
                    | print                             {printf("atomic_statement found\n");}
                    | function_invoke SEMICOLON             {printf("atomic_statement found\n");}
                    | RETURN SEMICOLON                  {printf("atomic_statement found\n");}
                    | CONTINUE SEMICOLON                {printf("atomic_statement found\n");}
                    | BREAK SEMICOLON                   {printf("atomic_statement found\n");}
                    | RETURN sub_expression SEMICOLON   {printf("atomic_statement found\n");}
                    ;
//Declarations
declaration : data_type assign_expression SEMICOLON %prec DECLAR    {printf("Declaration found\n");}
            | data_type IDENTIFIER SEMICOLON                        {printf("Declaration found\n");}
            | assign_expression SEMICOLON                           {printf("Declaration found\n");}
            ;
//If Block
if_block    : if_statement %prec PRECEED_ELSE   {printf("if_block found\n");}
            | if_statement ELSE statement       {printf("if_block found\n");}
            ;

if_statement    : IF LEFT_ROUND expression RIGHT_ROUND statement    {printf("if_statement found\n");}
                ;
//For Block
for_block   : FOR LEFT_ROUND for_init for_middle for_end RIGHT_ROUND statement  {printf("for_block found\n");}
            ;

for_init    : declaration   {printf("for_init found\n");}
            | SEMICOLON     {printf("for_init found\n");}
            ;

for_middle  : expression    {printf("for_middle found\n");}
            | SEMICOLON     {printf("for_middle found\n");}
            ;

for_end     : sub_expression    {printf("for_end found\n");}
            |
            ;
//While Block
while_block : WHILE LEFT_ROUND expression RIGHT_ROUND statement {printf("while_block found\n");}
            ;
//Expressions
expression      : expression COMMA sub_expression   {printf("expression found\n");}
                | sub_expression                    {printf("expression found\n");}
                ;

sub_expression  : arithmetic_expression {printf("sub_expression found\n");}
                | assign_expression     {printf("sub_expression found\n");}
                ;

assign_expression   : identifier assign_operation arithmetic_expression {printf("assign_expression found\n");}
                    ;
//Invoking Functions with parameters
function_invoke : identifier LEFT_ROUND parameter_list RIGHT_ROUND    {printf("function_invoke found\n");}
                | identifier LEFT_ROUND RIGHT_ROUND                   {printf("function_invoke found\n");}
                ;
parameter_list  : parameter_list COMMA parameter    {printf("parameter_list found\n");}
                | parameter                         {printf("parameter_list found\n");}
                ;
parameter       : sub_expression        {printf("parameter found\n");}
                ;
//Arithmetic operations
arithmetic_expression   : arithmetic_expression '+' arithmetic_expression       {printf("arithmetic_expression found\n");}
                        | arithmetic_expression '-' arithmetic_expression       {printf("arithmetic_expression found\n");}
                        | arithmetic_expression '*' arithmetic_expression       {printf("arithmetic_expression found\n");}
                        | arithmetic_expression '/' arithmetic_expression       {printf("arithmetic_expression found\n");}
                        | arithmetic_expression '%' arithmetic_expression       {printf("arithmetic_expression found\n");}
                        | arithmetic_expression LOGIC_AND arithmetic_expression {printf("arithmetic_expression found\n");}
                        | arithmetic_expression LOGIC_OR arithmetic_expression  {printf("arithmetic_expression found\n");}
                        | arithmetic_expression '<' arithmetic_expression       {printf("arithmetic_expression found\n");}
                        | arithmetic_expression '>' arithmetic_expression       {printf("arithmetic_expression found\n");}
                        | arithmetic_expression EQ arithmetic_expression        {printf("arithmetic_expression found\n");}
                        | arithmetic_expression NE arithmetic_expression        {printf("arithmetic_expression found\n");}
                        | arithmetic_expression LE arithmetic_expression        {printf("arithmetic_expression found\n");}
                        | arithmetic_expression GE arithmetic_expression        {printf("arithmetic_expression found\n");}
                        | '!' arithmetic_expression                             {printf("arithmetic_expression found\n");}
                        | LEFT_ROUND arithmetic_expression RIGHT_ROUND          {printf("arithmetic_expression found\n");}
                        | '-'arithmetic_expression                              {printf("arithmetic_expression found\n");}
                        | identifier                                            {printf("arithmetic_expression found\n");}
                        | primitive_constant                                    {printf("arithmetic_expression found\n");}
                        | function_invoke
                        ;
//Token to Data
identifier  : IDENTIFIER    {printf("identifier found\n");}
            ;
data_type   : INT_TYPE          {printf("data type found\n");}
            | DOUBLE_TYPE       {printf("data type found\n");}
            | BOOL_TYPE         {printf("data type found\n");}
            | CHAR_TYPE         {printf("data type found\n");}
            | STRING_TYPE       {printf("data type found\n");}
            | VOID              {printf("data type found\n");}
            ;
primitive_constant : INTEGER        {printf("primitive_constant found\n");}
                    | CHAR           {printf("primitive_constant found\n");}
                    | DOUBLE         {printf("primitive_constant found\n");}
                    | STRING         {printf("primitive_constant found\n");}
                    | BOOL           {printf("primitive_constant found\n");}
                    ;
assign_operation    : ASSIGN_OP {printf("assign_operation found\n");}
                    | ADD_EQ    {printf("assign_operation found\n");}
                    | SUB_EQ    {printf("assign_operation found\n");}
                    | MULT_EQ   {printf("assign_operation found\n");}
                    | DIV_EQ    {printf("assign_operation found\n");}
//Print                 ;
print   : PRINT LEFT_ROUND print_body RIGHT_ROUND SEMICOLON {printf("print found\n");}
        ;
print_body  : print_body COMMA print_expression {printf("print_body found\n");}
            | print_expression                  {printf("print_body found\n");}
            ;
print_expression    : STRING        {printf("print_expression found\n");}
                    | IDENTIFIER    {printf("print_expression found\n");}
                    ;
%%
int main(int argc, char *argv[]){
    printf("\n\n----------- Simple Programming Language Compiler ----------- \n\n");
    yyin = fopen(argv[1], "r");
    myfile.open("log.txt");
    yyparse();
    myfile.close();
    fclose(yyin);
    return 0;
}
int yyerror(char *s) {
    printf("Error in line: %d, with message %s at token: %s\n", yylineno, s, yytext);
    myfile << "Error in line: "<< yylineno << ", with message "<< s << " at token: "<<yytext<<"\n";
}
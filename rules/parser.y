%{
#include "../parse_tree/parse_tree.h"
#include <fstream>

int yylex(void);
int yyerror(char *s);
extern char* yytext;
extern int yylineno;

//outputting logs:
std::ofstream myfile;
%} 

%union {
    //Program *program;
    Node *node;
    int integerConstant;
    bool boolConstant;
    const char *stringConstant;
    double doubleConstant;
    //char identifier[MaxIdentLen+1]; // +1 for terminating null


    //Decl *decl;
    // VarDecl *vardecl;
    // FnDecl *fndecl;
    // ClassDecl *classdecl;
    // InterfaceDecl *interfacedecl;  
    
    //Type *simpletype;
    // NamedType *namedtype;
    // ArrayType *arraytype;
    
    // List<NamedType*> *implements;
    // List<Decl*> *declList;
    // List<VarDecl*> *vardecls;

   
    // StmtBlock *stmtblock;
    // Stmt *stmt;
    // IfStmt *ifstmt;
    // ForStmt *forstmt;
    // WhileStmt *whilestmt;
    // ReturnStmt *rtnstmt;	
    // BreakStmt *brkstmt;
    // SwitchStmt *switchstmt;
    // CaseStmt *casestmt;
    // DefaultStmt *defaultstmt;
    // PrintStmt *pntstmt;
    // List<Stmt*> *stmts;
    // List<CaseStmt*> *casestmts;
    
    // Expr *expr;
    // Expr *optexpr;
    // List<Expr*> *exprs;
    // Call *call;
    
    // IntConstant *intconst;
    // DoubleConstant *doubleconst;
    // BoolConstant *boolconst;
    // StringConstant *stringconst;
    // NullConstant *nullconst;
    
    // ArithmeticExpr *arithmeticexpr;
    // RelationalExpr *relationalexpr;
    // EqualityExpr   *equalityexpr;
    // LogicalExpr    *logicalexpr;
    // AssignExpr     *assignexpr;
    // PostfixExpr    *postfixexpr;
    
    // LValue *lvalue;
    // FieldAccess *fieldaccess;
    // ArrayAccess *arrayaccess;
}

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
%type<node>     VarType
/* %type <program>       Program
%type <declList>      DeclList
%type <decl>          Decl
%type <vardecl>       VarDecl
%type <fndecl>        FnDecl
%type <classdecl>     ClassDecl
%type <interfacedecl> InterfaceDecl*//*
%type <simpletype>    VarType
%type <namedtype>     NamedType
%type <arraytype>     ArrayType
%type <vardecls>      Formals
%type <vardecls>      Variables
%type <implements>    Implements
%type <implements>    Impl
%type <namedtype>     Extend
%type <decl>	      Field
%type <declList>      Fields
%type <decl>	      Prototype
%type <declList>      Prototypes
%type <vardecls>      VarDecls
%type <stmt>          Stmt
%type <stmts>         Stmts
%type <stmtblock>     StmtBlock
%type <ifstmt>        IfStmt
%type <whilestmt>     WhileStmt
%type <forstmt>	      ForStmt
%type <rtnstmt>       ReturnStmt
%type <brkstmt>	      BreakStmt
%type <switchstmt>    SwitchStmt
%type <casestmts>     Cases
%type <casestmt>      Case
%type <defaultstmt>   Default
%type <pntstmt>	      PrintStmt
%type <expr>          Expr
%type <expr>          OptExpr
%type <exprs>         Exprs
%type <exprs>	      Actuals
%type <expr>	      Constant
%type <intconst>      IntConstant 
%type <boolconst>     BoolConstant
%type <stringconst>   StringConstant
%type <doubleconst>   DoubleConstant
%type <nullconst>     NullConstant
%type <call>          Call
%type <arithmeticexpr> ArithmeticExpr
%type <relationalexpr> RelationalExpr
%type <equalityexpr>   EqualityExpr
%type <logicalexpr>    LogicalExpr
%type <assignexpr>     AssignExpr
%type <postfixexpr>    PostfixExpr
%type <lvalue>        LValue
%type <fieldaccess>   FieldAccess
%type <arrayaccess>   ArrayAccess */

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

Program     : DeclList {@1;}
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
            | '{' '}'
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
ForEnd 	: Expression
		| PostfixExpr
		;

ForStmt : FOR '(' AssignExpr ';' Expression ';' ForEnd ')' StmtBlock 
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

int yyerror(char *s) {
    printf("Error in line: %d, with message %s at token: %s\n", yylineno, s, yytext);
    myfile << "Error in line: "<< yylineno << ", with message "<< s << " at token: "<<yytext<<"\n";
}
%{
#include "../parse_tree/parse_tree.h"

int yylex(void);
int yyerror(char *s);
extern char* yytext;
extern int yylineno;

StatementNode* programRoot = NULL;
%} 

%union {
    ScopeNode* scopeNode;
    StatementNode* stmtNode;
    VarDeclarationNode* varDeclNode;
    IfNode* ifNode;
    WhileNode* whileNode;
    ForNode* forNode;
    FunctionNode* functionNode;
    FunctionInvokeNode* funcInvokeNode;
    ReturnNode* returnNode;
	PrintNode* printNode;
    ExpressionNode* exprNode;
    TypeNode* typeNode;
    ValueNode* valueNode;
    IdentifierNode* identifierNode;
    StmtList* stmtList;
    ExprList* exprList;
    VarList* varList;

    Token token;
    Location location;
}

// TERMINALS
%token<location> INT_TYPE 
%token<location> DOUBLE_TYPE 
%token<location> BOOL_TYPE 
%token<location> STRING_TYPE 
%token<location> VOID_TYPE

%token<location> OR
%token<location> AND
%token<location> EQUAL
%token<location> NOTEQUAL
%token<location> LTE
%token<location> GTE
%token<location> INCREMENT
%token<location> DECREMENT

%token<location> IF
%token<location> ELSE
%token<location> PRINT
%token<location> FOR
%token<location> WHILE
%token<location> BREAK
%token<location> RETURN

%token<token> IDENTIFIER
%token<token> INTEGER
%token<token> DOUBLE
%token<token> BOOL
%token<token> STRING

// NON-TERMINALS

%type<scopeNode>           	Program StmtBlock
%type<stmtNode>            	Statement Decl ForStart
%type<stmtList>            	Statements DeclList
%type<varDeclNode>         	VarDecl
%type<ifNode>              	IfStmt
%type<whileNode>           	WhileStmt
%type<forNode>             	ForStmt
%type<functionNode>        	FuncDecl
%type<funcInvokeNode>    	FuncInvoke
%type<returnNode>      		ReturnStmt
%type<printNode>			PrintStmt
%type<varList>             	Params
%type<exprList>            	Args 
%type<exprNode>            	Expression AssignExpr ArithmeticExpr EqualityExpr RelationalExpr LogicalExpr ForEnd PostfixExpr 
%type<typeNode>            	VarType
%type<valueNode>           	Constant
%type<identifierNode>      	Identifier

%type <location>            '-' '+' '*' '/' '%' '!' '<' '>' '=' '(' ')' '{' '}' ',' ';'  //'

// Precedence
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

Program     : DeclList 								{
														@1; 
														$$ = NULL;

														programRoot = new ScopeNode((*$1)[0]->loc, *$1);
													}
            ;

DeclList    : DeclList Decl							{ $$ = $1; $$->push_back($2); }
            | Decl									{ $$ = new StmtList(); $$->push_back($1); }
            ;

Decl    : VarDecl ';'								{ $$ = $1;}
        | FuncDecl									{ $$ = $1;}
        ;

VarType : INT_TYPE 	 								{ $$ = new TypeNode($1,TYPE_INT); }
        | DOUBLE_TYPE 								{ $$ = new TypeNode($1,TYPE_DOUBLE); }
        | BOOL_TYPE	 								{ $$ = new TypeNode($1,TYPE_BOOL); }
        | STRING_TYPE 								{ $$ = new TypeNode($1,TYPE_STRING); }
        ;

FuncDecl    : VarType Identifier '(' Params ')' StmtBlock		{ $$ = new FunctionNode($1, $2, *$4, $6); }
            | VOID_TYPE Identifier '(' Params ')' StmtBlock		{ $$ = new FunctionNode(new TypeNode($1, TYPE_VOID), $2, *$4, $6); }
            ;

Params  : Params ',' VarType Identifier				{ $$ = $1; $$->push_back(new VarDeclarationNode($3, $4)); }
        | VarType Identifier						{ $$ = new VarList(); $$->push_back(new VarDeclarationNode($1, $2)); }
        |											{ $$ = new VarList(); }
        ;

StmtBlock   : '{' Statements '}'					{ $$ = new ScopeNode($1, *$2); }
            | '{' '}'								{ $$ = new ScopeNode($1); }
            ;

Statements  : Statements Statement					{ $$ = $1; $$->push_back($2); }
            | Statement								{ $$ = new StmtList(); $$->push_back($1); }
            ;

Statement   : Expression ';'						{ $$ = new ExprContainerNode($1->loc, $1); }
            | VarDecl ';'							{ $$ = $1; }
            | IfStmt								{ $$ = $1; }
            | WhileStmt								{ $$ = $1; }
            | ForStmt								{ $$ = $1; }
            | BREAK ';'								{ $$ = new BreakStmtNode($1); }
            | ReturnStmt							{ $$ = $1; }
            | PrintStmt								{ $$ = $1; }
            ;

VarDecl : VarType Identifier						{ $$ = new VarDeclarationNode($1, $2); }
		| VarType Identifier '=' Expression 		{ $$ = new VarDeclarationNode($1, $2, $4); }
		;

IfStmt  : IF '(' Expression ')' StmtBlock ELSE StmtBlock	{ $$ = new IfNode($1, $3, $5, $7); }
        | IF '(' Expression ')' StmtBlock %prec PRECELSE	{ $$ = new IfNode($1, $3, $5); }
        ;

WhileStmt   : WHILE '(' Expression ')' StmtBlock	{ $$ = new WhileNode($1, $3, $5); }
            ;

ForEnd 	: Expression								
        | PostfixExpr								
        ;

ForStart	: AssignExpr							{ $$ = $1; }
			| VarDecl								{ $$ = $1; }
			;

ForStmt : FOR '(' ForStart ';' Expression ';' ForEnd ')' StmtBlock	{ $$ = new ForNode($1, $3, $5, $7, $9); }
        ;

ReturnStmt  : RETURN ';'							{ $$ = new ReturnNode($1,NULL); }
            | RETURN Expression ';'					{ $$ = new ReturnNode($1,$2); }
            ;

PrintStmt   : PRINT '(' Identifier ')' ';' 				{$$ = new PrintNode($1, "", $3);}
            | PRINT '(' STRING ')' ';'					{$$ = new PrintNode($1, $3.value, NULL);}
			| PRINT '(' STRING ',' Identifier ')' ';'	{$$ = new PrintNode($1, $3.value, $5);}
            ;

Expression  : AssignExpr							{ $$ = $1; }
            | Constant 								{ $$ = $1; }
            | Identifier							{ $$ = $1; }
            | FuncInvoke							{ $$ = $1; }
            | '(' Expression ')'					{ $$ = new ExprContainerNode($1, $2); }
            | ArithmeticExpr
            | EqualityExpr
            | RelationalExpr
            | LogicalExpr
            ;

AssignExpr  : Identifier '=' Expression				{ $$ = new AssignOprNode($2, $1, $3); }
            ;

Constant    : INTEGER								{ $$ = new ValueNode($1.loc, TYPE_INT, $1.value); }
            | DOUBLE								{ $$ = new ValueNode($1.loc, TYPE_DOUBLE, $1.value); }
            | BOOL									{ $$ = new ValueNode($1.loc, TYPE_BOOL, $1.value); }
            | STRING								{ $$ = new ValueNode($1.loc, TYPE_STRING, $1.value); }
            ;

FuncInvoke  : Identifier '(' Args ')' 				{ $$ = new FunctionInvokeNode($1, *$3); }
            ;

Args    : Args ',' Expression						{ $$ = $1; $$->push_back($3); }
        | Expression								{ $$ = new ExprList(); $$->push_back($1); }
        |											{ $$ = new ExprList(); }
        ;

ArithmeticExpr  : Expression '+' Expression 		{ $$ = new BinaryOprNode($2, OP_ADD, $1, $3); }
                | Expression '-' Expression 		{ $$ = new BinaryOprNode($2, OP_SUB, $1, $3); }
                | Expression '*' Expression 		{ $$ = new BinaryOprNode($2, OP_MUL, $1, $3); }
                | Expression '/' Expression 		{ $$ = new BinaryOprNode($2, OP_DIV, $1, $3); }
                | Expression '%' Expression 		{ $$ = new BinaryOprNode($2, OP_MOD, $1, $3); }
                | '-' Expression %prec UMINUS 		{ $$ = new UnaryOprNode($1, OP_MIN, $2); }
                ;

EqualityExpr    : Expression EQUAL Expression		{ $$ = new BinaryOprNode($2, OP_EQ, $1, $3); }
                | Expression NOTEQUAL Expression	{ $$ = new BinaryOprNode($2, OP_NEQ, $1, $3); }
                ;

RelationalExpr  : Expression '<' Expression			{ $$ = new BinaryOprNode($2, OP_G, $1, $3); }
                | Expression '>' Expression			{ $$ = new BinaryOprNode($2, OP_L, $1, $3); }
                | Expression LTE Expression			{ $$ = new BinaryOprNode($2, OP_LE, $1, $3); }
                | Expression GTE Expression			{ $$ = new BinaryOprNode($2, OP_GE, $1, $3); }
                ;

LogicalExpr : Expression AND Expression				{ $$ = new BinaryOprNode($2, OP_AND, $1, $3); }
            | Expression OR Expression				{ $$ = new BinaryOprNode($2, OP_OR, $1, $3); }
            | '!' Expression						{ $$ = new UnaryOprNode($1, OP_NOT, $2); }
            ;

PostfixExpr : Identifier INCREMENT 					{ $$ = new UnaryOprNode($2, OP_INC, $1); }
            | Identifier DECREMENT 					{ $$ = new UnaryOprNode($2, OP_DEC, $1); }
            ;

Identifier	: IDENTIFIER							{ $$ = new IdentifierNode($1.loc, $1.value); }
			;
%%

int yyerror(char *s) {
    printf("Error in line: %d, with message %s at token '%s' while parsing\n", yylineno, s, yytext);
}
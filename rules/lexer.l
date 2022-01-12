
%{
#include "../parse_tree/parse_tree.h"
#include "parser.hpp"

void saveLocation();
void saveToken();

static void DoBeforeEachAction();
#define YY_USER_ACTION DoBeforeEachAction();
#define MOVE_CURSOR      (curLoc.pos += yyleng)
int colno;
Location curLoc = {1,0,0};
// List<const char*> savedlines;
%}

PUNCTUATION        ([!:;,.[\]{}()])
ARITHMETIC         ([-+*/%])
RELATIONAL         ([<>=])
OPERATOR           ({ARITHMETIC}|{RELATIONAL})
DECIMAL            ([0-9]+)
BEG_STRING         (\"[^"\n]*)
STRING             ({BEG_STRING}\")
BOOLEAN            (true|false)  
FLOAT              ([0-9]*\.{DECIMAL})
IDENTIFIER         ([a-zA-Z][a-zA-Z0-9_]*)
SINGLE_COMMENT     ("//"[^\n]*)

%%            

 /* recognize all keywords and return the correct token */
"void"                   { saveLocation();return VOID_TYPE; }
"int"                    { saveLocation();return INT_TYPE; }
"double"                 { saveLocation();return DOUBLE_TYPE; }
"bool"                   { saveLocation();return BOOL_TYPE; }
"string"                 { saveLocation();return STRING_TYPE; }
"for"                    { saveLocation();return FOR; }
"while"                  { saveLocation();return WHILE; }
"if"                     { saveLocation();return IF; }
"else"                   { saveLocation();return ELSE; }
"return"                 { saveLocation();return RETURN; }
"break"                  { saveLocation();return BREAK; }
"printf"                 { saveLocation();return PRINT; }

 /* recognize punctuation and single-char operators */
{PUNCTUATION} | 
{OPERATOR} 				{ saveLocation();return yytext[0]; }

 /* recognize two-character operators and return the correct token */
"<="					          { return LTE; }
">="                    { saveLocation();return GTE; }
"=="                    { saveLocation();return EQUAL; }
"!="                    { saveLocation();return NOTEQUAL; }
"&&"                    { saveLocation();return AND; }
"||"                    { saveLocation();return OR; }
"++"                    { saveLocation();return INCREMENT; }
"--"                    { saveLocation();return DECREMENT; }

 /* recognize int, double, bool and string constants,
  * return the correct token
  * and set appropriate filed of yylval 
  */
{STRING}                { 
                           	saveToken();
                           	return STRING;
                        }

{BOOLEAN}               { 
							saveToken();
                           	return BOOL;
                        }

{DECIMAL}               { 
                           	saveToken(); 
                           	return INTEGER; 
						}

{FLOAT}                 {
							saveToken(); 
                        	return DOUBLE; 
						}

{IDENTIFIER}            {
							saveToken(); 
                        	return IDENTIFIER;
		        		}

 /* consume single-line comment */
{SINGLE_COMMENT}        ;
 /* count line */
\n                    	{curLoc.lineNum++; curLoc.pos = 0; yylineno++;}
\t						          {curLoc.pos += 4;}
" "                     {curLoc.pos++;}
 /* all other characters are reported as errors */
.                       {
                        //ReportError::UnrecogChar(&yylloc, yytext[0]);
                        }

%%

void saveLocation() {
    curLoc.len = yyleng;
    yylval.location = curLoc;
    yylval.location.pos++;

    MOVE_CURSOR;
}

void saveToken() {
    curLoc.len = yyleng;

    yylval.token.value = strdup(yytext);
    yylval.token.loc = curLoc;
    yylval.token.loc.pos++;

    MOVE_CURSOR;
}

static void DoBeforeEachAction()
{
  /* yylloc.first_line = yylloc.last_line = yylineno;
  //printf("%s",yytext);
  yylloc.first_column = colno;
  yylloc.last_column = colno + yyleng - 1;
  colno = colno + yyleng; */
}

int yywrap(void){
    return 1;
}
%{

#include "../parse_tree/parse_tree.h"
#include "parser.hpp"

static void DoBeforeEachAction();
#define YY_USER_ACTION DoBeforeEachAction();

#define TAB_SIZE 8

int colno;
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

%option stack

%%            

 /* recognize all keywords and return the correct token */
"void"                   { return VOID_TYPE; }
"int"                    { return INT_TYPE; }
"double"                 { return DOUBLE_TYPE; }
"bool"                   { return BOOL_TYPE; }
"string"                 { return STRING_TYPE; }
"for"                    { return FOR; }
"while"                  { return WHILE; }
"if"                     { return IF; }
"else"                   { return ELSE; }
"return"                 { return RETURN; }
"break"                  { return BREAK; }
"printf"                 { return PRINT; }

 /* recognize punctuation and single-char operators */
{PUNCTUATION} | 
{OPERATOR} 				{ return yytext[0]; }

 /* recognize two-character operators and return the correct token */
"<="					          { return LTE; }
">="                    { return GTE; }
"=="                    { return EQUAL; }
"!="                    { return NOTEQUAL; }
"&&"                    { return AND; }
"||"                    { return OR; }
"++"                    { return INCREMENT; }
"--"                    { return DECREMENT; }

 /* recognize int, double, bool and string constants,
  * return the correct token
  * and set appropriate filed of yylval 
  */
{STRING}                { 
                           	//yylval.stringConstant = strdup(yytext);
                           	return STRING;
                        }
{BEG_STRING}            {
                          	//ReportError::UntermString(&yylloc, yytext);       
                        }
{BOOLEAN}               { 
                //            if (strcmp("true", yytext) == 0)
                //                yylval.boolConstant = true;
                //            else
			    //    yylval.boolConstant = false;
                           	return BOOL;
                        }
{DECIMAL}               { 
                           	//yylval.integerConstant = strtol(yytext, NULL, 10); 
                           	return INTEGER; 
			 			            }
{FLOAT}                 { 
                           	//yylval.doubleConstant = atof(yytext); 
                        	return DOUBLE; 
			 			            }

 /* recognize identifiers, 
  * return the correct token and set appropriate fields of yylval 
  */
{IDENTIFIER}            { 
            //               if (yyleng > MaxIdentLen)
        	//             ReportError::LongIdentifier(&yylloc, yytext);
			//   strncpy(yylval.identifier, yytext, MaxIdentLen);
			//   yylval.identifier[MaxIdentLen] = '\0';   
			  
			  				return IDENTIFIER;
		        		}

 /* consume single-line comment */
{SINGLE_COMMENT}        ;
 /* count line */
\n                    	{yylineno++;}
 /* all other characters are reported as errors */
.                       {
                        //ReportError::UnrecogChar(&yylloc, yytext[0]);
                        }

%%

void InitScanner()
{
    //PrintDebug("lex", "Initializing scanner");
    //yy_flex_debug = false;
    //yy_push_state(COPY);
    colno = 1;
}

static void DoBeforeEachAction()
{
  yylloc.first_line = yylloc.last_line = yylineno;
  //printf("%s",yytext);
  yylloc.first_column = colno;
  yylloc.last_column = colno + yyleng - 1;
  colno = colno + yyleng;
}

/* const char *GetLineNumbered(int num) 
{
  if (num <= 0 || num > savedlines.NumElements())
    return NULL;

  return savedlines.Nth(num - 1);
} */

int yywrap(void){
    return 1;
}
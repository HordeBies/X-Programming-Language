%{

#include "main.h"

// static void DoBeforeEachAction(); 
// #define YY_USER_ACTION DoBeforeEachAction();

/* 
 * This flex built-in macro can be defined to provide an action which is
 * always executed prior to any matched rule's action. Basically, it is
 * a way of having a piece of code common to all actions factored out to
 * this routine. It will be called once for each pattern scanned from the file, 
 * before executing its action.
 */
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
"<="					{ return LTE; }
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
/* The closing %% above marks the end of the Rules section and the beginning
 * of the User Subroutines section. All text from here to the end of the
 * file is copied verbatim to the end of the generated lex.yy.c file.
 * This section is where you put definitions of helper functions.
 */


/* Function: InitScanner
 * ---------------------
 * This function will be called before any calls to yylex().  It is designed
 * to give you an opportunity to do anything that must be done to initialize
 * the scanner (set global variables, configure starting state, etc.). One
 * thing it already does for you is assign the value of the global variable
 * yy_flex_debug that controls whether flex prints debugging information
 * about each token and what rule was matched. If set to false, no information
 * is printed. Setting it to true will give you a running trail that might
 * be helpful when debugging your scanner. Please be sure the variable is
 * set to false when submitting your final version.
 */
void InitScanner()
{
    //PrintDebug("lex", "Initializing scanner");
    //yy_flex_debug = false;
    //yy_push_state(COPY);
    colno = 1;
}


/* Function: DoBeforeEachAction()
 * ------------------------------
 * This function is installed as the YY_USER_ACTION. This is a place
 * to group code common to all actions.
 */
static void DoBeforeEachAction()
{
  yylloc.first_line = yylloc.last_line = yylineno;
  //printf("%s",yytext);
  yylloc.first_column = colno;
  yylloc.last_column = colno + yyleng - 1;
  colno = colno + yyleng;
}

/* Function: GetLinenumbered()
 * ---------------------------
 * Returns string with contents of line numbered n or NULL if the
 * contents of that line are not available. Our scanner copies
 * each line scanned and appends each to a list so we can later
 * retrieve them to report the context for errors.
 */
/* const char *GetLineNumbered(int num) 
{
  if (num <= 0 || num > savedlines.NumElements())
    return NULL;

  return savedlines.Nth(num - 1);
} */
int yywrap(void){
    return 1;
}
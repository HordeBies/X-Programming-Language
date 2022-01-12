#include "utils/utilities.h"
#include "utils/constants.h"
#include "parse_tree/parse_tree.h"

extern int yyparse();
extern FILE *yyin;
extern StatementNode* programRoot;

std::string outputFilename = "data/quads";
std::string symbolTableFilename = "data/symbol_table";

int main(int argc, char *argv[]){
    printf("\n\n----------- My Programming Language Compiler ----------- \n\n");
    yyin = fopen(argv[1], "r");
    
    yyparse();

    //Analyze parse tree here

    if(true) //if no error while anaylzing
    {
        //Generate SymbolTable and Quads   
    }   

    fclose(yyin);

    if(programRoot != NULL)
        delete programRoot;
    
    return 0;
}
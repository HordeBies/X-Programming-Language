#include "compile/analyzer.h"
#include "compile/generator.h"
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

    Analyzer analyzer(argv[1]);

    if(programRoot != NULL && programRoot->Analyze(&analyzer))
    {
        //Generate SymbolTable and Quads   
    }   

    fclose(yyin);

    if(programRoot != NULL)
        delete programRoot;
    
    return 0;
}
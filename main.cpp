#include "utils/utilities.h"
#include "utils/constants.h"
#include "parse_tree/parse_tree.h"

extern int yyparse();
extern FILE *yyin;


int main(int argc, char *argv[]){
    printf("\n\n----------- Simple Programming Language Compiler ----------- \n\n");
    yyin = fopen(argv[1], "r");
    //myfile.open("log.txt");
    yyparse();
    //myfile.close();
    fclose(yyin);
    return 0;
}
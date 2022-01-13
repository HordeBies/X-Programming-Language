#include <iostream>
#include <string>
#include <string.h>

#include "compile/analyzer.h"
#include "compile/generator.h"
#include "parse_tree/parse_tree.h"
#include "utils/utilities.h"
#include "utils/constants.h"

extern int yyparse();
extern FILE *yyin;
extern StatementNode* programRoot;

void writeToFile(std::string data, std::string filename);

std::string outputFilename = "data/quads";
std::string symbolTableFilename = "data/symbol_table.csv";

int main(int argc, char *argv[]){
    printf("\n\n----------- X Programming Language Compiler ----------- \n\n");
    yyin = fopen(argv[1], "r");
    printf("Parsing...\n");
    yyparse();
    printf("Successfully Parsed.\n\n");
    Analyzer analyzer(argv[1]);
    Generator generator;
    printf("Analyzing the code...\n");
    if(programRoot != NULL && programRoot->Analyze(&analyzer))
    {
        printf("Successfully Analyzed.\n\n");
        printf("Generating the symbol table and instruction quads...\n");
        writeToFile(programRoot->GenerateQuad(&generator), outputFilename);
        writeToFile(analyzer.getSymbolTableStr(),symbolTableFilename);
        printf("Sucessfully generated symbol table and instruction quads.");
        printf("\n\n-----------Succesfully Compiled-----------\n\n");
    }   

    fclose(yyin);

    if(programRoot != NULL)
        delete programRoot;
    
    return 0;
}

void writeToFile(std::string data, std::string filename) {
    if (filename.empty()) {
        return;
    }

    std::ofstream fout(filename);

    if (!fout.is_open()) {
        fprintf(stderr, "error: could not write in file '%s'!\n", filename.c_str());
        return;
    }

    fout << data << std::endl;

    fout.close();
}
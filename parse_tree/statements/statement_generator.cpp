#include "../parse_tree.h"
#include "../../compile/generator.h"

std::string ScopeNode::GenerateQuad(Generator* context) {
    std::string ret;

    for (int i = 0; i < statements.size(); ++i) {
        ret += statements[i]->GenerateQuad(context);
    }
    
    return ret;
}

std::string VarDeclarationNode::GenerateQuad(Generator* context) {
    std::string ret;

    if (context->declareFuncParams) {
        ret += alias + " # # "+ Utils::oprToQuad(OP_POP) + "\n";
    }

    if (value) {
        if(value->constant) {
            ret += alias + " " + value->GenerateQuad(context) + " # " + Utils::oprToQuad(OP_LOAD)  + "\n";
        }
        else {
            ret += value->GenerateQuad(context);
            ret += alias + " " + context->getCurrTemp() + " # " + Utils::oprToQuad(OP_LOAD) + "\n";
        }
    }


    return ret;
}
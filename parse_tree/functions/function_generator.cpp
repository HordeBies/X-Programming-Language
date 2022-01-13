#include "../parse_tree.h"
#include "../../compile/generator.h"

std::string FunctionNode::GenerateQuad(Generator* context) {
    std::string ret;

    ret += alias+"_B:\n";
    context->declareFuncParams = true;

    for (int i = 0; i < paramList.size(); ++i) {
        ret += paramList[i]->GenerateQuad(context);
    }

    context->declareFuncParams = false;
    ret += body->GenerateQuad(context);
    ret += alias + "_E:\n";

    return ret;
}

std::string FunctionInvokeNode::GenerateQuad(Generator* context){
    std::string ret;

    for (int i = (int) argList.size() - 1; i >= 0; --i) {
        auto& arg =  argList[i];
        if(arg->constant || (arg->reference && arg->reference->id))
            ret += arg->GenerateQuad(context) +" # # PUSH\n";
        else{
            ret += arg->GenerateQuad(context);
            std::string arg_temp = context->getCurrTemp();
            ret += arg_temp +" # # PUSH\n";
        }
    }

    ret += func->alias+"_B" + " # # " + Utils::oprToQuad(OP_JL) + "\n";

    if(func->declaredType() != Utils::dtypeToStr(TYPE_VOID))
        ret += context->generateTemp() + " # # " + Utils::oprToQuad(OP_POP) + "\n";
    return ret;
}

std::string ReturnNode::GenerateQuad(Generator* context){
    std::string ret;

    if(value) {
        if(value->constant || (value->reference && value->reference->id))
            ret += value->GenerateQuad(context) + " # # " + Utils::oprToQuad(OP_PUSH) + "\n";
        else{
            ret += value->GenerateQuad(context);
            std::string value_temp = context->getCurrTemp();
            ret += value_temp + " # # " + Utils::oprToQuad(OP_PUSH) + "\n";
        }
    }
    ret += "# # # " + Utils::oprToQuad(OP_JB) + "\n";
    return ret;
}

std::string PrintNode::GenerateQuad(Generator* context){
    std::string ret;
    if(id)
        ret += (format.empty()?"#":format) + " " + id->GenerateQuad(context) + " # " + Utils::oprToQuad(OP_PR) + "\n";
    else
        ret += (format.empty()?"#":format) + " " + " # # " + Utils::oprToQuad(OP_PR) + "\n";

    return ret;
}

#include "../parse_tree.h"
#include "../../compile/generator.h"

std::string IfNode::GenerateQuad(Generator* context){
    std::string ret;
    std::string label = context->getIfLabelIdx();
    //ret += "IF_"+label+"_B:\n";

    if(cond->constant || (cond->reference&& cond->reference->id))
        ret += cond->GenerateQuad(context) + " " +(elseBody?"IFE_"+label+"_B":"IF_"+label+"_E") + " # " + Utils::oprToQuad(OP_JZ) + "\n";
    else
    {
        ret += cond->GenerateQuad(context);
        std::string cond_temp = context->getCurrTemp();
        ret += cond_temp + " " + (elseBody?"IFE_"+label+"_B":"IF_"+label+"_E") + " # " + Utils::oprToQuad(OP_JZ) + "\n";
    }
    
    ret += ifBody->GenerateQuad(context);

    if (elseBody) {
        ret += "IFE_"+label+"_B:\n";
        ret += elseBody->GenerateQuad(context);
    }

    ret += "IF_"+label+"_E:\n";
    return ret;
}

std::string WhileNode::GenerateQuad(Generator* context){
    std::string ret;
    std::string label = context->getWhileLabelIdx();
    
    ret += "W_"+label+"_B:\n";

    if(cond->constant || (cond->reference && cond->reference->id))
        ret += cond->GenerateQuad(context) + " W_"+label+"_E " + Utils::oprToQuad(OP_JZ) + "\n";
    else{
        ret += cond->GenerateQuad(context);
        std::string cond_temp = context->getCurrTemp();
        ret += cond_temp + " W_"+label+"_E " + Utils::oprToQuad(OP_JZ) + "\n";
    }

    context->breakLabels.push("W_"+label+"_E");
    ret += body->GenerateQuad(context);
    context->breakLabels.pop();

    ret += "W_"+label+"_B" + " # # " + Utils::oprToQuad(OP_JMP) + "\n";
    ret += "W_"+label+"_E:\n";
    return ret;
}

std::string ForNode::GenerateQuad(Generator* context){
    std::string ret;
    std::string label = context->getForLabelIdx();

    ret += initStmt->GenerateQuad(context);

    ret += "F_"+label+"_B:\n";

    if(cond->constant || (cond->reference && cond->reference->id))
        ret += cond->GenerateQuad(context) + " F_"+label+"_E " + Utils::oprToQuad(OP_JZ) + "\n";
    else{
        ret += cond->GenerateQuad(context);
        std::string cond_temp = context->getCurrTemp();
        ret += cond_temp + " F_"+label+"_E " + Utils::oprToQuad(OP_JZ) + "\n";
    }

    context->breakLabels.push("F_"+label+"_E");
    ret += body->GenerateQuad(context);
    context->breakLabels.pop();

    ret += inc->GenerateQuad(context);
    ret += "F_"+label+"_B" + " # # " + Utils::oprToQuad(OP_JMP) + "\n";
    ret += "F_"+label+"_E:\n";
    return ret;
}

std::string BreakStmtNode::GenerateQuad(Generator* context){
    return context->breakLabels.top() + " # # " + Utils::oprToQuad(OP_JMP) + "\n";
}
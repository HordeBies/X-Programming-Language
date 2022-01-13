#include "../parse_tree.h"
#include "../../compile/generator.h"

std::string ExprContainerNode::GenerateQuad(Generator* context) 
{
    return expr->GenerateQuad(context);
}

std::string AssignOprNode::GenerateQuad(Generator* context)
{
    std::string ret;
    if(rhs->constant || (rhs->reference && rhs->reference->id))
        ret += lhs->reference->alias + " " + rhs->GenerateQuad(context) + " # "+ Utils::oprToQuad(OP_LOAD) + "\n";
    else{
        ret += rhs->GenerateQuad(context);
        std::string rhs_temp = context->getCurrTemp();
        ret += lhs->reference->alias + " " + rhs_temp + " # " + Utils::oprToQuad(OP_LOAD) + "\n";
    }
    return ret;
}

std::string BinaryOprNode::GenerateQuad(Generator* context) {
    std::string ret;

    if (used) {
        if( (lhs->constant || (lhs->reference && lhs->reference->id)) && (rhs->constant || (rhs->reference && rhs->reference->id))){
            ret += context->generateTemp() + " " + lhs->GenerateQuad(context) + " " + rhs->GenerateQuad(context) + " " + Utils::oprToQuad(opr) + "\n";
        }else if(lhs->constant ||  (lhs->reference && lhs->reference->id))
        {
            ret += rhs->GenerateQuad(context);
            std::string rhs_temp = context->getCurrTemp();
            ret += context->generateTemp() + " " + lhs->GenerateQuad(context) + " " + rhs_temp + " " + Utils::oprToQuad(opr) + "\n";
        }else if(rhs->constant || (rhs->reference && rhs->reference->id))
        {
            ret += lhs->GenerateQuad(context);
            std::string lhs_temp = context->getCurrTemp();
            ret += context->generateTemp() + " " + lhs_temp + " " + rhs->GenerateQuad(context) + " " + Utils::oprToQuad(opr) + "\n";
        }else
        {
            ret += lhs->GenerateQuad(context);
            std::string lhs_temp = context->getCurrTemp();
            ret += rhs->GenerateQuad(context);
            std::string rhs_temp = context->getCurrTemp();
            ret += context->generateTemp() + " " + lhs_temp + " " + rhs_temp + " " + Utils::oprToQuad(opr) + "\n";
        }
    }
    else {
            // std::cout<<"DEBUG: " << "not used"  << "\n";
    }

    return ret;
}

std::string UnaryOprNode::GenerateQuad(Generator* context) {
    std::string ret;
    
    //ret += expr->GenerateQuad(context);
    if(opr == OP_INC || opr == OP_DEC){
        ret += expr->GenerateQuad(context) + " # # "+ Utils::oprToQuad(opr) + "\n";
    }
    else{
        if(expr->constant || (expr->reference && expr->reference->id))
            ret += context->generateTemp() + " " + expr->GenerateQuad(context)  + " # " + Utils::oprToQuad(opr) + "\n";
        else{
            expr->GenerateQuad(context);
            std::string expr_temp = context->getCurrTemp();
            ret += context->generateTemp() + " " + expr_temp  + " # " + Utils::oprToQuad(opr) + "\n";
        }
    }

    return ret;
}

std::string IdentifierNode::GenerateQuad(Generator* context) {
    std::string ret;
    if (used) {
        ret += reference->alias;
    }
    return ret;
}

std::string ValueNode::GenerateQuad(Generator* context) {
    std::string ret;
    if (used) {
        ret += value ;
    }
    return ret;
}
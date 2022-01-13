#include "../parse_tree.h"
#include "../../compile/analyzer.h"

bool ExprContainerNode::Analyze(Analyzer* context, bool valueUsed)
{
    if (!context->initializeVar && context->isGlobalScope()) {
        context->log("expression is not allowed in global scope", loc, LOG_ERROR);
        return false;
    }

    bool ret = expr->Analyze(context, valueUsed);

    type = expr->type;
    reference = expr->reference;
    constant = expr->constant;
    used = valueUsed;

    return ret;
}

bool AssignOprNode::Analyze(Analyzer* context, bool valueUsed) 
{
    if (!(rhs->Analyze(context, true) & lhs->Analyze(context, false))) {
        // Note that I used a bitwise AND to execute both lhs and rhs expressions
        return false;
    }

    if (lhs->type == TYPE_FUNC_PTR) {
        context->log("assignment of function '" + lhs->reference->declaredHeader() + "'", lhs->loc, LOG_ERROR);
        return false;
    }
    if (lhs->reference == NULL) {
        context->log("lvalue required as left operand of assignment", lhs->loc, LOG_ERROR);
        return false;
    }
    if (rhs->type == TYPE_VOID || rhs->type == TYPE_FUNC_PTR) {
        context->log("invalid conversion from '" + rhs->exprTypeStr() + "' to '" + lhs->exprTypeStr() + "'", rhs->loc, LOG_ERROR);
        return false;
    }

    type = lhs->type;
    reference = lhs->reference;
    constant = lhs->constant;
    used = valueUsed;

    reference->initialized = true;

    return true;
}

bool BinaryOprNode::Analyze(Analyzer* context, bool valueUsed) 
{
    if (!(lhs->Analyze(context, valueUsed) & rhs->Analyze(context, valueUsed))) {
        // Note that I used a bitwise AND to execute both lhs and rhs expressions
        return false;
    }

    if (lhs->type == TYPE_VOID || lhs->type == TYPE_FUNC_PTR ||
        rhs->type == TYPE_VOID || rhs->type == TYPE_FUNC_PTR )
    {
        context->log("invalid operands of types '" + lhs->exprTypeStr() + "' and '" + rhs->exprTypeStr() + "' to " +
                     getOpr(), loc, LOG_ERROR);
        return false;
    }

    if (Utils::isLogicalOpr(opr)) {
        type = TYPE_BOOL;
    } else {
        type = std::max(lhs->type, rhs->type);
    }

    constant = (lhs->constant && rhs->constant);
    used = valueUsed;

    return true;
}

bool UnaryOprNode::Analyze(Analyzer* context, bool valueUsed) 
{
    if (!expr->Analyze(context, valueUsed || Utils::isLvalueOpr(opr))) {
        return false;
    }

    if (expr->type == TYPE_VOID || expr ->type == TYPE_FUNC_PTR) {
        context->log("invalid operand of type '" + expr->exprTypeStr() + "' to " + getOpr(), loc, LOG_ERROR);
        return false;
    }

    if (Utils::isLvalueOpr(opr)) {
        if (expr->reference == NULL) {
            context->log("lvalue required as an operand of increment/decrement operator", expr->loc, LOG_ERROR);
            return false;
        }
        if (expr->reference && expr->constant) {
            context->log("increment/decrement of read-only variable '" + expr->reference->declaredHeader() + "'",
                         expr->loc, LOG_ERROR);
            return false;
        }
    }

    type = (Utils::isLogicalOpr(opr) ? TYPE_BOOL : expr->type);
    reference =  NULL;
    constant = expr->constant;
    used = valueUsed;

    return true;
}

bool IdentifierNode::Analyze(Analyzer* context, bool valueUsed) 
{
    DeclarationNode* ptr = context->getSymbol(name);

    if (ptr == NULL) {
        context->log("'" + name + "' was not declared in this scope", loc, LOG_ERROR);
        return false;
    }

    reference = ptr;

    if (dynamic_cast<FunctionNode*>(ptr)) {
        type = TYPE_FUNC_PTR;
    } else {
        type = ptr->type->type;
    }

    used = valueUsed;

    if (used) {
        reference->used++;
    }

    if (used && !reference->initialized) {
        context->log("variable or field '" + name + "' used without being initialized", loc, LOG_ERROR);
        return false;
    }

    return true;
}



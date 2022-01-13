#include "../parse_tree.h"
#include "../../compile/analyzer.h"

bool IfNode::Analyze(Analyzer* context) 
{
    if (context->isGlobalScope()) {
        context->log("if-statement is not allowed in global scope", loc, LOG_ERROR);
        return false;
    }

    bool ret = true;

    context->addScope(SCOPE_IF, this);

    ret &= cond->Analyze(context, true);

    if (ret && cond->type == TYPE_VOID) {
        context->log("invalid conversion from '" + cond->exprTypeStr() + "' to 'bool'", cond->loc, LOG_ERROR);
        ret = false;
    }

    ret &= ifBody->Analyze(context);

    if (elseBody) {
        ret &= elseBody->Analyze(context);
    }

    context->popScope();

    return ret;
}

bool WhileNode::Analyze(Analyzer* context)
{
    if (context->isGlobalScope()) {
        context->log("while-statement is not allowed in global scope", loc, LOG_ERROR);
        return false;
    }

    bool ret = true;

    context->addScope(SCOPE_LOOP, this);

    ret &= cond->Analyze(context, true);

    if (ret && cond->type == TYPE_VOID) {
        context->log("invalid conversion from '" + cond->exprTypeStr() + "' to 'bool'", cond->loc, LOG_ERROR);
        ret = false;
    }

    ret &= body->Analyze(context);

    context->popScope();

    return ret;
}

bool ForNode::Analyze(Analyzer* context) {
    if (context->isGlobalScope()) {
        context->log("for-statement is not allowed in global scope", loc, LOG_ERROR);
        return false;
    }

    bool ret = true;

    context->addScope(SCOPE_LOOP, this);

    if (initStmt) {
        ret &= initStmt->Analyze(context);
    }

    if (cond) {
        ret &= cond->Analyze(context, true);

        if (ret && cond->type == TYPE_VOID) {
            context->log("invalid conversion from '" + cond->exprTypeStr() + "' to 'bool'", cond->loc, LOG_ERROR);
            ret = false;
        }
    }

    if (inc) {
        ret &= inc->Analyze(context, false);
    }

    ret &= body->Analyze(context);

    context->popScope();

    return ret;
}

bool BreakStmtNode::Analyze(Analyzer* context) {
    if (!context->hasBreakScope()) {
        context->log("break-statement not within loop or switch", loc, LOG_ERROR);
        return false;
    }

    return true;
}
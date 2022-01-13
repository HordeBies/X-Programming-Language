#include "../parse_tree.h"
#include "../../compile/analyzer.h"

bool FunctionNode::Analyze(Analyzer* context) 
{
    if (!context->isGlobalScope()) {
        context->log("a function-definition is not allowed here", id->loc, LOG_ERROR);
        return false;
    }

    bool ret = true;

    if (!context->declareSymbol(this)) {
        context->log("'" + declaredHeader() + "' redeclared", id->loc, LOG_ERROR);
        ret = false;
    }

    context->addScope(SCOPE_FUNCTION, this);

    context->declareFuncParams = true;
    for (int i = 0; i < paramList.size(); ++i) {
        ret &= paramList[i]->Analyze(context);
    }
    context->declareFuncParams = false;

    ret &= body->Analyze(context);

    context->popScope();

    return ret;
}

bool FunctionInvokeNode::Analyze(Analyzer* context, bool valueUsed)
{
    bool ret = true;

    DeclarationNode* ptr = context->getSymbol(id->name);
    func = dynamic_cast<FunctionNode*>(ptr);

    if (ptr == NULL) {
        context->log("'" + id->name + "' was not declared in this scope", loc, LOG_ERROR);
        ret = false;
    }
    else if (func == NULL) {
        context->log("'" + id->name + "' cannot be used as a function", loc, LOG_ERROR);
        ret = false;
    }
    else if (argList.size() > func->paramList.size()) {
        context->log("too many arguments to function '" + func->declaredHeader() + "'", loc, LOG_ERROR);
        ret = false;
    }
    else if (argList.size() < func->paramList.size()) {
        context->log("too few arguments to function '" + func->declaredHeader() + "'", loc, LOG_ERROR);
        ret = false;
    } else {
        type = ptr->type->type;
    }

    for (int i = 0; i < argList.size(); ++i) {
        if (!argList[i]->Analyze(context, true)) {
            ret = false;
            continue;
        }

        if (func && argList[i]->type == TYPE_VOID || argList[i]->type == TYPE_FUNC_PTR) {
            context->log("invalid conversion from '" + argList[i]->exprTypeStr() + "' to '" +
                         func->paramList[i]->type->toString() + "' in function '" +
                         func->declaredHeader() + "' call", argList[i]->loc, LOG_ERROR);
            return false;
        }
    }

    used = valueUsed;

    if (ret) {
        func->used++;
    }

    return ret;
}

bool ReturnNode::Analyze(Analyzer* context) 
{
    func = context->getFunctionScope();

    if (func == NULL) {
        context->log("return-statement not within function", loc, LOG_ERROR);
        return false;
    }

    if (value) {
        if (!value->Analyze(context, true)) {
            return false;
        }

        if (func->type->type == TYPE_VOID && value->type != TYPE_VOID) {
            context->log("return-statement with '" + value->exprTypeStr() + "' value, in function returning 'void'",
                         value->loc, LOG_ERROR);
            return false;
        }
    }
    else {
        if (func->type->type != TYPE_VOID) {
            context->log("return-statement with no value, in function returning '" + func->type->toString() + "'", loc, LOG_ERROR);
            return false;
        }
    }

    return true;
}

bool PrintNode::Analyze(Analyzer* context)
{
    bool ret = true;
    if(id != NULL)
    {
        if(!id->Analyze(context,true))
        {
            ret =false;
            return ret;
        }
        if(!Utils::isPrintType(id->type))
        {
            context->log("invalid identifier passed down to print function ",loc,LOG_ERROR);
            ret = false;
        }
    } 
    return ret;
}
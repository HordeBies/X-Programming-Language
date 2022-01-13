#include "../parse_tree.h"
#include "../../compile/analyzer.h"

bool ScopeNode::Analyze(Analyzer* context)
{
    if (context->isGlobalScope()) {
        context->log("block is not allowed in global scope", loc, LOG_ERROR);
        return false;
    }
    
    bool ret = true;

    context->addScope(SCOPE_BLOCK, this);

    for (int i = 0; i < statements.size(); ++i) {
        ret &= statements[i]->Analyze(context);
    }

    context->popScope();

    return ret;
}

bool VarDeclarationNode::Analyze(Analyzer* context){
    bool ret = true;

    if (!context->declareSymbol(this)) {
        context->log("'" + declaredHeader() + "' redeclared", id->loc, LOG_ERROR);
        ret = false;
    }

    if (context->declareFuncParams) {
        initialized = true;
    }

    if (value) {
        context->initializeVar = true;
        ret &= value->Analyze(context, true);
        context->initializeVar = false;
    }

    if (ret && value != NULL && (value->type == TYPE_VOID || value->type == TYPE_FUNC_PTR)) {
        context->log("invalid conversion from '" + value->exprTypeStr() + "' to '" + type->toString() + "'", value->loc, LOG_ERROR);
        ret = false;
    }
    else if (context->declareFuncParams && value != NULL) {
        context->log("default function parameters are not allowed", value->loc, LOG_ERROR);
        ret = false;
    }

    return ret;
}
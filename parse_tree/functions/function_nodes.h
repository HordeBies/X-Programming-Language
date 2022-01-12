#ifndef FUNCTION_NODES
#define FUNCTION_NODES

#include "../node_base.h"
#include "../statements/statement_nodes.h"
struct FunctionNode : public DeclarationNode
{
    VarList paramList;
    ScopeNode* body;

    FunctionNode(TypeNode* type, IdentifierNode* id, const VarList& paramList, ScopeNode* body)
            : DeclarationNode(type->loc) {
        this->type = type;
        this->id = id;
        this->paramList = paramList;
        this->body = body;
        this->initialized = true;
    }
};

struct FunctionInvokeNode : public ExpressionNode 
{
    IdentifierNode* id;
    ExprList argList;
    FunctionNode* func;

    FunctionInvokeNode(IdentifierNode* id, const ExprList& argList) : ExpressionNode(id->loc) {
        this->id = id;
        this->argList = argList;
    }
};

struct ReturnNode : public StatementNode 
{
    ExpressionNode* value;
    FunctionNode* func;

    ReturnNode(const Location& loc, ExpressionNode* value) : StatementNode(loc) {
        this->value = value;
    }
};

struct PrintNode : public StatementNode
{
    std::string format;
    IdentifierNode* id;
    PrintNode(const Location& loc, std::string format, IdentifierNode* id) : StatementNode(loc){
        this->format = format;
        this->id = id;
    }
};

#endif
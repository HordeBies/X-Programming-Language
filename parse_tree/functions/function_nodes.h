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

    virtual bool Analyze(Analyzer* context);

    virtual std::string GenerateQuad(Generator* context);

    virtual std::string declaredHeader() {
        std::string ret = type->toString() + " " + id->name + "(";
        for (int i = 0; i < paramList.size(); ++i) {
            ret += (i > 0 ? ", " : "") + paramList[i]->type->toString();
        }
        ret += ")";
        return ret;
    }

    virtual std::string declaredType() {
        std::string ret = type->toString() + "(*)(";
        for (int i = 0; i < paramList.size(); ++i) {
            ret += (i > 0 ? ", " : "") + paramList[i]->type->toString();
        }
        ret += ")";
        return ret;
    }

    virtual std::string toString(int ind = 0) {
        std::string ret = std::string(ind, ' ') + type->toString() + " " + id->toString() + "(";
        for (int i = 0; i < paramList.size(); ++i) {
            ret += (i > 0 ? ", " : "") + paramList[i]->toString();
        }
        ret += ")\n";
        ret += body->toString(ind);
        return ret;
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

    virtual bool Analyze(Analyzer* context, bool valueUsed);

    virtual std::string GenerateQuad(Generator* context);

    virtual std::string toString(int ind = 0) {
        std::string ret = std::string(ind, ' ') + id->name + "(";
        for (int i = 0; i < argList.size(); ++i) {
            ret += (i > 0 ? ", " : "") + argList[i]->toString();
        }
        ret += ")";
        return ret;
    }
};

struct ReturnNode : public StatementNode 
{
    ExpressionNode* value;
    FunctionNode* func;

    ReturnNode(const Location& loc, ExpressionNode* value) : StatementNode(loc) {
        this->value = value;
    }

    virtual bool Analyze(Analyzer* context);

    virtual std::string GenerateQuad(Generator* context);

    virtual std::string toString(int ind = 0) {
        std::string ret = std::string(ind, ' ') + "return";
        if (value) {
            ret += " " + value->toString();
        }
        return ret;
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

    virtual bool Analyze(Analyzer* context);

    virtual std::string GenerateQuad(Generator* context);

    virtual std::string toString(int ind = 0){
        std::string ret = std::string(ind,' ') + "print(" + format + id->toString() + ")";
        return ret;
    }
};

#endif
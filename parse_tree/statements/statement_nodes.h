#ifndef STATEMENT_NODES
#define STATEMENT_NODES

#include "../node_base.h"
#include "../expressions/expression_nodes.h"

struct ScopeNode : public StatementNode
{
    StmtList statements;

    ScopeNode() {}

    ScopeNode(const Location& loc) : StatementNode(loc) {}

    ScopeNode(const Location& loc, const StmtList& statements) : StatementNode(loc) {
        this->statements = statements;
    }

    virtual bool Analyze(Analyzer* context);

    virtual std::string GenerateQuad(Generator* context);

    virtual std::string toString(int ind = 0) {
        std::string ret = std::string(ind, ' ') + "{\n";
        for (int i = 0; i < statements.size(); ++i) {
            ret += statements[i]->toString(ind + 4) + "\n";
        }
        ret += std::string(ind, ' ') + "}";
        return ret;
    }
};

struct VarDeclarationNode : public DeclarationNode {
    ExpressionNode* value;

    VarDeclarationNode(TypeNode* type, IdentifierNode* id, ExpressionNode* value = NULL)
            : DeclarationNode(type->loc) {
        this->type = type;
        this->id = id;
        this->value = value;
        this->initialized = (value != NULL);
    }

    virtual bool Analyze(Analyzer* context);

    virtual std::string GenerateQuad(Generator* context);

    virtual std::string toString(int ind = 0) {
        std::string ret = std::string(ind, ' ') + declaredHeader();
        if (value) {
            ret += " = " + value->toString();
        }
        return ret;
    }

    virtual std::string declaredHeader() {
        return type->toString() + " " + id->name;
    }

    virtual std::string declaredType() {
        return type->toString();
    }
};




#endif
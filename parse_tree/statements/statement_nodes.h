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
};




#endif
#ifndef CONTROL_NODES
#define CONTROL_NODES

#include "../node_base.h"
#include "../statements/statement_nodes.h"
#include "../expressions/expression_nodes.h"

struct IfNode : public StatementNode {
    ExpressionNode* cond;
    ScopeNode* ifBody;
    StatementNode* elseBody;

    IfNode(const Location& loc, ExpressionNode* cond, ScopeNode* ifBody, ScopeNode* elseBody = NULL)
            : StatementNode(loc) {
        this->cond = cond;
        this->ifBody = ifBody;
        this->elseBody = elseBody;
    }
};

struct WhileNode : public StatementNode {
    ExpressionNode* cond;
    ScopeNode* body;

    WhileNode(const Location& loc, ExpressionNode* cond, ScopeNode* body) : StatementNode(loc) {
        this->cond = cond;
        this->body = body;
    }
};

struct ForNode : public StatementNode {
    StatementNode* initStmt;
    ExpressionNode* cond;
    ExpressionNode* inc;
    ScopeNode* body;

    ForNode(const Location& loc, StatementNode* initStmt, ExpressionNode* cond, ExpressionNode* inc,
            ScopeNode* body) : StatementNode(loc) {
        this->initStmt = initStmt;
        this->cond = cond;
        this->inc = inc;
        this->body = body;
    }
};

struct BreakStmtNode : public StatementNode {

    BreakStmtNode(const Location& loc) : StatementNode(loc) {}
};

#endif
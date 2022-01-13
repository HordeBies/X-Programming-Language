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

    virtual bool Analyze(Analyzer* context);

    virtual std::string toString(int ind = 0) {
        std::string ret = std::string(ind, ' ') + "if (" + cond->toString() + ")\n";
        ret += ifBody->toString(ind + (dynamic_cast<ScopeNode*>(ifBody) ? 0 : 4));

        if (elseBody) {
            ret += "\n" + std::string(ind, ' ') + "else\n";
            ret += elseBody->toString(ind + (dynamic_cast<ScopeNode*>(elseBody) ? 0 : 4));
        }

        return ret;
    }
};

struct WhileNode : public StatementNode {
    ExpressionNode* cond;
    ScopeNode* body;

    WhileNode(const Location& loc, ExpressionNode* cond, ScopeNode* body) : StatementNode(loc) {
        this->cond = cond;
        this->body = body;
    }

    virtual bool Analyze(Analyzer* context);

    virtual std::string toString(int ind = 0) {
        std::string ret = std::string(ind, ' ') + "while (" + cond->toString() + ") \n";
        ret += body->toString(ind + (dynamic_cast<ScopeNode*>(body) ? 0 : 4));
        return ret;
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

    virtual bool Analyze(Analyzer* context);

    virtual std::string toString(int ind = 0) {
        std::string ret = std::string(ind, ' ') + "for (";
        ret += (initStmt ? initStmt->toString() : "") + ";";
        ret += (cond ? cond->toString() : "") + ";";
        ret += (inc ? inc->toString() : "") + ")\n";
        ret += body->toString(ind + (dynamic_cast<ScopeNode*>(body) ? 0 : 4));
        return ret;
    }
};

struct BreakStmtNode : public StatementNode {

    BreakStmtNode(const Location& loc) : StatementNode(loc) {}

    virtual bool Analyze(Analyzer* context);

    virtual std::string toString(int ind = 0) {
        return std::string(ind, ' ') + "break";
    }
};

#endif
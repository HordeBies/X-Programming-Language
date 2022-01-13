#ifndef EXPRESSION_NODES
#define EXPRESSION_NODES

#include "../node_base.h"

struct ExprContainerNode : ExpressionNode
{
    ExpressionNode* expr;

    ExprContainerNode(const Location& loc, ExpressionNode* expr) : ExpressionNode(loc) {
        this->expr = expr;
    }

    virtual int getConstIntValue() {
        return expr->getConstIntValue();
    }

    virtual bool Analyze(Analyzer* context, bool valueUsed);

    virtual std::string GenerateQuad(Generator* context);

    virtual std::string toString(int ind = 0) {
        return expr->toString(ind);
    }
};

struct AssignOprNode : public ExpressionNode
{
    ExpressionNode* lhs;
    ExpressionNode* rhs;

    AssignOprNode(const Location& loc, ExpressionNode* lhs, ExpressionNode* rhs) : ExpressionNode(loc) {
        this->lhs = lhs;
        this->rhs = rhs;
    }

    virtual bool Analyze(Analyzer* context, bool valueUsed);

    virtual std::string GenerateQuad(Generator* context);

    virtual std::string toString(int ind = 0) {
        return std::string(ind, ' ') + "(" + lhs->toString() + " = " + rhs->toString() + ")";
    }
};

struct BinaryOprNode : public ExpressionNode
{
    Operator opr;
    ExpressionNode* lhs;
    ExpressionNode* rhs;

    BinaryOprNode(const Location& loc, Operator opr, ExpressionNode* lhs, ExpressionNode* rhs) : ExpressionNode(loc) {
        this->opr = opr;
        this->lhs = lhs;
        this->rhs = rhs;
    }

    virtual int getConstIntValue();

    virtual bool Analyze(Analyzer* context, bool valueUsed);

    virtual std::string GenerateQuad(Generator* context);

    virtual std::string getOpr() {
        return "binary operator '" + Utils::oprToStr(opr) + "'";
    }

    virtual std::string toString(int ind = 0) {
        return std::string(ind, ' ') + "(" + lhs->toString() + " " + Utils::oprToStr(opr) + " " + rhs->toString() + ")";
    }
};

struct UnaryOprNode : public ExpressionNode
{
    Operator opr;
    ExpressionNode* expr;

    UnaryOprNode(const Location& loc, Operator opr, ExpressionNode* expr) : ExpressionNode(loc) {
        this->opr = opr;
        this->expr = expr;
    }

    virtual int getConstIntValue();

    virtual bool Analyze(Analyzer* context, bool valueUsed);

    virtual std::string GenerateQuad(Generator* context);

    virtual std::string getOpr() {
        return "unary operator '" + Utils::oprToStr(opr) + "'";
    }

    virtual std::string toString(int ind = 0) {
        std::string ret = std::string(ind, ' ') + "(";
        ret += expr->toString() + Utils::oprToStr(opr);
        ret += ")";
        return ret;
    }
};

struct IdentifierNode : public ExpressionNode
{
    std::string name;

    IdentifierNode(const Location& loc, const char* name) : ExpressionNode(loc) {
        this->name = name;
    }

    virtual int getConstIntValue();

    virtual bool Analyze(Analyzer* context, bool valueUsed);

    virtual std::string GenerateQuad(Generator* context);

    virtual std::string toString(int ind = 0) {
        return std::string(ind, ' ') + name;
    }
};

struct ValueNode : public ExpressionNode {
    std::string value;

    ValueNode(const Location& loc, DataType type, const char* value) : ExpressionNode(loc) {
        this->type = type;
        this->value = value;
        this->constant = true;
    }

    virtual int getConstIntValue();

    virtual std::string GenerateQuad(Generator* context);

    virtual std::string toString(int ind = 0) {
        return std::string(ind, ' ') + value;
    }
};
#endif
#ifndef EXPRESSION_NODES
#define EXPRESSION_NODES

#include "../node_base.h"

struct ExprContainerNode : ExpressionNode
{
    ExpressionNode* expr;

    ExprContainerNode(const Location& loc, ExpressionNode* expr) : ExpressionNode(loc) {
        this->expr = expr;
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
};

struct UnaryOprNode : public ExpressionNode
{
    Operator opr;
    ExpressionNode* expr;

    UnaryOprNode(const Location& loc, Operator opr, ExpressionNode* expr) : ExpressionNode(loc) {
        this->opr = opr;
        this->expr = expr;
    }
};

struct IdentifierNode : public ExpressionNode
{
    std::string name;

    IdentifierNode(const Location& loc, const char* name) : ExpressionNode(loc) {
        this->name = name;
    }
};

struct ValueNode : public ExpressionNode {
    std::string value;

    ValueNode(const Location& loc, DataType type, const char* value) : ExpressionNode(loc) {
        this->type = type;
        this->value = value;
        this->constant = true;
    }
};
#endif
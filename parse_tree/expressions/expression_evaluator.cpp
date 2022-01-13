#include "../parse_tree.h"
#include "../../compile/analyzer.h"


int BinaryOprNode::getConstIntValue() {
    int l = lhs->getConstIntValue();
    int r = rhs->getConstIntValue();

    switch (opr) {
        case OP_ADD:
            return l + r;
        case OP_SUB:
            return l - r;
        case OP_MUL:
            return l * r;
        case OP_DIV:
            return l / r;
        case OP_MOD:
            return l % r;
        case OP_AND:
            return l && r;
        case OP_OR:
            return l || r;
        case OP_G:
            return l > r;
        case OP_GE:
            return l >= r;
        case OP_L:
            return l < r;
        case OP_LE:
            return l <= r;
        case OP_EQ:
            return l == r;
        case OP_NEQ:
            return l != r;
    }

    return -1;
}

int UnaryOprNode::getConstIntValue() {
    int v = expr->getConstIntValue();

    switch (opr) {
        case OP_MIN:
            return -v;
        case OP_NOT:
            return !v;
    }
    return -1;
}

int IdentifierNode::getConstIntValue() {
    VarDeclarationNode* var = dynamic_cast<VarDeclarationNode*>(reference);

    if (Utils::isIntegerType(type) && var != NULL && var->initialized) {
        return var->value->getConstIntValue();
    }

    return -1;
}

int ValueNode::getConstIntValue() {
    switch (type) {
        case TYPE_BOOL:
            return value == "true";
        case TYPE_INT:
            return atoi(value.c_str());
    }

    return -1;
}

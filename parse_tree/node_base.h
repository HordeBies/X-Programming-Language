#ifndef Node_Base
#define Node_Base

#include <string>
#include <vector>
#include <iostream>

#include "../utils/constants.h"
#include "../utils/utilities.h"

struct Analyzer;
struct Generator;

struct Node;
struct StatementNode;
struct DeclarationNode;
struct VarDeclarationNode;
struct FunctionNode;
struct ExpressionNode;
struct IdentifierNode;
struct TypeNode;

typedef std::vector<Node*> NodeList;
typedef std::vector<StatementNode*> StmtList;
typedef std::vector<VarDeclarationNode*> VarList;
typedef std::vector<ExpressionNode*> ExprList;


typedef struct Node
{
    Location loc;
    Node() {}
    Node(const Location& loc) {
        this->loc = loc;
        //std::cout <<this->loc.lineNum << "\t" << this->loc.pos << "\t" << this->loc.len << "\n";
    }

    virtual bool Analyze(Analyzer* context) {
        return true;
    }

    virtual std::string GenerateQuad(Generator* context) {
        return "";
    }

    virtual std::string toString() {
        return "";
    }
};

struct StatementNode : public Node
{
    StatementNode() {}

    StatementNode(const Location& loc) : Node(loc) {}

    virtual std::string toString(int ind = 0) {
        return std::string(ind, ' ') + ";" ;
    }
};

struct DeclarationNode : public StatementNode
{
    TypeNode* type;
    IdentifierNode* id;

    std::string alias;
    int used = 0;
    bool initialized = false;

    DeclarationNode(const Location& loc) : StatementNode(loc) {}

    virtual std::string declaredHeader() = 0;

    virtual std::string declaredType() = 0;
};

struct ExpressionNode : public StatementNode
{
    DataType type = TYPE_ERROR;
    DeclarationNode* reference = NULL;
    bool constant = false;
    bool used = false;

    ExpressionNode() {}

    ExpressionNode(const Location& loc) : StatementNode(loc) {}

    virtual int getConstIntValue() {
        return -1;
    }

    virtual bool Analyze(Analyzer* context) {
        return Analyze(context, false);
    }

    virtual bool Analyze(Analyzer* context, bool valueUsed) {
        used = valueUsed;
        return true;
    }

    virtual std::string exprTypeStr() {
        return reference ? reference->declaredType() : Utils::dtypeToStr(type);
    }

};

struct TypeNode : public Node
{
    DataType type;

    TypeNode(const Location& loc, DataType type) : Node(loc) {
        this->type = type;
    }

    virtual std::string toString(int ind = 0) {
        return std::string(ind, ' ') + Utils::dtypeToStr(type);
    }
};

#endif
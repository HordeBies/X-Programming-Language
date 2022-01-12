#ifndef Node_Base
#define Node_Base

#include <string>
#include <vector>
#include <iostream>

#include "../utils/constants.h"
#include "../utils/utilities.h"

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
} Node;

struct StatementNode : public Node
{
    StatementNode() {}

    StatementNode(const Location& loc) : Node(loc) {}
};

struct DeclarationNode : public StatementNode
{
    TypeNode* type;
    IdentifierNode* id;

    std::string alias;
    int used = 0;
    bool initialized = false;

    DeclarationNode(const Location& loc) : StatementNode(loc) {}

};

struct ExpressionNode : public StatementNode
{
    DataType type = TYPE_ERROR;
    DeclarationNode* reference = NULL;
    bool constant = false;
    bool used = false;

    ExpressionNode() {}

    ExpressionNode(const Location& loc) : StatementNode(loc) {}

};

struct TypeNode : public Node
{
    DataType type;

    TypeNode(const Location& loc, DataType type) : Node(loc) {
        this->type = type;
    }

};

#endif
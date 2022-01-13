#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <unordered_map>

#include "constants.h"

union Value {
    bool boolVal;
    char charVal;
    int intVal;
    double doubleVal;
};

struct Location {
    int lineNum;
    int pos;
    int len;
};

struct Token {
    char* value;
    Location loc;
};

struct Utils{
    static std::string replaceTabsWithSpaces(const std::string& str) {
        std::string ret;
        for (int i = 0; i < str.size(); ++i) {
            if (str[i] == '\t') {
                ret += "    ";
            } else {
                ret += str[i];
            }
        }
        return ret;
    }

    static bool isIntegerType(DataType type) {
        return (type == TYPE_BOOL || type == TYPE_INT);
    }

    static bool isPrintType(DataType type)
    {
        return !(type == TYPE_FUNC_PTR || type == TYPE_UNKNOWN || type == TYPE_VOID);
    }

    static bool isLogicalOpr(Operator opr) {
        switch (opr) {
            case OP_AND:
            case OP_OR:
            case OP_NOT:
            case OP_G:
            case OP_GE:
            case OP_L:
            case OP_LE:
            case OP_EQ:
            case OP_NEQ:
                return true;
        }
        return false;
    }

    static bool isLvalueOpr(Operator opr) {
        switch (opr) {
            case OP_ASSIGN:
            case OP_INC:
            case OP_DEC:
                return true;
        }
        return false;
    }

    static std::string dtypeToStr(DataType type) {
        switch (type) {
            case TYPE_VOID:
                return "void";
            case TYPE_BOOL:
                return "bool";
            case TYPE_STRING:
                return "string";
            case TYPE_INT:
                return "int";
            case TYPE_DOUBLE:
                return "double";
            case TYPE_FUNC_PTR:
                return "pointer to function";
            case TYPE_ERROR:
                return "type error";
        }

        return "unknown";
    }
    static std::string oprToStr(Operator opr)
    {
        switch (opr)
        {
            case OP_ASSIGN:
                return "=";    
            case OP_ADD:
                return "+";           
            case OP_SUB:
            case OP_MIN:
                return "-";            
            case OP_MUL:
                return "*";               
            case OP_DIV:
                return "/";              
            case OP_MOD:
                return "%";           
            case OP_INC:
                return "++";             
            case OP_DEC:
                return "--";              
            case OP_AND:
                return "&&";               
            case OP_OR:
                return "||";             
            case OP_NOT:
                return "!";            
            case OP_G:
                return ">";             
            case OP_GE:
                return ">=";            
            case OP_L:
                return "<";             
            case OP_LE:
                return "<=";              
            case OP_EQ:
                return "==";              
            case OP_NEQ:
                return "!=";
        }
        return "#";
    }
};

#endif
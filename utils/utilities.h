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

};

#endif
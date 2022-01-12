#ifndef CONSTS
#define CONSTS

enum Operator {
    OP_ASSIGN = 100,        // a = b
    OP_ADD,                 // a + b
    OP_SUB,                 // a - b
    OP_MIN,                 // -54
    OP_MUL,                 // a * b
    OP_DIV,                 // a / b
    OP_MOD,                 // a % b
    OP_INC,                 // a++
    OP_DEC,                 // a--
    OP_AND,                 // a && b
    OP_OR,                  // a || b
    OP_NOT,                 // !a
    OP_G,                  // a > b
    OP_GE,                 // a >= b
    OP_L,                  // a < b
    OP_LE,                 // a <= b
    OP_EQ,                 // a == b
    OP_NEQ,                 // a != b

    //
    // Quadruples instructions
    //
	OPR_PUSH,				// PUSH X, push to the stack
	OPR_POP, 				// POP A, pop last element from the stack
	OPR_JMP,				// JMP L1, unconditional jump
    OPR_JNZ,                // JNZ L1, jmp if the top of the stack is not zero and pops it.
	OPR_JZ, 				// JZ L1, jmp if the top of the stack is zero and pops it.
};

enum DataType {
    TYPE_VOID = 200,
    TYPE_BOOL,
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_FUNC_PTR,
    TYPE_ERROR,
    TYPE_UNKNOWN
};

enum ScopeType {
    SCOPE_BLOCK = 300,
    SCOPE_FUNCTION,
    SCOPE_LOOP,
    SCOPE_IF
};

enum LogLevel {
    LOG_ERROR,
    LOG_WARNING,
    LOG_NOTE
};

#endif
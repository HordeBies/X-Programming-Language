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
    OP_G,                   // a > b
    OP_GE,                  // a >= b
    OP_L,                   // a < b
    OP_LE,                  // a <= b
    OP_EQ,                  // a == b
    OP_NEQ,                 // a != b

    //
    // Quadruples instructions
    //
    OP_LOAD,
	OP_PUSH,				// PUSH X, push to the stack
	OP_POP, 				// POP A, pop last element from the stack
	OP_JMP,				    // JMP L1, unconditional jump to L1
    OP_JNZ,                 // JNZ X L1, jmp to L1 if X is not zero.
	OP_JZ, 				    // JZ X L1, jmp to L1 if X is zero.
    OP_JL,                  // JL L1 , jmp to L1 and send return point to $ra
    OP_JB,                  // JB , jmp back using $ra provided from stack
    OP_PR,                  // PR STREAM X , syscall print with char stream and identifier value
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
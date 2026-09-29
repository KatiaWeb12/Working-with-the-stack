#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include "../universal-features/error.h"

#ifdef STACK_DEBUG
    #define ON_DBG(...) __VA_ARGS__
#else
    #define ON_DBG(...)
#endif


// Types

typedef double stackElem_t;

struct debugLog_t {
    char* time;
    const char* file;
    const char* function;
    int line;
    ErrorCode error;
};

struct debugStack_t {
    const char* name;
    const char* file;
    const char* function;
    size_t line;
};

struct stack_t {
    stackElem_t* data;
    size_t size;
    size_t capacity;

    ON_DBG(
        debugStack_t debugInfo;
    )
};

// Prototypes

ErrorCode stackOK(const struct stack_t* stk, const char* function, const int line);
ErrorCode printIntoLogFile(const char* filename, struct debugLog_t* debugLogInfo);
ErrorCode stackInit(
    struct stack_t* stk,
    size_t capacity
    ON_DBG(, debugStack_t debugInfo)
);
ErrorCode resizeStack(struct stack_t* stk, int direction);
ErrorCode stackPush(struct stack_t* stk, stackElem_t value);
ErrorCode stackPop(struct stack_t* stk, stackElem_t* value);
ErrorCode stackDestroy(struct stack_t* stk);
ErrorCode deletingLogFile(const char* fileName);
char* getOperationTime(char* timeBuffer, size_t size);

ON_DBG(
    ErrorCode stackDump(const stack_t* stk);
)


// Macro

#define ASSERT_OK(stk, function)            \
    if (stackOK((stk), (function), __LINE__) != 0) {      \
            ON_DBG(stackDump((stk));)       \
            abort();                        \
    }                                       \

#ifdef STACK_DEBUG
    #define STACK_DUMP(stk) stackDump((stk))
#else
    #define STACK_DUMP(stk) (void)
#endif

#ifdef STACK_DEBUG
#define STACK_INIT(stk, capacity)                 \
    stackInit(                                  \
        (stk),                                   \
        (capacity),                              \
        (debugStack_t) {                          \
            #stk,                                \
            __FILE__,                            \
            __func__,                            \
            __LINE__                             \
        }                                        \
    )

#else

#define STACK_INIT(stk, capacity) \
    stackInit((stk), (capacity))

#endif


#endif

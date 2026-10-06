#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include "../universal-features/error.h"

#define CANARY_DEBUG 1
#define HASH_DEBUG 2
#define DUMP_DEBUG 4

// #define STACK_DEBUG (CANARY_DEBUG | DUMP_DEBUG | HASH_DEBUG)

#ifndef STACK_DEBUG
    #define STACK_DEBUG 0
#endif

#if (STACK_DEBUG & CANARY_DEBUG)
    #define ON_CANARY(...) __VA_ARGS__
#else
    #define ON_CANARY(...)
#endif

#if (STACK_DEBUG & HASH_DEBUG)
    #define ON_HASH(...) __VA_ARGS__
#else
    #define ON_HASH(...)
#endif

#if (STACK_DEBUG & DUMP_DEBUG)
    #define ON_DBG(...) __VA_ARGS__
#else
    #define ON_DBG(...)
#endif


// Types

typedef double stackElem_t;
typedef unsigned long long canary_t;
typedef unsigned long long hash_t;

typedef struct debugLog_t {
    char* time;
    const char* file;
    const char* function;
    int line;
    ErrorCode error;
} debugLog_t;

typedef struct debugStack_t {
    const char* name;
    const char* file;
    const char* function;
    size_t line;
} debugStack_t;


typedef struct stack_t {

    ON_CANARY(
        canary_t leftStructCanary;
    )

    stackElem_t* data;
    size_t size;
    size_t capacity;

    ON_HASH(
        hash_t dataHash;
        hash_t structHash;
    )

    ON_DBG(
        debugStack_t debugInfo;
    )

    ON_CANARY(
        canary_t rightStructCanary;
    )
} stack_t;

// Prototypes

ErrorCode stackOK(struct stack_t* stk, const char* function, const void* functionPtr, const int line);
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
ErrorCode updateLogFile(const char* filename);
size_t getDataOffset();
size_t alignment(size_t value, size_t alignment);
size_t getAllocationSize(size_t capacity);
size_t getRightCanaryOffset(size_t capacity);
canary_t* getLeftDataCanary(const struct stack_t* stk);
canary_t* getRightDataCanary(const struct stack_t* stk);
void printStackElem(size_t index, stackElem_t value);
ErrorCode cleanData(struct stack_t* stk);
ErrorCode endLogIteration(const char* filename);

ON_DBG(
    ErrorCode stackDump(const stack_t* stk);
)
ON_HASH(
    hash_t hashing(const void* data, size_t size);
    hash_t calculateDataHash(const struct stack_t* stk);
    hash_t calculateStructHash(stack_t* stk);
    void updateStackHash(struct stack_t* stk);
)

// Macro

#define ASSERT_OK(stk, function)                                                                \
    if (stackOK((stk), (#function), reinterpret_cast<const void*>(function), __LINE__) != 0) {  \
            ON_DBG(stackDump((stk));)                                                           \
            endLogIteration("stack.log");                                                       \
            abort();                                                                            \
    }                                                                                           \


#define LOG_STRUCT_FORMAT(debugLogInfoPtr, stk, function, line)                 \
    char timeString[32] = {};                                                   \
    debugLogInfoPtr->time = getOperationTime(timeString, 32);                   \
    debugLogInfoPtr->file = __FILE__;                                           \
    debugLogInfoPtr->function = function;                                       \
    debugLogInfoPtr->line = line;                                               \


#if STACK_DEBUG >= 3
    #define STACK_DUMP(stk) stackDump((stk))
#else
    #define STACK_DUMP(stk) (void)
#endif

#if STACK_DEBUG >= 3
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

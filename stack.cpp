#include <TXLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <time.h>

#include "stack.h"
#include "../universal-features/error.h"
#include "../universal-features/colors.h"


#define STACK_POISON NAN
#define PRINT_ELEM_FORMAT "%f"

const char* LOG_FILE_NAME = "stack.log";


ErrorCode stackInit(
    stack_t* stk,
    size_t capacity
    ON_DBG(, debugStack_t debugInfo)
)
{

    stk->data = (stackElem_t*) calloc(
        capacity,
        sizeof(stackElem_t)
    );

    if (stk->data == NULL) {
        return ERR_OUT_OF_MEMORY;
    }

    stk->size = 0;
    stk->capacity = capacity;

    for (size_t i = 0; i < capacity; i++) {
        stk->data[i] = STACK_POISON;
    }

    ON_DBG(
        stk->debugInfo = debugInfo;
    )
    ASSERT_OK(stk, __func__);

    STACK_DUMP(stk);

    return ERR_OK;
}


ErrorCode stackPush(stack_t* stk, stackElem_t value)
{

    ASSERT_OK(stk, __func__);

    if ((stk->size) >= (stk->capacity)) {
        resizeStack(stk, 1);
    }

    stk->data[stk->size] = value;
    (stk->size)++;

    ASSERT_OK(stk, __func__);

    STACK_DUMP(stk);

    return ERR_OK;
}

ErrorCode stackPop(stack_t* stk, stackElem_t* value)
{

    ASSERT_OK(stk, __func__);

    if (stk->size == 0) {

        struct debugLog_t debugLogInfo = {};

        char timeString[32] = {};

        debugLogInfo.time = getOperationTime(timeString, 32);
        debugLogInfo.file = __FILE__;
        debugLogInfo.function = __func__;
        debugLogInfo.line = __LINE__;
        debugLogInfo.error = ERR_OUT_OF_BOUNDS;

        printIntoLogFile(LOG_FILE_NAME, &debugLogInfo);
        return ERR_OUT_OF_BOUNDS;
    }

    stk->size--;

    *value = stk->data[stk->size];

    stk->data[stk->size] = STACK_POISON;

    if((stk->size)+1 <= (stk->capacity / 2)){
        resizeStack(stk, -1);
    }

    ASSERT_OK(stk, __func__);

    STACK_DUMP(stk);

    return ERR_OK;
};

ErrorCode resizeStack(stack_t* stk, int direction){

    ASSERT_OK(stk, __func__);

    size_t oldCapacity = stk->capacity;
    size_t newCapacity = 0;

    if(direction > 0){
        newCapacity = stk->capacity * 2;
    }
    else{
        newCapacity = stk->capacity / 2;
    }

    if(newCapacity == 0) return ERR_OUT_OF_BOUNDS;

    stackElem_t* newData = (stackElem_t*) realloc(stk->data, newCapacity * sizeof(stackElem_t));

    if (newData == NULL) {
        return ERR_OUT_OF_MEMORY;
    }

    stk->data = newData;
    stk->capacity = newCapacity;

    if (newCapacity > oldCapacity) {
        for (size_t i = oldCapacity; i < newCapacity; i++) {
            stk->data[i] = STACK_POISON;
        }
    }

    ASSERT_OK(stk, __func__);

    printf(COLOR_RED "%s" COLOR_RESET, "Stack resize\n");

    return ERR_OK;
}


ErrorCode stackDestroy(stack_t* stk)
{
    ASSERT_OK(stk, __func__);

    free(stk->data);

    stk->data = NULL;
    stk->size = 0;
    stk->capacity = 0;

    printf("Stack was destroyed");
    return ERR_OK;
};

ErrorCode stackOK(const stack_t* stk, const char* function, const int line){

    struct debugLog_t debugLogInfo = {};

    char timeString[32] = {};
    debugLogInfo.time = getOperationTime(timeString, 32);

    debugLogInfo.file = __FILE__;
    debugLogInfo.function = function;
    debugLogInfo.line = line;

    if (stk == NULL) {
        debugLogInfo.error = ERR_INVALID_ARGUMENT;
        printIntoLogFile(LOG_FILE_NAME, &debugLogInfo);
        return ERR_INVALID_ARGUMENT;
    }

    if(_msize(stk->data) != (stk->capacity) * sizeof(stackElem_t)){
        debugLogInfo.error = ERR_INVALID_DATA;
        printIntoLogFile(LOG_FILE_NAME, &debugLogInfo);
        return ERR_INVALID_DATA;
    }

    if(stk->size > stk->capacity){
        debugLogInfo.error = ERR_OVERFLOW;
        printIntoLogFile(LOG_FILE_NAME, &debugLogInfo);
        return ERR_OVERFLOW;
    }

    return ERR_OK;
}

char* getOperationTime(char* timeBuffer, size_t size){

    time_t currentTime = time(NULL);
    struct tm* localTime = localtime(&currentTime);

    snprintf(timeBuffer, size,
             "%02d.%02d.%04d %02d:%02d:%02d",
             localTime->tm_mday,
             localTime->tm_mon + 1,
             localTime->tm_year + 1900,
             localTime->tm_hour,
             localTime->tm_min,
             localTime->tm_sec);

    return timeBuffer;
}

ErrorCode printIntoLogFile(const char* filename, struct debugLog_t* debugLogInfo) {

    FILE* logFile = fopen(filename, "a");

    if(logFile == NULL){
        printf("File '%s' does not exists", filename);
        return ERR_FILE_NOT_FOUND;
    }

    fprintf(logFile,
            "[%s] Function '%s' was completed with Error %d in file '%s' in line %d \n",
            debugLogInfo->time,
            debugLogInfo->function,
            debugLogInfo->error,
            debugLogInfo->file,
            debugLogInfo->line);

    fclose(logFile);

    return ERR_OK;
}

ErrorCode deletingLogFile(const char* fileName){
    int codeOfDeleting = unlink(fileName);
    if(codeOfDeleting < 0){
        printf("File '%s' does not exists", fileName);
        return ERR_FILE_NOT_FOUND;
    }
    return ERR_OK;
}


#ifdef STACK_DEBUG

ErrorCode stackDump(const stack_t* stk)
{

    if (stk == NULL) {
        return ERR_INVALID_ARGUMENT;
    }

    printf("stack_t '%s'[%p] created by %s() at %s:%u\n",
            stk->debugInfo.name,
            stk,
            stk->debugInfo.function,
            stk->debugInfo.file,
            stk->debugInfo.line);

    printf("{\n");
    printf("capacity = %u\n", stk->capacity);
    printf("size = %u    \n", stk->size);
    printf("data[%p]{    \n", stk->data);
    for(size_t i = 0; i < stk->capacity; i++){

        if(i < stk->size){
            printf("*[%u] = " PRINT_ELEM_FORMAT "\n", i, (stk->data)[i]);
        }
        else{
            printf("*[%u] = " PRINT_ELEM_FORMAT "(POISON)\n", i, (stk->data)[i]);
        }

    }
    printf("}\n");

    getchar();

    return ERR_OK;
}

#endif






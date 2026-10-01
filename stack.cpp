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
const int RESIZE_UP_INDICATOR = 1;
const int RESIZE_DOWN_INDICATOR = -1;
const canary_t LEFT_STRUCT_CANARY = 0x8BADF00D;
const canary_t RIGHT_STRUCT_CANARY = 0x50FFC001;
const canary_t LEFT_DATA_CANARY = 0xABADBABE;
const canary_t RIGHT_DATA_CANARY = 0xB16B00B5;

ErrorCode stackInit(stack_t* stk, size_t capacity ON_DBG(, debugStack_t debugInfo))
{

    size_t allocationSize = getAllocationSize(capacity);

    char* newMemory = (char*) calloc(1, allocationSize);

    if (newMemory == NULL) {
        return ERR_OUT_OF_MEMORY;
    }

    stk->data = (stackElem_t*)(newMemory + getDataOffset());

    stk->leftStructCanary  = LEFT_STRUCT_CANARY;
    stk->rightStructCanary = RIGHT_STRUCT_CANARY;

    stk->size = 0;
    stk->capacity = capacity;

    *getLeftDataCanary(stk)  = LEFT_DATA_CANARY;
    *getRightDataCanary(stk) = RIGHT_DATA_CANARY;

    for (size_t i = 0; i < capacity; i++) {
        stk->data[i] = STACK_POISON;
    }

    ON_DBG(
        stk->debugInfo = debugInfo;
    )

    ASSERT_OK(stk, stackInit);

    STACK_DUMP(stk);

    return ERR_OK;
}


ErrorCode stackPush(stack_t* stk, stackElem_t value)
{

    ASSERT_OK(stk, stackPush);

    if ((stk->size) >= (stk->capacity)) {
        resizeStack(stk, RESIZE_UP_INDICATOR);
    }

    stk->data[stk->size] = value;
    (stk->size)++;

    ASSERT_OK(stk, stackPush);

    STACK_DUMP(stk);

    return ERR_OK;
}

ErrorCode stackPop(stack_t* stk, stackElem_t* value)
{

    ASSERT_OK(stk, stackPop);


    stk->size--;

    *value = stk->data[stk->size];

    stk->data[stk->size] = STACK_POISON;

    if((stk->size)+1 <= (stk->capacity / 2)){
        resizeStack(stk, RESIZE_DOWN_INDICATOR);
    }

    printf("Last value" PRINT_ELEM_FORMAT "was deleted from stack\n\n", *value);

    ASSERT_OK(stk, stackPop);

    STACK_DUMP(stk);

    return ERR_OK;
};

ErrorCode resizeStack(stack_t* stk, int direction){

    ASSERT_OK(stk, resizeStack);

    size_t oldCapacity = stk->capacity;
    size_t newCapacity = 0;

    if (direction > 0){
        newCapacity = oldCapacity * 2;
    }
    else{
        if(oldCapacity <= 3){
            return ERR_OK;
        }
        newCapacity = oldCapacity / 2;
    }

    if(newCapacity == 0) return ERR_OUT_OF_BOUNDS;

    size_t newAllocationSize = getAllocationSize(newCapacity);

    char* oldData = (char*) stk->data - getDataOffset();
    char* newData = (char*) realloc(oldData, newAllocationSize);

    if (newData == NULL) {
        return ERR_OUT_OF_MEMORY;
    }

    stk->data = (stackElem_t*)(newData + getDataOffset());
    stk->capacity = newCapacity;

    if (newCapacity > oldCapacity) {

        for (size_t i = oldCapacity; i < newCapacity; i++) {
            stk->data[i] = STACK_POISON;
        }

    }

    *getLeftDataCanary(stk)  = LEFT_DATA_CANARY;
    *getRightDataCanary(stk) = RIGHT_DATA_CANARY;

    ASSERT_OK(stk, resizeStack);

    printf(COLOR_RED "%s" COLOR_RESET, "\nStack resize\n");

    return ERR_OK;
}

ErrorCode cleanData(struct stack_t* stk){

    ASSERT_OK(stk, cleanData);

    for(size_t i = 0; i < stk->capacity; i++){
        (stk->data)[i] = STACK_POISON;
    }

    ASSERT_OK(stk, cleanData);

    return ERR_OK;

}

ErrorCode stackDestroy(stack_t* stk)
{

    ASSERT_OK(stk, stackDestroy);

    cleanData(stk);

    char* dataMemory = (char*)stk->data - getDataOffset();

    free(dataMemory);

    stk->data = NULL;
    stk->size = 0;
    stk->capacity = 0;

    printf("COLOR_RED %s COLOR_RESET", "Stack was destroyed\n");

    return ERR_OK;

};

ErrorCode stackOK(const stack_t* stk, const char* function, const void* functionPtr, const int line){

    debugLog_t debugLogInfo = {};

    LOG_STRUCT_FORMAT(stk, function, line);

    if (stk == NULL) {

        debugLogInfo.error = ERR_INVALID_ARGUMENT;
        printIntoLogFile(LOG_FILE_NAME, &debugLogInfo);

        return ERR_INVALID_ARGUMENT;
    }

    if(functionPtr == stackPop && stk->size == 0){

        debugLogInfo.error = ERR_OUT_OF_BOUNDS;
        printIntoLogFile(LOG_FILE_NAME, &debugLogInfo);

        return ERR_OUT_OF_BOUNDS;
    }

    if ((stk->leftStructCanary != LEFT_STRUCT_CANARY) || (stk->rightStructCanary != RIGHT_STRUCT_CANARY)) {

        debugLogInfo.error = ERR_INVALID_DATA;
        printIntoLogFile(LOG_FILE_NAME, &debugLogInfo);

        return ERR_INVALID_DATA;
    }

    if ((*getLeftDataCanary(stk) != LEFT_DATA_CANARY) || (*getRightDataCanary(stk) != RIGHT_DATA_CANARY)) {

        debugLogInfo.error = ERR_INVALID_DATA;
        printIntoLogFile(LOG_FILE_NAME, &debugLogInfo);

        return ERR_INVALID_DATA;
    }

    if(_msize((char*)stk->data - getDataOffset()) != getAllocationSize(stk->capacity)){

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

ErrorCode updateLogFile(const char* filename){

    FILE* logFile = fopen(filename, "a");

    if(logFile == NULL){
        printf("File '%s' does not exists", filename);
        return ERR_FILE_NOT_FOUND;
    }

    fprintf(logFile, "\n");

    fclose(logFile);

    return ERR_OK;
}

ErrorCode endLogIteration(const char* filename){

    FILE* logFile = fopen(filename, "a");

    if(logFile == NULL){
        printf("File '%s' does not exists", filename);
        return ERR_FILE_NOT_FOUND;
    }

    char timeString[32] = "";
    getOperationTime(timeString, 32);
    fprintf(logFile, "[%s] Program ended", timeString);

    fclose(logFile);

    return ERR_OK;
}

size_t getDataOffset()
{
    return alignment(sizeof(canary_t), alignof(stackElem_t));
}

size_t alignment(size_t value, size_t alignment)
{
    return value + alignment - 1;
}

size_t getRightCanaryOffset(size_t capacity)
{
    size_t stackEnd = capacity * sizeof(stackElem_t) + getDataOffset();

    return alignment(stackEnd, alignof(canary_t));
}

canary_t* getLeftDataCanary(const stack_t* stk)
{

    char* leftDataCanary = (char*)stk->data - getDataOffset();

    return (canary_t*)leftDataCanary;
}

canary_t* getRightDataCanary(const stack_t* stk)
{

    char* rightDataCanary = (char*)stk->data - getDataOffset();

    return (canary_t*)(rightDataCanary + getRightCanaryOffset(stk->capacity));
}

size_t getAllocationSize(size_t capacity)
{
    return getRightCanaryOffset(capacity) + sizeof(canary_t);
}

void printStackElem(size_t index, stackElem_t value){

    if(isnan(value)){
        printf("\t\t*[%u] = " COLOR_MAGENTA "%s(POISON)" COLOR_RESET "\n", index, "NaN");
    }
    else{
        printf("\t\t*[%u] = " PRINT_ELEM_FORMAT "\n", index, value);
    }

    return;
}

#ifdef STACK_DEBUG

ErrorCode stackDump(const stack_t* stk)
{

    if (stk == NULL) {
        return ERR_INVALID_ARGUMENT;
    }

    printf("\nstack_t '%s'[%p] created by %s() at %s:%u\n",
            stk->debugInfo.name,
            stk,
            stk->debugInfo.function,
            stk->debugInfo.file,
            stk->debugInfo.line);

    printf("{\n");

    printf("\tleftStructCanary = %I64X\n", stk->leftStructCanary);
    printf("\tleftDataCanary   = %I64X\n\n", *getLeftDataCanary(stk));

    printf("\tcapacity = %u\n", stk->capacity);
    printf("\tsize = %u    \n", stk->size);
    printf("\tdata[%p]{    \n", stk->data);

    for(size_t i = 0; i < stk->capacity; i++){
        printStackElem(i, (stk->data)[i]);
    }

    printf("\t}\n\n");

    printf("\trightDataCanary   = %I64X\n", *getRightDataCanary(stk));
    printf("\trightStructCanary = %I64X\n", stk->rightStructCanary);

    printf("}\n");

    getchar();

    return ERR_OK;
}


#endif






#include <stdio.h>
#include "../universal-features/error.h"
#include "stack.cpp"
#include <math.h>

ErrorCode correctTest();
ErrorCode sizeMoreThanCapacityTest();
ErrorCode badDataCanaryTest();
ErrorCode badTryToPopEmptyStackTest();

int main(){

    updateLogFile("stack.log");

    correctTest();

    endLogIteration("stack.log");

}

ErrorCode correctTest(){

    struct stack_t stk1 = {};

    ErrorCode error = STACK_INIT(&stk1, 3);
    if(error){
        printf("The function STACK_INIT was completed with an error code: %d\n", error);
        return error;
    }

    error = stackPush(&stk1, 7.2);
    if(error){
        printf("The function stackPush was completed with an error code: %d\n", error);
        return error;
    }

    error = stackPush(&stk1, 3.5);
    if(error){
        printf("The function stackPush was completed with an error code: %d\n", error);
        return error;
    }

    error = stackPush(&stk1, 9.8);
    if(error){
        printf("The function stackPush was completed with an error code: %d\n", error);
        return error;
    }

    error = stackPush(&stk1, 8.0);
    if(error){
        printf("The function stackPush was completed with an error code: %d\n", error);
        return error;
    }

    stackElem_t lastValue = NAN;

    error = stackPop(&stk1, &lastValue);
    if(error){
        printf("The function stackPop was completed with an error code: %d\n", error);
        return error;
    }

    error = stackPop(&stk1, &lastValue);
    if(error){
        printf("The function stackPop was completed with an error code: %d\n", error);
        return error;
    }

    error = stackPop(&stk1, &lastValue);
    if(error){
        printf("The function stackPop was completed with an error code: %d\n", error);
        return error;
    }


    return ERR_OK;

}

ErrorCode sizeMoreThanCapacityTest(){

    struct stack_t stk1 = {};


    ErrorCode error = STACK_INIT(&stk1, 3);
    if(error){
        printf("The function STACK_INIT was completed with an error code: %d\n", error);
        return error;
    }

    error = stackPush(&stk1, 7.2);
    if(error){
        printf("The function stackPush was completed with an error code: %d\n", error);
        return error;
    }

    stk1.size = 4;

    error = stackPush(&stk1, 3.5);
    if(error){
        printf("The function stackPush was completed with an error code: %d\n", error);
        return error;
    }

    error = stackPush(&stk1, 9.8);
    if(error){
        printf("The function stackPush was completed with an error code: %d\n", error);
        return error;
    }

    error = stackPush(&stk1, 8.0);
    if(error){
        printf("The function stackPush was completed with an error code: %d\n", error);
        return error;
    }

    stackElem_t lastValue = NAN;

    error = stackPop(&stk1, &lastValue);
    if(error){
        printf("The function stackPop was completed with an error code: %d\n", error);
        return error;
    }

    error = stackPop(&stk1, &lastValue);
    if(error){
        printf("The function stackPop was completed with an error code: %d\n", error);
        return error;
    }

    error = stackPop(&stk1, &lastValue);
    if(error){
        printf("The function stackPop was completed with an error code: %d\n", error);
        return error;
    }


    return ERR_OK;

}

ErrorCode badDataCanaryTest(){

    struct stack_t stk1 = {};

    ErrorCode error = STACK_INIT(&stk1, 3);
    if(error){
        printf("The function stackInit was completed with an error code: %d\n", error);
        return error;
    }

    if (error) {
        return error;
    }

    error = stackPush(&stk1, 7.2);
    if(error){
        printf("The function stackPush was completed with an error code: %d\n", error);
        return error;
    }

    *getRightDataCanary(&stk1) = 8.5;

    error = stackPush(&stk1, 3.5);
    if(error){
        printf("The function stackPush was completed with an error code: %d\n", error);
        return error;
    }

    return ERR_OK;
}

ErrorCode badTryToPopEmptyStackTest(){

    struct stack_t stk1 = {};

    ErrorCode error = STACK_INIT(&stk1, 3);
    if(error){
        printf("The function STACK_INIT was completed with an error code: %d\n", error);
        return error;
    }

    error = stackPush(&stk1, 7.2);
    if(error){
        printf("The function stackPush was completed with an error code: %d\n", error);
        return error;
    }

    stackElem_t lastValue = NAN;

    error = stackPop(&stk1, &lastValue);
    if(error){
        printf("The function stackPop was completed with an error code: %d\n", error);
        return error;
    }

    error = stackPop(&stk1, &lastValue);
    if(error){
        printf("The function stackPop was completed with an error code: %d\n", error);
        return error;
    }

    error = stackPop(&stk1, &lastValue);
    if(error){
        printf("The function stackPop was completed with an error code: %d\n", error);
        return error;
    }

    return ERR_OK;
}

#include <stdio.h>
#include "../universal-features/error.h"
#include "stack.h"
#include <math.h>

int main(){

    deletingLogFile("stack.log");

    stack_t stk1 = {};

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
    printf("Last value %f was deleted from stack\n", lastValue);

    error = stackPop(&stk1, &lastValue);
    if(error){
        printf("The function stackPop was completed with an error code: %d\n", error);
        return error;
    }
    printf("Last value %f was deleted from stack\n", lastValue);

    error = stackPop(&stk1, &lastValue);
    if(error){
        printf("The function stackPop was completed with an error code: %d\n", error);
        return error;
    }
    printf("Last value %f was deleted from stack\n", lastValue);

    error = stackPop(&stk1, &lastValue);
    if(error){
        printf("The function stackPщз was completed with an error code: %d\n", error);
        return error;
    }
    printf("Last value %f was deleted from stack\n", lastValue);

    error = stackPop(&stk1, &lastValue);
    if(error){
        printf("The function stackPop was completed with an error code: %d\n", error);
        return error;
    }
    printf("Last value %f was deleted from stack\n", lastValue);

    error = stackPop(&stk1, &lastValue);
    if(error){
        printf("The function stackPop was completed with an error code: %d\n", error);
        return error;
    }
    printf("Last value %f was deleted from stack\n", lastValue);

    error = stackPop(&stk1, &lastValue);
    if(error){
        printf("The function stackPop was completed with an error code: %d\n", error);
        return error;
    }
    printf("Last value %f was deleted from stack\n", lastValue);

    error = stackPop(&stk1, &lastValue);
    if(error){
        printf("The function stackPop was completed with an error code: %d\n", error);
        return error;
    }
    printf("Last value %f was deleted from stack\n", lastValue);

    error = stackDestroy(&stk1);
    if(error){
        printf("The function stackDestroy was completed with an error code: %d\n", error);
        return error;
    }

    return ERR_OK;
}

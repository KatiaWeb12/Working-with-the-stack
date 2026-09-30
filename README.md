# STACK

## Author: Ekaterina Kraeva

## Description
The project involves working with stacks that have multiple layers of protection. All stack‑related functions have been created and are working properly.

## Compilation and launch
It’s better to launch it from the console.
```bash
gcc stack.h -o stack.h.exe
g++ -Wall -Wextra -DSTACK_DEBUG version1.cpp stack.cpp -o stack.exe

./stack.exe
```

## Project structure

```text
stack-project/
├── stack.h
├── stack.cpp
├── version1.cpp
├── stack.log
├── README.md
```

## Files

### stack.h
The file contains:
- Prototypes of the main structures
- Function prototypes
- Macro

### stack.cpp
The file contains implementation of functions

### version1.cpp
The file contains a call to the main functions for working with the stack.

## Functions

### stackInit
Stack initialization. Filling the structure with basic data. Array elements with a stack are created as STACK_POISON.
```bash
stk->leftCanary  = STACK_CANARY;
stk->rightCanary = STACK_CANARY;

stk->size = 0;
stk->capacity = capacity;

for (size_t i = 0; i < capacity; i++) {
    stk->data[i] = STACK_POISON;
}
```
### stackPush


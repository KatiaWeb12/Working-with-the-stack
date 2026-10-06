# STACK

## Author: Ekaterina Kraeva
## Mentor: Pavel Taracanov

## Description
The project involves working with stacks that have multiple layers of protection. All stack‑related functions have been created and are working properly.

## Compilation and launch
It’s better to launch it from the console.
```bash
gcc stack.h -o stack.h.exe
g++ -Wall -Wextra -DSTACK_DEBUG=3 version1.cpp stack.cpp -o stack.exe

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

## DEBUG CODES
- STACK_DEBUG = 0 — without protection
- STACK_DEBUG = 1 — canaries
- STACK_DEBUG = 2 — hashes
- STACK_DEBUG = 3 — dump/debugInfo

```
#define CANARY_DEBUG 1
#define HASH_DEBUG 2
#define DUMP_DEBUG 4
```

```
#define STACK_DEBUG (CANARY_DEBUG | DUMP_DEBUG | HASH_DEBUG)

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
```

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
### Stack verification

```bash
stackOK();
```

The verifier checks:
- stack pointer validity
- structure canaries
- data canaries
- allocated memory size using _msize
- size <= capacity

All errors are displayed in detail in the log file:

```bash
[30.09.2026 21:54:35] Function 'stackPush' was completed with Error 8 in file 'stack.cpp' in line 64
```

### stackDump
The function outputs a complete printout of all stack data to the console.
```bash
stack_t &stk1[0061FEA0] created by main() at version1cpp:22
{
    leftStructCanary = 8BADF00D
    leftDataCanary   = ABADBABE

    capacity = 3
    size = 2

    data[01234567]{
        *[0] = 7.200000
        *[1] = 3.500000
        *[2] = NaN(POISON)
    }

    rightDataCanary   = B16B00B5
    rightStructCanary = 50FFC001
}
```

## Tests
Several test variations have been implemented.
```bash
ErrorCode correctTest();
ErrorCode sizeMoreThanCapacityTest();
ErrorCode badDataCanaryTest();
```
## Canary Protection

The stack structure is protected by canaries. The array of values itself also contains canaries.

```bash
typedef unsigned long long canary_t;
```
There are functions for alignment:
```bash
size_t getDataOffset();
size_t alignment(size_t value, size_t alignment);
size_t getAllocationSize(size_t capacity);
size_t getRightCanaryOffset(size_t capacity);
canary_t* getLeftDataCanary(const struct stack_t* stk);
canary_t* getRightDataCanary(const struct stack_t* stk);
```
## Hash Protection

Hashing is used to detect changes in stack data.
In debug mode, the `stack_t' structure contains two hashes:

- `dataHash` — hash of the dynamic array `data`;
- `structHash` is the hash of the entire `stack_t` structure.

Before calculating the hash of the structure, the `structHash` field is temporarily
reset so that the old hash value does not affect the calculation of the new one.

Before performing the `stackOK()` operations recalculates the hashes and compares them with the stored values.

## Changed features - Dedinsky Advises

- write to the end of the log that the program is completed +
- In stackOk(), write information to the structure using a macro +
- Compare function pointers or strcmp ([worse]) +


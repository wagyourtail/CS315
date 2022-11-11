#ifndef STACK
#define STACK

#include "LinkedList.h"

#include <stdbool.h>

typedef LinkedList Stack;

int stack_push(Stack* stack, void* val);
void* stack_pop(Stack* stack);
bool stack_isEmpty(Stack* stack);

#include "Stack.c"

#endif
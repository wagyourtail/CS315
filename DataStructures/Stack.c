#include "Stack.h"
#include "Status.h"

#include <stdlib.h>
#include <stdio.h>


int stack_push(Stack* stack, void* val) {
    struct ListNode* node = malloc(sizeof(struct ListNode));
    if (node == NULL) {
        return FAILURE;
    } else {
        node->val = val;
        node->next = *stack;
        *stack = node;
        return SUCCESS;
    }
}

void* stack_pop(Stack* stack) {
    if (*stack == NULL) {
        return NULL;
    } else {
        struct ListNode* node = *stack;
        *stack = node->next;
        void* val = node->val;
        free(node);
        return val;
    }
}

bool stack_isEmpty(Stack* stack) {
    return *stack == NULL;
}
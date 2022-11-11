#include "Queue.h"
#include "Status.h"

#include <stdlib.h>
#include <stdio.h>


int queue_push(Queue* queue, void* val) {
    if (*queue == NULL) {
        *queue = malloc(sizeof(struct ListNode));
        if (*queue == NULL) {
            fputs("mmalloc failed, queue_push failed", stderr);
            return FAILURE;
        }
        (*queue)->val = val;
        (*queue)->next = *queue;
        return SUCCESS;
    } else {
        struct ListNode *node = malloc(sizeof(struct ListNode));
        if (node == NULL) {
            fputs("mmalloc failed, queue_push failed", stderr);
            return FAILURE;
        }
        node->val = val;
        node->next = (*queue)->next;
        (*queue)->next = node;
        *queue = node;
        return SUCCESS;
    }
}

void* queue_pop(Queue* queue) {
    if (*queue == NULL) {
        return NULL;
    }
    struct ListNode *node = (*queue)->next;
    if (node == *queue) {
        *queue = NULL;
    } else {
        (*queue)->next = node->next;
    }
    void* val = node->val;
    free(node);
    return val;
}
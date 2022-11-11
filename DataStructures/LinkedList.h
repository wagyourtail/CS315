#ifndef LINKED_LIST
#define LINKED_LIST

#include <stdbool.h>

struct ListNode {
    void* val;
    struct ListNode* next;
};

typedef struct ListNode* LinkedList;

#include "LinkedList.c"

#endif
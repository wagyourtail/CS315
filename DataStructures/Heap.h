#ifndef HEAP
#define HEAP

struct Heap {
    void** data;
    int size;
    int capacity;
};

typedef struct Heap Heap;

Heap* heap_create(int capacity);
int heap_resize(Heap* this, int capacity);
int heap_insert(Heap* this, void* val, int(* cmp)(void* a, void* b));
void* heap_remove(Heap* this, int(* cmp)(void*, void*));

void heap_debugPrint(Heap* this, void(* print)(void* val));

#include "Heap.c"

#endif
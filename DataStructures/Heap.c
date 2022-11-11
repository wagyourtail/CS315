#include "Heap.h"
#include "Status.h"

#include <stdlib.h>
#include <stdio.h>


Heap* heap_create(int capacity) {
    Heap* heap = malloc(sizeof(Heap));
    heap->data = malloc(sizeof(void*) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
    if (heap->data == NULL) {
        return NULL;
    }
    return heap;
}

int heap_insert(Heap* this, void* val, int(* cmp)(void* a, void* b)) {
    if (this->size == this->capacity) {
        if (heap_resize(this, this->capacity << 1) == FAILURE) {
            return FAILURE;
        }
    }
    this->data[this->size] = val;
    int i = this->size++;
    while (i > 0) {
        int parent = i >> 1;
        if (cmp(this->data[i], this->data[parent]) < 0) {
            void* temp = this->data[i];
            this->data[i] = this->data[parent];
            this->data[parent] = temp;
            i = parent;
        } else {
            break;
        }
    }
    return SUCCESS;
}

void* heap_remove(Heap* this, int(* cmp)(void*, void*)) {
    if (this->size == 0) {
        return NULL;
    }
    void* val = this->data[0];
    this->data[0] = this->data[--this->size];
    int i = 0;
    while (true) {
        int left = (i << 1) + 1;
        int right = (i << 1) + 2;
        if (left >= this->size) {
            break;
        } else if (right >= this->size) {
            if (cmp(this->data[i], this->data[left]) > 0) {
                void* temp = this->data[i];
                this->data[i] = this->data[left];
                this->data[left] = temp;
            }
            break;
        } else {
            int min = cmp(this->data[left], this->data[right]) < 0 ? left : right;
            if (cmp(this->data[i], this->data[min]) > 0) {
                void* temp = this->data[i];
                this->data[i] = this->data[min];
                this->data[min] = temp;
                i = min;
            } else {
                break;
            }
        }
    }
    return val;
}

int heap_resize(Heap* this, int capacity) {
    void** newData = realloc(this->data, capacity * sizeof(void*));
    if (newData == NULL) {
        return FAILURE;
    } else {
        this->capacity = capacity;
        this->data = newData;
    }
}

void heap_debugPrint(Heap* this, void(* print)(void* val)) {
    printf("\n\n[debug] Heap size: %d Heap capacity: %d Heap data:\n", this->size, this->capacity);

    for (int i = 0; i < this->size; i++) {
        print(this->data[i]);
    }

    printf("\n\n");
}
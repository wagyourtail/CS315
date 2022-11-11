#ifndef QUEUE
#define QUEUE

#include "LinkedList.h"

typedef LinkedList Queue;

int queue_push(Queue* queue, void* val);
void* queue_pop(Queue* queue);

#include "Queue.c"

#endif
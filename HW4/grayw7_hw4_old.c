#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <unistd.h>

struct BinaryTree {
    void* val;
    struct BinaryTree* left;
    struct BinaryTree* right;
};

typedef struct BinaryTree BinaryTree;

typedef BinaryTree* BinaryTreeHead;

bool btInsert(BinaryTreeHead* this, void* val, int(* cmp)(void* a, void* b));
void* btRetrieve(BinaryTreeHead* this, void* val, int(* cmp)(void* a, void* b));
void btPreOrderTraverse(BinaryTreeHead* this, void(* visitor)(void* val));
void btInOrderTraverse(BinaryTreeHead* this, void(* visitor)(void* val));


struct ListNode {
    void* val;
    struct ListNode* next;
};

typedef struct ListNode* Stack;

void stackPush(Stack* this, void* val);
void* stackPop(Stack* this);
bool stackIsEmpty(Stack* this);

struct Heap {
    void** data;
    int size;
    int capacity;
};

typedef struct Heap Heap;

Heap heapCreate(int capacity);
Heap heapResize(Heap* this, int capacity);
bool heapInsert(Heap* this, void* val, int(* cmp)(void*, void*));
void* heapRemove(Heap* this, int(* cmp)(void*, void*));

struct HuffmanNode {
    int freq;
    char c;
};

int huffmanNodeCmp(const struct BinaryTree* a, const struct BinaryTree* b);
void printHuffmanNode(const struct HuffmanNode* c);

void debugPrintHeap(Heap* this);

int main() {
    Heap heap = heapCreate(16);

    // construct heap
    do {
        printf("Enter a character (enter a '$' to quit entering characters): ");
        char c;
        if (scanf(" %c", &c) == 1) {
            if (c == '$') {
                break;
            }
            struct HuffmanNode* node = malloc(sizeof(struct HuffmanNode));
            node->c = c;
            BinaryTree* nodeTree = malloc(sizeof(BinaryTree));
            nodeTree->val = node;
            nodeTree->left = NULL;
            nodeTree->right = NULL;
            printf("Enter the frequency of the character: ");
            if (scanf(" %d", &node->freq)) {
                heapInsert(&heap, nodeTree, (int (*)(void*, void*)) huffmanNodeCmp);
            } else {
                free(node);
                free(nodeTree);
                printf("Invalid frequency. Try again.");
            }
        } else {
            printf("Invalid character. Try again.");
        }
    } while (true);

    debugPrintHeap(&heap);

    // heap to huffman tree
    while (heap.size > 2) {
        BinaryTree* left = heapRemove(&heap, (int (*)(void*, void*)) huffmanNodeCmp);
        BinaryTree* right = heapRemove(&heap, (int (*)(void*, void*)) huffmanNodeCmp);
        struct HuffmanNode* node = malloc(sizeof(struct HuffmanNode));
        node->c = '\0';
        node->freq = ((struct HuffmanNode*) left->val)->freq + ((struct HuffmanNode*) right->val)->freq;
        BinaryTree* nodeTree = malloc(sizeof(BinaryTree));
        nodeTree->val = node;
        nodeTree->left = left;
        nodeTree->right = right;

        debugPrintHeap(&heap);

        heapInsert(&heap, nodeTree, (int (*)(void*, void*)) huffmanNodeCmp);

        debugPrintHeap(&heap);
    }

    BinaryTree* root = heapRemove(&heap, (int (*)(void*, void*)) huffmanNodeCmp);

    // print in-order and pre-order
    printf("In-order: \n\n");
    btInOrderTraverse(&root, (void (*)(void*)) printHuffmanNode);
    printf(" \n\nPre-order: \n\n");
    btPreOrderTraverse(&root, (void (*)(void*)) printHuffmanNode);
    printf(" \n\n");


    return 0;
}

void debugPrintHeap(Heap* this) {
     printf("\n\n[debug] Heap size: %d Heap capacity: %d Heap data:\n", this->size, this->capacity);

    for (int i = 1; i < this->size; i++) {
        printHuffmanNode(((BinaryTree*) this->data[i])->val);
    }

    printf("\n\n");
}

int huffmanNodeCmp(const struct BinaryTree* a, const struct BinaryTree* b) {
    return ((struct HuffmanNode*) a->val)->freq - ((struct HuffmanNode*) b->val)->freq;
}

void printHuffmanNode(const struct HuffmanNode* c) {
    if (c->c == '\0') {
        printf("(internal) %5d\n", c->freq);
    } else {
        printf("     %c     %5d\n", c->c, c->freq);
    }
}

bool btInsert(BinaryTreeHead* this, void* val, int(* cmp)(void*, void*)) {
    if (*this == NULL) {
        *this = malloc(sizeof(BinaryTree));
        (*this)->val = val;
        (*this)->left = NULL;
        (*this)->right = NULL;
        return true;
    } else {
        BinaryTree* node = *this;
        while (true) {
            int cmpResult = cmp(val, node->val);
            if (cmpResult == 0) {
                return false;
            } else if (cmpResult < 0) {
                if (node->left == NULL) {
                    node->left = malloc(sizeof(BinaryTree));
                    node->left->val = val;
                    node->left->left = NULL;
                    node->left->right = NULL;
                    return true;
                } else {
                    node = node->left;
                }
            } else {
                if (node->right == NULL) {
                    node->right = malloc(sizeof(BinaryTree));
                    node->right->val = val;
                    node->right->left = NULL;
                    node->right->right = NULL;
                    return true;
                } else {
                    node = node->right;
                }
            }
        }
    }
}

void* btRetrieve(BinaryTreeHead* this, void* val, int(* cmp)(void*, void*)) {
    if (*this == NULL) {
        return NULL;
    } else {
        BinaryTree* node = *this;
        while (true) {
            int cmpResult = cmp(val, node->val);
            if (cmpResult == 0) {
                return node->val;
            } else if (cmpResult < 0) {
                if (node->left == NULL) {
                    return NULL;
                } else {
                    node = node->left;
                }
            } else {
                if (node->right == NULL) {
                    return NULL;
                } else {
                    node = node->right;
                }
            }
        }
    }
}

void btPreOrderTraverse(BinaryTreeHead* this, void(* visitor)(void*)) {
    if (*this == NULL) {
        return;
    } else {
        Stack stack = {NULL};
        BinaryTree* node = *this;
        while (true) {
            visitor(node->val);
            if (node->left != NULL) {
                stackPush(&stack, node);
                node = node->left;
            } else if (node->right != NULL) {
                node = node->right;
            } else if (!stackIsEmpty(&stack)) {
                node = stackPop(&stack);
                node = node->right;
                while (node == NULL && !stackIsEmpty(&stack)) {
                    node = stackPop(&stack);
                    node = node->right;
                }
                if (node == NULL) {
                    return;
                }
            } else {
                return;
            }
        }
    }
}

void btInOrderTraverse(BinaryTreeHead* this, void(* visitor)(void*)) {
    if (*this == NULL) {
        return;
    } else {
        Stack stack = {NULL};
        BinaryTree* node = *this;
        while (true) {
            if (node->left != NULL) {
                stackPush(&stack, node);
                node = node->left;
            } else {
                visitor(node->val);
                if (node->right != NULL) {
                    node = node->right;
                } else if (!stackIsEmpty(&stack)) {
                    node = stackPop(&stack);
                    visitor(node->val);
                    node = node->right;
                    while (node == NULL && !stackIsEmpty(&stack)) {
                        node = stackPop(&stack);
                        visitor(node->val);
                        node = node->right;
                    }
                    if (node == NULL) {
                        return;
                    }
                } else {
                    return;
                }
            }
        }
    }
}

void stackPush(Stack* this, void* val) {
    struct ListNode* item = malloc(sizeof(struct ListNode));
    item->val = val;
    item->next = *this;
    *this = item;
}

void* stackPop(Stack* this) {
    if (*this == NULL) {
        return NULL;
    } else {
        struct ListNode* item = *this;
        *this = item->next;
        void* val = item->val;
        free(item);
        return val;
    }
}

bool stackIsEmpty(Stack* this) {
    return *this == NULL;
}

Heap heapCreate(int capacity) {
    Heap heap;
    heap.data = malloc(sizeof(void*) * capacity);
    heap.size = 1;
    heap.capacity = capacity;
    if (heap.data == NULL) {
        printf("Error: Out of memory.");
        _exit(1);
    }
    return heap;
}

Heap heapResize(Heap* this, int capacity) {
    this->data = realloc(this->data, sizeof(void*) * capacity);
    this->capacity = capacity;
    if (this->data == NULL) {
        printf("Error: Out of memory.");
        _exit(1);
    }
    return *this;
}

bool heapInsert(Heap* this, void* val, int(* cmp)(void*, void*)) {
    if (this->size == this->capacity) {
        heapResize(this, this->capacity << 1);
    }
    this->data[this->size] = val;
    int i = this->size;
    while (i > 1 && cmp(this->data[i], this->data[i >> 1]) < 0) {
        void* temp = this->data[i];
        this->data[i] = this->data[i >> 1];
        this->data[i >> 1] = temp;
        i >>= 1;
    }
    this->size++;
    return true;
}

void* heapRemove(Heap* this, int(* cmp)(void*, void*)) {
    if (this->size == 1) {
        return NULL;
    }
    void* val = this->data[1];
    this->data[1] = this->data[this->size - 1];
    this->size--;
    int i = 1;
    while (i <= this->size && cmp(this->data[i], this->data[i << 1]) > 0) {
        void* temp = this->data[i];
        this->data[i] = this->data[i << 1];
        this->data[i << 1] = temp;
        i <<= 1;
    }
    return val;
}
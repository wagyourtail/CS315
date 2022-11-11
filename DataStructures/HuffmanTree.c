#include "HuffmanTree.h"

int huffman_nodeCmp(HuffmanNode* a, HuffmanNode* b) {
    return ((struct HuffmanNodeVal*) a->val)->freq - ((struct HuffmanNodeVal*) b->val)->freq;
}

void huffman_printNode(HuffmanNode* val, void(* print)(void* item, int freq)) {
    huffman_printNodeVal(val->val, print);
}

void huffman_printNodeVal(struct HuffmanNodeVal* val, void(* print)(void* item, int freq)) {
    print(val->item, val->freq);
}

HuffmanBuilder* huffman_createBuilder(int initialCapacity) {
    return heap_create(initialCapacity);
}

int huffman_addItem(HuffmanBuilder* builder, int freq, void* item) {
    HuffmanNode* node = malloc(sizeof(HuffmanNode));
    struct HuffmanNodeVal* val = malloc(sizeof(struct HuffmanNodeVal));
    if (node == NULL || val == NULL) return FAILURE;
    node->val = val;
    val->item = item;
    val->freq = freq;
    node->right = NULL;
    node->left = NULL;
    int rv = heap_insert(builder, node, (int (*)(void*, void*)) huffman_nodeCmp);
    if (rv != SUCCESS) {
        free(val);
        free(node);
    }
    return rv;
}

HuffmanTree* huffman_buildTree(HuffmanBuilder* builder, void(* print)(HuffmanNode* node)) {
#ifdef DEBUG
    heap_debugPrint(builder, print);
#endif

    while (builder->size > 1) {
        HuffmanNode* left = heap_remove(builder, (int (*)(void*, void*)) huffman_nodeCmp);
        HuffmanNode* right = heap_remove(builder, (int (*)(void*, void*)) huffman_nodeCmp);
        HuffmanNode* combined = malloc(sizeof(HuffmanNode));
        struct HuffmanNodeVal* val = malloc(sizeof(struct HuffmanNodeVal));
        if (combined == NULL || val == NULL) {
            heap_insert(builder, left, (int (*)(void*, void*)) huffman_nodeCmp);
            heap_insert(builder, right, (int (*)(void*, void*)) huffman_nodeCmp);
            return NULL;
        }

        combined->val = val;
        val->item = NULL;
        val->freq = ((struct HuffmanNodeVal*) left->val)->freq + ((struct HuffmanNodeVal*) right->val)->freq;

#ifdef DEBUG
        heap_debugPrint(builder, print);
#endif
        combined->left = left;
        combined->right = right;

        heap_insert(builder, combined, (int (*)(void*, void*)) huffman_nodeCmp);

#ifdef DEBUG
        heap_debugPrint(builder, print);
#endif
    }

    return heap_remove(builder, (int (*)(void*, void*)) huffman_nodeCmp);
}
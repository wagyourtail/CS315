#include "HuffmanTree.h"

#include <string.h>

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

HuffmanTree* huffman_buildTree(HuffmanBuilder* builder, void(* debugPrint)(HuffmanNode* node)) {
#ifdef DEBUG
    heap_debugPrint(builder, debugPrint);
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
        heap_debugPrint(builder, debugPrint);
#endif
        combined->left = left;
        combined->right = right;

        heap_insert(builder, combined, (int (*)(void*, void*)) huffman_nodeCmp);

#ifdef DEBUG
        heap_debugPrint(builder, debugPrint);
#endif
    }

    HuffmanNode* head = heap_remove(builder, (int (*)(void*, void*)) huffman_nodeCmp);
    return head;
}

int huffman_traverseInternal(HuffmanNode* tree, char* code, void(* visitor)(void* item, char* code)) {
    if (tree->left == NULL && tree->right == NULL) {
        visitor(((struct HuffmanNodeVal*) tree->val)->item, code);
        return SUCCESS;
    } else {
        char* innerCode = malloc(strlen(code) + 2);
        if (innerCode == NULL) return FAILURE;
        strcpy(innerCode, code);
        if (tree->left != NULL) {
            strcat(innerCode, "0");
            int rv = huffman_traverseInternal(tree->left, innerCode, visitor);
            if (rv != SUCCESS) {
                free(innerCode);
                return rv;
            }
        }
        if (tree->right != NULL) {
            strcpy(innerCode, code);
            strcat(innerCode, "1");
            int rv = huffman_traverseInternal(tree->right, innerCode, visitor);
            if (rv != SUCCESS) {
                free(innerCode);
                return rv;
            }
        }
        free(innerCode);
    }
}

int huffman_printCode(HuffmanTree* tree, void(* visitor)(void* item, char* code)) {
    if (tree == NULL) return SUCCESS;
    return huffman_traverseInternal(tree, "", visitor);
}
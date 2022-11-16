#ifndef HUFFMAN_TREE
#define HUFFMAN_TREE

#include "BinaryTree.h"
#include "Heap.h"

struct HuffmanNodeVal {
    int freq;
    void* item;
};

typedef BinaryTree HuffmanNode;
typedef BinaryTree HuffmanTree;
typedef Heap HuffmanBuilder;

int huffman_nodeCmp(HuffmanNode* a, HuffmanNode* b);
void huffman_printNode(HuffmanNode* val, void(* print)(void* item, int freq));
void huffman_printNodeVal(struct HuffmanNodeVal* val, void(* print)(void* item, int freq));
HuffmanBuilder* huffman_createBuilder(int initialCapacity);
int huffman_addItem(HuffmanBuilder* builder, int freq, void* item);
HuffmanTree* huffman_buildTree(HuffmanBuilder* builder, void(* debugPrint)(HuffmanNode* node));
int huffman_printCode(HuffmanTree* tree, void(* visitor)(void* item, char* code));

#include "HuffmanTree.c"

#endif
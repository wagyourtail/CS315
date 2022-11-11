#ifndef BINARY_TREE
#define BINARY_TREE

#include <stdbool.h>

struct BinaryTree {
    void* val;
    struct BinaryTree* left;
    struct BinaryTree* right;
};

typedef struct BinaryTree BinaryTree;
typedef BinaryTree* BinaryTreeHead;

int binaryTree_insert(BinaryTreeHead* this, void* val, int(* cmp)(void* a, void* b));
bool binaryTree_contains(BinaryTreeHead* this, void* val, int(* cmp)(void* a, void* b));
void binaryTree_preOrderTraverse(BinaryTreeHead* this, void(* visitor)(void* value));
void binaryTree_inOrderTraverse(BinaryTreeHead* this, void(* visitor)(void* value));
void binaryTree_postOrderTraverse(BinaryTreeHead* this, void(* visitor)(void* value));

#include "BinaryTree.c"

#endif
#include "BinaryTree.h"
#include "Stack.h"
#include "Status.h"

#include <stdlib.h>


int binaryTree_insert(BinaryTreeHead* this, void* val, int(* cmp)(void* a, void* b)) {
    if (*this == NULL) {
        *this = malloc(sizeof(BinaryTree));
        if (*this == NULL) {
            return FAILURE;
        }
        (*this)->val = val;
        (*this)->left = NULL;
        (*this)->right = NULL;
        return SUCCESS;
    } else {
        BinaryTree* node = *this;
        while (true) {
            int cmpResult = cmp(val, node->val);
            if (cmpResult == 0) {
                return DUPLICATE;
            } else if (cmpResult < 0) {
                if (node->left == NULL) {
                    node->left = malloc(sizeof(BinaryTree));
                    if (node->left == NULL) {
                        return FAILURE;
                    }
                    node->left->val = val;
                    node->left->left = NULL;
                    node->left->right = NULL;
                    return SUCCESS;
                } else {
                    node = node->left;
                }
            } else {
                if (node->right == NULL) {
                    node->right = malloc(sizeof(BinaryTree));
                    if (node->right == NULL) {
                        return FAILURE;
                    }
                    node->right->val = val;
                    node->right->left = NULL;
                    node->right->right = NULL;
                    return SUCCESS;
                } else {
                    node = node->right;
                }
            }
        }
    }
}

bool binaryTree_contains(BinaryTreeHead* this, void* val, int(* cmp)(void* a, void* b)) {
    if (*this == NULL) {
        return false;
    } else {
        BinaryTree* node = *this;
        while (true) {
            int cmpResult = cmp(val, node->val);
            if (cmpResult == 0) {
                return true;
            } else if (cmpResult < 0) {
                if (node->left == NULL) {
                    return false;
                } else {
                    node = node->left;
                }
            } else {
                if (node->right == NULL) {
                    return false;
                } else {
                    node = node->right;
                }
            }
        }
    }
}

void binaryTree_preOrderTraverse(BinaryTreeHead* this, void(* visitor)(void* value)) {
    if (*this == NULL) {
        return;
    } else {
        Stack stack = {NULL};
        BinaryTree* node = *this;
        while (true) {
            visitor(node->val);
            if (node->left != NULL) {
                stack_push(&stack, node);
                node = node->left;
            } else if (node->right != NULL) {
                node = node->right;
            } else if (!stack_isEmpty(&stack)) {
                node = stack_pop(&stack);
                node = node->right;
                while (node == NULL && !stack_isEmpty(&stack)) {
                    node = stack_pop(&stack);
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

void binaryTree_inOrderTraverse(BinaryTreeHead* this, void(* visitor)(void* value)) {
    if (*this == NULL) {
        return;
    } else {
        Stack stack = {NULL};
        BinaryTree* node = *this;
        while (true) {
            if (node->left != NULL) {
                stack_push(&stack, node);
                node = node->left;
            } else {
                visitor(node->val);
                if (node->right != NULL) {
                    node = node->right;
                } else if (!stack_isEmpty(&stack)) {
                    node = stack_pop(&stack);
                    visitor(node->val);
                    node = node->right;
                    while (node == NULL && !stack_isEmpty(&stack)) {
                        node = stack_pop(&stack);
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

struct PostOrderTraverseStackItem {
    BinaryTree* node;
    bool visitedRight;
};

struct PostOrderTraverseStackItem* POTSItem(BinaryTree* node, bool visitedRight) {
    struct PostOrderTraverseStackItem* item = malloc(sizeof(struct PostOrderTraverseStackItem));
    item->node = node;
    item->visitedRight = visitedRight;
    return item;
}

void binaryTree_postOrderTraverse(BinaryTreeHead* this, void(* visitor)(void* value)) {
    if (*this == NULL) {
        return;
    } else {
        Stack stack = {NULL};
        struct PostOrderTraverseStackItem* item;
        BinaryTree* node = *this;
        while (true) {
            if (node->left != NULL) {
                item = POTSItem(node, node->right == NULL);
                stack_push(&stack, item);
                node = node->left;
            } else if (node->right != NULL) {
                item = POTSItem(node, true);
                stack_push(&stack, item);
                node = node->right;
            } else {
                visitor(node->val);
                if (!stack_isEmpty(&stack)) {
                    item = stack_pop(&stack);
                    while (item->visitedRight && !stack_isEmpty(&stack)) {
                        visitor(item->node->val);
                        free(item);
                        item = stack_pop(&stack);
                    }
                    if (item->visitedRight) {
                        visitor(item->node->val);
                        free(item);
                        return;
                    }
                    node = item->node->right;
                    item->visitedRight = true;
                    stack_push(&stack, item);
                } else {
                    return;
                }
            }
        }
    }
}
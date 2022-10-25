#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>

struct BinaryTree {
    void* val;
    struct BinaryTree* left;
    struct BinaryTree* right;
};

typedef struct BinaryTree BinaryTree;

typedef BinaryTree* BinaryTreeHead;

bool btInsert(BinaryTreeHead* this, void* val, int(* cmp)(void*, void*));
bool btContains(BinaryTreeHead* this, void* val, int(* cmp)(void*, void*));
void btPreOrderTraverse(BinaryTreeHead* this, void(* visitor)(void*));
void btInOrderTraverse(BinaryTreeHead* this, void(* visitor)(void*));

struct PostOrderTraverseStackItem {
    BinaryTree* node;
    bool visitedRight;
};

void btPostOrderTraverse(BinaryTreeHead* this, void(* visitor)(void*));

struct ListNode {
    void* val;
    struct ListNode* next;
};

typedef struct ListNode* Stack;

void stackPush(Stack* this, void* val);
void* stackPop(Stack* this);
bool stackIsEmpty(Stack* this);

int charCmp(const char* a, const char* b);
void printChar(const char* c);

int main() {
    BinaryTreeHead head = {NULL};
    do {
        printf("Enter Choice - (I)nsert, (F)ind, (Q)uit: ");
        char choice;
        scanf(" %c", &choice);
        switch (choice) {
            case 'i':
            case 'I': {
                printf("Enter character to insert: ");
                char* val = malloc(sizeof(char));
                int success = scanf(" %c", val);
                if (success == 1) {
                    if (!btInsert(&head, val, (int (*)(void*, void*)) charCmp)) {
                        puts("\x1b[31mInsert failed\x1b[0m");
                    }
                    puts("");
                    puts("Preorder:");
                    btPreOrderTraverse(&head, (void (*)(void*)) printChar);
                    puts("");
                    puts("Inorder:");
                    btInOrderTraverse(&head, (void (*)(void*)) printChar);
                    puts("");
                    puts("Postorder:");
                    btPostOrderTraverse(&head, (void (*)(void*)) printChar);
                    puts("\n");
                } else {
                    // print err
                    puts("\x1b[31mscanf failed, insert failed\x1b[0m");
                    free(val);
                }
                break;
            }
            case 'f':
            case 'F': {
                printf("Enter character to find: ");
                char* val = malloc(sizeof(char));
                int success = scanf(" %c", val);
                if (success == 1) {
                    if (btContains(&head, val, (int (*)(void*, void*)) charCmp)) {
                        puts("\x1b[32mFound\x1b[0m");
                    } else {
                        puts("\x1b[31mNot Found\x1b[0m");
                    }
                } else {
                    // print err
                    puts("\x1b[31mscanf failed, find failed\x1b[0m");
                    free(val);
                }
                break;
            }
            case 'q':
            case 'Q': {
                return 0;
            }
            default: {
                puts("\x1b[31mInvalid choice\x1b[0m");
            }
        }

    } while (1);
}

int charCmp(const char* a, const char* b) {
    return *a - *b;
}

void printChar(const char* c) {
    printf("%c ", *c);
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

bool btContains(BinaryTreeHead* this, void* val, int(* cmp)(void*, void*)) {
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

struct PostOrderTraverseStackItem* POTSItem(BinaryTree* node, bool visitedRight) {
    struct PostOrderTraverseStackItem* item = malloc(sizeof(struct PostOrderTraverseStackItem));
    item->node = node;
    item->visitedRight = visitedRight;
    return item;
}

void btPostOrderTraverse(BinaryTreeHead* this, void(* visitor)(void*)) {
    if (*this == NULL) {
        return;
    } else {
        Stack stack = {NULL};
        struct PostOrderTraverseStackItem* item;
        BinaryTree* node = *this;
        while (true) {
            if (node->left != NULL) {
                item = POTSItem(node, node->right == NULL);
                stackPush(&stack, item);
                node = node->left;
            } else if (node->right != NULL) {
                item = POTSItem(node, true);
                stackPush(&stack, item);
                node = node->right;
            } else {
                visitor(node->val);
                if (!stackIsEmpty(&stack)) {
                    item = stackPop(&stack);
                    while (item->visitedRight && !stackIsEmpty(&stack)) {
                        visitor(item->node->val);
                        free(item);
                        item = stackPop(&stack);
                    }
                    if (item->visitedRight) {
                        visitor(item->node->val);
                        free(item);
                        return;
                    }
                    node = item->node->right;
                    item->visitedRight = true;
                    stackPush(&stack, item);
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
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
void* btRetrieve(BinaryTreeHead* this, void* val, int(* cmp)(void*, void*));
void btPreOrderTraverse(BinaryTreeHead* this, void(* visitor)(void*));
void btInOrderTraverse(BinaryTreeHead* this, void(* visitor)(void*));

struct ListNode {
    void* val;
    struct ListNode* next;
};

typedef struct ListNode* Stack;

void stackPush(Stack* this, void* val);
void* stackPop(Stack* this);
bool stackIsEmpty(Stack* this);

struct CharStrPair {
    char key;
    char* value;
};

int pairCmp(const struct CharStrPair* a, const struct CharStrPair* b);
void printPairKey(const struct CharStrPair* c);

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
                char key;
                struct CharStrPair* pair = malloc(sizeof(struct CharStrPair));
                if (scanf(" %c", &key) == 1) {
                    pair->key = key;
                    if (!btInsert(&head, pair, (int (*)(void*, void*)) pairCmp)) {
                        puts("\x1b[31mInsert failed\x1b[0m");
                    } else {
                        printf("Enter string of up to 10 characters for '%c's data: ", key);
                        pair->value = malloc(sizeof(char) * 11);
                        while (scanf(" %10s", pair->value) != 1) {
                            puts("\x1b[31mFailed to read string\x1b[0m");
                        }
                    }
                    puts("");
                    puts("Preorder:");
                    btPreOrderTraverse(&head, (void (*)(void*)) printPairKey);
                    puts("");
                    puts("Inorder:");
                    btInOrderTraverse(&head, (void (*)(void*)) printPairKey);
                    puts("\n");
                } else {
                    // print err
                    puts("\x1b[31mscanf failed, insert failed\x1b[0m");
                    free(pair);
                }
                break;
            }
            case 'f':
            case 'F': {
                printf("Enter character to find: ");
                char* val = malloc(sizeof(char));
                int success = scanf(" %c", val);
                if (success == 1) {
                    struct CharStrPair* pair = btRetrieve(&head, val, (int (*)(void*, void*)) pairCmp);
                    if (pair != NULL) {
                        printf("Found the string \"%s\" there\n\n", pair->value);
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

int pairCmp(const struct CharStrPair* a, const struct CharStrPair* b) {
    return a->key - b->key;
}

void printPairKey(const struct CharStrPair* c) {
    printf("%c ", c->key);
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
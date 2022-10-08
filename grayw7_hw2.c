#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>

struct ListNode {
    char val;
    struct ListNode *next;
};

// currently this just makes me not have to use ListNode**, but this is good if I needed to add something like length in the future
typedef struct CircularLinkedList {
    struct ListNode *head;
} CircularLinkedList;

typedef struct Optional {
    bool isPresent;
    char val;
} Optional;

void push(struct CircularLinkedList* this, char val) {
    struct ListNode *node = malloc(sizeof(struct ListNode));
    node->val = val;
    // detect if malloc failed
    if (node == NULL) {
        // print err
        puts("\x1b[31mmalloc failed, push failed\x1b[0m");
        return;
    }

    if (this->head == NULL) {
        this->head = node;
        node->next = node;
        return;
    }

    node->next = this->head->next;
    this->head->next = node;
    this->head = node;
}

Optional pop(struct CircularLinkedList* this) {
    Optional result = {false, 0};
    if (this->head == NULL) {
        // print err
        puts("\x1b[31mpop failed, list is empty\x1b[0m");
        return result;
    }

    struct ListNode *node = this->head->next;

    // len 0
    if (node == NULL) {
        puts("\x1b[31mpop failed, NULL FOUND ON NEXT, this is not supposed to be possible on a circular list\x1b[0m");
    }

    // len 1
    if (node == this->head) {
        this->head = NULL;
        result.val = node->val;
        free(node);
        result.isPresent = true;
        return result;
    }

    // len > 1
    this->head->next = node->next;
    result.val = node->val;
    free(node);
    result.isPresent = true;
    return result;
}

int main() {
    CircularLinkedList l = {NULL};
    char c;
    do {
        printf("Enter \"i\" to insert a new element, \"r\" to remove an element, \"q\" to quit: ");
        int success = scanf(" %c", &c);
        if (success != 1) {
            puts("\x1b[31mscanf failed\x1b[0m");
        } else {
            switch (c) {
                case 'i': {
                    char x;
                    printf("Enter character to be enqueued (inserted): ");
                    success = scanf(" %c", &x);
                    if (success != 1) {
                        puts("\x1b[31mscanf failed\x1b[0m");
                    } else {
                        push(&l, x);
                    }
                    break;
                }
                case 'r': {
                    Optional x;
                    x = pop(&l);
                    if (x.isPresent) {
                        printf("Removed character: %c\n", x.val);
                    }
                    break;
                }
                case 'q':
                default:
                    break;
            }

        }
    } while (c != 'q');
}


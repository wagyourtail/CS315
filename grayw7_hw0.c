#include <stdio.h>
#include <malloc.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

typedef struct LinkedList {
    struct ListNode *head;
} LinkedList;

void push(struct LinkedList* list, int val) {
    struct ListNode *node = malloc(sizeof(struct ListNode));
    // detect if malloc failed
    if (node == NULL) {
        // print err
        puts("\x1b[31mmalloc failed, push failed\x1b[0m");
        return;
    }
    node->val = val;
    node->next = list->head;
    list->head = node;
}

int pop(struct LinkedList* list, char* success) {
    if (list->head == NULL) {
        // print err
        puts("\x1b[31mpop failed, list is empty\x1b[0m");
        *success = 0;
        return 0;
    }
    struct ListNode *node = list->head;
    list->head = node->next;
    int val = node->val;
    free(node);
    *success = 1;
    return val;
}

int main() {
    LinkedList head = {NULL};
    puts("What do you want to do? (Push, pOp, Stop)");
    while (1) {
        char input;
        scanf("%c", &input);
        // if input is capital letter, convert it to lower case
        if (input >= 'A' && input <= 'Z') {
            input += 'a' - 'A';
        }
        switch (input) {
            case 'p': {
                puts("Enter a number to push");
                int val;
                int success = scanf("%d", &val); // NOLINT(cert-err34-c)
                if (success == 1) {
                    push(&head, val);
                } else {
                    // print err
                    puts("\x1b[31mscanf failed, push failed\x1b[0m");
                }
                break;
            }
            case 'o': {
                char success;
                int val;
                val = pop(&head, &success);
                if (success) {
                    printf("Popped %d\n", val);
                }
                break;
            }
            case 's':
                return 0;
            default:
                continue;
        }
        puts("What do you want to do? (Push, pOp, Stop)");
    }
}

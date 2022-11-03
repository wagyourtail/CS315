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
    node->val = val;
    // detect if malloc failed
    if (node == NULL) {
        // print err
        puts("\x1b[31mmalloc failed, push failed\x1b[0m");
        return;
    }

    if (list->head == NULL) {
        list->head = node;
        return;
    }

    if (list->head->val >= val) {
        node->next = list->head;
        list->head = node;
        return;
    }

    struct ListNode *current = list->head;
    while (current->next != NULL && current->next->val < val) {
        current = current->next;
    }
    node->next = current->next;
    current->next = node;
}

void pop(struct LinkedList* list, int x) {
    if (list->head == NULL) {
        // print err
        puts("\x1b[31mpop failed, list is empty\x1b[0m");
        return;
    }

    struct ListNode *node = list->head;

    if (list->head->val == x) {
        list->head = node->next;
        free(node);
        return;
    }

    while (node->next != NULL && node->next->val < x) {
        node = node->next;
    }
    if (node->next == NULL || node->next->val != x) {
        printf("\x1b[31mpop failed, list does not contain %d\x1b[0m\n", x);
        return;
    }

    struct ListNode *temp = node->next;
    node->next = temp->next;
    free(temp);
}

void printList(struct LinkedList* list) {
    struct ListNode *node = list->head;
    while (node != NULL) {
        printf("%d", node->val);
        node = node->next;
        if (node != NULL) {
            printf(" -> ");
        }
    }
    puts("");
}

int main() {
    int x;
    LinkedList l = {NULL};
    do {
        printf("Enter an integer: ");
        int success = scanf("%d", &x);
        if (success != 1) {
            puts("invalid input");
            x = 1;
            continue;
        }
        if (x != 0) {
            push(&l, x);
        }
    } while (x != 0);

    if (l.head == NULL) {
        puts("Your list is empty");
        return 0;
    }

    printf("Ok, here's your list in numeric order: ");
    printList(&l);

    do {
        printf("What do you want to delete? ");
        int success = scanf("%d", &x);
        if (success != 1) {
            puts("invalid input");
            x = 1;
            continue;
        }
        if (x != 0) {
            pop(&l, x);
            printf("Ok, here's your list in numeric order: ");
            printList(&l);
        }
    } while (x != 0);

    puts("Goodbye!");
}


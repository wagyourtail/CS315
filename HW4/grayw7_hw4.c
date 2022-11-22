#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

//#define DEBUG

#include "../DataStructures/HuffmanTree.h"

void printCharWithFreq(const char* c, int freq);
void printHuffmanNode(HuffmanNode* val);
void printHuffmanNodeVal(struct HuffmanNodeVal* val);

void printCharWithCode(const char* c, char* code);
void printHuffmanCode(HuffmanNode* node);

int main() {
    HuffmanBuilder* builder = huffman_createBuilder(16);
    if (builder == NULL) {
        return 1;
    }
    // construct heap
    do {
        printf("Enter a character (enter a '$' to quit entering characters): ");
        char* c = malloc(sizeof(char));
        if (scanf(" %c", c) == 1) {
            if (*c == '$') {
                break;
            }
            int freq;
            printf("Enter the frequency of the character: ");
            if (scanf(" %d", &freq)) {
                if (huffman_addItem(builder, freq, c) != 0) {
                    free(c);
                    printf("Error adding item to Huffman builder.");
                    return -1;
                }
            } else {
                printf("Invalid frequency. Try again.");
            }
        } else {
            printf("Invalid character. Try again.");
            free(c);
        }
    } while (true);

    HuffmanTree* tree = huffman_buildTree(builder, (void (*)(HuffmanNode*)) printHuffmanNode);


    // print in-order and pre-order
    printf("In-order: \n\n");
    binaryTree_inOrderTraverse(&tree, (void (*)(void*)) printHuffmanNodeVal);
    printf(" \n\nPre-order: \n\n");
    binaryTree_preOrderTraverse(&tree, (void (*)(void*)) printHuffmanNodeVal);
    printf(" \n\n");

    printf("Huffman codes: \n\n");
    printHuffmanCode(tree);

    return 0;
}

void printCharWithFreq(const char* c, int freq) {
    if (c == NULL) {
        printf("(internal) %5d\n", freq);
    } else {
        printf("     %c     %5d\n", *c, freq);
    }
}

void printHuffmanNode(HuffmanNode* val) {
    huffman_printNode(val, (void (*)(void*, int)) printCharWithFreq);
}

void printHuffmanNodeVal(struct HuffmanNodeVal* val) {
    huffman_printNodeVal(val, (void (*)(void*, int)) printCharWithFreq);
}

void printCharWithCode(const char* c, char* code) {
    printf("%c       %s\n", *c, code);
}

void printHuffmanCode(HuffmanNode* node) {
    huffman_printCode(node, (void (*)(void*, char*)) printCharWithCode);
}
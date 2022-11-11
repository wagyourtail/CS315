#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#define DEBUG

#include "../DataStructures/BinaryTree.h"
#include "../DataStructures/Heap.h"
#include "../DataStructures/HuffmanTree.h"

void printCharWithFreq(const char* c, int freq);
void printHuffmanNode(HuffmanNode* val);
void printHuffmanNodeVal(struct HuffmanNodeVal* val);

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
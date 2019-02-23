#include "priority_queue.h"
#include <stdio.h>
#include <stdlib.h>

#define SIZE_OF_ALPHABET 256

int encode(char *fileName) {
    int *frequencies = calloc(SIZE_OF_ALPHABET, sizeof(int));
    FILE *file = fopen(fileName, "r");
    if (!file) {
        return -1;
    }

    int frequency;
    while ((frequency = getc(file)) != EOF) {
        *(frequencies + frequency) += 1;
    }

    for (int i = 0; i < SIZE_OF_ALPHABET; i++) {
        frequency = *(frequencies + i);
        if (frequency) {
            struct queue_element *element = malloc(sizeof(struct queue_element));
            element->value = i;
            element->priority = frequency;
            push(element);
        }
    }

    fclose(file);
    return 0;
}
#include <stdio.h>
#include "haff.h"

int main(int argc, char** argv) {
    if (argc != 4) {
        printf("Define three params: 1. encode or decode (e/d) 2. input file 3.output file");
        return -1;
    }
    FILE *input = fopen(*(argv + 2), "r");
    if (!input) {
        printf("Can't open input file %s", *(argv + 1));
        return -1;
    }
    FILE *output = fopen(*(argv + 3), "w");
    if (!output) {
        printf("Can't create output file %s", *(argv + 2));
        return -1;
    }

    if (**(argv + 1) == 'c') {
        encode(input, output);
    } else if (**(argv + 1) == 'd') {
        decode(input, output);
    } else {
        printf("Illegal command %s", *argv);
        return -1;
    }

    fclose(input);
    fclose(output);
}